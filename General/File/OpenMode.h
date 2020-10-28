#ifndef OPENMODE_H
#define OPENMODE_H

#include <string>
#include <string_view>

#include "General/Common/Common.h"


//Struct holding the OpenMode and the associated functionality
//Is in this struct to separate the declarations into multiple "namespace" structs
//This holder is ment to be inherited into other classes using OpenMode to ensure
// the fucntions are distributed to the correct namespace levels

template<typename OsCharTpeArg>
struct OpenModeHolder
{
        //The OpenModeFlag should be directly below the class which inherits this type (File)
        // So it is possible too write File::OpenModeFlag::Write
        // This is a significant improvement over File::OpenMode::OpenModeFlag::Write
        //
        //It is safer to have every struct declared as enum class so there is no automatic
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
        static constexpr int OpenModeOptionsCount = 17;

    public:
        enum class CFileOpenMode : OpenModeFlagUnderlyingType
        {
            ReadMustExist,      //Read && MustExist
            Write,              //Write
            WriteAppend,        //Write && Append
            ReadWriteMustExist, //Read && Write && MustExist
            ReadWrite,          //Read && Write
            ReadWriteAppend     //Read && Write && Append
        };

    public:
        struct OpenMode
        {
            public:
                using ThisType = OpenMode;
                using CharType = OsCharTpeArg;
                using StringView = std::basic_string_view<CharType>;

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
                        CFileOpenMode OpenMode = {};

                        constexpr COpenModeWithValidty(CFileOpenMode mode, OpenModeValidity val = OpenModeValidity::Valid) : Validity(val), OpenMode(mode) {}
                        constexpr COpenModeWithValidty(OpenModeValidity val) : Validity(val), OpenMode(CFileOpenMode::ReadWrite) {}
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
                static constexpr CFileOpenMode DefaultCOpenMode = CFileOpenMode::ReadWrite;
                static constexpr OpenModeValidity DefaultValidity = OpenModeValidity::Invalid;
                using OpenModeString = MetaPrograming::ConstexprCStr<OpenModeMaxChars, CharType>;

            protected:
                OpenModeString OpenModeStr;
                CFileOpenMode COpenMode;
                OpenModeValidity Validity;

            public:
                constexpr OpenMode() noexcept
                    : OpenModeStr(), COpenMode(ThisType::DefaultCOpenMode), Validity(ThisType::DefaultValidity)
                {}

                constexpr OpenMode(CFileOpenMode openMode) noexcept
                    : OpenModeStr(), COpenMode(), Validity()
                {
                    AssignCOpenMode(openMode);
                }

            public:
                constexpr OpenMode(const ThisType REF other) : OpenModeStr(other.OpenModeStr), COpenMode(other.COpenMode), Validity(other.Validity) {};
                constexpr OpenMode(ThisType RVALUE_REF other) : OpenModeStr(other.OpenModeStr), COpenMode(other.COpenMode), Validity(other.Validity) {};

                constexpr ThisType REF operator=(const ThisType REF) = default;
                constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;

                template<typename ... FlagTypes>
                constexpr OpenMode(FlagTypes ... flags) noexcept
                    : OpenModeStr(), COpenMode(), Validity()
                {
                    this->operator =(GetOpenMode<FlagTypes ...>(flags...));
                }

