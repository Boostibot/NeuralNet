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

        protected:
            using CharSupport       = typename UnsafeFile::CharSupport;
            using StaticFunctions   = typename UnsafeFile::StaticFunctions;

        public:
            using Size              = typename UnsafeFile::Size;
            using FileSize          = typename UnsafeFile::FileSize;
            using Position          = typename UnsafeFile::Position;
            using OriginPosition    = typename UnsafeFile::OriginPosition;
            using BufferingCode     = typename UnsafeFile::BufferingCode;
            using FileDescriptor    = typename UnsafeFile::FileDescriptor;
            using Stats             = typename UnsafeFile::Stats;

            using OpenMode          = typename UnsafeFile::OpenMode;

            template<typename CharType>
            using String        = typename StaticFunctions:: template String<CharType>;
            template<typename CharType>
            using StringView    = typename StaticFunctions:: template StringView<CharType>;
            template<typename CharType>
            using CStringRef    = typename StaticFunctions:: template CStringRef<CharType>;

            using OsCharType    = typename StaticFunctions::OsCharType;
            using OsString      = typename StaticFunctions::OsString;
            using OsStringView  = typename StaticFunctions::OsStringView;
            using OsCStringRef  = typename StaticFunctions::OsCStringRef;

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
            inline BasicFile(const OsStringView path, const OpenMode REF openMode) : UnsafeFile(path, openMode)
            {}
            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline BasicFile(const OsStringView path, OpenModeTypes ... openModes) noexcept : UnsafeFile(path, openModes...)
            {}

        protected:
            inline UnsafeFile REF GetUnsafe() noexcept
            {
                return static_cast<UnsafeFile REF>(PTR_VAL(this));
            }
            inline const UnsafeFile REF GetUnsafe() const noexcept
            {
                return static_cast<const UnsafeFile REF>(PTR_VAL(this));
            }


        public:
            bool WasEndOfFileRieched() const noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().WasEndOfFileRieched();
            }


        public:
            Position GetPosInFile() noexcept
            {
                if(this->IsClosed())
                    return static_cast<Position>(0);

                return this->GetUnsafe().GetPosInFile();
            }

            bool SetPosInFile(Position pos, OriginPosition from = OriginPosition::Beggining) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().SetPosInFile(pos, from);
            }
            void MoveToBegging() noexcept
            {
                if(this->IsClosed())
                    return;

                this->GetUnsafe().MoveToBegging();
            }
            void MoveToEnd() noexcept
            {
                if(this->IsClosed())
                    return;

                this->GetUnsafe().MoveToEnd();
            }
            bool MoveBy(Position by) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().MoveBy(by);
            }

        public:
            template<typename PointerType>
            [[nodiscard]] bool Read(PointerType PTR ptrToBuffer, Size count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().Read(ptrToBuffer, count);
            }

            template<typename PointerType>
            [[nodiscard]] Size ReadAndCount(PointerType PTR ptrToBuffer, Size count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().ReadAndCount(ptrToBuffer, count);
            }

            template<typename CharT>
            [[nodiscard]] bool ReadString(CStringRef<CharT> to) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().ReadString(to);
            }

            template<typename CharT>
            [[nodiscard]] bool ReadString(CStringRef<CharT> to, const Size lenght) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().ReadString(to, lenght);
            }

            template<typename ObjectType>
            [[nodiscard]] bool ReadObject(ObjectType RVALUE_REF object) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().ReadObject(object);
            }

        public:
            template<typename PointerType>
            [[nodiscard]] bool Write(PointerType PTR ptrToData, Size count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().Write(ptrToData, count);
            }
            template<typename PointerType>
            [[nodiscard]] Size WriteAndCount(PointerType PTR ptrToData, Size count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().WriteAndCount(ptrToData, count);
            }
            template<typename ObjectType>
            [[nodiscard]] bool WriteObject(ObjectType RVALUE_REF object) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().WriteObject(object);
            }

            template <typename T,
                      std::enable_if_t<IsAnyString_v<T>, int> = 0>
            [[nodiscard]] bool WriteString(T RVALUE_REF str) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().WriteString(std::forward<T>(str));
            }


        public:
            bool SetBuffer(void PTR bufferPtr, Size bufferSize, BufferingCode mode) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().SetBuffer(bufferPtr, bufferSize, mode);
            }
            void Flush() noexcept
            {
                if(this->IsClosed())
                    return;

                return this->GetUnsafe().Flush();
            }
            void SwitchReadAndWrite() noexcept
            {
                if(this->IsClosed())
                    return;

                return this->GetUnsafe().SwitchBetweenReadAndWrite();
            }

        public:
            FileDescriptor GetFileDescriptor() const noexcept
            {
                if(this->IsClosed())
                    return FileDescriptor();

                return this->GetUnsafe().GetFileDescriptor();
            }

            bool GetFileStatics(Stats REF stats) const noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().GetFileStatics(stats);
            }

            FileSize GetFileSize() const noexcept
            {
                if(this->IsClosed())
                    return 0;

                return this->GetUnsafe().GetFileSize();
            }
    };

    using File          = BasicFile<char8>;
    using WFile         = BasicFile<charW>;
}
#endif // FILE_H
