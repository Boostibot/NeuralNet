#ifndef UNSAFEFILE_H
#define UNSAFEFILE_H

#include "CFileManager.h"

//TODO
// - Sort out GetFileSize() and GetFileStatistics() return types - make A or B
// - Test all functions
namespace CIo
{
    //A full wrapper class which should implemnt a wrapper function for all necessary C FILE functions
    //It should also ease the use of C FILE and be the base for more specific classes
    template<typename OsCharTypeArg>
    class BasicUnsafeFile : public BasicCFileManager<OsCharTypeArg>
    {
        protected:
            using ThisType          = BasicUnsafeFile;
            using CFileManager      = BasicCFileManager<OsCharTypeArg>;
            using CharSupport       = typename CFileManager::CharSupport;

        public:
            using Size              = size_t;
            using Position          = CompilerSpecific::OffsetType;
            using FileSize          = decltype (::_stat64::st_size);

            using OpenMode          = typename CFileManager::OpenMode;

            template<typename CharType>
            using String        = typename CFileManager:: template String<CharType>;
            template<typename CharType>
            using StringView    = typename CFileManager:: template StringView<CharType>;
            template<typename CharType>
            using CStringRef    = typename CFileManager:: template CStringRef<CharType>;

            using OsCharType    = typename CFileManager::OsCharType;
            using OsString      = typename CFileManager::OsString;
            using OsStringView  = typename CFileManager::OsStringView;
            using OsCStringRef  = typename CFileManager::OsCStringRef;

