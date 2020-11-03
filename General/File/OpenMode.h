#ifndef OPENMODE_H
#define OPENMODE_H

#include <string>
#include <string_view>

#include "General/Common/Common.h"

namespace CIo
{
    //Struct holding the OpenMode and the associated functionality
    //Is in this struct to separate the declarations into multiple "namespace" structs
    //This holder is ment to be inherited into other classes using OpenMode to ensure
    // the fucntions are distributed to the correct namespace levels

    struct OpenModeSimpleHolder
    {
            //The OpenModeFlag should be directly below the class which inherits this type (File)
            // So it is possible too write File::OpenModeFlag::Write
            // This is a significant improvement over File::OpenMode::OpenModeFlag::Write
            //
            //It is safer to have every enum declared as enum class so there is no automatic
            // degradation to the underlying type. Because of this its necessary to have the
            // OpenModeFlag name present.
            using OpenModeFlagUnderlyingType = u8;
            //This is used as the actual argument for OpenMode
            enum class OpenModeFlag : OpenModeFlagUnderlyingType
            {
                Read,
                Write,
                Append,
                MustExist,

                Text,
                Binary,

                MustNotExist,

                CommitDirectlyToDisk,
                CommitIndirectlyToDisk,

                NotInheritedByChildProcess,

                SequntialAccessOptimized,
                RandomAccessOptimized,

                IfPossibleNoFlushingToDisk,

                DeleteAfterClose,

                UnicodeEncoding,
                Utf8Encoding,
                Utf16Encoding

                //***************************************//
                //  OpenModeOptionsCount needs to be     //
                //  updated to match the actual number   //
                //             of options                //
                //***************************************//
                //This is to not bother the user with the count option
            };

        protected:
            static constexpr u32 OpenModeOptionsCount = 17;

        public:
            enum class COpenMode : OpenModeFlagUnderlyingType
            {
                ReadMustExist,      //Read && MustExist
                Write,              //Write
                WriteAppend,        //Write && Append
                ReadWriteMustExist, //Read && Write && MustExist
                ReadWrite,          //Read && Write
                ReadWriteAppend     //Read && Write && Append
            };
    };

    template<size_t capacity, typename CharT>
    struct SimpleConstexprString
    {
            using SizeType = size_t;
            using ThisType = SimpleConstexprString<capacity, CharT>;

        protected:
            static constexpr SizeType TotalSize = capacity + 1;
            static constexpr CharT NullTermination = '\0';

            SizeType CurrentSize;
            std::array<CharT, capacity> String;

        public:
            constexpr SimpleConstexprString() : CurrentSize(0), String()
            {
                String[CurrentSize] = NullTermination;
            }

            constexpr SimpleConstexprString(const CharT * string) : CurrentSize(0), String()
            {
                for(SizeType i = 0; i < capacity; i++)
                {
                    String[i] = string[i];
                    if(String[i] == NullTermination)
                    {
                        CurrentSize = i + 1;
                        return;
                    }
                }

                CurrentSize = capacity;
                String[capacity] = NullTermination;
            }

            static constexpr auto Capacity() noexcept {return capacity;}
            constexpr auto Size() const noexcept {return CurrentSize;}

            constexpr const CharT REF operator[](const SizeType at) const
            {
                return String[at];
            }
            constexpr CharT REF operator[](const SizeType at)
            {
                return String[at];
            }

            constexpr ThisType REF operator+=(const ThisType string) noexcept
            {
                for(; CurrentSize < capacity; CurrentSize++)
                {
                    String[CurrentSize] = string[CurrentSize];
                    if(String[CurrentSize] == NullTermination)
                        return PTR_VAL(this);
                }

                String[capacity] = NullTermination;
                return PTR_VAL(this);
            }

            constexpr ThisType REF operator+=(const CharT singleChar) noexcept
            {
                if(CurrentSize < capacity)
                {
                    String[CurrentSize] = singleChar;
                    CurrentSize ++;
                }
                return PTR_VAL(this);
            }

            constexpr operator std::basic_string_view<CharT>() const noexcept
            {
                return std::basic_string_view<CharT>(String.data(), CurrentSize);
            }
    };

