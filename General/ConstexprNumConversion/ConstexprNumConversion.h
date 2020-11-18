#ifndef COMPILETIMENUMERICCONVERSION_H
#define COMPILETIMENUMERICCONVERSION_H

#include "General/Common/Common.h"
#include <iostream>

namespace  CompileTimeNumericConversion
{
    template<u32 size, typename CharT = char>
    struct CompileTimeString
    {
            //std::array<CharT, size> Str;
            char Str[size] = {};
            bool IsNull = true;

            constexpr CompileTimeString() {}
            constexpr CompileTimeString(const bool isNull) : IsNull(isNull) {}
    };

    constexpr u32 StringLenght(const char* str)
    {
        u32 i = 0;
        while(str[i] != '\0')
            i++;

        return i;
    }

    constexpr u32 StrSizeForConversion(const int num, const u32 base = 10)
    {
        using NumType = int;
        NumType remainder = num;
        u32 i = 0;

        if(num < 0)
        {
            //Sets the remeinder to teh abs of num
            remainder = -num;
            //Adds space for the minus sign
            i ++;
        }

        for(; remainder > 0; i++)
            remainder /= base;

        //+1 for termination
        return i + 1;
    }

    template<u32 strSize, typename CharT = char>
    constexpr CompileTimeString<strSize> ConvertToString(const int num, const u32 base = 10)
    {
        using NumType = int;

        constexpr const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        constexpr u32 charsetLenght = StringLenght(charset);

        NumType remainder = num;
        NumType divisionResult = 0;
        CompileTimeString<strSize> str;

        //If base is too large
        if(base > charsetLenght)
            return str;

        u32 sizeNeeded = StrSizeForConversion(num, base);

        //If the size needed is bigger then the one given return
        if(sizeNeeded > strSize)
            return str;

        if(num < 0)
            remainder = -num;

        u32 i = sizeNeeded - 1;
        str.Str[i] = '\0';
        for(i--; remainder > 0; i--)
        {
            divisionResult = remainder / base;

            str.Str[i] = charset[remainder - divisionResult * base];
            remainder = divisionResult;
        }

        //Adds the minus sign
        if(num < 0)
            str.Str[i] = '-';

        str.IsNull = false;
        return str;
    }

    template <int num, u32 base = 10, typename CharT = char>
    struct Convert
    {
        public:
            static constexpr const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        private:
            static constexpr bool AssertIfBaseIsInsufficient()
            {
                if constexpr(base > StringLenght(charset))
                {
                    static_assert (base < StringLenght(charset), "CompileTimeNumericConversion::Convert: Base is too big");
                    return false;
                }
                return true;
            }

        public:
            static constexpr u32 Size = StrSizeForConversion(num, base);
            static constexpr CompileTimeString<Size, CharT> CmpStr = ConvertToString<Size, CharT>(num, base);
            static constexpr bool IsNull = AssertIfBaseIsInsufficient();
            static constexpr const CharT* Value = CmpStr.Str;
    };


    HEADER_ONLY void ConstexprNumConversionUse()
    {
        using namespace MetaPrograming;
        using namespace CompileTimeNumericConversion;

        //constexpr u32 charsetLenght = StringLenght(charset);
        //constexpr CompileTimeString<32> str = CompileTimeString<32>(true);
        CompileTimeString<StrSizeForConversion(-1358)> converted = ConvertToString<StrSizeForConversion(-1358)>(-1358);
        converted = ConvertToString<6>(1358);

        std::cout << Convert<123456>::Value << std::endl;
        std::cout << Convert<-123456>::Value << std::endl;
    }
}

#endif // COMPILETIMENUMERICCONVERSION_H
