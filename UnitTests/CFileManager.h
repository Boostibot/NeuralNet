#ifndef CFILEMANAGERTEST_H
#define CFILEMANAGERTEST_H
#include "Catch2/Catch.hpp"
#include "General/File/CFileManager.h"

namespace CFileManagerTesting
{
    template<typename T>
    struct BasicCFileManagerTester : public BasicCFileManager<T>
    {
            using BasicCFileManager = BasicCFileManager<T>;
            using BasicCFileManager::FilePtr;
            using BasicCFileManager::CloseFile;
    };

    template<typename T>
    BasicCFileManagerTester<T> REF ToTester(BasicCFileManager<T> REF file)
    {
        return static_cast<BasicCFileManagerTester<T> REF>(file);
    }

    template<typename CharT, size_t CharsMax = L_tmpnam>
    struct BasicFilenameCleaner
    {
            CharT Filename[CharsMax];

            static constexpr bool IsChar8 = std::is_same_v<CharT, char8>;
            static constexpr bool IsCharW = std::is_same_v<CharT, charW>;

            static_assert (IsChar8 || IsCharW, "Only char and wchar_t allowed as CharT");

        private:
            bool Success;

        public:
            BasicFilenameCleaner()
            {
                if constexpr(IsChar8)
                    Success = (tmpnam_s(Filename, CharsMax) == 0);
                else
                    Success = (_wtmpnam_s(Filename, CharsMax) == 0);
            }
            ~BasicFilenameCleaner()
            {
                if(HasError())
                    return;

                //It is ok to remove file that does not exist
                // in that case the functions should do nothing
                if constexpr(IsChar8)
                    remove(Filename);
                else
                    _wremove(Filename);
            }

            inline bool IsOk()      {return Success; }
            inline bool HasError()  {return NOT Success;}

        public:
            BasicFilenameCleaner(const BasicFilenameCleaner REF) = delete;
            BasicFilenameCleaner(BasicFilenameCleaner RVALUE_REF) = delete;
            BasicFilenameCleaner REF operator=(const BasicFilenameCleaner REF) = delete;
            BasicFilenameCleaner REF operator=(BasicFilenameCleaner RVALUE_REF) = delete;
    };

    using FilenameCleaner = BasicFilenameCleaner<char8>;
    using WFilenameCleaner = BasicFilenameCleaner<charW>;
}

namespace CFileManagerTesting
{
    TEST_CASE("Only char and wchar_t specialisations should compile")
    {
        BasicCFileManager<char8> file8;
        BasicCFileManager<charW> fileW;
        //Should not compile
        //BasicCFileManager<int> fileInt;
        //BasicCFileManager<double> fileD;
        //BasicCFileManager<short> fileShort;
        //BasicCFileManager<long> fileLing;
        //BasicCFileManager<std::string> fileString;
        //BasicCFileManager<std::wstring> fileWString;
    }

    TEST_CASE("After construction file should be closed")
    {
        CFileManager file;
        WCFileManager fileW;

        //The pointer should be set to null
        REQUIRE(file.IsOpen() == false);
        REQUIRE(fileW.IsOpen() == false);
    }


    TEST_CASE("Constructing by opening a file should open the file")
    {
        FilenameCleaner cleaner;
        WFilenameCleaner wcleaner;
        REQUIRE(cleaner.IsOk() == true);
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is opened in the constructor it is opened")
        {
            CFileManager file(cleaner.Filename, "wb");
            WCFileManager wfile(wcleaner.Filename, L"wb");

            //It should be declared open
            REQUIRE(file.IsOpen() == true);
            REQUIRE(wfile.IsOpen() == true);

            //Its file pointer should be set to something
            //REQUIRE(file.FilePtr != nullptr);
            //REQUIRE(wfile.FilePtr != nullptr);

            //Another file tries to open a file that should be already present
            // the 'x' specifier in the argument string forces it to fail if
            // the file already exists
            CFileManager file2(cleaner.Filename, "wbx");
            WCFileManager wfile2(wcleaner.Filename, L"wbx");

            //It should not be opened
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(wfile2.IsOpen() == false);
        }

    }


