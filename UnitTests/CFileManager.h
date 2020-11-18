#ifndef CFILEMANAGERTEST_H
#define CFILEMANAGERTEST_H
#include "Catch2/Catch.hpp"
#include "General/File/CFileManager.h"

namespace CIo::CFileManagerTesting
{
    template<typename CharT, size_t CharsMax = L_tmpnam>
    struct BasicFilenameCleaner
    {
            using CharSupport = CharTypeSupport<CharT>;

        private:
            bool State;
            CharT FilenameString[CharsMax];

        public:
            BasicFilenameCleaner()
            {
                State = (CharSupport::tmpnam_s(FilenameString, CharsMax) == 0);
            }
            BasicFilenameCleaner(const BasicFilenameCleaner REF) = default;
            ~BasicFilenameCleaner()
            {
                if(IsOk() == false)
                    return;

                //It is ok to remove file that does not exist
                // in that case the functions should do nothing
                CharSupport::remove(FilenameString);
            }
            inline auto Filename() const noexcept {return FilenameString;}
            inline bool IsOk()     const noexcept {return State; }

        public:
            BasicFilenameCleaner(BasicFilenameCleaner RVALUE_REF) = delete;
            BasicFilenameCleaner REF operator=(const BasicFilenameCleaner REF) = delete;
            BasicFilenameCleaner REF operator=(BasicFilenameCleaner RVALUE_REF) = delete;
    };

    constexpr OpenMode defaultOpenMode(OpenMode::OpenModeFlag::Read, OpenMode::OpenModeFlag::Write, OpenMode::OpenModeFlag::Binary);
    static_assert (defaultOpenMode.IsValid(),                                              "INVALID");
    static_assert (defaultOpenMode.IsSupported(),                                          "UNSUPPORTED");
    static_assert (defaultOpenMode.GetOpenModeString() == OpenMode::OpenModeString("w+b"), "UNEXPECTED");
}

#define CFileManager_TEST_COMPILE_ERROR false
#define CFileManagerTestedTypes char8, charW

namespace CIo::CFileManagerTesting
{
    TEST_CASE("[BasicCFileManager] : Only char8 amd charW (char and wchar_t) specialisations should compile", "[BasicCFileManager]")
    {
        BasicCFileManager<char8> file8;
        BasicCFileManager<charW> fileW;

        #if CFileManager_TEST_COMPILE_ERROR == true
        //Should not compile
        //BasicCFileManager<int> fileInt;
        //BasicCFileManager<double> fileD;
        //BasicCFileManager<short> fileShort;
        //BasicCFileManager<long> fileLing;
        //BasicCFileManager<std::string> fileString;
        //BasicCFileManager<std::wstring> fileWString;
        #endif
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : After construction file should be closed", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        const BasicCFileManager<TestType> file;
        REQUIRE(file.IsOpen() == false);
    }


