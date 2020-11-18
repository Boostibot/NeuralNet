#ifndef STATICFILEFUNCTIONS_H
#define STATICFILEFUNCTIONS_H

#include "CharSupport.h"

namespace CIo
{
    template<typename OsCharTypeArg>
    struct StaticFileFunctions
    {
        protected:
            using ThisType          = StaticFileFunctions;
            using OsCharType        = OsCharTypeArg;

        public:
            using CharSupport       = CharTypeSupport<OsCharType>;
            using FileSize          = decltype (::_stat64::st_size);

            template<typename CharType>
            using String        = std::basic_string<CharType>;
            template<typename CharType>
            using StringView    = std::basic_string_view<CharType>;
            template<typename CharType>
            using CString       = CharType *;

            using OsString      = String<OsCharType>;
            using OsStringView  = StringView<OsCharType>;
            using OsCString     = CString<OsCharType>;

        public:
            static constexpr int EndOfFile              = CharSupport::EndOfFile;
            static constexpr int FileNamesLenghtMax     = FILENAME_MAX;
            static constexpr int OpenFilesMax           = FOPEN_MAX;
            static constexpr int TempFileNamesLenghtMax = L_tmpnam;
            static constexpr int TemporaryFilesMax      = TMP_MAX;

            struct Stats
            {
                    using CStatType = struct _stat64;
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

                protected:
                    #if defined (_MSC_VER)
                    [[maybe_unused]]
                    #endif
                    InnerDescriptor Descriptor = -1; //DEFINE AS ERROR DESCRIPTOR
                    FileDescriptor(InnerDescriptor descriptor) noexcept : Descriptor(descriptor) {}

                public:
                    FileDescriptor() = default;
                    explicit operator InnerDescriptor() const noexcept {return Descriptor;}
                    inline bool IsValid() const noexcept {return Descriptor >= 0;}
            };

            static bool GetUniqueTempFileName(OsString REF fileName) noexcept //tmpnam_s
            {
                if(fileName.size() < ThisType::TempFileNamesLenghtMax)
                    return false;

                return (CharSupport::tmpnam_s(fileName.data(), fileName.size()) == 0);
            }

            static bool CreateDirectory(const OsStringView dirName) noexcept
            {
                return (CharSupport::mkdir(dirName.data()) == 0);
            }
            inline static bool CreateFile(const OsStringView filename) noexcept
            {
                return CharSupport::createFile(filename);
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

            inline static bool IsFileAccessible(const OsStringView filename)
            {
                Stats stats;
                return GetFileStatics(stats, filename);
            }

            static FileSize GetFileSize(const OsStringView filename) noexcept
            {
                Stats stats;
                if(NOT ThisType::GetFileStatics(stats, filename))
                    return 0;

                return stats.Size();
            }

    };

}


#endif // STATICFILEFUNCTIONS_H
