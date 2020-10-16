#ifndef TEMP_H
#define TEMP_H

#include <iostream>



void* operator new (size_t size)
{
    std::cout << "Allocation: " << size << std::endl;
    return malloc(size);
}
void operator delete(void* ptr)
{
    std::cout << "Deleting" << std::endl;
    free(ptr);
}
void operator delete(void* ptr, size_t size)
{
    std::cout << "Deleting: " << size << std::endl;
    free(ptr);
}


#include "Libraries/Fmt/fmt/core.h"
#include "Libraries/Fmt/fmt/os.h"
#include "Libraries/Fmt/fmt/printf.h"
#include "Libraries/Fmt/fmt/compile.h"

void FmtTesting()
{
    std::string buffer;
    std::cout << "Reserving buffer ================" << std::endl;
    buffer.reserve(128);
    std::cout << "Converting ====================" << std::endl;


    //buffer = fmt::sprintf("%0.8i", 32);
    buffer.clear();
    buffer = fmt::sprintf("%#0.16x", 0xffff);
    std::cout << buffer << std::endl;

    buffer.clear();
    fmt::basic_memory_buffer<char, 10> memBuff;
    //fmt::memory_buffer memBuff;
    using context = fmt::basic_printf_context_t<char>;

    fmt::basic_format_args<context> args = fmt::make_format_args<context>(32);
    fmt::vprintf(memBuff, fmt::to_string_view("                                 %i"), args);
    //auto newStr = std::basic_string<char>(bufferChar.data(), bufferChar.size());
    buffer.assign(memBuff.data(), memBuff.size());
    std::cout << buffer << std::endl;
    std::cout << "====================" << std::endl;

    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), FMT_STRING("{:#016x}"), 0xffff);
    std::cout << buffer << std::endl;


    buffer.clear();
    std::cout << "====================" << std::endl;
    void * ptr = &(buffer);
    fmt::format_to(std::back_inserter(buffer), FMT_STRING("{:p}"), ptr);
    std::cout << buffer << std::endl;
}

#include "General/StaticLog/StaticLog.h"
#include "General/StaticLog/TesterClasses.h"

void StaticLogInterpretTesting()
{

    std::string output;
    output.reserve(512);
    ::StaticLog::DefaultDataInterpret interpret;
    interpret.InterpretArg(0, output);
}

void DefaultLogWritterPackageTesting()
{
    using namespace StaticLog;
    using namespace StaticLogTesting;

    ::StaticLog::StaticLogTesting::DefaultLoggerPackageTester Tester1("My file");

    Tester1.CollectionString = "Collection string";
    Tester1.TempString = "Temp string";
    Tester1.Tags = "Tags";
    Tester1.EntryIndex = 1;

    ::StaticLog::StaticLogTesting::DefaultLoggerPackageTester Tester2(std::move(Tester1));

    StaticLog::LoggerInterface<StaticLog::DefaultLogger<>> LoggerInterface("File");

}

void LoggerCopyTesting()
{
    ::StaticLog::LoggerInterface<::StaticLog::StaticLogTesting::TestingLoggerCopy> logWriterCopy1(11, "Copy1");
    logWriterCopy1.DoLog(false);
    logWriterCopy1.DoLoggingLevels<1, 3>(false);

    ::StaticLog::LoggerInterface<::StaticLog::StaticLogTesting::TestingLoggerCopy> copyConstructed(logWriterCopy1);

    ::StaticLog::LoggerInterface<::StaticLog::StaticLogTesting::TestingLoggerMove> logWriterMove1(11, "Move1");
    logWriterMove1.DoLog(false);
    logWriterMove1.DoLoggingLevels<1, 3>(false);
    ::StaticLog::LoggerInterface<::StaticLog::StaticLogTesting::TestingLoggerMove> logWriterMove2(std::move(logWriterMove1));
    //int a = 0;
}

void LogDefLoggerTesting()
{
    StaticLog::DefaultLoggerPackage dataPackage("FilePackage");
    StaticLog::StaticLogTesting::GetDefPackageTester(dataPackage).CollectionString   = "PackageCollectionString";
    StaticLog::StaticLogTesting::GetDefPackageTester(dataPackage).TempString         = "PackageTempString";
    StaticLog::StaticLogTesting::GetDefPackageTester(dataPackage).Tags               = "PackageTags";
    StaticLog::StaticLogTesting::GetDefPackageTester(dataPackage).EntryIndex         = 33;


    StaticLog::LoggerInterface<StaticLog::DefaultLogger<>> logWriter1("File1");
    StaticLog::LoggerInterface<StaticLog::DefaultLogger<>> logWriter2("File2");

    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).CollectionString   = "Logger1CollectionString";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).TempString         = "Logger1TempString";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).Tags               = "Logger1Tags";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).EntryIndex         = 11;


    {
        StaticLog::LoggerInterface<StaticLog::DefaultLogger<>> fromWriter(std::move(logWriter1));

        bool trueFalse = false;
        (void)trueFalse;

        std::cout << StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).CollectionString << std::endl;
        std::cout << StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).TempString << std::endl;
        std::cout << StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).Tags << std::endl;
        std::cout << StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).EntryIndex << std::endl;

        trueFalse = StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).CollectionString   == "Logger1CollectionString";
        trueFalse = StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).TempString         == "Logger1TempString";
        trueFalse = StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).Tags               == "Logger1Tags";
        trueFalse = StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).EntryIndex         == 11;
    }
}

