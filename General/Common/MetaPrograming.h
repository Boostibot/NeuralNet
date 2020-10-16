#ifndef METAPROGRAMING_H
#define METAPROGRAMING_H

#include "General/Common/Common.h"

template<typename Type>
struct TypeIdentity
{
        using type = Type;
};

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
    namespace
    {
        template <typename, template <typename...> typename>
        struct IsInstanceImpl : public std::false_type {};

        template <template <typename...> typename U, typename...Ts>
        struct IsInstanceImpl<U<Ts...>, U> : public std::true_type {};
    }

    template <typename T, template <typename ...> typename U>
    using IsInstance = IsInstanceImpl<std::decay_t<T>, U>;

    template < typename Type>
    struct IsTypeTemplate : std::false_type {} ;

    template < template <typename...> class TypeTemplate, typename... TemplateArgs>
    struct IsTypeTemplate< TypeTemplate<TemplateArgs...> > : std::true_type {} ;


    template <int N, typename... T>
    struct tuple_element;

    template <typename T0, typename... T>
    struct tuple_element<0, T0, T...> {
        typedef T0 type;
    };
    template <int N, typename T0, typename... T>
    struct tuple_element<N, T0, T...> {
        typedef typename tuple_element<N-1, T...>::type type;
    };

    template<int index, typename... Types>
    using TypeOnIndex = typename tuple_element<index, Types...>::type;

    template<typename ...>
    using void_t = void;

    template<typename ... ParentTypes> struct TypePack : public ParentTypes... {};

    struct ValidType {static constexpr bool IsValid = true;};
    struct InvalidType {static constexpr bool IsValid = false;};
    struct EmptyType{};

    enum class Indetifier : u8 {Indentify};

    template<typename SameTo, typename ... ArgumentTypes>
    struct IsTypePresent
    {
            template<Indetifier indetifier, typename FirstType, typename ... OtherArgumentTypes>
            constexpr static bool RecursiveCall()
            {
                if(std::is_same<FirstType, SameTo>::value)
                    return true;
                else
                    return RecursiveCall<indetifier, OtherArgumentTypes...>();
            }
            template<Indetifier indetifier>
            constexpr static bool RecursiveCall()
            {
                return false;
            }

            static constexpr bool value = RecursiveCall<Indetifier::Indentify, ArgumentTypes...>();

    };
    template<typename SameTo, typename ... ArgumentTypes>
    struct AreTypesSameTo
    {
            template<Indetifier indetifier, typename FirstType, typename ... OtherArgumentTypes>
            constexpr static bool RecursiveCall()
            {
                if(std::is_same<FirstType, SameTo>::value == false)
                    return false;
                else
                    return RecursiveCall<indetifier, OtherArgumentTypes...>();
            }
            template<Indetifier indetifier>
            constexpr static bool RecursiveCall()
            {
                return true;
            }

            static constexpr bool value = RecursiveCall<Indetifier::Indentify, ArgumentTypes...>();

    };

    template<typename ... ArgumentTypes>
    using AreTypesSame = AreTypesSameTo<ArgumentTypes...>;
}

//ConstexprCString
namespace MetaPrograming
{

    template<size_t size, typename CharT = char>
    struct ConstexprCStr
    {
            using CharType = CharT;
            using SizeType = size_t;
            using ThisType = ConstexprCStr<size, CharType>;

            static constexpr u32 StringLenght(const char* str)
            {
                u32 i = 0;
                while(str[i] != '\0')
                    i++;

                return i;
            }

            static constexpr SizeType MaxSize = size;
            static constexpr SizeType MaxTotalSize = MaxSize + 1;
            static constexpr CharT NullTermination = '\0';

            SizeType Size = 0;
            CharType String[MaxTotalSize] = {};

            constexpr ConstexprCStr() : Size(0)
            {
                String[Size] = NullTermination;
            }
            constexpr ConstexprCStr(const ThisType REF) = default;
            constexpr ConstexprCStr(ThisType RVALUE_REF) = default;

            constexpr ConstexprCStr(const CharType * string)
            {
                for(SizeType i = 0; i < MaxSize; i++)
                {
                    String[i] = string[i];
                    if(String[i] == NullTermination)
                    {
                        Size = i + 1;
                        return;
                    }
                }

                Size = MaxSize;
                String[MaxSize] = NullTermination;
            }

            constexpr ThisType REF operator=(const ThisType REF) = default;
            constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;
            constexpr const CharType REF operator[](const SizeType at) const
            {
                return String[at];
            }
            constexpr CharType REF operator[](const SizeType at)
            {
                return String[at];
            }
            constexpr ThisType REF operator+=(const ThisType string)
            {
                for(; Size < MaxSize; Size++)
                {
                    String[Size] = string[Size];
                    if(String[Size] == NullTermination)
                        return PTR_VAL(this);
                }

                String[MaxSize] = NullTermination;
                return PTR_VAL(this);
            }
            constexpr bool operator==(const ThisType string) const
            {
                if(this->Size != string.Size)
                    return false;

                for(SizeType i = 0; i < Size; i++)
                {
                    if(this->String[i] != string[i])
                        return false;
                }

                return true;
            }
            constexpr bool operator!=(const ThisType string) const
            {
                if(this->Size != string.Size)
                    return true;

                for(SizeType i = 0; i < Size; i++)
                {
                    if(this->String[i] != string[i])
                        return true;
                }

                return false;
            }

    };

}


#endif // METAPROGRAMING_H
