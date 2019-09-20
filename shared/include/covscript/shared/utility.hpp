#pragma once
#include <functional>
#include <utility>
#include <vector>

namespace cs {
	namespace utility {
		class event_type final {
		public:
			using listener_type = std::function<bool(void *)>;

		private:
			std::vector<listener_type> m_listener;

		public:
			event_type() = default;

			event_type(const event_type &) = delete;

			explicit event_type(listener_type default_listener)
			{
				m_listener.emplace_back(std::move(default_listener));
			}

			void add_listener(listener_type listener)
			{
				m_listener.emplace_back(std::move(listener));
			}

			bool touch(void *arg)
			{
				for (auto &listener : m_listener)
					if (listener(arg))
						return true;
				return false;
			}
		};
	} // namespace utility
} // namespace cs

#ifdef COVSCRIPT_SDK_DEBUG
#include <cstdio>
#define cs_sdk_log_ev(msg) ::printf("[%s] In file %s, line %d: %s\n", __TIME__, __FILE__, __LINE__, msg);
#else
#define cs_sdk_log_ev(msg)
#endif