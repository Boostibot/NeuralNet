#ifndef NEWOPENMODE_H
#define NEWOPENMODE_H

#include <string>
#include <string_view>

#include "General/Common/Common.h"

namespace CIo
{
    namespace  OpenModeHelpers
    {
            using OpenModeEnumsBaseType = u8;
            //This is used as the actual argument for OpenMode
            enum class OpenModeFlag : OpenModeEnumsBaseType
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
                //    OpenModeFlagsCount needs to be     //
                //  updated to match the actual number   //
                //             of options                //
                //***************************************//
                //This is to not bother the user with the count option
            };

            static constexpr u32 OpenModeFlagsCount = 17;

            enum class COpenMode : OpenModeEnumsBaseType
            {
                ReadMustExist,      //Read && MustExist
                Write,              //Write
                WriteAppend,        //Write && Append
                ReadWriteMustExist, //Read && Write && MustExist
                ReadWrite,          //Read && Write
                ReadWriteAppend     //Read && Write && Append
            };

            enum class OpenModeValidity : OpenModeEnumsBaseType
            {
                Valid,              //Arguemnts are valid
                Invalid,            //Arguments are invalid. ie. both Read and Write are missing
                Unsupported,        //Arguments that cannot be parsed due to fopen argument limitations
                                    // ie. all combinations containing Append && MustExist
            };

            class FlagPresence
            {
                public:
                    using ThisType = FlagPresence;

                private:
                    bool Presence[OpenModeFlagsCount] = {};

                public:
                    template<typename ... FlagTypes>
                    constexpr FlagPresence(const FlagTypes ... flags) : Presence()
                    {
                        FillFlagPresence<FlagTypes...>(flags...);
                    }

                public:
                    constexpr bool operator[](OpenModeFlag flag) const
                    {
                        return Presence[static_cast<OpenModeEnumsBaseType>(flag)];
                    }

                private:
                    constexpr void SetFlag(OpenModeFlag flag, bool presence)
                    {
                        Presence[static_cast<OpenModeEnumsBaseType>(flag)] = presence;
                    }

                    template<typename ... FlagTypes>
                    constexpr void FillFlagPresence(const FlagTypes ... flags) noexcept
                    {
                        using Underlying = OpenModeEnumsBaseType;

                        for(Underlying i = 0; i < OpenModeFlagsCount; i++)
                        {
                            const OpenModeFlag currFlag = static_cast<OpenModeFlag>(i);
                            this->SetFlag(currFlag, ThisType::IsFlagPresent<FlagTypes...>(currFlag, flags...));
                        }
                    }

                public:
                    template<typename ... FlagTypes>
                    static constexpr bool IsFlagPresent(const OpenModeFlag lookingForFlag, const FlagTypes ... flags) noexcept
                    {
                        return ThisType::IsFlagPresentInternal<MetaPrograming::Indetifier::Indentify, FlagTypes...>(lookingForFlag, flags...);
                    }

                private:
                    template<MetaPrograming::Indetifier identifier>
                    static constexpr bool IsFlagPresentInternal(const OpenModeFlag) noexcept {return false;}

                    template<MetaPrograming::Indetifier identifier, typename FirstOpneModeType, typename ... FlagTypes>
                    static constexpr bool IsFlagPresentInternal(const OpenModeFlag lookingForFlag, const FirstOpneModeType firstOpenMode, const FlagTypes ... flags) noexcept
                    {
                        if(lookingForFlag == firstOpenMode)
                            return true;
                        else
                            return ThisType::IsFlagPresentInternal<MetaPrograming::Indetifier::Indentify, FlagTypes...>(lookingForFlag, flags...);
                    }
            };

            class OpenModeConversionState
            {
                public:
                    using ThisType = OpenModeConversionState;

                public:
                    static constexpr u32 OpenModeMaxChars = 26;
                    using BaseCharType = char8;

                    template<typename CharT>
                    using ConstexprString = MetaPrograming::ConstexprStr<OpenModeMaxChars, CharT>;

                    using OpenModeString = ConstexprString<BaseCharType>;

                    template<typename CharT>
                    using StringView              = std::basic_string_view<CharT>;
                    using BaseStringView          = StringView<BaseCharType>;

                public:
                    const FlagPresence Presence;

                public:
                    template<typename ... FlagTypes>
                    constexpr OpenModeConversionState(const FlagTypes ... flags) : Presence(flags...)
                    {}

