#ifndef G_LOG_H
#define G_LOG_H


#include "LogWriter.h"
#include "LogDataInterpret.h"

//
template <typename DataInterpretType = DefaultDataInterpret>
class Logger
{

    public:
        using ThisType = Logger<DataInterpretType>;
        using DataInterpretPackageType = typename DataInterpretType::ThisPackageType;
        using DataInterpretInterfaceType = DataInterpretInterface<DataInterpretType, DataInterpretPackageType>;

    public:
        static constexpr u32 LevelArraySize = 16; //This should be equal to the max number of levels suported
        static constexpr u32 InitialConcatenationStringSize = 256;

    public:
        LogWriterInterface POINTER LevelPointerArray[LevelArraySize];
        bool LevelStatusArray [LevelArraySize];
        bool LoggingStatus;
        std::string ConcatenationString;

        std::unique_ptr<LogWriterInterface> NonemptyLogWriter;
        std::unique_ptr<LogWriterInterface> EmptyLogWriter;

    public:
        DataInterpretInterfaceType Interpret;

    public:
        ///Initialises the LogWriters to the default LogWriters
        Logger()
            : Interpret()
        {
            this->SetUp();
        }
        ///Initialises the LogWriters to the provided LogWriters
        Logger(std::unique_ptr<LogWriterInterface> PASS_REF nonemptyLogWriter,
               std::unique_ptr<LogWriterInterface> PASS_REF emptyLogWriter,
               const u32 concatenationStringSize = InitialConcatenationStringSize)
            : Interpret()
        {
            this->SetUp(nonemptyLogWriter, emptyLogWriter, concatenationStringSize);
        }

        ~Logger() = default;

    public:
        //Nontemplated level argument
        ///Logs by appending all given arguments together
        template<typename...ArgumentTypes>
        inline void LogMsg(const u32 level, const ArgumentTypes ... args)
        {
            this->ResetConcatenationString();
            this->UnravelAndConcatenateArguments(args...);
            this->GetLevelPointer(level)->LogMsg(level, this->ConcatenationString);
        }
        ///Logs by appending all given arguments together and adding source of the LOG command
        template<typename...ArgumentTypes>
        inline void LogMsgSource(const u32 level, const std::string_view fileName ,const u32 lineNum, const ArgumentTypes ... args)
        {
            this->ResetConcatenationString();
            this->UnravelAndConcatenateArguments(args...);
            this->GetLevelPointer(level)->LogMsgSource(level, fileName, lineNum, this->ConcatenationString);
        }
        ///Logs a pair of a variable name and its value
        template<typename VarType>
        inline void LogVar(const u32 level, const std::string_view varName, const VarType varValue)
        {
            this->GetLevelPointer(level)->LogVar(level, varName, this->Interpret.InetrpretArg(varValue));
        }
        ///Logs a pair of a variable name and its value and source of the LOG command
        template<typename VarType>
        inline void LogVarSource(const u32 level, const std::string_view fileName ,const u32 lineNum, const std::string_view varName, const VarType varValue)
        {           
            this->GetLevelPointer(level)->LogVarSource(level, fileName, lineNum, varName, this->Interpret.InetrpretArg(varValue));
        }
        
        //Templated level argument
        ///Logs by appending all given arguments together
        template<u32 level, typename...ArgumentTypes>
        inline void LogMsg(const ArgumentTypes ... args)
        {
            this->ResetConcatenationString();
            this->UnravelAndConcatenateArguments(args...);
            this->GetLevelPointer<level>()->LogMsg(level, this->ConcatenationString);
        }
        ///Logs by appending all given arguments together and adding source of the LOG command
        template<u32 level, typename...ArgumentTypes>
        inline void LogMsgSource(const std::string_view fileName, const u32 lineNum, const ArgumentTypes ... args)
        {
            this->ResetConcatenationString();
            this->UnravelAndConcatenateArguments(args...);
            this->GetLevelPointer<level>()->LogMsgSource(level, fileName, lineNum, this->ConcatenationString);
        }
        ///Logs a pair of a variable name and its value
        template<u32 level, typename VarType>
        inline void LogVar(const std::string_view varName, const VarType varValue)
        {
            this->GetLevelPointer<level>()->LogVar(level, varName, this->Interpret.InetrpretArg(varValue));
        }
        ///Logs a pair of a variable name and its value and source of the LOG command
        template<u32 level, typename VarType>
        inline void LogVarSource(const std::string_view fileName ,const u32 lineNum, const std::string_view varName, const VarType varValue)
        {
            this->GetLevelPointer<level>()->LogVarSource(level, fileName, lineNum, varName, this->Interpret.InetrpretArg(varValue));
        }

