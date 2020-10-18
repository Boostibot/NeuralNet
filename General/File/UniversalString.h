#ifndef UNIVERSALSTRING_H
#define UNIVERSALSTRING_H

#include <string>
#include <string_view>
#include "General/Common/Common.h"


using FalseType = std::false_type;
using TrueType = std::true_type;

namespace Detail
{
    template <typename T>
    struct GetStringClassTypeImpl : TypeIdentity<FalseType> {};

    template <typename T>
    struct GetStringClassTypeImpl<std::basic_string<T>> : TypeIdentity<T> {};

    template <typename T>
    struct GetStringClassTypeImpl<std::basic_string_view<T>> : TypeIdentity<T> {};
}


//GetStringClassType
template <typename T>
struct GetStringClassType : Detail::GetStringClassTypeImpl<std::decay_t<T>> {};

template <typename T>
using GetStringClassType_t = typename GetStringClassType<T>::type;


//IsStringClass
template <typename T>
struct IsStringClass : std::bool_constant<NOT std::is_same_v<typename GetStringClassType<T>::type, FalseType>>{};

template <typename T>
constexpr bool IsStringClass_v{IsStringClass<T>::value};


//GetCStringType
template<typename T>
struct GetCStringType : std::decay<std::remove_pointer_t<std::decay_t<T>>>
{};

template<typename T>
using GetCStringType_t = typename GetCStringType<T>::type;


//IsCString
template<class T>
struct IsCString : std::bool_constant<
      std::is_same_v<GetCStringType_t<T> const *,   std::decay_t<T>> ||
      std::is_same_v<GetCStringType_t<T> *,         std::decay_t<T>>>
{};

template <typename T>
constexpr bool IsCString_v{IsCString<T>::value};


//IsStringLiteral
template<typename T>
struct IsStringLiteral :
    std::is_same<T, std::add_lvalue_reference_t<const GetCStringType_t<T> [std::extent_v<std::remove_reference_t<T>>]>>
{};

template <typename T>
constexpr bool IsStringLiteral_v{IsStringLiteral<T>::value};


//IsAnyString
template<typename T>
struct IsAnyString : std::bool_constant<IsCString_v<T> || IsStringClass_v<T>>
{};

template<typename T>
constexpr bool IsAnyString_v {IsAnyString<T>::value};


//GetAnyStringType
template<typename T>
struct GetAnyStringType : std::conditional<IsCString<T>::value, GetCStringType_t<T>, GetStringClassType_t<T>>
{};

template<typename T>
using GetAnyStringType_t = typename GetAnyStringType<T>::type;



namespace Testing
{
    template< typename ... T>
    struct TestingStruct;

