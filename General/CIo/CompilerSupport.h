#ifndef COMPILERSUPPORT_H
#define COMPILERSUPPORT_H

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#define __STDC_WANT_LIB_EXT1__ 1

#include <io.h>
#include <locale.h>
#include <cstdio>
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <locale>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "General/Common/Common.h"
namespace CIo
{
    namespace CompilerSupport
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

        #if defined (_MSC_VER)
        using OffsetType = __int64;
        #else
        using OffsetType = off64_t;
        #endif

        #if defined (_MSC_VER)
        using StatType = struct ::_stat64;
        #else
        using StatType = struct _stat64;
        #endif

        #define MSVC_MAYBE_UNUSED \
            #if defined (_MSC_VER) \
            [[maybe_unused]] \
            #endif


        inline OffsetType ftell(FILE POINTER stream) noexcept
        {
            #if defined (_MSC_VER)
            return _ftelli64(stream);
            #else
            return ftello64(stream);
            #endif
        }

        inline auto fseek(FILE POINTER stream, OffsetType offset, i32 origin) noexcept
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
