#ifndef NEWOPENMODE_H
#define NEWOPENMODE_H

#include "ConstexprString.h"

namespace CIo
{
    namespace  OpenModeHelpers
    {
        using OpenModeEnumsBaseType = u8;
    }

    //These are all of the open mode flags necessary to represent all
    // c standard compilant open mode strings
    enum class OpenModeFlag : OpenModeHelpers::OpenModeEnumsBaseType
    {
        Read,
        Write,
        Append,
        MustExist,

        Binary,
        MustNotExist


        //***************************************//
        //    OpenModeFlagsCount needs to be     //
        //  updated to match the actual number   //
        //             of options                //
        //***************************************//
        //This is to not bother the user with the count option
    };
    //These are all of the open mode flags necessary to represent all
    // visual c++ additional open mode symbols
    enum class WindowsOpenModeFlag : OpenModeHelpers::OpenModeEnumsBaseType
    {
        Text,

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
        // WindowsOpenModeFlagsCount needs to be //
        //  updated to match the actual number   //
        //             of options                //
        //***************************************//
        //This is to not bother the user with the count option
    };

    namespace  OpenModeHelpers
    {
            static constexpr OpenModeEnumsBaseType OpenModeFlagsCount = 6;
            static constexpr OpenModeEnumsBaseType WindowsOpenModeFlagsCount = 11;

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

            template <typename FlagType>
            constexpr bool IsTypeOpenModeFlag()
            {
                return std::is_same_v<FlagType, OpenModeFlag> || std::is_same_v<FlagType, WindowsOpenModeFlag>;
            }

            template <typename ... FlagTypes>
            constexpr bool AreOpenModeFlags()
            {
                return MetaPrograming::And(IsTypeOpenModeFlag<FlagTypes>()...);
            }

            class FlagPresence
            {
                public:
                    using ThisType = FlagPresence;
                    using Underlying = OpenModeEnumsBaseType;

                public:
                    static constexpr Underlying WindowsFlagsOffset = OpenModeFlagsCount;
                    static constexpr Underlying TotalFlagsCount = OpenModeFlagsCount + WindowsOpenModeFlagsCount;

                private:
                    bool Presence[TotalFlagsCount] = {};

                public:
                    template<typename ... FlagTypes,
                             std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<FlagTypes...>(), u32> = 0>
                    constexpr FlagPresence(const FlagTypes ... flags) : Presence()
                    {
                        FillFlagPresence<FlagTypes...>(flags...);
                    }

                public:
                    constexpr bool operator[](OpenModeFlag flag) const
                    {
                        return Presence[ThisType::ToIndex(flag)];
                    }
                    constexpr bool operator[](WindowsOpenModeFlag flag) const
                    {
                        return Presence[ThisType::ToIndex(flag)];
                    }

                private:
                    static constexpr Underlying ToIndex(OpenModeFlag flag) noexcept
                    {
                        return static_cast<Underlying>(flag);
                    }
                    static constexpr Underlying ToIndex(WindowsOpenModeFlag flag) noexcept
                    {
                        return static_cast<Underlying>(flag) + WindowsFlagsOffset;
                    }

                private:
                    template<typename ... FlagTypes>
                    constexpr void FillFlagPresence(const FlagTypes ... flags) noexcept
                    {
                        for(Underlying i = 0; i < TotalFlagsCount; i++)
                            Presence[i] = ThisType::IsFlagPresentInternal<MetaPrograming::Indetifier::Indentify, FlagTypes...>(i, flags...);
                    }

                public:
                    template<typename ... FlagTypes>
                    static constexpr bool IsFlagPresent(const OpenModeFlag lookingForFlag, const FlagTypes ... flags) noexcept
                    {
                        return ThisType::IsFlagPresentInternal<MetaPrograming::Indetifier::Indentify, FlagTypes...>(ThisType::ToIndex(lookingForFlag), flags...);
                    }
                    template<typename ... FlagTypes>
                    static constexpr bool IsFlagPresent(const WindowsOpenModeFlag lookingForFlag, const FlagTypes ... flags) noexcept
                    {
                        return ThisType::IsFlagPresentInternal<MetaPrograming::Indetifier::Indentify, FlagTypes...>(ThisType::ToIndex(lookingForFlag), flags...);
                    }

