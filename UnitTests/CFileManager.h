#ifndef CFILEMANAGERTEST_H
#define CFILEMANAGERTEST_H
#include "Catch2/Catch.hpp"
#include "General/File/CFileManager.h"

namespace CIo::CFileManagerTesting
{
    template<typename CharT, size_t CharsMax = L_tmpnam>
    struct BasicFilenameCleaner
    {
        private:
            using CharSupport = CharSupport<CharT>;
            using CStringRef = CStringRef<CharT>;

        private:
            bool State;
            CharT FilenameString[CharsMax];

        public:
            BasicFilenameCleaner()
            {
                State = (CharSupport::tmpnam_s(FilenameString, CharsMax) == 0);
                //State = StaticFunctions::GetUniqueTempFileName(CStringRef(FilenameString, CharsMax));
            }
            BasicFilenameCleaner(const BasicFilenameCleaner REF) = default;
            ~BasicFilenameCleaner()
            {
                if(IsOk() == false)
                    return;

                //It is ok to remove file that does not exist
                // in that case the functions should do nothing
                CharSupport::remove(FilenameString);
                //StaticFunctions::RemoveFile(CStringRef(FilenameString, CharsMax));
            }
            inline auto Filename() const noexcept {return FilenameString;}
            inline bool IsOk()     const noexcept {return State; }

        public:
            BasicFilenameCleaner(BasicFilenameCleaner RVALUE_REF) = delete;
            BasicFilenameCleaner REF operator=(const BasicFilenameCleaner REF) = delete;
            BasicFilenameCleaner REF operator=(BasicFilenameCleaner RVALUE_REF) = delete;
    };

    template<typename T>
    constexpr BasicOpenMode<T> defaultModeTmpl(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Binary);
    static_assert (defaultModeTmpl<char8>.GetOpenModeString() == OpenMode::OpenModeString("w+b"),   "UNEXPECTED");
    static_assert (defaultModeTmpl<charW>.GetOpenModeString() == WOpenMode::OpenModeString(L"w+b"), "UNEXPECTED");
}

#define CFileManager_TEST_COMPILE_ERROR false
#define CFileManagerTestedTypes char8, charW

namespace CIo::CFileManagerTesting::FilenameCleanerTesting
{
    TEMPLATE_TEST_CASE("[FilenameCleaner] : Constructor should create a unique filename", "[FilenameCleaner]", CFileManagerTestedTypes)
    {
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using StringView = std::basic_string_view<TestType>;

        WHEN("a cleaner is constructed its filename should be set to something")
        {
            const FilenameCleaner cleaner;
            REQUIRE(cleaner.IsOk() == true);

            const StringView view = cleaner.Filename();
            const StringView empty;
            REQUIRE(view != empty);
        }

        WHEN("a multiple cleaners are constructed their filenames should differ")
        {
            const FilenameCleaner cleaner1;
            const FilenameCleaner cleaner2;
            const FilenameCleaner cleaner3;

            REQUIRE(cleaner1.IsOk() == true);
            REQUIRE(cleaner2.IsOk() == true);
            REQUIRE(cleaner3.IsOk() == true);

            const StringView view1 = cleaner1.Filename();
            const StringView view2 = cleaner2.Filename();
            const StringView view3 = cleaner3.Filename();

            REQUIRE(view1 != view2);
            REQUIRE(view2 != view3);
            REQUIRE(view3 != view1);
        }
    }

    TEMPLATE_TEST_CASE("[FilenameCleaner] : IsOk should return true if the constructor succeeded", "[FilenameCleaner]", CFileManagerTestedTypes)
    {
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);
    }

    TEMPLATE_TEST_CASE("[FilenameCleaner] : Copy constructor should copy the filename", "[FilenameCleaner]", CFileManagerTestedTypes)
    {
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using StringView = std::basic_string_view<TestType>;

        const FilenameCleaner original;
        const FilenameCleaner copy(original);
        const StringView originalView = original.Filename();
        const StringView copyView     = copy.Filename();

        REQUIRE(original.IsOk() == true);
        REQUIRE(copy.IsOk() == true);
        REQUIRE(originalView == copyView);
    }

    TEMPLATE_TEST_CASE("[FilenameCleaner] : Destructor should destruct the file associated with the filename", "[FilenameCleaner]", CFileManagerTestedTypes)
    {
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using CFileManager = BasicCFileManager<TestType>;

        WHEN("a file is associated with the filename it should get removed")
        {
            const FilenameCleaner cleaner;
            REQUIRE(cleaner.IsOk() == true);

            {
                //Opens a the specified file for writing (Creates a file)
                const CFileManager manager(cleaner.Filename(), COpenMode::Write);
                REQUIRE(manager.IsOpen() == true);
            }

            {
                //Copies the associated filanem and then destructs it
                const FilenameCleaner copy(cleaner);
            }

            REQUIRE(CFileManager::IsFileOpenable(cleaner.Filename()) == false);


        }
        WHEN("no file is associated with the filename nothing shsould happen")
        {
            const FilenameCleaner cleaner;
            REQUIRE(cleaner.IsOk() == true);

            REQUIRE(CFileManager::IsFileOpenable(cleaner.Filename()) == false);

            {
                //Copies the associated filanem and then destructs it
                const FilenameCleaner copy(cleaner);
            }

            REQUIRE(CFileManager::IsFileOpenable(cleaner.Filename()) == false);
        }

    }
}