                public:
                    constexpr bool AreFlagsConflicting() const noexcept
                    {
                        if(Presence[OpenModeFlag::MustExist] && Presence[OpenModeFlag::MustNotExist])
                            return true;

                        if(Presence[OpenModeFlag::Text] && Presence[OpenModeFlag::Binary])
                            return true;

                        if(Presence[OpenModeFlag::CommitDirectlyToDisk] && Presence[OpenModeFlag::CommitIndirectlyToDisk])
                            return true;

                        if(Presence[OpenModeFlag::SequntialAccessOptimized] && Presence[OpenModeFlag::RandomAccessOptimized])
                            return true;

                        //Encodings
                        if(Presence[OpenModeFlag::UnicodeEncoding] && Presence[OpenModeFlag::Utf8Encoding])
                            return true;

                        if(Presence[OpenModeFlag::Utf16Encoding] && Presence[OpenModeFlag::Utf8Encoding])
                            return true;

                        if(Presence[OpenModeFlag::UnicodeEncoding] && Presence[OpenModeFlag::Utf16Encoding])
                            return true;

                        return false;
                    }

                    constexpr void SetCOpenModeAndValidty(COpenMode REF mode, OpenModeValidity REF validity) const noexcept
                    {
                        const bool read         = Presence[OpenModeFlag::Read];
                        const bool write        = Presence[OpenModeFlag::Write];
                        const bool append       = Presence[OpenModeFlag::Append];
                        const bool mustExist    = Presence[OpenModeFlag::MustExist];

                        auto setValidity = [REF mode, REF validity](OpenModeValidity _validity)
                        {
                            validity = _validity;
                            mode = COpenMode::ReadWrite;
                        };
                        auto setMode = [REF mode, REF validity](COpenMode _mode)
                        {
                            validity = OpenModeValidity::Valid;
                            mode = _mode;
                        };

                        struct Helper
                        {
                                static constexpr u32 Binary(u8 a, u8 b, u8 c, u8 d) {return 8*a + 4*b + 2*c + 1*d;}
                        };

                        switch (Helper::Binary(read, write, append, mustExist))
                        {
                            //                  R  W  A  M
                            case Helper::Binary(0, 0, 0, 0): setValidity(OpenModeValidity::Invalid); break;
                            case Helper::Binary(0, 0, 0, 1): setValidity(OpenModeValidity::Invalid); break;
                            case Helper::Binary(0, 0, 1, 0): setValidity(OpenModeValidity::Invalid); break;
                            case Helper::Binary(0, 0, 1, 1): setValidity(OpenModeValidity::Invalid); break;
                            case Helper::Binary(0, 1, 0, 0): setMode(COpenMode::Write); break;
                            case Helper::Binary(0, 1, 0, 1): setMode(COpenMode::ReadWriteMustExist); break;
                            case Helper::Binary(0, 1, 1, 0): setMode(COpenMode::WriteAppend); break;
                            case Helper::Binary(0, 1, 1, 1): setValidity(OpenModeValidity::Unsupported); break;
                            case Helper::Binary(1, 0, 0, 0): setMode(COpenMode::ReadWrite); break;
                            case Helper::Binary(1, 0, 0, 1): setMode(COpenMode::ReadMustExist); break;
                            case Helper::Binary(1, 0, 1, 0): setMode(COpenMode::ReadWriteAppend); break;
                            case Helper::Binary(1, 0, 1, 1): setValidity(OpenModeValidity::Unsupported); break;
                            case Helper::Binary(1, 1, 0, 0): setMode(COpenMode::ReadWrite); break;
                            case Helper::Binary(1, 1, 0, 1): setMode(COpenMode::ReadWriteMustExist); break;
                            case Helper::Binary(1, 1, 1, 0): setMode(COpenMode::ReadWriteAppend); break;
                            case Helper::Binary(1, 1, 1, 1): setValidity(OpenModeValidity::Unsupported); break;

                            default: setValidity(OpenModeValidity::Invalid); break;
                        }

                    }

                    constexpr static OpenModeString GetCOpenModeStr(const COpenMode mode) noexcept
                    {
                        switch(mode)
                        {
                            case COpenMode::ReadMustExist:      return "r";
                            case COpenMode::Write:              return "w";
                            case COpenMode::WriteAppend:        return "a";
                            case COpenMode::ReadWriteMustExist: return "r+";
                            case COpenMode::ReadWrite:          return "w+";
                            case COpenMode::ReadWriteAppend:    return "a+";
                            default:                            return "i!";
                        }
                    }

