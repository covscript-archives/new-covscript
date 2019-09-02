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
private:
    class stor_base {
    public:
        stor_base()=default;
        stor_base(const stor_base&)=default;
        virtual ~stor_base()=default;
        virtual void suicide() const = 0;
        virtual stor_base* clone() const = 0;
        virtual type_support* extension() const = 0;
    };
    template<typename T>class stor_impl:public stor_base {
        T m_data;
    public:
        stor_impl()=default;
        virtual ~stor_impl()=default;
        stor_impl(const stor_impl&)=delete;
        stor_impl(const T& dat):m_data(dat) {}
        stor_impl(T&& dat) noexcept:m_data(std::move(dat)) {}
        void suicide() const override
        {
            this->~stor_base();
        }
        stor_base* clone() const override
        {
            return new stor_impl<T>(m_data);
        }
        type_support* extension() const override
        {
            return nullptr;
        }
    };
    class stor_proxy final {
    public:
        bool is_rvalue = false;
        stor_base* data_ptr = nullptr;
        std::atomic_int_fast64_t ref_count;
        stor_proxy():ref_count(0) {}
    };
    class stor_union final {
        static constexpr unsigned int static_stor_size=sizeof(stor_proxy);
    public:
        union {
            unsigned char static_stor[static_stor_size];
            stor_proxy* dynamic_stor;
        } data_stor;
        bool is_static=false;
        stor_base* data;
    };
};