#pragma once

//#include <covscript/shared/exception.hpp>
#include <covscript/covscript.hpp>
#include <cstddef>

namespace vm {
    using vm_expression = cs::expression_t;
	enum class frame_type : std::uint8_t {
		_null,
        _data,
		_class,
		_scope,
		_fcall,
		_regeh,
		_callback
	};
	class vm_instance final {
		struct frame_header {
			frame_type type;
			std::size_t size;
			frame_header(frame_type _type, std::size_t _size) : type(_type), size(_size) {}
		};
		using byte_t = std::uint8_t;
		byte_t *stack_start = nullptr, *stack_limit = nullptr, *stack_pointer = nullptr;
		inline void push_header(frame_type type, std::size_t size)
		{
			::new (stack_pointer-sizeof(frame_header)) frame_header(type, size);
		}
        template<typename T, typename... ArgsT>
        inline void push_data(frame_type type, ArgsT&&...args)
        {
            ::new (stack_pointer-=sizeof(T)) T(std::forward<ArgsT>(args)...);
            push_header(type, sizeof(T));
        }
        using stmt_type = std::function<void()>;
        std::vector<stmt_type> m_assembly;
        cs::runtime_type cov3_rt;
        std::size_t m_pc = 0;
	public:
		vm_instance() = delete;
		vm_instance(std::size_t stack_size)
		{
			if (!(stack_start = reinterpret_cast<byte_t *>(::malloc(stack_size))))
				//cs::shared::throw_ex<cs::shared::runtime_error>("VM Instance Allocation Failed.");
                throw std::runtime_error("VM Instance Allocation Failed.");
			stack_limit = stack_start + stack_size;
			stack_pointer = stack_limit;
			::new (stack_pointer - sizeof(frame_header)) frame_header(frame_type::_null, 0);
		}
		void pop_stack(frame_type type = frame_type::_scope)
		{
			for (frame_header *header = reinterpret_cast<frame_header *>(stack_pointer - sizeof(frame_header)); header->type != type; stack_pointer += sizeof(frame_header) + header->size) {
				if (header->type == frame_type::_null)
					//COVSDK_RTERR("Pop empty stack.");
                    throw std::runtime_error("Pop empty stack.");
			}
		}
		/**
		 *	VM Instructions
		 *	Note: Every instruction execution will increase PC register except jumping instruction
		 */
		/**
		 *	Expression Instruction
		 *	Description: Evaluate a expression, which generally is a subtree of AST
		 *	Effect: 	 Depends on different expression
		 *				 (1) Calling Function:   push fcall frame
		 *				 (2) Throwing Exception: trigger stack unwinding
		 */
		void do_expr(const vm_expression &expr)
        {
            m_assembly.push_back([expr, this](){
                cov3_rt.parse_expr(expr.root());
            });
        }
		/**
		 *	Direct Jumping
		 *	Description: Modify the Program Counter register
		 *	Effect:		 Same as description
		 */
		void do_jmp(std::size_t offset)
        {
            m_assembly.push_back([offset, this](){
                m_pc = offset;
            });
        }
		/**
		 *	Conditional Jumping
		 *	Description: Modify the Program Counter register if condition is true
		 *	Effect:		 Save as description
		 */
		void do_jcond(const vm_expression& expr, std::size_t offset)
        {
            m_assembly.push_back([expr, offset, this](){
                if (cov3_rt.parse_expr(expr.root()).const_val<bool>())
                    m_pc = offset;
            });
        }
        void push_val(const vm_expression &expr)
        {
            m_assembly.push_back([expr, this](){
                push_data(frame_type::_data, cov3_rt.parse_expr(expr.root()));
            });
        }
		/**
		 *	Push a scope frame into stack
		 *	Description: Push a scope label into stack
		 *	Effect:		 Same as description
		 */
		void do_pushs();
		/**
		 *	Pop a scope frame from stack
		 *	Description: Trigger stack unwinding until scope label
		 *	Effect:		 Same as description
		 */
		void do_pops();
        void run()
        {
            for(;m_pc<m_assembly.size();++m_pc)
                m_assembly[m_pc]();
        }
	};
}