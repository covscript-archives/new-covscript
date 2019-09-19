#pragma once
#include <typeindex>
#include <atomic>

namespace cs {
    class type_support;
    namespace impl {
        class any;
    }
}

class cs::impl::any final {
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
        virtual void suicide() const = 0;
    // 克隆函数，在指定地址上构造一个当前对象的克隆
        virtual void clone(byte_t*) const = 0;
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
        // 默认构造函数，使用default实现
        stor_impl()=default;
        // 析构函数，使用default实现
        virtual ~stor_impl()=default;
        // 禁用复制构造函数
        stor_impl(const stor_impl&)=delete;
        // 自定义构造函数，构造存储的数据
        stor_impl(const T& dat):m_data(dat) {}
        // 自定义构造函数，构造存储的数据，此处使用移动语义
        stor_impl(T&& dat) noexcept:m_data(std::move(dat)) {}
        // 以下三个函数为实现基类的virtual函数
        void suicide() const override
        {
            this->~stor_base();
        }
        void clone(byte_t *ptr) const override
        {
            ::new (ptr) stor_impl<T>(m_data);
        }
        type_support* extension() const override
        {
            // NOT IMPLEMENTED YET
            return nullptr;
        }
    };
/*
    数据存储中介，实现引用计数垃圾回收器
    此处使用原子引用计数器
*/
    class stor_proxy final {
    public:
        bool is_rvalue = false;
        stor_base* data_ptr = nullptr;
        std::atomic_int_fast64_t ref_count;
        stor_proxy():ref_count(0) {}
    };
/*
    实现小对象优化
    对于复制成本低的对象不再使用引用计数器持有
*/
    class stor_union final {
        // 触发小对象优化的阈值，需大于sizeof(stor_proxy*)
        static constexpr unsigned int static_stor_size=sizeof(stor_proxy);
    public:
        unsigned char data_stor[static_stor_size];
        bool is_static=false;
    };

    stor_union m_data;

    template<typename T>
    void store(T&& val)
    {
        if(sizeof(T)<stor_union::static_stor_size)
        {
            ::new (m_data.data_stor) T(std::forward<T>(val));
            m_data.is_static=true;
        }else{
            ::new (m_data.data_stor) stor_proxy*(new stor_proxy);
            m_data.is_static=false;
        }
    }

    void clone()
    {
        
    }
};