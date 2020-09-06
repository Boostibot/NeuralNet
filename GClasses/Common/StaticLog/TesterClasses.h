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

                /*
                template<typename ... AdditionalTypes>
                void AppendSourceOverload(StringViewType file, const u32 lineNum)
                {
                    this->LastFunctionCalled = "AppendSource";
                    this->FileName = file;
                    this->LineNum = lineNum;
                }
                */
                template<typename ... TagTypes>
                void AppendTagsOverload(TagTypes ...)                   {this->LastFunctionCalled = "AppendTags";}

                /*
                void OpenStreamOverload(StringType)                     {this->LastFunctionCalled = "OpenStream";  this->StreamState = true;}
                void CloseStreamOverload()                              {this->LastFunctionCalled = "CloseStream"; this->StreamState = false;}
                bool IsStreamOpenOverload()                             {this->LastFunctionCalled = "IsStreamOpen"; return this->StreamState;}
                void FlushStreamOverload()                              {this->LastFunctionCalled = "FlushStream";}

                void OnConstructionOverload()                           {this->LastFunctionCalled = "OnConstruction";}
                void OnDestructionOverload()
                {
                    this->LastFunctionCalled = "OnDestruction";
                    if(OnDestructionWriteLocation != nullptr)
                        POINTER_VALUE(OnDestructionWriteLocation) = "OnDestruction";
                }
                */

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

                /*
                void OpenStreamOverload(StringType)                     {this->LastFunctionCalled = "OpenStream";  this->StreamState = true;}
                void CloseStreamOverload()                              {this->LastFunctionCalled = "CloseStream"; this->StreamState = false;}
                bool IsStreamOpenOverload()                             {this->LastFunctionCalled = "IsStreamOpen"; return this->StreamState;}
                void FlushStreamOverload()                              {this->LastFunctionCalled = "FlushStream";}

                void OnConstructionOverload()                           {this->LastFunctionCalled = "OnConstruction";}
                void OnDestructionOverload()                            {this->LastFunctionCalled = "OnDestruction";}
                */

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
            TestingDataPackageMove(const TestingDataPackageMove PASS_REF) = delete;
            TestingDataPackageMove(TestingDataPackageMove PASS_RVALUE_REF other)
            {
                //Swapping is to make the fact that the move really happened visbible while testing
                std::swap(this->LineNum, other.LineNum);
                this->Str.swap(other.Str);
            };

            //Assignment
            TestingDataPackageMove REF operator=(TestingDataPackageMove PASS_RVALUE_REF other)
            {
                //Swapping is to make the fact that the move really happened visbible while testing
                std::swap(this->LineNum, other.LineNum);
                this->Str.swap(other.Str);
                return POINTER_VALUE(this);
            }
            TestingDataPackageMove REF operator=(const TestingDataPackageMove PASS_REF) = delete ;

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
                /*
                void OpenStreamOverload() {}
                void CloseStreamOverload() {}
                bool IsStreamOpenOverload() {return false;}
                void FlushStreamOverload() {}
                void OnConstructionOverload() {}
                void OnDestructionOverload() {}
                */
                TestingLoggerMove() = delete;
                TestingLoggerMove(const TestingLoggerMove PASS_REF) = delete;
                TestingLoggerMove(TestingLoggerMove PASS_RVALUE_REF) = default;

                TestingLoggerMove REF operator=(TestingLoggerMove PASS_RVALUE_REF other) = default;
                TestingLoggerMove REF operator=(const TestingLoggerMove PASS_REF) = delete ;
        };

        struct TestingDataPackageCopy
        {
            u32 LineNum = 0;
            StringType Str = "";

            TestingDataPackageCopy(StringViewType str) : Str(str) {}
            TestingDataPackageCopy(u32 lineNum, StringViewType str) : LineNum(lineNum), Str(str) {}
            TestingDataPackageCopy() = delete;
            TestingDataPackageCopy(const TestingDataPackageCopy PASS_REF) = default;
            TestingDataPackageCopy(TestingDataPackageCopy PASS_RVALUE_REF) = delete;

            //Assignment
            TestingDataPackageCopy REF operator=(TestingDataPackageCopy PASS_RVALUE_REF) = delete;
            TestingDataPackageCopy REF operator=(const TestingDataPackageCopy PASS_REF) = default;

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
                /*
                void OpenStreamOverload() {}
                void CloseStreamOverload() {}
                bool IsStreamOpenOverload() {return false;}
                void FlushStreamOverload() {}
                void OnConstructionOverload() {}
                void OnDestructionOverload() {}
                */
                TestingLoggerCopy() = delete;
                TestingLoggerCopy(const TestingLoggerCopy PASS_REF) = default;
                TestingLoggerCopy(TestingLoggerCopy PASS_RVALUE_REF) = delete;

                TestingLoggerCopy REF operator=(TestingLoggerCopy PASS_RVALUE_REF) = delete;
                TestingLoggerCopy REF operator=(const TestingLoggerCopy PASS_REF other) = default;
        };


        struct TestingDataPackageConstruct
        {
            u32 LineNum = 0;
            StringType Str = "";

            TestingDataPackageConstruct(StringViewType str) : Str(str) {}
            TestingDataPackageConstruct(u32 lineNum, StringViewType str) : LineNum(lineNum), Str(str) {}
            TestingDataPackageConstruct() = default;
            TestingDataPackageConstruct(const TestingDataPackageConstruct PASS_REF) = default;
            TestingDataPackageConstruct(TestingDataPackageConstruct PASS_RVALUE_REF) = delete;

            //Assignment
            TestingDataPackageConstruct REF operator=(TestingDataPackageConstruct PASS_RVALUE_REF) = delete;
            TestingDataPackageConstruct REF operator=(const TestingDataPackageConstruct PASS_REF) = default;

        };

        struct TestingLoggerConstruct : public LoggerInterface<TestingLoggerConstruct, TestingDataPackageConstruct>
        {
                using ThisPackageType = StaticLogTesting::TestingDataPackageConstruct;
                using LoggerInterfaceType = LoggerInterface<TestingLoggerConstruct, TestingDataPackageConstruct>;

                void LogMsgsOverload(const u32) {}
                void LogVarsOverload(const u32) {}
                void LogMsgsSourceOverload(StringViewType, const u32, const u32) {}
                void LogVarsSourceOverload(StringViewType, const u32, const u32) {}
                //void AppendSourceOverload() {}
                void AppendTagsOverload() {}
                /*
                void OpenStreamOverload() {}
                void CloseStreamOverload() {}
                bool IsStreamOpenOverload() {return false;}
                void FlushStreamOverload() {}
                void OnConstructionOverload() {}
                void OnDestructionOverload() {}
                */
                TestingLoggerConstruct() = default;
                TestingLoggerConstruct(const TestingLoggerConstruct PASS_REF) = default;
                TestingLoggerConstruct(TestingLoggerConstruct PASS_RVALUE_REF) = delete;

                TestingLoggerConstruct REF operator=(TestingLoggerConstruct PASS_RVALUE_REF) = delete;
                TestingLoggerConstruct REF operator=(const TestingLoggerConstruct PASS_REF other) = default;
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
                DefaultLoggerPackageTester(DefaultLoggerPackageTester PASS_RVALUE_REF) = default;
                DefaultLoggerPackageTester REF operator=(DefaultLoggerPackageTester PASS_RVALUE_REF) = default;

                DefaultLoggerPackageTester(const DefaultLoggerPackageTester PASS_REF) = delete;
                void operator=(const DefaultLoggerPackageTester PASS_REF) = delete ;

        };

        DefaultLoggerPackageTester REF GetDefPackageTester(DefaultLoggerPackage<> PASS_REF package)
        {
            return static_cast<DefaultLoggerPackageTester REF>(package);
        }
    }

    namespace StaticLogTesting
    {
        struct DefaultLoggerTester : public DefaultLogger<>
        {
            public:
                using DefaultLogger::Interpret;
                using DefaultLogger::Formater;
                using DefaultLogger::File;
                using DefaultLogger::CollectionString;
                using DefaultLogger::TempString;
                using DefaultLogger::Tags;
                using DefaultLogger::EntryIndex;

                using DefaultLogger::LogIterationFieldSize;
                using DefaultLogger::LogClockFieldSize;
                using DefaultLogger::Separator;
                using DefaultLogger::DoAppOutputLog;

                using DefaultLogger::LogMsgsOverload;
                using DefaultLogger::LogVarsOverload;
                using DefaultLogger::AppendTagsOverload;

                using DefaultLogger::AddSource;
                using DefaultLogger::AddIterations;
                using DefaultLogger::AddTime;
                using DefaultLogger::AddLvl;
                using DefaultLogger::AddMsgs;
                using DefaultLogger::AddVars;
                using DefaultLogger::AddTags;

                using DefaultLogger::IncrementEntryIndex;
                using DefaultLogger::PushLogOut;
                using DefaultLogger::AfterLog;
                using DefaultLogger::CleanUp;

                using DefaultLogger::UnravelAndAddMsgs;
                using DefaultLogger::UnravelAndAddVariables;
                using DefaultLogger::UnravelAndAddTags;

                using DefaultLogger::AddFormatedTime;
                using DefaultLogger::AddFormatedSource;
                using DefaultLogger::AddFormatedIterationCount;
                using DefaultLogger::AddFormatedLevel;
                using DefaultLogger::AddFormatedMsg;
                using DefaultLogger::AddUnformatedMsgPart;
                using DefaultLogger::AddFormatedTag;
                using DefaultLogger::AddFormatedVariable;
                using DefaultLogger::AddFormatedVariableAsListPart;

                using DefaultLogger::AddSeparator;
                using DefaultLogger::AddNewline;
                using DefaultLogger::PutNumIntoCharcterField;
                using DefaultLogger::ResetString;
        };

        DefaultLoggerTester REF GetDefLoggerTester(LoggerInterface<DefaultLogger<>> PASS_REF logger)
        {
            return static_cast<DefaultLoggerTester REF>(logger.GetWriter());
        }
        DefaultLoggerTester REF GetDefLoggerTester(DefaultLogger<> PASS_REF logger)
        {
            return static_cast<DefaultLoggerTester REF>(logger);
        }
    }
}
#endif // STATICLOGTESTERCLASSES_H
