#ifndef STATICLOGTESTERCLASSES_H
#define STATICLOGTESTERCLASSES_H

#include "StaticLog.h"

namespace StaticLog
{
    namespace StaticLogTesting
    {
        struct TestingDataPackage1
        {
            StringType FileName = "";
            u32 LineNum = 0;
            StringType Tags = "";
            StringType LastFunctionCalled = "";
            StringType Identifier = "TestingDataPackage";
            bool StreamState = false;
            StringType POINTER OnDestructionWriteLocation = nullptr;
        };

        struct TestingLogger1 : public LoggerInterface<TestingLogger1, TestingDataPackage1>
        {
            public:
                using ThisPackageType = StaticLogTesting::TestingDataPackage1;
            public:
                using StringType = ::std::string;
                using StringViewType = ::std::string_view;

                template<typename ... MsgTypes>
                void LogMsgsOverload(MsgTypes ... )             {this->LastFunctionCalled = "LogMsgs";}
                template<typename ... NameAndValueTypes>
                void LogVarsOverload(NameAndValueTypes ...)     {this->LastFunctionCalled = "LogVars";}

                template<typename ... MsgTypes>
                void LogMsgsSourceOverload(StringViewType file, const u32 lineNum, const u32, MsgTypes ... )
                {
                    this->LastFunctionCalled = "LogMsgsSource";
                    this->FileName = file;
                    this->LineNum = lineNum;
                }
                template<typename ... NameAndValueTypes>
                void LogVarsSourceOverload(StringViewType file, const u32 lineNum, const u32, NameAndValueTypes ...)
                {
                    this->LastFunctionCalled = "LogVarsSource";
                    this->FileName = file;
                    this->LineNum = lineNum;
                }

                template<typename ... TagTypes>
                void AppendTagsOverload(TagTypes ...)                   {this->LastFunctionCalled = "AppendTags";}

            public:
                TestingLogger1() = default;
        };

        struct TestingDataPackage2
        {
            public:
            int MyInt1 = 1;
            int MyInt2 = 2;
            int MyInt3 = 3;
            StringType LastFunctionCalled = "";
            StringType Identifier = "TestingDataPackage2";
            bool StreamState = false;
        };

        struct TestingLogger2 : public LoggerInterface<TestingLogger2, TestingDataPackage2>
        {
            public:
                using ThisPackageType = StaticLogTesting::TestingDataPackage2;
            public:
                using StringType = ::std::string;
                using StringViewType = ::std::string_view;

                template<typename ... MsgTypes>
                void LogMsgsOverload(const u32, MsgTypes ... )             {this->LastFunctionCalled = "LogMsgs";}
                template<typename ... NameAndValueTypes>
                void LogVarsOverload(const u32, NameAndValueTypes ...)     {this->LastFunctionCalled = "LogVars";}

                template<typename ... MsgTypes>
                void LogMsgsSourceOverload(StringViewType, const u32, const u32, MsgTypes ... )
                {
                    this->LastFunctionCalled = "LogMsgsSource";
                }
                template<typename ... NameAndValueTypes>
                void LogVarsSourceOverload(StringViewType, const u32, const u32, NameAndValueTypes ...)
                {
                    this->LastFunctionCalled = "LogVarsSource";
                }

                template<typename ... AdditionalTypes>
                void AppendSourceOverload(StringViewType, const u32)
                {
                    this->LastFunctionCalled = "AppendSource";
                }
                template<typename ... TagTypes>
                void AppendTagsOverload(TagTypes ...)                   {this->LastFunctionCalled = "AppendTags";}


            public:
                TestingLogger2() = default;
        };



        struct TestingDataPackageMove
        {
            u32 LineNum = 0;
            StringType Str = "";

            TestingDataPackageMove(StringViewType str) : Str(str) {}
            TestingDataPackageMove(u32 lineNum, StringViewType str) : LineNum(lineNum), Str(str) {}
            TestingDataPackageMove() = delete;
            TestingDataPackageMove(const TestingDataPackageMove REF) = delete;
            TestingDataPackageMove(TestingDataPackageMove RVALUE_REF other)
            {
                //Swapping is to make the fact that the move really happened visbible while testing
                std::swap(this->LineNum, other.LineNum);
                this->Str.swap(other.Str);
            };

            //Assignment
            TestingDataPackageMove REF operator=(TestingDataPackageMove RVALUE_REF other)
            {
                //Swapping is to make the fact that the move really happened visbible while testing
                std::swap(this->LineNum, other.LineNum);
                this->Str.swap(other.Str);
                return POINTER_VALUE(this);
            }
            TestingDataPackageMove REF operator=(const TestingDataPackageMove REF) = delete ;

        };

