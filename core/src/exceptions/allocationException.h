#pragma once

#include <string>
#include "common.h"

namespace GRAVEngine
{
	namespace Exceptions
	{
		class allocationException : public std::bad_alloc
		{
		public:
			explicit allocationException(const char* message);
			explicit allocationException(const std::string& message) : allocationException(message.c_str()) {}
			allocationException(const allocationException& other) noexcept;

			allocationException& operator= (const allocationException& other) noexcept;

			const char* what() const noexcept override;

			~allocationException() noexcept;

		private:
			const char* m_Message;

		};
	}
}