                    constexpr OpenModeString GetAdditionalModeStr() const noexcept
                    {
                        OpenModeString additionalModeStr;

                        if(Presence[OpenModeFlag::Text])                       additionalModeStr += "t";
                        if(Presence[OpenModeFlag::Binary])                     additionalModeStr += "b";
                        if(Presence[OpenModeFlag::MustNotExist])               additionalModeStr += "x";
                        if(Presence[OpenModeFlag::CommitDirectlyToDisk])       additionalModeStr += "c";
                        if(Presence[OpenModeFlag::CommitIndirectlyToDisk])     additionalModeStr += "n";
                        if(Presence[OpenModeFlag::NotInheritedByChildProcess]) additionalModeStr += "N";
                        if(Presence[OpenModeFlag::SequntialAccessOptimized])   additionalModeStr += "S";
                        if(Presence[OpenModeFlag::RandomAccessOptimized])      additionalModeStr += "R";
                        if(Presence[OpenModeFlag::IfPossibleNoFlushingToDisk]) additionalModeStr += "T";
                        if(Presence[OpenModeFlag::DeleteAfterClose])           additionalModeStr += "D";
                        if(Presence[OpenModeFlag::UnicodeEncoding])            additionalModeStr += ",css=UNICODE";
                        if(Presence[OpenModeFlag::Utf8Encoding])               additionalModeStr += ",css=UTF-8";
                        if(Presence[OpenModeFlag::Utf16Encoding])              additionalModeStr += ",css=UTF-16LE";

                        return additionalModeStr;
                    }

                    //TODO - put somewhere else
                public:
                    template<size_t strSize, typename CharFrom, typename CharTo>
                    static constexpr auto ConvertString(const MetaPrograming::ConstexprStr<strSize, CharFrom> str) noexcept
                    {
                        static_assert (sizeof (CharFrom) <= sizeof (CharTo), "CharFrom needs to be smaller char size than CharTo");
                        MetaPrograming::ConstexprStr<strSize, CharTo> returnStr;

                        for(size_t i = 0; i < str.Size(); i++)
                            returnStr += static_cast<CharTo>(str[i]);

                        return returnStr;
                    }

