#ifndef TEMP_H
#define TEMP_H

#include <iostream>
#include "General/Common/Common.h"
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

HEADER_ONLY void FmtTesting()
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

HEADER_ONLY void StaticLogInterpretTesting()
{

    std::string output;
    output.reserve(512);
    ::StaticLog::DefaultDataInterpret interpret;
    interpret.InterpretArg(0, output);
}

HEADER_ONLY void DefaultLogWritterPackageTesting()
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

HEADER_ONLY void LoggerCopyTesting()
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

HEADER_ONLY void LogDefLoggerTesting()
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

HEADER_ONLY void LogDefLoggerConstructing()
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

HEADER_ONLY void UnorderedArgumentChainTesting()
{
    UnorderedArgumentChain::Caller caller;
    caller.Function("Hello World").WithTags("Tag").WithSource("File");
}


HEADER_ONLY void StringAdditionSpeedTesting()
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

    HEADER_ONLY void Function1(ClassRef ref)
    {
        std::cout << ref.Data.PassCount << std::endl;
    }

    HEADER_ONLY void Function2(ClassRef ref)
    {
        std::cout << ref.Data.PassCount << std::endl;
        Function1(ref);
        std::cout << ref.Data.PassCount << std::endl;
    }

    HEADER_ONLY void Function3(ClassRef ref)
    {
        std::cout << ref.Data.PassCount << std::endl;
        Function2(ref);
        std::cout << ref.Data.PassCount << std::endl;
    }
}

HEADER_ONLY u32 Counter()
{
    static u32 counter = 0;
    u32 counterCopy = counter;

    counter++;

    return counterCopy;
}


HEADER_ONLY std::string CounterText()
{
    return std::to_string(Counter());
}

thread_local StaticLog::DefaultLogger<> Log("Temp" + CounterText() + ".txt");

HEADER_ONLY void GenericProgramingTests()
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

struct Block;

struct BlockHeader
{
        Block * nextBlock;
        Block * prevBlock;
        size_t size;
};

template<size_t size>
struct alignas(size) AlignedMemory {};

using ElemtT = int;

template <size_t size>
struct BlockTempl
{
        BlockHeader header;
        ElemtT data[size];
};

HEADER_ONLY void AllocatingBlocksTesting()
{
    //size_t dynamicSize = 0;
    //void * blockPtr =  new BlockTempl<dynamicSize>();
}

constexpr void AddPaddingSize(size_t INOUT totalSize, size_t IN align)
{
    const size_t modulo = totalSize % align;

    if(modulo != 0)
        totalSize += align - (modulo);
}

constexpr void AddSize(size_t INOUT totalSize, size_t IN size)
{
    totalSize += size;
}

template<typename T>
constexpr void AddMember(size_t INOUT totalSize)
{
    AddPaddingSize(totalSize, alignof (T));
    AddSize(totalSize, sizeof (T));
}


template<size_t size>
using Size = std::integral_constant<size_t, size>;

template<MetaPrograming::Indetifier>
constexpr size_t GetMaxInternal(size_t max)
{
    return max;
}

template<MetaPrograming::Indetifier, typename FirstArg, typename ... Args>
constexpr size_t GetMaxInternal(size_t curMax, FirstArg first, Args... args)
{
    if(first > curMax)
        curMax = first;
    return GetMaxInternal<MetaPrograming::Indetifier::Indentify, Args...>(curMax, args...);
}


template<bool error = true>
constexpr size_t GetMax()
{
    //static_assert (error, "No set provided; At least one parameter must be present");
    return 0;
}

template<typename FirstArg, typename ... Args>
constexpr size_t GetMax(FirstArg first, Args... args)
{
    return GetMaxInternal<MetaPrograming::Indetifier::Indentify, Args...>(first, args...);
}

template<typename ... MemberTypes>
constexpr size_t GetMaxSize()
{
    return GetMax(sizeof (MemberTypes)...);
}

template<typename ... MemberTypes>
constexpr size_t GetMaxAlignment()
{
    return GetMax(alignof (MemberTypes)...);
}

template<MetaPrograming::Indetifier>
constexpr void GetStructureSizeRecursive(size_t INOUT)
{}

template<MetaPrograming::Indetifier, typename FirstMemberType, typename ... MemberTypes>
constexpr void GetStructureSizeRecursive(size_t INOUT totalSize)
{
    AddMember<FirstMemberType>(totalSize);
    GetStructureSizeRecursive<MetaPrograming::Indetifier::Indentify, MemberTypes...>(totalSize);
}


template<typename ... MemberTypes>
constexpr size_t GetStructureSize()
{
    size_t returnSize = 0;
    //Add main size and padding
    GetStructureSizeRecursive<MetaPrograming::Indetifier::Indentify, MemberTypes...>(returnSize);

    //Add trailing padding
    constexpr size_t maxMemberSize = GetMaxAlignment<MemberTypes...>();
    AddPaddingSize(returnSize, maxMemberSize);

    return returnSize;
}

template<typename ... MemberTypes>
using GetSize = std::integral_constant<size_t, GetStructureSize<MemberTypes...>()>;


struct CustomSize
{
        int d1;
        int d2;
        int d3;
};

struct TestSize1
{
        int d1;
        double d2;
};



HEADER_ONLY void AllocatingBlocksTesting2()
{
    //alignas(BlockHeader) int a;
    //AlignedMemory<16> memory[106];
    //size_t padding = 0 /* Some way to calculcate the nrcessary padding*/;

    //void * allocated = new char[sizeof(BlockHeader) + padding +  sizeof (ElemtT)];


    //Block * blockPtr =  static_cast<Block *>(allocated);

    static_assert (std::is_same_v<GetSize<double, int>, Size<sizeof(TestSize1)>>, "");
}

HEADER_ONLY void PassingSupportTesting()
{
    PassingSupport::ClassData data;
    PassingSupport::ClassRef ref(data);
    ref->MyInt1 = 0;
    PassingSupport::Function3(data);
}


#include "General/File/File.h"
#include "General/File/UseExamples.h"

template<typename T, T... values>
void templFUnc()
{

}

HEADER_ONLY void OpenModeConstructortesting()
{
    using namespace CIo;
    using namespace OpenModeHelpers;
    FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Read);
    FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Read, OpenModeFlag::Write);
    FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Read, OpenModeFlag::Read);

    templFUnc<int, 1, 2, 3>();

    UnsafeFile file;
    file.IsOpen();
}


HEADER_ONLY void RunTemp()
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
    OpenModeConstructortesting();

}

#endif // TEMP_H
