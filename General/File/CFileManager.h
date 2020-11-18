#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include "StaticFileFunctions.h"
#include "OpenMode.h"

namespace CIo
{
    //Class responsible for safe management of the FILE pointer
    template<typename OsCharTypeArg>
    class BasicCFileManager : public OpenModeHolder
    {
        public:
            using ThisType       = BasicCFileManager;
            using OsCharType     = OsCharTypeArg;

        protected:
            using OpenModeHolder = OpenModeHolder;
            using StaticFileFunctions = StaticFileFunctions<OsCharType>;

        public:
            using OpenMode       = OpenModeHolder::OpenMode;

        public:
            using CharSupport    = typename StaticFileFunctions::CharSupport;

            template<typename CharType>
            using String        = typename StaticFileFunctions::template String<CharType>;
            template<typename CharType>
            using StringView    = typename StaticFileFunctions::template StringView<CharType>;
            template<typename CharType>
            using CString       = typename StaticFileFunctions::template CString<CharType>;

            using OsString      = typename StaticFileFunctions::OsString;
            using OsStringView  = typename StaticFileFunctions::OsString;
            using OsCString     = typename StaticFileFunctions::OsString;

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
                //TODO - test speed and compare compiled code
                BasicCFileManager temp(std::move(other));

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
            //TODO - move IsFileAccessible to here
            //        or move it to its own struct that will be used here
            inline bool OpenNew(const OsStringView path, const OpenMode openMode) noexcept
            {
                if(NOT openMode.IsValid())
                    return false;

                //When opening with ReadWriteMustExist the file must exist
                // else the behaviour is undefined -> explicit check if the file exists
                if(openMode.GetCOpenMode() == COpenMode::ReadWriteMustExist)
                {
                    //If it is impossible to access the file it most likely wont be
                    // possible to open the it -> return false
                    if(StaticFileFunctions::IsFileAccessible(path))
                        return false;
                }

                return OpenNewInternal(path, openMode);
            }

        private:
            //TODO - try to use the reopen function when possible
            //        (should be faster since uses only one OS call instead of two)
            bool OpenNewInternal(const OsStringView path, const OsStringView arguments) noexcept
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
                // a new one. If such behaviour is desired one should assure that it was infact the closing that
                // caused the error not the opening. Such checking can be done through the Close() function.
                // This implementation is faster and equally as convinient
                ThisType::CloseFile(this->FilePtr);

                //No need to check for return state - will be checked at the end through IsOpen
                FilePtr = CharSupport::fopen(path.data(), arguments.data());

                //if(this->IsOpen())
                //    FilePtr = CharSupport::freopen(path.data(), arguments.data(), FilePtr);
                //else
                //    FilePtr = CharSupport::fopen(path.data(), arguments.data());

                return IsOpen();
            }

            //template<typename ... OpenModeTypes,
            //         std::enable_if_t<MetaPrograming::AreTypesSameTo<typename OpenModeHolder::OpenModeFlag, OpenModeTypes...>::value, int> = 0>
            //inline bool OpenNew(const OsStringView path, OpenModeTypes ... openModes)
            //{
            //    return OpenNew(path, OpenModeHolder::OpenMode::GetOpenMode(openModes...));
            //}

        public:
            inline bool IsOpen() const noexcept {return (FilePtr != nullptr);}
            inline bool IsClosed() const noexcept {return (FilePtr == nullptr);}

        protected:
            inline static bool CloseFile(FILE PTR filePtr) noexcept
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
}
namespace std
{
    //Reimplementation of the swap inside std namespsace
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
