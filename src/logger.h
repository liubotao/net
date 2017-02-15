
#include <stdio.h>

class Logger {
public:
    enum MsgType {
        Message = 0,
        Warning = 1,
        Error   = 2
    };

    Logger(void);
    virtual ~Logger(void);

    virtual void output(MsgType type, const char *msg);

    static void log(MsgType type, const char *format, ...);

    static void warn(const char *format, ...);
    
    static void message(const char *format, ...);

    static void error(const char *format, ...);

    static Logger *defaultLogger(void);
    
    static void setDefaultLogger(Logger *logger);

};

class FileLogger : public Logger {
public:
    FileLogger();
    FileLogger(const char *fileName);
    ~FileLogger();

    bool setFileName(const char *fileName);
    const char *fileName(void) const {
        return fileName_;
    }

    virtual void output(MsgType type, const char *msg);

private:
    FILE* fp_;
    char fileName_[512];
};
