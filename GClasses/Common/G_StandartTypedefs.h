#ifndef CUSTOMTYPEDEFS_H
#define CUSTOMTYPEDEFS_H

#include "G_Defines.h"

//Basic data types
typedef int BOOL;
typedef unsigned char byte;
typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;

//EmptyType
class EmptyType{};

//TypePack
//Is used to "pack" multiple types into one to clarify template arguments and prevent unncesarry complexity in function definitions
template<typename ... ParentTypes> class TypePack : public ParentTypes...
{
    public:
        inline TypePack(const ParentTypes PASS_BY_REFERENCE ... parentTypes) : ParentTypes(parentTypes)... {}
};


#endif // CUSTOMTYPEDEFS_H
