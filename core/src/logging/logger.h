#pragma once

#include <stdio.h>			// For va_lsit et al
#include <initializer_list> // For bracketed list of objects

#include <atomic>
#include <string>
#include <vector>
#include <memory>

#include "common.h"

#include "logVerbosity.h"
#include "logMessage.h"
#include "sinks/sink.h"

// TODO: Allow a logger to have the capability of having a backlog of message such that message don't have to be logged immediately

namespace GRAVEngine
{
	namespace Logging
	{
		// Each logger has a name, verbosity, and sinks
		// On Write
		//	1. Check if verbosity is enough
		//	2. Call sinks to handle logging
		//	3. Each sink has its own sink formatting
		class GRAVAPI logger
		{
		public:
			explicit logger(std::string name);
			explicit logger(std::string name, ref<Sinks::sink> sinkPtr);
			template<typename it>
			logger(std::string name, it begin, it end) : m_Name(std::move(name)), m_Sinks(begin, end) {}
			logger(std::string name, std::initializer_list<ref<Sinks::sink>> sinks) : logger(std::move(name), sinks.begin(), sinks.end()) {}
			logger(const logger& other);
			logger(logger&& other) noexcept;

			logger& operator= (logger other);
			logger& operator= (logger&& other) noexcept;

			void swap(logger& other);

			~logger() = default;

			int log(Logging::verbosity verbosity, const char* format, ...);
			int logLine(Logging::verbosity verbosity, const char* format, ...);

			// Name
			inline std::string name() const
			{
				return m_Name;
			}

			// Should the verbosity be logged in this logger
			inline bool shouldLog(Logging::verbosity verbosity) const
			{
				return m_Verbosity.load(std::memory_order_relaxed) >= static_cast<int>(verbosity);
			}
			// Set this logger's verbosity
			inline void setVerbosity(Logging::verbosity verbosity)
			{
				m_Verbosity.store(static_cast<int>(verbosity));
			}
			// Get this logger's verbosity
			inline Logging::verbosity verbosity() const
			{
				return static_cast<Logging::verbosity>(m_Verbosity.load(std::memory_order_relaxed));
			}

			// Flush this logger's sinks
			void flush();
			// Get the flush verbosity
			inline Logging::verbosity flushVerbosity() const
			{
				return static_cast<Logging::verbosity>(m_FlushVerbosity.load(std::memory_order_relaxed));
			}
			// Set the flush verbosity
			inline void setFlushVerbosity(Logging::verbosity verbosity)
			{
				m_FlushVerbosity.store(static_cast<int>(verbosity));
			}
			// Should a message be flushed based on the flush verbosity
			inline bool shouldFlush(const Logging::logMessage& message)
			{
				auto verbosity = m_FlushVerbosity.load(std::memory_order_relaxed);
				return verbosity >= static_cast<int>(message.m_Verbosity) && message.m_Verbosity != Logging::verbosity::off;
			}


			// Get the sinks
			inline const std::vector<std::shared_ptr<Sinks::sink>>& sinks() const
			{
				return m_Sinks;
			}
			// Get the sinks
			inline std::vector<std::shared_ptr<Sinks::sink>>& sinks()
			{
				return m_Sinks;
			}

		private:
			std::string m_Name;
			std::vector<ref<Sinks::sink>> m_Sinks;

			Logging::verbosity_t m_Verbosity;
			Logging::verbosity_t m_FlushVerbosity;

		private:
			int log(Logging::verbosity verbosity, bool logEnabled, const char* format, va_list args);
			// Log a message
			void logMessage(Logging::logMessage& message, bool logEnabled);
			// Send a message to the sinks
			void sinkMessage(Logging::logMessage& message);
			
		};
	}
}