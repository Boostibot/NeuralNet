#ifndef G_GENERALFUNCTIONS_H
#define G_GENERALFUNCTIONS_H

#include "G_Defines.h"
#include <type_traits>

namespace G
{
    ///Working with pointers
    //Returns an address of a variable
    template<typename ItemType>
    inline ItemType RETURN_POINTER Address (const ItemType PASS_BY_REFERENCE item)
    {
        return const_cast<ItemType POINTER>(ADRESS(item));
    }
/*
    //Returns an address of a variable in the given type
    template<typename ReturnType CLASSIFIED_AS_POINTER, typename ItemType,
             std::enable_if_t<std::is_pointer<ReturnType>::value, int> = 0>
    inline ReturnType CLASSIFIED_AS_POINTER Address (const ItemType PASS_BY_REFERENCE item)
    {
        return const_cast<ReturnType>(ADDRESS(item));
    }
*/
    //Returns the given data as the given type (Works on any conversion)
    //When sizeof(oldDataType) > sizeof(newDataType) the function behaviour is unspecified (Can even crash sometimes)
    template<typename NewDataType, typename OldDataType>
    inline NewDataType RETURN_REF HandleAs (const OldDataType PASS_BY_REFERENCE data)
    {
        return POINTER_VALUE(const_cast<NewDataType*>(ADDRESS(data)));
    }


    ///Working with templates
    //Returns true if two classes are derived from the same template
    //Can be only used on classes with typename only template arguments
    template<typename, typename>
    constexpr bool is_same_type_template{false};


    template<   template<typename...> class T, //typename T in C++17
                typename... A,
                typename... B>
    constexpr bool is_same_type_template<
        T<A...>,
        T<B...>>
    {true};


    template<int N, typename... Ts> using NthTypeOf =
            typename std::tuple_element<N, std::tuple<Ts...>>::type;
}

#endif // G_GENERALFUNCTIONS_H
