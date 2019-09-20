#pragma once
#include <functional>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>

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

		class timer final {
			static std::chrono::time_point<std::chrono::high_resolution_clock> m_timer;
		public:
			typedef std::size_t timer_t;
			enum class time_unit {
				nano_sec, micro_sec, milli_sec, second, minute
			};

			static void reset()
			{
				m_timer = std::chrono::high_resolution_clock::now();
			}

			static timer_t time(time_unit unit=time_unit::milli_sec)
			{
				switch (unit) {
				case time_unit::nano_sec:
					return std::chrono::duration_cast<std::chrono::nanoseconds>(
					           std::chrono::high_resolution_clock::now() - m_timer).count();
				case time_unit::micro_sec:
					return std::chrono::duration_cast<std::chrono::microseconds>(
					           std::chrono::high_resolution_clock::now() - m_timer).count();
				case time_unit::milli_sec:
					return std::chrono::duration_cast<std::chrono::milliseconds>(
					           std::chrono::high_resolution_clock::now() - m_timer).count();
				case time_unit::second:
					return std::chrono::duration_cast<std::chrono::seconds>(
					           std::chrono::high_resolution_clock::now() - m_timer).count();
				case time_unit::minute:
					return std::chrono::duration_cast<std::chrono::minutes>(
					           std::chrono::high_resolution_clock::now() - m_timer).count();
				}
				return 0;
			}

			static void delay(timer_t time, time_unit unit=time_unit::milli_sec)
			{
				switch (unit) {
				case time_unit::nano_sec:
					std::this_thread::sleep_for(std::chrono::nanoseconds(time));
					break;
				case time_unit::micro_sec:
					std::this_thread::sleep_for(std::chrono::microseconds(time));
					break;
				case time_unit::milli_sec:
					std::this_thread::sleep_for(std::chrono::milliseconds(time));
					break;
				case time_unit::second:
					std::this_thread::sleep_for(std::chrono::seconds(time));
					break;
				case time_unit::minute:
					std::this_thread::sleep_for(std::chrono::minutes(time));
					break;
				}
			}

			static timer_t measure(const std::function<void()> &func, time_unit unit=time_unit::milli_sec)
			{
				timer_t begin(0), end(0);
				begin = time(unit);
				func();
				end = time(unit);
				return end - begin;
			}
		};

		std::chrono::time_point<std::chrono::high_resolution_clock>
		timer::m_timer(std::chrono::high_resolution_clock::now());
	} // namespace utility
} // namespace cs

/*
* Covariant Script SDK Log System
*
* use COVSDK_LOGEV(message) to log a normal event
* use COVSDK_LOGCR(message) to log a critical event
*
* define COVSCRIPT_SDK_DEBUG to enable the log
* define COVSDK_LOGCR_ONLY to disable normal events
*
* All macros must be defined before include
*/

#ifdef COVSCRIPT_SDK_DEBUG
#include <cstdio>

#ifndef COVSDK_LOGCR_ONLY
// Event Log
#define COVSDK_LOGEV(msg) ::printf("EV[%s] In file %s, line %d: %s\n", __TIME__, __FILE__, __LINE__, msg);

#else

#define COVSDK_LOGEV(msg)

#endif
// Critical Event Log
#define COVSDK_LOGCR(msg) ::printf("CR[%s] In file %s, line %d: %s\n", __TIME__, __FILE__, __LINE__, msg);

#else

#define COVSDK_LOGEV(msg)
#define COVSDK_LOGCR(msg)

#endif