                constexpr void AssignCOpenMode(CFileOpenMode cOpenMode) noexcept
                {
                    this->COpenMode = cOpenMode;
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

                    openMode.OpenModeStr = ThisType::GetCOpenModeStr(openMode.COpenMode);
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

                inline constexpr StringView GetStr() const noexcept
                {
                    return StringView(this->OpenModeStr.String, this->OpenModeStr.Size);
                }
                inline constexpr operator StringView() const noexcept
                {
                    return this->GetStr();
                }

                //Helpers

            protected:
                //For some reason if this is implemented as constructor it breaks (only when compiled with MSVC)
                //Is protected because the user should have no direct control over validity
                //constexpr OpenMode(OpenModeValidity validity, CFileOpenMode openMode = ThisType::DefaultCOpenMode) noexcept
                //    : OpenModeStr(), COpenMode(openMode), Validity(validity)
                //{}

                //constexpr OpenMode(COpenModeWithValidty pair) noexcept
                //    : OpenModeStr(), COpenMode(pair.OpenMode), Validity(pair.Validity)
                //{}

                static constexpr OpenMode OpenModeConstructor(OpenModeValidity validity, CFileOpenMode copenMode = ThisType::DefaultCOpenMode) noexcept
                {
                    OpenMode openMode;
                    openMode.Validity = validity;
                    openMode.COpenMode = copenMode;
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
                        case BINARY(0, 1, 0, 0): return CFileOpenMode::Write;
                        case BINARY(0, 1, 0, 1): return CFileOpenMode::ReadWriteMustExist;
                        case BINARY(0, 1, 1, 0): return CFileOpenMode::WriteAppend;
                        case BINARY(0, 1, 1, 1): return OpenModeValidity::Unsupported;
                        case BINARY(1, 0, 0, 0): return CFileOpenMode::ReadWrite;
                        case BINARY(1, 0, 0, 1): return CFileOpenMode::ReadMustExist;
                        case BINARY(1, 0, 1, 0): return CFileOpenMode::ReadWriteAppend;
                        case BINARY(1, 0, 1, 1): return OpenModeValidity::Unsupported;
                        case BINARY(1, 1, 0, 0): return CFileOpenMode::ReadWrite;
                        case BINARY(1, 1, 0, 1): return CFileOpenMode::ReadWriteMustExist;
                        case BINARY(1, 1, 1, 0): return CFileOpenMode::ReadWriteAppend;
                        case BINARY(1, 1, 1, 1): return OpenModeValidity::Unsupported;

                        default: return OpenModeValidity::Invalid;
                    }

                    #undef BINARY
                }

                static constexpr OpenModeString GetCOpenModeStr(const CFileOpenMode mode) noexcept
                {
                    switch(mode)
                    {
                        case CFileOpenMode::ReadMustExist:      return ConvertStr<char8, CharType>("r");
                        case CFileOpenMode::Write:              return ConvertStr<char8, CharType>("w");
                        case CFileOpenMode::WriteAppend:        return ConvertStr<char8, CharType>("a");
                        case CFileOpenMode::ReadWriteMustExist: return ConvertStr<char8, CharType>("r+");
                        case CFileOpenMode::ReadWrite:          return ConvertStr<char8, CharType>("w+");
                        case CFileOpenMode::ReadWriteAppend:    return ConvertStr<char8, CharType>("a+");
                        default:                                return ConvertStr<char8, CharType>("i!");
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

                template<typename CharFrom, typename CharTo>
                static constexpr auto ConvertStr(const MetaPrograming::ConstexprCStr<OpenModeMaxChars, CharFrom> str) noexcept
                {
                    MetaPrograming::ConstexprCStr<OpenModeMaxChars, CharTo> returnStr;
                    returnStr.Size = str.Size;

                    for(size_t i = 0; i < str.Size; i++)
                        returnStr[i] = static_cast<CharTo>(str[i]);

                    return returnStr;
                }

                static constexpr void AddStr(OpenModeString REF openMode, const char8 * str)
                {
                    openMode += ConvertStr<char8, CharType>(str);
                }
        };

        template<typename ... FlagTypes>
        static constexpr OpenMode GetOpenMode(FlagTypes ... flags)
        {
            return OpenMode::template GetOpenMode<FlagTypes...>(flags...);
        }

        template<OpenModeFlag ... openModeArguments>
        static inline constexpr OpenMode GetOpenMode()
        {
            return OpenMode::template GetOpenMode<openModeArguments...>();
        }

};

#endif // OPENMODE_H
