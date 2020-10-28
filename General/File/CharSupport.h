#ifndef CHARSUPPORT_H
#define CHARSUPPORT_H

#include "CompilerSupport.h"

template<typename OsCharTpeArg>
struct CharTypeSupport
{
        using OsCharType    = OsCharTpeArg;

    public:
        static constexpr bool IsUtf8{std::is_same_v<OsCharType, char8>};
        static constexpr bool IsUtfW{std::is_same_v<OsCharType, charW>};
        static constexpr bool IsValid{IsUtf8 || IsUtfW};

        static_assert (IsValid, "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        static constexpr auto EndOfFile = IsUtf8 ? EOF : WEOF;

    public:
        static auto fopen(const OsCharType * fileName, const OsCharType * arguments) noexcept
        {
            if constexpr (IsUtf8)
                    return CompilerSpecific::fopen(fileName, arguments);
            else
                return CompilerSpecific::wfopen(fileName, arguments);
        }
        static inline auto fclose(FILE POINTER ptr) noexcept
        {
            return ::fclose(ptr);
        }
        static inline auto fgetc(FILE POINTER ptr) noexcept
        {
            if constexpr (IsUtf8)
                    return ::fgetc(ptr);
            else
                return ::fgetwc(ptr);
        }
        static inline auto fgets(OsCharType * str, int numChars, FILE POINTER ptr)  noexcept
        {
            if constexpr (IsUtf8)
                    return ::fgets(str, numChars, ptr);
            else
                return ::fgetws(str, numChars, ptr);
        }
        static inline auto fputc(OsCharType c, FILE POINTER ptr) noexcept
        {
            if constexpr (IsUtf8)
                    return ::fputc(static_cast<int>(c), ptr);
            else
                return ::fputwc(c, ptr);
        }
        static inline auto fputs(const OsCharType * str, FILE POINTER ptr)  noexcept
        {
            if constexpr (IsUtf8)
                    return ::fputs(str, ptr);
            else
                return ::fputws(str, ptr);
        }

    public:
        static inline auto tmpnam_s(OsCharType * dirName, size_t sizeInChars) noexcept
        {
            if constexpr (IsUtf8)
                    return ::tmpnam_s(dirName, sizeInChars);
            else
                return ::_wtmpnam_s(dirName, sizeInChars);
        }
        static inline auto mkdir(const OsCharType * dirName) noexcept
        {
            if constexpr (IsUtf8)
                    return ::_mkdir(dirName);
            else
                return ::_wmkdir(dirName);
        }
        static inline auto rename(const OsCharType * oldName, const OsCharType * newName) noexcept
        {
            if constexpr (IsUtf8)
                    return ::rename(oldName, newName);
            else
                return ::_wrename(oldName, newName);
        }
        static inline auto remove(const OsCharType * name) noexcept
        {
            if constexpr (IsUtf8)
                    return ::remove(name);
            else
                return ::_wremove(name);
        }

    public:
        static inline auto _stat64(const OsCharType * fileName, struct _stat64 * stats) noexcept
        {
            if constexpr (IsUtf8)
                    return ::_stat64(fileName, stats);
            else
                return ::_wstat64(fileName, stats);
        }
};

#endif // CHARSUPPORT_H
