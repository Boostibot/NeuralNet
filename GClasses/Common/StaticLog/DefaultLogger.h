#ifndef DEFAULTLOGGER_H
#define DEFAULTLOGGER_H

#include "DefaultDataInterpret.h"

namespace StaticLog
{
    //The default logger interpret
    template <typename DataInterpretType = DefaultDataInterpret>
    struct DefaultLoggerPackage
    {
        public:
            struct AlwaysReadyFile
            {
                public:
                    std::unique_ptr<fmt::ostream> File;

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
                        File = std::make_unique<fmt::ostream>(fmt::output_file(filePath.data()));
                    }
                    inline void Print(const StringViewType msg)
                    {
                        File->print(msg);
                    }
            };

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
            AlwaysReadyFile File;
            StringType CollectionString;
            StringType TempString;

            StringType Tags;

            u32 EntryIndex;

        protected:
            void SetUp()
            {
                this->ReserveStrings();

                EntryIndex = 0;
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


    template<typename DataInterpretTypeArg = DefaultDataInterpret>
    class DefaultLogger : public LoggerInterface<DefaultLogger<DataInterpretTypeArg>,
                                                 DefaultLoggerPackage<DataInterpretTypeArg>
                                                >
    {
        public:
            using DataInterpretType = DataInterpretTypeArg;
            using ThisPackageType = DefaultLoggerPackage<DataInterpretType>;
            using ThisType = DefaultLogger<DataInterpretType>;
            using ThisLoggerInterfaceType = LoggerInterface<ThisType, ThisPackageType>;
            using StringType = ::std::string;
            using StringViewType = ::std::string_view;
            friend ThisLoggerInterfaceType; //Is needed in order for LoggerInterface to access the protected necesarry Overloaded functions

        protected:
            static constexpr u32 LogIterationFieldSize = 8;
            static constexpr u32 LogClockFieldSize = 8;
            static constexpr char Newline = '\n';
            static constexpr char Separator = ' ';
            static constexpr const char ListSeparator[] = ", ";
            static constexpr bool DoAppOutputLog = false;

            struct StringLiteralArray
            {
                    //Crashes debugger when part of the class
                    //So its necessary to put it into unralated class
                    static constexpr char LevelMarkers[ThisType::LevelCount][3] {{"00"}, {"01"}, {"02"}, {"03"}, {"04"}, {"05"}, {"06"}, {"07"}, {"08"}, {"09"}, {"10"}, {"11"}, {"12"}, {"13"}, {"14"}, {"15"}};
            };


            //The obligatory function overloads
        protected:
            template<typename ... MsgTypes>
            void LogMsgsOverload(const u32 level, const MsgTypes ... msgs)
            {
                this->AddIterations();
                this->AddTime();
                this->AddLvl(level);
                this->AddMsgs(msgs...);
                this->AddTags();

                this->PushLogOut();
            }
            template<typename ... NameAndValueTypes>
            void LogVarsOverload(const u32 level, NameAndValueTypes... namesAndValues)
            {
                this->AddIterations();
                this->AddTime();
                this->AddLvl(level);
                this->AddVars(namesAndValues...);
                this->AddTags();

                this->PushLogOut();
            }
            template<typename ... MsgTypes>
            void LogMsgsSourceOverload(const StringViewType file, const u32 lineNum, const u32 level, const MsgTypes ... msgs)
            {
                this->AddSource(file, lineNum);
                this->AddIterations();
                this->AddTime();
                this->AddLvl(level);
                this->AddMsgs(msgs...);
                this->AddTags();

                this->PushLogOut();
            }
            template<typename ... NameAndValueTypes>
            void LogVarsSourceOverload(const StringViewType file, const u32 lineNum, const u32 level, NameAndValueTypes... namesAndValues)
            {
                this->AddSource(file, lineNum);
                this->AddIterations();
                this->AddTime();
                this->AddLvl(level);
                this->AddVars(namesAndValues...);
                this->AddTags();

                this->PushLogOut();
            }

            template<typename ... TagTypes>
            void AppendTagsOverload(TagTypes ... tags)
            {
                this->UnravelAndAddTags(tags...);
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
                     std::enable_if_t<!(AreTypesSame<ThisType PASS_REF, ArgumentTypes...>::value && (sizeof...(ArgumentTypes) == 1)), int> = 0
                     >
            DefaultLogger(ArgumentTypes PASS_RVALUE_REF ... args) : ThisLoggerInterfaceType(std::forward<ArgumentTypes>(args)...) {this->LogConstructionMessage();}
            ~DefaultLogger() {this->LogDestructionMessage();}

            DefaultLogger REF operator=(const DefaultLogger PASS_REF other) = delete;
            DefaultLogger REF operator=(DefaultLogger PASS_RVALUE_REF) = default;

        protected:
            void AddSource(const StringViewType file, const u32 lineNum)
            {
                AddFormatedSource(file, lineNum, this->Formater, this->CollectionString);
                AddSeparator(this->CollectionString);
            }

            void AddIterations()
            {
                AddFormatedIterationCount(this->EntryIndex, this->Formater, this->CollectionString);
                AddSeparator(this->CollectionString);
            }

            void AddTime()
            {
                time_t rawTime;
                time(&rawTime);

                AddFormatedTime(this->Formater, rawTime, clock(), this->CollectionString);
                AddSeparator(this->CollectionString);
            }

            void AddLvl(const u32 level)
            {
                AddFormatedLevel(level, this->CollectionString);
                AddSeparator(this->CollectionString);
            }

            template<typename ... MsgTypes>
            void AddMsgs(MsgTypes... msgs)
            {
                AddFormatedMsgHeader(this->CollectionString);
                UnravelAndAddMsgs(msgs...);
                AddSeparator(this->CollectionString);
            }

            template<typename FirstStringArgument, typename FirstValueArgument,
                     typename ... NameAndValueTypes>
            inline void AddVars(const FirstStringArgument firstString, const FirstValueArgument firstValue,
                                                NameAndValueTypes ... namesAndValues)
            {
                static_assert (AreEven<NameAndValueTypes...>(), "DefaultLogger: AppendVars requires even number of arguments");

                this->AddFormatedVariableAsListPart(firstString, firstValue, this->Interpret, this->TempString, this->CollectionString);
                this->UnravelAndAddVariables(namesAndValues...);

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

                this->AppendTagsOverload("log_construction", "log_starting");
                this->LogMsgsOverload(level, "Log Starting................");
            }
            template<u32 level = ThisType::MaxLevelIndex>
            void LogDestructionMessage()
            {
                if(NOT(this->template IsLogEnabled<level>()))
                    return;

                this->AppendTagsOverload("log_destruction", "log_ending");
                this->LogMsgsOverload(level, "Log Ending..................");
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


        protected:
            inline void IncrementEntryIndex() {this->EntryIndex ++;}
            inline void PushLogOut()
            {
                this->AddNewline(this->CollectionString);

                this->File.Print(this->CollectionString);

                if(DoAppOutputLog)
                    fmt::print(this->CollectionString);

                this->AfterLog();
            }
            inline void AfterLog()
            {
                this->IncrementEntryIndex();
                this->CleanUp();
            }
            inline void CleanUp()
            {
                this->ResetString(this->CollectionString);
                this->ResetString(this->Tags);
            }

        protected:
            //TODO - Add type checks using AreSameType<...>::value
            template<typename FirstMsgType, typename ... MsgTypes>
            inline void UnravelAndAddMsgs(FirstMsgType firstMsg, MsgTypes ... msgs)
            {
                this->Interpret.InterpretArg(firstMsg, this->TempString);
                this->AddUnformatedMsgPart(this->TempString, this->CollectionString);
                this->UnravelAndAddMsgs(msgs...);
            }
            inline void UnravelAndAddMsgs() {}


            template<typename FirstStringArgument, typename FirstValueArgument,
                     typename ... NameAndValueTypes>
            inline void UnravelAndAddVariables(const FirstStringArgument firstString, const FirstValueArgument firstValue,
                                                NameAndValueTypes ... namesAndValues)
            {
                this->AddListSeparator(this->CollectionString);
                this->AddFormatedVariableAsListPart<FirstValueArgument>(firstString, firstValue, this->Interpret, this->TempString, this->CollectionString);
                this->UnravelAndAddVariables(namesAndValues...);
            }
            inline void UnravelAndAddVariables() {}


            template<typename FirstTagType, typename ... TagTypes>
            inline void UnravelAndAddTags(FirstTagType firstTag, TagTypes ... tags)
            {
                this->AddFormatedTag(firstTag, this->Tags);
                this->AddSeparator(this->Tags);
                this->UnravelAndAddTags(tags...);
            }
            inline void UnravelAndAddTags() {}


        protected:
            inline void AddFormatedTime(fmt::format_int PASS_REF formater, const time_t rawTime, const clock_t clock, std::string PASS_REF output) const
            {
                char str[26];
                ctime_s(str, sizeof str, &rawTime);

                output += '[';
                output.append(str);
                output.pop_back(); //Removes th newline character
                output += " - ";
                this->PutNumIntoCharcterField(static_cast<unsigned long>(clock), this->LogIterationFieldSize, formater, output);
                output += ']';
            }

            inline void AddFormatedSource(const std::string_view file, const u32 lineNum, fmt::format_int PASS_REF formater, std::string PASS_REF output) const
            {
                output += file;
                output += " [";
                formater.format_unsigned(lineNum);
                output.append(formater.data(), formater.size());
                output += ']';
            }

            inline void AddFormatedIterationCount(const u32 iterations, fmt::format_int PASS_REF formater, std::string PASS_REF output) const
            {
                this->PutNumIntoCharcterField(iterations, ThisType::LogIterationFieldSize, formater, output);
            }

            inline void AddFormatedLevel(const u32 level, std::string PASS_REF output) const
            {
                //TODO - add these together
                output += '<';
                output += StringLiteralArray::LevelMarkers[level];
                output += '>';
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

            template<typename FirstValueArgument>
            inline void AddFormatedVariableAsListPart(const StringViewType name, const FirstValueArgument value, DataInterpretType PASS_REF interpret, std::string PASS_REF temp,  std::string PASS_REF output) const
            {
                interpret.InterpretArg(static_cast<FirstValueArgument>(value), temp);
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
