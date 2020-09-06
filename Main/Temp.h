#ifndef TEMP_H
#define TEMP_H

#include <iostream>

/*
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
*/

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

#include "GClasses/Common/DynamicLog/Log.h"
#include "GClasses/Common/DynamicLog/LogTests.h"
#include "GClasses/Common/Tester.h"
#include "GClasses/Common/StaticLog/StaticLog.h"
#include "GClasses/Common/StaticLog/TesterClasses.h"

G_TEST_CASE("Test test-case")
{
    std::vector<int> a(3);
    G_ASSERT(a.size() == 3);
}

template<typename type>
struct IndetifierStruct {};


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


    StaticLog::DefaultLogger logWriter1("File1");
    StaticLog::DefaultLogger logWriter2("File2");

    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).CollectionString   = "Logger1CollectionString";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).TempString         = "Logger1TempString";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).Tags               = "Logger1Tags";
    StaticLog::StaticLogTesting::GetDefLoggerTester(logWriter1).EntryIndex         = 11;


    {
        StaticLog::DefaultLogger fromWriter(std::move(logWriter1));
        StaticLog::DefaultLogger fromPackage(std::move(dataPackage));

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
            {

            }
            Executer()
            {

            }
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

namespace  DefaultLoggerHelper
{
    template<u32 size, typename CharT = char>
    struct CompileTimeString
    {
            //std::array<CharT, size> Str;
            char Str[size] = {};
            bool IsNull = true;

            constexpr CompileTimeString() {}
            constexpr CompileTimeString(const bool isNull) : IsNull(isNull) {}
    };

    constexpr u32 StringLenght(const char* str)
    {
        u32 i = 0;
        while(str[i] != '\0')
            i++;

        return i;
    }

    constexpr u32 StrSizeForConversion(const int num, const u32 base = 10)
    {
        using NumType = int;
        NumType remainder = num;
        u32 i = 0;

        if(num < 0)
        {
            //Sets the remeinder to teh abs of num
            remainder = -num;
            //Adds space for the minus sign
            i ++;
        }

        for(; remainder > 0; i++)
            remainder /= base;


        //+1 for termination
        return i + 1;
    }

    template<u32 strSize, typename CharT = char>
    constexpr CompileTimeString<strSize> ConvertToString(const int num, const u32 base = 10)
    {
        using NumType = int;
        NumType remainder = num;
        NumType divisionResult = 0;
        constexpr const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        constexpr u32 charsetLenght = StringLenght(charset);
        CompileTimeString<strSize> str;

        //If base is too large
        if(base > charsetLenght)
            return str;

        u32 sizeNeeded = StrSizeForConversion(num, base);

        //If the size needed is bigger then the one given return
        if(sizeNeeded > strSize)
            return str;


        if(num < 0)
            remainder = -num;

        u32 i = sizeNeeded - 1;
        str.Str[i] = '\0';
        for(i--; remainder > 0; i--)
        {
            divisionResult = remainder / base;

            str.Str[i] = charset[remainder - divisionResult * base];
            remainder = divisionResult;
        }

        //Adds the minus sign
        if(num < 0)
            str.Str[i] = '-';

        str.IsNull = false;
        return str;
    }

    template<u32 strSize, typename CharT = char>
    CompileTimeString<strSize> ConvertToStringRun(const int num, const u32 base = 10)
    {
        using NumType = int;
        NumType remainder = num;
        NumType divisionResult = 0;
        constexpr const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        constexpr u32 charsetLenght = StringLenght(charset);
        CompileTimeString<strSize> str;

        //If base is too large
        if(base > charsetLenght)
            return str;

        u32 sizeNeeded = StrSizeForConversion(num, base);

        //If the size needed is bigger then the one given return
        if(sizeNeeded > strSize)
            return str;


        if(num < 0)
            remainder = -num;

        u32 i = sizeNeeded - 1;
        str.Str[i] = '\0';
        for(i--; remainder > 0; i--)
        {
            divisionResult = remainder / base;

            str.Str[i] = charset[remainder - divisionResult * base];
            remainder = divisionResult;
        }

        //Adds the minus sign
        if(num < 0)
            str.Str[i] = '-';

        str.IsNull = false;
        return str;
    }

    template <int num, u32 base = 10, typename CharT = char>
    struct Convert
    {
        public:
            static constexpr const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        private:
            static constexpr bool AssertIfBaseIsInsufficient()
            {
                if(base > StringLenght(charset))
                {
                    static_assert (base > StringLenght(charset), "Base is too big");
                    return false;
                }
                return true;
            }

        public:
            static constexpr u32 Size = StrSizeForConversion(num, base);
            static constexpr CompileTimeString<Size, CharT> CmpStr = ConvertToString<Size, CharT>(num, base);
            static constexpr bool IsNull = AssertIfBaseIsInsufficient();
            static constexpr const CharT* Value = CmpStr.Str;
    };

    //constexpr char LevelIndicatorStrings[LoggerInterface<>::LevelCount][3] {{"00"}, {"01"}, {"02"}, {"03"}, {"04"}, {"05"}, {"06"}, {"07"}, {"08"}, {"09"}, {"10"}, {"11"}, {"12"}, {"13"}, {"14"}, {"15"}};
}

void ConstexprNumConversion()
{
    using namespace DefaultLoggerHelper;

    //constexpr u32 charsetLenght = StringLenght(charset);
    //constexpr CompileTimeString<32> str = CompileTimeString<32>(true);
    CompileTimeString<StrSizeForConversion(-1358)> converted = ConvertToStringRun<StrSizeForConversion(-1358)>(-1358);
    converted = ConvertToStringRun<6>(1358);

    std::cout << Convert<123456>::Value << std::endl;
    std::cout << Convert<-123456>::Value << std::endl;
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

struct CrashDebugger
{
        static constexpr const char MyString[6] = {"Hello"};
};

struct DontCrashDebugger
{
        const char MyString[6] = {"Hello"};
};

void DebuggerCrashTesting()
{
    DontCrashDebugger dont;
    CrashDebugger crash;
    int b = 0;
}

void RunTemp()
{
    //FmtTesting();
    StaticLogInterpretTesting();
    DefaultLogWritterPackageTesting();
    LoggerCopyTesting();
    LogDefLoggerTesting();
    LogDefLoggerConstructing();
    //UnorderedArgumentChainTesting();
    //ConstexprNumConversion();
    //StringAdditionSpeedTesting();
    DebuggerCrashTesting();
}

#endif // TEMP_H