        template<typename ... StringTypes, typename ... ValueTypes>
        void LogVars(const u32 level, const StringTypes ... stringArgs,  const ValueTypes ... valueArgs)
        {
            LogVarsInternal(this->GetLevelPointer(level),
                            level, stringArgs..., valueArgs...);
        }

        template<u32 level, typename ... StringTypes, typename ... ValueTypes>
        void LogVars(const StringTypes ... stringArgs,  const ValueTypes ... valueArgs)
        {
            LogVarsInternal(this->GetLevelPointer<level>(),
                            level, stringArgs..., valueArgs...);
        }

        template<typename ... StringTypes, typename ... ValueTypes>
        void LogVarsSource(const u32 level, const std::string_view fileName, const u32 lineNum, const StringTypes ... stringArgs,  const ValueTypes ... valueArgs)
        {
            LogVarsSourceInternal(this->GetLevelPointer(level),
                                  level, fileName, lineNum, stringArgs..., valueArgs...);
        }

        template<u32 level, typename ... StringTypes, typename ... ValueTypes>
        void LogVarsSource(const std::string_view fileName, const u32 lineNum, const StringTypes ... stringArgs,  const ValueTypes ... valueArgs)
        {           
            LogVarsSourceInternal(this->GetLevelPointer<level>(),
                                  level, fileName, lineNum, stringArgs..., valueArgs...);
        }


    private:
        template<typename ... StringTypes, typename ... ValueTypes>
        inline void LogVarsInternal(LogWriterInterface POINTER logger, const u32 level, const StringTypes ... stringArgs,  const ValueTypes ... valueArgs)
        {
            logger->LogByPartsStart();
            logger->LogByPartsLvl(level);
            this->UnravelAndSendVariables(stringArgs..., valueArgs...);
            logger->LogByPartsFinish();
        }

        template<typename ... StringTypes, typename ... ValueTypes>
        inline void LogVarsSourceInternal(LogWriterInterface POINTER logger, const u32 level, const std::string_view fileName, const u32 lineNum, const StringTypes ... stringArgs,  const ValueTypes ... valueArgs)
        {
            logger->LogByPartsStart();
            logger->LogByPartsSource(fileName, lineNum);
            logger->LogByPartsLvl(level);
            this->UnravelAndSendVariables(stringArgs..., valueArgs...);
            logger->LogByPartsFinish();
        }


    public:
        void DoLog(const bool onOff)
        {
            this->LoggingStatus = onOff;
            this->UpdateLevelPointerArray();
        }
        inline void DisableLog()
        {
            this->DoLog(false);
        }
        inline void EnableLog()
        {
            this->DoLog(true);
        }

        template <u32 ... levels>
        void DoLoggingLevels(const bool onOff)
        {
            this->UnravelAndSetLoggingLevel<true, levels...>(onOff);
            //true is identifier due to technical limitations of variadic templates...
        }
        template <typename ... LevelTypes>
        void DoLoggingLevels(const bool onOff, const LevelTypes ... levels)
        {
            this->UnravelAndSetLoggingLevel(onOff, levels...);
        }

        template <u32 level>
        inline void DoLoggingLevel(const bool onOff)
        {
            this->LevelStatusArray[level] = onOff;
            if(onOff)
                this->LevelPointerArray[level] = this->GetNonemptyLogWriter();
            else
                this->LevelPointerArray[level] = this->GetEmptyLogWriter();
        }
        inline void DoLoggingLevel(const u32 level, const bool onOff)
        {
            this->LevelStatusArray[level] = onOff;
            if(onOff)
                this->LevelPointerArray[level] = this->GetNonemptyLogWriter();
            else
                this->LevelPointerArray[level] = this->GetEmptyLogWriter();
        }

