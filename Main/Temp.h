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

/*

template <typename S, typename... Args,
          typename Char = enable_if_t<detail::is_string<S>::value, char_t<S>>>
inline std::basic_string<Char> sprintf(const S& format, const Args&... args) {
  using context = basic_printf_context_t<Char>;
  return vsprintf(to_string_view(format), make_format_args<context>(args...));
}


template <typename S, typename Char = char_t<S>>
inline std::basic_string<Char> vsprintf(
    const S& format,
    basic_format_args<basic_printf_context_t<type_identity_t<Char>>> args) {
  basic_memory_buffer<Char> buffer;
  vprintf(buffer, to_string_view(format), args);
  return to_string(buffer);
}


// Converts an integer argument to T for printf, if T is an integral type.
// If T is void, the argument is converted to corresponding signed or unsigned
// type depending on the type specifier: 'd' and 'i' - signed, other -
// unsigned).
template <typename T, typename Context, typename Char>
void convert_arg(basic_format_arg<Context>& arg, Char type) {
  visit_format_arg(arg_converter<T, Context>(arg, type), arg);
}

OutputIt basic_printf_context<OutputIt, Char>::format() {
*/

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

#include "GClasses/Common/Log.h"
#include "GClasses/Common/LogTests.h"
#include "GClasses/Common/Tester.h"
#include "GClasses/Common/StaticLog.h"

G_TEST_CASE("Test test-case")
{
    std::vector<int> a(3);
    G_ASSERT(a.size() == 3);
}

template<typename type>
struct IndetifierStruct {};


void StaticLoggerInterpretTesting()
{

    std::string output;
    output.reserve(512);
    ::StaticLogger::DefaultDataInterpret interpret;
    interpret.InterpretArg(0, output);
}

void DefaultLogWritterPackageTesting()
{
    using namespace StaticLogger;
    using namespace StaticLogTesting;

    ::StaticLogger::StaticLogTesting::DefaultLoggerPackageTester Tester1("My file");

    Tester1.CollectionString = "Collection string";
    Tester1.TempString = "Temp string";
    Tester1.Messages = "Messages";
    Tester1.Variables = "Variables";
    Tester1.Tags = "Tags";
    Tester1.Source = "Source";
    Tester1.EntryIndex = 1;

    ::StaticLogger::StaticLogTesting::DefaultLoggerPackageTester Tester2(std::move(Tester1));

    StaticLogger::LoggerInterface<StaticLogger::DefaultLogger> LoggerInterface("File");

}


void LoggerCopyTesting()
{
    ::StaticLogger::LoggerInterface<::StaticLogger::StaticLogTesting::TestingLoggerCopy> logWriterCopy1(11, "Copy1");
    logWriterCopy1.DoLog(false);
    logWriterCopy1.DoLoggingLevels<1, 3>(false);

    ::StaticLogger::LoggerInterface<::StaticLogger::StaticLogTesting::TestingLoggerCopy> copyConstructed(logWriterCopy1);

    ::StaticLogger::LoggerInterface<::StaticLogger::StaticLogTesting::TestingLoggerMove> logWriterMove1(11, "Move1");
    logWriterMove1.DoLog(false);
    logWriterMove1.DoLoggingLevels<1, 3>(false);
    ::StaticLogger::LoggerInterface<::StaticLogger::StaticLogTesting::TestingLoggerMove> logWriterMove2(std::move(logWriterMove1));
    //int a = 0;
}

void LogDefLoggerTesting()
{
    StaticLogger::DefaultLoggerPackage dataPackage("FilePackage");
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).CollectionString   = "PackageCollectionString";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).TempString         = "PackageTempString";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).Messages           = "PackageMessages";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).Variables          = "PackageVariables";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).Tags               = "PackageTags";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).Source             = "PackageSource";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).EntryIndex         = 33;


    StaticLogger::LoggerInterface<StaticLogger::DefaultLogger> logWriter1("File1");
    StaticLogger::LoggerInterface<StaticLogger::DefaultLogger> logWriter2("File2");

    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).CollectionString   = "Logger1CollectionString";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).TempString         = "Logger1TempString";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).Messages           = "Logger1Messages";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).Variables          = "Logger1Variables";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).Tags               = "Logger1Tags";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).Source             = "Logger1Source";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).EntryIndex         = 11;


    {
        StaticLogger::LoggerInterface<StaticLogger::DefaultLogger> fromWriter(std::move(logWriter1));

        bool trueFalse = false;
        (void)trueFalse;
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).CollectionString   == "Logger1CollectionString";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).TempString         == "Logger1TempString";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).Messages           == "Logger1Messages";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).Variables          == "Logger1Variables";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).Tags               == "Logger1Tags";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).Source             == "Logger1Source";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).EntryIndex         == 11;
    }
}

