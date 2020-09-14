#ifndef DEFAULTLOGGER_H
#define DEFAULTLOGGER_H

#include <iostream>
#include <fstream>
#include <chrono>

#include "DefaultDataInterpret.h"

namespace StaticLog
{
    namespace Internal
    {
        struct AlwaysReadyFile
        {
            public:
                using FileType = std::fstream;
                FileType File;
                //static constexpr size_t BufferSize = 1023; //1 smaller than the basic_filebuf 1024 Byte limit
                //char Buffer[BufferSize];
                //size_t BufferUsage = 0;

            public:
                inline AlwaysReadyFile(const StringViewType filePath)
                {
                    this->OpenFile(filePath);
                }

                inline AlwaysReadyFile(AlwaysReadyFile PASS_RVALUE_REF other) :
                    File(std::move(other.File))
                {
                    //Opens a new file to not leave the other in an invalid state
                    other.OpenFile("TempFile.txt");
                }

                inline AlwaysReadyFile REF operator=(AlwaysReadyFile PASS_RVALUE_REF other)
                {
                    this->File.swap(other.File);
                    return POINTER_VALUE(this);
                }

                AlwaysReadyFile(AlwaysReadyFile PASS_REF) = delete;
                AlwaysReadyFile REF operator=(AlwaysReadyFile PASS_REF) = delete;

            public:
                inline void OpenFile(const StringViewType filePath)
                {
                    File.open(filePath.data(), FileType::binary | FileType::trunc | FileType::out);

                    if(NOT(File.is_open()))
                        throw std::runtime_error("StaticLog::AlwaysReadyFile: File couldnt be opened");
                }
                inline void Print(const char* data, size_t size)
                {
                    File.write(data, size);
                    /*
                    BufferUsage += size;

                    if(BufferUsage > BufferSize)
                    {
                        File.write(Buffer, BufferUsage - size);
                        BufferUsage = size;
                    }

                    memcpy(Buffer, data, size);
                    */
                }
        };
    }

    //The default logger interpret
    template <typename DataInterpretType = DefaultDataInterpret>
    struct DefaultLoggerPackage
    {

        public:
            DefaultLoggerPackage(const StringViewType filePath) : File(filePath) {this->SetUp();}
            DefaultLoggerPackage(DefaultLoggerPackage PASS_RVALUE_REF other) = default;
            DefaultLoggerPackage REF operator=(DefaultLoggerPackage PASS_RVALUE_REF other) = default;

            DefaultLoggerPackage(const DefaultLoggerPackage PASS_REF) = delete;
            void operator=(const DefaultLoggerPackage PASS_REF) = delete ;

        protected:
            DataInterpretType Interpret;

        protected:
            fmt::format_int Formater;
            Internal::AlwaysReadyFile File;
            StringType CollectionString;
            StringType TempString;

            StringType Tags;

            u32 EntryIndex = 0;
            bool AppOutputLogStatus = true;

        protected:
            void SetUp()
            {
                this->ReserveStrings();
            }
            void ReserveStrings()
            {
                CollectionString.reserve(MinimumCollectionStringSize);
                TempString.reserve(MinimumTempStringSize);
                Tags.reserve(MinimumTagsSize);
            }

        protected:
            static constexpr u32 MinimumCollectionStringSize = 512;
            static constexpr u32 MinimumTempStringSize = 128;
            static constexpr u32 MinimumTagsSize = 128;

    };