                public:
                    template<typename OsCharType>
                    static constexpr auto ConvertToOsString(const ConstexprString<BaseCharType> str) noexcept
                    {
                        return ConvertString<OpenModeMaxChars, BaseCharType, OsCharType>(str);
                    }
            };
    };

    class OpenMode
    {
        public:
            using ThisType = OpenMode;

        protected:
            using OpenModeConversionState = OpenModeHelpers::OpenModeConversionState;
            using OpenModeValidity        = OpenModeHelpers::OpenModeValidity;

        public:
            using OpenModeEnumsBaseType   = OpenModeHelpers::OpenModeEnumsBaseType;
            using OpenModeFlag            = OpenModeHelpers::OpenModeFlag;
            using COpenMode               = OpenModeHelpers::COpenMode;

        public:
            using OpenModeString          = OpenModeConversionState::OpenModeString;
            template<typename CharT>
            using ConstexprString         = OpenModeConversionState::template ConstexprString<CharT>;
            template<typename CharT>
            using StringView              = OpenModeConversionState::template StringView<CharT>;
            using BaseCharType            = OpenModeConversionState::BaseCharType;
            using BaseStringView          = OpenModeConversionState::BaseStringView;

        protected:
            OpenModeString OpenModeStr;
            OpenModeValidity Validity;

        public:
            constexpr OpenMode() noexcept
                : OpenModeStr(), Validity(OpenModeValidity::Invalid)
            {}

            constexpr OpenMode(COpenMode openMode) noexcept
                : OpenModeStr(), Validity()
            {
                AssignCOpenMode(openMode);
            }

        public:
            constexpr OpenMode(const ThisType REF other) = default;
            constexpr OpenMode(ThisType RVALUE_REF other) = default;

            constexpr ThisType REF operator=(const ThisType REF) = default;
            constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;

            template<typename ... FlagTypes,
                     std::enable_if_t<MetaPrograming::AreTypesSameTo<OpenModeFlag, FlagTypes...>::value, u32> = 0>
            constexpr OpenMode(FlagTypes ... flags) noexcept
                : OpenModeStr(), Validity()
            {
                this->operator =(CreateOpenMode<FlagTypes ...>(flags...));
            }

            constexpr void AssignCOpenMode(COpenMode cOpenMode) noexcept
            {
                this->Validity = OpenModeValidity::Valid;
                this->OpenModeStr = OpenModeConversionState::GetCOpenModeStr(cOpenMode);
            }

            template<typename ... FlagTypes>
            static constexpr ThisType CreateOpenMode(FlagTypes ... flags) noexcept
            {
                static_assert (MetaPrograming::AreTypesSameTo<OpenModeFlag, FlagTypes...>::value,
                        "BasicOpenMode::CreateOpenMode : All provided flags must be of type OpenModeFlag");

                OpenModeConversionState state(flags...);
                OpenModeValidity validity = OpenModeValidity::Invalid;
                COpenMode copenMode = COpenMode::ReadWrite;

                if(state.AreFlagsConflicting())
                    return ThisType::OpenModeConstructor(validity);

                state.SetCOpenModeAndValidty(copenMode, validity);

                if(NOT ThisType::IsValid(validity))
                    return ThisType::OpenModeConstructor(validity);

                ThisType openMode;
                openMode.OpenModeStr += state.GetCOpenModeStr(copenMode);
                openMode.OpenModeStr += state.GetAdditionalModeStr();
                openMode.Validity = OpenModeValidity::Valid;
                return openMode;
            }

            template<OpenModeFlag ... openModeArguments,
                     std::enable_if_t<(sizeof...(openModeArguments) > 0), int> = 0>
            static inline constexpr ThisType CreateOpenMode() noexcept
            {
                constexpr ThisType openMode = CreateOpenMode(openModeArguments...);

                static_assert (openMode.IsValid(), "BasicOpenMode::CreateOpenMode : Invalid open mode arguments; See BasicOpenMode::AreOpenModesConflicting(...) for more info");
                static_assert (openMode.IsSupported(), "BasicOpenMode::CreateOpenMode : Unsupported open mode argument combination; "
                                                       "The combination of mode flags is unsupported by the underlying fopen function;"
                                                       "This Is probably cause by having both OpenModeFlag::Append and OpenModeFlag::MustExist flags present");
                return openMode;
            }

            inline constexpr bool IsValid()     const noexcept {return IsValid(this->Validity);}
            inline constexpr bool IsSupported() const noexcept {return IsSupported(this->Validity);}

            template<typename OsCharType = OpenModeString::CharType>
            inline constexpr auto GetOpenModeString() const noexcept
            {
                return OpenModeConversionState::ConvertToOsString<OsCharType>(OpenModeStr);
            }

            template<typename OsCharType>
            inline constexpr operator StringView<OsCharType>() const noexcept
            {
                return GetOpenModeString<OsCharType>().operator std::basic_string_view<OsCharType>();
            }

        protected:
            static constexpr bool IsValid(OpenModeValidity validity)        noexcept {return validity == OpenModeValidity::Valid;}
            static constexpr bool IsSupported(OpenModeValidity validity)    noexcept {return NOT(validity == OpenModeValidity::Unsupported);}

            //Todo - try update
            static constexpr ThisType OpenModeConstructor(OpenModeValidity validity) noexcept
            {
                ThisType openMode;
                openMode.Validity = validity;
                return openMode;
            }
    };

    //Struct holding the OpenMode and the associated functionality
    //This holder is ment to be inherited into other classes using OpenMode to ensure
    // the fucntions are distributed to the correct namespace levels
    // (ie. directly below the base inherited to class       )
    // (so it is possible to write File::CreateOpenMode(...) )
    struct OpenModeHolder
    {
            using OpenMode              = OpenMode;
            //The OpenModeFlag should be directly below the class which inherits this type (File)
            // So it is possible too write File::OpenModeFlag::Write
            // This is a significant improvement over File::OpenMode::OpenModeFlag::Write
            //
            //It is safer to have every enum declared as enum class so there is no automatic
            // degradation to the underlying type. Because of this its necessary to have the
            // OpenModeFlag name present.
            using OpenModeEnumsBaseType = OpenModeHelpers::OpenModeEnumsBaseType;
            //This is used as the actual argument for OpenMode
            using OpenModeFlag          = OpenModeHelpers::OpenModeFlag;
            using COpenMode             = OpenModeHelpers::COpenMode;

            template<typename ... FlagTypes>
            static constexpr OpenMode CreateOpenMode(FlagTypes ... flags)
            {
                return OpenMode::CreateOpenMode<FlagTypes...>(flags...);
            }

            template<OpenModeHelpers::OpenModeFlag ... openModeArguments>
            static inline constexpr OpenMode CreateOpenMode()
            {
                return OpenMode::CreateOpenMode<openModeArguments...>();
            }

    };
}
#endif // NEWOPENMODE_H