    TEST_CASE("ReOpening files")
    {
        FilenameCleaner cleaner;
        WFilenameCleaner wcleaner;
        REQUIRE(cleaner.IsOk() == true);
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is not yet opened OpenNew should leave it opened")
        {
            CFileManager file;
            WCFileManager wfile;

            //OpenNew should succeed
            REQUIRE(file.OpenNew(cleaner.Filename, "wb") == true);
            REQUIRE(wfile.OpenNew(wcleaner.Filename, L"wb") == true);

            REQUIRE(file.IsOpen() == true);
            REQUIRE(wfile.IsOpen() == true);
        }

        WHEN("A file is opened using constructor OpenNew will keep it opened")
        {
            FilenameCleaner cleanerNew;
            WFilenameCleaner wcleanerNew;
            REQUIRE(cleaner.IsOk() == true);
            REQUIRE(cleaner.IsOk() == true);

            {
                CFileManager file(cleaner.Filename, "wb");
                WCFileManager wfile(wcleaner.Filename, L"wb");

                //Constructor should not fail
                REQUIRE(file.IsOpen() == true);
                REQUIRE(wfile.IsOpen() == true);

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleanerNew.Filename, "wb") == true);
                REQUIRE(wfile.OpenNew(wcleanerNew.Filename, L"wb") == true);

                REQUIRE(file.IsOpen() == true);
                REQUIRE(wfile.IsOpen() == true);
            }
        }

        WHEN("A file is opened using OpenNew, OpenNew will keep it opened")
        {
            FilenameCleaner cleanerNew;
            WFilenameCleaner wcleanerNew;
            REQUIRE(cleaner.IsOk() == true);
            REQUIRE(cleaner.IsOk() == true);

            {
                CFileManager file;
                WCFileManager wfile;

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleaner.Filename, "wb") == true);
                REQUIRE(wfile.OpenNew(wcleaner.Filename, L"wb") == true);

                //Constructor should not fail
                REQUIRE(file.IsOpen() == true);
                REQUIRE(wfile.IsOpen() == true);

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleanerNew.Filename, "wb") == true);
                REQUIRE(wfile.OpenNew(wcleanerNew.Filename, L"wb") == true);

                REQUIRE(file.IsOpen() == true);
                REQUIRE(wfile.IsOpen() == true);
            }
        }
    }

    TEST_CASE("IsOpen and IsClosed should tell if the file is opened or closed")
    {
        FilenameCleaner cleaner;
        WFilenameCleaner wcleaner;
        REQUIRE(cleaner.IsOk() == true);
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file was never opened it should be closed")
        {
            const CFileManager file;
            const WCFileManager wfile;

            REQUIRE(file.IsOpen() == false);
            REQUIRE(wfile.IsOpen() == false);

            REQUIRE(file.IsClosed() == true);
            REQUIRE(wfile.IsClosed() == true);

            //Calling the function should not chanage the state
            REQUIRE(file.IsOpen() == false);
            REQUIRE(wfile.IsOpen() == false);
        }

        WHEN("A file is opened in the constructor it is opened")
        {
            const CFileManager file(cleaner.Filename, "wb");
            const WCFileManager wfile(wcleaner.Filename, L"wb");

            //It should be declared open
            REQUIRE(file.IsOpen() == true);
            REQUIRE(wfile.IsOpen() == true);

            REQUIRE(file.IsClosed() == false);
            REQUIRE(wfile.IsClosed() == false);

            //Calling the function should not chanage the state
            REQUIRE(file.IsOpen() == true);
            REQUIRE(wfile.IsOpen() == true);
        }

        WHEN("A file is opened using OpenNew it is opened")
        {
            CFileManager file;
            WCFileManager wfile;

            REQUIRE(file.OpenNew(cleaner.Filename, "wb") == true);
            REQUIRE(wfile.OpenNew(wcleaner.Filename, L"wb") == true);

            //It should be declared open
            REQUIRE(file.IsOpen() == true);
            REQUIRE(wfile.IsOpen() == true);

            REQUIRE(file.IsClosed() == false);
            REQUIRE(wfile.IsClosed() == false);
        }
    }

    TEST_CASE("Move constructing should transfer the file")
    {
        FilenameCleaner cleaner;
        WFilenameCleaner wcleaner;
        REQUIRE(cleaner.IsOk() == true);
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is open move constructing from it transfer the file")
        {
            CFileManager file1(cleaner.Filename, "wb");
            WCFileManager wfile1(wcleaner.Filename, L"wb");

            const FILE PTR before = file1.FilePtr;
            const FILE PTR wbefore = wfile1.FilePtr;

            CFileManager file2(std::move(file1));
            WCFileManager wfile2(std::move(wfile1));

            REQUIRE(file2.FilePtr == before);
            REQUIRE(wfile2.FilePtr == wbefore);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);

            REQUIRE(wfile1.IsOpen() == false);
            REQUIRE(wfile2.IsOpen() == true);
        }

        WHEN("A file is not open move constructing from it leaves both unopen")
        {
            CFileManager file1;
            WCFileManager wfile1;

            CFileManager file2(std::move(file1));
            WCFileManager wfile2(std::move(wfile1));

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }

    }

    TEST_CASE("Move assigning should stransfer the file")
    {
        FilenameCleaner cleaner;
        WFilenameCleaner wcleaner;
        REQUIRE(cleaner.IsOk() == true);
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is open move assigning from it transfer the file")
        {
            CFileManager file1(cleaner.Filename, "wb");
            WCFileManager wfile1(wcleaner.Filename, L"wb");
            CFileManager file2;
            WCFileManager wfile2;

            const FILE PTR before = file1.FilePtr;
            const FILE PTR wbefore = wfile1.FilePtr;

            file2 = std::move(file1);
            wfile2 = std::move(wfile1);

            REQUIRE(file2.FilePtr == before);
            REQUIRE(wfile2.FilePtr == wbefore);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);

            REQUIRE(wfile1.IsOpen() == false);
            REQUIRE(wfile2.IsOpen() == true);
        }

        WHEN("A file is open move assigning to it closes the file")
        {
            CFileManager file1(cleaner.Filename, "wb");
            WCFileManager wfile1(wcleaner.Filename, L"wb");
            CFileManager file2;
            WCFileManager wfile2;

            const FILE PTR before = file1.FilePtr;
            const FILE PTR wbefore = wfile1.FilePtr;

            file1 = std::move(file2);
            wfile1 = std::move(wfile2);

            REQUIRE(file2.FilePtr != before);
            REQUIRE(wfile2.FilePtr != wbefore);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);

            REQUIRE(wfile1.IsOpen() == false);
            REQUIRE(wfile2.IsOpen() == false);
        }

        WHEN("A file is not open move assigning from it leaves both unopen")
        {
            CFileManager file1;
            WCFileManager wfile1;
            CFileManager file2;
            WCFileManager wfile2;

            file2 = std::move(file1);
            wfile2 = std::move(wfile1);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);

            REQUIRE(wfile1.IsOpen() == false);
            REQUIRE(wfile2.IsOpen() == false);
        }

        WHEN("Move assigning a file to itself should not change its state")
        {
            CFileManager file1(cleaner.Filename, "wb");
            WCFileManager wfile1(wcleaner.Filename, L"wb");
            CFileManager file2;
            WCFileManager wfile2;

            const FILE PTR before1  = file1.FilePtr;
            const FILE PTR wbefore1 = wfile1.FilePtr;
            const FILE PTR before2  = file2.FilePtr;
            const FILE PTR wbefore2 = wfile2.FilePtr;

            file1  = std::move(file1);
            wfile1 = std::move(wfile1);
            file2  = std::move(file2);
            wfile2 = std::move(wfile2);

            REQUIRE(file1.FilePtr  == before1);
            REQUIRE(wfile1.FilePtr == wbefore1);

            REQUIRE(file2.FilePtr  == before2);
            REQUIRE(wfile2.FilePtr == wbefore2);

            REQUIRE(file1.IsOpen() == true);
            REQUIRE(file2.IsOpen() == false);

            REQUIRE(wfile1.IsOpen() == true);
            REQUIRE(wfile2.IsOpen() == false);
        }
    }

    TEST_CASE("bool conversion should tell if the file is opened or closed")
    {
        FilenameCleaner cleaner;
        WFilenameCleaner wcleaner;
        REQUIRE(cleaner.IsOk() == true);
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is closed bool conversion should be false")
        {
            const CFileManager file;
            const WCFileManager wfile;

            REQUIRE(file.operator bool() == false);
            REQUIRE(wfile.operator bool() == false);

            //Calling the function should not chanage the state
            REQUIRE(file.operator bool() == false);
            REQUIRE(wfile.operator bool() == false);
        }

        WHEN("A file is opened in the constructor bool conversion should be true")
        {
            const CFileManager file(cleaner.Filename, "wb");
            const WCFileManager wfile(wcleaner.Filename, L"wb");

            REQUIRE(file.operator bool() == true);
            REQUIRE(wfile.operator bool() == true);

            //Calling the function should not chanage the state
            REQUIRE(file.operator bool() == true);
            REQUIRE(wfile.operator bool() == true);
        }

        WHEN("A file is opened using OpenNew bool conversion should be true")
        {
            CFileManager file;
            WCFileManager wfile;

            file.OpenNew(cleaner.Filename, "wb");
            wfile.OpenNew(wcleaner.Filename, L"wb");

            REQUIRE(file.operator bool() == true);
            REQUIRE(wfile.operator bool() == true);
        }
    }

    TEST_CASE("Swaping files")
    {
        FilenameCleaner cleaner;
        WFilenameCleaner wcleaner;
        REQUIRE(cleaner.IsOk() == true);
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Both swapped file are not opened they should remain unopened")
        {
            CFileManager file1;
            CFileManager file2;
            WCFileManager wfile1;
            WCFileManager wfile2;

            file1.Swap(file2);
            wfile1.Swap(wfile2);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(wfile1.IsOpen() == false);
            REQUIRE(wfile2.IsOpen() == false);
        }

        WHEN("One swapped file is opened the \"openness\" should swap")
        {
            CFileManager file1(cleaner.Filename, "wb");
            WCFileManager wfile1(wcleaner.Filename, L"wb");
            CFileManager file2;
            WCFileManager wfile2;

            const FILE PTR before = file1.FilePtr;
            const FILE PTR wbefore = wfile1.FilePtr;

            file1.Swap(file2);
            wfile1.Swap(wfile2);

            //Is this check (pointers) needed?
            REQUIRE(file2.FilePtr == before);
            REQUIRE(wfile2.FilePtr == wbefore);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);

            REQUIRE(wfile1.IsOpen() == false);
            REQUIRE(wfile2.IsOpen() == true);
        }

        WHEN("Both files are opened both should remain opened but their file pointers need to swap")
        {
            FilenameCleaner cleanerNew;
            WFilenameCleaner wcleanerNew;
            REQUIRE(cleaner.IsOk() == true);
            REQUIRE(cleaner.IsOk() == true);

            {
                CFileManager file1(cleaner.Filename, "wb");
                WCFileManager wfile1(wcleaner.Filename, L"wb");
                CFileManager file2(cleanerNew.Filename, "wb");
                WCFileManager wfile2(wcleanerNew.Filename, L"wb");

                const FILE PTR before1  = file1.FilePtr;
                const FILE PTR wbefore1 = wfile1.FilePtr;
                const FILE PTR before2  = file2.FilePtr;
                const FILE PTR wbefore2 = wfile2.FilePtr;

                file1.Swap(file2);
                wfile1.Swap(wfile2);

                REQUIRE(file1.FilePtr  == before2);
                REQUIRE(wfile1.FilePtr == wbefore2);
                REQUIRE(file2.FilePtr  == before1);
                REQUIRE(wfile2.FilePtr == wbefore1);

                REQUIRE(file1.IsOpen() == true);
                REQUIRE(file2.IsOpen() == true);

                REQUIRE(wfile1.IsOpen() == true);
                REQUIRE(wfile2.IsOpen() == true);
            }
        }

    }

    TEST_CASE("Close testing")
    {
        FilenameCleaner cleaner;
        WFilenameCleaner wcleaner;
        REQUIRE(cleaner.IsOk() == true);
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is not opened closing it should not change its state")
        {
            CFileManager file;
            WCFileManager wfile;

            //The function should not fail
            REQUIRE(file.Close() == true);
            REQUIRE(wfile.Close() == true);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(file.IsOpen() == false);
        }

        WHEN("A file is opened closing it should leave it closed")
        {
            CFileManager file(cleaner.Filename, "wb");
            WCFileManager wfile(wcleaner.Filename, L"wb");

            //The function should not fail
            REQUIRE(file.Close() == true);
            REQUIRE(wfile.Close() == true);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(file.IsOpen() == false);
        }
    }
}

#endif // CFILEMANAGERTEST_H
