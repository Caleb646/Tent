#pragma once
#include "Logger.h"
#include <memory>
/*
* To turn off logging #define LOG_DEBUG false before including log.h
*/
#if defined(LOG_DEBUG_SETTING)
#else
	#define LOG_DEBUG_SETTING true
#endif

/*
* Commonly used values
*/
#define COMMON_VALUES(lvl) logger::commonLoggableValues(std::string(lvl), std::string(__FILE__), __LINE__)
#define GET_MAIN_LOGGER logger::Logger::me()
#define CREATE_LOGGABLE logger::createLoggable

#define DEFAULT_LOGGABLE 0 //index to the default Loggable added on creation of Logger.
#define GET_LOGGABLE_INSTANCE(loggableIndex) GET_MAIN_LOGGER.get(loggableIndex)
#define CREATE_COUT_LOGGER GET_MAIN_LOGGER.add(logger::Loggable(logger::CoutLoggable{}));
#define CREATE_FILE_LOGGER(path) GET_MAIN_LOGGER.add(logger::Loggable(logger::FileLoggable(std::string(path))));

/*
* Cout Logger Macros
*/

#define LOG(lvl, loggableIndex, msg) if(LOG_DEBUG_SETTING) GET_LOGGABLE_INSTANCE(loggableIndex).write(COMMON_VALUES(lvl), msg) //if(LOG_DEBUG)

#define LOG_INFO(loggableIndex, msg) LOG("INFO", loggableIndex, msg)
#define LOG_DEBUG(loggableIndex, msg) LOG("DEBUG", loggableIndex, msg)
#define LOG_WARNING(loggableIndex, msg) LOG("WARNING", loggableIndex, msg)
#define LOG_ERROR(loggableIndex, msg) LOG("ERROR", loggableIndex, msg)
#define LOG_FATAL(loggableIndex, msg) LOG("FATAL", loggableIndex, msg)

#define LOG_IF(lvl, loggableIndex, condition, msg) if((condition)) LOG(lvl, loggableIndex, msg)
#define LOG_INFO_IF(loggableIndex, condition, msg) LOG_IF("INFO", loggableIndex, condition, msg)
#define LOG_DEBUG_IF(loggableIndex, condition, msg) LOG_IF("DEBUG", loggableIndex, condition, msg)
#define LOG_WARNING_IF(loggableIndex, condition, msg) LOG_IF("WARNING", loggableIndex, condition, msg)
#define LOG_ERROR_IF(loggableIndex, condition, msg) LOG_IF("ERROR", loggableIndex, condition, msg)
#define LOG_FATAL_IF(loggableIndex, condition, msg) LOG_IF("FATAL", loggableIndex, condition, msg)

#define ASSERT_ERROR(loggableIndex, condition, msg) if(!(condition)) { LOG_ERROR(loggableIndex, msg); exit(EXIT_FAILURE); }
#define ASSERT_FATAL(loggableIndex, condition, msg) if(!(condition)) { LOG_FATAL(loggableIndex, msg); exit(EXIT_FAILURE); }