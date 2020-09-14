#ifndef METAPROGRAMING_H
#define METAPROGRAMING_H

#include "GClasses/Common/G_Common.h"

template <typename ValueTypeArg, ValueTypeArg valueArg>
struct ValueTemplate
{
        using ValueType = ValueTypeArg;
        static constexpr ValueType Value = valueArg;
};

template<bool value>
using BoolVal = ValueTemplate<bool, value>;

template<char value>
using CharVal = ValueTemplate<char, value>;

template<unsigned char value>
using ByteVal = ValueTemplate<unsigned char, value>;

template<int value>
using IntVal = ValueTemplate<int, value>;

template<unsigned int value>
using UintVal = ValueTemplate<unsigned int, value>;


namespace MetaPrograming
{       
    template<typename ...>
    using void_t = void;

    template<typename ... ParentTypes> struct TypePack : public ParentTypes... {};

    struct ValidType {static constexpr bool IsValid = true;};
    struct InvalidType {static constexpr bool IsValid = false;};
    struct EmptyType{};

    template<typename SameTo, typename ... ArgumentTypes>
    struct IsTypePresent
    {
            template<bool indetifier, typename FirstType, typename ... OtherArgumentTypes>
            constexpr static bool RecursiveCall()
            {
                if(std::is_same<FirstType, SameTo>::value)
                    return true;
                else
                    return RecursiveCall<indetifier, OtherArgumentTypes...>();
            }
            template<bool indetifier>
            constexpr static bool RecursiveCall()
            {
                return false;
            }

            static constexpr bool value = RecursiveCall<true, ArgumentTypes...>();

    };
}



#endif // METAPROGRAMING_H
