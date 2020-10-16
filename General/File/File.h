#ifndef FILE_H
#define FILE_H

#include "UnsafeFile.h"

//TODO - decide if inline is needed
class File : public UnsafeFile
{
    private:
        using ThisType          = File;

    public:
        using UnsafeFile        = UnsafeFile;
        using CFileManager      = CFileManager;

        /*
    public:
        using SizeType          = typename UnsafeFile::SizeType;
        using FileSizeType      = typename UnsafeFile::FileSizeType;
        using PosType           = typename UnsafeFile::PosType;
        using OriginPosition    = typename UnsafeFile::OriginPosition;
        using BufferingCode     = typename UnsafeFile::BufferingCode;
        using FileDescriptor    = typename UnsafeFile::FileDescriptor;
        using Stats             = typename UnsafeFile::Stats;
        */

    private:
        using UnsafeFile::FilePtr;

    public:
        File() = default;
        File(const ThisType REF) = delete;
        File(ThisType RVALUE_REF) = default;
        ThisType REF operator=(const ThisType REF) = delete;
        ThisType REF operator=(ThisType RVALUE_REF) = default;

    public:
        inline File(const OsStringView path, const OsStringView openMode) : UnsafeFile(path, openMode)
        {}
        template<typename ... OpenModeTypes,
                 std::enable_if_t<MetaPrograming::AreTypesSameTo<typename UnsafeFile::CFileManager::OpenModeFlag, OpenModeTypes...>::value, int> = 0>
        inline File(const OsStringView path, OpenModeTypes ... openModes) noexcept : UnsafeFile(path, openModes...)
        {}


    public:
        inline UnsafeFile REF GetUnsafeFile() noexcept
        {
            return static_cast<UnsafeFile REF>(PTR_VAL(this));
        }
        inline const UnsafeFile REF GetUnsafeFile() const noexcept
        {
            return static_cast<const UnsafeFile  REF>(PTR_VAL(this));
        }


    public:
        inline bool WasLastErrorEndOfFile() const noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().WasLastErrorEndOfFile();
        }


    public:
        inline PosType GetPosInFile() noexcept
        {
            if(this->IsClosed())
                return static_cast<PosType>(0);

            return this->GetUnsafeFile().GetPosInFile();
        }

        inline bool SetPosInFile(PosType pos, OriginPosition from = OriginPosition::Beggining) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().SetPosInFile(pos, from);
        }
        inline void MoveToBegging() noexcept
        {
            if(this->IsClosed())
                return;

            this->GetUnsafeFile().MoveToBegging();
        }
        inline void MoveToEnd() noexcept
        {
            if(this->IsClosed())
                return;

            return this->GetUnsafeFile().MoveToEnd();
        }
        inline bool MoveBy(PosType by) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().MoveBy(by);
        }


    public:
        template<typename PointerType>
        [[nodiscard]] inline bool Write(PointerType POINTER ptrToData, SizeType count) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().Write(ptrToData, count);
        }
        template<typename PointerType>
        [[nodiscard]] inline SizeType WriteAndCount(PointerType POINTER ptrToData, SizeType count) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().WriteAndCount(ptrToData, count);
        }
        template<typename ObjectType>
        [[nodiscard]] inline bool WriteObject(ObjectType RVALUE_REF object) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().WriteObject(object);
        }
        [[nodiscard]] inline bool WriteString(const OsStringView str) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().WriteString(str);
        }


    public:
        template<typename PointerType>
        [[nodiscard]] inline bool Read(PointerType POINTER ptrToBuffer, SizeType count) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().Read(ptrToBuffer, count);
        }

        template<typename PointerType>
        [[nodiscard]] inline SizeType ReadAndCount(PointerType POINTER ptrToBuffer, SizeType count) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().ReadAndCount(ptrToBuffer, count);
        }
        [[nodiscard]] inline bool ReadString(OsString REF to, u32 lenght) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().ReadString(to, lenght);
        }

    public:
        template<typename ObjectType>
        [[nodiscard]] inline bool ReadObject(ObjectType RVALUE_REF object) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().ReadObject(object);
        }


    public:
        inline bool SetBuffer(void POINTER bufferPtr, SizeType bufferSize, BufferingCode mode) noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().SetBuffer(bufferPtr, bufferSize, mode);
        }
        inline void Flush() noexcept
        {
            if(this->IsClosed())
                return;

            return this->GetUnsafeFile().Flush();
        }
        inline void SwitchBetweenReadAndWrite() noexcept
        {
            if(this->IsClosed())
                return;

            return this->GetUnsafeFile().SwitchBetweenReadAndWrite();
        }

    public:
        inline FileDescriptor GetFileDescriptor() const noexcept
        {
            if(this->IsClosed())
                return UnsafeFile::GetErrorFileDescriptor();

            return this->GetUnsafeFile().GetFileDescriptor();
        }

        inline bool GetFileStatics(Stats REF stats) const noexcept
        {
            if(this->IsClosed())
                return false;

            return this->GetUnsafeFile().GetFileStatics(stats);
        }

        FileSizeType GetFileSize() const noexcept
        {
            if(this->IsClosed())
                return 0;

            return this->GetUnsafeFile().GetFileSize();
        }
};