            static_assert (CharSupport::IsValid, "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        public:
            static constexpr i32 EndOfFile              = CharSupport::EndOfFile;
            static constexpr i32 FileNameMaxLenght      = FILENAME_MAX;
            static constexpr i32 OpenFilesMax           = FOPEN_MAX;
            static constexpr i32 TempFileNameMaxLenght  = L_tmpnam;
            static constexpr i32 TempFilesMax           = TMP_MAX;

            struct Stats
            {
                    //SHOULD BE IN COMPILER SPECIFIC!!!
                    #if defined (_MSC_VER)
                    using CStatType = struct ::_stat64;
                    #else
                    using CStatType = struct _stat64;
                    #endif
                    CStatType CStats;

                    inline auto GroupIdOwningFile()         const noexcept {return CStats.st_gid;}
                    inline auto UserIdOwningFile()          const noexcept {return CStats.st_uid;}
                    inline auto TimeOfCreation()            const noexcept {return CStats.st_ctime;}
                    inline auto TimeOfLastAccess()          const noexcept {return CStats.st_atime;}
                    inline auto TimeOfLastModification()    const noexcept {return CStats.st_mtime;}
                    inline auto DriveNumberOfDiskWithFile() const noexcept {return CStats.st_dev;}
                    inline auto NumberOfInformationNodes()  const noexcept {return CStats.st_ino;}
                    inline auto FileModeBitMask()           const noexcept {return CStats.st_mode;}
                    inline auto NumberOfHardLinks()         const noexcept {return CStats.st_nlink;}
                    inline auto Size()                      const noexcept {return CStats.st_size;}
            };

            struct FileDescriptor
            {
                    using InnerDescriptor = int;
                    static constexpr InnerDescriptor ErrorDescriptor = {-1};

                protected:
                    #if defined (_MSC_VER)
                    [[maybe_unused]]
                    #endif
                    const InnerDescriptor Descriptor = ErrorDescriptor;

                public:
                    constexpr FileDescriptor() noexcept = default;
                    constexpr explicit operator InnerDescriptor() const noexcept {return Descriptor;}
                    constexpr inline bool IsValid() const noexcept               {return Descriptor >= 0;}
            };

            enum class OriginPosition : i32
            {
                Beggining = SEEK_SET,
                CurrentPosition = SEEK_CUR,
                End = SEEK_END
            };

            enum class BufferingCode : i32
            {
                Full = _IOFBF,
                Line = _IONBF,
                None = _IONBF
            };

        private:
            using CFileManager::FilePtr;

        public:
            BasicUnsafeFile() = default;
            BasicUnsafeFile(const ThisType REF) = delete;
            BasicUnsafeFile(ThisType RVALUE_REF) = default;
            ThisType REF operator=(const ThisType REF) = delete;
            ThisType REF operator=(ThisType RVALUE_REF) = default;

        public:
            inline BasicUnsafeFile(const OsStringView path, const OpenMode REF openMode) noexcept : CFileManager(path, openMode)
            {}

            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline BasicUnsafeFile(const OsStringView path, OpenModeTypes ... openModes) noexcept : CFileManager(path, openModes...)
            {}

        public:
            ~BasicUnsafeFile() = default;

        //public:
            //inline CFileManager REF GetCFileManager() noexcept
            //{
            //    return static_cast<CFileManager REF>(PTR_VAL(this));
            //}
            //inline const CFileManager REF GetCFileManager() const noexcept
            //{
            //    return static_cast<const CFileManager REF>(PTR_VAL(this));
            //}

        public:
            inline bool WasEndOfFileRieched() const noexcept
            {
                return (feof(FilePtr) != 0);
            }
            //inline bool HasError() const noexcept {return (ferror(FilePtr) != 0);}
            //inline void ClearError() noexcept {clearerr(FilePtr);}
            //inline friend void PrintErrorToStdErr(StringView msgBeforeError = "") noexcept {perror(msgBeforeError.data());}


            //These functions are safer and are capable of storing position
            // inside files larger than 2GB but lack some of the usability.
            // For example they dont enable moving position forward just
            // restoring some previous location.
            //
            // -> Use the ftell and fseek that support larger files
            //     This is compiler specific (_ftelli64 on MSVC ftello64 on other)
            //
            //Should these functions be present even though there is an alternative?
            // ie. having a differently named function with very specific name can
            // be more verbose and thus more telling of the probkem it is trying to
            // solve.
            // For example the names SaveCurrentPosInFile(arg) RestorePosInFile(arg)
            //
            // -> No the fonfusion assosieted with two of the same function
            //     is not worth it. Esepcially when the function arguments differ
            //     and are not universally convertible

            //struct PosInFileition
            //{
            //        friend BasicUnsafeFile;
            //    private:
            //        [[maybe_unused]] fpos_t InnerPos;
            //};
            //inline bool SaveCurrentPosInFile(PosInFileition REF position) noexcept //fgetpos
            //{
            //    return (fgetpos(this->FilePtr, ADDRESS(position.InnerPos)) == 0);
            //}
            //inline bool RestorePosInFile(PosInFileition position) noexcept //fsetpos
            //{
            //    return (fsetpos(this->FilePtr, ADDRESS(position.InnerPos)) == 0);
            //}


            //Deciding over checking in position moving functions
            //
            //At first the default checks performed by fseek seem undervelming but
            // in reality they are sufficient. All we care about is if calling to
            // write or read would break the file / fail. This does not mean the
            // writing or reading must succeed. Success on those functions is
            // returned by their return value not this function. Further more
            // the Write / Reed fuctions return should always be checked to
            // ensure that the data was actually written.
            //
            // -> The checks are unnecessary an expensive
            //bool IsCurrentPosInFileValidForReading()
            //{
            //    PosInFileition pos;
            //    if(SaveCurrentPosInFile(pos) == false)
            //        return false;
            //
            //    bool returnVal = (fgetc(this->FilePtr) != ThisType::EndOfFile);
            //
            //    if(RestorePosInFile(pos) == false)
            //        return false;
            //
            //    return returnVal;
            //}

        protected:
            template<typename T1, typename T2, typename ReturnType = T1>
            inline constexpr static auto Min(T1 RVALUE_REF a1, T2 RVALUE_REF a2)
            {
                if(a1 < a2)
                    return a1;
                else
                    return a2;
            }

        public:
            inline Position GetPosInFile() const noexcept //ftell
            {
                return CompilerSpecific::ftell(this->FilePtr);
            }
            inline bool SetPosInFile(const Position pos, const OriginPosition from = OriginPosition::Beggining) noexcept //fseek
            {
                return (CompilerSpecific::fseek(this->FilePtr, pos, static_cast<int>(from)) == 0);
            }
            inline void MoveToBegging() noexcept //frewind
            {
                rewind(this->FilePtr);
            }
            inline void MoveToEnd() noexcept //fseek end
            {
                //No return statement - moving to the end should never fail
                // The fseek function only checks if the final position would be
                // negative
                SetPosInFile(0, OriginPosition::End);
            }
            inline bool MoveBy(Position by) noexcept //fseek curr
            {
                return SetPosInFile(by, OriginPosition::CurrentPosition);
            }

        public:
            template<typename PointerType>
            [[nodiscard]] inline bool Read(PointerType PTR ptrToBuffer, const Size count) noexcept
            {
                return (ReadAndCount(ptrToBuffer, count) == count);
            }

            template<typename PointerType>
            [[nodiscard]] inline Size ReadAndCount(PointerType PTR ptrToBuffer, const Size count) noexcept //fread
            {
                return fread(ptrToBuffer, sizeof (PointerType), count, FilePtr);
            }

            template<typename CharT>
            [[nodiscard]] inline bool ReadString(const CStringRef<CharT> REF output) noexcept
            {
                //Even though CStringRef output is marked const it doesnt mean the
                // managed reference is const.
                // This is done because passed reference is of smaller size
                return Read(output.Data, output.Size);
            }
            template<typename CharT>
            [[nodiscard]] inline bool ReadString(const CStringRef<CharT> REF output, const Size maxlenght) noexcept
            {
                //Even though CStringRef output is marked const it doesnt mean the
                // managed reference is const.
                // This is done because passed reference is of smaller size
                return Read(output.Data, ThisType::Min(output.Size, maxlenght));
            }

            template<typename ObjectType>
            [[nodiscard]] inline bool ReadObject(ObjectType REF object) noexcept
            {
                //static_assert (std::is_trivial_v<ObjectType>, "UnsafeFile::ReadObject : Object must be trivially constructible");
                return Read(ADDRESS(object), 1);
            }

        public:
            template<typename PointerType>
            [[nodiscard]] inline bool Write(const PointerType PTR const ptrToData, const Size count) noexcept
            {
                return (WriteAndCount(ptrToData, count) == count);
            }

            template<typename PointerType>
            [[nodiscard]] inline Size WriteAndCount(const PointerType PTR const ptrToData, const Size count) noexcept //fwrite
            {
                return fwrite(ptrToData, sizeof (PointerType), count, FilePtr);
            }

            template<typename ObjectType>
            [[nodiscard]] inline bool WriteObject(ObjectType RVALUE_REF object) noexcept
            {
                //static_assert (std::is_trivial_v<ObjectType>, "UnsafeFile::WriteObject : Object must be trivially constructible");
                return Write(ADDRESS(object), 1);
            }

        private:
            template <typename CharT>
            [[nodiscard]] inline bool WriteStringImpl(const std::basic_string_view<CharT> str) noexcept
            {
                return Write(str.data(), str.size());
                //return CharSupport::fputs(str.data(), this->FilePtr); //-> does not support all string types
            }

        public:
            template <typename T,
                      std::enable_if_t<IsAnyString_v<T>, i32> = 0>
            [[nodiscard]] inline bool WriteString(T RVALUE_REF str)
            {
                using CharT = GetAnyStringType_t<T>;

                return WriteStringImpl<CharT>(str);
            }
            //Formated IO is not supported since the formating is slow
            // and there are faster and more robust ways of formating
            //
            // -> Use {fmt} lib for formating
            //
            //template<typename ... ArgumentTypes>
            //bool WriteFormated(StringView cFormatString, ArgumentTypes ... args);
            //bool WriteFormatedToBuffer();
            //bool ReadFormatedFromBuffer();
            //bool ReadFormated();


            //This function is very specific and there are generally
            // better ways to achieve the same result
            // this function also breaks other functions (ftell fseek)
            // so they would require further runtime checks
            //
            // -> Use ftell and fseek to restore the file position
            //     and character buffers to put charcters back into
            //     streams
            //
            //bool UnReadCharcter();

        public:
            inline bool SetBuffer(void PTR bufferPtr, const Size bufferSize, const BufferingCode mode) noexcept //Setvbuf
            {
                return setvbuf(this->FilePtr, static_cast<char PTR>(bufferPtr), static_cast<int>(mode), bufferSize);
            }
            inline void Flush() noexcept //fflush
            {
                fflush(this->FilePtr);
            }
            inline void SwitchBetweenReadAndWrite() noexcept
            {
                this->MoveBy(0);
            }

        public:
            static bool GetUniqueTempFileName(const OsCStringRef REF fileName) noexcept //tmpnam_s
            {
                //Even though CStringRef output is marked const it doesnt mean the
                // managed reference is const.
                // This is done because passed reference is of smaller size
                if((fileName.Size) < ThisType::TempFileNameMaxLenght)
                    //The parentheses around fileName.Size are necessary
                    //Without them MinGW reports strange error - compiler bug?
                    return false;

                return (CharSupport::tmpnam_s(fileName.Data, fileName.Size) == 0);
            }

            static bool CreateDirectory(const OsStringView dirName) noexcept
            {
                return (CharSupport::mkdir(dirName.data()) == 0);
            }

            static bool RemoveEmptyDirectory(const OsStringView dirName) noexcept
            {
                return (CharSupport::rmdir(dirName.data()) == 0);
            }

            inline static bool CreateFile(const OsStringView filename) noexcept
            {
                constexpr OpenMode appendMode = COpenMode::WriteAppend;
                ThisType file;
                return file.OpenNew(filename, appendMode);
            }

            inline static bool RenameFile(const OsStringView oldFileName, const OsStringView newFileName) noexcept
            {
                return (CharSupport::rename(oldFileName.data(), newFileName.data()) == 0);
            }

            inline static bool RemoveFile(const OsStringView fileName) noexcept
            {
                return (CharSupport::remove(fileName.data()) == 0);
            }

            inline static bool GetFileStatics(Stats REF stats, const FileDescriptor descriptor) noexcept
            {
                return (_fstat64(descriptor, ADDRESS(stats.CStats)) == 0);
            }

            inline static bool GetFileStatics(Stats REF stats, const OsStringView filename) noexcept
            {
                return (CharSupport::_stat64(filename.data(), ADDRESS(stats.CStats)) == 0);
            }

            static FileSize GetFileSize(const OsStringView filename) noexcept
            {
                Stats stats;
                if(NOT ThisType::GetFileStatics(stats, filename))
                    return -1;

                return stats.Size();
            }

        public:
            inline FileDescriptor GetFileDescriptor() const noexcept
            {
                return _fileno(this->FilePtr);
            }

            inline bool GetFileStatics(Stats REF stats) const noexcept
            {
                return ThisType::GetFileStatics(stats, this->GetFileDescriptor());
            }

            FileSize GetFileSize() const noexcept
            {
                Stats stats;

                if(NOT this->GetFileStatics(stats))
                    return 0;

                return stats.Size();
            }
    };

