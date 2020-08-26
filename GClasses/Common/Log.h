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
        using DefaultOperativeLogWriterType = DefaultOperativeLogWriter;
        using DefaultPassiveLogWriterType = DefaultPassiveLogWriter;

    public:
        static constexpr u32 LevelArraySize = 16; //This should be equal to the max number of levels suported
        static constexpr u32 InitialStringBufferSize = 256;
        static constexpr u32 InitialTempStringSize = 128;

    private:
        LogWriterInterface POINTER LevelPointerArray[LevelArraySize];
        bool LevelStatusArray [LevelArraySize];
        bool LoggingStatus;

        std::string StringBuffer;
        std::string TempString;

        std::unique_ptr<LogWriterInterface> OperativeLogWriter;
        std::unique_ptr<LogWriterInterface> PassiveLogWriter;

    public:
        DataInterpretInterfaceType Interpret;

    public:
        Logger() : Interpret() noexcept
        {
            this->SetUp();
        }
        ///Initialises the LogWriters to the provided LogWriters
        Logger(std::unique_ptr<LogWriterInterface> operativeLogWriter,
               std::unique_ptr<LogWriterInterface> passiveLogWriter,
               const u32 StringBufferSize = InitialStringBufferSize,
               const u32 TempStringSize = InitialTempStringSize)
            : Interpret()
        {
            this->SetUp(operativeLogWriter, passiveLogWriter, StringBufferSize, TempStringSize);
        }
        Logger(ThisType PASS_RVALUE_REF other) = default;

    public:
        //Nontemplated level argument
        ///Logs by appending all given arguments together
        template<typename...ArgumentTypes>
        inline void LogMsg(const u32 level, const ArgumentTypes ... args)
        {
            this->LogMsgInternal(this->GetLevelPointer(level),
                                 level, args...);
        }
        ///Logs by appending all given arguments together and adding source of the LOG command
        template<typename...ArgumentTypes>
        inline void LogMsgSource(const u32 level, const std::string_view fileName ,const u32 lineNum, const ArgumentTypes ... args)
        {
            this->LogMsgSourceInternal(this->GetLevelPointer(level),
                                       level, fileName, lineNum, args...);
        }
        ///Logs a pair of a variable name and its value
        template<typename VarType>
        inline void LogVar(const u32 level, const std::string_view varName, const VarType varValue)
        {
            this->LogVarInternal(this->GetLevelPointer(level),
                                       level, varName, varValue);
        }
        ///Logs a pair of a variable name and its value and source of the LOG command
        template<typename VarType>
        inline void LogVarSource(const u32 level, const std::string_view fileName ,const u32 lineNum, const std::string_view varName, const VarType varValue)
        {
            this->LogVarSourceInternal(this->GetLevelPointer(level),
                                       level, fileName, lineNum, varName, varValue);
        }
        
        //Templated level argument
        ///Logs by appending all given arguments together
        template<u32 level, typename...ArgumentTypes>
        inline void LogMsg(const ArgumentTypes ... args)
        {
            this->LogMsgInternal(this->GetLevelPointer<level>(),
                                 level, args...);
        }
        ///Logs by appending all given arguments together and adding source of the LOG command
        template<u32 level, typename...ArgumentTypes>
        inline void LogMsgSource(const std::string_view fileName, const u32 lineNum, const ArgumentTypes ... args)
        {
            this->LogMsgSourceInternal(this->GetLevelPointer<level>(),
                                       level, fileName, lineNum, args...);
        }
        ///Logs a pair of a variable name and its value
        template<u32 level, typename VarType>
        inline void LogVar(const std::string_view varName, const VarType varValue)
        {
            this->LogVarSourceInternal(this->GetLevelPointer<level>(),
                                       level, varName, varValue);
        }
        ///Logs a pair of a variable name and its value and source of the LOG command
        template<u32 level, typename VarType>
        inline void LogVarSource(const std::string_view fileName ,const u32 lineNum, const std::string_view varName, const VarType varValue)
        {
            this->LogVarSourceInternal(this->GetLevelPointer<level>(),
                                       level, fileName, lineNum, varName, varValue);
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
        template<typename VarType>
        inline void LogVarInternal(LogWriterInterface POINTER logger, const u32 level, const std::string_view varName, const VarType varValue)
        {
            logger->LogVar(level, varName, this->InterpretSingleArgument(varValue));
        }

        template<typename VarType>
        inline void LogVarSourceInternal(LogWriterInterface POINTER logger, const u32 level, const std::string_view fileName ,const u32 lineNum, const std::string_view varName, const VarType varValue)
        {
            logger->LogVarSource(level, fileName, lineNum, varName, this->InterpretSingleArgument(varValue));
        }

        template<typename...ArgumentTypes>
        inline void LogMsgInternal(LogWriterInterface POINTER logger, const u32 level, const ArgumentTypes ... args)
        {
            logger->LogMsg(level, this->InterpretConcatenateArguments(args...));
        }

        template<typename...ArgumentTypes>
        inline void LogMsgSourceInternal(LogWriterInterface POINTER logger, const u32 level, const std::string_view fileName, const u32 lineNum, const ArgumentTypes ... args)
        {
            logger->LogMsgSource(level, fileName, lineNum, this->InterpretConcatenateArguments(args...));
        }

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
            this->RebuildLevelPointerArray();
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
            constexpr bool dummy = AssertIfIndexIsOutOfRange<level>();
            (void)dummy; //Dummy is to force the function to be evaluated at compile time

            this->LevelStatusArray[level] = onOff;
            if(onOff)
                this->LevelPointerArray[level] = this->GetOperativeLogWriter();
            else
                this->LevelPointerArray[level] = this->GetPassiveLogWriter();
        }
        inline void DoLoggingLevel(const u32 level, const bool onOff)
        {
            ThrowIfIndexIsOutOfRange(level);

            this->LevelStatusArray[level] = onOff;
            if(onOff)
                this->LevelPointerArray[level] = this->GetOperativeLogWriter();
            else
                this->LevelPointerArray[level] = this->GetPassiveLogWriter();
        }

        ///Returns if logging is disabled
        inline bool IsLoggingEnabled() const {return this->LoggingStatus;} noexcept
        ///Returns if logging is for a concrete level disabled
        inline bool IsLoggingLevelEnabled(const u32 level) const
        {
            ThrowIfIndexIsOutOfRange(level);

            return this->LevelStatusArray[level];
        }
        ///Returns if logging is for a concrete level disabled
        template <u32 level>
        inline bool IsLoggingLevelEnabled() const
        {
            constexpr bool dummy = AssertIfIndexIsOutOfRange<level>();
            (void)dummy; //Dummy is to force the function to be evaluated at compile time

            return this->LevelStatusArray[level];
        }


        ///Changes the LogWriters for a pair of new ones; The old LogWriters will be destroyed
        void ChangeWriters(std::unique_ptr<LogWriterInterface> operativeLogWriter,
                          std::unique_ptr<LogWriterInterface> passiveLogWriter)
        {
            ChangeOperativeWriter(std::move(operativeLogWriter));
            ChangePassiveWriter(std::move(passiveLogWriter));
        }
        template <typename OperativeLogWriterType,  //OperativeLogWriterType must be a derived class from LogWriterInterface
                  typename PassiveLogWriterType,    //PassiveLogWriterType must be a derived clas from LogWriterInterface
                  std::enable_if_t<std::is_base_of<LogWriterInterface, OperativeLogWriterType>::value &&
                                   std::is_base_of<LogWriterInterface, PassiveLogWriterType>::value, int> = 0>
        void ChangeWriters()
        {
            ChangeOperativeWriter<OperativeLogWriterType>();
            ChangePassiveWriter<PassiveLogWriterType>();
        }

        ///Changes the operative logger core
        void ChangeOperativeWriter(std::unique_ptr<LogWriterInterface> operativeLogWriter)
        {
            this->ChangeUsedWriter(std::move(operativeLogWriter), this->OperativeLogWriter);
        }       
        template <typename LogWriterType, //LogWriterType must be a derived class from LogWriterInterface
                  std::enable_if_t<std::is_base_of<LogWriterInterface, LogWriterType>::value, int> = 0>
        inline void ChangeOperativeWriter()
        {
            this->ChangeOperativeWriter(ThisType::GetNewLogWritter<LogWriterType>());
        }

        ///Changes the passive logger core
        void ChangePassiveWriter(std::unique_ptr<LogWriterInterface> passiveLogWriter)
        {
            this->ChangeUsedWriter(std::move(passiveLogWriter), this->PassiveLogWriter);
        }
        template <typename LogWriterType, //LogWriterType must be a derived class from LogWriterInterface
                  std::enable_if_t<std::is_base_of<LogWriterInterface, LogWriterType>::value, int> = 0>
        inline void ChangePassiveWriter()
        {
            this->ChangePassiveWriter(ThisType::GetNewLogWritter<LogWriterType>());
        }

        ///Changes to the default loggers (DefaulOperativetLogWriterType, DefaultPassiveLogWriterType)
        void ChangeToDefaultWriters()
        {
            this->ChangeWriters(ThisType::GetNewDefaultOperativeWriter(), ThisType::GetNewDefaultPassiveWriter());
        }

    public:
        template <typename LogWriterType, //LogWriterType must be a derived class from LogWriterInterface
                  std::enable_if_t<std::is_base_of<LogWriterInterface, LogWriterType>::value, int> = 0>
        static inline std::unique_ptr<LogWriterInterface> GetNewLogWritter()
        {
            return std::make_unique<LogWriterType>();
        }
        static inline std::unique_ptr<LogWriterInterface> GetNewDefaultOperativeWriter()
        {
            return ThisType::GetNewLogWritter<DefaultOperativeLogWriterType>();
        }
        static inline std::unique_ptr<LogWriterInterface> GetNewDefaultPassiveWriter()
        {
            return ThisType::GetNewLogWritter<DefaultPassiveLogWriterType>();
        }

    private:
        inline void ExchangeWriters(std::unique_ptr<LogWriterInterface> newWriter, std::unique_ptr<LogWriterInterface> PASS_REF oldWriter) const
        {
            oldWriter = std::move(newWriter);
        }

        //IS NOT CONST! - Rebuilds the level pointer array
        void ChangeUsedWriter(std::unique_ptr<LogWriterInterface> newWriter, std::unique_ptr<LogWriterInterface> PASS_REF oldWriter)
        {
            ExchangeWriters(std::move(newWriter), oldWriter);
            RebuildLevelPointerArray();
        }




        void RebuildLevelPointerArray()
        {
            //cycles through all of the level logging pointers and sets each of them according to the LevelStatusArray
            for(u32 i = 0; i < ThisType::LevelArraySize; i++)
            {
                if(IsLoggingLevelEnabled(i))
                    this->LevelPointerArray[i] = this->GetOperativeLogWriter();
                else
                    this->LevelPointerArray[i] = this->GetPassiveLogWriter();
            }
        }

        void ResetLevelPointerArray(const bool doLog = true)
        {
            if(doLog)
                SetPointerArray(this->LevelPointerArray, ThisType::LevelArraySize, this->GetOperativeLogWriter());
            else
                SetPointerArray(this->LevelPointerArray, ThisType::LevelArraySize, this->GetPassiveLogWriter());
        }
        inline void SetPointerArray(LogWriterInterface POINTER ptrArray[], const u32 arraySize, LogWriterInterface POINTER value) const noexcept
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

        inline void ReserveStringBuffer(const u32 size = InitialStringBufferSize)
        {
            this->StringBuffer.reserve(size);
        }
        inline void ReserveTempString(const u32 size = InitialTempStringSize)
        {
            this->TempString.reserve(size);
        }

        inline void ResetStringBuffer()
        {
            this->StringBuffer.clear();
        }       
        inline void ResetTempString()
        {
            this->TempString.clear();
        }

        inline void AppendToStringBuffer(const std::string REF str)
        {
            this->StringBuffer.append(str);
        }

        void SetUp(std::unique_ptr<LogWriterInterface> operativeLogWriter,
                   std::unique_ptr<LogWriterInterface> passiveLogWriter,
                   const u32 StringBufferSize = InitialStringBufferSize,
                   const u32 TempStringSize = InitialTempStringSize)
        {
            this->ChangeWriters(std::move(operativeLogWriter), std::move(passiveLogWriter));
            this->DoLog(true);
            this->ResetLevelArrays();
            this->ReserveStringBuffer(StringBufferSize);
            this->ReserveTempString(TempStringSize);
            this->ResetStringBuffer();
            this->ResetTempString();
        }

        inline void SetUp()
        {
            this->SetUp(ThisType::GetNewDefaultOperativeWriter(), ThisType::GetNewDefaultPassiveWriter(),
                        ThisType::InitialStringBufferSize, ThisType::InitialTempStringSize);
        }


        template<typename ArgumentType>
        inline std::string REF InterpretSingleArgument(const ArgumentType argument)
        {
            this->Interpret.InetrpretArg(argument, this->StringBuffer);
            return this->StringBuffer;
        }
        template<typename...ArgumentTypes>
        inline std::string REF InterpretConcatenateArguments(const ArgumentTypes... args)
        {
            this->ResetTempString();
            this->UnravelAndConcatenateArguments(args...);
            return this->StringBuffer;
        }

        //unravel
        template<typename FirstArgument, typename...ArgumentTypes>
        inline void UnravelAndConcatenateArguments(const FirstArgument firstArg, const ArgumentTypes... args)
        {
            this->Interpret.InetrpretArg(firstArg, this->TempString);
            this->AppendToStringBuffer(this->TempString);
            this->UnravelAndConcatenateArguments(args...);
        }
        inline void UnravelAndConcatenateArguments() {}

        
        template<typename FirstStringArgument, typename FirstValueArgument,
                 typename ... StringArgumentTypes, typename ... ValueArgumentTypes>
        inline void UnravelAndSendVariables(const FirstStringArgument firstString, const FirstValueArgument firstValue,
                                            const StringArgumentTypes ... stringArgs, const ValueArgumentTypes ... valueArgs)
        {
            this->ResetStringBuffer();
            this->Interpret.InetrpretArg(firstValue, this->StringBuffer);
            this->OperativeLogWriter->LogByPartsVar(firstString, this->StringBuffer);
            this->UnravelAndSendVariables(stringArgs..., valueArgs...);
        }
        inline void UnravelAndSendVariables() {}


        template<bool idnetifier, u32 firstLevel, u32 ... levels>
        inline void UnravelAndSetLoggingLevel(const bool onOff)
        {
            this->DoLoggingLevel<firstLevel>(onOff);
            UnravelAndSetLoggingLevel<idnetifier, levels...>(onOff);
        }
        template<bool idnetifier>
        inline void UnravelAndSetLoggingLevel(const bool) {} noexcept


        template<typename FirstLevelType, typename ... LevelTypes>
        inline void UnravelAndSetLoggingLevel(const bool onOff, const FirstLevelType firstLevel, const LevelTypes ... levels)
        {
            this->DoLoggingLevel(firstLevel, onOff);
            UnravelAndSetLoggingLevel(onOff, levels...);
        }
        inline void UnravelAndSetLoggingLevel(const bool) noexcept {}

    public:
        static constexpr bool IsIndexInLevelArrayRange(const u32 num)
        {
            return (num < ThisType::LevelArraySize);
        }
        ///Returns the First (currently in use) logger core
        inline LogWriterInterface POINTER GetOperativeLogWriter() const noexcept {return OperativeLogWriter.get();}
        ///Returns the Second (currently not in use) logger core
        inline LogWriterInterface POINTER GetPassiveLogWriter() const noexcept  {return PassiveLogWriter.get();}
        #include <stdexcept>
        template <u32 level>
        inline LogWriterInterface POINTER GetLevelPointer() const
        {
            //Makes the level checking evaluate at compile time
            constexpr bool dummy = AssertIfIndexIsOutOfRange<level>();
            (void)dummy; //Surpresses unused variable warning
            return GetLevelPointerInternal(level);
        }
        inline LogWriterInterface POINTER GetLevelPointer(const u32 level) const
        {
            ThrowIfIndexIsOutOfRange(level);
            return GetLevelPointerInternal(level);
        }

    private:
        static constexpr bool ThrowIfIndexIsOutOfRange(const u32 num)
        {
            if(num < ThisType::LevelArraySize)
                return true;
            else
            {
                throw std::runtime_error("Logger : The given log level is out of array Range: " + std::to_string(ThisType::LevelArraySize));
                return false;
            }
        }
        template<u32 index>
        static constexpr bool AssertIfIndexIsOutOfRange()
        {
            static_assert(index < ThisType::LevelArraySize);
            return (index < ThisType::LevelArraySize); //return is for dummy constexpr variables
        }
        inline LogWriterInterface POINTER GetLevelPointerInternal(const u32 level) const {return LevelPointerArray[level];}
};

#include "LogDefines.h"



#endif // G_LOG_H