/*
void CFileUse()
{
    File::OsString path = "path.txt";
    File::CFileManager manager;
    File::UnsafeFile unsafe;
    File file2;

    bool status = true;

    constexpr File::OpenMode openMode = File::GetOpenMode(File::OpenModeFlag::Write, File::OpenModeFlag::Text, File::OpenModeFlag::MustExist);
    static_assert (openMode.IsValid(), "Invalid arguments");

    file2.OpenNew("file.log", "wb");
    file2.OpenNew("file.log", openMode);
    file2.OpenNew("file.log", File::GetOpenMode<File::OpenModeFlag::Write, File::OpenModeFlag::MustExist>());
    file2.OpenNew("file.log", File::OpenModeFlag::Write, File::OpenModeFlag::MustExist);

    File file(path, "wb");

    if(NOT file.IsOpen())
        return;


    std::string myStr = "My msg";
    status = file.Write(myStr.data(), myStr.size());
    status = file.WriteString("My msg");

    if(NOT file.OpenNew("path", "wb"))
        return;

    status = file.Write(myStr.data(), myStr.size());
    status = file.WriteString("My msg");

    (void)status;
}

enum ReturnState
{
    Error,
    Success
};

void UseString(std::string_view)
{

}

template<typename T>
void FuncSomething(T arg)
{
    static_assert (std::is_convertible<T, std::string_view>::value, "is not conevrtibe;");
    UseString(arg);
}


ReturnState DoSomethingWithFile2(std::string fileName,
                                 unsigned int max,
                                 unsigned int min)
{
    FuncSomething(fileName);
    unsigned int data;
    File file;
    File::PosType beforeReadPos;
    File::OpenMode openMode = File::GetOpenMode(File::OpenModeFlag::Read,
                                                File::OpenModeFlag::Write,
                                                File::OpenModeFlag::Binary,
                                                File::OpenModeFlag::MustExist);

    file.OpenNew(fileName, openMode);

    if(file.IsClosed())
        return Error;

    while(true)
    {
        beforeReadPos = file.GetPosInFile();

        if(file.ReadObject(data) == false)
            break;

        if(data > max)
        {
            file.SetPosInFile(beforeReadPos);
            if(file.WriteObject(max) == false)
                break;
        }
        else if(data < min)
        {
            file.SetPosInFile(beforeReadPos);
            if(file.WriteObject(min) == false)
                break;
        }
    }

    if(file.WasLastErrorEndOfFile())
        return Success;
    else
        return Error;
}

bool DoSomethingWithCFile2(std::string fileName,
                           unsigned int max,
                           unsigned int min)
{

    unsigned int data;
    FILE * file;

    fopen_s(ADDRESS(file), fileName.data(), "r+b");

    if(file == nullptr)
        return Error;

    while(true)
    {
        auto beforeReadPos = _ftelli64(file);

        if(fread_s(ADDRESS(data), sizeof (unsigned int), sizeof (unsigned int), 1, file) != 1)
            break;

        if(data > max)
        {
            _fseeki64(file, beforeReadPos, SEEK_SET);
            fwrite(ADDRESS(max), sizeof (unsigned int), 1, file);
        }
        else if(data < min)
        {
            _fseeki64(file, beforeReadPos, SEEK_SET);
            fwrite(ADDRESS(min), sizeof (unsigned int), 1, file);
        }
    }

    if(feof(file))
        return Success;
    else
        return Error;
}
*/




#endif // FILE_H
