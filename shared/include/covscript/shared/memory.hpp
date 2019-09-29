#pragma once
#include <covscript/shared/exception.hpp>
#include <cstdlib>
#include <cstddef>

namespace cs {
	namespace shared {
		template <typename T>
		class stack_type final {
			using aligned_type = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;
			std::size_t m_size;
			aligned_type *m_data;
			T *m_start = nullptr, *m_current = nullptr;

		public:
			class iterator final {
				friend class stack_type;

				T *m_ptr = nullptr;

				explicit iterator(T *const ptr) : m_ptr(ptr) {}

			public:
				iterator() = delete;

				iterator(const iterator &) = default;

				iterator(iterator &&) noexcept = default;

				~iterator() = default;

				inline T &operator*() const noexcept
				{
					return *m_ptr;
				}

				inline T *operator->() const noexcept
				{
					return m_ptr;
				}

				inline iterator &operator++() noexcept
				{
					--m_ptr;
					return *this;
				}

				inline const iterator operator++(int) noexcept
				{
					return iterator(m_ptr--);
				}

				inline bool operator==(const iterator &it) const noexcept
				{
					return m_ptr == it.m_ptr;
				}

				inline bool operator!=(const iterator &it) const noexcept
				{
					return m_ptr != it.m_ptr;
				}
			};

			stack_type(std::size_t size = 512) : m_size(size)
			{
				m_data = ::malloc(size * sizeof(aligned_type));
				m_start = m_current = reinterpret_cast<T *>(m_data);
			}

			stack_type(const stack_type &) = delete;

			~stack_type()
			{
				while (m_current != m_start)
					(--m_current)->~T();
				::free(m_data);
			}

			inline bool empty() const
			{
				return m_current == m_start;
			}

			inline std::size_t size() const
			{
				return m_current - m_start;
			}

			inline bool full() const
			{
				return m_current - m_start == m_size;
			}

			inline T &top() const
			{
				if (empty())
					cs::shared::throw_ex<cs::shared::runtime_error>("Stack is empty.");
				return *(m_current - 1);
			}

			inline T &bottom() const
			{
				if (empty())
					cs::shared::throw_ex<cs::shared::runtime_error>("Stack is empty.");
				return *m_start;
			}

			inline T &at(std::size_t offset) const
			{
				if (offset >= size())
					cs::shared::throw_ex<cs::shared::runtime_error>("Stack out of range.");
				return *(m_current - offset - 1);
			}

			inline T &operator[](std::size_t offset) const
			{
				return *(m_current - offset - 1);
			}

			template <typename... ArgsT>
			inline void push(ArgsT &&... args)
			{
				if (full())
					cs::shared::throw_ex<cs::shared::runtime_error>("Stack overflow.");
				::new (m_current++) T(std::forward<ArgsT>(args)...);
			}

			inline T pop()
			{
				if (empty())
					cs::shared::throw_ex<cs::shared::runtime_error>("Stack is empty.");
				T data(std::move(*m_current));
				(m_current - 1)->~T();
				--m_current;
				return std::move(data);
			}

			inline void pop_no_return()
			{
				if (empty())
					throw cs::shared::runtime_error("Stack is empty.");
				(m_current - 1)->~T();
				--m_current;
			}

			iterator begin() const noexcept
			{
				return iterator(m_current - 1);
			}

			iterator end() const noexcept
			{
				return iterator(m_start - 1);
			}
		};

		template <typename T, std::size_t blck_size, template <typename> class allocator_t = std::allocator>
		class allocator_type final {
			T *mPool[blck_size];
			allocator_t<T> mAlloc;
			std::size_t mOffset = 0;

		public:
			allocator_type()
			{
				while (mOffset < 0.5 * blck_size)
					mPool[mOffset++] = mAlloc.allocate(1);
			}

			allocator_type(const allocator_type &) = delete;

			~allocator_type()
			{
				while (mOffset > 0)
					mAlloc.deallocate(mPool[--mOffset], 1);
			}

			template <typename... ArgsT>
			inline T *alloc(ArgsT &&... args)
			{
				T *ptr = nullptr;
				if (mOffset > 0)
					ptr = mPool[--mOffset];
				else
					ptr = mAlloc.allocate(1);
				mAlloc.construct(ptr, std::forward<ArgsT>(args)...);
				return ptr;
			}

			inline void free(T *ptr)
			{
				mAlloc.destroy(ptr);
				if (mOffset < blck_size)
					mPool[mOffset++] = ptr;
				else
					mAlloc.deallocate(ptr, 1);
			}
		};

		template <typename T, std::size_t blck_size, template <typename> class allocator_t = std::allocator>
		class plain_allocator_type final {
			allocator_t<T> mAlloc;

		public:
			plain_allocator_type() = default;

			plain_allocator_type(const plain_allocator_type &) = delete;

			~plain_allocator_type() = default;

			template <typename... ArgsT>
			inline T *alloc(ArgsT &&... args)
			{
				T *ptr = mAlloc.allocate(1);
				mAlloc.construct(ptr, std::forward<ArgsT>(args)...);
				return ptr;
			}

			inline void free(T *ptr)
			{
				mAlloc.destroy(ptr);
				mAlloc.deallocate(ptr, 1);
			}
		};
	} // namespace runtime
} // namespace cs
