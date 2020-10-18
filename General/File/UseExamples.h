#ifndef USEEXAMPLES_H
#define USEEXAMPLES_H

#include "File.h"
#include "UtfConversion.h"


using MFile = BasicFile<utf8>;

void CFileUse()
{
    MFile::OsString path = "path.txt";
    MFile::CFileManager manager;
    MFile::UnsafeFile unsafe;
    MFile file2;

    bool status = true;

    constexpr MFile::OpenMode openMode = MFile::GetOpenMode(MFile::OpenModeFlag::Write, MFile::OpenModeFlag::Text, MFile::OpenModeFlag::MustExist);
    static_assert (openMode.IsValid(), "Invalid arguments");

    file2.OpenNew("file.log", "wb");
    file2.OpenNew("file.log", openMode);
    file2.OpenNew("file.log", MFile::GetOpenMode<MFile::OpenModeFlag::Write, MFile::OpenModeFlag::MustExist>());
    file2.OpenNew("file.log", MFile::OpenModeFlag::Write, MFile::OpenModeFlag::MustExist);

    MFile file(path, "wb");

    if(NOT file.IsOpen())
        return;


    std::basic_string<utf8> utf8Str = "My msg";
    std::basic_string<utf16> utf16Str = u"My msg";
    std::basic_string<utf32> utf32Str = U"My msg";
    std::basic_string<utfW> utfWStr = L"My msg";

    status = file.Write(utf8Str.data(), utf8Str.size());
    status = file.Write(utf16Str.data(), utf16Str.size());

    status = file.WriteString(utf8Str);
    status = file.WriteString(utf16Str);
    status = file.WriteString(utf32Str);
    status = file.WriteString(utfWStr);


    if(status == false)
        return;

    if(NOT file.OpenNew("path", "wb"))
        return;
}


void ProcessChunk(byte[], size_t) {}

bool ReadFileByChunks(MFile::OsStringView path) noexcept
{
    constexpr size_t chunkSize = 1024;
    constexpr MFile::OpenMode openMode = MFile::GetOpenMode(MFile::OpenModeFlag::Read, MFile::OpenModeFlag::Binary, MFile::OpenModeFlag::MustExist);
    static_assert (openMode.IsValid(), "Invalid arguments");

    MFile file;
    byte chunk[chunkSize];

    file.OpenNew(path, openMode);

    while(file.Read(chunk, chunkSize) == true)
        ProcessChunk(chunk, chunkSize);

    if(file.WasLastErrorEndOfFile())
        return true;
    else
        return false;
}

void UpdateProgress(MFile::PosType current, MFile::PosType max)
{
    const double percentage = static_cast<double>(max) / static_cast<double>(current);
    (void)percentage;
}

//Note that this function can be declared noexcept since all
// functions in the File (BasicFile<>) class are noexcept
bool ReadFileWithProgress(MFile::OsStringView path) noexcept
{

    constexpr size_t chunkSize = 1024;

    //We create OpenMode with the desired flags
    constexpr MFile::OpenMode openMode(MFile::OpenModeFlag::Read, MFile::OpenModeFlag::Binary, MFile::OpenModeFlag::MustExist);
    //We can check at compile time if the arguments are valid
    //(ie. if it does not contain any errors such having both MustExist and MustNotExist flags)
    static_assert (openMode.IsValid(), "Invalid arguments");
    //We can also check if the current combination of arguments are supported
    // (ie. not invalid but unimplmented due to restriction by the cstdio)
    // (this happens every time both MustExist and Append flags are present)
    static_assert (openMode.IsSupported(), "Unsupported arguments");

    MFile file;
    byte chunk[chunkSize];

    //Open a new file given the provided (relative) path
    // and the open mode
    file.OpenNew(path, openMode);

    //If one wants to use the standard cstdio open mode
    // it can be done normally ie. file.OpenNew(path, "rb")

    //We check if the file was opened sucessfully
    if(file.IsClosed())
        return false;

    //We get the maximum position by moving to end and getting it
    file.MoveToEnd();
    const MFile::PosType maxPos = file.GetPosInFile();
    //We restore the pos to the beggining afterwards
    file.MoveToBegging();

    //Please note that maxPos does neccessarly have to correspond
    // with the file size; This can be the result of opening the file
    // in OpenModeFlag::Text or with OpenModeFlag::Unicode
    // To get the actual file size use File::GetFileSize()

    //We read the file by using the Read function and checking
    // the return status
    while(file.Read(chunk, chunkSize) == true)
    {
        //We process the data
        ProcessChunk(chunk, chunkSize);

        //And update the progress comparing the current pos to the maxPos
        UpdateProgress(file.GetPosInFile(), maxPos);
    }

    //Check for the reason why the while loop stopped
    // if the cause was that the whole file is was read
    // (end of file) the process executed successfully
    //
    // if the cause was anything else process executed
    // with error
    if(file.WasLastErrorEndOfFile())
    {
        UpdateProgress(maxPos, maxPos); //Sets progress to 100%
        return true;
    }
    else
        return false;
}

