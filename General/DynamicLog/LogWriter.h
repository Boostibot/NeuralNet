#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <string_view>
#include <chrono>
#include <string>
#include <sstream>
#include <memory>
#include <iostream>
#include <fstream>
#include <typeinfo>

#include "General/Common/Defines.h"
#include "General/Common/Typedefs.h"

//LogWriters are the most influencial classes on the logging process
//Logging cores are tasked with doing the following:
//--1.Format the messages
//--2.Manage the logging tools and resources (files, buffers etc.)
//--3.Put the final messages on to the logs
//
//--All LogWriters must inherit from LogWriterInterface and override the specified functions
//--All LogWriters must be either PassiveLogWriters - used to switch logging off (generaly do nothing)
//-- or OperativeLogWriters used to normally do all of the explained above responsabilities

class LogWriterInterface
{
    public:
        ///Logs the given msg on the given level
        virtual void LogMsg(const u32 level, const std::string_view msg) = 0;
        ///Logs the given varName alongside its value on the given level
        virtual void LogVar(const u32 level, const std::string_view varName, const std::string_view varValue) = 0;
        ///Logs the given msg on the given leveland the initial LOG command origin file and line on the given level
        virtual void LogMsgSource(const u32 level, const std::string_view file, const u32 lineNum, const std::string_view msg)  = 0;
        ///Logs the given varName alongside its value and the initial LOG command origin file and line on the given level
        virtual void LogVarSource(const u32 level, const std::string_view file, const u32 lineNum, const std::string_view varName, const std::string_view varValue)  = 0;

        ///Starts the logging by parts process; Always should be accompanied by LogByPartsFinish()
        virtual void LogByPartsStart() = 0;
        ///Adds a level (level) part to the LogByParts process
        virtual void LogByPartsLvl(const u32 level) = 0;
        ///Adds a source (file, line) part to the LogByParts process
        virtual void LogByPartsSource(const std::string_view file, const u32 lineNum) = 0;
        ///Adds a message (msg) part to the LogByParts process
        virtual void LogByPartsMsg(const std::string_view msg) = 0;
        ///Adds a variable (varName, varValue) part to the LogByParts process
        virtual void LogByPartsVar(const std::string_view varName, const std::string_view varValue) = 0;
        ///Adds a tags (tags) part to the LogByParts process
        virtual void LogByPartsTags(const std::string_view tags) = 0;
        ///Ends the logging by parts process; Always should be accompanied by LogByPartsStart()
        virtual void LogByPartsFinish() = 0;

        ///Returns if this LogWriter is an passive logger (passive logger = doesnt write log with any given input)
        virtual bool IsPassiveLogger() = 0;

    public:
        ///Logs a custom message; The functionSpecifier shoudl be filled with the implementaton specific enum values
        virtual void LogCustom(const i32 functionSpecifier, const u32 level, void POINTER pointer = nullptr) {(void)functionSpecifier; (void)level; (void)pointer;}
        ///Sets a custom log state; The functionSpecifier shoudl be filled with the implementaton specific enum values
        virtual void SetCustomLogState(const i32 stateSpecifier, void POINTER pointer = nullptr) {(void)stateSpecifier; (void)pointer;}

        ///Returns if the output stream is open; This can mean the file, buffer or anything else is ready for writing log
        virtual bool IsOutputStreamOpen() {return false;}
        ///Opens the output stream
        virtual void OpenOutputStream(const std::string_view path, const i32 flag) {(void)path; (void)flag;}
        ///Closes the output stream
        virtual void CloseOutputStream() {}
        ///Flushes the output stream (if flushing is used)
        virtual void FlushOutputStream() {}
        virtual ~LogWriterInterface() {}

    public:
        ///Returns the hash code of this logger
        std::size_t GetLoggerTypeHash() const
        {
            return typeid (POINTER_VALUE(this)).hash_code();
        }
        ///Returns the name of the type of this logger
        std::string GetLoggerTypeName() const
        {
            return std::string(typeid (POINTER_VALUE(this)).name());
        }
};

//The default loggers
class DefaultPassiveLogWriter : public LogWriterInterface
{
    public:
        virtual void LogMsg(const u32, const std::string_view) noexcept final {}
        virtual void LogVar(const u32, const std::string_view, const std::string_view) noexcept final {}
        virtual void LogMsgSource(const u32, const std::string_view, const u32, const std::string_view) noexcept final {}
        virtual void LogVarSource(const u32, const std::string_view, const u32, const std::string_view, const std::string_view) noexcept final {}

