#ifndef G_MEMORYFUNCTIONS_H
#define G_MEMORYFUNCTIONS_H

#include "G_Defines.h"
#include <type_traits>
#include <utility>
#include <cstring>

///Setting
//Sets [size] bytes on the [ptr] address to specified [value]
template<typename PtrType,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void SetMemory (PtrType CLASSIFIED_AS_POINTER ptr, size_t size, int value)
{
    memset(reinterpret_cast<void*>(ptr), value, size);
}

//Sets all [ptr] type bytes on the [ptr] address to specified [value]
template<typename PtrType,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void SetPtrType (PtrType CLASSIFIED_AS_POINTER ptr, int value)
{
    SetMemory(ptr, sizeof(POINTER_VALUE(ptr)), value);
}

//Sets all [variable] type bytes on the [variable]'s address to specified [value]
template<typename Type>
inline void SetType (Type PASS_BY_REFERENCE variable, int value)
{
    SetPtrType(ADDRESS(variable), value);
}

//Sets [size] bytes on the [ptr] address to NULL (0)
template<typename PtrType,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void NullMemory (PtrType CLASSIFIED_AS_POINTER ptr, size_t size)
{
    SetMemory(ptr, size, NULL);
}

//Sets all [ptr] type bytes on the [ptr] address to NULL (0)
template<typename PtrType,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void NullPtrType (PtrType CLASSIFIED_AS_POINTER ptr)
{
    NullMemory(ptr, sizeof(POINTER_VALUE(ptr)));
}

//Sets all [variable] type bytes on the [variable]'s address to NULL (0)
template<typename Type>
inline void NullType (Type PASS_BY_REFERENCE variable)
{
    NullPtrType(ADDRESS(variable));
}


///Moving
//Moves [numOfBytes] number of bytes from [source] address to [destination] address
template<typename SourceType, typename DestinationType,
         std::enable_if_t<std::is_pointer<SourceType>::value && std::is_pointer<DestinationType>::value, int> = 0>
inline void MoveMemory (SourceType CLASSIFIED_AS_POINTER source, DestinationType CLASSIFIED_AS_POINTER destination, size_t numOfBytes)
{
    memmove(destination, source, numOfBytes);
}

//Moves [numOfBytes] number of bytes from [source] address to [destination] address
template<typename SourceType, typename DestinationType,
         std::enable_if_t<std::is_pointer<SourceType>::value && std::is_pointer<DestinationType>::value, int> = 0>
inline void MoveMemory (SourceType CLASSIFIED_AS_POINTER source, size_t sourceSize, DestinationType CLASSIFIED_AS_POINTER destination, size_t destinationSize)
{
    if(sourceSize < destinationSize)
        MoveMemory(source, destination, sourceSize);
    else
        MoveMemory(source, destination, destinationSize);
}

//Moves all [source] type bytes from [source] address to [destination] address
template<typename SourceType,
         std::enable_if_t<std::is_pointer<SourceType>::value, int> = 0>
inline void MovePtrType (SourceType CLASSIFIED_AS_POINTER source, SourceType CLASSIFIED_AS_POINTER destination)
{
    MoveMemory(source, destination, sizeof(POINTER_VALUE(source)));
}

//Moves all [source] type bytes from [source]'s address to [destination]'s address
template<typename SourceType>
inline void MoveType (SourceType PASS_BY_REFERENCE source, SourceType PASS_BY_REFERENCE destination)
{
    MovePtrType(ADDRESS(source), ADDRESS(destination));
}

//Moves all [source] or [destination] type bytes (whichever is smaller) from [source] address to [destination] address;
template<typename SourceType, typename DestinationType,
         std::enable_if_t<std::is_pointer<SourceType>::value && std::is_pointer<DestinationType>::value, int> = 0>
inline void MovePtrType (SourceType CLASSIFIED_AS_POINTER source, DestinationType CLASSIFIED_AS_POINTER destination)
{
    if(sizeof(ADDRESS(source)) < sizeof(ADDRESS(destination)))
        MoveMemory(source, destination, sizeof(POINTER_VALUE(source)));
    else
        MoveMemory(source, destination, sizeof(POINTER_VALUE(destination)));
}

//Moves all [source] or [destination] type bytes (whichever is smaller) from [source] address to [destination] address;
template<typename SourceType, typename DestinationType>
inline void MoveType (SourceType PASS_BY_REFERENCE source, DestinationType PASS_BY_REFERENCE destination)
{
    MovePtrType(ADDRESS(source), ADDRESS(destination));
}


///Copying
//Copies [numOfBytes] number of bytes from [source] address to [destination] address
template<typename SourceType, typename DestinationType,
         std::enable_if_t<std::is_pointer<SourceType>::value && std::is_pointer<DestinationType>::value, int> = 0>
