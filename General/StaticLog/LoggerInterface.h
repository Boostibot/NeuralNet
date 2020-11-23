#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

#include "General/Common/Common.h"

struct LogWriterTraits
{
        template<typename LoggerT>
        static constexpr bool IsLogWriter()
        {
            return std::is_base_of<LogWriterTraits, LoggerT>();
        }
        template<typename TemplateSpecialisationType, template<typename ... ClassTypes> class LogWriterTemplate>
        static constexpr bool IsLogWriter()
        {
            return std::is_base_of<LogWriterTraits, LogWriterTemplate<TemplateSpecialisationType>>();
        }
};

struct LogTraits
{
        template<typename LoggerT>
        static constexpr bool IsLogger()
        {
            return std::is_base_of<LogTraits, LoggerT>();
        }
        template<typename TemplateSpecialisationType, template<typename ... ClassTypes> class LoggerTemplate>
        static constexpr bool IsLogger()
        {
            return std::is_base_of<LogTraits, LoggerTemplate<TemplateSpecialisationType>>();
        }
};

namespace MetaPrograming 
{  
    template<typename ... NameAndValueTypes>
    constexpr bool AreEven() {return (sizeof...(NameAndValueTypes) % 2) == 0; }
    
    template<typename OfType, typename ... Arguements>
    constexpr bool IsConstructorWithArgumentsNotCopyConstructor() 
    {
        return NOT (MetaPrograming::IsTypePresent<OfType REF, Arguements...>::value && (sizeof...(Arguements) == 1)); 
    }        
}

#include "Libraries/Fmt/fmt/compile.h"
#include "Libraries/Fmt/fmt/core.h"
#include "Libraries/Fmt/fmt/os.h"

#include <array>
namespace StaticLog
{
    using StringType = ::std::string;
    using StringViewType = ::std::string_view;


    template <typename Derived,
              typename DerivedDataPackage = typename Derived::ThisPackageType>
    class LoggerInterface : public DerivedDataPackage, public LogWriterTraits
    {
        public:
            using StringType = ::std::string;
            using StringViewType = ::std::string_view;
            using ThisType = LoggerInterface<Derived, DerivedDataPackage>;

        public:
            static constexpr u32 LevelCount = 16; //This should be equal to the max number of levels suported
            static constexpr u32 MaxLevelIndex = LevelCount - 1;
            //static constexpr u32 InitialStringBufferSize = 256;
            //static constexpr u32 InitialTempStringSize = 128;

        private:
            std::array<bool, LevelCount> LevelLoggingStatus;
            bool LoggingStatus = true;

        public:
            Derived REF GetWriter() {return POINTER_VALUE(static_cast<Derived POINTER>(this));}

        public:
            LoggerInterface() : DerivedDataPackage()
            {
                this->SetUp();
            }
            LoggerInterface(const ThisType REF other) : DerivedDataPackage(other)
            {
                this->LevelLoggingStatus = other.LevelLoggingStatus;
                this->LoggingStatus = other.LoggingStatus;
                //this->GetWriter().OnConstruction();
            }
            LoggerInterface(ThisType RVALUE_REF other) : DerivedDataPackage(std::move(other))
            {
                //Move constructor just copies instead of swapping because its faster
                this->LevelLoggingStatus = other.LevelLoggingStatus;
                this->LoggingStatus = other.LoggingStatus;
                //this->GetWriter().OnConstruction();
            }

            //Any other constructor for the DerivedDataPackage
            template<typename ... ArgumentTypes ,
                     //Checks assure that the ArgumentTypes are not identical to the ones of copy and move constructor
                     //This is to resolve the ambiguous function call
                     //Checks if any of the types arent same as the contructor types but only blocks the function if the type is alone (sizeof...(ArgumentTypes) == 1)
                     std::enable_if_t<MetaPrograming::IsConstructorWithArgumentsNotCopyConstructor<ThisType, ArgumentTypes...>(), int> = 0
                     >
            LoggerInterface(ArgumentTypes RVALUE_REF ... args) : DerivedDataPackage(std::forward<ArgumentTypes>(args)...)
            {
                //TypeViewer(std::forward<ArgumentTypes>(args)...);
                this->SetUp();
            }
            ~LoggerInterface()
            {
                //GetWriter().OnDestruction();
            }

            //template<typename FirstType, typename... ArgTypes>
            //void TypeViewer(FirstType RVALUE_REF first, ArgTypes RVALUE_REF... args)
            //{
            //    (void)first;
            //    std::string name = typeid (FirstType).name();
            //    (void)name;
            //    TypeViewer(std::forward<ArgTypes>(args)...);
            //}
            //void TypeViewer(){}

        public:
            ThisType REF operator=(ThisType RVALUE_REF) = default;
            ThisType REF operator=(const ThisType REF) = default;


            //Overloaded functions - begin
        public:
            template<u32 level, typename ... MsgTypes>
            void LogMsgs(MsgTypes RVALUE_REF ... msgs)
            {
                if(this->IsLogEnabled<level>())
                    GetWriter().LogMsgsOverload(level, std::forward<MsgTypes>(msgs)...);
            }
            template<u32 level, typename ... NameAndValueTypes>
            void LogVars(NameAndValueTypes RVALUE_REF ... namesAndValues)
            {
                static_assert (MetaPrograming::AreEven<NameAndValueTypes...>(), "LoggerInterface: LogVars requires even number of arguments, ie. name and value pairs");

                if(this->IsLogEnabled<level>())
                    GetWriter().template LogVarsOverload<NameAndValueTypes...>(level, std::forward<NameAndValueTypes>(namesAndValues)...);
            }

