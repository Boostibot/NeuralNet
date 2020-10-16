#ifndef UNIVERSALSTRING_H
#define UNIVERSALSTRING_H

#include <string>
#include <string_view>
#include "General/Common/Common.h"

template<typename T>
struct fooFunctor
{
        using ResturnType = int;
        ResturnType ReturnValue;
        inline constexpr operator ResturnType() {return ReturnValue;}

        template <typename CharT>
        inline fooFunctor(const CharT arg)
        {
            ReturnValue = fooImpl<T>(arg);
        }

        template <typename CharT>
        ResturnType fooImpl(const std::basic_string<CharT> str)
        {
            (void)str; // do something with str
            return 1;
        }
};

template <typename T>
fooFunctor(const T * cstr) -> fooFunctor<T>;
template <typename T>
fooFunctor(const std::basic_string<T> str) -> fooFunctor<T>;
template <typename T>
fooFunctor(const std::basic_string_view<T> str) -> fooFunctor<T>;

/*
int main(void)
{
    std::string str = "hello";
    std::wstring str2 = L"hello";
    std::string REF strRef = str;
    int result;
    result = fooFunctor(str);
    result = fooFunctor(str2);
    result = fooFunctor(strRef);
    result = fooFunctor("hello");
    result = fooFunctor(U"hello");
    //result = fooFunctor(1); //Compile time error
}
*/

namespace
{
    template <typename T>
    struct IsStringClassImpl : public std::false_type {};

    template <typename T>
    struct IsStringClassImpl<std::basic_string<T>> : public std::true_type {};

    template <typename T>
    struct IsStringClassImpl<std::basic_string_view<T>> : public std::true_type {};

    template <typename T>
    struct IsStringClass : public IsStringClassImpl<std::remove_cv_t<std::remove_reference_t<T>>> {};

    //Removes one by one each of the qualifier until only the core type is left (int const (&)[5] -> int)
    template<typename T>
    using StringLiteralType = std::remove_all_extents_t<std::remove_const_t<std::remove_reference_t<T>>>;

    template<typename T1, typename T2, typename ReturnT = T1>
    constexpr ReturnT Min(T1 a1, T2 a2)
    {
        if(a1 < a2)
            return a1;

        return a2;
    }

    template<typename T1, typename T2, typename ReturnT = T1>
    constexpr ReturnT Max(T1 a1, T2 a2)
    {
        if(a1 > a2)
            return a1;

        return a2;
    }

    //Gets the size of the array (string literal) ater removing reference
    template<typename T>
    static constexpr u64 ArrayTypeSize = Max<u64, u64>(std::extent_v<std::remove_reference_t<T>>, 1);

    //Creates a string literal from the given type T and CharT (T is used to get the size of array)
    template<typename T, typename CharT>
    using CreateStringLiteralFromType = std::add_lvalue_reference_t<const CharT[ArrayTypeSize<T>]>;

    template<typename T>
    struct IsStringLiteral : public std::is_same<T, CreateStringLiteralFromType<T, StringLiteralType<T>>>
    {};

    template<typename Type>
    struct TypeIdentity {using type = Type;};

    template < template <typename...> class TypeTemplate, typename ...TypeArgs>
    using GetStringClassType = std::conditional_t<IsStringClass<TypeTemplate<TypeArgs...>>::value, MetaPrograming::TypeOnIndex<0, TypeArgs...>, std::false_type>;

    template < typename Type>
    struct StringClassCharTypeImpl
            : TypeIdentity<std::false_type>
    {};

    template < template <typename...> class TypeTemplate, typename ...Args>
    struct StringClassCharTypeImpl<TypeTemplate<Args...>>
            : TypeIdentity<GetStringClassType<TypeTemplate, Args...>>
    {};


    template < typename Type>
    struct IsTypeTemplate : std::false_type {} ;

    template < template <typename...> class TypeTemplate, typename... TemplateArgs>
    struct IsTypeTemplate< TypeTemplate<TemplateArgs...> > : std::true_type {} ;

    template < typename Type>
    struct StringClassCharType : StringClassCharTypeImpl<std::decay_t<Type>> {};
    template <typename T>
    struct StringLiteralCharType : TypeIdentity<StringLiteralType<T>>
    {};
}

template <typename T>
struct IsSupportedString : public std::bool_constant<IsStringClass<T>::value || IsStringLiteral<T>::value>
{};

template < typename Type>
using GetStringCharType =  std::conditional_t<IsStringLiteral<Type>::value, StringLiteralCharType<Type>, StringClassCharType<Type>>;


template <typename CharT>
void fooImplementation(std::basic_string_view<CharT> str)
{
    (void)str; //do something with str
}


template <typename T,
          //Use either this or the static assert inside the function
          std::enable_if_t<IsSupportedString<T>::value, int> = 0>
void foo(T RVALUE_REF str)
{
    static_assert (IsSupportedString<T>::value, "Invalid argument type. Only std::basic_string, "
                                                "std::basic_string_view and string literals are allowed");

    using CharT = typename GetStringCharType<T>::type;

    //const std::basic_string_view<CharT> strView = str;

    //return fooImplementation<CharT>(strView);
}

template <typename T>
void fooGeneral(T RVALUE_REF str)
{
    //static_assert (IsStringClass<T>::value, "No2");
    //static_assert (IsStringLiteral<T>::value, "No2");

    using CharT = typename GetStringCharType<T>::type;
    //using CharT = typename StringClassCharTypeImpl<std::decay_t<T>>::type;

    static_assert (std::is_same_v<CharT, char8>, "");
    static_assert (IsTypeTemplate<std::decay_t<T>>::value, "");
    static_assert (MetaPrograming::IsTypeTemplate<std::decay_t<T>>::value, "");
    //static_assert (IsStringClass<T>::value, "");


    //const std::basic_string_view<CharT> strView = str;

    //return fooImplementation<CharT>(strView);
}


int main2(void)
{
    std::string str = "hello";
    const std::wstring wstr = L"hello";
    //std::string REF strRef = str;
    //std::string_view strView = str;

    static_assert (MetaPrograming::IsTypeTemplate<std::string>::value, "No");

    foo("hello");
    foo(str);
    //fooGeneral("Hello");
    fooGeneral(str);

    //foo(wstr);
    //foo(strRef);
    //foo(strView);
    //foo(1); //Compile time error

    return 0;
}

template <typename CharT>
void fooMultiple(std::basic_string_view<CharT> str1, std::basic_string_view<CharT> str2, std::basic_string_view<CharT> str3)
{
    (void)str1;
    (void)str2;
    (void)str3;
}
template <typename T1, typename T2, typename T3,
          std::enable_if_t<IsSupportedString<T1>::value
                           && IsSupportedString<T2>::value
                           && IsSupportedString<T3>::value, int> = 0>
void fooMultiple(T1 RVALUE_REF str1, T2 RVALUE_REF str2, T3 RVALUE_REF str3)
{
    using CharT = typename GetStringCharType<T1>::type;
    //ideally we should do the above for all three types acd check if are equal...
    return fooMultiple<CharT>(str1, str2, str3);
}
/*
int main3(void)
{
    std::string str = "hello";
    std::string_view strView = "hello";
    fooMultiple("Hello", str, strView);
    fooMultiple(str, str, strView);
    fooMultiple(str, "Hello", strView);
    //fooMultiple("Hello", str, 1); //Compile time error
}
*/

#endif // UNIVERSALSTRING_H