bool ReadFileFastest(MFile::OsStringView path) noexcept
{
    constexpr size_t chunkSize = 4096;
    constexpr MFile::OpenMode openMode(MFile::OpenModeFlag::Read, MFile::OpenModeFlag::Binary, MFile::OpenModeFlag::MustExist);
    static_assert (openMode.IsValid(), "Invalid arguments");

    //We use UnsafeFile instead of File
    // UnsafeFile behaves the same way as File
    // only doesnt check if the C FILE pointer
    // is null
    // (ie. using most of the function on unitialised
    //   will cause a SEGFAULT)

    MFile::UnsafeFile file;
    byte chunk[chunkSize];

    //We need to check if the file was opened
    // since none of the function check in themselfs
    if(file.OpenNew(path, openMode) == false)
        return false;

    //We can also use:
    //if(file.IsClosed())
    //    return false;

    //We set the buffering to None
    // (in this case the first two parameters dont matter)
    file.SetBuffer(nullptr, 0, MFile::BufferingCode::None);

    //We read the file normally and process the data
    while(file.Read(chunk, chunkSize))
        ProcessChunk(chunk, chunkSize);

    //We check the last error and exit normally
    return file.WasLastErrorEndOfFile();
    //This is the same as the last checking of error
    // only in single line
}

bool ReadFileBackwards(MFile::OsStringView path) noexcept
{
    constexpr size_t chunkSize = 4096;
    //Using this notation the open mode is checked inside the GetOpenMode function
    // please note that this creates a specialisation of the function
    constexpr MFile::OpenMode openMode = MFile::GetOpenMode<MFile::OpenModeFlag::Read, MFile::OpenModeFlag::Binary, MFile::OpenModeFlag::MustExist>();

    MFile::UnsafeFile file;
    byte chunk[chunkSize];

    //We open the file normally
    if(file.OpenNew(path, openMode) == false)
        return false;

    //We move to the end of the file
    file.MoveToEnd();

    //We move negative (backwards) chunksize
    file.MoveBy(- static_cast<MFile::SizeType>(chunkSize));

    //Both of the above functons can be fused into the following one:
    //file.SetPosInFile(- static_cast<MFile::SizeType>(chunkSize), MFile::OriginPosition::End);
    //This is faster because the file is moved only once instead of two

    //We set the buffering to None
    file.SetBuffer(nullptr, 0, MFile::BufferingCode::None);

    //We read the file normally and process the data
    while(true)
    {
        if(file.Read(chunk, chunkSize) == false)
            break;

        ProcessChunk(chunk, chunkSize);

        //We move backwards the size of chunk TWO TIMES
        // this is because the File::Read function automatically
        // moves after the read data
        // (this is cstdio restriction and can not be changed)
        file.MoveBy(- static_cast<MFile::SizeType>(chunkSize) * 2);
    }

    //We check the last error and exit normally
    return file.WasLastErrorEndOfFile();
}