            template<u32 level, typename ... MsgTypes>
            void LogMsgsSource(StringViewType file, const u32 lineNum, MsgTypes RVALUE_REF ... msgs)
            {
                if(this->IsLogEnabled<level>())
                    GetWriter().LogMsgsSourceOverload(file, lineNum, level, std::forward<MsgTypes>(msgs)...);
            }
            template<u32 level, typename ... NameAndValueTypes>
            void LogVarsSource(StringViewType file, const u32 lineNum, NameAndValueTypes RVALUE_REF ... namesAndValues)
            {
                static_assert (MetaPrograming::AreEven<NameAndValueTypes...>(), "LoggerInterface: LogVars requires even number of arguments, ie. name and value pairs");

                if(this->IsLogEnabled<level>())
                    GetWriter().template LogVarsSourceOverload<NameAndValueTypes...>(file, lineNum, level, std::forward<NameAndValueTypes>(namesAndValues)...);
            }
            template<u32 level, typename ... TagTypes>
            void AppendTags(TagTypes RVALUE_REF ... tags)
            {
                if(this->IsLogEnabled<level>())
                    GetWriter().AppendTagsOverload(std::forward<TagTypes>(tags)...);
            }
            //Overloaded functions - end


        public:
            static constexpr bool IsLevelInRange(const u32 level)
            {
                return (level < ThisType::LevelCount);
            }
            inline void DoLog(const bool onOff)
            {
                LoggingStatus = onOff;
            }
            inline void EnableLog()
            {
                this->DoLog(true);
            }
            inline void DisableLog()
            {
                this->DoLog(false);
            }

            template <u32 ... levels>
            void DoLoggingLevels(const bool onOff)
            {
                //Checks all levels
                constexpr bool dummy = ThisType::UnravelAndCheckLevel_Assert<true, levels...>(); //Forces the function to be evaluated at compile time
                (void)dummy;

                //Sets them
                this->UnravelAndSetLoggingLevel<true, levels...>(onOff);
                //true is identifier used to identify the variadic template - is there another way??
            }
            template <typename ... LevelTypes>
            void DoLoggingLevels(const bool onOff, const LevelTypes ... levels)
            {
                //Checks all levels
                this->UnravelAndCheckLevel_Throw(levels...);

                //Sets them
                this->UnravelAndSetLoggingLevel(onOff, levels...);
                //Both could be done in a single recursive functions but if invalid arguments are
                //  provided no levels should be set before the invalid argiment is encountered
                //  ie. LoggerInterface.DoLoggingLevels(true, 0, 1, 2, 123, 4) would still set levels 0, 1, 2 and throw
                //  important to notice the level 4 wouldnt be set - this is why this separate checking is necessary
            }

            inline bool IsLogEnabled() const
            {
                return LoggingStatus;
            }
            inline bool IsLogEnabled(const u32 level) const
            {
                this->ThrowIfLevelIsOutOfRange(level);
                return this->IsLogEnabledInternal(level);
            }
            template<u32 level>
            inline bool IsLogEnabled() const
            {
                constexpr bool dummy = ThisType::AssertIfLevelIsOutOfRange<level>(); //Forces the function to be evaluated at compile time
                (void)dummy;
                return this->IsLogEnabledInternal(level);
            }

        private:
            template<u32 level>
            static constexpr bool AssertIfLevelIsOutOfRange()
            {
                static_assert (ThisType::IsLevelInRange(level), "LoggerInterface: The provided level is out of array range;");
                return ThisType::IsLevelInRange(level);
            }
            static void ThrowIfLevelIsOutOfRange(const u32 level)
            {
                if(NOT(ThisType::IsLevelInRange(level)))
                    throw std::runtime_error("LoggerInterface: The provided level is out of array range;");
            }

            inline bool IsLogEnabledInternal(const u32 level) const
            {
                if(IsLogEnabled())
                    return LevelLoggingStatus[level];
                else
                    return false;
            }

            inline void DoLoggingLevelInternal(const u32 level, const bool onOff)
            {
                this->LevelLoggingStatus[level] = onOff;
            }

            //Setting
            template<bool idnetifier, u32 firstLevel, u32 ... levels>
            inline void UnravelAndSetLoggingLevel(const bool onOff)
            {
                this->DoLoggingLevelInternal(firstLevel, onOff);
                UnravelAndSetLoggingLevel<idnetifier, levels...>(onOff);
            }
            template<bool idnetifier>
            inline void UnravelAndSetLoggingLevel(const bool) {}

            template<typename FirstLevelType, typename ... LevelTypes>
            inline void UnravelAndSetLoggingLevel(const bool onOff, const FirstLevelType firstLevel, const LevelTypes ... levels)
            {
                this->DoLoggingLevelInternal(firstLevel, onOff);
                UnravelAndSetLoggingLevel(onOff, levels...);
            }
            inline void UnravelAndSetLoggingLevel(const bool) {}

            //Checking
            template<bool idnetifier, u32 firstLevel, u32 ... levels>
            static constexpr bool UnravelAndCheckLevel_Assert()
            {
                ThisType::AssertIfLevelIsOutOfRange<firstLevel>();
                return UnravelAndCheckLevel_Assert<idnetifier, levels...>();
            }
            template<bool idnetifier>
            static constexpr bool UnravelAndCheckLevel_Assert() {return true;}

            template<typename FirstLevelType, typename ... LevelTypes>
            inline void UnravelAndCheckLevel_Throw(const FirstLevelType firstLevel, const LevelTypes ... levels)
            {
                this->ThrowIfLevelIsOutOfRange(firstLevel);
                UnravelAndCheckLevel_Throw(levels...);
            }
            inline void UnravelAndCheckLevel_Throw() {}
           
            //SetUp
            inline void SetUp()
            {
                LevelLoggingStatus.fill(true);
            }
            
    };
}
#endif // LOGGERINTERFACE_H
