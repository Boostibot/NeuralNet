#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include "CharSupport.h"
#include "UniversalString.h"
#include "OpenMode.h"

namespace CIo
{
    //Class responsible for safe management of the FILE pointer
    template<typename OsCharTypeArg>
    class BasicCFileManager
    {
        public:
            using ThisType       = BasicCFileManager;
            using OsCharType     = OsCharTypeArg;

        protected:
            using CharSupport    = CharSupport<OsCharType>;

        public:
            using OpenMode       = BasicOpenMode<OsCharType>;

            template<typename CharType>
            using String        = std::basic_string<CharType>;
            template<typename CharType>
            using StringView    = std::basic_string_view<CharType>;
            template<typename CharType>
            using CStringRef    = CStringRef<CharType>;

            using OsString      = String<OsCharType>;
            using OsStringView  = StringView<OsCharType>;
            using OsCStringRef  = CStringRef<OsCharType>;

            static_assert (CharSupport::IsValid, "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        public:
            FILE PTR FilePtr;

        public:
            BasicCFileManager() noexcept : FilePtr(nullptr)
            {}

            BasicCFileManager(const OsStringView path, const OpenMode REF openMode) noexcept : FilePtr(nullptr)
            {
                OpenNew(path, openMode);
            }

            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            BasicCFileManager(const OsStringView path, OpenModeTypes ... openModes) noexcept : FilePtr(nullptr)
            {
                OpenNew(path, openModes...);
            }

        public:
            BasicCFileManager(const ThisType REF) = delete;
            BasicCFileManager(ThisType RVALUE_REF other) noexcept
            {
                this->FilePtr = other.FilePtr;
                other.FilePtr = nullptr;
            }

        public:
            ThisType REF operator=(const ThisType REF) = delete;
            ThisType REF operator=(ThisType RVALUE_REF other) noexcept
            {
                BasicCFileManager temp(std::move(other));
                this->Swap(temp);

                return POINTER_VALUE(this);
            }

            explicit inline operator bool() const noexcept
            {
                return this->IsOpen();
            }

        public:
            ~BasicCFileManager() noexcept
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
            bool OpenNew(const OsStringView path, const OpenMode REF openMode) noexcept
            {
                if(NOT openMode.IsValid())
                    return false;

                //When opening with ReadWriteMustExist the file must be openable
                // else the behaviour is undefined -> explicit check if the file can eb opened
                if(openMode.GetCOpenMode() == COpenMode::ReadWriteMustExist)
                {
                    if(ThisType::IsFileOpenable(path) == false)
                        return false;
                }

                return OpenNewInternal(path, openMode);
            }
            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline bool OpenNew(const OsStringView path, OpenModeTypes ... openModes) noexcept
            {
                return OpenNew(path, OpenMode(openModes...));
            }

        private:
            bool OpenNewInternal(const OsStringView path, const OsStringView REF openMode) noexcept
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

                if(path.empty())
                    return false;

                //The checking as above for the return of the function can be considered redundant
                // This function will not be used for checking if file was closed succesfully but rather for opening
                // a new one. If such behaviour is desired one should assure that it was infact the closing that
                // caused the error not the opening. Such checking can be done through the Close() function.
                // This implementation is faster and equally as convinient
                ThisType::CloseFile(this->FilePtr);

                //No need to check for return state - will be checked at the end through IsOpen
                FilePtr = CharSupport::fopen(path.data(), openMode.data());

                //if(this->IsOpen())
                //    FilePtr = CharSupport::freopen(path.data(), arguments.data(), FilePtr);
                //else
                //    FilePtr = CharSupport::fopen(path.data(), arguments.data());

                return IsOpen();
            }


        public:
            inline bool IsOpen() const noexcept {return (FilePtr != nullptr);}
            inline bool IsClosed() const noexcept {return (FilePtr == nullptr);}

        private:
            inline static bool CloseFile(FILE PTR filePtr) noexcept
            {
                if(filePtr == nullptr)
                    return true;

                //returns if the closing happened successfully
                // (0 = success)
                return (fclose(filePtr) == 0);
            }
        public:
            static bool IsFileOpenable(const OsStringView REF filename) noexcept
            {
                constexpr OpenMode readMode = COpenMode::ReadMustExist;

                ThisType file;
                //OpenOpenNewInternal is slightly faster since it doenst check the openmode
                return file.OpenNewInternal(filename, readMode);
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
}
namespace std
{
    template <typename OsCharT>
    HEADER_ONLY void swap (CIo::BasicCFileManager<OsCharT> REF file1, CIo::BasicCFileManager<OsCharT> REF file2) noexcept
    {
        file1.Swap(file2);
    }
}
namespace CIo
{
    using CFileManager  = BasicCFileManager<char8>;
    using WCFileManager = BasicCFileManager<charW>;
}
#endif // CFILEMANAGER_H