inline void CopyMemory (SourceType CLASSIFIED_AS_POINTER source, DestinationType CLASSIFIED_AS_POINTER destination, size_t numOfBytes)
{
    memcpy(destination, source, numOfBytes);
}

//Copies [numOfBytes] number of bytes from [source] address to [destination] address
template<typename SourceType, typename DestinationType,
         std::enable_if_t<std::is_pointer<SourceType>::value && std::is_pointer<DestinationType>::value, int> = 0>
inline void CopyMemory (SourceType CLASSIFIED_AS_POINTER source, size_t sourceSize, DestinationType CLASSIFIED_AS_POINTER destination, size_t destinationSize)
{
    if(sourceSize < destinationSize)
        CopyMemory(source, destination, sourceSize);
    else
        CopyMemory(source, destination, destinationSize);
}

//Copies all [source] type bytes from [source] address to [destination] address
template<typename SourceType,
         std::enable_if_t<std::is_pointer<SourceType>::value, int> = 0>
inline void CopyPtrType (SourceType CLASSIFIED_AS_POINTER source, SourceType CLASSIFIED_AS_POINTER destination)
{
    CopyMemory(source, destination, sizeof(POINTER_VALUE(source)));
}

//Moves all [source] type bytes from [source]'s address to [destination]'s address
template<typename SourceType>
inline void CopyType (SourceType PASS_BY_REFERENCE source, SourceType PASS_BY_REFERENCE destination)
{
    CopyPtrType(ADDRESS(source), ADDRESS(destination));
}

//Copies all [source] or [destination] type bytes (whichever is smaller) from [source] address to [destination] address;
template<typename SourceType, typename DestinationType,
         std::enable_if_t<std::is_pointer<SourceType>::value && std::is_pointer<DestinationType>::value, int> = 0>
inline void CopyPtrType (SourceType CLASSIFIED_AS_POINTER source, DestinationType CLASSIFIED_AS_POINTER destination)
{
    if(sizeof(ADDRESS(source)) < sizeof(ADDRESS(destination)))
        CopyMemory(source, destination, sizeof(POINTER_VALUE(source)));
    else
        CopyMemory(source, destination, sizeof(POINTER_VALUE(destination)));
}

//Moves all [source] or [destination] type bytes (whichever is smaller) from [source] address to [destination] address;
template<typename SourceType, typename DestinationType>
inline void CopyType (SourceType PASS_BY_REFERENCE source, DestinationType PASS_BY_REFERENCE destination)
{
    CopyPtrType(ADDRESS(source), ADDRESS(destination));
}


///Constructing
//Calls {ConstructorType} constructor on [ptr] (address)
template<typename ConstructionType, typename PtrType CLASSIFIED_AS_POINTER,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void ConstructType (PtrType CLASSIFIED_AS_POINTER ptr)
{
    new (ptr) ConstructionType();
}

//Calls {ConstructorType} constructor on [ptr] (address) then moves the [ptr] one {ConstructorType} size
//Repeats untill there is no space for anpopther {ConstructorType}
template<typename ConstructionType, typename PtrType CLASSIFIED_AS_POINTER,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void ConstructTypeRepeat (PtrType CLASSIFIED_AS_POINTER ptr, size_t blockSize)
{
    size_t times = blockSize / sizeof(ConstructionType);
    for(size_t i = 0; i < times; i++)
    {
        ConstructType<ConstructionType>(ptr);
        ptr += sizeof(ConstructionType);
    }
}

//Calls {ConstructorType} constructor on [ptr] (address)
template<typename PtrType CLASSIFIED_AS_POINTER,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void ConstructPtrType (PtrType CLASSIFIED_AS_POINTER ptr)
{
    //new (ptr) (decltype(*std::declval<PtrType>()))();
    new (ptr) (typename DEREFERENCE_TYPE(PtrType))();
}

//Calls {ConstructorType} constructor on [ptr] (address) then moves the [ptr] one {ConstructorType} size
//Repeats untill there is no space for anpopther {ConstructorType}
template<typename ConstructionType, typename PtrType CLASSIFIED_AS_POINTER,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void ConstructPtrTypeRepeat (PtrType CLASSIFIED_AS_POINTER ptr)
{
    size_t times = sizeof(POINTER_VALUE(ptr)) / sizeof(ConstructionType);
    for(size_t i = 0; i < times; i++)
    {
        ConstructType<ConstructionType>(ptr);
        ptr += sizeof(ConstructionType);
    }
}


///=============================================================================================
///ALLOCATAING SHOULD ONLY BE USED WHEN THE USE OF REALLOC IS BENEFICIAL OTHERWISE USE NEW (C++)
///=============================================================================================

///Allocating
//Allocates [numOfBytes] number of bytes and returns a pointer to the start of the allocated block
inline void RETURN_POINTER AllocateMemory (size_t numOfBytes)
{
    return malloc(numOfBytes);
}