                private:
                    template<MetaPrograming::Indetifier identifier>
                    static constexpr bool IsFlagPresentInternal(const Underlying) noexcept {return false;}

                    template<MetaPrograming::Indetifier identifier, typename FirstOpneModeType, typename ... FlagTypes>
                    static constexpr bool IsFlagPresentInternal(const Underlying lookingForFlag, const FirstOpneModeType firstOpenMode, const FlagTypes ... flags) noexcept
                    {
                        const Underlying firstModeIndex = ThisType::ToIndex(firstOpenMode);

                        if(lookingForFlag == firstModeIndex)
                            return true;
                        else
                            return ThisType::IsFlagPresentInternal<identifier, FlagTypes...>(lookingForFlag, flags...);
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
                    using ConstexprString = ConstexprString<CharT, OpenModeMaxChars>;
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

                        if(Presence[WindowsOpenModeFlag::Text] && Presence[OpenModeFlag::Binary])
                            return true;

                        if(Presence[WindowsOpenModeFlag::CommitDirectlyToDisk] && Presence[WindowsOpenModeFlag::CommitIndirectlyToDisk])
                            return true;

                        if(Presence[WindowsOpenModeFlag::SequntialAccessOptimized] && Presence[WindowsOpenModeFlag::RandomAccessOptimized])
                            return true;

                        //Encodings
                        if(Presence[WindowsOpenModeFlag::UnicodeEncoding] && Presence[WindowsOpenModeFlag::Utf8Encoding])
                            return true;

                        if(Presence[WindowsOpenModeFlag::Utf16Encoding] && Presence[WindowsOpenModeFlag::Utf8Encoding])
                            return true;

                        if(Presence[WindowsOpenModeFlag::UnicodeEncoding] && Presence[WindowsOpenModeFlag::Utf16Encoding])
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

                        if(Presence[OpenModeFlag::Binary])                            additionalModeStr += "b";
                        if(Presence[WindowsOpenModeFlag::Text])                       additionalModeStr += "t"; //Text needs to be before MustNotExist
                        if(Presence[OpenModeFlag::MustNotExist])                      additionalModeStr += "x";
                        if(Presence[WindowsOpenModeFlag::CommitDirectlyToDisk])       additionalModeStr += "c";
                        if(Presence[WindowsOpenModeFlag::CommitIndirectlyToDisk])     additionalModeStr += "n";
                        if(Presence[WindowsOpenModeFlag::NotInheritedByChildProcess]) additionalModeStr += "N";
                        if(Presence[WindowsOpenModeFlag::SequntialAccessOptimized])   additionalModeStr += "S";
                        if(Presence[WindowsOpenModeFlag::RandomAccessOptimized])      additionalModeStr += "R";
                        if(Presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk]) additionalModeStr += "T";
                        if(Presence[WindowsOpenModeFlag::DeleteAfterClose])           additionalModeStr += "D";
                        if(Presence[WindowsOpenModeFlag::UnicodeEncoding])            additionalModeStr += ",css=UNICODE";
                        if(Presence[WindowsOpenModeFlag::Utf8Encoding])               additionalModeStr += ",css=UTF-8";
                        if(Presence[WindowsOpenModeFlag::Utf16Encoding])              additionalModeStr += ",css=UTF-16LE";

                        return additionalModeStr;
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
            using OpenModeFlag            = OpenModeFlag;
            using WindowsOpenModeFlag     = OpenModeFlag;
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
            COpenMode CMode;
            OpenModeValidity Validity;