        struct TestingLoggerMove : public LoggerInterface<TestingLoggerMove, TestingDataPackageMove>
        {
                using ThisPackageType = StaticLogTesting::TestingDataPackageMove;
                using LoggerInterfaceType = LoggerInterface<TestingLoggerMove, TestingDataPackageMove>;

                void LogMsgsOverload(const u32) {}
                void LogVarsOverload(const u32) {}
                void LogMsgsSourceOverload(StringViewType, const u32, const u32) {}
                void LogVarsSourceOverload(StringViewType, const u32, const u32) {}
                void AppendSourceOverload() {}
                void AppendTagsOverload() {}
                TestingLoggerMove() = delete;
                TestingLoggerMove(const TestingLoggerMove REF) = delete;
                TestingLoggerMove(TestingLoggerMove RVALUE_REF) = default;

                TestingLoggerMove REF operator=(TestingLoggerMove RVALUE_REF other) = default;
                TestingLoggerMove REF operator=(const TestingLoggerMove REF) = delete ;
        };

        struct TestingDataPackageCopy
        {
            u32 LineNum = 0;
            StringType Str = "";

            TestingDataPackageCopy(StringViewType str) : Str(str) {}
            TestingDataPackageCopy(u32 lineNum, StringViewType str) : LineNum(lineNum), Str(str) {}
            TestingDataPackageCopy() = delete;
            TestingDataPackageCopy(const TestingDataPackageCopy REF) = default;
            TestingDataPackageCopy(TestingDataPackageCopy RVALUE_REF) = delete;

            //Assignment
            TestingDataPackageCopy REF operator=(TestingDataPackageCopy RVALUE_REF) = delete;
            TestingDataPackageCopy REF operator=(const TestingDataPackageCopy REF) = default;

        };

        struct TestingLoggerCopy : public LoggerInterface<TestingLoggerCopy, TestingDataPackageCopy>
        {
                using ThisPackageType = StaticLogTesting::TestingDataPackageCopy;
                using LoggerInterfaceType = LoggerInterface<TestingLoggerCopy, TestingDataPackageCopy>;

                void LogMsgsOverload(const u32) {}
                void LogVarsOverload(const u32) {}
                void LogMsgsSourceOverload(StringViewType, const u32, const u32) {}
                void LogVarsSourceOverload(StringViewType, const u32, const u32) {}
                //void AppendSourceOverload() {}
                void AppendTagsOverload() {}
                TestingLoggerCopy() = delete;
                TestingLoggerCopy(const TestingLoggerCopy REF) = default;
                TestingLoggerCopy(TestingLoggerCopy RVALUE_REF) = delete;

                TestingLoggerCopy REF operator=(TestingLoggerCopy RVALUE_REF) = delete;
                TestingLoggerCopy REF operator=(const TestingLoggerCopy REF other) = default;
        };


        struct TestingDataPackageConstruct
        {
            u32 LineNum = 0;
            StringType Str = "";

            TestingDataPackageConstruct(StringViewType str) : Str(str) {}
            TestingDataPackageConstruct(u32 lineNum, StringViewType str) : LineNum(lineNum), Str(str) {}
            TestingDataPackageConstruct() = default;
            TestingDataPackageConstruct(const TestingDataPackageConstruct REF) = default;
            TestingDataPackageConstruct(TestingDataPackageConstruct RVALUE_REF) = delete;

            //Assignment
            TestingDataPackageConstruct REF operator=(TestingDataPackageConstruct RVALUE_REF) = delete;
            TestingDataPackageConstruct REF operator=(const TestingDataPackageConstruct REF) = default;

        };

        struct TestingLoggerConstruct : public LoggerInterface<TestingLoggerConstruct, TestingDataPackageConstruct>
        {
                using ThisPackageType = StaticLogTesting::TestingDataPackageConstruct;
                using LoggerInterfaceType = LoggerInterface<TestingLoggerConstruct, TestingDataPackageConstruct>;

                void LogMsgsOverload(const u32) {}
                void LogVarsOverload(const u32) {}
                void LogMsgsSourceOverload(StringViewType, const u32, const u32) {}
                void LogVarsSourceOverload(StringViewType, const u32, const u32) {}
                void AppendTagsOverload() {}

                TestingLoggerConstruct() = default;
                TestingLoggerConstruct(const TestingLoggerConstruct REF) = default;
                TestingLoggerConstruct(TestingLoggerConstruct RVALUE_REF) = delete;

                TestingLoggerConstruct REF operator=(TestingLoggerConstruct RVALUE_REF) = delete;
                TestingLoggerConstruct REF operator=(const TestingLoggerConstruct REF other) = default;
        };

    }