    TEMPLATE_TEST_CASE("[BasicCFileManager] : Constructing a file with filename and OpenMode should open the file", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;

        constexpr OpenMode invalidOpenMode;
        constexpr OpenMode unsupportedOpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
        static_assert (invalidOpenMode.IsValid() == false, "!");
        static_assert (unsupportedOpenMode.IsSupported() == false, "!");

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("and a valid openMode is provided the file is opened")
            {
                const CFileManager file(cleaner.Filename(), defaultOpenMode);

                REQUIRE(file.IsOpen() == true);
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const CFileManager file(cleaner.Filename(), invalidOpenMode);

                REQUIRE(file.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const CFileManager file(cleaner.Filename(), unsupportedOpenMode);

                REQUIRE(file.IsOpen() == false);
            }
        }


        WHEN("A infvalid filename is passed into the constructor (and the mode requires valid file) no file is opened")
        {
            //Creates a new cleaner that manages the same generated
            // filename and destructs it
            // This leaves us with an unique filename in the first cleaner that is guaranteed to be deleted
            {
                const FilenameCleaner cleanerTemp(cleaner);
                REQUIRE(cleanerTemp.IsOk() == true);
            }

            WHEN("and a valid openMode is provided no file is opened")
            {
                constexpr OpenMode mustExistOpenMode(OpenMode::OpenModeFlag::Read, OpenMode::OpenModeFlag::MustExist, OpenMode::OpenModeFlag::Binary);
                static_assert (mustExistOpenMode.IsValid()   == true, "!");

                //Opens a deleetd unique filename that must exist
                const CFileManager file(cleaner.Filename(), mustExistOpenMode);

                REQUIRE(file.IsOpen() == false);
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const CFileManager file(cleaner.Filename(), invalidOpenMode);

                REQUIRE(file.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const CFileManager file(cleaner.Filename(), unsupportedOpenMode);

                REQUIRE(file.IsOpen() == false);
            }
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : ReOpening files", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is not yet opened OpenNew should leave it opened")
        {
            CFileManager file;

            //OpenNew should succeed
            REQUIRE(file.OpenNew(cleaner.Filename(), defaultOpenMode) == true);

            REQUIRE(file.IsOpen() == true);
        }

        WHEN("A file is opened using constructor OpenNew will keep it opened")
        {
            const FilenameCleaner cleanerNew;
            REQUIRE(cleaner.IsOk() == true);

            {
                CFileManager file(cleaner.Filename(), defaultOpenMode);

                //Constructor should not fail
                REQUIRE(file.IsOpen() == true);

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleanerNew.Filename(), defaultOpenMode) == true);

                REQUIRE(file.IsOpen() == true);
            }
        }

        WHEN("A file is opened using OpenNew, OpenNew will keep it opened")
        {
            const FilenameCleaner cleanerNew;
            REQUIRE(cleaner.IsOk() == true);

            {
                CFileManager file;

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleaner.Filename(), defaultOpenMode) == true);

