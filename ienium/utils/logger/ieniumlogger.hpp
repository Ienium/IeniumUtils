#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

namespace ienium::utils
{
    // Define log levels
    enum LOGLEVEL
    {
        IENIUM_DEBUG = 1 << 0,
        IENIUM_INFO = 1 << 1,
        IENIUM_WARNING = 1 << 2,
        IENIUM_ERROR = 1 << 3
    };

    // Abstract Logger class
    class Logger
    {
        public:
        virtual void Log (LOGLEVEL level, std::string message) = 0;
        virtual ~Logger() {};
    };

    // Implementing console logger class
    class ConsoleLogger : public Logger
    {
        virtual void Log (LOGLEVEL level, std::string message) override
        /*
            Prints message with a header containing current time and loglevel to the console
        */
        {
            std::string color;
            std::string def = "\033[0m";

            std::string level_string;

            struct tm current_time;
            auto t = std::time (nullptr);
            localtime_s (&current_time,&t);
            auto time_string = std::put_time(&current_time, "%H:%M:%S ");
            
            switch (level)
            {
                case IENIUM_DEBUG:
                color = "\x1B[34m";
                level_string = "DEBUG";
                break;

                case IENIUM_INFO:
                color = "\x1B[32m";
                level_string = "INFO";
                break;

                case IENIUM_WARNING:
                color = "\x1B[33m";
                level_string = "WARNING";
                break;

                case IENIUM_ERROR:
                color = "\x1B[31m";
                level_string = "ERROR";
                break;
            }
            std::stringstream message_stream;
            message_stream << color << time_string << level_string << "\n  " << message << def;

            std::cout  << message_stream.str () << std::endl;
        }
    };

    // Singelton logger manager
    class LoggerManager
    {
        private:
        LoggerManager () {};

        Logger* logger;

        void DeleteLogger ()
        {
            if (logger != nullptr)
                delete logger;
        }

        public:
        static LoggerManager& GetInstance ()
        {
            static LoggerManager instance;

            return instance;
        }

        static void Shutdown ()
        {
            GetInstance ().DeleteLogger ();
        }

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Logger, T>>
        SetLogger ()
        /*
            Create the implementing logger object
        */
        {
            if (logger != nullptr)
            {
                delete logger;
            }

            logger = new T ();
        }

        Logger* GetLogger ()
        {
            return logger;
        }

    };
}