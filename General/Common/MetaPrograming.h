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

#include <string>
//ConstexprCString
namespace MetaPrograming
{

    /*
    template<size_t capacity, typename CharT = char>
    struct ConstexprStr
    {
            using CharType = CharT;
            using SizeType = size_t;
            using OffsetType = i64;
            using ThisType = ConstexprStr<capacity, CharType>;

            static constexpr u32 StringLenght(const char* str)
            {
                u32 i = 0;
                while(str[i] != '\0')
                    i++;

                return i;
            }

        protected:
            static constexpr SizeType TotalSize = capacity + 1;
            static constexpr CharT NullTermination = '\0';

            SizeType CurrentSize = 0;
            CharType String[TotalSize] = {};

        public:
            constexpr ConstexprStr() : CurrentSize(0), String()
            {
                String[CurrentSize] = NullTermination;
            }
            constexpr ConstexprStr(const ThisType REF) = default;
            constexpr ConstexprStr(ThisType RVALUE_REF) = default;

            constexpr ConstexprStr(const CharType * string) : CurrentSize(0), String()
            {
                for(SizeType i = 0; i < capacity; i++)
                {
                    String[i] = string[i];
                    if(String[i] == NullTermination)
                    {
                        CurrentSize = i;
                        return;
                    }
                }

                CurrentSize = capacity;
                String[capacity] = NullTermination;
            }

            static constexpr auto Capacity() noexcept {return capacity;}
            constexpr auto Size() const noexcept {return CurrentSize;}

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
            constexpr ThisType REF operator+=(const ThisType other)
            {
                SizeType indexInOther = 0;
                for(; CurrentSize < capacity; CurrentSize++, indexInOther++)
                {
                    String[CurrentSize] = other[indexInOther];
                    if(String[CurrentSize] == NullTermination)
                        break;
                }

                String[capacity] = NullTermination;
                return PTR_VAL(this);
            }

            constexpr ThisType REF operator+=(const CharT singleChar)
            {
                if(CurrentSize < capacity)
                {
                    String[CurrentSize] = singleChar;
                    CurrentSize ++;
                    String[CurrentSize] = NullTermination;
                }
                return PTR_VAL(this);
            }

            constexpr bool operator==(const ThisType string) const
            {
                if(this->CurrentSize != string.CurrentSize)
                    return false;

                for(SizeType i = 0; i < this->CurrentSize; i++)
                {
                    if(this->String[i] != string[i])
                        return false;
                }

                return true;
            }
            constexpr bool operator!=(const ThisType string) const
            {
                if(this->CurrentSize != string.CurrentSize)
                    return true;

                for(SizeType i = 0; i < this->CurrentSize; i++)
                {
                    if(this->String[i] != string[i])
                        return true;
                }

                return false;
            }
            constexpr operator std::basic_string_view<CharT>() const noexcept
            {
                return std::basic_string_view<CharT>(ADDRESS(String[0]), CurrentSize);
            }

        private:
            //Does not check if it is in bounds
            constexpr void FinishStringAt(SizeType atPos)
            {
                CurrentSize = atPos;
                String[atPos] = NullTermination;
            }

        private:
            constexpr bool AssignForwards(const CharT PTR data, SizeType IN size, SizeType IN atPos = 0)
            {
                struct Helper{ static constexpr bool WereAllCharsCopied(SizeType index, SizeType size) {return index == size;} };

                SizeType indexInThis = atPos;
                SizeType indexInData = 0;

                for(;
                    indexInThis < capacity && indexInData < size;
                    indexInThis++, indexInData++)
                {
                    String[indexInThis] = data[indexInData];

                    if(String[indexInThis] == ThisType::NullTermination)
                    {
                        FinishStringAt(indexInThis);
                        return true;
                    }
                }

                FinishStringAt(capacity);

                return Helper::WereAllCharsCopied(indexInData, size);
            }
            constexpr bool AssignBackwards(const CharT PTR data, SizeType IN size, SizeType IN atPos = 0)
            {
                struct Helper{ static constexpr bool WereAllCharsCopied(SizeType index, SizeType size) {return index == size;} };

                SizeType indexInThis = capacity;
                SizeType indexInData = size;

                for(;
                    indexInThis-- > atPos && indexInData-- > 0;
                    )
                {
                    String[indexInThis] = data[indexInData];

                    if(String[indexInThis] == ThisType::NullTermination)
                    {
                        FinishStringAt(indexInThis);
                        return true;
                    }
                }

                FinishStringAt(capacity);

                return Helper::WereAllCharsCopied(indexInData, size);
            }

            inline constexpr bool Assign(const CharT PTR data, SizeType IN size, SizeType IN atPos = 0)
            {
                return AssignForwards(data, size, atPos);
            }

            constexpr bool ShiftForward(SizeType IN by, SizeType IN size, SizeType IN atPos)
            {
                return AssignBackwards(ADDRESS(String[atPos]), size, atPos + by);
            }
            constexpr bool ShiftBackward(SizeType IN by, SizeType IN size, SizeType IN atPos)
            {
                return AssignForwards(ADDRESS(String[atPos]), size, atPos - by);
            }

            constexpr bool Shift(OffsetType IN by, SizeType IN size, SizeType IN atPos)
            {
                if(by > 0)
                    return ShiftForward(static_cast<SizeType>(by), size, atPos);
                else
                    return ShiftBackward(static_cast<SizeType>(-by), size, atPos);
            }

            constexpr void Set(SizeType IN size, SizeType IN atPos)
            {

            }

            constexpr bool Erase(SizeType IN size, SizeType IN atPos)
            {
                ShiftBackward(size, size, atPos);

            }

            inline constexpr bool Assign(ThisType IN other, SizeType IN atPos = 0)
            {
                return Assign(other.String, other.Size(), atPos);
            }
            constexpr bool Append(ThisType IN other)
            {
                return Assign(other, Size());
            }
    };
    */
}


#endif // METAPROGRAMING_H