                //Constructor should not fail
                REQUIRE(file.IsOpen() == true);

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleanerNew.Filename(), defaultOpenMode) == true);

                REQUIRE(file.IsOpen() == true);
            }
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : IsOpen and IsClosed should tell if the file is opened or closed", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file was never opened it should be closed")
        {
            const CFileManager file;

            REQUIRE(file.IsOpen() == false);

            REQUIRE(file.IsClosed() == true);

            //Calling the function should not chanage the state
            REQUIRE(file.IsOpen() == false);
        }

        WHEN("A file is opened in the constructor it is opened")
        {
            const CFileManager file(cleaner.Filename(), defaultOpenMode);

            //It should be declared open
            REQUIRE(file.IsOpen() == true);

            REQUIRE(file.IsClosed() == false);

            //Calling the function should not chanage the state
            REQUIRE(file.IsOpen() == true);
        }

        WHEN("A file is opened using OpenNew it is opened")
        {
            CFileManager file;

            REQUIRE(file.OpenNew(cleaner.Filename(), defaultOpenMode) == true);

            //It should be declared open
            REQUIRE(file.IsOpen() == true);

            REQUIRE(file.IsClosed() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : File should not be copy constructible", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;

        REQUIRE(std::is_copy_constructible_v<CFileManager> == false);
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : Move constructing should transfer the file", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is open move constructing from it transfer the file")
        {
            CFileManager file1(cleaner.Filename(), defaultOpenMode);

            const FILE PTR before = file1.FilePtr;

            const CFileManager file2(std::move(file1));

            REQUIRE(file2.FilePtr == before);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);

        }

        WHEN("A file is not open move constructing from it leaves both unopen")
        {
            CFileManager file1;

            const CFileManager file2(std::move(file1));

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : File should not be copy assignible", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;

        REQUIRE(std::is_copy_assignable_v<CFileManager> == false);
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : Move assigning should stransfer the file", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is open move assigning from it transfer the file")
        {
            CFileManager file1(cleaner.Filename(), defaultOpenMode);
            CFileManager file2;

            const FILE PTR before = file1.FilePtr;

            file2 = std::move(file1);

            REQUIRE(file2.FilePtr == before);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);
        }

        WHEN("A file is open move assigning to it closed file closes the file")
        {
            CFileManager file1(cleaner.Filename(), defaultOpenMode);
            CFileManager file2;

            const FILE PTR before = file1.FilePtr;

            file1 = std::move(file2);

            REQUIRE(file2.FilePtr != before);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }

        WHEN("A file is open move assigning to it opened file transfers it")
        {
            const FilenameCleaner cleaner2;
            REQUIRE(cleaner2.IsOk() == true);

            {
                CFileManager file1(cleaner.Filename(), defaultOpenMode);
                CFileManager file2(cleaner2.Filename(), defaultOpenMode);

                const FILE PTR before = file2.FilePtr;

                file1 = std::move(file2);

                REQUIRE(file1.FilePtr == before);

                REQUIRE(file1.IsOpen() == true);

                REQUIRE(file2.IsOpen() == false);
            }
        }

        WHEN("A file is not open move assigning from it leaves both unopen")
        {
            CFileManager file1;
            CFileManager file2;

            file2 = std::move(file1);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }

        WHEN("Move assigning a file to itself should not change its state")
        {
            CFileManager file1(cleaner.Filename(), defaultOpenMode);
            CFileManager file2;

            const FILE PTR before1  = file1.FilePtr;
            const FILE PTR before2  = file2.FilePtr;

            file1  = std::move(file1);
            file2  = std::move(file2);

            REQUIRE(file1.FilePtr  == before1);

            REQUIRE(file2.FilePtr  == before2);

            REQUIRE(file1.IsOpen() == true);
            REQUIRE(file2.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : bool conversion should tell if the file is opened or closed", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is closed bool conversion should be false")
        {
            const CFileManager file;

            REQUIRE(file.operator bool() == false);

            //Calling the function should not chanage the state
            REQUIRE(file.operator bool() == false);
        }

        WHEN("A file is opened in the constructor bool conversion should be true")
        {
            const CFileManager file(cleaner.Filename(), defaultOpenMode);

            REQUIRE(file.operator bool() == true);

            //Calling the function should not chanage the state
            REQUIRE(file.operator bool() == true);
        }

        WHEN("A file is opened using OpenNew bool conversion should be true")
        {
            CFileManager file;

            file.OpenNew(cleaner.Filename(), defaultOpenMode);

            REQUIRE(file.operator bool() == true);
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : Swaping files", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Both swapped file are not opened they should remain unopened")
        {
            CFileManager file1;
            CFileManager file2;

            file1.Swap(file2);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }

        WHEN("One swapped file is opened the \"openness\" should swap")
        {
            CFileManager file1(cleaner.Filename(), defaultOpenMode);
            CFileManager file2;

            const FILE PTR before = file1.FilePtr;

            file1.Swap(file2);

            //Is this check (pointers) needed?
            REQUIRE(file2.FilePtr == before);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);
        }

        WHEN("Both files are opened both should remain opened but their file pointers need to swap")
        {
            const FilenameCleaner cleanerNew;
            REQUIRE(cleaner.IsOk() == true);

            {
                CFileManager file1(cleaner.Filename(), defaultOpenMode);
                CFileManager file2(cleanerNew.Filename(), defaultOpenMode);

                const FILE PTR before1  = file1.FilePtr;
                const FILE PTR before2  = file2.FilePtr;

                file1.Swap(file2);

                REQUIRE(file1.FilePtr  == before2);
                REQUIRE(file2.FilePtr  == before1);

                REQUIRE(file1.IsOpen() == true);
                REQUIRE(file2.IsOpen() == true);
            }
        }

    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : Close testing", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is not opened closing it should not change its state")
        {
            CFileManager file;

            //The function should not fail
            REQUIRE(file.Close() == true);

            REQUIRE(file.IsOpen() == false);
        }

        WHEN("A file is opened closing it should leave it closed")
        {
            CFileManager file(cleaner.Filename(), defaultOpenMode);

            //The function should not fail
            REQUIRE(file.Close() == true);

            REQUIRE(file.IsOpen() == false);
        }
    }
}

#endif // CFILEMANAGERTEST_H