    using WUnsafeFile   = BasicUnsafeFile<charW>;
    using UnsafeFile    = BasicUnsafeFile<char8>;

    namespace
    {
        //Deciding over error checking functions (ferror feof)
        //
        //Consider the following example:
        // Use A:
        //  File file("temp.txt", "wb");
        //  while(file.HasError() == false)
        //  {
        //      file.WriteString("Str");
        //      //...Clears error using fseek or ClearError
        //      file.ClearError();
        //  }
        //
        // Use B:
        //  while(file.WriteString("Str"))
        //  {}
        //  if(file.WasLastErrorEndOfFile())
        //  {}
        //  else
        //  {}
        //
        //The incorrect use of error checking functions (HasError
        // IsAtEndOfFile) can result in bugs in code
        // and as such this class should not aspire the user to
        // do so. Therefore it is important to make the use case
        // clear. We can achive this by renaming the function to emphesise
        // when it is valid (only after the error occured).
        //
        //Notice how in B there is no longer HasError() function since it
        // now does not make sense (what would it be called?
        // WasLastErrorError? WasLastErrorPresent? - ofcourse it was
        // else the loop wouldnt have terminated) and it serves no purpose
        // ie. there are no other error checking functions that would not
        // be a subset of HasError therefore making this function equivalent
        // to and else statement

