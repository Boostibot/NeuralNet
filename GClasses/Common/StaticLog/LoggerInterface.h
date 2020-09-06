#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

#include "GClasses/Common/G_Common.h"

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

template<typename SameTo, typename ... ArgumentTypes>
struct AreTypesSame
{
        template<bool indetifier, typename FirstType, typename ... OtherArgumentTypes>
        constexpr static bool RecursiveCall()
        {
            if(std::is_same<FirstType, SameTo>::value)
                return true;
            else
                return RecursiveCall<indetifier, OtherArgumentTypes...>();
        }
        template<bool indetifier>
        constexpr static bool RecursiveCall()
        {
            return false;
        }

        static constexpr bool value = RecursiveCall<true, ArgumentTypes...>();

};

template<typename ... NameAndValueTypes>
constexpr bool AreEven() {return (sizeof...(NameAndValueTypes) % 2) == 0; }

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
            LoggerInterface(const ThisType PASS_REF other) : DerivedDataPackage(other)
            {
                this->LevelLoggingStatus = other.LevelLoggingStatus;
                this->LoggingStatus = other.LoggingStatus;
                //this->GetWriter().OnConstruction();
            }
            LoggerInterface(ThisType PASS_RVALUE_REF other) : DerivedDataPackage(std::move(other))
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
                     std::enable_if_t<!(AreTypesSame<ThisType PASS_REF, ArgumentTypes...>::value && (sizeof...(ArgumentTypes) == 1)), int> = 0
                     >
            LoggerInterface(ArgumentTypes PASS_RVALUE_REF ... args) : DerivedDataPackage(std::forward<ArgumentTypes>(args)...)
            {
                //TypeViewer(std::forward<ArgumentTypes>(args)...);
                this->SetUp();
            }
            ~LoggerInterface()
            {
                //GetWriter().OnDestruction();
            }

            //template<typename FirstType, typename... ArgTypes>
            //void TypeViewer(FirstType PASS_RVALUE_REF first, ArgTypes PASS_RVALUE_REF... args)
            //{
            //    (void)first;
            //    std::string name = typeid (FirstType).name();
            //    (void)name;
            //    TypeViewer(std::forward<ArgTypes>(args)...);
            //}
            //void TypeViewer(){}

        public:
            ThisType REF operator=(ThisType PASS_RVALUE_REF) = default;
            ThisType REF operator=(const ThisType PASS_REF) = default;


            //Overloaded functions - begin
        public:
            template<u32 level, typename ... MsgTypes>
            void LogMsgs(MsgTypes ... msgs)
            {
                if(this->IsLogEnabled<level>())
                    GetWriter().LogMsgsOverload(level, msgs...);
            }
            template<u32 level, typename ... NameAndValueTypes>
            void LogVars(NameAndValueTypes ... namesAndValues)
            {
                static_assert (AreEven<NameAndValueTypes...>(), "LoggerInterface: LogVars requires even number of arguments");

                if(this->IsLogEnabled<level>())
                    GetWriter().template LogVarsOverload<NameAndValueTypes...>(level, namesAndValues...);
            }

            template<u32 level, typename ... MsgTypes>
            void LogMsgsSource(StringViewType file, const u32 lineNum, MsgTypes ... msgs)
            {
                if(this->IsLogEnabled<level>())
                    GetWriter().LogMsgsSourceOverload(file, lineNum, level, msgs...);
            }
            template<u32 level, typename ... NameAndValueTypes>
            void LogVarsSource(StringViewType file, const u32 lineNum, NameAndValueTypes ... namesAndValues)
            {
                static_assert (AreEven<NameAndValueTypes...>(), "LoggerInterface: LogVars requires even number of arguments");

                if(this->IsLogEnabled<level>())
                    GetWriter().template LogVarsSourceOverload<NameAndValueTypes...>(file, lineNum, level, namesAndValues...);
            }

            /*
            template<u32 level, typename ... AdditionalTypes>
            void AppendSource(StringViewType file, const u32 lineNum, AdditionalTypes ... additional)
            {
                if(this->IsLogEnabled<level>())
                    GetWriter().AppendSourceOverload(file, lineNum, additional...);
            }
            */

            template<u32 level, typename ... TagTypes>
            void AppendTags(TagTypes ... tags)
            {
                if(this->IsLogEnabled<level>())
                    GetWriter().AppendTagsOverload(tags...);
            }

            /*
            template<typename ... PathTypes>
            void OpenStream(PathTypes ... paths)
            {
                GetWriter().OpenStreamOverload(paths...);
            }
            template<typename ... StreamIdentifierTypes>
            void CloseStream(StreamIdentifierTypes ... indetifiers)
            {
                GetWriter().CloseStreamOverload(indetifiers...);
            }
            template<typename ... StreamIdentifierTypes>
            bool IsStreamOpen(StreamIdentifierTypes ... indetifiers)
            {
                return GetWriter().IsStreamOpenOverload(indetifiers...);
            }
            template<typename ... ArgTypes>
            void FlushStream(ArgTypes ... args)
            {
                GetWriter().FlushStreamOverload(args...);
            }

        private:
            void OnConstruction()
            {
                GetWriter().OnConstructionOverload();
            }
            void OnDestruction()
            {
                GetWriter().OnDestructionOverload();
            }
            */
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
                //GetWriter().OnConstruction();
            }
    };
}
#endif // LOGGERINTERFACE_H
