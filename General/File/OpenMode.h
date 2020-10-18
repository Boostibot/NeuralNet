#ifndef OPENMODE_H
#define OPENMODE_H

#include <string>
#include <string_view>

#include "General/Common/Common.h"
#include "OsSupport.h"


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
            protected:
                enum class OpenModeVality : OpenModeFlagUnderlyingType
                {
                    Valid,              //Arguemnts are valid
                    Invalid,            //Arguments are invalid. ie. both Read and Write are missing
                    Unsupported,        //Arguments that cannot be parsed due to fopen argument limitations
                                        // ie. all combinations containing Append && MustExist
                };

            protected:
                static constexpr u32 OpenModeMaxChars = 26;
                static constexpr CFileOpenMode DefaultOpenMode = CFileOpenMode::ReadWrite;
                using OpenModeString = MetaPrograming::ConstexprCStr<OpenModeMaxChars>;

                struct PresenceArray
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

            public:
                using ThisType = OpenMode;

            public:
                using CharType = OsCharTpeArg;
                using StringView = std::basic_string_view<CharType>;

            protected:
                OpenModeString OpenModeStr;
                CFileOpenMode COpenMode;
                OpenModeVality Validity;


            public:
                constexpr OpenMode() noexcept
                    : OpenModeStr(), COpenMode(), Validity()
                {
                    AssignCOpenMode(ThisType::DefaultOpenMode, OpenModeVality::Invalid);
                }

                constexpr OpenMode(CFileOpenMode openMode) noexcept
                    : OpenModeStr(), COpenMode(), Validity()
                {
                    AssignCOpenMode(openMode, OpenModeVality::Valid);
                }

            protected:
                //Is protected because the user should have no direct control over validity
                constexpr OpenMode(OpenModeVality validity) noexcept
                    : OpenModeStr(), COpenMode(), Validity(validity)
                {
                    AssignCOpenMode(ThisType::DefaultOpenMode, validity);
                }

            public:
                constexpr OpenMode(const ThisType REF) = default;
                constexpr OpenMode(ThisType RVALUE_REF) = default;

                constexpr ThisType REF operator=(const ThisType REF) = default;
                constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;

                template<typename ... OpenModeTypes>
                constexpr OpenMode(OpenModeTypes ... openModes) noexcept
                    : OpenModeStr(), COpenMode(), Validity()
                {
                    this->operator =(GetOpenMode<OpenModeTypes ...>(openModes...));
                }

                constexpr void AssignCOpenMode(CFileOpenMode cOpenMode, OpenModeVality validity) noexcept
                {
                    this->COpenMode = cOpenMode;
                    this->Validity = validity;
                    this->OpenModeStr = GetCOpenModeStr(cOpenMode);
                }

                template<typename ... OpenModeTypes>
                static constexpr OpenMode GetOpenMode(OpenModeTypes ... openModes) noexcept
                {
                    static_assert (MetaPrograming::AreTypesSameTo<OpenModeFlag, OpenModeTypes...>::value,
                                   "CFile::OpenMode::GetOpenMode : All provided openModes must be of type OpenModeFlag");

                    PresenceArray modePresence;
                    ThisType::FillOpenModePresenceArray<OpenModeTypes...>(modePresence, openModes...);

                    if(ThisType::AreOpenModesConflicting(modePresence))
                        return OpenModeVality::Invalid;

                    OpenMode openMode = ThisType::GetCOpenMode(modePresence);
                    if(openMode.IsValid() == false)
                        return openMode;

                    OpenMode additionalOpenMode = ThisType::GetAdditionalMode(modePresence);
                    if(additionalOpenMode.IsValid() == false)
                        return additionalOpenMode;

                    openMode.OpenModeStr += additionalOpenMode.OpenModeStr;

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

                constexpr inline bool IsValid() const noexcept
                {
                    return this->Validity == OpenModeVality::Valid;
                }
                constexpr inline bool IsSupported() const noexcept
                {
                    return NOT(this->Validity == OpenModeVality::Unsupported);
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
                template<MetaPrograming::Indetifier identifier>
                static constexpr bool IsOpenModePresentInternal(OpenModeFlag) noexcept {return false;}

                template<MetaPrograming::Indetifier identifier, typename FirstOpneModeType, typename ... OpenModeTypes>
                static constexpr bool IsOpenModePresentInternal(OpenModeFlag lookingForMode, FirstOpneModeType firstOpenMode, OpenModeTypes ... openModes) noexcept
                {
                    if(lookingForMode == firstOpenMode)
                        return true;
                    else
                        return ThisType::IsOpenModePresentInternal<MetaPrograming::Indetifier::Indentify, OpenModeTypes...>(lookingForMode, openModes...);
                }

            protected:
                template<typename ... OpenModeTypes>
                static constexpr bool IsOpenModePresent(OpenModeFlag lookingForMode, OpenModeTypes ... openModes) noexcept
                {
                    return ThisType::IsOpenModePresentInternal<MetaPrograming::Indetifier::Indentify, OpenModeTypes...>(lookingForMode, openModes...);
                }

                template<typename ... OpenModeTypes>
                static constexpr void FillOpenModePresenceArray(PresenceArray REF presenceArray, OpenModeTypes ... openModes) noexcept
                {
                    using Underlying = OpenModeFlagUnderlyingType;
                    for(Underlying i = 0; i < OpenModeOptionsCount; i++)
                    {
                        const OpenModeFlag CurrentMode = static_cast<OpenModeFlag>(i);
                        presenceArray[CurrentMode] = ThisType::IsOpenModePresent<OpenModeTypes...>(CurrentMode, openModes...);
                    }
                }

                static constexpr bool AreOpenModesConflicting(PresenceArray REF modes) noexcept
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

                static constexpr OpenModeString GetCOpenModeStr(CFileOpenMode mode) noexcept
                {
                    switch(mode)
                    {
                        case CFileOpenMode::ReadMustExist:      return "r";
                        case CFileOpenMode::Write:              return "w";
                        case CFileOpenMode::WriteAppend:        return "a";
                        case CFileOpenMode::ReadWriteMustExist: return "r+";
                        case CFileOpenMode::ReadWrite:          return "w+";
                        case CFileOpenMode::ReadWriteAppend:    return "a+";
                        default:                                return "i!";
                    }
                }

                static constexpr OpenMode GetCOpenMode(PresenceArray REF presenceArray) noexcept
                {
                    const bool read         = presenceArray[OpenModeFlag::Read];
                    const bool write        = presenceArray[OpenModeFlag::Write];
                    const bool append       = presenceArray[OpenModeFlag::Append];
                    const bool mustExist    = presenceArray[OpenModeFlag::MustExist];

                    #define BINARY(a, b, c, d) 8*a + 4*b + 2*c + 1*d

                    switch (BINARY(read, write, append, mustExist))
                    {
                        //          R  W  A  M
                        case BINARY(0, 0, 0, 0): return OpenModeVality::Invalid;
                        case BINARY(0, 0, 0, 1): return OpenModeVality::Invalid;
                        case BINARY(0, 0, 1, 0): return OpenModeVality::Invalid;
                        case BINARY(0, 0, 1, 1): return OpenModeVality::Invalid;
                        case BINARY(0, 1, 0, 0): return CFileOpenMode::Write;
                        case BINARY(0, 1, 0, 1): return CFileOpenMode::ReadWriteMustExist;
                        case BINARY(0, 1, 1, 0): return CFileOpenMode::WriteAppend;
                        case BINARY(0, 1, 1, 1): return OpenModeVality::Unsupported;
                        case BINARY(1, 0, 0, 0): return CFileOpenMode::ReadWrite;
                        case BINARY(1, 0, 0, 1): return CFileOpenMode::ReadMustExist;
                        case BINARY(1, 0, 1, 0): return CFileOpenMode::ReadWriteAppend;
                        case BINARY(1, 0, 1, 1): return OpenModeVality::Unsupported;
                        case BINARY(1, 1, 0, 0): return CFileOpenMode::ReadWrite;
                        case BINARY(1, 1, 0, 1): return CFileOpenMode::ReadWriteMustExist;
                        case BINARY(1, 1, 1, 0): return CFileOpenMode::ReadWriteAppend;
                        case BINARY(1, 1, 1, 1): return OpenModeVality::Unsupported;

                        default: return OpenModeVality::Invalid;
                    }

                    #undef BINARY
                }

                static constexpr OpenMode GetAdditionalMode(PresenceArray REF presenceArray) noexcept
                {
                    OpenMode additionalMode;
                    //For some reason the constructor declaring validty cannot be used here
                    //"Cannot acccess protected constrcor of class"
                    additionalMode.Validity = OpenModeVality::Valid;
                    if(presenceArray[OpenModeFlag::Text])                       additionalMode.OpenModeStr += "t";
                    if(presenceArray[OpenModeFlag::Binary])                     additionalMode.OpenModeStr += "b";
                    if(presenceArray[OpenModeFlag::MustNotExist])               additionalMode.OpenModeStr += "x";
                    if(presenceArray[OpenModeFlag::CommitDirectlyToDisk])       additionalMode.OpenModeStr += "c";
                    if(presenceArray[OpenModeFlag::CommitIndirectlyToDisk])     additionalMode.OpenModeStr += "n";
                    if(presenceArray[OpenModeFlag::NotInheritedByChildProcess]) additionalMode.OpenModeStr += "N";
                    if(presenceArray[OpenModeFlag::SequntialAccessOptimized])   additionalMode.OpenModeStr += "R";
                    if(presenceArray[OpenModeFlag::RandomAccessOptimized])      additionalMode.OpenModeStr += "R";
                    if(presenceArray[OpenModeFlag::IfPossibleNoFlushingToDisk]) additionalMode.OpenModeStr += "T";
                    if(presenceArray[OpenModeFlag::DeleteAfterClose])           additionalMode.OpenModeStr += "D";
                    if(presenceArray[OpenModeFlag::UnicodeEncoding])            additionalMode.OpenModeStr += ",css=UNICODE";
                    if(presenceArray[OpenModeFlag::Utf8Encoding])               additionalMode.OpenModeStr += ",css=UTF-8";
                    if(presenceArray[OpenModeFlag::Utf16Encoding])              additionalMode.OpenModeStr += ",css=UTF-16LE";
                    return additionalMode;
                }

        };

        template<typename ... OpenModeTypes>
        static constexpr OpenMode GetOpenMode(OpenModeTypes ... openModes)
        {
            return OpenMode::template GetOpenMode<OpenModeTypes...>(openModes...);
        }

        template<OpenModeFlag ... openModeArguments>
        static inline constexpr OpenMode GetOpenMode()
        {
            return OpenMode::template GetOpenMode<openModeArguments...>();
        }

};

#endif // OPENMODE_H
