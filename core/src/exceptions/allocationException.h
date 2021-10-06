#pragma once

#include <string>
#include "common.h"

namespace GRAVEngine
{
	namespace Exceptions
	{
		class GRAVAPI allocationException : public std::bad_alloc
		{
		public:
			explicit allocationException(const char* message) : allocationException(std::string(message)) {}
			explicit allocationException(const std::string& message) : m_Message("bad allocation: " + message) {}
			allocationException(const allocationException& other) noexcept : m_Message(other.m_Message) {}

			allocationException& operator= (const allocationException& other) noexcept;

			const char* what() const noexcept override;

			~allocationException() noexcept = default;

		private:
			std::string m_Message;

		};
	}
}