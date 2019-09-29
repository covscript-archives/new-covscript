#pragma once

#include <covscript/shared/memory.hpp>
#include <covscript/shared/any.hpp>
#include <cstdint>

namespace cs {
	enum class instrument_types {
		// Expression
		expr,
		// Direct Jump
		jmp,
		// Conditional Jump
		jcond,
		// Push Stack Frame
		push,
		// Pop Stack Frame
		pop,
		// Call Function
		call,
		// Return Function
		ret,
		// Join Stackfull Coroutine
		joins,
		// Join Stackless Coroutine
		joinl,
		// Trigger Coroutine Dispatch
		yield,
		// Register Exception Handler
		regeh,
		// Throw Exception
		thex,
		// Variable Declaration
		vdecl,
		// Null
		null
	};
	class vm_instrument {
	};
	class vm_class {
	};
	enum class frame_type : std::uint8_t {
		_null,
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
			::new (--stack_pointer) frame_header(type, size);
		}

	public:
		vm_instance() = delete;
		vm_instance(std::size_t stack_size)
		{
			if (!(stack_start = reinterpret_cast<byte_t *>(::malloc(stack_size))))
				cs::shared::throw_ex<cs::shared::runtime_error>("VM Instance Allocation Failed.");
			stack_limit = stack_start + stack_size;
			stack_pointer = stack_limit;
			::new (stack_pointer - sizeof(frame_header)) frame_header(frame_type::_null, 0);
		}
		void pop_stack(frame_type type = frame_type::_scope)
		{
			for (frame_header *header = reinterpret_cast<frame_header *>(stack_pointer - sizeof(frame_header)); header->type != type; stack_pointer += sizeof(frame_header) + header->size) {
				if (header->type == frame_type::_null)
					COVSDK_RTERR("Pop empty stack.");
			}
		}
	};
} // namespace cs