void LogDefLoggerConstructing()
{
    StaticLogger::DefaultLoggerPackage dataPackage("FilePackage");
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).CollectionString   = "PackageCollectionString";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).TempString         = "PackageTempString";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).Messages           = "PackageMessages";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).Variables          = "PackageVariables";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).Tags               = "PackageTags";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).Source             = "PackageSource";
    StaticLogger::StaticLogTesting::GetDefPackageTester(dataPackage).EntryIndex         = 33;


    StaticLogger::DefaultLogger logWriter1("File1");
    StaticLogger::DefaultLogger logWriter2("File2");

    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).CollectionString   = "Logger1CollectionString";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).TempString         = "Logger1TempString";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).Messages           = "Logger1Messages";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).Variables          = "Logger1Variables";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).Tags               = "Logger1Tags";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).Source             = "Logger1Source";
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter1).EntryIndex         = 11;


    {
        StaticLogger::DefaultLogger fromWriter(std::move(logWriter1));
        StaticLogger::DefaultLogger fromPackage(std::move(dataPackage));

        bool trueFalse = false;
        (void)trueFalse;

        std::cout << StaticLogger::StaticLogTesting::GetDefLoggerTester(fromPackage).CollectionString << std::endl;
        std::cout << StaticLogger::StaticLogTesting::GetDefLoggerTester(fromPackage).TempString << std::endl;
        std::cout << StaticLogger::StaticLogTesting::GetDefLoggerTester(fromPackage).Messages << std::endl;
        std::cout << StaticLogger::StaticLogTesting::GetDefLoggerTester(fromPackage).Variables << std::endl;
        std::cout << StaticLogger::StaticLogTesting::GetDefLoggerTester(fromPackage).Tags << std::endl;
        std::cout << StaticLogger::StaticLogTesting::GetDefLoggerTester(fromPackage).Source << std::endl;
        std::cout << StaticLogger::StaticLogTesting::GetDefLoggerTester(fromPackage).EntryIndex << std::endl;

        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).CollectionString   == "Logger1CollectionString";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).TempString         == "Logger1TempString";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).Messages           == "Logger1Messages";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).Variables          == "Logger1Variables";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).Tags               == "Logger1Tags";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).Source             == "Logger1Source";
        trueFalse = StaticLogger::StaticLogTesting::GetDefLoggerTester(fromWriter).EntryIndex         == 11;
    }
    //StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter2).UnravelAndAddVariables("var1", "value", "var2", 32, "var3", static_cast<void POINTER>(0x0));
    StaticLogger::StaticLogTesting::GetDefLoggerTester(logWriter2).UnravelAndAddMsgs("Msg1", "Msg2", "Msg3", "Msg4");

    StaticLogger::StaticLogTesting::TestingLoggerConstruct logger;
    logger.WriteMsgs<3>();
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
    constexpr CompileTimeString<32> str = CompileTimeString<32>(true);
    //constexpr bool isConstexpr = ConvertToString<32>(1358);
    CompileTimeString<6> converted = ConvertToStringRun<6>(-1358);
    converted = ConvertToStringRun<6>(1358);

    std::cout << Convert<123456>::Value << std::endl;
    std::cout << Convert<-123456>::Value << std::endl;
}

void RunTemp()
{
    //FmtTesting();
    //StaticLoggerInterpretTesting();
    //DefaultLogWritterPackageTesting();
    //LoggerCopyTesting();
    //LogDefLoggerTesting();
    //LogDefLoggerConstructing();
    //UnorderedArgumentChainTesting();
    ConstexprNumConversion();
}

#endif // TEMP_H
