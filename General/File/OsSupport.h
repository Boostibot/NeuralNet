#ifndef OSSUPPORT_H
#define OSSUPPORT_H

#include "General/Common/Common.h"

#define FILE_OS_INTERACTON_UTF_WIDE
//#define FILE_OS_INTERACTON_UTF_8
//#define FILE_OS_INTERACTON_UTF_16
//#define FILE_OS_INTERACTON_UTF_32

#define ASSERT_MULTIPLE_MODES static_assert (false, "More than one OS interaction mode selected; Select only one");

#ifdef FILE_OS_INTERACTON_UTF_16
static_assert (false, "Interaction with OS using utf-16 is not supported by the underlying c functions");
#endif

#ifdef FILE_OS_INTERACTON_UTF_32
static_assert (false, "Interaction with OS using utf-32 is not supported by the underlying c functions");
#endif

//Checking multiple selected
#ifdef FILE_OS_INTERACTON_UTF_8
using OsCharType = char8;
#ifdef FILE_OS_INTERACTON_DEFINED
ASSERT_MULTIPLE_MODES
#endif
#define FILE_OS_INTERACTON_DEFINED
#endif

#ifdef FILE_OS_INTERACTON_UTF_16
using OsCharType = char8; //So the code doesnt break everywhere
#ifdef FILE_OS_INTERACTON_DEFINED
ASSERT_MULTIPLE_MODES
#endif
#define FILE_OS_INTERACTON_DEFINED
#endif

#ifdef FILE_OS_INTERACTON_UTF_32
using OsCharType = char8; //So the code doesnt break everywhere
#ifdef FILE_OS_INTERACTON_DEFINED
ASSERT_MULTIPLE_MODES
#endif
#define FILE_OS_INTERACTON_DEFINED
#endif

#ifdef FILE_OS_INTERACTON_UTF_WIDE
using OsCharType = charW;
#ifdef FILE_OS_INTERACTON_DEFINED
ASSERT_MULTIPLE_MODES
#endif
#define FILE_OS_INTERACTON_DEFINED
#endif

//If none selected selects UTF_8
#ifndef FILE_OS_INTERACTON_DEFINED
#define FILE_OS_INTERACTON_UTF_8
using OsCharType = char8;
#endif

#endif // OSSUPPORT_H
