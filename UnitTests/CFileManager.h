#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H
#include "Catch2/Catch.hpp"
#include "Main/UnbufferedFile.h"

namespace CFileManagerTesting
{
    struct CFileManagerTester : public CFileManager
    {
            using CFileManager::File;
            using CFileManager::DisableBuffering;
            using CFileManager::CloseFile;
    };

    CFileManagerTester REF ToTester(CFileManager REF file)
    {
        return static_cast<CFileManagerTester REF>(file);
    }
}

namespace CFileManagerTesting
{
    TEST_CASE("Default construction testing")
    {
        CFileManager file;

        //The pointer should be set to null
        REQUIRE(ToTester(file).File == nullptr);
        REQUIRE(file.IsOpen() == false);
    }

    TEST_CASE("Constructing by opening a file")
    {
        CFileManager file1("temp.txt", "wb");
        REQUIRE(file1.IsOpen() == true);

        //Another file tries to open a file that should be already present
        // the 'x' specifier in the argument string forces it to fail if
        // the file already exists
        CFileManager file2("temp.txt", "wbx");
        REQUIRE(file2.IsOpen() == false);
    }

    TEST_CASE("Movec constructing")
    {
        CFileManager file0;
        CFileManager file1("temp.txt", "wb");

        CFileManager file2(std::move(file1));

        REQUIRE(file1.IsOpen() == false);
        REQUIRE(file2.IsOpen() == true);

        CFileManager file3(std::move(file0));

        //file3 should be not open because file0 was not open
        REQUIRE(file0.IsOpen() == false);
        REQUIRE(file3.IsOpen() == false);
    }

    TEST_CASE("Move assigning")
    {
        CFileManager file0;
        CFileManager file1("temp.txt", "wb");
        CFileManager file2("temp3.txt", "wb");

        CFileManager fileMove;

        fileMove = std::move(file0);
        //file3 should be not open because file0 was not open
        REQUIRE(file0.IsOpen() == false);
        REQUIRE(fileMove.IsOpen() == false);

        fileMove = std::move(file1);
        REQUIRE(file1.IsOpen() == false);
        REQUIRE(fileMove.IsOpen() == true);

        //When setting even to a open file the other
        // file should be closed
        fileMove = std::move(file2);
        REQUIRE(file1.IsOpen() == false);
        REQUIRE(fileMove.IsOpen() == true);
    }

    TEST_CASE("bool conversion operator")
    {
        CFileManager file0;
        CFileManager file1("temp.txt", "wb");

        REQUIRE(file0.operator bool() == false);
        REQUIRE(file1.operator bool() == true);
        REQUIRE(file1.operator bool() == true);
    }

    TEST_CASE("Swaping files")
    {
        CFileManager file0;
        CFileManager file1("temp.txt", "wb");
        CFileManager file2("temp3.txt", "wb");
        CFileManager fileMove;

        fileMove.Swap(file0);
        REQUIRE(file0.IsOpen() == false);
        REQUIRE(fileMove.IsOpen() == false);

        fileMove.Swap(file1);
        REQUIRE(file1.IsOpen() == false);
        REQUIRE(fileMove.IsOpen() == true);

        FILE POINTER temp1 = ToTester(fileMove).File;
        FILE POINTER temp2 = ToTester(file2).File;

        fileMove.Swap(file2);
        REQUIRE(temp1 == ToTester(file2).File);
        REQUIRE(temp2 == ToTester(fileMove).File);

        file2.Swap(fileMove);
        REQUIRE(temp1 == ToTester(fileMove).File);
        REQUIRE(temp2 == ToTester(file2).File);
    }

    TEST_CASE("ReOpening files")
    {
        CFileManager file0;
        CFileManager file1("temp.txt", "wb");

        REQUIRE(file0.ReOpen("temp.txt", "wb") == true);
        REQUIRE(file0.IsOpen() == true);

        REQUIRE(file1.ReOpen("temp.txt", "wb") == true);
        REQUIRE(file1.IsOpen() == true);

        //Opening the same file should result in the same output
        REQUIRE(file1.ReOpen("temp.txt", "wb") == true);
        REQUIRE(file1.IsOpen() == true);

        //This should fail if the other opening succeeded
        //  due to the x in the argument list
        REQUIRE(file1.ReOpen("temp.txt", "wbx") == false);
        REQUIRE(file1.IsOpen() == false);
    }

    TEST_CASE("IsOpen and IsClosed testing")
    {
        CFileManager file;
        ToTester(file).File = nullptr;
        REQUIRE(file.IsOpen() == false);
        REQUIRE(file.IsClosed() == true);

        file.ReOpen("temp.txt", "wb");
        REQUIRE(file.IsOpen() == true);
        REQUIRE(file.IsClosed() == false);
    }

    TEST_CASE("Close testing")
    {
        CFileManager file;
        REQUIRE(file.Close() == true);
        REQUIRE(file.IsOpen() == false);

        REQUIRE(file.ReOpen("temp.txt", "wb") == true);
        REQUIRE(file.IsOpen() == true);

        REQUIRE(file.Close() == true);
        REQUIRE(file.IsOpen() == false);
    }
}

#endif // CFILEMANAGER_H