    template<typename OsCharTpeArg>
    struct OpenModeHolder
    {
            class OpenMode : OpenModeSimpleHolder
            {
                public:
                    using ThisType = OpenMode;
                    using OsCharType = OsCharTpeArg;
                    using OsStringView = std::basic_string_view<OsCharType>;

                protected:
                    using OpenModeFlagUnderlyingType = OpenModeSimpleHolder::OpenModeFlagUnderlyingType;
                    using OpenModeFlag = OpenModeSimpleHolder::OpenModeFlag;
                    using COpenMode = OpenModeSimpleHolder::COpenMode;
                    static constexpr u32 OpenModeOptionsCount = OpenModeSimpleHolder::OpenModeOptionsCount;

                protected:
                    enum class OpenModeValidity : OpenModeFlagUnderlyingType
                    {
                        Valid,              //Arguemnts are valid
                        Invalid,            //Arguments are invalid. ie. both Read and Write are missing
                        Unsupported,        //Arguments that cannot be parsed due to fopen argument limitations
                                            // ie. all combinations containing Append && MustExist
                    };

                    struct COpenModeWithValidty
                    {
                            OpenModeValidity Validity = {};
                            COpenMode OpenMode = {};

                            constexpr COpenModeWithValidty(COpenMode mode, OpenModeValidity val = OpenModeValidity::Valid) : Validity(val), OpenMode(mode) {}
                            constexpr COpenModeWithValidty(OpenModeValidity val) : Validity(val), OpenMode(COpenMode::ReadWrite) {}
                    };


                    struct FlagPresence
                    {
                            bool Presence[OpenModeOptionsCount] = {};
                            constexpr bool REF operator[](OpenModeFlag modeArg)
                            {
                                return Presence[static_cast<OpenModeFlagUnderlyingType>(modeArg)];
                            }
                            constexpr bool operator[](OpenModeFlag modeArg) const
                            {
                                return Presence[static_cast<OpenModeFlagUnderlyingType>(modeArg)];
                            }
                    };



                protected:
                    static constexpr u32 OpenModeMaxChars = 26;
                    static constexpr COpenMode DefaultCOpenMode = COpenMode::ReadWrite;
                    static constexpr OpenModeValidity DefaultValidity = OpenModeValidity::Invalid;
                    template<size_t capacity, typename CharT>
                    using ConstexprStr = MetaPrograming::ConstexprStr<capacity, CharT>;
                    using OpenModeString = ConstexprStr<OpenModeMaxChars, OsCharType>;
                    //using FlagPresence = std::array<bool, OpenModeOptionsCount>;

                protected:
                    OpenModeString OpenModeStr;
                    COpenMode COpenModeCode;
                    OpenModeValidity Validity;

                public:
                    constexpr OpenMode() noexcept
                        : OpenModeStr(), COpenModeCode(ThisType::DefaultCOpenMode), Validity(ThisType::DefaultValidity)
                    {}

                    constexpr OpenMode(COpenMode openMode) noexcept
                        : OpenModeStr(), COpenModeCode(), Validity()
                    {
                        AssignCOpenMode(openMode);
                    }

                public:
                    constexpr OpenMode(const ThisType REF other) : OpenModeStr(other.OpenModeStr), COpenModeCode(other.COpenModeCode), Validity(other.Validity) {};
                    constexpr OpenMode(ThisType RVALUE_REF other) : OpenModeStr(other.OpenModeStr), COpenModeCode(other.COpenModeCode), Validity(other.Validity) {};

                    constexpr ThisType REF operator=(const ThisType REF) = default;
                    constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;

                    template<typename ... FlagTypes,
                             std::enable_if_t<MetaPrograming::AreTypesSameTo<OpenModeFlag, FlagTypes...>::value, u32> = 0>
                    constexpr OpenMode(FlagTypes ... flags) noexcept
                        : OpenModeStr(), COpenModeCode(), Validity()
                    {
                        this->operator =(GetOpenMode<FlagTypes ...>(flags...));
                    }

