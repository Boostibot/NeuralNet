#ifndef UNSAFEFILETETSING_H
#define UNSAFEFILETETSING_H

#include <array>
#include "General/File/UnsafeFile.h"
#include "CFileManager.h"
#include <iostream>

namespace CIo::UnsafeFileTesting
{
    template<typename CharT>
    using BasicFilenameCleaner = CFileManagerTesting::BasicFilenameCleaner<CharT>;

    template<typename T>
    constexpr BasicOpenMode<T> defaultModeTmpl = CFileManagerTesting::defaultModeTmpl<T>;
    static_assert (defaultModeTmpl<char8>.GetOpenModeString() == OpenMode::OpenModeString("w+b"),   "UNEXPECTED");
    static_assert (defaultModeTmpl<charW>.GetOpenModeString() == WOpenMode::OpenModeString(L"w+b"), "UNEXPECTED");

    template<typename CharT>
    struct BasicMockFile
    {
            BasicFilenameCleaner<CharT> Cleaner;

            BasicMockFile() = default;
            ~BasicMockFile() = default;

            template<typename FunctionType>
            inline bool ApplyToFile(FunctionType REF lambda)
            {
                if(Cleaner.IsOk() == false)
                    return false;

                BasicCFileManager<CharT> manager(Filename(), defaultModeTmpl<CharT>);
                if(manager.IsClosed())
                    return false;

                return lambda(manager);
            }

            inline auto Filename() const noexcept {return Cleaner.Filename();}
            inline auto IsOk()     const noexcept {return Cleaner.IsOk();}
    };

    #define UnsafeFile_TEST_COMPILE_ERROR false
}

enum class StringIOError
{
    NoError,
    Reading,
    Writing,
    Comapring,
    Opening,
};

CATCH_REGISTER_ENUM(StringIOError, StringIOError::NoError, StringIOError::Reading, StringIOError::Writing, StringIOError::Comapring, StringIOError::Opening);

namespace CIo::UnsafeFileTesting
{