//Allocates all sizeof(Type) bytes and returns a Type pointer to the start of the allocated block
template<typename Type>
inline std::remove_cv<Type> RETURN_POINTER AllocateType ()
{
    return reinterpret_cast<Type POINTER>(AllocateMemory(sizeof(Type)));
}

//Allocates all sizeof...(Types) bytes and returns a void pointer to the start of the allocated block
template<typename ... Types>
inline void RETURN_POINTER AllocateTypes ()
{
    return AllocateMemory(sizeof...(Types));
}

//Allocates all [ptr] bytes and returns a pointer to the start of the allocated block
template<typename PtrType CLASSIFIED_AS_POINTER,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline PtrType CLASSIFIED_AS_POINTER const AllocatePtrType (PtrType CLASSIFIED_AS_POINTER ptr)
{
    return reinterpret_cast<PtrType>(AllocateMemory(sizeof(POINTER_VALUE(ptr))));
}

//Allocates [numOfBytes] number of bytes and returns a pointer to the start of the allocated block
//If successful Sets all allocated bytes to NULL (0)
inline void RETURN_POINTER AllocateMemoryNull (size_t numOfBytes)
{
    //Allocates space
    void* ptr = AllocateMemory(numOfBytes);

    if(ptr == nullptr) //Returns if malloc wasnt successful
        return ptr;

    //Nulls the allocated space
    NullMemory(ptr, numOfBytes);

    return ptr;
}

//Allocates all sizeof(Type) bytes and returns a Type pointer to the start of the allocated block
//If successful Sets all allocated bytes to NULL (0)
template<typename Type>
inline std::remove_cv<Type> RETURN_POINTER AllocateTypeNull ()
{
    return reinterpret_cast<Type POINTER>(AllocateMemoryNull(sizeof(Type)));
}

//Allocates all sizeof...(Types) bytes and returns a void pointer to the start of the allocated block
//If successful Sets all allocated bytes to NULL (0)
template<typename ... Types>
inline void RETURN_POINTER AllocateTypesNull ()
{
    return AllocateMemoryNull(sizeof...(Types));
}

//Allocates all [ptr] bytes and returns a pointer to the start of the allocated block
//If successful Sets all allocated bytes to NULL (0)
template<typename PtrType CLASSIFIED_AS_POINTER,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline PtrType CLASSIFIED_AS_POINTER const AllocatePtrTypeNull (PtrType CLASSIFIED_AS_POINTER ptr)
{
    return reinterpret_cast<PtrType>(AllocateMemoryNull(sizeof(POINTER_VALUE(ptr))));
}


///Dealocating
//Frees allocated space on [ptr] address (does delete(ptr);)
template<typename PtrType,
         std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
inline void Deallocate (PtrType CLASSIFIED_AS_POINTER ptr)
{
    free(ptr);
}


///Realocating
//Reallocates previosly allocated space from [source] to be of the [newSize] size and return a pointer to the final block
//If the operation is unsuccessful returns NULLPTR
template<typename SourceType,
         std::enable_if_t<std::is_pointer<SourceType>::value, int> = 0>
inline void RETURN_POINTER ReallocateMemory (SourceType CLASSIFIED_AS_POINTER source, size_t newSize)
{
    return realloc(reinterpret_cast<void POINTER>(source), newSize);
}

//Same as ReallocateMemory (Is done for syntax consistancy)
template<typename SourceType>
inline void RETURN_POINTER ReallocateType (SourceType PASS_BY_REFERENCE source, size_t newSize)
{
    return ReallocateMemory(ADDRESS(source), newSize);
}

//Reallocates previosly allocated space from [source] to be of the sizeof({DestinationType}) size and return a pointer to the final block
//If the operation is unsuccessful returns NULLPTR
template<typename DestinationType, typename SourceType>
inline std::remove_cv<DestinationType> RETURN_POINTER ReallocateRefToType (SourceType PASS_BY_REFERENCE source)
{
    return reinterpret_cast<DestinationType POINTER>(ReallocateMemory(ADDRESS(source), sizeof(DestinationType)));
}

//Reallocates previosly allocated space from [source] to be of the sizeof({DestinationType}) size and return a pointer to the final block
//If the operation is unsuccessful returns NULLPTR
template<typename DestinationType, typename SourceType CLASSIFIED_AS_POINTER,
         std::enable_if_t<std::is_pointer<SourceType>::value, int> = 0>
inline std::remove_cv<DestinationType> RETURN_POINTER ReallocatePtrToType (SourceType CLASSIFIED_AS_POINTER source)
{
    return reinterpret_cast<DestinationType POINTER>(ReallocateMemory(source, sizeof(DestinationType)));
}

#endif // G_MEMORYFUNCTIONS_H
