#ifndef CHARSUPPORT_H
#define CHARSUPPORT_H

#include "OsSupport.h"
#include "CompilerSupport.h"

struct CharTypeSupport
{
        using OsCharType    = OsCharType;

    public:
        #ifdef FILE_OS_INTERACTON_UTF_WIDE
        static constexpr int EndOfFile = WEOF;
        #else
        static constexpr int EndOfFile = EOF;
        #endif


    public:
        static auto fopen(const OsCharType * fileName, const OsCharType * arguments) noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return CompilerSpecific::wfopen(fileName, arguments);

            #else
            return CompilerSpecific::fopen(fileName, arguments);

            #endif
        }
        static inline auto fclose(FILE POINTER ptr) noexcept
        {
            return ::fclose(ptr);
        }
        static inline auto fgetc(FILE POINTER ptr) noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::fgetwc(ptr);

            #else
            return ::fgetc(ptr);

            #endif
        }
        static inline auto fgets(OsCharType * str, int numChars, FILE POINTER ptr)  noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::fgetws(str, numChars, ptr);

            #else
            return ::fgets(str, numChars, ptr);

            #endif
        }
        static inline auto fputc(OsCharType c, FILE POINTER ptr) noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::fputwc(c, ptr);

            #else
            return ::fputc(static_cast<int>(c), ptr);

            #endif
        }
        static inline auto fputs(const OsCharType * str, FILE POINTER ptr)  noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::fputws(str, ptr);

            #else
            return ::fputs(str, ptr);

            #endif
        }

    public:
        static inline auto tmpnam_s(OsCharType * dirName, size_t sizeInChars) noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::_wtmpnam_s(dirName, sizeInChars);

            #else
            return ::tmpnam_s(dirName, sizeInChars);

            #endif
        }
        static inline auto mkdir(const OsCharType * dirName) noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::_wmkdir(dirName);

            #else
            return ::_mkdir(dirName);

            #endif
        }
        static inline auto rename(const OsCharType * oldName, const OsCharType * newName) noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::_wrename(oldName, newName);

            #else
            return ::rename(oldName, newName);

            #endif
        }
        static inline auto remove(const OsCharType * name) noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::_wremove(name);

            #else
            return ::remove(name);

            #endif
        }

    public:
        static inline auto _stat64(const OsCharType * fileName, struct _stat64 * stats) noexcept
        {
            #ifdef FILE_OS_INTERACTON_UTF_WIDE
            return ::_wstat64(fileName, stats);

            #else

            return ::_stat64(fileName, stats);
            #endif
        }
};

#endif // CHARSUPPORT_H