        public:
            constexpr OpenMode() noexcept
                : OpenModeStr(), CMode(), Validity(OpenModeValidity::Invalid)
            {}

            constexpr OpenMode(COpenMode openMode) noexcept
                : OpenModeStr(), CMode(), Validity()
            {
                AssignCOpenMode(openMode);
            }

        public:
            constexpr OpenMode(const ThisType REF other) = default;
            constexpr OpenMode(ThisType RVALUE_REF other) = default;

            constexpr ThisType REF operator=(const ThisType REF) = default;
            constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;

            template<typename ... FlagTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<FlagTypes...>(), u32> = 0>
            constexpr OpenMode(FlagTypes ... flags) noexcept
                : OpenModeStr(), CMode(), Validity()
            {
                this->operator =(CreateOpenMode<FlagTypes ...>(flags...));
            }

            constexpr void AssignCOpenMode(COpenMode cOpenMode) noexcept
            {
                this->CMode = cOpenMode;
                this->Validity = OpenModeValidity::Valid;
                this->OpenModeStr = OpenModeConversionState::GetCOpenModeStr(cOpenMode);
            }

            template<typename ... FlagTypes>
            static constexpr ThisType CreateOpenMode(FlagTypes ... flags) noexcept
            {
                static_assert (OpenModeHelpers::AreOpenModeFlags<FlagTypes...>(),
                        "BasicOpenMode::CreateOpenMode : All provided flags must be of type OpenModeFlag");

                OpenModeConversionState state(flags...);
                ThisType openMode = OpenModeConstructor(OpenModeValidity::Invalid, COpenMode::ReadWrite);

                if(state.AreFlagsConflicting())
                    return openMode;

                state.SetCOpenModeAndValidty(openMode.CMode, openMode.Validity);

                if(NOT openMode.IsValid())
                    return openMode;

                openMode.OpenModeStr += state.GetCOpenModeStr(openMode.CMode);
                openMode.OpenModeStr += state.GetAdditionalModeStr();
                return openMode;
            }

            /*
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
            */

            inline constexpr bool IsValid()     const noexcept {return IsValid(this->Validity);}
            inline constexpr bool IsSupported() const noexcept {return IsSupported(this->Validity);}

            template<typename OsCharType = OpenModeString::CharType>
            inline constexpr auto GetOpenModeString() const noexcept
            {
                if(IsValid())
                    return PromoteStringCharsTo<OsCharType>(OpenModeStr);
                else
                    return ConstexprString<OsCharType>();
            }
            inline constexpr auto GetCOpenMode() const noexcept
            {
                return this->CMode;
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
            static constexpr ThisType OpenModeConstructor(OpenModeValidity validity, COpenMode cmode) noexcept
            {
                ThisType openMode;
                openMode.Validity = validity;
                openMode.CMode = cmode;
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
            using OpenModeFlag          = OpenModeFlag;
            using WindowsOpenModeFlag   = WindowsOpenModeFlag;
            using COpenMode             = OpenModeHelpers::COpenMode;

            //The OpenModeFlag should be directly below the class which inherits this type (File)
            // So it is possible too write File::OpenModeFlag::Write
            // This is a significant improvement over File::OpenMode::OpenModeFlag::Write
            //
            //It is safer to have every enum declared as enum class so there is no automatic
            // degradation to the underlying type. Because of this its necessary to have the
            // OpenModeFlag name present.

            template<typename ... FlagTypes>
            static constexpr OpenMode CreateOpenMode(FlagTypes ... flags)
            {
                return OpenMode::CreateOpenMode<FlagTypes...>(flags...);
            }

            //Compile time evaluation isnt now possible due to there being two types of arguments
            //template<OpenModeFlag ... openModeArguments>
            //static inline constexpr OpenMode CreateOpenMode()
            //{
            //    return OpenMode::CreateOpenMode<openModeArguments...>();
            //}

    };
}
#endif // NEWOPENMODE_H
