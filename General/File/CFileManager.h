#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include "CharSupport.h"
#include "OpenMode.h"

//Class responsible for safe management of the FILE pointer
class CFileManager : public OpenModeHolder
{
    public:
        template<typename CharType>
        using String        = std::basic_string<CharType>;
        template<typename CharType>
        using StringView    = std::basic_string_view<CharType>;
        template<typename CharType>
        using CString       = CharType *;

        using OsCharType    = ::OsCharType;
        using OsString      = String<OsCharType>;
        using OsStringView  = StringView<OsCharType>;
        using OsCString     = CString<OsCharType>;

    protected:
        using ThisType      = CFileManager;
        using CharSupport   = CharTypeSupport;

        //static_assert (CharSupport::Is8bit OR CharSupport::Is16bit OR CharSupport::Is32bit OR CharSupport::IsWide, "CharTypeDependendt : Unsupported char type; only 8bit, 16bit and 32bit characters allowed");

    public:
        FILE POINTER FilePtr;

    public:
        CFileManager() noexcept : FilePtr(nullptr) {}

        CFileManager(const OsStringView path, const OsStringView openMode) noexcept : FilePtr(nullptr)
        {
            OpenNew(path, openMode);
        }

        template<typename ... OpenModeTypes,
                 std::enable_if_t<MetaPrograming::AreTypesSameTo<OpenModeFlag, OpenModeTypes...>::value, int> = 0>
        CFileManager(const OsStringView path, OpenModeTypes ... openModes) noexcept : FilePtr(nullptr)
        {
            OpenNew(path, openModes...);
        }

    public:
        CFileManager(const ThisType REF) = delete;
        CFileManager(ThisType RVALUE_REF other) noexcept
        {
            this->FilePtr = other.FilePtr;
            other.FilePtr = nullptr;
        }

    public:
        ThisType REF operator=(const ThisType REF) = delete;
        ThisType REF operator=(ThisType RVALUE_REF other) noexcept
        {
            //TODO - test speed and compiled code
            CFileManager temp(std::move(other));

            this->Swap(temp);

            return POINTER_VALUE(this);
        }
        /*
        ThisType REF operator=(ThisType RVALUE_REF other) noexcept
        {
            if(this == ADDRESS(other))
                return POINTER_VALUE(this);

            ThisType::CloseFile(this->FilePtr); //Slightly faster than calling this->Close() since it does not set the pointer
            this->FilePtr = other.FilePtr;
            other.FilePtr = nullptr;
            return POINTER_VALUE(this);
        }
        */

        explicit inline operator bool() const noexcept
        {
            return this->IsOpen();
        }

    public:
        ~CFileManager() noexcept
        {
            Close();
        }

    public:
        inline void Swap(ThisType REF other) noexcept
        {
            std::swap(this->FilePtr, other.FilePtr);
        }
        inline friend void Swap(ThisType REF first, ThisType REF second) noexcept
        {
            first.Swap(second);
        }

        bool OpenNew(const OsStringView path, const OsStringView arguments) noexcept
        {
            //Slightly faster than calling this->Close() since it does not set the pointer
            // would be a wastfull assignment since in most cases the function wont fail and the file will be
            // set to the newly opened file
            //if(NOT ThisType::CloseFile(this->FilePtr))
            //{
            //   //In case the function fails we need to assign the filePtr to nullptr
            //   this->FilePtr = nullptr;
            //   return false;
            //}

            //The checking as above for the return of the function can be considered redundant
            // This function will not be used for checking if file was closed succesfully but rather for opening
            // a new one. If such behavious is desired one should assure that it was infact the closing that
            // caused the error not the opening. Such checking can be done through the Close() function.
            // This implementation is faster and equally as convinient
            ThisType::CloseFile(this->FilePtr);

            //No need to check for return state - will be checked at the end through IsOpen
            FilePtr = CharSupport::fopen(path.data(), arguments.data());

            //This function causes errors on nullptr streams??
            //Causes the output of the app to be corrupted
            //FilePtr = freopen(path.data(), arguments.data(), FilePtr);

            return IsOpen();
        }

        template<typename ... OpenModeTypes,
                 std::enable_if_t<MetaPrograming::AreTypesSameTo<OpenModeFlag, OpenModeTypes...>::value, int> = 0>
        inline bool OpenNew(const OsStringView path, OpenModeTypes ... openModes)
        {
            return OpenNew(path, OpenMode::GetOpenMode(openModes...));
        }

    public:
        inline bool IsOpen() const noexcept {return (FilePtr != nullptr);}
        inline bool IsClosed() const noexcept {return (FilePtr == nullptr);}

    protected:
        inline static bool CloseFile(FILE POINTER filePtr) noexcept
        {
            if(filePtr == nullptr)
                return true;

            //returns if the closing happened successfully
            // (0 = success)
            return (CharSupport::fclose(filePtr) == 0);
        }

    public:
        bool Close() noexcept
        {
            bool returnStatus = ThisType::CloseFile(this->FilePtr);

            //Sets the file to null
            this->FilePtr = nullptr;
            return returnStatus;
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
}

#endif // CFILEMANAGER_H
