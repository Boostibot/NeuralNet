#ifndef LOGTESTS_H
#define LOGTESTS_H

#include <vector>
#include "Log.h"

class LogTestWriter : public LogWriterInterface
{
    public:
        std::vector<std::string> Results;

        ///Logs the given msg on the given level
        virtual void LogMsg(const u32 level, const std::string_view msg)
        {

        }
        ///Logs the given varName alongside its value on the given level
        virtual void LogVar(const u32 level, const std::string_view varName, const std::string_view varValue)
        {

        }
        ///Logs the given msg on the given leveland the initial LOG command origin file and line on the given level
        virtual void LogMsgSource(const u32 level, const std::string_view file, const u32 lineNum, const std::string_view msg)
        {

        }

        ///Logs the given varName alongside its value and the initial LOG command origin file and line on the given level
        virtual void LogVarSource(const u32 level, const std::string_view file, const u32 lineNum, const std::string_view varName, const std::string_view varValue)
        {

        }

        ///Starts the logging by parts process; Always should be accompanied by LogByPartsFinish()
        virtual void LogByPartsStart() = 0;
        ///Adds a level (level) part to the LogByParts process
        virtual void LogByPartsLvl(const u32 level) = 0;
        ///Adds a source (file, line) part to the LogByParts process
        virtual void LogByPartsSource(const std::string_view file, const u32 lineNum) = 0;
        ///Adds a message (msg) part to the LogByParts process
        virtual void LogByPartsMsg(const std::string_view msg) = 0;
        ///Adds a variable (varName, varValue) part to the LogByParts process
        virtual void LogByPartsVar(const std::string_view varName, const std::string_view varValue) = 0;
        ///Ends the logging by parts process; Always should be accompanied by LogByPartsStart()
        virtual void LogByPartsFinish() = 0;

        ///Returns if this LogWriter is an empty logger (empty logger = doesnt write log with any given input)
        virtual bool IsEmptyLogger() = 0;

    public:
        ///Logs a custom message; The functionSpecifier shoudl be filled with the implementaton specific enum values
        virtual void LogCustom(const i32 functionSpecifier, const u32 level, void POINTER pointer = nullptr) {(void)functionSpecifier; (void)level; (void)pointer;}
        ///Sets a custom log state; The functionSpecifier shoudl be filled with the implementaton specific enum values
        virtual void SetCustomLogState(const i32 stateSpecifier, void POINTER pointer = nullptr) {(void)stateSpecifier; (void)pointer;}

        virtual ~LogTestWriter() {}

};


bool TestLoggerSetting()
{
    Logger log;

    //Checking of the allocation state
    if(NOT(log.GetEmptyLogWriter()->IsEmptyLogger()))
    {
        std::cout << "After creation Empty logger core is not empty" << std::endl;
        return false;
    }

    if(log.GetNonemptyLogWriter()->IsEmptyLogger())
    {
        std::cout << "After creation Nonempty logger core is empty" << std::endl;
        return false;
    }

    if(NOT(log.LoggingStatus))
    {
        std::cout << "After creation Logging status is off" << std::endl;
        return false;
    }

    return true;
}

bool TestLoggingStandard()
{
    Logger log;

    log.GetNonemptyLogWriter()->OpenOutputStream("Test logger.txt", 1);

    LOG(0, log, "MSG");

    //Testing logging msgs
    std::cout << "Check output below if they match" << std::endl;
    std::cout << "<prefixes> <0> : msg = Hello" << std::endl;
    std::cout << "<prefixes> <0> : msg = Hello World" << std::endl;
    std::cout << "<prefixes> <1> : msg = Hello World" << std::endl;
    std::cout << "<prefixes> <0> : msg = var1" << std::endl;
    std::cout << "<prefixes> <0> : msg = var1 true" << std::endl;
    std::cout << "File [58] <prefixes> <0> : msg = Logging with source" << std::endl;
    std::cout << "Output below:" << std::endl;
    log.LogMsg(0, "Hello");
    log.LogMsg(0, "Hello", " World");
    log.LogMsg(1, "Hello", " World");
    log.LogMsg(0, "var", 1);
    log.LogMsg(0, "var", 1, " ", true);
    log.LogMsgSource(0, "File", 58, "Logging with source");

    std::cout<<std::endl;

    //Testing logging vars
    std::cout << "Check output below if they match" << std::endl;
    std::cout << "<prefixes> <0> : int1 = 1" << std::endl;
    std::cout << "<prefixes> <3> : int2 = 2" << std::endl;
    std::cout << "File [58] <prefixes> <0> : int1 = 1" << std::endl;
    std::cout << "File [58] <prefixes> <0> : int2 = 2" << std::endl;
    std::cout << "Output below:" << std::endl;

    i32 int1 = 1;
    i32 int2 = 2;
    i32 int3 = 3;

    log.LogVar(0, "int1", int1);
    log.LogVar(3, "int2", int2);
    log.LogVarSource(0, "File", 58, "int1", int1);
    log.LogVarSource(0, "File", 58, "int2", int2);

    std::cout<<std::endl;


    return true;
}

bool TestLoggingTemplated()
{

    return true;
}

bool TestLoggerInfo()
{

    return true;
}


bool TestLogger()
{
    Logger errorLog;

    if(NOT(TestLoggerSetting()))    {std::cout << "TestLoggerSetting fail" << std::endl;}
    if(NOT(TestLoggingStandard()))  {std::cout << "TestLoggingStandard fail" << std::endl;}
    if(NOT(TestLoggingTemplated())) {std::cout << "TestLoggingTemplated fail" << std::endl;}
    if(NOT(TestLoggerInfo()))       {std::cout << "TestLoggerInfo fail" << std::endl;}

    return true;
}

#endif // LOGTESTS_H
