#ifndef STATICFILEFUNCTIONSTEST_H
#define STATICFILEFUNCTIONSTEST_H

/*
#include "Catch2/Catch.hpp"
#include "General/File/StaticFileFunctions.h"

#define StaticFileFunctions_TEST_COMPILE_ERROR false
#define StaticFileFunctionsTestedTypes char8, charW
namespace CIo::StaticFileFunctionsTesting
{
    constexpr i32 filenameSize = StaticFileFunctions<char8>::TempFileNameMaxLenght;
}
namespace CIo::StaticFileFunctionsTesting
{
    TEST_CASE("[StaticFileFunctions] : Only char8 amd charW (char and wchar_t) specialisations should compile", "[StaticFileFunctions]")
    {
        [[maybe_unused]] StaticFileFunctions<char8> static8;
        [[maybe_unused]] StaticFileFunctions<charW> staticW;

        #if CFileManager_TEST_COMPILE_ERROR == true
        //Should not compile
        //[[maybe_unused]] StaticFileFunctions<int> staticInt;
        //[[maybe_unused]] StaticFileFunctions<double> staticD;
        //[[maybe_unused]] StaticFileFunctions<short> staticShort;
        //[[maybe_unused]] StaticFileFunctions<long> staticLing;
        //[[maybe_unused]] StaticFileFunctions<std::string> staticString;
        //[[maybe_unused]] StaticFileFunctions<std::wstring> staticWString;
        #endif
    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : GetUniqueTempFileName should create a unique filename", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using CharType = TestType;
        using StringRef = typename StaticFunctions::OsCStringRef;


        CharType filename[filenameSize];

        WHEN("is called with valid sized ref should succeed")
        {
            REQUIRE(StaticFunctions::GetUniqueTempFileName(StringRef(filename, filenameSize)) == true);

            WHEN("is called more than once should create different names")
            {
                CharType filenameNew[StaticFunctions::TempFileNameMaxLenght];
                REQUIRE(StaticFunctions::GetUniqueTempFileName(StringRef(filenameNew, filenameSize)) == true);

                bool differ = false;
                for(u32 i = 0; i < filenameSize; i++)
                    if(filename[i] == filenameNew[i])
                    {
                        differ = true;
                        break;
                    }

                REQUIRE(differ == true);
            }
        }

        WHEN("is called with ref of unsufficient size should fail and return false")
        {
            REQUIRE(StaticFunctions::GetUniqueTempFileName(StringRef(filename, 0)) == false);
        }
    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : IsFileOpenable should return true if the file could be opened", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using CharType = TestType;
        using StringRef = typename StaticFunctions::OsCStringRef;

        CharType filename[filenameSize];
        const StringRef filenameRef(filename, filenameSize);
        StaticFunctions::GetUniqueTempFileName(filenameRef);

        WHEN("a file does not exist it should not be openable")
        {
            REQUIRE(StaticFunctions::IsFileOpenable(filenameRef) == false);
        }
        WHEN("a file does exist it should be openable")
        {
            //Creating the file should not fail
            CHECK(StaticFunctions::CreateFile(filename) == true);
            CHECK(StaticFunctions::IsFileOpenable(filenameRef) == true);

            //Cleanup
            StaticFunctions::RemoveFile(filenameRef);
        }


        WHEN("provided with invalid filename a file should not be openable")
        {
            CharType filename[] = {'\0'};
            REQUIRE(StaticFunctions::IsFileOpenable(filename) == false);
        }

        WHEN("a file does exist it should be acessible - independent")
        {
            FILE PTR file = CharSupport<TestType>::fopenWrite(filename);
            REQUIRE(file != nullptr);
            fclose(file);

            CHECK(StaticFunctions::IsFileOpenable(filenameRef) == true);

            //Cleanup
            StaticFunctions::RemoveFile(filenameRef);
        }

    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : CreateDirectory should create an empty directory", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using CharType = TestType;
        using StringRef = typename StaticFunctions::OsCStringRef;

    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : RemoveEmptyDirectory should remove an empty directory", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using StringRef = typename StaticFunctions::OsCStringRef;

    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : CreateFile shoudl create an empty file", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using CharType = TestType;
        using StringRef = typename StaticFunctions::OsCStringRef;

        WHEN("provided with proper filename a file should be created")
        {
            //Setup
            CharType filename[filenameSize];
            const StringRef filenameRef(filename, filenameSize);
            REQUIRE(StaticFunctions::GetUniqueTempFileName(filenameRef) == true);

            CHECK(StaticFunctions::CreateFile(filenameRef) == true);
            CHECK(StaticFunctions::IsFileOpenable(filename) == true);

            //
            if(StaticFunctions::IsFileOpenable(filename))
            {
                //Do something
            }
            else if (StaticFunctions::CreateFile(filenameRef))
            {
                //Do somethign with created
            }
            else
            {
                //Error
            }


            WHEN("attempting to create a already created file it should still succeed but not override anything written in it")
            {
                FILE PTR file = CharSupport<TestType>::fopenAppend(filename);
                REQUIRE(file != nullptr);

                int buffer[3] = {1, 2, 3};

                fwrite(buffer, 3, sizeof (int), file);
            }

            StaticFunctions::RemoveFile(filenameRef);
        }

        WHEN("provided with invalid filename a file should not be created")
        {
            CharType filename[] = {'\0'};
            REQUIRE(StaticFunctions::CreateFile(filename) == false);
        }
    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : RenameFile should rename file", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using CharType = TestType;
        using StringRef = typename StaticFunctions::OsCStringRef;

    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : RemoveFile should remove file", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using CharType = TestType;
        using StringRef = typename StaticFunctions::OsCStringRef;

        CharType filename[filenameSize];
        const StringRef filenameRef(filename, filenameSize);
        REQUIRE(StaticFunctions::GetUniqueTempFileName(filenameRef) == true);

        WHEN("provided with proper filename specifying file it should get removed")
        {
            REQUIRE(StaticFunctions::CreateFile(filenameRef) == true);
            REQUIRE(StaticFunctions::RemoveFile(filenameRef) == true);

            //FILE PTR file = CharSupport<TestType>::fopenSimpleMode(filename, 'r');
        }

        WHEN("provided with invalid filename a file should not get removed")
        {
            CharType filename[] = {'\0'};
            REQUIRE(StaticFunctions::CreateFile(filename) == false);
        }
    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : GetFileStatics should save file statistics", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using CharType = TestType;
        using StringRef = typename StaticFunctions::OsCStringRef;

    }

    TEMPLATE_TEST_CASE("[StaticFileFunctions] : GetFileSize should shoudl return the size of the file or -1 if the file does not exist", "[StaticFileFunctions]", StaticFileFunctionsTestedTypes)
    {
        using StaticFunctions = StaticFileFunctions<TestType>;
        using CharType = TestType;
        using StringRef = typename StaticFunctions::OsCStringRef;

    }
}
*/



#endif // STATICFILEFUNCTIONS_H