#include "UtfConversion.h"
bool WorkingWithFilesInUtf(std::string_view path) //NOT!!! noexcept
{
    //When working with files in UTF there are few important things to note:
    //
    // 1. ALWAYS OPEN IN BINARY MODE (see below for more explanation)
    // 2. Open files with UnicodeEncoding flag (this skips the unicode BOM)
    // 3. Only char and wchar_t can be used to comunicate with the OS (opening, renaming, deleting files) (see below)
    //

    //IMPORTANT: even though these functions operate with strings they do not use
    // the cstdio string functions as they cannot be used with other string types than
    // utf8 (char) string;
    // As such they might behave incorectly when the file not opened with the OpenModeFlag::Binary

    //*****************************************************************//
    //                                                                 //
    //        ALWAYS OPEN FILES WITH THE OpenModeFlag::Binary          //
    //                                                                 //
    //*****************************************************************//

    // OS interaction and utf
    //************************
    //
    // The cstdio library only uses utf8 (char) to interact with the OS but there are also
    // expansions for wchar_t support.
    //
    // Only one type can be used at a time as specified with BasicFile<char / wchar_t>.
    // The File class is just a template specialisation of BasicFile with the OsCharType
    // This type can be changed with the following defines that must be defined before including "File.h":
    //
    //   #define FILE_OS_INTERACTON_UTF_WIDE //wchat_t
    //   #define FILE_OS_INTERACTON_UTF_8    //char
    //
    // If possible use the utf8 version. The utfWide (note that not necesarly the same as utf16) is present just for
    // Windows support which doesnt use utf8 for file system.
    //
    // If we do not want to worry about the Os interaction beyond the IO layer we can include
    // the "UtfConversion.h" header and use the conversion functions from any string type.
    // this is showed below:

    using WFile = BasicFile<wchar_t>;

    const std::wstring osPath = ConvertToUtfWide(path);

    //If we are simply using the File and chnaging the whole interaction layers using the defines
    // we can also use:
    //const File::OsString osPath = ConvertToOsUtf(path);

    constexpr WFile::OpenMode openMode = WFile::GetOpenMode<WFile::OpenModeFlag::Read, WFile::OpenModeFlag::Write, WFile::OpenModeFlag::Binary,
            //Unicode flag makes the file automatically skip the BOM mark
            WFile::OpenModeFlag::UnicodeEncoding>();

    WFile file;
    file.OpenNew(osPath, openMode);

    // File function interacting with the OS are:
    // - File::File()
    // - File::OpenNew(...)
    // - File::CreateDirectory(...)
    // - File::CreateFile(...)
    // - File::RenameFile(...)
    // - File::RemoveFile(...)
    // - File::GetFileStatics(...) //Static overload version
    // - File::GetFileSize(...) //Static overload version
}

bool WriteFileUtf(MFile::OsStringView path)
{
    constexpr MFile::OpenMode openMode = MFile::GetOpenMode<MFile::OpenModeFlag::Read, MFile::OpenModeFlag::Write, MFile::OpenModeFlag::Binary,
            //Unicode flag makes the file automatically skip the BOM mark
            MFile::OpenModeFlag::UnicodeEncoding>();


    //Note utf8, utf16, utf32 and utfW are just char, char16_t, char32_t and wchar_t respectively
    const std::basic_string<utf8> utf8Str     = "My msg";
    const std::basic_string<utf16> utf16Str   = u"My msg";
    const std::basic_string<utf32> utf32Str   = U"My msg";
    const std::basic_string<utfW> utfWStr     = L"My msg";

    const std::basic_string_view<utf8> utf8StrView     = "My msg";
    const utf16 * utf16CStr = u"My msg";

    MFile::UnsafeFile file;

    //We open the file normally
    if(file.OpenNew(path, openMode) == false)
        return false;

    //(Is used for silencing the [[nodiscard]] attribute)
    bool status;

    //We can use the WriteString function to write the string
    // the argument can be any string
    // (std::string, std::string_view, c style string, string literal)
    status = file.WriteString(utf8Str);
    status = file.WriteString(utf16Str);
    status = file.WriteString(utf32Str);
    status = file.WriteString(utfWStr);

    status = file.WriteString(utf8StrView);

    status = file.WriteString(utf16CStr);

    status = file.WriteString(U"My msg");



    //Note that these function simply wrap around the File::Write funcion
    // as such the following two lines are equivalent:
    status = file.WriteString(utf8Str);
    status = file.Write(utf8Str.data(), utf8Str.size());

    return file.WasLastErrorEndOfFile();
}


#endif // USEEXAMPLES_H