    namespace StaticLogTesting
    {
        class DefaultLoggerPackageTester : public DefaultLoggerPackage<>
        {
            public:
                using DefaultLoggerPackage::Interpret;
                using DefaultLoggerPackage::Formater;
                using DefaultLoggerPackage::File;
                using DefaultLoggerPackage::CollectionString;
                using DefaultLoggerPackage::TempString;
                using DefaultLoggerPackage::Tags;
                using DefaultLoggerPackage::EntryIndex;

                using DefaultLoggerPackage::SetUp;
                using DefaultLoggerPackage::ReserveStrings;

                using DefaultLoggerPackage::MinimumCollectionStringSize;
                using DefaultLoggerPackage::MinimumTempStringSize;
                using DefaultLoggerPackage::MinimumTagsSize;


            public:
                DefaultLoggerPackageTester(const StringType filePath) : DefaultLoggerPackage(filePath) {}
                DefaultLoggerPackageTester(DefaultLoggerPackageTester RVALUE_REF) = default;
                DefaultLoggerPackageTester REF operator=(DefaultLoggerPackageTester RVALUE_REF) = default;

                DefaultLoggerPackageTester(const DefaultLoggerPackageTester REF) = delete;
                void operator=(const DefaultLoggerPackageTester REF) = delete ;

        };

        DefaultLoggerPackageTester REF GetDefPackageTester(DefaultLoggerPackage<> REF package)
        {
            return static_cast<DefaultLoggerPackageTester REF>(package);
        }
    }

    namespace StaticLogTesting
    {
        template<typename InterpretType = DefaultDataInterpret, bool DoAppOutput = true>
        struct DefaultLoggerTester : public DefaultLogger<InterpretType, DoAppOutput>
        {
                using DefaultLoggerType = DefaultLogger<InterpretType, DoAppOutput>;
            public:
                using DefaultLoggerType::Interpret;
                using DefaultLoggerType::Formater;
                using DefaultLoggerType::File;
                using DefaultLoggerType::CollectionString;
                using DefaultLoggerType::TempString;
                using DefaultLoggerType::Tags;
                using DefaultLoggerType::EntryIndex;
                using DefaultLoggerType::AppOutputLogStatus;

                using DefaultLoggerType::LogIterationFieldSize;
                using DefaultLoggerType::LogClockFieldSize;
                using DefaultLoggerType::Separator;
                using DefaultLoggerType::DoAppOutputLog;

                using DefaultLoggerType::LogMsgsOverload;
                using DefaultLoggerType::LogVarsOverload;
                using DefaultLoggerType::AppendTagsOverload;

                using DefaultLoggerType::AddSource;
                using DefaultLoggerType::AddIterations;
                using DefaultLoggerType::AddTime;
                using DefaultLoggerType::AddLvl;
                using DefaultLoggerType::AddMsgs;
                using DefaultLoggerType::AddVars;
                using DefaultLoggerType::AddTags;

                using DefaultLoggerType::IncrementEntryIndex;
                using DefaultLoggerType::PushLogOut;
                using DefaultLoggerType::AfterLog;
                using DefaultLoggerType::CleanUp;

                using DefaultLoggerType::UnravelAndAddMsgs;
                using DefaultLoggerType::UnravelAndAddVariables;
                using DefaultLoggerType::UnravelAndAddTags;

                using DefaultLoggerType::AddFormatedTime;
                using DefaultLoggerType::AddFormatedSource;
                using DefaultLoggerType::AddFormatedIterationCount;
                using DefaultLoggerType::AddFormatedLevel;
                using DefaultLoggerType::AddFormatedMsg;
                using DefaultLoggerType::AddUnformatedMsgPart;
                using DefaultLoggerType::AddFormatedTag;
                using DefaultLoggerType::AddFormatedVariable;
                using DefaultLoggerType::AddFormatedVariableAsListPart;

                using DefaultLoggerType::AddSeparator;
                using DefaultLoggerType::AddNewline;
                using DefaultLoggerType::PutNumIntoCharcterField;
                using DefaultLoggerType::ResetString;
        };

        template<typename InterpretType, bool DoAppOutput>
        DefaultLoggerTester<InterpretType, DoAppOutput> REF
            GetDefLoggerTester(LoggerInterface<DefaultLogger<InterpretType, DoAppOutput>> REF logger)
        {
            return static_cast<DefaultLoggerTester<InterpretType, DoAppOutput> REF>(logger.GetWriter());
        }
        template<typename InterpretType, bool DoAppOutput>
        DefaultLoggerTester<InterpretType, DoAppOutput> REF
            GetDefLoggerTester(DefaultLogger<InterpretType, DoAppOutput> REF logger)
        {
            return static_cast<DefaultLoggerTester<InterpretType, DoAppOutput> REF>(logger);
        }
    }
}
#endif // STATICLOGTESTERCLASSES_H