                    constexpr void AssignCOpenMode(COpenMode cOpenMode) noexcept
                    {
                        this->COpenModeCode = cOpenMode;
                        this->Validity = OpenModeValidity::Valid;
                        this->OpenModeStr = GetCOpenModeStr(cOpenMode);
                    }

                    template<typename ... FlagTypes>
                    static constexpr OpenMode GetOpenMode(FlagTypes ... flags) noexcept
                    {
                        static_assert (MetaPrograming::AreTypesSameTo<OpenModeFlag, FlagTypes...>::value,
                                "CFile::OpenMode::GetOpenMode : All provided flags must be of type OpenModeFlag");

                        FlagPresence flagPresence;
                        ThisType::FillFlagPresence<FlagTypes...>(flagPresence, flags...);

                        if(ThisType::AreOpenModesConflicting(flagPresence))
                        {
                        //return OpenMode(OpenModeValidity::Invalid);
                        return OpenModeConstructor(OpenModeValidity::Invalid);
                        }

                        OpenMode openMode = OpenModeConstructor(ThisType::GetCOpenMode(flagPresence));
                        //OpenMode openMode = ThisType::GetCOpenMode(flagPresence);

                        if(openMode.IsValid() == false)
                        return openMode;

                        openMode.OpenModeStr += ThisType::GetCOpenModeStr(openMode.COpenModeCode);
                        openMode.OpenModeStr += ThisType::GetAdditionalModeStr(flagPresence);

                        return openMode;
                    }

                    template<OpenModeFlag ... openModeArguments>
                    static inline constexpr OpenMode GetOpenMode() noexcept
                    {
                        constexpr OpenMode openMode = GetOpenMode(openModeArguments...);

                        static_assert (openMode.IsValid(), "CFile::OpenMode::GetOpenMode : Invalid open mode arguments; See OpenMode::AreOpenModesConflicting(...) for more info");
                        static_assert (openMode.IsSupported(), "CFile::OpenMode::GetOpenMode : Unsupported open mode argument combination; "
                                                               "The combination of mode flags is unsupported by the underlying fopen function;"
                                                               "This Is probably cause by having both OpenModeFlag::Append and OpenModeFlag::MustExist flags present");
                        return openMode;
                    }

                    inline constexpr bool IsValid() const noexcept
                    {
                        return this->Validity == OpenModeValidity::Valid;
                    }
                    inline constexpr bool IsSupported() const noexcept
                    {
                        return NOT(this->Validity == OpenModeValidity::Unsupported);
                    }

                    inline constexpr OsStringView GetStr() const noexcept
                    {
                        return OpenModeStr.operator std::basic_string_view<OsCharType>();
                    }
                    inline constexpr operator OsStringView() const noexcept
                    {
                        return this->GetStr();
                    }

                    //Helpers

                protected:
                    //For some reason if this is implemented as constructor it breaks (only when compiled with MSVC)
                    //Is protected because the user should have no direct control over validity
                    //constexpr OpenMode(OpenModeValidity validity, COpenMode openMode = ThisType::DefaultCOpenMode) noexcept
                    //    : OpenModeStr(), COpenModeCode(openMode), Validity(validity)
                    //{}

                    //constexpr OpenMode(COpenModeWithValidty pair) noexcept
                    //    : OpenModeStr(), COpenModeCode(pair.OpenMode), Validity(pair.Validity)
                    //{}

                    static constexpr OpenMode OpenModeConstructor(OpenModeValidity validity, COpenMode copenMode = ThisType::DefaultCOpenMode) noexcept
                    {
                        OpenMode openMode;
                        openMode.Validity = validity;
                        openMode.COpenModeCode = copenMode;
                        return openMode;
                    }
                    static constexpr inline  OpenMode OpenModeConstructor(COpenModeWithValidty copenModeWithValidity) noexcept
                    {
                        return OpenModeConstructor(copenModeWithValidity.Validity, copenModeWithValidity.OpenMode);
                    }
                    template<typename ... FlagTypes>
                    static constexpr void FillFlagPresence(FlagPresence REF presenceArray, const FlagTypes ... flags) noexcept
                    {
                        using Underlying = OpenModeFlagUnderlyingType;

                        for(Underlying i = 0; i < OpenModeOptionsCount; i++)
                        {
                            const OpenModeFlag currFlag = static_cast<OpenModeFlag>(i);
                            presenceArray[currFlag] = ThisType::IsFlagPresent<FlagTypes...>(currFlag, flags...);
                        }
                    }

