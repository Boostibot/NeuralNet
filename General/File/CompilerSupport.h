#ifndef COMPILERSUPPORT_H
#define COMPILERSUPPORT_H

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#define __STDC_WANT_LIB_EXT1__ 1

#include <locale.h>
#include <cstdio>
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
//#include <unistd.h>
#include <locale>

#include "General/Common/Common.h"
namespace CIo
{
    namespace CompilerSpecific
    {
        inline auto fopen(const char8 * fileName, const char8 * arguments) noexcept
        {
            #if defined (_MSC_VER)
            FILE POINTER ptr;
            fopen_s(ADDRESS(ptr), fileName, arguments);
            return ptr;
            #else
            return fopen64(fileName, arguments);
            #endif
        }

        inline auto wfopen(const charW * fileName, const charW * arguments) noexcept
        {
            FILE POINTER ptr;
            _wfopen_s(ADDRESS(ptr), fileName, arguments);
            return ptr;
        }

        inline auto freopen(const char8 * fileName, const char * arguments, FILE POINTER stream)
        {
            return ::freopen(fileName, arguments, stream);
        }
        inline auto wfreopen(const charW * fileName, const charW * arguments, FILE POINTER stream)
        {
            return ::_wfreopen(fileName, arguments, stream);
        }

        #if defined (_MSC_VER)
        using OffsetType = __int64;
        #else
        using OffsetType = off64_t;
        #endif

        inline OffsetType ftell(FILE POINTER stream) noexcept
        {
            #if defined (_MSC_VER)
            return _ftelli64(stream);
            #else
            return ftello64(stream);
            #endif
        }

        inline int fseek(FILE POINTER stream, OffsetType offset, int origin) noexcept
        {
            #if defined (_MSC_VER)
            return _fseeki64(stream, offset, origin);
            #else
            return fseeko64(stream, offset, origin);
            #endif
        }
    }
}
#endif // COMPILERSUPPORT_H