        virtual void LogByPartsStart() noexcept final {}
        virtual void LogByPartsLvl(const u32) noexcept final {}
        virtual void LogByPartsSource(const std::string_view, const u32) noexcept final {}
        virtual void LogByPartsMsg(const std::string_view) noexcept final {}
        virtual void LogByPartsVar(const std::string_view, const std::string_view) noexcept final {}
        virtual void LogByPartsTags(const std::string_view) noexcept final {}
        virtual void LogByPartsFinish() noexcept final {}

        virtual bool IsPassiveLogger() noexcept final {return true;}

        virtual bool IsOutputStreamOpen() noexcept final {return false;}
        virtual void OpenOutputStream(const std::string_view, const i32) noexcept final {}
        virtual void CloseOutputStream() noexcept final {}
        virtual void FlushOutputStream() noexcept final {}
        ~DefaultPassiveLogWriter() {}
};

class DefaultOperativeLogWriter : public LogWriterInterface
{
    public:
        using ThisType = DefaultOperativeLogWriter;

    public:
        enum CustomLogFunctions
        {
            LogLoggerinfo = 0,
            LogConstructionMsg = 1,
            LogDestructionMsg = 2,
            LogLoggerIndentifier = 3,
        };
        enum CustomLogStates
        {
            EnableAppOutputLog = 0,
            DisableAppOutputLog = 1,
            EnableFileLog = 2,
            DisableFileLog = 3,
        };
        /*
        enum _Ios_Openmode
          {
            _S_app 		= 1L << 0,
            _S_ate 		= 1L << 1,
            _S_bin 		= 1L << 2,
            _S_in 		= 1L << 3,
            _S_out 		= 1L << 4,
            _S_trunc 		= 1L << 5,
            _S_ios_openmode_end = 1L << 16,
            _S_ios_openmode_max = __INT_MAX__,
            _S_ios_openmode_min = ~__INT_MAX__
          };
        */

        //TODO - Add opening, closing and writing to a file (+flushing)
    public:
        std::ofstream File;
        std::string CollectionString;
        std::string TempString;
        u32 EntryIndex;

        //Parts
        std::string SourcePartString;
        std::string LevelPartString;
        std::string MessagesPartString;
        std::string VariablesPartString;
        std::string TagsPartString;


    public:
        static constexpr u32 MinimumCollectionStringSize = 512;
        static constexpr u32 MinimumTempStringSize = 256;
        static constexpr u32 MinimumPartStringSize = 128;
        static constexpr u32 LogIterationFieldSize = 8;
        static constexpr u32 LogClockFieldSize = 8;
        static constexpr char Separator = ' ';


    public:
        DefaultOperativeLogWriter() noexcept {this->SetUp();}
        DefaultOperativeLogWriter(const u32 collectioStringSize,
                                  const u32 tempStringSize,
                                  const u32 entryIndex,
                                  const u32 partStringSize)
        {
            this->SetUp(collectioStringSize, tempStringSize, partStringSize, entryIndex);
        }
        //DefaultOperativeLogWriter(const DefaultOperativeLogWriter REF) = default;
        DefaultOperativeLogWriter(DefaultOperativeLogWriter RVALUE_REF) = default;


    public:
        virtual void LogMsg(const u32 level, const std::string_view msg) final
        {
            this->LogByPartsStart();
            this->LogByPartsLvl(level);
            this->LogByPartsMsg(msg);
            this->LogByPartsFinish();
        }
        virtual void LogVar(const u32 level, const std::string_view varName, const std::string_view varValue) final
        {
            this->LogByPartsStart();
            this->LogByPartsLvl(level);
            this->LogByPartsVar(varName, varValue);
            this->LogByPartsFinish();
        }
        virtual void LogMsgSource(const u32 level, const std::string_view file, const u32 lineNum, const std::string_view msg) final
        {

            this->LogByPartsStart();
            this->LogByPartsSource(file, lineNum);
            this->LogByPartsLvl(level);
            this->LogByPartsMsg(msg);
            this->LogByPartsFinish();
        }
        virtual void LogVarSource(const u32 level, const std::string_view file, const u32 lineNum, const std::string_view varName, const std::string_view varValue) final
        {
            this->LogByPartsStart();
            this->LogByPartsSource(file, lineNum);
            this->LogByPartsLvl(level);
            this->LogByPartsVar(varName, varValue);
            this->LogByPartsFinish();
        }