                    static constexpr bool AreOpenModesConflicting(const FlagPresence REF modes) noexcept
                    {
                        if(modes[OpenModeFlag::MustExist] && modes[OpenModeFlag::MustNotExist])
                            return true;

                        if(modes[OpenModeFlag::Text] && modes[OpenModeFlag::Binary])
                            return true;

                        if(modes[OpenModeFlag::CommitDirectlyToDisk] && modes[OpenModeFlag::CommitIndirectlyToDisk])
                            return true;

                        if(modes[OpenModeFlag::SequntialAccessOptimized] && modes[OpenModeFlag::RandomAccessOptimized])
                            return true;

                        //Encodings
                        if(modes[OpenModeFlag::UnicodeEncoding] && modes[OpenModeFlag::Utf8Encoding])
                            return true;

                        if(modes[OpenModeFlag::Utf16Encoding] && modes[OpenModeFlag::Utf8Encoding])
                            return true;

                        if(modes[OpenModeFlag::UnicodeEncoding] && modes[OpenModeFlag::Utf16Encoding])
                            return true;

                        return false;
                    }

                    static constexpr COpenModeWithValidty GetCOpenMode(const FlagPresence REF presenceArray) noexcept
                    {
                        const bool read         = presenceArray[OpenModeFlag::Read];
                        const bool write        = presenceArray[OpenModeFlag::Write];
                        const bool append       = presenceArray[OpenModeFlag::Append];
                        const bool mustExist    = presenceArray[OpenModeFlag::MustExist];

                        #define BINARY(a, b, c, d) 8*a + 4*b + 2*c + 1*d

                        switch (BINARY(read, write, append, mustExist))
                        {
                            //          R  W  A  M
                            case BINARY(0, 0, 0, 0): return OpenModeValidity::Invalid;
                            case BINARY(0, 0, 0, 1): return OpenModeValidity::Invalid;
                            case BINARY(0, 0, 1, 0): return OpenModeValidity::Invalid;
                            case BINARY(0, 0, 1, 1): return OpenModeValidity::Invalid;
                            case BINARY(0, 1, 0, 0): return COpenMode::Write;
                            case BINARY(0, 1, 0, 1): return COpenMode::ReadWriteMustExist;
                            case BINARY(0, 1, 1, 0): return COpenMode::WriteAppend;
                            case BINARY(0, 1, 1, 1): return OpenModeValidity::Unsupported;
                            case BINARY(1, 0, 0, 0): return COpenMode::ReadWrite;
                            case BINARY(1, 0, 0, 1): return COpenMode::ReadMustExist;
                            case BINARY(1, 0, 1, 0): return COpenMode::ReadWriteAppend;
                            case BINARY(1, 0, 1, 1): return OpenModeValidity::Unsupported;
                            case BINARY(1, 1, 0, 0): return COpenMode::ReadWrite;
                            case BINARY(1, 1, 0, 1): return COpenMode::ReadWriteMustExist;
                            case BINARY(1, 1, 1, 0): return COpenMode::ReadWriteAppend;
                            case BINARY(1, 1, 1, 1): return OpenModeValidity::Unsupported;

                            default: return OpenModeValidity::Invalid;
                        }

                        #undef BINARY
                    }

                    static constexpr OpenModeString GetCOpenModeStr(const COpenMode mode) noexcept
                    {
                        switch(mode)
                        {
                            case COpenMode::ReadMustExist:      return ConvertStr<char8, OsCharType>("r");
                            case COpenMode::Write:              return ConvertStr<char8, OsCharType>("w");
                            case COpenMode::WriteAppend:        return ConvertStr<char8, OsCharType>("a");
                            case COpenMode::ReadWriteMustExist: return ConvertStr<char8, OsCharType>("r+");
                            case COpenMode::ReadWrite:          return ConvertStr<char8, OsCharType>("w+");
                            case COpenMode::ReadWriteAppend:    return ConvertStr<char8, OsCharType>("a+");
                            default:                                return ConvertStr<char8, OsCharType>("i!");
                        }
                    }

