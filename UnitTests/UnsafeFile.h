#ifndef UNSAFEFILETETSING_H
#define UNSAFEFILETETSING_H
#include "General/File/UnsafeFile.h"
#include "CFileManager.h"
#include <fstream>
namespace UnsafeFileTesting
{
    template<typename CharT>
    struct BasicMockFile
    {
            CFileManagerTesting::BasicFilenameCleaner<CharT> Cleaner;

            BasicMockFile() = default;
            ~BasicMockFile() = default;

            template<typename FunctionType>
            inline bool ChangeFile(FunctionType REF lambda)
            {
                if(Cleaner.HasError())
                    return false;

                BasicCFileManager<CharT> manager;
                if constexpr(Cleaner.IsChar8)
                        manager.OpenNew(Filename(), "w+b");
                else
                    manager.OpenNew(Filename(), L"w+b");

                if(manager.IsClosed())
                    return false;

                return lambda(manager);
            }

            inline auto Filename() {return Cleaner.Filename;}
    };

    using MockFile = BasicMockFile<char8>;
    using WMockFile = BasicMockFile<charW>;
}
namespace UnsafeFileTesting
{
    TEST_CASE("Only char and wchar_t specialisations should compile")
    {
        BasicUnsafeFile<char8> file8;
        BasicUnsafeFile<charW> fileW;
        //Should not compile
        //BasicUnsafeFile<int> fileInt;
        //BasicUnsafeFile<double> fileD;
        //BasicUnsafeFile<short> fileShort;
        //BasicUnsafeFile<long> fileLing;
        //BasicUnsafeFile<std::string> fileString;
        //BasicUnsafeFile<std::wstring> fileWString;
    }

    TEST_CASE("Only char and wchar_t specialisations should compile")
    {
        MockFile mock;
        WMockFile wmock;

        auto fileChange = [](auto& cFileManager)
        {
            int myData[10] = {};
            myData[0] = 100;
            myData[3] = 100;
            myData[9] = 100;
            return fwrite(myData, sizeof (int), 10, cFileManager.FilePtr);
        };

        //Mockign should not fail
        REQUIRE(mock.ChangeFile(fileChange) == true);
        REQUIRE(wmock.ChangeFile(fileChange) == true);
    }
}
#endif // UNSAFEFILETETSING_H