        ///Starts the logging by parts process; Always should be accompanied by LogByPartsFinish()
        virtual void LogByPartsStart() final
        {
            this->ResetPartStrings();
        }
        ///Adds a source (file, line) part to the LogByParts process
        virtual void LogByPartsSource(const std::string_view file, const u32 lineNum) final
        {
            //Support only one source at a time - clears the string every time
            ResetString(this->SourcePartString);
            AddFormatedSource(file, lineNum, this->SourcePartString);
            AddSeparator(this->SourcePartString);
        }
        ///Adds a level (level) part to the LogByParts process
        virtual void LogByPartsLvl(const u32 level) final
        {
            //Support only one level at a time - clears the string every time
            ResetString(this->LevelPartString);
            AddFormatedLevel(level, this->LevelPartString);
            AddSeparator(this->LevelPartString);
        }
        ///Adds a message (msg) part to the LogByParts process
        virtual void LogByPartsMsg(const std::string_view msg) final
        {
            //Adds the message to the MessagesPartString
            this->AddFormatedMsg(msg, this->MessagesPartString);
            AddSeparator(this->MessagesPartString);
        }
        ///Adds a variable (varName, varValue) part to the LogByParts process
        virtual void LogByPartsVar(const std::string_view varName, const std::string_view varValue) final
        {
            //Adds the message to the MessagesPartString
            this->AddFormatedVariable(varName, varValue, this->VariablesPartString);
            AddSeparator(this->VariablesPartString);
        }        
        ///Adds a tags (tags) part to the LogByParts process
        virtual void LogByPartsTags(const std::string_view tags)
        {
            this->AddFormatedTags(tags, this->TagsPartString);
            AddSeparator(this->VariablesPartString);
        }
        ///Ends the logging by parts process; Always should be accompanied by LogByPartsStart()
        virtual void LogByPartsFinish() final
        {
            //---Resets the collection
            {
                this->ResetCollectionString();
            }

            //---Composes the message
            {
                //Source
                CollectionString += SourcePartString;

                //Interations
                this->ResetTempString();
                AddFormatedIterationCount(this->EntryIndex, this->TempString, this->CollectionString);
                AddSeparator(this->CollectionString);

                //Time
                this->ResetTempString();
                AddFormatedDate(this->TempString, this->CollectionString);
                AddSeparator(this->CollectionString);

                //Level
                CollectionString += LevelPartString;

                //Between level and msg
                CollectionString += ": ";

                //Message
                CollectionString += MessagesPartString;

                //Variables
                CollectionString += VariablesPartString;

                //Tags
                CollectionString += TagsPartString;
            }

            //---Sends the message
            {
                PushStringOut(CollectionString);
            }

            this->IncrementEntryIndex();
        }


        ///Logs a custom message; The functionSpecifier shoudl be filled with the implementaton specific enum values
        virtual void LogCustom(const i32 functionSpecifier, const u32 level, void POINTER pointer = nullptr) noexcept final
        {
            (void)functionSpecifier; (void)level; (void)pointer;
        }
        ///Sets a custom log state; The functionSpecifier shoudl be filled with the implementaton specific enum values
        virtual void SetCustomLogState(const i32 functionSpecifier, void POINTER pointer = nullptr) noexcept final
        {
            (void)functionSpecifier; (void)pointer;
        }

        ///Logs a message signifying the construction of the logger
        void LogConstructionMessage()
        {
            this->ResetCollectionString();

            this->ResetTempString();
            AddFormatedIterationCount(this->EntryIndex, this->TempString, this->CollectionString);
            AddSeparator(this->CollectionString);

            this->ResetTempString();
            AddFormatedDate(this->TempString, this->CollectionString);
            AddSeparator(this->CollectionString);

            CollectionString += "=== Logging start: Default Logger Core constructed ===";

            PushStringOut(CollectionString);
        }
        ///Logs a message signifying the destruction of the logger
        void LogDestructionMessage()
        {
            this->ResetCollectionString();

            this->ResetTempString();
            AddFormatedIterationCount(this->EntryIndex, this->TempString, this->CollectionString);
            AddSeparator(this->CollectionString);

            this->ResetTempString();
            AddFormatedDate(this->TempString, this->CollectionString);
            AddSeparator(this->CollectionString);

            CollectionString += "=== Logging end: Default Logger Core destructed ===";

            PushStringOut(CollectionString);
        }
        ///Logs info messege about the logger
        void LogLoggerInfo(const u32 level)
        {
            this->LogByPartsStart();
            this->LogByPartsLvl(level);
            this->LogByPartsMsg("Logger info : Default logger core; Displaying in order: 1-Code source 2-iteration count 3-Date and time 4-Level 5-Messages/variables");
            this->LogByPartsFinish();
        }

        virtual bool IsPassiveLogger() final {return false;}
        virtual bool IsOutputStreamOpen() final
        {
            return  this->File.is_open();
        }
        virtual void OpenOutputStream(const std::string_view path, const i32) final
        {
            this->File.open(path.data(), std::ios::out | std::ios::ate);
        }
        virtual void CloseOutputStream() final
        {
            this->File.close();
        }
        virtual void FlushOutputStream() final
        {
            this->File.flush();
        }
        ~DefaultOperativeLogWriter() {this->TearDown();}


