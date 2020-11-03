#ifndef FILE_H
#define FILE_H

#include "UnsafeFile.h"
namespace CIo
{
    template<typename OsCharTypeArg>
    class BasicFile : public BasicUnsafeFile<OsCharTypeArg>
    {
        private:
            using ThisType          = BasicFile;

        public:
            using UnsafeFile        = BasicUnsafeFile<OsCharTypeArg>;
            using CFileManager      = typename UnsafeFile::CFileManager;
            using CharSupport       = typename CFileManager::CharSupport;

        public:
            using SizeType          = typename UnsafeFile::SizeType;
            using FileSizeType      = typename UnsafeFile::FileSizeType;
            using PosType           = typename UnsafeFile::PosType;
            using OriginPosition    = typename UnsafeFile::OriginPosition;
            using BufferingCode     = typename UnsafeFile::BufferingCode;
            using FileDescriptor    = typename UnsafeFile::FileDescriptor;
            using Stats             = typename UnsafeFile::Stats;

            using OpenMode          = typename UnsafeFile::OpenMode;

            template<typename CharType>
            using String        = typename CFileManager:: template String<CharType>;
            template<typename CharType>
            using StringView    = typename CFileManager:: template StringView<CharType>;
            template<typename CharType>
            using CString       = typename CFileManager:: template CString<CharType>;

            using OsCharType    = typename CFileManager::OsCharType;
            using OsString      = typename CFileManager::OsString;
            using OsStringView  = typename CFileManager::OsStringView;
            using OsCString     = typename CFileManager::OsCString;

            static_assert (CharSupport::IsValid, "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        private:
            using UnsafeFile::FilePtr;

        public:
            BasicFile() = default;
            BasicFile(const ThisType REF) = delete;
            BasicFile(ThisType RVALUE_REF) = default;
            ThisType REF operator=(const ThisType REF) = delete;
            ThisType REF operator=(ThisType RVALUE_REF) = default;

        public:
            inline BasicFile(const OsStringView path, const OsStringView openMode) : UnsafeFile(path, openMode)
            {}
            template<typename ... OpenModeTypes,
                     std::enable_if_t<MetaPrograming::AreTypesSameTo<typename UnsafeFile::CFileManager::OpenModeFlag, OpenModeTypes...>::value, int> = 0>
            inline BasicFile(const OsStringView path, OpenModeTypes ... openModes) noexcept : UnsafeFile(path, openModes...)
            {}


        protected:
            inline UnsafeFile REF GetUnsafeFile() noexcept
            {
                return static_cast<UnsafeFile REF>(PTR_VAL(this));
            }
            inline const UnsafeFile REF GetUnsafeFile() const noexcept
            {
                return static_cast<const UnsafeFile REF>(PTR_VAL(this));
            }


        public:
             bool WasLastErrorEndOfFile() const noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().WasLastErrorEndOfFile();
            }


        public:
            PosType GetPosInFile() noexcept
            {
                if(this->IsClosed())
                    return static_cast<PosType>(0);

                return this->GetUnsafeFile().GetPosInFile();
            }

            bool SetPosInFile(PosType pos, OriginPosition from = OriginPosition::Beggining) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().SetPosInFile(pos, from);
            }
            void MoveToBegging() noexcept
            {
                if(this->IsClosed())
                    return;

                this->GetUnsafeFile().MoveToBegging();
            }
            void MoveToEnd() noexcept
            {
                if(this->IsClosed())
                    return;

                this->GetUnsafeFile().MoveToEnd();
            }
            bool MoveBy(PosType by) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().MoveBy(by);
            }

        public:
            template<typename PointerType>
            [[nodiscard]] bool Read(PointerType POINTER ptrToBuffer, SizeType count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().Read(ptrToBuffer, count);
            }

            template<typename PointerType>
            [[nodiscard]] SizeType ReadAndCount(PointerType POINTER ptrToBuffer, SizeType count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().ReadAndCount(ptrToBuffer, count);
            }

            template<typename CharT>
            [[nodiscard]] bool ReadString(OsString REF to) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().ReadString(to);
            }

            template<typename CharT>
            [[nodiscard]] bool ReadString(OsString REF to, const SizeType lenght) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().ReadString(to, lenght);
            }

            template<typename ObjectType>
            [[nodiscard]] bool ReadObject(ObjectType RVALUE_REF object) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().ReadObject(object);
            }

        public:
            template<typename PointerType>
            [[nodiscard]] bool Write(PointerType POINTER ptrToData, SizeType count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().Write(ptrToData, count);
            }
            template<typename PointerType>
            [[nodiscard]] SizeType WriteAndCount(PointerType POINTER ptrToData, SizeType count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().WriteAndCount(ptrToData, count);
            }
            template<typename ObjectType>
            [[nodiscard]] bool WriteObject(ObjectType RVALUE_REF object) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().WriteObject(object);
            }

            template <typename T,
                      std::enable_if_t<IsAnyString_v<T>, int> = 0>
            [[nodiscard]] bool WriteString(T RVALUE_REF str) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().WriteString(std::forward<T>(str));
            }


        public:
            bool SetBuffer(void POINTER bufferPtr, SizeType bufferSize, BufferingCode mode) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafeFile().SetBuffer(bufferPtr, bufferSize, mode);
            }
            void Flush() noexcept
            {
                if(this->IsClosed())
                    return;

                return this->GetUnsafeFile().Flush();
            }
            void SwitchBetweenReadAndWrite() noexcept
            {
                if(this->IsClosed())
                    return;

                return this->GetUnsafeFile().SwitchBetweenReadAndWrite();
            }

        public:
            FileDescriptor GetFileDescriptor() const noexcept
            {
                if(this->IsClosed())
                    return UnsafeFile::GetErrorFileDescriptor();

                return this->GetUnsafeFile().GetFileDescriptor();
            }

            bool GetFileStatics(Stats REF stats) const noexcept
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

    using File          = BasicFile<char8>;
    using WFile         = BasicFile<charW>;
}
#endif // FILE_H