                    static constexpr OpenModeString GetAdditionalModeStr(const FlagPresence REF presenceArray) noexcept
                    {
                        OpenModeString additionalModeStr;

                        if(presenceArray[OpenModeFlag::Text])                       AddStr(additionalModeStr, "t");
                        if(presenceArray[OpenModeFlag::Binary])                     AddStr(additionalModeStr, "b");
                        if(presenceArray[OpenModeFlag::MustNotExist])               AddStr(additionalModeStr, "x");
                        if(presenceArray[OpenModeFlag::CommitDirectlyToDisk])       AddStr(additionalModeStr, "c");
                        if(presenceArray[OpenModeFlag::CommitIndirectlyToDisk])     AddStr(additionalModeStr, "n");
                        if(presenceArray[OpenModeFlag::NotInheritedByChildProcess]) AddStr(additionalModeStr, "N");
                        if(presenceArray[OpenModeFlag::SequntialAccessOptimized])   AddStr(additionalModeStr, "R");
                        if(presenceArray[OpenModeFlag::RandomAccessOptimized])      AddStr(additionalModeStr, "R");
                        if(presenceArray[OpenModeFlag::IfPossibleNoFlushingToDisk]) AddStr(additionalModeStr, "T");
                        if(presenceArray[OpenModeFlag::DeleteAfterClose])           AddStr(additionalModeStr, "D");
                        if(presenceArray[OpenModeFlag::UnicodeEncoding])            AddStr(additionalModeStr, ",css=UNICODE");
                        if(presenceArray[OpenModeFlag::Utf8Encoding])               AddStr(additionalModeStr, ",css=UTF-8");
                        if(presenceArray[OpenModeFlag::Utf16Encoding])              AddStr(additionalModeStr, ",css=UTF-16LE");

                        return additionalModeStr;
                    }

                protected:
                    template<typename ... FlagTypes>
                    static constexpr bool IsFlagPresent(const OpenModeFlag lookingForMode, const FlagTypes ... flags) noexcept
                    {
                        return ThisType::IsFlagPresentInternal<MetaPrograming::Indetifier::Indentify, FlagTypes...>(lookingForMode, flags...);
                    }

                protected:
                    template<MetaPrograming::Indetifier identifier>
                    static constexpr bool IsFlagPresentInternal(const OpenModeFlag) noexcept {return false;}

                    template<MetaPrograming::Indetifier identifier, typename FirstOpneModeType, typename ... FlagTypes>
                    static constexpr bool IsFlagPresentInternal(const OpenModeFlag lookingForMode, const FirstOpneModeType firstOpenMode, const FlagTypes ... flags) noexcept
                    {
                        if(lookingForMode == firstOpenMode)
                            return true;
                        else
                            return ThisType::IsFlagPresentInternal<MetaPrograming::Indetifier::Indentify, FlagTypes...>(lookingForMode, flags...);
                    }

                protected:
                    template<typename CharFrom, typename CharTo>
                    static constexpr auto ConvertStr(const ConstexprStr<OpenModeMaxChars, CharFrom> str) noexcept
                    {
                        ConstexprStr<OpenModeMaxChars, CharTo> returnStr;

                        for(size_t i = 0; i < str.Size(); i++)
                            returnStr += static_cast<CharTo>(str[i]);

                        return returnStr;
                    }

                    static constexpr void AddStr(OpenModeString REF openMode, const char8 * str)
                    {
                        openMode += ConvertStr<char8, OsCharType>(str);
                    }
            };

            template<typename ... FlagTypes>
            static constexpr OpenMode GetOpenMode(FlagTypes ... flags)
            {
                return OpenMode::template GetOpenMode<FlagTypes...>(flags...);
            }

            template<OpenModeSimpleHolder::OpenModeFlag ... openModeArguments>
            static inline constexpr OpenMode GetOpenMode()
            {
                return OpenMode::template GetOpenMode<openModeArguments...>();
            }

    };
}

#endif // OPENMODE_H