        ///Returns if logging is disabled
        inline bool IsLoggingEnabled() const {return this->LoggingStatus;}
        ///Returns if logging is for a concrete level disabled
        inline bool IsLoggingLevelEnabled(const u32 level) const {return this->LevelStatusArray[level];}
        ///Returns if logging is for a concrete level disabled
        template <u32 level>
        inline bool IsLoggingLevelEnabled() const {return this->LevelStatusArray[level];}


        ///Changes the LogWriters for a pair of new ones; The old LogWriters will be destroyed
        void ChangeLoggers(std::unique_ptr<LogWriterInterface> PASS_REF nonemptyLogWriter,
                          std::unique_ptr<LogWriterInterface> PASS_REF emptyLogWriter)
        {
            ChangeNonemptyLogger(nonemptyLogWriter);
            ChangeEmptyLogger(emptyLogWriter);
        }
        ///Changes the nonempty logger core
        inline void ChangeNonemptyLogger(std::unique_ptr<LogWriterInterface> PASS_REF nonemptyLogWriter)
        {
            this->ChangeUsedLogger(nonemptyLogWriter, this->NonemptyLogWriter);
        }
        ///Changes the empty logger core
        inline void ChangeEmptyLogger(std::unique_ptr<LogWriterInterface> PASS_REF emptyLogWriter)
        {
            this->ChangeUsedLogger(emptyLogWriter, this->EmptyLogWriter);
        }
        ///Changes to the default loggers (DefaulNonemptytLogWriterType, DefaultEmptyLogWriterType)
        void ChangeToDefaultLoggers()
        {
            std::unique_ptr<LogWriterInterface> nonemptyLogger(std::make_unique<DefaulNonemptytLogWriter>());
            std::unique_ptr<LogWriterInterface> emptyLogger(std::make_unique<DefaultEmptyLogWriter>());

            ChangeLoggers(nonemptyLogger, emptyLogger);
        }

    public:
        template <typename LoggerType>
        static void SetLogger(std::unique_ptr<LogWriterInterface> PASS_REF logger)
        {
            logger = std::move(std::make_unique<LoggerType>());
        }

    private:
        void ExchangeLoggers(std::unique_ptr<LogWriterInterface> PASS_REF newLogger, std::unique_ptr<LogWriterInterface> PASS_REF oldLogger) const
        {
            oldLogger = std::move(newLogger);
        }

        void ChangeUsedLogger(std::unique_ptr<LogWriterInterface> PASS_REF newLogger, std::unique_ptr<LogWriterInterface> PASS_REF oldLogger)
        {
            ExchangeLoggers(newLogger, oldLogger);
            RebuildLevelPointerArray();
        }

        void RebuildLevelPointerArray()
        {
            //cycles through all of the level logging pointers and sets each of them according to the LevelStatusArray
            for(u32 i = 0; i < ThisType::LevelArraySize; i++)
            {
                if(IsLoggingLevelEnabled(i))
                    this->LevelPointerArray[i] = this->GetNonemptyLogWriter();
                else
                    this->LevelPointerArray[i] = this->GetEmptyLogWriter();
            }
        }

        void ResetLevelPointerArray(const bool doLog = true)
        {
            if(doLog)
                SetPointerArray(this->LevelPointerArray, ThisType::LevelArraySize, this->GetNonemptyLogWriter());
            else
                SetPointerArray(this->LevelPointerArray, ThisType::LevelArraySize, this->GetEmptyLogWriter());
        }
        inline void SetPointerArray(LogWriterInterface POINTER ptrArray[], const u32 arraySize, LogWriterInterface POINTER value) const
        {
            for(u32 i = 0; i < arraySize; i++)
                ptrArray[i] = value;
        }

        void ResetLevelStatusArray(const bool doLog = true)
        {
            SetStatusArray(this->LevelStatusArray, ThisType::LevelArraySize, doLog);
        }
        inline void SetStatusArray(bool statusArray[], const u32 arraySize, bool value) const
        {
            for(u32 i = 0; i < arraySize; i++)
                statusArray[i] = value;
        }

        void ResetLevelArrays(const bool doLog = true)
        {
            this->ResetLevelStatusArray(doLog);
            this->ResetLevelPointerArray(doLog);
        }