    TEST_CASE("[UnsafeFile] : Only char and wchar_t specialisations should compile", "[UnsafeFile]")
    {
        [[maybe_unused]] BasicUnsafeFile<char8> file8;
        [[maybe_unused]] BasicUnsafeFile<charW> fileW;

        #if CFileManager_TEST_COMPILE_ERROR == true
        //Should not compile
        [[maybe_unused]] BasicUnsafeFile<int> fileInt;
        [[maybe_unused]] BasicUnsafeFile<double> fileDouble;
        [[maybe_unused]] BasicUnsafeFile<short> fileShort;
        [[maybe_unused]] BasicUnsafeFile<long> fileLing;
        [[maybe_unused]] BasicUnsafeFile<std::string> fileString;
        [[maybe_unused]] BasicUnsafeFile<std::wstring> fileWString;
        #endif
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Constructor should construct a closed file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const UnsafeFile file;

        REQUIRE(file.IsClosed() == true);
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Constructing a file with filename and OpenModeFlags should open the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using MockFile   = BasicMockFile<TestType>;
        using OpenMode   = BasicOpenMode<TestType>;
        constexpr OpenMode defaultOpenMode = defaultModeTmpl<TestType>;

        const MockFile mock;
        REQUIRE(mock.IsOk() == true);

        //Creates a new mock that manages the same generated
        // filename and destructs it
        // This leaves us with an unique filename in the first mock that is guaranteed to be deleted
        {
            const MockFile mockTemp(mock);
        }

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("provided with COpenMode::ReadMustExist equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist);
                REQUIRE(file.IsOpen() == false);
            }
            WHEN("provided with COpenMode::Write equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenModeFlag::Write);
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::WriteAppend equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenModeFlag::Append,  OpenModeFlag::Binary, OpenModeFlag::Write);
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::ReadWriteMustExist equivalent")
            {
                {
                    const UnsafeFile creator(mock.Filename(), OpenModeFlag::Write);
                }

                const UnsafeFile file(mock.Filename(), OpenModeFlag::Write,  OpenModeFlag::Binary, OpenModeFlag::MustExist, OpenModeFlag::Read);
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::ReadWrite equivalent")
            {
                {
                    const UnsafeFile creator(mock.Filename(), OpenModeFlag::Write);
                }
                const UnsafeFile file(mock.Filename(), OpenModeFlag::Read,  OpenModeFlag::MustNotExist, OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(file.IsOpen() == false);
            }
            WHEN("provided with COpenMode::ReadWriteAppend equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);
                REQUIRE(file.IsOpen() == true);
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const UnsafeFile file(mock.Filename());
                REQUIRE(file.IsOpen() == false);
                const UnsafeFile file2(mock.Filename(), OpenModeFlag::Append);
                REQUIRE(file2.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const UnsafeFile file(mock.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
                REQUIRE(file.IsOpen() == false);
            }
        }

        WHEN("A infvalid filename is passed into the constructor no file is opened")
        {
            typename UnsafeFile::OsStringView filename;

            const UnsafeFile file1(filename, OpenModeFlag::MustExist);
            const UnsafeFile file2(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
            const UnsafeFile file3(filename, OpenModeFlag::Read, OpenModeFlag::MustExist);
            const UnsafeFile file4(filename, OpenModeFlag::Write);
            const UnsafeFile file5(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);
            const UnsafeFile file6(filename, OpenModeFlag::Write, OpenModeFlag::MustNotExist);
            const UnsafeFile file7(filename, OpenModeFlag::Binary, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(file3.IsOpen() == false);
            REQUIRE(file4.IsOpen() == false);
            REQUIRE(file5.IsOpen() == false);
            REQUIRE(file6.IsOpen() == false);
            REQUIRE(file7.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Constructing a file with filename and OpenMode should open the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using MockFile   = BasicMockFile<TestType>;
        using OpenMode   = BasicOpenMode<TestType>;
        constexpr OpenMode defaultOpenMode = defaultModeTmpl<TestType>;

        constexpr OpenMode invalidOpenMode;
        constexpr OpenMode unsupportedOpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
        static_assert (invalidOpenMode.IsValid() == false, "!");
        static_assert (unsupportedOpenMode.IsSupported() == false, "!");

        const MockFile mock;
        REQUIRE(mock.IsOk() == true);

        //Creates a new mock that manages the same generated
        // filename and destructs it
        // This leaves us with an unique filename in the first mock that is guaranteed to be deleted
        {
            const MockFile mockTemp(mock);
        }

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("provided with COpenMode::ReadMustExist equivalent")
            {
                {
                    const UnsafeFile creator(mock.Filename(), OpenMode(OpenModeFlag::Write, OpenModeFlag::Binary));
                }

                const UnsafeFile file(mock.Filename(), OpenMode(OpenModeFlag::MustExist, OpenModeFlag::Read));
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::Write equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenMode(OpenModeFlag::MustNotExist,  OpenModeFlag::Binary, OpenModeFlag::Write));
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::WriteAppend equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenMode(OpenModeFlag::Append, OpenModeFlag::Write));
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::ReadWriteMustExist equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenMode(OpenModeFlag::MustExist, OpenModeFlag::Read, OpenModeFlag::Write));
                REQUIRE(file.IsOpen() == false);
            }
            WHEN("provided with COpenMode::ReadWrite equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenMode(COpenMode::ReadWrite));
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::ReadWriteAppend equivalent")
            {
                const UnsafeFile file(mock.Filename(), OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Binary, OpenModeFlag::Append));
                REQUIRE(file.IsOpen() == true);
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const UnsafeFile file(mock.Filename(), invalidOpenMode);

                REQUIRE(file.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const UnsafeFile file(mock.Filename(), unsupportedOpenMode);

                REQUIRE(file.IsOpen() == false);
            }
        }

        WHEN("A infvalid filename is passed into the constructor no file is opened")
        {
            typename UnsafeFile::OsStringView filename;

            const UnsafeFile file1(filename, invalidOpenMode);
            const UnsafeFile file2(filename, unsupportedOpenMode);
            const UnsafeFile file3(filename, OpenMode(OpenModeFlag::Write, OpenModeFlag::Append));
            const UnsafeFile file4(filename, OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist));
            const UnsafeFile file5(filename, OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Binary));
            const UnsafeFile file6(filename, OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist));
            const UnsafeFile file7(filename, OpenMode(OpenModeFlag::MustNotExist, OpenModeFlag::Read, OpenModeFlag::Binary, OpenModeFlag::Write));

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(file3.IsOpen() == false);
            REQUIRE(file4.IsOpen() == false);
            REQUIRE(file5.IsOpen() == false);
            REQUIRE(file6.IsOpen() == false);
            REQUIRE(file7.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Default constructor should construct a closed file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const UnsafeFile file;
        REQUIRE(file.IsClosed() == true);
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : File should not be copy constructible", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;

        REQUIRE(std::is_copy_constructible_v<UnsafeFile> == false);
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Move constructor should transfer the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using MockFile   = BasicMockFile<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const MockFile mock;
        REQUIRE(mock.IsOk() == true);

        WHEN("Move constructing a opened file, the file should transfer")
        {
            UnsafeFile file(mock.Filename(), defaultOpenMode);
            const UnsafeFile constructed = std::move(file);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(constructed.IsOpen() == true);
        }

        WHEN("Move constructing a closed file, both files should be closed")
        {
            UnsafeFile file;
            const UnsafeFile constructed = std::move(file);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(constructed.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : File should not be copy assignable", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;

        REQUIRE(std::is_copy_assignable_v<UnsafeFile> == false);
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Move assigning should transfer the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using MockFile   = BasicMockFile<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const MockFile mock;
        REQUIRE(mock.IsOk() == true);

        WHEN("A file is open move assigning from it transfer the file")
        {
            UnsafeFile file(mock.Filename(), defaultOpenMode);
            UnsafeFile assigned;

            assigned = std::move(file);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(assigned.IsOpen() == true);
        }

        WHEN("A file is open move assigning to it closed file closes the file")
        {
            UnsafeFile file(mock.Filename(), defaultOpenMode);
            UnsafeFile assigned;

            file = std::move(assigned);


            REQUIRE(file.IsOpen() == false);
            REQUIRE(assigned.IsOpen() == false);
        }

        WHEN("A file is open move assigning to it opened file transfers it")
        {
            const MockFile mock2;
            REQUIRE(mock2.IsOk() == true);

            {
                UnsafeFile file(mock.Filename(), defaultOpenMode);
                UnsafeFile assigned(mock2.Filename(), defaultOpenMode);

                file = std::move(assigned);

                REQUIRE(file.IsOpen() == true);
                REQUIRE(assigned.IsOpen() == false);
            }
        }

        WHEN("A file is not open move assigning from it leaves both unopen")
        {
            UnsafeFile file;
            UnsafeFile assigned;

            assigned = std::move(file);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(assigned.IsOpen() == false);
        }

        WHEN("Move assigning a file to itself should not change its state")
        {
            UnsafeFile file1(mock.Filename(), defaultOpenMode);
            UnsafeFile file2;

            file1  = std::move(file1);
            file2  = std::move(file2);

            REQUIRE(file1.IsOpen() == true);
            REQUIRE(file2.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Mocking use", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using MockFile   = BasicMockFile<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        MockFile mock;

        auto fileChange = [](auto& cFileManager)
        {
            int myData[10] = {};
            myData[0] = 100;
            myData[3] = 100;
            myData[9] = 100;
            return fwrite(myData, sizeof (int), 10, cFileManager.FilePtr);
        };

        //Mockign should not fail
        REQUIRE(mock.ApplyToFile(fileChange) == true);

        const UnsafeFile file(mock.Filename(), defaultOpenMode);
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Read should read data from the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t data0Size = 0;
        constexpr size_t data1Size = 10;
        constexpr size_t data2Size = 20;
        const std::array<int, 1> data0 = {0};
        const std::array<double, data1Size> data1 = {21.2, -0.32, 67.666663};
        const std::array<long, data2Size> data2 = {0, 426, 572734, 2211, -523, 1, 5, 9, -4211};

        std::array<int, 1> buffer0 = {0};
        std::array<double, data1Size> buffer1;
        std::array<long, data2Size> buffer2;

        WHEN("reading data from a valid file they should match the ones written")
        {
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.Write(data0.data(), data0Size) == true);
                REQUIRE(writer.Write(data1.data(), data1Size) == true);
                REQUIRE(writer.Write(data2.data(), data2Size) == true);
            }

            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.Read(buffer0.data(), data0Size) == true);
            REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
            REQUIRE(reader.Read(buffer2.data(), data2Size) == true);

            REQUIRE(buffer0 == data0);
            REQUIRE(buffer1 == data1);
            REQUIRE(buffer2 == data2);
        }
        WHEN("reading data from a invalid file (open for write only) no data is read")
        {
            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.Read(buffer0.data(), data0Size) == true);
            REQUIRE(reader.Read(buffer1.data(), data1Size) == false);
            REQUIRE(reader.Read(buffer2.data(), data2Size) == false);
        }

    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : ReadAndCount should read data from the file and return the ammount of data read", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t data0Size = 0;
        constexpr size_t data1Size = 18;
        constexpr size_t data2Size = 120;
        const std::array<double, 1> data0 = {0};
        const std::array<float, data1Size> data1 = {1.8884f, -0.46f, 88241.0f};
        const std::array<char, data2Size> data2 = {0, 25, -64, -51, 123, 1, 5, 9, -100};

        std::array<double, 1> buffer0 = {0};
        std::array<float, data1Size> buffer1;
        std::array<char, data2Size> buffer2;


        WHEN("reading data from a valid file they should match the ones written")
        {
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.WriteAndCount(data0.data(), data0Size) == data0Size);
                REQUIRE(writer.WriteAndCount(data1.data(), data1Size) == data1Size);
                REQUIRE(writer.WriteAndCount(data2.data(), data2Size) == data2Size);
            }

            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadAndCount(buffer0.data(), data0Size) == data0Size);
            REQUIRE(reader.ReadAndCount(buffer1.data(), data1Size) == data1Size);
            REQUIRE(reader.ReadAndCount(buffer2.data(), data2Size) == data2Size);

            REQUIRE(buffer0 == data0);
            REQUIRE(buffer1 == data1);
            REQUIRE(buffer2 == data2);
        }
        WHEN("reading data from a invalid file (open for write only) no data is read")
        {
            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadAndCount(buffer0.data(), data0Size) == data0Size);
            REQUIRE(reader.ReadAndCount(buffer1.data(), data1Size) != data1Size);
            REQUIRE(reader.ReadAndCount(buffer2.data(), data2Size) != data2Size);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : ReadObject should read the object data from the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        const struct Obj0 {}                                                  obj0;
        const struct Obj1 {float m1 = 123.0f;}                                  obj1;
        const struct Obj2 {long m1 = -12454; u8 m2 = 222;}                    obj2;
        const struct Obj3 {double m1 = 0.5; double m2 = 0.333333; short m3 = 10;} obj3;

        Obj0 read0;
        Obj1 read1;
        Obj2 read2;
        Obj3 read3;

        WHEN("Writing to a valid file the data is written and can be read back")
        {
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.WriteObject(obj0) == true);
                REQUIRE(writer.WriteObject(obj1) == true);
                REQUIRE(writer.WriteObject(obj2) == true);
                REQUIRE(writer.WriteObject(obj3) == true);
            }

            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadObject(read0) == true);
            REQUIRE(reader.ReadObject(read1) == true);
            REQUIRE(reader.ReadObject(read2) == true);
            REQUIRE(reader.ReadObject(read3) == true);

            CHECK(read1.m1 == obj1.m1);

            CHECK(read2.m1 == obj2.m1);
            CHECK(read2.m2 == obj2.m2);

            CHECK(read3.m1 == obj3.m1);
            CHECK(read3.m2 == obj3.m2);
            CHECK(read3.m3 == obj3.m3);
        }
        WHEN("reading data from a invalid file (open for write only) no data is read")
        {
            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadObject(read0) == false); //Even though has no members has size of > 0
            REQUIRE(reader.ReadObject(read1) == false);
            REQUIRE(reader.ReadObject(read2) == false);
            REQUIRE(reader.ReadObject(read3) == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : ReadString should read data from the file into the string argument", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        constexpr size_t maxStrSize = 50;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr std::string_view data0 = "";
        constexpr std::string_view data1 = u8"abcdex1234";
        constexpr std::wstring_view data2 = L"abcdex1234";
        constexpr std::u16string_view data3 = u"abcdex1234";
        constexpr std::u32string_view data4 = U"abcdex1234";

        char8 buffer0[1]; //Array cannot be of size 0
        char8 buffer1[data1.size()];
        charW buffer2[data2.size()];
        char16 buffer3[data3.size()];
        char32 buffer4[data4.size()];

        const CStringRef<char8> ref0(buffer0, data0.size());
        const CStringRef<char8> ref1(buffer1, data1.size());
        const CStringRef<charW> ref2(buffer2, data2.size());
        const CStringRef<char16> ref3(buffer3, data3.size());
        const CStringRef<char32> ref4(buffer4, data4.size());

        WHEN("reading data from a valid file they should match the ones written")
        {
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                REQUIRE(writer.WriteString(data0) == true);
                REQUIRE(writer.WriteString(data1) == true);
                REQUIRE(writer.WriteString(data2) == true);
                REQUIRE(writer.WriteString(data3) == true);
                REQUIRE(writer.WriteString(data4) == true);
            }

            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadString(ref0) == true);
            REQUIRE(reader.ReadString(ref1) == true);
            REQUIRE(reader.ReadString(ref2) == true);
            REQUIRE(reader.ReadString(ref3) == true);
            REQUIRE(reader.ReadString(ref4) == true);

            REQUIRE(data0 == ref0);
            REQUIRE(data1 == ref1);
            REQUIRE(data2 == ref2);
            REQUIRE(data3 == ref3);
            REQUIRE(data4 == ref4);
        }
        WHEN("reading data from a invalid file (open for write only) no data is read")
        {
            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadString(ref0) == true);
            REQUIRE(reader.ReadString(ref1) == false);
            REQUIRE(reader.ReadString(ref2) == false);
            REQUIRE(reader.ReadString(ref3) == false);
            REQUIRE(reader.ReadString(ref4) == false);
        }

    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Write should put the appropriate data in the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        constexpr size_t data0Size = 0;
        constexpr size_t data1Size = 41;
        constexpr size_t data2Size = 130;
        const std::array<int, 1> data0 = {0};
        const std::array<char32, data1Size> data1 = {21, 89, 6521};
        const std::array<bool, data2Size> data2 = {true, true, true, false, true, false, false, true};

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Writing to a valid file the data is written and can be read back")
        {
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.Write(data0.data(), data0Size) == true);
                REQUIRE(writer.Write(data1.data(), data1Size) == true);
                REQUIRE(writer.Write(data2.data(), data2Size) == true);
            }

            std::array<int, 1> buffer0 = {0};
            std::array<char32, data1Size> buffer1;
            std::array<bool, data2Size> buffer2;
            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.Read(buffer0.data(), data0Size) == true);
            REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
            REQUIRE(reader.Read(buffer2.data(), data2Size) == true);

            REQUIRE(buffer0 == data0);
            REQUIRE(buffer1 == data1);
            REQUIRE(buffer2 == data2);
        }
        WHEN("Writing to an invalid file (open for reading only) no data is written")
        {
            {
                const UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write);
            }

            UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(writer.IsOpen() == true);

            REQUIRE(writer.Write(data0.data(), data0Size) == true);
            REQUIRE(writer.Write(data1.data(), data1Size) == false);
            REQUIRE(writer.Write(data2.data(), data2Size) == false);
        }

    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : WriteAndCount should put the appropriate data in the file and return the ammount of data written", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        constexpr size_t data0Size = 0;
        constexpr size_t data1Size = 3;
        constexpr size_t data2Size = 91;
        const std::array<double, 1> data0 = {0};
        const std::array<short, data1Size> data1 = {10, 89, 872,};
        const std::array<int, data2Size> data2 = {0, 89, -32021, 18999, -322};

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Writing to a valid file the data is written and can be read back")
        {
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.WriteAndCount(data0.data(), data0Size) == data0Size);
                REQUIRE(writer.WriteAndCount(data1.data(), data1Size) == data1Size);
                REQUIRE(writer.WriteAndCount(data2.data(), data2Size) == data2Size);
            }

            std::array<double, 1> buffer0 = {0};
            std::array<short, data1Size> buffer1;
            std::array<int, data2Size> buffer2;
            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadAndCount(buffer0.data(), data0Size) == data0Size);
            REQUIRE(reader.ReadAndCount(buffer1.data(), data1Size) == data1Size);
            REQUIRE(reader.ReadAndCount(buffer2.data(), data2Size) == data2Size);

            REQUIRE(buffer0 == data0);
            REQUIRE(buffer1 == data1);
            REQUIRE(buffer2 == data2);
        }
        WHEN("Writing to an invalid file (open for reading only) no data is written")
        {
            {
                const UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write);
            }

            UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(writer.IsOpen() == true);

            REQUIRE(writer.WriteAndCount(data0.data(), data0Size) == data0Size);
            REQUIRE(writer.WriteAndCount(data1.data(), data1Size) != data1Size);
            REQUIRE(writer.WriteAndCount(data2.data(), data2Size) != data2Size);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : WriteObject should put the whole object in the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const struct Obj0 {}                                                  obj0;
        const struct Obj1 {char8 m1 = 123;}                                   obj1;
        const struct Obj2 {int m1 = -12454; int m2 = 8899;}                   obj2;
        const struct Obj3 {double m1 = 0.5; double m2 = -0.003; int m3 = 10;} obj3;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Writing to a valid file the data is written and can be read back")
        {
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.WriteObject(obj0) == true);
                REQUIRE(writer.WriteObject(obj1) == true);
                REQUIRE(writer.WriteObject(obj2) == true);
                REQUIRE(writer.WriteObject(obj3) == true);
            }

            Obj0 read0;
            Obj1 read1;
            Obj2 read2;
            Obj3 read3;
            UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadObject(read0) == true);
            REQUIRE(reader.ReadObject(read1) == true);
            REQUIRE(reader.ReadObject(read2) == true);
            REQUIRE(reader.ReadObject(read3) == true);

            CHECK(read1.m1 == obj1.m1);

            CHECK(read2.m1 == obj2.m1);
            CHECK(read2.m2 == obj2.m2);

            CHECK(read3.m1 == obj3.m1);
            CHECK(read3.m2 == obj3.m2);
            CHECK(read3.m3 == obj3.m3);
        }
        WHEN("Writing to an invalid file (open for reading only) no data is written")
        {
            {
                const UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write);
            }

            UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(writer.IsOpen() == true);

            REQUIRE(writer.WriteObject(obj1) == false);
            REQUIRE(writer.WriteObject(obj2) == false);
            REQUIRE(writer.WriteObject(obj3) == false);
        }
    }

    template<typename StringType, typename OsCharT, size_t maxStrSize = 50>
    StringIOError TestWritingAnyStringInvalidMode(StringType RVALUE_REF str, const OsCharT * filaneme)
    {
        using UnsafeFile = BasicUnsafeFile<OsCharT>;
        {
            const UnsafeFile creator(filaneme, OpenModeFlag::Write);
        }

        UnsafeFile writer(filaneme, OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
        if(writer.IsOpen() == false)
            return StringIOError::Opening;

        if(writer.WriteString(str) == false)
            return StringIOError::Writing;

        return StringIOError::NoError;
    }

    template<typename InputCharT, typename StringType, typename OsCharT, size_t maxStrSize = 50>
    StringIOError TestWritingAnyStringValidMode(StringType RVALUE_REF str, const OsCharT * filaneme)
    {
        using UnsafeFile = BasicUnsafeFile<OsCharT>;

        const std::basic_string_view<InputCharT> view = str;

        {
            UnsafeFile writer(filaneme, OpenModeFlag::Write, OpenModeFlag::Binary);
            if(writer.IsOpen() == false)
                return StringIOError::Opening;

            if(writer.WriteString(str) == false)
                return StringIOError::Writing;
        }

        char8 read1[maxStrSize];

        UnsafeFile reader(filaneme, OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
        if(reader.IsOpen() == false)
            return StringIOError::Opening;

        if(reader.ReadString(ToCStringRef(read1, view.size())) == false)
                return StringIOError::Reading;

        if(view != str)
            return StringIOError::Comapring;

        return StringIOError::NoError;
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : WriteString should put the contents of the provided string of any type in the file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;

        constexpr size_t maxStrSize = 50;
        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("provided with C strings they should be written as expected")
        {
            const char8 * str1 = "";
            const char8 * str2 = "123456abcd";
            const charW * str3 = L"123456abcd";
            const char16 * str4 = u"xyzw123";
            const char32 * str5 = U"xyzw123";

            WHEN("Writing to a valid file the data is written and can be read back")
            {
                REQUIRE(TestWritingAnyStringValidMode<char8>(str1, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char8>(str2, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<charW>(str3, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char16>(str4, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char32>(str5, cleaner.Filename()) == StringIOError::NoError);
            }

            WHEN("Writing to an invalid file (open for reading only) no data is written")
            {
                REQUIRE(TestWritingAnyStringInvalidMode(str1, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringInvalidMode(str2, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str3, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str4, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str5, cleaner.Filename()) == StringIOError::Writing);
            }
        }

        WHEN("provided with string_views they should be written as expected")
        {
            const std::string_view str1 = "";
            const std::string_view str2 = u8"123456abcd";
            const std::wstring_view str3 = L"123456abcd";
            const std::u16string_view str4 = u"xyzw123";
            const std::u32string_view str5 = U"xyzw123";

            WHEN("Writing to a valid file the data is written and can be read back")
            {
                REQUIRE(TestWritingAnyStringValidMode<char8>(str1, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char8>(str2, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<charW>(str3, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char16>(str4, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char32>(str5, cleaner.Filename()) == StringIOError::NoError);
            }

            WHEN("Writing to an invalid file (open for reading only) no data is written")
            {
                REQUIRE(TestWritingAnyStringInvalidMode(str1, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringInvalidMode(str2, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str3, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str4, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str5, cleaner.Filename()) == StringIOError::Writing);
            }
        }

        WHEN("provided with strings they should be written as expected")
        {
            const std::string str1 = "";
            const std::string str2 = "123456abcd";
            const std::wstring str3 = L"123456abcd";
            const std::u16string str4 = u"xyzw123";
            const std::u32string str5 = U"xyzw123";

            WHEN("Writing to a valid file the data is written and can be read back")
            {
                REQUIRE(TestWritingAnyStringValidMode<char8>(str1, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char8>(str2, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<charW>(str3, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char16>(str4, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char32>(str5, cleaner.Filename()) == StringIOError::NoError);
            }

            WHEN("Writing to an invalid file (open for reading only) no data is written")
            {
                REQUIRE(TestWritingAnyStringInvalidMode(str1, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringInvalidMode(str2, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str3, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str4, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str5, cleaner.Filename()) == StringIOError::Writing);
            }
        }

        WHEN("provided with CStringRefs they should be written as expected")
        {
            std::string data1 = "";
            std::string data2 = "123456abcd";
            std::wstring data3 = L"123456abcd";
            std::u16string data4 = u"xyzw123";
            std::u32string data5 = U"xyzw123";

            const CStringRef<char8> str1 = data1;
            const CStringRef<char8> str2 = data2;
            const CStringRef<charW> str3 = data3;
            const CStringRef<char16> str4 = data4;
            const CStringRef<char32> str5 = data5;

            WHEN("Writing to a valid file the data is written and can be read back")
            {
                REQUIRE(TestWritingAnyStringValidMode<char8>(str1, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char8>(str2, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<charW>(str3, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char16>(str4, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char32>(str5, cleaner.Filename()) == StringIOError::NoError);
            }

            WHEN("Writing to an invalid file (open for reading only) no data is written")
            {
                REQUIRE(TestWritingAnyStringInvalidMode(str1, cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringInvalidMode(str2, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str3, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str4, cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(str5, cleaner.Filename()) == StringIOError::Writing);
            }
        }

        WHEN("provided with strings literals they should be written as expected")
        {
            WHEN("Writing to a valid file the data is written and can be read back")
            {
                REQUIRE(TestWritingAnyStringValidMode<char8>("", cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char8>("123456abcd", cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<charW>(L"123456abcd", cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char16>(u"xyzw123", cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringValidMode<char32>(U"xyzw123", cleaner.Filename()) == StringIOError::NoError);
            }

            WHEN("Writing to an invalid file (open for reading only) no data is written")
            {
                REQUIRE(TestWritingAnyStringInvalidMode("", cleaner.Filename()) == StringIOError::NoError);
                REQUIRE(TestWritingAnyStringInvalidMode("123456abcd", cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(L"123456abcd", cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(u"xyzw123", cleaner.Filename()) == StringIOError::Writing);
                REQUIRE(TestWritingAnyStringInvalidMode(U"xyzw123", cleaner.Filename()) == StringIOError::Writing);
            }
        }
    }

}





















































#endif // UNSAFEFILETETSING_H
