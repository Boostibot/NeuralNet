#ifndef UNBUFFEREDFILE_H
#define UNBUFFEREDFILE_H

#include <cstdio>
#include <string>
#include "GClasses/Common/G_Common.h"


//Class responsible for safe management of the FILE pointer
//Is ment to be inherited from if desired to be used since the File pointer
//is declared protected
class CFileManager
{
    public:
        using ThisType = CFileManager;
        using SizeType = size_t;
        using StringView = std::string_view;

    public:
        static constexpr int EndOfFile = EOF;
        static constexpr int FileNamesLenghtMax = FILENAME_MAX;
        static constexpr int OpenFilesMax = FOPEN_MAX;
        static constexpr int TempFileNamesLenghtMin = L_tmpnam;
        static constexpr int TemporaryFilesMax = TMP_MAX;

        static constexpr int FullBufferingCode = _IOFBF;
        static constexpr int LineBufferingCode = _IOLBF;
        static constexpr int NoBufferingCode = _IONBF;

    protected:
        FILE POINTER File;

    public:
        CFileManager() noexcept : File(nullptr) {}
        CFileManager(std::string_view path, std::string_view openMode) noexcept : File(nullptr)
        {
            ReOpen(path, openMode);
        }

    public:
        CFileManager(const ThisType REF) = delete;
        CFileManager(ThisType RVALUE_REF other) noexcept
        {
            this->File = other.File;
            other.File = nullptr;
        }

    public:
        ThisType REF operator=(const ThisType REF) = delete;
        ThisType REF operator=(ThisType RVALUE_REF other) noexcept
        {
            ThisType::CloseFile(this->File); //Slightly faster than calling this->Close() since it does not set the pointer
            this->File = other.File;
            other.File = nullptr;
            return POINTER_VALUE(this);
        }

        explicit operator bool() const noexcept
        {
            return this->IsOpen();
        }

    public:
        ~CFileManager() noexcept
        {
            Close();
        }

    public:
        void Swap(ThisType REF other) noexcept
        {
            std::swap(this->File, other.File);
        }
        bool ReOpen(const StringView path, const StringView arguments) noexcept
        {
            //Slightly faster than calling this->Close() since it does not set the pointer
            // would be a wastfull assignment since in most cases the function wont fail and the file will be
            // set to the newly opened file
            //if(NOT ThisType::CloseFile(this->File))
            //{
            //   //In case the function fails we need to assign the file to nullptr
            //   this->File = nullptr;
            //   return false;
            //}

            //The checking as above for the return of the function can be considered redundant
            // This function will not be used for checking if file was closed succesfully but rather for opening
            // a new one. If such behavious is desired one should assure that it was infact the closing that
            // caused the error not the opening. Such checking can be done through the Close() function.
            // This implementation is faster and equally as convinient
            ThisType::CloseFile(this->File);

            //No need to check for return state - will be checked at the end through IsOpen
            File = fopen(path.data(), arguments.data());

            //This function causes errors on nullptr streams??
            //Causes the output of the app to be corrupted
            //File = freopen(path.data(), arguments.data(), File);

            if(IsClosed())
                return false;

            return DisableBuffering();
        }

    public:
        inline bool IsOpen() const noexcept {return (File != nullptr);}
        inline bool IsClosed() const noexcept {return (File == nullptr);}

    protected:
        inline bool DisableBuffering() noexcept
        {
            return (setvbuf(File, static_cast<char *>(nullptr), ThisType::NoBufferingCode, 0) == 0);
        }
        inline static bool CloseFile(FILE POINTER file) noexcept
        {
            if(file == nullptr)
                return true;

            //returns if the closing happened successfully
            // (0 = success)
            return (fclose(file) == 0);
        }

    public:
        inline bool Close() noexcept
        {
            bool returnStatus = ThisType::CloseFile(this->File);

            //Sets the file to null
            this->File = nullptr;
            return returnStatus;
        }
};

class UnbufferedFile : public CFileManager
{
    public:
        using ThisType = UnbufferedFile;

    public:
        UnbufferedFile() = default;
        UnbufferedFile(const ThisType REF) = delete;
        UnbufferedFile(ThisType RVALUE_REF) = default;
        ThisType REF operator=(const ThisType REF) = delete;
        ThisType REF operator=(ThisType RVALUE_REF) = default;

    public:
        UnbufferedFile(std::string_view path, std::string_view openMode) : CFileManager(path, openMode) {}

    public:
        ~UnbufferedFile() = default;

    protected:
        template<typename PointerType>
        bool WriteUnsafe(PointerType POINTER ptrToData, SizeType count)
        {
            SizeType written = fwrite(ptrToData, sizeof (PointerType), count, File);

            return (written == count);
        }

    public:
        template<typename PointerType>
        bool Write(PointerType POINTER ptrToData, SizeType count)
        {
            if(this->IsOpen())
                return this->WriteUnsafe(ptrToData, count);
            else
                return false;
        }
        template<typename PointerType>
        bool WriteObject(const PointerType PASS_REF variable)
        {
            return Write(ADDRESS(variable), 1);
        }
        bool WriteString(const StringView str)
        {
            return Write(str.data(), str.size());
        }
};

namespace std
{
    //Reimplementation of the swap inside std namespsace
    template <>
    void swap (CFileManager REF file1, CFileManager REF file2) noexcept
    {
        file1.Swap(file2);
    }
    template <>
    void swap (UnbufferedFile REF file1, UnbufferedFile REF file2) noexcept
    {
        file1.Swap(file2);
    }
}

void UnbufferedFileUse()
{
    std::string myStr = "My msg";

    UnbufferedFile file("path", "wb");

    if(NOT file.IsOpen())
        return;

    file.Write(myStr.data(), myStr.size());
    file.WriteString("My msg");

    if(NOT file.ReOpen("path", "wb"))
        return;

    file.Write(myStr.data(), myStr.size());
    file.WriteString("My msg");
}

#endif // UNBUFFEREDFILE_H
