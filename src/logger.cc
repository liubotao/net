#include <time.h>
#include <stdarg.h>
#include <memory.h>

#include "logger.h"

static Logger _stdoutput;
Logger *defaultLogger_ = &_stdoutput;

static const char *msgTypeText[] = {
    "Message",
    "Warning",
    "Error"
};

Logger::Logger(void){}

Logger::~Logger(void){}

void Logger::output(Logger::MsgType type, const char *msg) {
    time_t t = time(NULL);
    tm *lt = localtime(&t);

    printf("[%d-%02d-%02d %02d:%02d:%02d] %s: %s\n",
           lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
           lt->tm_hour, lt->tm_min, lt->tm_sec, msgTypeText[type], msg);
}

void Logger::error(const char* format, ...) {
    if (!format || !defaultLogger_) {
        return;
    }
    
    char buffer[10240];
    va_list marker;
    va_start(marker, format);
    vsprintf(buffer, format, marker);
    va_end(marker);

    defaultLogger_->output(Logger::Error, buffer);
}

void Logger::message(const char *format, ...) {
   if (!format || !defaultLogger_) {
        return;
    }
 
   char buffer[10240];
   va_list marker;
   va_start(marker, format);
   vsprintf(buffer, format, marker);
   va_end(marker);
   defaultLogger_->output(Logger::Message, buffer);
}

 void Logger::warn(const char *format, ...) {
    if (!format || !defaultLogger_) {
        return;
    }

    char buffer[10240];
    va_list marker;
    va_start(marker, format);
    vsprintf(buffer, format, marker);
    va_end(marker);
    defaultLogger_->output(Logger::Warning, buffer);
}

void Logger::log(Logger::MsgType type, const char *format, ...) {
    if (!format || !defaultLogger_) {
        return;
    }

    char buffer[10240];
    va_list marker;
    va_start(marker, format);
    vsprintf(buffer, format, marker);
    va_end(marker);

    defaultLogger_->output(type, buffer);
}


Logger *Logger::defaultLogger(void) {
    return defaultLogger_;
}

void Logger::setDefaultLogger(Logger *logger) {
    defaultLogger_ = logger;
}

FileLogger::FileLogger(void) {
    fp_ = NULL;
    memset(fileName_, 0, sizeof(fileName_));
}

FileLogger::FileLogger(const char *fileName) {
    setFileName(fileName);
}

FileLogger::~FileLogger(void) {
    if (fp_) {
        fclose(fp_);
    }
}

bool FileLogger::setFileName(const char *fileName) {
    FILE *fp = fopen(fileName, "a+");
    if (!fp) {
        return false;
    }

    strcpy(fileName_, fileName);
    if (fp_) {
        fclose(fp_);
    }
    
    fp_ = fp;
    return true;
}

void FileLogger::output(Logger::MsgType type, const char *msg) {
    time_t t = time(NULL);
    tm* lt = localtime(&t);

    fprintf(fp_, "[%d-%02d-%02d %02d:%02d:%02d] %s: %s\n",
            lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
            lt->tm_hour, lt->tm_min, lt->tm_sec, msgTypeText[type], msg);
    fflush(fp_);
}