namespace CIo::CFileManagerTesting
{
    TEST_CASE("[BasicCFileManager] : Only char8 amd charW (char and wchar_t) specialisations should compile", "[BasicCFileManager]")
    {
        [[maybe_unused]] BasicCFileManager<char8> file8;
        [[maybe_unused]] BasicCFileManager<charW> fileW;

        #if CFileManager_TEST_COMPILE_ERROR == true
        //Should not compile
        //[[maybe_unused]] BasicCFileManager<int> fileInt;
        //[[maybe_unused]] BasicCFileManager<double> fileD;
        //[[maybe_unused]] BasicCFileManager<short> fileShort;
        //[[maybe_unused]] BasicCFileManager<long> fileLing;
        //[[maybe_unused]] BasicCFileManager<std::string> fileString;
        //[[maybe_unused]] BasicCFileManager<std::wstring> fileWString;
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
        using OpenMode = BasicOpenMode<TestType>;
        constexpr OpenMode defaultOpenMode = defaultModeTmpl<TestType>;

        constexpr OpenMode invalidOpenMode;
        constexpr OpenMode unsupportedOpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
        static_assert (invalidOpenMode.IsValid() == false, "!");
        static_assert (unsupportedOpenMode.IsSupported() == false, "!");

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        //Creates a new cleaner that manages the same generated
        // filename and destructs it
        // This leaves us with an unique filename in the first cleaner that is guaranteed to be deleted
        {
            const FilenameCleaner cleanerTemp(cleaner);
        }

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("provided with COpenMode::ReadMustExist")
            {
                WHEN("The filename does not exist no file is opened")
                {
                    const CFileManager manager(cleaner.Filename(), COpenMode::ReadMustExist);
                    REQUIRE(manager.IsOpen() == false);
                }
                WHEN("The filename does exist the file is opened")
                {
                    {
                        const CFileManager creator(cleaner.Filename(), COpenMode::Write);
                    }

                    const CFileManager manager(cleaner.Filename(), COpenMode::ReadMustExist);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::Write")
            {
                WHEN("is without the MustNotExist a file is always opened")
                {
                    const CFileManager manager(cleaner.Filename(), COpenMode::Write);
                    REQUIRE(manager.IsOpen() == true);
                }
                WHEN("is with the MustNotExist a file is always opened only when the file does not exist")
                {
                    WHEN("the file does not exists")
                    {
                        const CFileManager manager(cleaner.Filename(), OpenMode(OpenModeFlag::Write, OpenModeFlag::MustNotExist));
                        REQUIRE(manager.IsOpen() == true);
                    }
                    WHEN("the file does exist")
                    {
                        {
                            const CFileManager creator(cleaner.Filename(), COpenMode::Write);
                        }
                        const CFileManager manager(cleaner.Filename(), OpenMode(OpenModeFlag::Write, OpenModeFlag::MustNotExist));
                        REQUIRE(manager.IsOpen() == false);
                    }
                }
            }
            WHEN("provided with COpenMode::WriteAppend")
            {
                THEN("a file is always opened (given sufficient permission - untested)")
                {
                    const CFileManager manager(cleaner.Filename(), COpenMode::WriteAppend);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::ReadWriteMustExist")
            {
                WHEN("The filename does not exist no file is opened")
                {
                    const CFileManager manager(cleaner.Filename(), COpenMode::ReadWriteMustExist);
                    REQUIRE(manager.IsOpen() == false);
                }
                WHEN("The filename does exist the file is opened")
                {
                    {
                        const CFileManager creator(cleaner.Filename(), COpenMode::Write);
                    }

                    const CFileManager manager(cleaner.Filename(), COpenMode::ReadWriteMustExist);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::ReadWrite")
            {
                WHEN("is without the MustNotExist a file is always opened")
                {
                    const CFileManager manager(cleaner.Filename(), COpenMode::ReadWrite);
                    REQUIRE(manager.IsOpen() == true);
                }
                WHEN("is with the MustNotExist a file is always opened only when the file does not exist")
                {
                    WHEN("the file does not exists")
                    {
                        const CFileManager manager(cleaner.Filename(), OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist));
                        REQUIRE(manager.IsOpen() == true);
                    }
                    WHEN("the file does exist")
                    {
                        {
                            const CFileManager creator(cleaner.Filename(), COpenMode::Write);
                        }
                        const CFileManager manager(cleaner.Filename(), OpenMode(OpenModeFlag::Read, OpenModeFlag::Write,  OpenModeFlag::MustNotExist));
                        REQUIRE(manager.IsOpen() == false);
                    }
                }
            }
            WHEN("provided with COpenMode::ReadWriteAppend")
            {

                THEN("a file is always opened (given sufficient permission - untested)")
                {
                    const CFileManager manager(cleaner.Filename(), COpenMode::ReadWriteAppend);
                    REQUIRE(manager.IsOpen() == true);
                }
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

        WHEN("A infvalid filename is passed into the constructor no file is opened")
        {
            typename CFileManager::OsStringView filename;

            const CFileManager file1(filename, invalidOpenMode);
            const CFileManager file2(filename, unsupportedOpenMode);
            const CFileManager file3(filename, COpenMode::ReadMustExist);
            const CFileManager file4(filename, COpenMode::Write);
            const CFileManager file5(filename, COpenMode::WriteAppend);
            const CFileManager file6(filename, COpenMode::ReadWriteMustExist);
            const CFileManager file7(filename, COpenMode::ReadWrite);
            const CFileManager file8(filename, COpenMode::ReadWriteAppend);

            const CFileManager file9(filename, OpenMode(OpenModeFlag::Write, OpenModeFlag::MustNotExist));
            const CFileManager file10(filename, OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist));

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(file3.IsOpen() == false);
            REQUIRE(file4.IsOpen() == false);
            REQUIRE(file5.IsOpen() == false);
            REQUIRE(file6.IsOpen() == false);
            REQUIRE(file7.IsOpen() == false);
            REQUIRE(file8.IsOpen() == false);
            REQUIRE(file9.IsOpen() == false);
            REQUIRE(file10.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : Constructing a file with filename and OpenModeFlags should open the file", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using OpenMode = BasicOpenMode<TestType>;
        constexpr OpenMode defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        //Creates a new cleaner that manages the same generated
        // filename and destructs it
        // This leaves us with an unique filename in the first cleaner that is guaranteed to be deleted
        {
            const FilenameCleaner cleanerTemp(cleaner);
        }

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("provided with COpenMode::ReadMustExist equivalent")
            {
                WHEN("The filename does not exist no file is opened")
                {
                    const CFileManager manager(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist);
                    REQUIRE(manager.IsOpen() == false);
                }
                WHEN("The filename does exist the file is opened")
                {
                    {
                        const CFileManager creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    }

                    const CFileManager manager(cleaner.Filename(), OpenModeFlag::MustExist, OpenModeFlag::Read);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::Write equivalent")
            {
                WHEN("is without the MustNotExist a file is always opened")
                {
                    const CFileManager manager(cleaner.Filename(), OpenModeFlag::Write);
                    REQUIRE(manager.IsOpen() == true);
                }
                WHEN("is with the MustNotExist a file is always opened only when the file does not exist")
                {
                    WHEN("the file does not exists")
                    {
                        const CFileManager manager(cleaner.Filename(), OpenModeFlag::MustNotExist,  OpenModeFlag::Binary, OpenModeFlag::Write);
                        REQUIRE(manager.IsOpen() == true);
                    }
                    WHEN("the file does exist")
                    {
                        {
                            const CFileManager creator(cleaner.Filename(), OpenModeFlag::Write);
                        }
                        const CFileManager manager(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::MustNotExist);
                        REQUIRE(manager.IsOpen() == false);
                    }
                }
            }
            WHEN("provided with COpenMode::WriteAppend equivalent")
            {
                THEN("a file is always opened (given sufficient permission - untested)")
                {
                    const CFileManager manager(cleaner.Filename(), OpenModeFlag::Append,  OpenModeFlag::Binary, OpenModeFlag::Write);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::ReadWriteMustExist equivalent")
            {
                WHEN("The filename does not exist no file is opened")
                {
                    const CFileManager manager(cleaner.Filename(), OpenModeFlag::MustExist, OpenModeFlag::Read, OpenModeFlag::Write);
                    REQUIRE(manager.IsOpen() == false);
                }
                WHEN("The filename does exist the file is opened")
                {
                    {
                        const CFileManager creator(cleaner.Filename(), OpenModeFlag::Write);
                    }

                    const CFileManager manager(cleaner.Filename(), OpenModeFlag::Write,  OpenModeFlag::Binary, OpenModeFlag::MustExist, OpenModeFlag::Read);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::ReadWrite equivalent")
            {
                WHEN("is without the MustNotExist a file is always opened")
                {
                    const CFileManager manager(cleaner.Filename(), COpenMode::ReadWrite);
                    REQUIRE(manager.IsOpen() == true);
                }
                WHEN("is with the MustNotExist a file is always opened only when the file does not exist")
                {
                    WHEN("the file does not exists")
                    {
                        const CFileManager manager(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::MustNotExist, OpenModeFlag::Read);
                        REQUIRE(manager.IsOpen() == true);
                    }
                    WHEN("the file does exist")
                    {
                        {
                            const CFileManager creator(cleaner.Filename(), OpenModeFlag::Write);
                        }
                        const CFileManager manager(cleaner.Filename(), OpenModeFlag::Read,  OpenModeFlag::MustNotExist, OpenModeFlag::Write, OpenModeFlag::Binary);
                        REQUIRE(manager.IsOpen() == false);
                    }
                }
            }
            WHEN("provided with COpenMode::ReadWriteAppend equivalent")
            {

                THEN("a file is always opened (given sufficient permission - untested)")
                {
                    const CFileManager manager(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);
                    REQUIRE(manager.IsOpen() == true);
                }
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const CFileManager file(cleaner.Filename());
                REQUIRE(file.IsOpen() == false);
                const CFileManager file2(cleaner.Filename(), OpenModeFlag::MustExist);
                REQUIRE(file2.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const CFileManager file(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);

                REQUIRE(file.IsOpen() == false);
            }
        }

        WHEN("A infvalid filename is passed into the constructor no file is opened")
        {
            typename CFileManager::OsStringView filename;

            const CFileManager file1(filename, OpenModeFlag::Binary);
            const CFileManager file2(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
            const CFileManager file3(filename, OpenModeFlag::Read, OpenModeFlag::MustExist);
            const CFileManager file4(filename, OpenModeFlag::Write);
            const CFileManager file5(filename, OpenModeFlag::Write, OpenModeFlag::Append);
            const CFileManager file6(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist);
            const CFileManager file7(filename, OpenModeFlag::Read, OpenModeFlag::Write);
            const CFileManager file8(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);

            const CFileManager file9(filename, OpenModeFlag::Write, OpenModeFlag::MustNotExist);
            const CFileManager file10(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist);

            const CFileManager file11(filename, OpenModeFlag::Write, OpenModeFlag::Binary);
            const CFileManager file12(filename, OpenModeFlag::Binary, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);
            const CFileManager file13(filename, OpenModeFlag::MustNotExist, OpenModeFlag::Read, OpenModeFlag::Binary, OpenModeFlag::Write);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(file3.IsOpen() == false);
            REQUIRE(file4.IsOpen() == false);
            REQUIRE(file5.IsOpen() == false);
            REQUIRE(file6.IsOpen() == false);
            REQUIRE(file7.IsOpen() == false);
            REQUIRE(file8.IsOpen() == false);
            REQUIRE(file9.IsOpen() == false);
            REQUIRE(file10.IsOpen() == false);
            REQUIRE(file11.IsOpen() == false);
            REQUIRE(file12.IsOpen() == false);
            REQUIRE(file13.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : OpenNew should open a new file independent of if a file is already opened", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Passing a complete openmode")
        {
            WHEN("A file is not yet opened OpenNew should leave it opened")
            {
                CFileManager file;

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleaner.Filename(), defaultOpenMode) == true);
            }

            WHEN("A file is opened using constructor OpenNew will keep it opened")
            {
                const FilenameCleaner cleanerNew;
                REQUIRE(cleaner.IsOk() == true);

                {
                    CFileManager file(cleaner.Filename(), defaultOpenMode);

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleanerNew.Filename(), defaultOpenMode) == true);
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

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleanerNew.Filename(), defaultOpenMode) == true);
                }
            }
        }
        WHEN("Passing individual openmode flags")
        {
            WHEN("A file is not yet opened OpenNew should leave it opened")
            {
                CFileManager file;

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Binary) == true);
            }

            WHEN("A file is opened using constructor OpenNew will keep it opened")
            {
                const FilenameCleaner cleanerNew;
                REQUIRE(cleaner.IsOk() == true);

                {
                    CFileManager file(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append);

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleanerNew.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary) == true);
                }
            }

            WHEN("A file is opened using OpenNew, OpenNew will keep it opened")
            {
                const FilenameCleaner cleanerNew;
                REQUIRE(cleaner.IsOk() == true);

                {
                    CFileManager file;

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Append) == true);

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleanerNew.Filename(), OpenModeFlag::Write, OpenModeFlag::Read) == true);
                }
            }
        }
    }

    TEMPLATE_TEST_CASE("[BasicCFileManager] : IsOpen and IsClosed should tell if the file is opened or closed", "[BasicCFileManager]", CFileManagerTestedTypes)
    {
        using CFileManager = BasicCFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

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
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

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
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

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
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

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
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

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
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

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