    template<typename DataInterpretTypeArg = DefaultDataInterpret,
             bool StaticEnableAppOutputLogArg = true>
    class DefaultLogger : public LoggerInterface<DefaultLogger<DataInterpretTypeArg, StaticEnableAppOutputLogArg>,
                                                 DefaultLoggerPackage<DataInterpretTypeArg>
                                                >
    {
        public:
            using DataInterpretType = DataInterpretTypeArg;
            using ThisPackageType = DefaultLoggerPackage<DataInterpretType>;
            using ThisType = DefaultLogger<DataInterpretType, StaticEnableAppOutputLogArg>;
            using ThisLoggerInterfaceType = LoggerInterface<ThisType, ThisPackageType>;
            using StringType = ::std::string;
            using StringViewType = ::std::string_view;
            friend ThisLoggerInterfaceType; //Is needed in order for LoggerInterface to access the protected necesarry Overloaded functions

        protected:
            static constexpr u32 LogIterationFieldSize = 8;
            static constexpr u32 LogClockFieldSize = 8;
            static constexpr char Newline = '\n';
            static constexpr char NameIndicator = '\'';
            static constexpr char ValueIndicator = '"';
            static constexpr char Separator = ' ';
            static constexpr const char ListSeparator[] = ", ";
            static constexpr bool StaticEnableAppOutputLog = StaticEnableAppOutputLogArg;


            struct StringLiteralArray
            {
                    //Crashes debugger when part of the class
                    //So its necessary to put it into unralated class
                    static constexpr char LevelMarkers[ThisType::LevelCount][8] {{"<00> : "}, {"<01> : "}, {"<02> : "}, {"<03> : "}, {"<04> : "}, {"<05> : "}, {"<06> : "}, {"<07> : "},
                                                                                 {"<08> : "}, {"<09> : "}, {"<10> : "}, {"<11> : "}, {"<12> : "}, {"<13> : "}, {"<14> : "}, {"<15> : "}};
            };


            //The obligatory function overloads
        protected:
            template<typename ... MsgTypes>
            void LogMsgsOverload(const u32 level, MsgTypes RVALUE_REF ... msgs)
            {
                this->AddIterations();
                this->AddTime();
                this->AddLvl(level);
                this->AddMsgs(std::forward<MsgTypes>(msgs)...);
                this->AddTags();

                this->PushLogOut();
            }
            template<typename ... NameAndValueTypes>
            void LogVarsOverload(const u32 level, NameAndValueTypes RVALUE_REF ... namesAndValues)
            {
                this->AddIterations();
                this->AddTime();
                this->AddLvl(level);
                this->AddVars(std::forward<NameAndValueTypes>(namesAndValues)...);
                this->AddTags();

                this->PushLogOut();
            }
            template<typename ... MsgTypes>
            void LogMsgsSourceOverload(const StringViewType file, const u32 lineNum, const u32 level, MsgTypes RVALUE_REF ... msgs)
            {
                this->AddSource(file, lineNum);
                this->AddIterations();
                this->AddTime();
                this->AddLvl(level);
                this->AddMsgs(std::forward<MsgTypes>(msgs)...);
                this->AddTags();

                this->PushLogOut();
            }
            template<typename ... NameAndValueTypes>
            void LogVarsSourceOverload(const StringViewType file, const u32 lineNum, const u32 level, NameAndValueTypes RVALUE_REF ... namesAndValues)
            {
                this->AddSource(file, lineNum);
                this->AddIterations();
                this->AddTime();
                this->AddLvl(level);
                this->AddVars(std::forward<NameAndValueTypes>(namesAndValues)...);
                this->AddTags();

                this->PushLogOut();
            }

            template<typename ... TagTypes>
            void AppendTagsOverload(TagTypes RVALUE_REF ... tags)
            {
                this->UnravelAndAddTags(std::forward<TagTypes>(tags)...);
            }

        public:
            DefaultLogger() = delete;
            DefaultLogger(const ThisType PASS_REF other)= delete;
            DefaultLogger(ThisType PASS_RVALUE_REF other) : ThisLoggerInterfaceType(std::move(static_cast<ThisLoggerInterfaceType &&>(other))) {this->LogConstructionMessage();}

            //Any other constructor for the DerivedDataPackage
            template<typename ... ArgumentTypes ,
                     //Checks assure that the ArgumentTypes are not identical to the ones of copy and move constructor
                     //This is to resolve the ambiguous function call
                     //Checks if any of the types arent same as the contructor types but only blocks the function if the type is alone (sizeof...(ArgumentTypes) == 1)
                     std::enable_if_t<!(MetaPrograming::IsTypePresent<ThisType PASS_REF, ArgumentTypes...>::value && (sizeof...(ArgumentTypes) == 1)), int> = 0
                     >
            DefaultLogger(ArgumentTypes PASS_RVALUE_REF ... args) : ThisLoggerInterfaceType(std::forward<ArgumentTypes>(args)...) {this->LogConstructionMessage();}
            ~DefaultLogger() {this->LogDestructionMessage();}

            DefaultLogger REF operator=(const DefaultLogger PASS_REF other) = delete;
            DefaultLogger REF operator=(DefaultLogger PASS_RVALUE_REF) = default;

        protected:
            void AddSource(const StringViewType file, const u32 lineNum)
            {
                AddFormatedSource(file, lineNum, this->Formater, this->CollectionString);
                //AddSeparator(this->CollectionString); //Merged within the AddFormatedTime for performance
            }

            void AddIterations()
            {
                AddFormatedIterationCount(this->EntryIndex, this->Formater, this->CollectionString);
                AddSeparator(this->CollectionString);
            }

            void AddTime()
            {
                AddFormatedTime(this->Formater, clock(), this->CollectionString);
                //AddSeparator(this->CollectionString); //Merged within the AddFormatedTime for performance
            }

            void AddLvl(const u32 level)
            {
                AddFormatedLevel(level, this->CollectionString);
                //AddSeparator(this->CollectionString); //Merged within the AddFormatedLevel for performance
            }

            template<typename ... MsgTypes>
            void AddMsgs(MsgTypes RVALUE_REF ... msgs)
            {
                AddFormatedMsgHeader(this->CollectionString);
                UnravelAndAddMsgs(std::forward<MsgTypes>(msgs)...);
                AddSeparator(this->CollectionString);
            }

            template<typename FirstStringType, typename FirstValueType,
                     typename ... NameAndValueTypes>
            inline void AddVars(FirstStringType RVALUE_REF firstString, FirstValueType RVALUE_REF firstValue,
                                                NameAndValueTypes RVALUE_REF ... namesAndValues)
            {
                static_assert (AreEven<NameAndValueTypes...>(), "DefaultLogger: AppendVars requires even number of arguments");

                this->AddFormatedVariableAsListPart<FirstValueType>(
                            std::forward<FirstStringType>(firstString),
                            std::forward<FirstValueType>(firstValue), this->Interpret, this->TempString, this->CollectionString);

                this->UnravelAndAddVariables(std::forward<NameAndValueTypes>(namesAndValues)...);

                this->AddSeparator(this->CollectionString);
            }

            void AddTags()
            {
                //Adds some speed both in debug and release mode
                if(NOT(this->Tags.empty()))
                {
                    this->CollectionString.append(this->Tags);
                    //this->AddSeparator(this->CollectionString); //No need for this since tags are always at the end (in the current use)
                }
            }

        protected:
            template<u32 level = ThisType::MaxLevelIndex>
            void LogConstructionMessage()
            {
                if(NOT(this->template IsLogEnabled<level>()))
                    return;

                time_t rawTime;
                time(&rawTime);
                StringType date;
                date.reserve(40);

                this->TempString.clear();
                this->AddFormatedDate(rawTime, date);

                this->AppendTagsOverload("log_construction", "log_starting");
                this->LogMsgsOverload(level, date.data(), "Log Starting................");
            }
            template<u32 level = ThisType::MaxLevelIndex>
            void LogDestructionMessage()
            {
                if(NOT(this->template IsLogEnabled<level>()))
                    return;

                time_t rawTime;
                time(&rawTime);
                StringType date;
                date.reserve(40);

                this->TempString.clear();
                this->AddFormatedDate(rawTime, date);

                this->AppendTagsOverload("log_destruction", "log_ending");
                this->LogMsgsOverload(level, date, "Log Ending..................");
            }


        public:
            template<u32 level>
            void LogLoggerInfo()
            {
                if(NOT(this->template IsLogEnabled<level>()))
                    return;

                this->AppendTagsOverload("log_info");
                this->LogMsgsOverload(level, "Logger info : Default log writer; Displaying in order: 1-Code source 2-iteration count 3-Date and time 4-Level 5-Messages 6-variables 7-tags");
            }

        public:
            inline void DoAppOutputLog(const bool doLog) noexcept
            {
                if constexpr(ThisType::StaticEnableAppOutputLog)
                        this->AppOutputLogStatus = doLog;
                else
                    return;

            }
            inline void EnableAppOutputLog() noexcept {this->DoAppOutputLog(true);}
            inline void DisableAppOutputLog() noexcept {this->DoAppOutputLog(false);}

            inline bool IsAppOutputLogEnabled() const noexcept
            {
                //If the static log is enabled returns the dynamic AppOutputLogStatus
                if constexpr(ThisType::StaticEnableAppOutputLog)
                    return this->AppOutputLogStatus;
                else
                    return false;
            }


        protected:
            inline void IncrementEntryIndex() noexcept {this->EntryIndex ++;}
            inline void PushLogOut()
            {
                this->AddNewline(this->CollectionString);

                //this->File.Print(this->CollectionString);

                if constexpr (StaticEnableAppOutputLog)
                {
                    if(this->IsAppOutputLogEnabled())
                    {
                        //fmt::print(this->CollectionString); //Slow and bad
                        std::cout.write(this->CollectionString.data(), this->CollectionString.size());
                        std::cout.flush();
                    }
                }
                this->AfterLog();
            }
            inline void AfterLog() noexcept
            {
                this->IncrementEntryIndex();
                this->CleanUp();
            }
            inline void CleanUp() noexcept
            {
                this->ResetString(this->CollectionString);
                this->ResetString(this->Tags);
            }

        protected:
            template<typename FirstMsgType, typename ... MsgTypes>
            inline void UnravelAndAddMsgs(FirstMsgType RVALUE_REF firstMsg, MsgTypes RVALUE_REF ... msgs)
            {
                this->Interpret.InterpretArg(std::forward<FirstMsgType>(firstMsg), this->TempString);
                this->AddUnformatedMsgPart(this->TempString, this->CollectionString);
                this->UnravelAndAddMsgs(std::forward<MsgTypes>(msgs)...);
            }
            inline void UnravelAndAddMsgs() {}


            template<typename FirstStringType, typename FirstValueType,
                     typename ... NameAndValueTypes>
            inline void UnravelAndAddVariables(FirstStringType RVALUE_REF firstString, FirstValueType RVALUE_REF firstValue,
                                                NameAndValueTypes RVALUE_REF ... namesAndValues)
            {
                this->AddListSeparator(this->CollectionString);

                this->AddFormatedVariableAsListPart<FirstValueType>(
                            std::forward<FirstStringType>(firstString),
                            std::forward<FirstValueType>(firstValue), this->Interpret, this->TempString, this->CollectionString);

                this->UnravelAndAddVariables(std::forward<NameAndValueTypes>(namesAndValues)...);
            }
            inline void UnravelAndAddVariables() {}


            template<typename FirstTagType, typename ... TagTypes>
            inline void UnravelAndAddTags(FirstTagType RVALUE_REF firstTag, TagTypes RVALUE_REF ... tags)
            {
                this->AddFormatedTag(std::forward<FirstTagType>(firstTag), this->Tags);
                this->AddSeparator(this->Tags);
                this->UnravelAndAddTags(std::forward<TagTypes>(tags)...);
            }
            inline void UnravelAndAddTags() {}


        protected:
            inline void AddFormatedDate(const time_t rawTime, std::string PASS_REF output) const
            {
                char str[26];
                ctime_s(str, sizeof str, &rawTime);

                output += "Date : [";
                output.append(str, 24); // doesnt put in the newline and nullcharcter
                output += "] ";
            }

            inline void AddFormatedTime(fmt::format_int PASS_REF formater, const clock_t clock, std::string PASS_REF output) const
            {
                output += '[';
                this->PutNumIntoCharcterField(static_cast<unsigned long>(clock), this->LogClockFieldSize, formater, output);
                output += "] ";
            }

            inline void AddFormatedSource(const std::string_view file, const u32 lineNum, fmt::format_int PASS_REF formater, std::string PASS_REF output) const
            {
                output += file;
                output += " : ";
                formater.format_unsigned(lineNum);
                output.append(formater.data(), formater.size());
                output += " - ";
            }

            inline void AddFormatedIterationCount(const u32 iterations, fmt::format_int PASS_REF formater, std::string PASS_REF output) const
            {
                this->PutNumIntoCharcterField(iterations, ThisType::LogIterationFieldSize, formater, output);
            }

            inline void AddFormatedLevel(const u32 level, std::string PASS_REF output) const
            {
                output += StringLiteralArray::LevelMarkers[level];
            }

            inline void AddFormatedMsg(std::string_view msg, std::string PASS_REF output) const
            {
                AddFormatedMsgHeader(output);
                output += msg;
            }

            inline void AddFormatedMsgHeader(std::string PASS_REF output) const
            {
                output += "msg = ";
            }

            inline void AddUnformatedMsgPart(std::string_view msg, std::string PASS_REF output) const
            {
                output += msg;
            }

            inline void AddFormatedTag(std::string_view tag, std::string PASS_REF output) const
            {
                output += "<";
                output += tag;
                output += ">";
            }

            template<typename FirstValueType>
            inline void AddFormatedVariableAsListPart(const StringViewType name, FirstValueType RVALUE_REF value, DataInterpretType PASS_REF interpret, std::string PASS_REF temp,  std::string PASS_REF output) const
            {
                interpret.InterpretArg(std::forward<FirstValueType>(value), temp);
                this->AddFormatedVariable(name, temp, output);
            }

            inline void AddFormatedVariable(const std::string_view varName, const std::string_view varValue, std::string PASS_REF output) const
            {
                output += varName;
                output += " = ";
                output += varValue;
            }

            inline void AddSeparator(std::string PASS_REF output) const
            {
                output += ThisType::Separator;
            }

            inline void AddListSeparator(std::string PASS_REF output) const
            {
                output += ThisType::ListSeparator;
            }

            inline void AddNewline(std::string PASS_REF output) const
            {
                output += ThisType::Newline;
            }


        protected:
            inline void PutNumIntoCharcterField(const unsigned long num, const u32 fieldLenght, fmt::format_int PASS_REF formater, std::string PASS_REF output) const
            {
                formater.format_unsigned(num);
                auto formatedSize = formater.size();

                //Only puts in the field if its size is bigger than the num as string size
                //Doesnt use if statements cause they are generally slow
                //Converts the the result of the comparison to i32 (false = 0; true = 1) and multiplies the size calculataion by it
                //This results in if the expression is untrue the result of the whole is always 0
                output.append(static_cast<u32>(fieldLenght > formatedSize) * (fieldLenght - formatedSize), '0');
                output.append(formater.data(), formatedSize);
            }
            inline void ResetString(std::string PASS_REF temp) const noexcept {temp.clear();}
    };


}
#endif // DEFAULTLOGGER_H
