#ifndef UTFCONVERSION_H
#define UTFCONVERSION_H

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <string>
#include <array>
#include <type_traits>
#include <codecvt>
#include <locale>

#include "General/Common/Common.h"

//Utf assumed for types
using utf8 = char8;
using utf16 = char16;
using utf32 = char32;
using utfW = charW;

//TODO - maybe move to Detail
template<typename CharType>
using BasicString = std::basic_string<CharType>;
template<typename CharType>
using BasicStringView = std::basic_string_view<CharType>;

namespace Detail
{
    template<size_t WideCharSize>
    struct GetWideCharEquivalentInternal : std::false_type
    {
            static constexpr bool IsSupported()
            {
                return WideCharSize ==  sizeof (utf8)
                       OR WideCharSize == sizeof (utf16)
                       OR WideCharSize == sizeof (utf32);
            }

            static_assert (NOT IsSupported(), "Character Sizes other than 8, 16, 32 bits not supported");
    };

    template<>
    struct GetWideCharEquivalentInternal<sizeof (utf8)> : public TypeIdentity<utf8> {};
    template<>
    struct GetWideCharEquivalentInternal<sizeof (utf16)> : public TypeIdentity<utf16> {};
    template<>
    struct GetWideCharEquivalentInternal<sizeof (utf32)> : public TypeIdentity<utf32> {};

    template <typename T1, typename T2>
    constexpr bool IsSameType = std::is_same_v<T1, T2>;
    template <typename CharType>
    constexpr bool IsValidCharType = std::is_same_v<CharType, utf8> OR
                                            std::is_same_v<CharType, utf16> OR
                                            std::is_same_v<CharType, utf32> OR
                                            std::is_same_v<CharType, utfW>;

    using WideCharEquivalent = Detail::GetWideCharEquivalentInternal<sizeof (utfW)>::type;

    template<typename FromType, typename ToType>
    inline bool CopyStringDataDisregardType(BasicStringView<FromType> from, BasicString<ToType> REF to)
    {
        static_assert(sizeof(FromType) == sizeof(ToType), "CopyStringDataDisregardType : Types must have the same size for safety reasons");

        to.resize(from.size());

        if(to.size() != from.size())
            return false;

        //Byte size must be same now for both from and to
        // the types have the same width (staticly asserted)
        // the sizes must be the same (runtime asserted)
        const size_t byteSize = sizeof (FromType) * from.size();

        memcpy(to.data(), from.data(), byteSize);
        return true;
    }

    template<typename CharType>
    inline auto ConvertAnyNonWideToUtf8(BasicStringView<CharType> convertFrom)
    {
        if constexpr(Detail::IsSameType<CharType, utf8>)
        {
            return convertFrom;
        }
        else if constexpr(Detail::IsSameType<CharType, utf16>)
        {
            std::wstring_convert<std::codecvt_utf8<utf16>, utf16> cvt;
            return cvt.to_bytes(convertFrom.data());
        }
        else if constexpr(Detail::IsSameType<CharType, utf32>)
        {
            std::wstring_convert<std::codecvt_utf8<utf32>, utf32> cvt;
            return cvt.to_bytes(convertFrom.data());
        }
        else
        {
            return "";
        }
    }

    inline BasicString<utf8> ConvertUtfWideToUtf8(BasicStringView<utfW> convertFrom)
    {
        if constexpr(Detail::IsSameType<WideCharEquivalent, utf8>)
        {
            BasicString<utf8> returnStr;
            Detail::CopyStringDataDisregardType(convertFrom, returnStr);
            return returnStr;
        }
        else if constexpr(Detail::IsSameType<WideCharEquivalent, utf16>)
        {
            BasicString<utf16> tempStr;
            Detail::CopyStringDataDisregardType(convertFrom, tempStr);
            return ConvertAnyNonWideToUtf8<utf16>(tempStr);
        }
        else if constexpr(Detail::IsSameType<WideCharEquivalent, utf32>)
        {
            BasicString<utf32> tempStr;
            Detail::CopyStringDataDisregardType(convertFrom, tempStr);
            return ConvertAnyNonWideToUtf8<utf32>(tempStr);
        }
        else
        {
            return "";
        }
    }

