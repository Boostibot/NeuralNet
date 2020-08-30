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
    ::StaticLogger::StaticLogTesting::DefaultLogWiriterPackageTester Tester1("My file");

    Tester1.CollectionString = "Collection string";
    Tester1.TempString = "Temp string";
    Tester1.Messages = "Messages";
    Tester1.Variables = "Variables";
    Tester1.Tags = "Tags";
    Tester1.Source = "Source";
    Tester1.EntryIndex = 1;

    ::StaticLogger::StaticLogTesting::DefaultLogWiriterPackageTester Tester2(std::move(Tester1));
}


void LoggerCopyTesting()
{
    ::StaticLogger::Logger<::StaticLogger::StaticLogTesting::TestingLoggerCopy> logWriterCopy1(11, "Copy1");
    logWriterCopy1.DoLog(false);
    logWriterCopy1.DoLoggingLevels<1, 3>(false);

    ::StaticLogger::Logger<::StaticLogger::StaticLogTesting::TestingLoggerCopy> copyConstructed(logWriterCopy1);

    ::StaticLogger::Logger<::StaticLogger::StaticLogTesting::TestingLoggerMove> logWriterMove1(11, "Move1");
    logWriterMove1.DoLog(false);
    logWriterMove1.DoLoggingLevels<1, 3>(false);
    ::StaticLogger::Logger<::StaticLogger::StaticLogTesting::TestingLoggerMove> logWriterMove2(std::move(logWriterMove1));

    StaticLogger::StaticLogTesting::DefaultLogWiriterTester logWriterTest;
    int a = 0;
}

void RunTemp()
{
    //FmtTesting();
    //StaticLoggerInterpretTesting();
    //DefaultLogWritterPackageTesting();
    LoggerCopyTesting();
}

#endif // TEMP_H