    private:

        inline void AddFormatedDate(std::string REF temp, std::string REF output) const
        {
            time_t rawTime;
            time(&rawTime);

            char str[26];
            ctime_s(str, sizeof str, &rawTime);

            temp.append(str);
            output += '[';
            temp.pop_back();
            output += temp;
            output += " - ";
            this->ResetString(temp);
            this->PutNumIntocharcterField(clock(), this->LogIterationFieldSize, temp, output);
            output += ']';
        }

        inline void AddFormatedSource(const std::string_view file, const u32 lineNum, std::string REF output) const
        {
            output += file;
            output += " [";
            output += std::to_string(lineNum);
            output += "]";
        }

        inline void AddFormatedIterationCount(const u32 iterations, std::string REF temp, std::string REF output) const
        {
            this->PutNumIntocharcterField(iterations, ThisType::LogIterationFieldSize, temp, output);
        }

        inline void AddFormatedLevel(const u32 level, std::string REF output) const
        {
            output += '<';
            output += std::to_string(level);
            output += '>';
        }

        inline void AddFormatedMsg(const std::string_view msg, std::string REF output) const
        {
            output += "msg = ";
            output += msg;
        }

        inline void AddFormatedVariable(const std::string_view varName, const std::string_view varValue, std::string REF output) const
        {
            output += varName;
            output += " = ";
            output += varValue;
        }

        inline void AddFormatedTags(const std::string_view tags, std::string REF output) const
        {
            output += "tags = <";
            output += tags;
            output += ">";
        }

        inline void AddSeparator(std::string REF output) const
        {
            output += Separator;
        }

    private:
        inline void PutNumIntocharcterField(const u32 num, const u32 fieldLenght, std::string REF temp, std::string REF output) const
        {
            temp += std::to_string(num);
            //Only puts in the field if its size is bigger than the num as string size
            //Doesnt use if statements cause they are generally slow
            //Converts the the result of the comparison to i32 (false = 0; true = 1) and multiplies the size calculataion by it
            //This results in if the expression is untrue the result of the whole is always 0
            output.append(static_cast<u32>(fieldLenght > temp.size()) * (fieldLenght - temp.size()), '0');
            output += temp;
        }


    private:
        inline void ResetString(std::string REF temp) const noexcept {temp.clear();}
        inline void ResetCollectionString() {this->ResetString(this->CollectionString);}
        inline void ResetTempString() {this->ResetString(this->TempString);}
        inline void ReserveStrings(const u32 collectioStringSize = MinimumCollectionStringSize,
                                   const u32 tempStringSize = MinimumTempStringSize,
                                   const u32 partStringSize = MinimumPartStringSize)
        {
            this->CollectionString.reserve(collectioStringSize);
            this->TempString.reserve(tempStringSize);
            this->SourcePartString.reserve(partStringSize);
            this->LevelPartString.reserve(partStringSize);
            this->MessagesPartString.reserve(partStringSize);
            this->VariablesPartString.reserve(partStringSize);
            this->TagsPartString.reserve(partStringSize);
        }

        inline void SetUp(const u32 collectioStringSize = MinimumCollectionStringSize,
                                        const u32 tempStringSize = MinimumTempStringSize,
                                        const u32 partStringSize = MinimumPartStringSize,
                                        const u32 entryIndex = 0)
        {
            this->InitialiseVariables(collectioStringSize, tempStringSize, partStringSize, entryIndex);
            this->LogConstructionMessage();
        }
        inline void TearDown()
        {
            this->LogDestructionMessage();
            this->CloseOutputStream();
        }

        inline void InitialiseVariables(const u32 collectioStringSize = MinimumCollectionStringSize,
                                        const u32 tempStringSize = MinimumTempStringSize,
                                        const u32 partStringSize = MinimumPartStringSize,
                                        const u32 entryIndex = 0)
        {
            this->ReserveStrings(collectioStringSize, tempStringSize, partStringSize);
            this->EntryIndex = entryIndex;
        }
        inline void ResetPartStrings()
        {
            this->ResetString(SourcePartString);
            this->ResetString(LevelPartString);
            this->ResetString(MessagesPartString);
            this->ResetString(VariablesPartString);
            this->ResetString(TagsPartString);
        }
        inline void IncrementEntryIndex() {this->EntryIndex ++;}
        inline void PushStringOut(const std::string_view REF string)
        {
            if(File.is_open())
                this->File << string << std::endl;
            std::cout << string << std::endl;
        }
};




#endif // LOGWRITER_H
