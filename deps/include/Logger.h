#pragma once
#include "Loggable.h"

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

namespace logger
{
	/*
	* @brief The Main Logger Class. Is a Singleton
	*/
	class Logger
	{
	private:
		std::vector<Loggable> logs;

	private:
		/*
		* @brief On creation of the Logger create a default Loggable.
		*/
		Logger() { this->add(Loggable(CoutLoggable{})); }

	public:
		static Logger& me()
		{
			static Logger l;
			return l;
		}

		/*
		* @brief Using the provided index gets that instance
		* of the logger from the logs vector and returns it.
		*
		* @param index is the location of the desired Loggable
		* in the vector logs.
		* @return A reference to the desired instance of Loggable
		*/
		Loggable& get(std::size_t index)
		{
			assert(index < logs.size());
			return logs[index];
		}

		/*
		* @brief Adds a Loggable to the vector logs.
		* Uses the Loggables clone method to clone a 
		* log before adding it to the vector.
		* 
		* @param log is a reference to a Loggable
		* @return The index of the log the vector logs.
		*/
		std::size_t add(Loggable& log)
		{
			logs.push_back(std::move(log));
			//return *this;
			return logs.size() - 1;
		}

		//Loggable contains a unique ptr so it cannot be copied
		//so std::move the rvalue of Loggable into the vector.
		//Means that l = Loggable(); add(l); l will now be empty because
		//its value has been moved into std::vector<Loggable> logs.
		/*
		* @brief Adds a Loggable to the vector logs.
		* Uses the Loggables clone method to clone a
		* log before adding it to the vector. Uses std::move
		* to move log into logs vector without requiring a copy.
		*
		* @param log is a rvalue of a Loggable instance
		* @return The index of the log the vector logs
		*/
		std::size_t add(Loggable&& log)
		{
			logs.push_back(std::move(log));
			return logs.size() - 1;
		}

		/*
		* @brief Calls the most recently added Loggable instance's write method
		* and passes it any msg.
		*
		* @tparam T is the type of value
		* @param logger is a reference to the main Logger instance
		* @param value is the message that will be passed to the loggable instance to be written.
		* @return A reference to the Logger class.
		*/
		template<typename T>
		friend Logger& operator<<(Logger& logger, const T* value)
		{
			static_assert(false, "This method should not be used.");
			logger.logs.back().write(value);
			logger.logs.pop_back();
			return logger;
		}
	};
}