        //Deciding over error setting functions (clearerr)
        //
        //This function should not be avalible to public as it aspires
        // to use checking for errors as a way to check state of io.
        // This can result in issues as shown above with (accidentally)
        // clearing the error flag that is checked for
        // If the error checking function is called WasLastError...
        // it makes it clear that the WasLast... is only valid after some
        // error occured and therefore eliminating the need to clear it
        // When is the correct setting to use this function anyway?

        //Deciding over the entire existence of error checking functions
        //
        //The WasLastErrorEndOfFile does not even have to exist since even now
        // it CAN (not is it likely) be missused. We can assure the function
        // can not be missused by removing it and replacing it by error code.
        // consider the following examples:
        //
        // Example A:
        //  File file("temp.txt", "wb");
        //  File::ErrorFlag lastError;
        //
        //  while(lastError = file.WriteString("Str"))
        //  {}
        //  switch(ErrorFlag)
        //  {}
        //
        // Example B:
        //  File file("temp.txt", "wb");
        //  File::ErrorFlag lastError = File::ErrorFlag::NoError;
        //
        //  while(lastError == File::ErrorFlag::NoError)
        //  {
        //      lastError = file.WriteString("Str");
        //  }
        //  switch(ErrorFlag)
        //  {}
        //
        //In both A and B there is no way of invalid checking since
        // there is only one thing to check and that is the same
        // as checking for any other error. The error flag can be
        // enum inheriting from u8 therefore not decreasing
        // performance by any bit. Its values shall be 0 for NoError
        // and values other from 0 for everything else - this is to
        // ensure it is freely convertible to bool
        // The only pitfall could be repeated calling of feof inside the
        // write/read function - no this is not the case.
        // Consider the following code:
        //
        //  File::ErrorCode Read(*ptr, count)
        //  {
        //      if(this->ReadUnsafe(ptrToBuffer, count))
        //          return true;
        //      else
        //      {
        //          if(WasLastErrorEndOfFile())
        //              return ErrorCode::EndOfFile;
        //          else
        //              return ErrorCode::OtherError;
        //      }
        //  }
        //
        // Here the call is only done if teh call fails.
        // The issue can still be the one extra if statement to
        // determine if the call is needed. This results in two
        // checks being performed for each iteration in the while
        // loop. This is bad especially for smaller files though
        // not too severe.
        //
        // The issue with this approach is that functions such as
        // WriteWithCount require the return value to specify the
        // count of elements read - this would either require keeping
        // the WasLastErrorEndOfFile function just for these (this
        // circumvents the entire issue of trying to get rid of it)
        // or to chage such functions to take another out parameter
        // to track the count written. This would introduce another
        // slowdown.
        //


    }
}

#endif // UNSAFEFILE_H
