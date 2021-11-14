#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Utils.h"


namespace logger
{
	struct commonLoggableValues
	{
		std::string lvl;
		std::string fileName;
		std::size_t lineNumber;

		commonLoggableValues(std::string&& _lvl, std::string&& _fileName, std::size_t&& _lineNumber)
			: lvl(std::move(_lvl)), fileName(std::move(_fileName)), lineNumber(std::move(_lineNumber)) {}

		std::string toString()
		{
			return lvl + " " + _internal::shortenByDelimiter(fileName, 2, "\\") + " " + std::to_string(lineNumber);
		}
	};


	/**
	* @brief The main interface which creates the blueprint for all Loggables.
	* Allows 
	* @param _log is copy of a Log Instance
	* @return LoggableModel<Log>
	*/
	class LoggableInterface
	{
	public:
		virtual ~LoggableInterface() = default;
		virtual std::unique_ptr<LoggableInterface> clone() = 0;
		virtual std::string toString() = 0;
		virtual void write(commonLoggableValues&&, const char*) = 0;
	};

	/**
	* @brief Provides a wrapper for Log and holds an instance of it
	* Allows for Log's type to be erased and a ptr to LoggableModel<Log> is held by Loggable
	* @tparam Log is the type of Log that needs to be wrapped
	*/
	template<typename Log>
	class LoggableModel : public LoggableInterface
	{
	private:
		Log logger;
	public:
		/**
		* @brief Main Constructor
		* Forwards a copy of the Log's instance into the logger variable
		* @param _log is copy of a Log Instance
		* @return LoggableModel<Log>
		*/
		explicit LoggableModel(Log _log)
			: logger(std::forward<Log>(_log)) {}

		/**
		* @brief allows for this to copied
		* Returns a new unique_ptr with the type of LoggableModel<Log>
		* and a copy of the Log instance
		* @return std::make_unique<LoggableModel<Log>>(logger)
		*/
		virtual std::unique_ptr<LoggableInterface> clone() override
		{
			return std::make_unique<LoggableModel<Log>>(logger);
		}
		/**
		* @brief Forwards toString call onto Log instance's method toString
		* @return std::string
		*/
		virtual std::string toString() override
		{
			return logger.toString();
		}
		/**
		* @brief Forwards write call onto Log instance's method write
		* @param common is a struct filled with common log info such
		* as the file path line number and severity lvl.
		* @param value is the msg the user add along with the log.
		* @return void
		*/
		virtual void write(commonLoggableValues&& common, const char* value) override
		{
			logger.write(std::move(common), value);
		}
	};

	/*
	* @brief The base type of all Loggables.
	* Uses type erasure to hide type Log's type.
	*/
	class Loggable
	{
	private:
		///because of the unique ptr the Loggable class cannot be copied but it can be cloned
		std::unique_ptr<LoggableInterface> loggableInterface;
	public:

		/*
		* @brief On Copy clone other's loggableInterface
		* @param other is another instance of Loggable to be copied
		* @return Loggable Instance
		*/
		Loggable(const Loggable& other) 
			: loggableInterface(other.loggableInterface->clone()) {}

		/**
		* @brief Move constructor. Swaps this with
		* @param other is another instance of Loggable to be copied
		* @return Loggable Instance
		*/
		Loggable(Loggable&& other) noexcept { swap(other); }

		/**
		* @brief Main constructor
		* Forwards Log and its instance onto LoggableModel<Log> constructor and on 
		* return takes LoggableModel<Log>'s instance and creates a unique ptr to it
		* @tparam Log type is the Log type that is being wrapped by Loggable
		* @param _log rvalue instance of Log
		* @return Loggable Instance
		*/
		template<typename Log> 
		Loggable(Log&& _log) // double && allows for rvalues to be used in constructor. so Loggable(FileLoggable()) is a valid constructor.
			: loggableInterface(std::make_unique<LoggableModel<Log>>(std::forward<Log>(_log))) {}
		~Loggable() = default;

		/**
		* @brief Swaps this's unique ptr with other's unique ptr
		* @param other is a reference to a different instance of Loggable
		* @return void
		*/
		void swap(Loggable& other) { loggableInterface.swap(other.loggableInterface); } //swap loggableInterface ptr with others ptr.

		/**
		* @brief On assignment swaps this with other
		* @param other is a reference to a different instance of Loggable
		* @return a reference to the swapped Loggable instance
		*/
		Loggable& operator=(Loggable& other) { swap(other); return *this; }

		/**
		* @brief Forwards call to LoggableModel<Log>
		* @return a std::string
		*/
		std::string toString()
		{
			return loggableInterface.get()->toString();
		}

		/**
		* @brief Forwards call to LoggableModel<Log>
		* @param common is a struct filled with common log info such
		* as the file path line number and severity lvl.
		* @param value is the msg the user add along with the log.
		* @return void
		*/
		void write(commonLoggableValues&& common, const char* value)
		{
			loggableInterface.get()->write(std::move(common), value);
		}
	};

	/**
	* @brief Factory function to create Loggables
	* @tparam Log type to be passed to Loggable constructor
	* @tparam Args type of arguments used to construct Log
	* @param args the arguments used to initialize the Log
	* @return LoggableModel<Log>
	*/
	template<typename Log, typename ...Args>
	Loggable createLoggable(Args&& ... args)
	{
		static_assert(false, "This function should not be used.");
		return Loggable(std::move(Log(std::forward<Args>(args)...)));
	}


	class CoutLoggable
	{

	public:
		CoutLoggable() {}
		CoutLoggable(const CoutLoggable& other) {}
		explicit CoutLoggable(CoutLoggable&& other) noexcept {}

		std::string toString()
		{
			return std::string("hello");
		}
		void write(commonLoggableValues&& common, const char* value)
		{
			std::cout << "\n" << common.toString() << " " << value << std::endl;
		}
	};

	class FileLoggable
	{
	private:
		std::string path;

	public:
		FileLoggable(std::string _path)
			: path(_path) {}
		FileLoggable(const FileLoggable& other)
			: path(other.path) {}
		explicit FileLoggable(FileLoggable&& other) noexcept
			: path(std::move(other.path)) {}

		std::string toString()
		{
			return std::string("hello");
		}

		void write(commonLoggableValues&& common, const char* value)
		{
			std::ofstream f;
			f.open(path, std::fstream::app);
			f << "\n" << common.toString() << " " << value;
			f.close();
		}
	};
}