    inline BasicString<utfW> ConvertUtf8ToWide(BasicStringView<utf8> convertFrom)
    {
        BasicString<utfW> returnStr;

        if constexpr(Detail::IsSameType<WideCharEquivalent, utf8>)
        {
            Detail::CopyStringDataDisregardType(convertFrom, returnStr);
            return returnStr;
        }

        else if constexpr(Detail::IsSameType<WideCharEquivalent, utf16>)
        {
            std::wstring_convert<std::codecvt_utf8<utf16>, utf16> cvt;
            BasicString<utf16> utf16Str = cvt.from_bytes(convertFrom.data());

            Detail::CopyStringDataDisregardType<utf16>(utf16Str, returnStr);
            return returnStr;
        }
        else if constexpr(Detail::IsSameType<WideCharEquivalent, utf32>)
        {
            std::wstring_convert<std::codecvt_utf8<utf32>, utf32> cvt;
            BasicString<utf32> utf32Str = cvt.from_bytes(convertFrom.data());

            Detail::CopyStringDataDisregardType<utf32>(utf32Str, returnStr);
            return returnStr;
        }

        return returnStr;
    }

}

//TODO - incorporate universal string argument
template<typename CharType>
inline BasicString<utf8> ConverToUtf8(BasicStringView<CharType> convertFrom)
{
    static_assert (Detail::IsValidCharType<CharType>,
            "Invalid char type; Only accepting char, utf16_t, utf32_t, wchar_t");

    if constexpr(Detail::IsSameType<CharType, utfW>)
        return Detail::ConvertUtfWideToUtf8(convertFrom);
    else
        return Detail::ConvertAnyNonWideToUtf8(convertFrom);
}

template<typename CharType>
inline auto ConvertToUtfWide(BasicStringView<CharType> convertFrom)
{
    static_assert (Detail::IsValidCharType<CharType>,
            "Invalid char type; Only accepting char, utf16_t, utf32_t, wchar_t");

    //If already is wide returns it
    if constexpr(Detail::IsSameType<CharType, utfW>)
    {
            return convertFrom;
    }
    else
    {
        BasicString<utf8> temp(Detail::ConvertAnyNonWideToUtf8(convertFrom));
        return Detail::ConvertUtf8ToWide(temp);
    }
}

//TODO - find a better place for this one
//template<typename CharType>
//inline auto ConvertToOsUtf(BasicStringView<CharType> convertFrom)
//{
//    #ifdef FILE_OS_INTERACTON_UTF_WIDE
//    return ConvertToUtfWide(convertFrom);
//    #else
//    return ConvertToUtf8(convertFrom);
//    #endif
//}


void func()
{
    auto convertedName = ConvertToUtfWide(BasicStringView<char>("hello"));
    static_assert(std::is_same_v<decltype (convertedName), BasicString<utfW>>, "Hello");
}


//NEW
/*
std::size_t GetWideConvertedSize(utf8 IN_COPY conevrtFrom[], std::mbstate_t INOUT state)
{
    return mbsrtowcs(NULL, &conevrtFrom, 0, &state);
}

bool ConvertToWide(utf8 IN_COPY conevrtFrom[], utfW convertTo[], std::size_t wideLenght)
{
    std::mbstate_t state = std::mbstate_t();

}

bool ConvertToWide(BasicStringView<utf8> IN conevrtFrom, BasicString<utf8> OUT convertTo)
{
    std::mbstate_t state = std::mbstate_t();
}
*/
//No except function would require the lenght argument as to not make it resize the string

#endif // UTFCONVERSION_H