    void IsStringClassTesting()
    {
        static_assert (IsStringClass<std::string>::value, "");
        static_assert (IsStringClass<std::basic_string<char16>>::value, "");

        static_assert (NOT std::is_same_v<typename GetStringClassType<std::string>::type, FalseType>, "");
        static_assert (NOT std::is_same_v<typename GetStringClassType<std::wstring>::type, FalseType>, "");
        static_assert (NOT std::is_same_v<typename GetStringClassType<std::u16string>::type, FalseType>, "");

        static_assert (NOT std::is_same_v<typename GetStringClassType<std::string_view>::type, FalseType>, "");
        static_assert (NOT std::is_same_v<typename GetStringClassType<std::wstring_view>::type, FalseType>, "");
        static_assert (NOT std::is_same_v<typename GetStringClassType<std::u16string_view>::type, FalseType>, "");

        static_assert (std::is_same_v<typename GetStringClassType<std::string_view>::type, char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const std::string_view>::type, char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const volatile std::string_view>::type, char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const volatile std::string_view REF>::type, char8>, "");

        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string<char8>>::type, char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string<char16>>::type, char16>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string<char32>>::type, char32>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string<charW>>::type, charW>, "");

        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string_view<char8>>::type, char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string_view<char16>>::type, char16>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string_view<char32>>::type, char32>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string_view<charW>>::type, charW>, "");

        static_assert (NOT IsStringClass<int>::value, "");
        static_assert (NOT IsStringClass<double>::value, "");
        static_assert (NOT IsStringClass<TestingStruct<int, double>>::value, "");
        static_assert (NOT IsStringClass<std::integral_constant<int, 0>>::value, "");
    }

    void StringLiteralTesting()
    {
        using StrLiteralType = decltype ("Hello");
        using DecayStrLiteral = std::decay_t<StrLiteralType>;
        using RemovePtrDecayStr = std::remove_pointer_t<DecayStrLiteral>;
        using RemoveConstDecayStr = std::decay_t<RemovePtrDecayStr>;

        static_assert (NOT std::is_same_v<StrLiteralType, char8>, "");
        static_assert (NOT std::is_same_v<DecayStrLiteral, char8>, "");
        static_assert (NOT std::is_same_v<RemovePtrDecayStr, char8>, "");
        static_assert (std::is_same_v<RemoveConstDecayStr, char8>, "");

        static_assert (IsCString<DecayStrLiteral>::value, "");
        static_assert (IsCString<char8 *>::value, "");
        static_assert (IsCString<const char8 *>::value, "");
        static_assert (IsCString<const char8 * const>::value, "");
        static_assert (NOT IsCString<charW>::value, "");
        static_assert (IsCString<charW *>::value, "");
        static_assert (IsCString<const charW *>::value, "");
        static_assert (IsCString<const charW * const>::value, "");

        static_assert (std::is_same_v<GetCStringType<char8>::type, char8>, "");
        static_assert (std::is_same_v<GetCStringType<char16>::type, char16>, "");
        static_assert (std::is_same_v<GetCStringType<char32>::type, char32>, "");

        static_assert (std::is_same_v<GetCStringType<const char32 *>::type, char32>, "");
        static_assert (std::is_same_v<GetCStringType<const char32 * const>::type, char32>, "");
    }

    void IsAnyStringTesting()
    {
        static_assert (IsAnyString_v<char8 *>, "");
        static_assert (IsAnyString_v<const char8 *>, "");
        static_assert (IsAnyString_v<const char8 * const>, "");

        static_assert (IsAnyString_v<char16 *>, "");
        static_assert (IsAnyString_v<const char16 *>, "");
        static_assert (IsAnyString_v<const char16 * const>, "");

        static_assert (IsAnyString_v<std::basic_string<char8>>, "");
        static_assert (IsAnyString_v<std::basic_string_view<char8>>, "");

        static_assert (IsAnyString_v<std::basic_string<char16>>, "");
        static_assert (IsAnyString_v<std::basic_string_view<char16>>, "");

        static_assert (IsAnyString_v<decltype ("Hello")>, "");
        static_assert (IsAnyString_v<decltype (u"Hello")>, "");

        static_assert (IsAnyString_v<decltype (u"Hello")>, "");

        static_assert (NOT IsAnyString_v<int>, "");
        static_assert (NOT IsAnyString_v<double>, "");
        static_assert (NOT IsAnyString_v<TestingStruct<int>>, "");
        static_assert (NOT IsAnyString_v<TestingStruct<int, double>>, "");
        static_assert (NOT IsAnyString_v<std::integral_constant<int, 0>>, "");
    }

    void GetStrTesting()
    {
        static_assert (std::is_same_v<GetAnyStringType_t<char8 *>, char8>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<const char8 *>, char8>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<const char8 * const>, char8>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<char16 *>, char16>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<const char16 *>, char16>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<const char16 * const>, char16>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<std::basic_string<char8>>, char8>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<std::basic_string_view<char8>>, char8>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<std::basic_string<char16>>, char16>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<std::basic_string_view<char16>>, char16>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<decltype ("Hello")>, char8>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<decltype (u"Hello")>, char16>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<decltype (u"Hello")>, char16>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<int>, FalseType>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<double>, FalseType>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<TestingStruct<int>>, FalseType>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<TestingStruct<int, double>>, FalseType>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<std::integral_constant<int, 0>>, FalseType>, "");
    }

}

#endif // UNIVERSALSTRINGNEW_H
