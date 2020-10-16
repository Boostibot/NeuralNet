#ifndef FILE_H
#define FILE_H
#include "Catch2/Catch.hpp"
#include "Main/UnbufferedFile.h"
#include <iostream>

#include "General/StaticLog/StaticLog.h"

#undef DO_SOURCE_LOG
#define DO_SOURCE_LOG false

namespace FileTesting
{
    struct FileTester : public File
    {
            using File::FilePtr;
            using File::CloseFile;
    };

    FileTester REF ToTester(File REF file)
    {
        return static_cast<FileTester REF>(file);
    }

    TEST_CASE("Default Constructing file", "[File]")
    {
        File file;
        REQUIRE(ToTester(file).FilePtr == nullptr);
    }

    TEST_CASE("Move Constructing file", "[File]")
    {
        SECTION("Empty file")
        {
            File file0;
            File file(std::move(file0));

            REQUIRE(ToTester(file).FilePtr == nullptr);
        }
        SECTION("NonEmpty file")
        {
            File file1;
            file1.ReOpen("temp.txt", "wb");
            File file(std::move(file1));

            REQUIRE(ToTester(file).FilePtr != nullptr);
            REQUIRE(file.IsOpen() == true);

            REQUIRE(ToTester(file1).FilePtr == nullptr);
            REQUIRE(file1.IsOpen() == false);
        }
    }

    TEST_CASE("Move assigning file", "[File]")
    {
        SECTION("Empty file")
        {
            File file0;
            File file(std::move(file0));

            REQUIRE(ToTester(file).FilePtr == nullptr);
        }
        SECTION("NonEmpty file")
        {
            File file1;
            file1.ReOpen("temp.txt", "wb");
            File file(std::move(file1));

            REQUIRE(ToTester(file).FilePtr != nullptr);
            REQUIRE(file.IsOpen() == true);

            REQUIRE(ToTester(file1).FilePtr == nullptr);
            REQUIRE(file1.IsOpen() == false);
        }
        SECTION("Self")
        {           
            File file0;
            File file1;
            file1.ReOpen("temp.txt", "wb");

            #pragma warning(suppress : 7633)
            file0 = std::move(file0);
            #pragma warning(suppress : 7633)
            file1 = std::move(file1);

            REQUIRE(file0.IsOpen() == false);
            REQUIRE(file1.IsOpen() == true);
        }
    }

    TEST_CASE("Construction File by opening a file", "[File]")
    {
        File file1("temp.txt", "wb");
        File file2("temp.txt", "wbx");

        REQUIRE(file1.IsOpen() == true);
        REQUIRE(file2.IsOpen() == false);
    }

    TEST_CASE("End of file testing", "[File]")
    {
    }

    TEST_CASE("Has error testing", "[File]")
    {
    }

    TEST_CASE("Clear error testing", "[File]")
    {
    }

    TEST_CASE("Geting pos in file", "[File]")
    {
        File file;
        StaticLog::DefaultLogger<> log("MyFile.txt");

        if(file.ReOpen("tempXX.txt", "r+b") == false)
        {
            std::cout << "Error opening file" << std::endl;
            return;
        }

        file.MoveToEnd();
        std::cout << "Pos in file after move to end" << file.GetFilePos() << std::endl;
        std::string_view str = "This is a string\n";

        for(int i = 0; i < 10 && file.WriteString(str); i++)
        {
            std::cout << "Write used" << std::endl;
        }

        fflush(ToTester(file).FilePtr);

        File::StatType stats;
        if(file.GetFileStatics(stats) == false)
        {
            std::cout << "Error retrieving stats" << std::endl;
            return;
        }


        LOGV(0, log, file.GetFilePos());

        LOG(0, log, "Moving before begin      - " "Default file move: ", file.SetFilePos(-1, File::OriginPosition::Beggining));

        LOGV(0, log, file.GetFilePos());

        LOG(0, log, "Moving past end          - " "Default file move: ", file.SetFilePos(10, File::OriginPosition::End));

        LOGV(0, log, file.GetFilePos());

        LOG(0, log, "Moving before begin from cur - " "Default file move: ", file.SetFilePos(-1000, File::OriginPosition::CurrentPosition));

        LOGV(0, log, file.GetFilePos());

        LOG(0, log, "Moving past end from cur - " "Default file move: ", file.SetFilePos(1000, File::OriginPosition::CurrentPosition));

        LOGV(0, log, file.GetFilePos());

        std::cout << "File lenght:   " << stats.st_size  << std::endl;
        std::cout << "File st_atime: " << stats.st_atime << std::endl;
        std::cout << "File st_ctime: " << stats.st_ctime << std::endl;
        std::cout << "File st_dev:   " << stats.st_dev   << std::endl;
        std::cout << "File st_gid:   " << stats.st_gid   << std::endl;
        std::cout << "File st_ino:   " << stats.st_ino   << std::endl;
        std::cout << "File st_mode:  " << stats.st_mode  << std::endl;
        std::cout << "File st_mtime: " << stats.st_mtime << std::endl;
        std::cout << "File st_nlink: " << stats.st_nlink << std::endl;
        std::cout << "File st_rdev:  " << stats.st_rdev  << std::endl;
        std::cout << "File st_uid:   " << stats.st_uid   << std::endl;
    }

}

#endif // FILE_H
