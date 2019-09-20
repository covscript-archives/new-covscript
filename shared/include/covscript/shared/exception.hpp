#pragma once
#include <covscript/shared/utility.hpp>
#include <exception>
#include <string>

namespace cs {
	class runtime_error final : public std::exception {
		std::string mWhat = "Runtime Error";

	public:
		runtime_error() = default;

		explicit runtime_error(const std::string &str) noexcept : mWhat("Runtime Error: " + str) {}

		runtime_error(const runtime_error &) = default;

		runtime_error(runtime_error &&) noexcept = default;

		~runtime_error() override = default;

		runtime_error &operator=(const runtime_error &) = default;

		runtime_error &operator=(runtime_error &&) = default;

		const char *what() const noexcept override
		{
			return this->mWhat.c_str();
		}
	};

	namespace event {
		utility::event_type exception_raised;
	}

	template <typename T, typename... ArgsT>
	void throw_ex(ArgsT &&... args)
	{
		T exception(std::forward<ArgsT>(args)...);
		std::exception &stdexcept = exception;
		COVSDK_LOGCR(stdexcept.what())
		// Handle exceptions here
		event::exception_raised.touch(&stdexcept);
#ifdef COVSCRIPT_NOEXCEPT
		std::terminate();
#else
		throw exception;
#endif
	}
} // namespace cs