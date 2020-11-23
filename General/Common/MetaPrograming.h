#ifndef METAPROGRAMING_H
#define METAPROGRAMING_H

#include <type_traits>
#include "General/Common/Common.h"

template<typename Type>
struct TypeIdentity
{
        using type = Type;
};

/*
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
*/

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
}

namespace MetaPrograming
{
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
}

namespace MetaPrograming
{
    //template<typename ...>
    //using void_t = void;

    //template<typename ... ParentTypes> struct TypePack : public ParentTypes... {};

    //struct ValidType {static constexpr bool IsValid = true;};
    //struct InvalidType {static constexpr bool IsValid = false;};
    //struct EmptyType{};

    enum class Indetifier : u8 {Indentify};
}

namespace MetaPrograming
{
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

namespace MetaPrograming
{
    //TODO - fix
    template<typename FunctionType, typename ResultType, typename FirstType, typename ... OtherArgumentTypes>
    constexpr static auto EvaluateFunctionRecursivelyBackwards(FunctionType REF func, FirstType firstValue, OtherArgumentTypes... values)
    {
        if(sizeof... (values) > 0)
            return func(firstValue, EvaluateFunctionRecursivelyBackFirst(func, values...));
        else
            return firstValue;
    }
    template<typename FunctionType, typename ResultType, typename FirstType, typename ... OtherArgumentTypes>
    constexpr static auto EvaluateFunctionRecursivelyBackwards(FunctionType REF, ResultType currentResult)
    {
        return currentResult;
    }
    template<typename FunctionType, typename ResultType, typename FirstType, typename ... OtherArgumentTypes>
    constexpr static auto EvaluateFunctionRecursivelyForwards(FunctionType REF func, ResultType currentResult, FirstType firstValue, OtherArgumentTypes... values)
    {
        return EvaluateFunctionRecursivelyForwards(func, func(currentResult, firstValue), values...);
    }
    template<typename FunctionType, typename ResultType, typename FirstType, typename ... OtherArgumentTypes>
    constexpr static auto EvaluateFunctionRecursivelyForwards(FunctionType REF, ResultType currentResult)
    {
        return currentResult;
    }


    //Functions
    template<typename FirstType, typename SecondType>
    constexpr static auto AndFunction(FirstType firstValue, SecondType secondValue)
    {
        return firstValue && secondValue;
    }
    template<typename FirstType, typename SecondType>
    constexpr static auto OrFunction(FirstType firstValue, SecondType secondValue)
    {
        return firstValue || secondValue;
    }
    template<typename FirstType, typename SecondType>
    constexpr static auto EqualsFunction(FirstType firstValue, SecondType secondValue)
    {
        return firstValue == secondValue;
    }
    template<typename FirstType, typename SecondType>
    constexpr static auto NotEqualsFunction(FirstType firstValue, SecondType secondValue)
    {
        return firstValue != secondValue;
    }
    template<typename FirstType, typename SecondType>
    constexpr static auto BiggerFunction(FirstType firstValue, SecondType secondValue)
    {
        return firstValue > secondValue;
    }
    template<typename FirstType, typename SecondType>
    constexpr static auto BiggerOrEqualFunction(FirstType firstValue, SecondType secondValue)
    {
        return firstValue >= secondValue;
    }
    template<typename FirstType, typename SecondType>
    constexpr static auto SmallerFunction(FirstType firstValue, SecondType secondValue)
    {
        return firstValue < secondValue;
    }
    template<typename FirstType, typename SecondType>
    constexpr static auto SmallerOrEqualFunction(FirstType firstValue, SecondType secondValue)
    {
        return firstValue < secondValue;
    }


    template<typename ... ArgumentTypes>
    constexpr static auto AndApplyFunctionImplemented(ArgumentTypes... args)
    {
        static_assert (sizeof...(args) > 0, "No arguments provided");
        return EvaluateFunctionRecursivelyForwards(AndFunction<bool, bool>, args...);
    }

    template<Indetifier identifier>
    constexpr static bool AndInternal()
    {
        return true;
    }
    template<Indetifier identifier, typename FirstType, typename ... OtherArgumentTypes>
    constexpr static bool AndInternal(FirstType firstValue, OtherArgumentTypes... values)
    {
        return firstValue && AndInternal<identifier>(values...);
    }
    template<typename ... ArgumentTypes>
    constexpr bool And(ArgumentTypes... values)
    {
        return AndInternal<Indetifier::Indentify, ArgumentTypes...>(values...);
    }

    template<Indetifier identifier>
    constexpr static bool OrInternal()
    {
        return false;
    }
    template<Indetifier identifier, typename FirstType, typename ... OtherArgumentTypes>
    constexpr static bool OrInternal(FirstType firstValue, OtherArgumentTypes... values)
    {
        return firstValue || OrInternal<identifier>(values...);
    }
    template<typename ... ArgumentTypes>
    constexpr bool Or(ArgumentTypes... values)
    {
        return OrInternal<Indetifier::Indentify, ArgumentTypes...>(values...);
    }
}

/*
namespace MetaPrograming
{
    //Used to signal errors thorugh default templte arguments
    struct ErrorSignalType{};

    //template<typename Signal = MetaPrograming::ErrorSignalType>
    //void SomeFunction(int)
    //{
    //    static_assert (NOT std::is_same_v<Signal, MetaPrograming::ErrorSignalType>, "Is unimplmeneted");
    //}
}
*/


#endif // METAPROGRAMING_H