void LogDefLoggerConstructing()
{
    StaticLog::DefaultLoggerPackage dataPackage("FilePackage");
    StaticLog::StaticLogTesting::GetDefPackageTester(dataPackage).CollectionString   = "PackageCollectionString";
    StaticLog::StaticLogTesting::GetDefPackageTester(dataPackage).TempString         = "PackageTempString";
    StaticLog::StaticLogTesting::GetDefPackageTester(dataPackage).Tags               = "PackageTags";
    StaticLog::StaticLogTesting::GetDefPackageTester(dataPackage).EntryIndex         = 33;


    StaticLog::DefaultLogger<> logWriter1("File1");
    StaticLog::DefaultLogger<> logWriter2("File2");

    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).CollectionString   = "Logger1CollectionString";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).TempString         = "Logger1TempString";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).Tags               = "Logger1Tags";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).EntryIndex         = 11;


    {
        StaticLog::DefaultLogger<> fromWriter(std::move(logWriter1));
        StaticLog::DefaultLogger<> fromPackage(std::move(dataPackage));

        bool trueFalse = false;
        (void)trueFalse;

        std::cout << StaticLog::StaticLogTesting::GetDefLoggerTester(fromPackage).CollectionString << std::endl;
        std::cout << StaticLog::StaticLogTesting::GetDefLoggerTester(fromPackage).TempString << std::endl;
        std::cout << StaticLog::StaticLogTesting::GetDefLoggerTester(fromPackage).Tags << std::endl;
        std::cout << StaticLog::StaticLogTesting::GetDefLoggerTester(fromPackage).EntryIndex << std::endl;

        trueFalse = StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).CollectionString   == "Logger1CollectionString";
        trueFalse = StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).TempString         == "Logger1TempString";
        trueFalse = StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).Tags               == "Logger1Tags";
        trueFalse = StaticLog::StaticLogTesting::GetDefLoggerTester(fromWriter).EntryIndex         == 11;
    }
    //StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter2).UnravelAndAddVariables("var1", "value", "var2", 32, "var3", static_cast<void POINTER>(0x0));
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter2).UnravelAndAddMsgs("Msg1", "Msg2", "Msg3", "Msg4");
}

namespace UnorderedArgumentChain
{
    //This is a very interesting structure that can be used to pass optional arguments into functions
    // + can be used to pass multiple parameter packs
    //
    //The only question is the level of overhead generated by copying all the arguments to the temp variable - is there a way to circumwent this?
    //
    class Executer
    {

            std::string MyString1;
            std::string MyString2;
            std::string MyString3;

        public:
            Executer REF WithTags(const std::string tag)
            {
                MyString2 += tag;
                return POINTER_VALUE(this);
            }

            Executer REF WithSource(const std::string source)
            {
                MyString3 += source;
                return POINTER_VALUE(this);
            }

            Executer(const std::string msg) : MyString1(msg)
            {}
            Executer()
            {}
            ~Executer()
            {
                Execute();
            }
            //DataU MyData;
        private:
            void Execute()
            {
                std::cout << MyString1 << " " << MyString2 << " " << MyString3 << std::endl;
            }
    };

    struct Caller
    {
            Executer Function(const std::string msg)
            {
                return ToDerived(msg);
            }

            inline Executer ToDerived(const std::string msg)
            {
                return Executer(msg);
            }
    };

}

void UnorderedArgumentChainTesting()
{
    UnorderedArgumentChain::Caller caller;
    caller.Function("Hello World").WithTags("Tag").WithSource("File");
}


