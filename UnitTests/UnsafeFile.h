#ifndef UNSAFEFILETETSING_H
#define UNSAFEFILETETSING_H
#include "General/File/UnsafeFile.h"
#include "CFileManager.h"
#include <fstream>
namespace CIo::UnsafeFileTesting
{
    template<typename CharT>
    using BasicFilenameCleaner = CFileManagerTesting::BasicFilenameCleaner<CharT>;

    constexpr OpenMode defaultOpenMode(OpenMode::OpenModeFlag::Read, OpenMode::OpenModeFlag::Write, OpenMode::OpenModeFlag::Binary);
    static_assert (defaultOpenMode.IsValid(),  "INVALID");

    template<typename CharT>
    struct BasicMockFile
    {
            BasicFilenameCleaner<CharT> Cleaner;

            BasicMockFile() = default;
            ~BasicMockFile() = default;

            template<typename FunctionType>
            inline bool ApplyToFile(FunctionType REF lambda)
            {
                if(Cleaner.HasError())
                    return false;

                BasicCFileManager<CharT> manager(Filename(), defaultOpenMode);
                if(manager.IsClosed())
                    return false;

                return lambda(manager);
            }

            inline auto Filename() const noexcept {return Cleaner.Filename();}
            inline auto IsOk()     const noexcept {return Cleaner.IsOk();}
    };

    #define UnsafeFile_TEST_COMPILE_ERROR false
}
namespace CIo::UnsafeFileTesting
{
    TEST_CASE("Only char and wchar_t specialisations should compile", "[UnsafeFile]")
    {
        [[maybe_unused]] BasicUnsafeFile<char8> file8;
        [[maybe_unused]] BasicUnsafeFile<charW> fileW;

        #if CFileManager_TEST_COMPILE_ERROR == true
        [[maybe_unused]] Should not compile
        [[maybe_unused]] BasicUnsafeFile<int> fileInt;
        [[maybe_unused]] BasicUnsafeFile<double> fileDouble;
        [[maybe_unused]] BasicUnsafeFile<short> fileShort;
        [[maybe_unused]] BasicUnsafeFile<long> fileLing;
        [[maybe_unused]] BasicUnsafeFile<std::string> fileString;
        [[maybe_unused]] BasicUnsafeFile<std::wstring> fileWString;
        #endif
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

    TEMPLATE_TEST_CASE("[UnsafeFile] :  constructor should construct a closed file", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const UnsafeFile file;

        REQUIRE(file.IsClosed() == true);
    }
    TEMPLATE_TEST_CASE("[UnsafeFile] : Mocking use", "[UnsafeFile]", CFileManagerTestedTypes)
    {
        using MockFile   = BasicMockFile<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

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
}
#endif // UNSAFEFILETETSING_H
