#pragma once
#include <covscript/shared/memory.hpp>
#include <typeindex>

namespace cs {
	class type_support;
	namespace runtime {
		class any;
	}
}

class cs::runtime::any final {
public:
	using typeid_t=std::type_index;
	using byte_t=unsigned char;
private:
	/*
	    数据存储基类
	    使用多态实现类型擦除的关键，即抽象出类型无关的接口
	    此类为接口类，或称之为纯虚基类
	*/
	class stor_base {
	public:
		// 默认构造函数，直接使用default版本
		stor_base()=default;
		// 复制构造函数，直接使用default版本
		stor_base(const stor_base&)=default;
		// 析构函数，声明为虚函数并使用default实现
		virtual ~stor_base()=default;
		// 自杀函数，释放占用的资源
		virtual void suicide(bool) = 0;
		// 克隆函数，在指定地址上构造一个当前对象的克隆
		virtual void clone(byte_t*) const = 0;
        // 克隆函数，构造一个当前对象的克隆并返回
		virtual stor_base* clone() const = 0;
		// 扩展函数，返回存储的数据类型对应的扩展对象
		virtual type_support* extension() const = 0;
	};
	/*
	    数据存储模版派生类
	    存储数据的具体实现
	    此类将利用模版类的特性自动生成所需要的派生类
	*/
	template<typename T>class stor_impl:public stor_base {
		// 实际存储的数据
		T m_data;
	public:
        // 分配器
        static allocator_type<stor_impl<T>, 96> allocator;
		// 默认构造函数，使用default实现
		stor_impl()=default;
		// 析构函数，使用default实现
		virtual ~stor_impl()=default;
		// 禁用复制构造函数
		stor_impl(const stor_impl&)=delete;
		// 自定义构造函数，构造存储的数据
		stor_impl(const T& dat):m_data(dat) {}
		// 以下三个函数为实现基类的virtual函数
		void suicide(bool is_static) override
		{
            if(is_static)
                this->~stor_impl();
            else
			    allocator.free(this);
		}
		void clone(byte_t *ptr) const override
		{
			::new (ptr) stor_impl<T>(m_data);
		}
        stor_base* clone() const override
        {
            return allocator.alloc(m_data);
        }
		type_support* extension() const override
		{
			// NOT IMPLEMENTED YET
			return nullptr;
		}
	};
	/*
	    实现小对象优化
	    减少内存分配瓶颈
	*/
    enum class stor_status {
        null, ptr, data
    };

	struct stor_union {
		// 触发小对象优化的阈值，需大于alignof(stor_impl<std::size_t>)
		static constexpr unsigned int static_stor_size=alignof(stor_impl<std::size_t>);
		unsigned char data_stor[static_stor_size];
		stor_status status=stor_status::null;
	};

	stor_union m_data;

    inline stor_base* get_handler()
    {
        return reinterpret_cast<stor_base*>(m_data.data_stor);
    }

    inline void recycle()
    {
        if(m_data.status!=stor_status::null)
            get_handler()->suicide(m_data.status==stor_status::data);
    }

	template<typename T>
	void store(T&& val)
	{
		if(sizeof(T)<=stor_union::static_stor_size) {
			::new (m_data.data_stor) stor_impl<T>(std::forward<T>(val));
			m_data.status=stor_status::data;
		}
		else {
			::new (m_data.data_stor) stor_base*(stor_impl<T>::allocator.alloc(std::forward<T>(val)));
			m_data.status=stor_status::ptr;
		}
	}

    void copy(const stor_union& data)
    {
        if(data.status!=stor_status::null){
            const stor_base* ptr=reinterpret_cast<const stor_base*>(data.data_stor);
            if(data.status==stor_status::ptr)
            {
                recycle();
                ::new (m_data.data_stor) stor_base*(ptr->clone());
            }else
                ptr->clone(m_data.data_stor);
            m_data.status=data.status;
        }
    }

public:
    any() {}

    template<typename T>
    any(const T &val) {
        store(val);
    }

    any(const any& val)
    {
        copy(val.m_data);
    }

    ~any()
    {
        recycle();
    }
};

template<typename T>
cs::runtime::allocator_type<cs::runtime::any::stor_impl<T>, 96> cs::runtime::any::stor_impl<T>::allocator;