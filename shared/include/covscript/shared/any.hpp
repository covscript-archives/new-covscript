#pragma once
#include <covscript/shared/memory.hpp>
#include <typeindex>

namespace cs {
	class type_support;
	namespace runtime {
		class any;
	}
} // namespace cs

class cs::runtime::any final {
public:
	using typeid_t = std::type_index;
	using byte_t = unsigned char;
	static constexpr std::size_t default_allocate_buffer_size = 64;
	template<typename T> using default_allocator_provider=std::allocator<T>;
	template<typename T> using default_allocator=allocator_type<T, default_allocate_buffer_size, default_allocator_provider>;

private: 
	/*
	    数据存储基类
	    使用多态实现类型擦除的关键，即抽象出类型无关的接口
	    此类为接口类，或称之为纯虚基类
	*/
	class stor_base {
	public:
		// 默认构造函数，直接使用default版本
		stor_base() = default;
		// 复制构造函数，直接使用default版本
		stor_base(const stor_base &) = default;
		// 析构函数，声明为虚函数并使用default实现
		virtual ~stor_base() = default;
		// RTTI类型函数
		virtual std::type_index type() const noexcept = 0;
		// 自杀函数，释放占用的资源
		virtual void suicide(bool) = 0;
		// 克隆函数，在指定地址上构造一个当前对象的克隆
		virtual void clone(byte_t *) const = 0;
		// 克隆函数，构造一个当前对象的克隆并返回
		virtual stor_base *clone() const = 0;
		// 扩展函数，返回存储的数据类型对应的扩展对象
		virtual type_support *extension() const = 0;
	};
	/*
	    数据存储模版派生类
	    存储数据的具体实现
	    此类将利用模版类的特性自动生成所需要的派生类
	*/
	template <typename T>
	class stor_impl : public stor_base {
		// 实际存储的数据
		T m_data;

	public:
		// 分配器
		static default_allocator<stor_impl<T>> allocator;
		// 默认构造函数，使用default实现
		stor_impl() = default;
		// 析构函数，使用default实现
		virtual ~stor_impl() = default;
		// 禁用复制构造函数
		stor_impl(const stor_impl &) = delete;
		// 自定义构造函数，构造存储的数据
		stor_impl(const T &dat) : m_data(dat) {}
		// 以下五个函数为实现基类的virtual函数
		std::type_index type() const noexcept override
		{
			return typeid(T);
		}
		void suicide(bool is_static) override
		{
			if (is_static)
				this->~stor_impl();
			else
				allocator.free(this);
		}
		void clone(byte_t *ptr) const override
		{
			::new (ptr) stor_impl<T>(m_data);
		}
		stor_base *clone() const override
		{
			return allocator.alloc(m_data);
		}
		type_support *extension() const override
		{
			// NOT IMPLEMENTED YET
			return nullptr;
		}
		// 访问数据
		inline T &get_data()
		{
			return m_data;
		}
		inline const T &get_data() const
		{
			return m_data;
		}
	};
	/*
	    实现小对象优化
	    减少内存分配瓶颈
	*/
	enum class stor_status {
		null,
		ptr,
		data
	};

	struct stor_union {
		// 触发小对象优化的阈值，需大于std::alignment_of<stor_base *>::value
		static constexpr unsigned int static_stor_size = std::alignment_of<stor_base *>::value;
		union {
			unsigned char data[static_stor_size];
			stor_base *ptr;
		} impl;
		stor_status status = stor_status::null;
	};

	stor_union m_data;

	inline stor_base *get_handler()
	{
		switch (m_data.status) {
		case stor_status::null:
			return nullptr;
		case stor_status::data:
			return reinterpret_cast<stor_base *>(m_data.impl.data);
		case stor_status::ptr:
			return m_data.impl.ptr;
		}
	}

	inline const stor_base *get_handler() const
	{
		switch (m_data.status) {
		case stor_status::null:
			return nullptr;
		case stor_status::data:
			return reinterpret_cast<const stor_base *>(m_data.impl.data);
		case stor_status::ptr:
			return m_data.impl.ptr;
		}
	}

	inline void recycle()
	{
		if (m_data.status != stor_status::null){
			get_handler()->suicide(m_data.status == stor_status::data);
			cs_sdk_log_ev(m_data.status == stor_status::data?"Any Small Data Recycled.":"Any Normal Data Recycled.")
		}
	}

	template <typename T>
	void store(const T &val)
	{
		if (sizeof(T) <= stor_union::static_stor_size) {
			::new (m_data.impl.data) stor_impl<T>(val);
			m_data.status = stor_status::data;
			cs_sdk_log_ev("Any SDO Enabled.")
		}
		else {
			m_data.impl.ptr = stor_impl<T>::allocator.alloc(val);
			m_data.status = stor_status::ptr;
			cs_sdk_log_ev("Any SDO Disabled.")
		}
	}

	void copy(const any &data)
	{
		if (data.m_data.status != stor_status::null) {
			const stor_base *ptr = data.get_handler();
			if (data.m_data.status == stor_status::ptr) {
				recycle();
				m_data.impl.ptr = ptr->clone();
				cs_sdk_log_ev("Any Small Data Copied.")
			}
			else
			{
				ptr->clone(m_data.impl.data);
				cs_sdk_log_ev("Any Normal Data Copied.")
			}
			m_data.status = data.m_data.status;
		}
	}

public:
	any() {}

	template <typename T>
	any(const T &val)
	{
		store(val);
	}

	any(const any &val)
	{
		copy(val);
	}

	~any()
	{
		recycle();
	}

	template <typename T>
	T &get()
	{
		if (m_data.status == stor_status::null)
			throw_ex<runtime_error>("Access null any object.");
		if (get_handler()->type() != typeid(T))
			throw_ex<runtime_error>("Access wrong type of any.");
		return static_cast<stor_impl<T> *>(get_handler())->get_data();
	}

	template <typename T>
	const T &get() const
	{
		if (m_data.status == stor_status::null)
			throw_ex<runtime_error>("Access null any object.");
		if (get_handler()->type() != typeid(T))
			throw_ex<runtime_error>("Access wrong type of any.");
		return static_cast<const stor_impl<T> *>(get_handler())->get_data();
	}
};

template <typename T>
cs::runtime::any::default_allocator<cs::runtime::any::stor_impl<T>> cs::runtime::any::stor_impl<T>::allocator;