        inline void ReserveConcatenationString(const u32 size = InitialConcatenationStringSize)
        {
            this->ConcatenationString.reserve(size);
        }

        inline void ResetConcatenationString()
        {
            this->ConcatenationString.clear();
        }

        inline void AppendToConcatenationString(const std::string_view string)
        {
            this->ConcatenationString.append(string);
        }

        void SetUp(std::unique_ptr<LogWriterInterface> PASS_REF nonemptyLogWriter,
                   std::unique_ptr<LogWriterInterface> PASS_REF emptyLogWriter,
                   const u32 concatenationStringSize = InitialConcatenationStringSize)
        {
            this->ChangeLoggers(nonemptyLogWriter, emptyLogWriter);
            this->ResetLevelArrays();
            this->ReserveConcatenationString(concatenationStringSize);
            this->ResetConcatenationString();
        }
        inline void SetUp()
        {
            std::unique_ptr<LogWriterInterface> nonemptyLogger(std::make_unique<DefaulNonemptytLogWriter>());
            std::unique_ptr<LogWriterInterface> emptyLogger(std::make_unique<DefaultEmptyLogWriter>());

            this->SetUp(nonemptyLogger, emptyLogger, InitialConcatenationStringSize);
        }

        //unravel
        template<typename FirstArgument, typename...ArgumentTypes>
        inline void UnravelAndConcatenateArguments(const FirstArgument firstArg, const ArgumentTypes... args)
        {
            this->AppendToConcatenationString(this->Interpret.InetrpretArg(firstArg));
            this->UnravelAndConcatenateArguments(args...);
        }
        inline void UnravelAndConcatenateArguments() {}


        template<bool idnetifier, u32 firstLevel, u32 ... levels>
        void UnravelAndSetLoggingLevel(const bool onOff)
        {
            this->DoLoggingLevel<firstLevel>(onOff);
            UnravelAndSetLoggingLevel<idnetifier, levels...>(onOff);
        }
        template<bool idnetifier>
        void UnravelAndSetLoggingLevel(const bool) {}


        template<typename FirstLevelType, typename ... LevelTypes>
        void UnravelAndSetLoggingLevel(const bool onOff, const FirstLevelType firstLevel, const LevelTypes ... levels)
        {
            this->DoLoggingLevel(firstLevel, onOff);
            UnravelAndSetLoggingLevel(onOff, levels...);
        }
        void UnravelAndSetLoggingLevel(const bool) {}


        template<typename FirstStringArgument, typename FirstValueArgument,
                 typename ... StringArgumentTypes, typename ... ValueArgumentTypes>
        inline void UnravelAndSendVariables(const FirstStringArgument firstString, const FirstValueArgument firstValue,
                                            const StringArgumentTypes ... stringArgs, const ValueArgumentTypes ... valueArgs)
        {
            this->NonemptyLogWriter->LogByPartsVar(firstString, this->Interpret.InetrpretArg(firstValue));
            this->UnravelAndSendVariables(stringArgs..., valueArgs...);
        }
        inline void UnravelAndSendVariables() {}

    private:
        static constexpr bool IsIndexInLevelArrayRange(const u32 num)
        {
            return (num < ThisType::LevelArraySize);
        }

    public:
        ///Returns the First (currently in use) logger core
        inline LogWriterInterface POINTER GetNonemptyLogWriter() const {return NonemptyLogWriter.get();}
        ///Returns the Second (currently not in use) logger core
        inline LogWriterInterface POINTER GetEmptyLogWriter() const {return EmptyLogWriter.get();}
        template <u32 level>
        inline LogWriterInterface POINTER GetLevelPointer() const
        {
            if(ThisType::IsIndexInLevelArrayRange(level))
                return LevelPointerArray[level];
            else
                return LevelPointerArray[0]; //TODO throw exeption
        }
        inline LogWriterInterface POINTER GetLevelPointer(const u32 level) const
        {
            if(ThisType::IsIndexInLevelArrayRange(level))
                return LevelPointerArray[level];
            else
                return LevelPointerArray[0]; //TODO throw exeption
        }
};

#include "LogDefines.h"



#endif // G_LOG_H