void StringAdditionSpeedTesting()
{
    using MicrosecondsChronoDurations = std::chrono::duration<double, std::ratio<1, 1000>>;

    auto tempTimePoint = std::chrono::high_resolution_clock::now();
    std::vector<MicrosecondsChronoDurations> duartions(15);

    std::string str1 = "Hello ";
    std::string str2 = "";

    for(u32 rotations = 0; rotations < 5; rotations ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < 100000000; i++)
        {
            str1.append(str2);
        }
        duartions[0 + rotations] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotations = 0; rotations < 5; rotations ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < 100000000; i++)
        {
            if(NOT(str2.empty()))
                str1.append(str2);
        }
        duartions[5 + rotations] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotations = 0; rotations < 5; rotations ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < 100000000; i++)
        {
            //str1.append(str2);
        }
        duartions[10 + rotations] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    u64 append = 0;
    u64 emptySwitch = 0;
    u64 control = 0;

    for(u32 rotations = 0; rotations < 5; rotations ++)
    {
        append += duartions[0 + rotations].count();
        emptySwitch += duartions[5 + rotations].count();
        control += duartions[10 + rotations].count();
    }

    append /= 5;
    emptySwitch /= 5;
    control /= 5;

    std::cout << "Simple append: " << append << "ms\n";
    std::cout << "Empty switch: " << emptySwitch << "ms\n";
    std::cout << "Control loop: " << control << "ms\n";
}

namespace PassingSupport
{
    struct ClassData
    {
        public:
            i32 MyInt1;
            i32 MyInt2;
            i32 MyInt3;
        public:
            u32 PassCount = 0;
    };

    struct ClassRef
    {
            ClassData REF Data;
            ClassRef(ClassData & data) : Data(data) {}
            ClassRef(ClassData && data) : Data(data) {}

            ClassRef() = delete;
            ClassRef(const ClassRef REF other) : Data(other.Data)
            {
                Data.PassCount++;

            }
            ClassRef(ClassRef RVALUE_REF) = delete;
            ~ClassRef()
            {
                Data.PassCount--;
            }
            ClassData POINTER operator->() {return ADDRESS(Data);}
    };

    void Function1(ClassRef ref)
    {
        std::cout << ref.Data.PassCount << std::endl;
    }

    void Function2(ClassRef ref)
    {
        std::cout << ref.Data.PassCount << std::endl;
        Function1(ref);
        std::cout << ref.Data.PassCount << std::endl;
    }

    void Function3(ClassRef ref)
    {
        std::cout << ref.Data.PassCount << std::endl;
        Function2(ref);
        std::cout << ref.Data.PassCount << std::endl;
    }
}

u32 Counter()
{
    static u32 counter = 0;
    u32 counterCopy = counter;

    counter++;

    return counterCopy;
}


std::string CounterText()
{
    return std::to_string(Counter());
}

thread_local StaticLog::DefaultLogger<> Log("Temp" + CounterText() + ".txt");

void GenericProgramingTests()
{

    const char * myStr = "Hello world";
    std::string stdString ="Longer than 15 msg XXXXXXXXX";
    LOG(0, Log, myStr);
    LOG(0, Log, stdString);
    LOG_VARS(0, Log, stdString, stdString, stdString);
    LOG_VARS(0, Log, stdString, stdString, stdString);

    LOG(0, Log, myStr);
    std::cout << myStr << std::endl;
}

void PassingSupportTesting()
{
    PassingSupport::ClassData data;
    PassingSupport::ClassRef ref(data);
    ref->MyInt1 = 0;
    PassingSupport::Function3(data);
}

using prevdefined = int;

#include "UnbufferedFile.h"
#include "General/StaticLog/SpeedTests.h"

//
// #define OS_NATIVE_UNICODE_SUPPORT utf8 / utf16 // utf32
// #define INTERNAL_OS_SUPPORT_     -1
// #define INTERNAL_OS_SUPPORT_utf8  1
// #define INTERNAL_OS_SUPPORT_utf16 2
// #define INTERNAL_OS_SUPPORT_utf32 3

// #define INTERNAL_OS_SUPPORT CONCAT(OS_NATIVE_, OS_NATIVE_UNICODE_SUPPORT)

#include "General/File/UniversalString.h"

void WideCharTesting()
{
    std::string str = "hello";
    std::wstring wstr = L"hello";
    //std::string REF strRef = str;
    //std::string_view strView = "hello";

    int result;
    result = fooFunctor(str);
    result = fooFunctor(wstr);
    result = fooFunctor("hello");
    result = fooFunctor(U"hello");
    (void)result;

    //foo("hello");
    //foo(str);
    //foo(wstr);
    //foo(strRef);
    //foo(strView);
    //foo(1);
}


void RunTemp()
{
    //FmtTesting();
    //StaticLogInterpretTesting();
    //DefaultLogWritterPackageTesting();
    //LoggerCopyTesting();
    //LogDefLoggerTesting();
    //LogDefLoggerConstructing();
    //UnorderedArgumentChainTesting();
    //ConstexprNumConversion();
    //StringAdditionSpeedTesting();
    //GenericProgramingTests();
    //PassingSupportTesting();
    WideCharTesting();

    //SpeedTests();
    //AllocationTests();
    //StaticLogLogCheckingOverheadTests();
    //LibSpeedTests();
}

#endif // TEMP_H
