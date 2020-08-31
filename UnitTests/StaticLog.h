#ifndef STATICLOG2_H
#define STATICLOG2_H

#include "Catch2/Catch.hpp"
#include "Libraries/Fmt/fmt/os.h"
#include "Libraries/Fmt/fmt/printf.h"
#include "GClasses/Common/StaticLog.h"


//#include "gtest/gtest.h"
namespace StaticLogger
{
    using namespace StaticLogTesting;

    TEST_CASE("CRTP overloading", "[StaticLogger]")
    {
        //StaticLogger::Logger<StaticLogger::TestingLogger1>  logWriter1;
        Logger<TestingLogger1> logWriter1;
        REQUIRE(logWriter1.Identifier == "TestingDataPackage");
        REQUIRE(logWriter1.LastFunctionCalled.empty() == false); //- not true cause of TestingLogger2::OnCOnstruction

        Logger<TestingLogger2> logWriter2;
        REQUIRE(logWriter2.Identifier == "TestingDataPackage2");
        REQUIRE(logWriter2.LastFunctionCalled.empty() == false); //- not true cause of TestingLogger2::OnCOnstruction
        REQUIRE(logWriter2.MyInt1 == 1);
        REQUIRE(logWriter2.MyInt2 == 2);
        REQUIRE(logWriter2.MyInt3 == 3);


        Logger<TestingLogger2> eqWriter1;
        Logger<TestingLogger2, TestingDataPackage2> eqWriter2;
        //REQUIRE(eqWriter1, eqWriter2);


        //Logger<TestingLogger2, TestingDataPackage1 > invalidWriter; //Should not compile!!!

    }

    TEST_CASE("CRTP overloaded copy constructors and assignments", "[StaticLogger]")
    {
        GIVEN("Two logWriters with copy constructors and assignments")
        {
            TestingDataPackageCopy dataPackageCopy(32, "Copy");

            REQUIRE(dataPackageCopy.Str == "Copy");
            REQUIRE(dataPackageCopy.LineNum == 32);

            Logger<TestingLoggerCopy> logWriterCopy1(11, "Copy1");
            Logger<TestingLoggerCopy> logWriterCopy2(22, "Copy2");
            logWriterCopy1.DoLog(false);
            logWriterCopy1.DoLoggingLevels<1, 3>(false);

            WHEN("Copy constructing from package")
            {
                Logger<TestingLoggerCopy> fromPackage((const TestingDataPackageCopy)dataPackageCopy);
                THEN("The contents will be copied")
                {
                    REQUIRE(fromPackage.Str == "Copy");
                    REQUIRE(fromPackage.LineNum == 32);
                    REQUIRE(dataPackageCopy.Str == "Copy");
                    REQUIRE(dataPackageCopy.LineNum == 32);
                }
            }

            WHEN("Copy constructing from logWriter")
            {
                Logger<TestingLoggerCopy> copyConstructed(logWriterCopy1);
                THEN("The contents will be copied")
                {
                    REQUIRE(copyConstructed.Str == "Copy1");
                    REQUIRE(copyConstructed.LineNum == 11);
                    REQUIRE(logWriterCopy1.Str == "Copy1");
                    REQUIRE(logWriterCopy1.LineNum == 11);

                    REQUIRE(logWriterCopy1.IsLogEnabled() == false);
                    REQUIRE(logWriterCopy1.IsLogEnabled(1) == false);
                    REQUIRE(logWriterCopy1.IsLogEnabled(3) == false);

                    REQUIRE(copyConstructed.IsLogEnabled() == false);
                    REQUIRE(copyConstructed.IsLogEnabled(1) == false);
                    REQUIRE(copyConstructed.IsLogEnabled(3) == false);
                }
            }

            WHEN("Copy assigning from logWriter")
            {
                logWriterCopy2 = logWriterCopy1;
                THEN("The contents will be copied")
                {
                    REQUIRE(logWriterCopy2.Str == "Copy1");
                    REQUIRE(logWriterCopy2.LineNum == 11);
                    REQUIRE(logWriterCopy1.Str == "Copy1");
                    REQUIRE(logWriterCopy1.LineNum == 11);

                    REQUIRE(logWriterCopy2.IsLogEnabled() == false);
                    REQUIRE(logWriterCopy2.IsLogEnabled(1) == false);
                    REQUIRE(logWriterCopy2.IsLogEnabled(3) == false);

                    REQUIRE(logWriterCopy1.IsLogEnabled() == false);
                    REQUIRE(logWriterCopy1.IsLogEnabled(1) == false);
                    REQUIRE(logWriterCopy1.IsLogEnabled(3) == false);
                }
            }
        }
    }

    TEST_CASE("CRTP overloaded move constructors and assignments", "[StaticLogger]")
    {
        GIVEN("Two logWriters with move constructors and assignments")
        {
            TestingDataPackageMove dataPackageMove(32, "Move");

            REQUIRE(dataPackageMove.Str == "Move");
            REQUIRE(dataPackageMove.LineNum == 32);

            Logger<TestingLoggerMove> logWriterMove1(11, "Move1");
            Logger<TestingLoggerMove> logWriterMove2(22, "Move2");
            logWriterMove1.DoLog(false);
            logWriterMove1.DoLoggingLevels<1, 3>(false);

            WHEN("Move constructing from package")
            {
                Logger<TestingLoggerMove> fromPackage(std::move(dataPackageMove));
                THEN("The contents will be swaped with empty package")
                {
                    REQUIRE(fromPackage.Str == "Move");
                    REQUIRE(fromPackage.LineNum == 32);
                    REQUIRE(dataPackageMove.Str == "");
                    REQUIRE(dataPackageMove.LineNum == 0);
                }
            }

            WHEN("Move constructing from logWriter")
            {
                Logger<TestingLoggerMove> moveConstructed(std::move(logWriterMove1));
                THEN("The contents will be swaped with the other logWriter")
                {
                    REQUIRE(moveConstructed.Str == "Move1");
                    REQUIRE(moveConstructed.LineNum == 11);
                    REQUIRE(logWriterMove1.Str == "");
                    REQUIRE(logWriterMove1.LineNum == 0);

                    REQUIRE(moveConstructed.IsLogEnabled() == false);
                    REQUIRE(moveConstructed.IsLogEnabled(1) == false);
                    REQUIRE(moveConstructed.IsLogEnabled(3) == false);

                    //The array is just copies beacuse its faster
                    // - the swapping with packages is here done only to prove that the move assignment happened
                    REQUIRE(logWriterMove1.IsLogEnabled() == false);
                    REQUIRE(logWriterMove1.IsLogEnabled(1) == false);
                    REQUIRE(logWriterMove1.IsLogEnabled(3) == false);
                }
            }

            WHEN("Move assigning from logWriter")
            {
                logWriterMove2 = std::move(logWriterMove1);
                THEN("The contents will be swaped with the other logWriter")
                {
                    REQUIRE(logWriterMove2.Str == "Move1");
                    REQUIRE(logWriterMove2.LineNum == 11);
                    REQUIRE(logWriterMove1.Str == "Move2");
                    REQUIRE(logWriterMove1.LineNum == 22);

                    REQUIRE(logWriterMove2.IsLogEnabled() == false);
                    REQUIRE(logWriterMove2.IsLogEnabled(1) == false);
                    REQUIRE(logWriterMove2.IsLogEnabled(3) == false);

                    REQUIRE(logWriterMove1.IsLogEnabled() == false);
                    REQUIRE(logWriterMove1.IsLogEnabled(1) == false);
                    REQUIRE(logWriterMove1.IsLogEnabled(3) == false);
                }
            }
        }
    }

    namespace LoggerFunctionCalling
    {

        TEST_CASE("Calling func: WriteMsgs", "[StaticLogger]")
        {
            Logger<TestingLogger1> logWriter1;
            logWriter1.WriteMsgs<2>("Msg1", "Msg2");
            REQUIRE(logWriter1.LastFunctionCalled == "WriteMsgs");
        }

        TEST_CASE("Calling func: WriteVars", "[StaticLogger]")
        {

            Logger<TestingLogger1> logWriter1;
            logWriter1.WriteVars<2, int, int>(1, 2);
            REQUIRE(logWriter1.LastFunctionCalled == "WriteVars");
        }

        TEST_CASE("Calling func: AppendSource", "[StaticLogger]")
        {

            Logger<TestingLogger1> logWriter1;
            StringType file = "File";
            u32 lineNum = 32;
            //logWriter1.AppendSource(file, 32, "Value"); //Compiler error - no function overload
            logWriter1.AppendSource<2>(file, 32);
            REQUIRE(logWriter1.LastFunctionCalled == "AppendSource");
            REQUIRE(logWriter1.FileName == file);
            REQUIRE(logWriter1.LineNum == lineNum);
        }

        TEST_CASE("Calling func: AppendTags", "[StaticLogger]")
        {

            Logger<TestingLogger1> logWriter;
            logWriter.AppendTags<2>("tag", "tag1");
            REQUIRE(logWriter.LastFunctionCalled == "AppendTags");
        }

        TEST_CASE("Calling func: OpenStream", "[StaticLogger]")
        {
            Logger<TestingLogger1> logWriter;
            //logWriter.OpenStream("path", "path"); //Compiler error - no function overload
            REQUIRE(logWriter.StreamState == false);
            logWriter.OpenStream("path");
            REQUIRE(logWriter.LastFunctionCalled == "OpenStream");
            REQUIRE(logWriter.StreamState == true);
        }

        TEST_CASE("Calling func: CloseStream", "[StaticLogger]")
        {

            Logger<TestingLogger1> logWriter;
            REQUIRE(logWriter.StreamState == false);
            logWriter.CloseStream();
            REQUIRE(logWriter.LastFunctionCalled == "CloseStream");
            REQUIRE(logWriter.StreamState == false);
        }

        TEST_CASE("Calling func: IsStreamOpen", "[StaticLogger]")
        {

            Logger<TestingLogger1> logWriter;
            logWriter.IsStreamOpen();
            REQUIRE(logWriter.IsStreamOpen() == false);
            REQUIRE(logWriter.LastFunctionCalled == "IsStreamOpen");

            logWriter.OpenStream("path");
            REQUIRE(logWriter.IsStreamOpen() == true);
            logWriter.CloseStream();
            REQUIRE(logWriter.IsStreamOpen() == false);
        }

        TEST_CASE("Calling func: FlushStream", "[StaticLogger]")
        {
            Logger<TestingLogger1> logWriter;
            logWriter.FlushStream();
            REQUIRE(logWriter.LastFunctionCalled == "FlushStream");
        }

        TEST_CASE("Calling func: OnConstruction", "[StaticLogger]")
        {
            Logger<TestingLogger1> logWriter;
            //OnConstruction gets called automatically
            REQUIRE(logWriter.LastFunctionCalled == "OnConstruction");
        }

        TEST_CASE("Calling function: OnDestruction", "[StaticLogger]")
        {
            //Used to save the msg - the data from the logger will get destroyed
            //  along with it so its necessary to write them somwhre else
            StringType onDestructionMsg;
            {
                Logger<TestingLogger1> logWriter;
                logWriter.OnDestructionWriteLocation = &(onDestructionMsg);
            }
            REQUIRE(onDestructionMsg == "OnDestruction");
        }

    }

    namespace LoggerLogEnabling
    {
        TEST_CASE("IsLevelInRange", "[StaticLogger]")
        {
            Logger<TestingLogger1> log;
            REQUIRE(log.IsLevelInRange(0) == true);
            REQUIRE(log.IsLevelInRange(log.LevelCount) == false);
            REQUIRE(log.IsLevelInRange(log.LevelCount - 1) == true);
        }
        TEST_CASE("IsLogEnabled", "[StaticLogger]")
        {
            Logger<TestingLogger1> log;
            REQUIRE(log.IsLogEnabled() == true);
            log.DoLog(false);
            REQUIRE(log.IsLogEnabled() == false);
        }
        TEST_CASE("Setting Log status", "[StaticLogger]")
        {
            Logger<TestingLogger1> log;
            REQUIRE(log.IsLogEnabled() == true);
            log.DoLog(false);
            REQUIRE(log.IsLogEnabled() == false);
            log.DoLog(true);
            REQUIRE(log.IsLogEnabled() == true);
        }
        TEST_CASE("Enabling Log", "[StaticLogger]")
        {
            Logger<TestingLogger1> log;
            REQUIRE(log.IsLogEnabled() == true);
            log.DoLog(false);
            REQUIRE(log.IsLogEnabled() == false);
            log.EnableLog();
            REQUIRE(log.IsLogEnabled() == true);
            log.EnableLog();
            REQUIRE(log.IsLogEnabled() == true);
        }
        TEST_CASE("Disabling log", "[StaticLogger]")
        {
            Logger<TestingLogger1> log;
            REQUIRE(log.IsLogEnabled() == true);
            log.DisableLog();
            REQUIRE(log.IsLogEnabled() == false);
            log.DisableLog();
            REQUIRE(log.IsLogEnabled() == false);
        }

        TEST_CASE("Getting and Setting Log levels using function arguments", "[StaticLogger]")
        {
            Logger<TestingLogger1> log;

            //All should be on by default
            for (auto i = log.LevelCount; i-- > 0; )
                REQUIRE(log.IsLogEnabled(i) == true);

            //All are impacted by the DoLog function
            log.DisableLog();
            for (auto i = log.LevelCount; i-- > 0; )
                REQUIRE(log.IsLogEnabled(i) == false);

            //Global switch should beat individual levels
            log.DoLoggingLevels(true, 1, 2, 3, 4);
            for (auto i = log.LevelCount; i-- > 0; )
                REQUIRE(log.IsLogEnabled(i) == false);

            log.EnableLog();

            //Setting and multiple of the same argument
            log.DoLoggingLevels(false, 0, 2, 3, 3);
            REQUIRE(log.IsLogEnabled(0) == false);
            REQUIRE(log.IsLogEnabled(2) == false);
            REQUIRE(log.IsLogEnabled(3) == false);

            log.DoLoggingLevels(true, 0, 2, 3, 3);
            REQUIRE(log.IsLogEnabled(0) == true);
            REQUIRE(log.IsLogEnabled(2) == true);
            REQUIRE(log.IsLogEnabled(3) == true);

            //Order shouldnt matter
            log.DoLoggingLevels(false, 3, 2, 0, 0);
            REQUIRE(log.IsLogEnabled(0) == false);
            REQUIRE(log.IsLogEnabled(2) == false);
            REQUIRE(log.IsLogEnabled(3) == false);

            //Switching the global logging status shoudlnt reset the levels
            log.DisableLog();
            log.EnableLog();
            REQUIRE(log.IsLogEnabled(0) == false);
            REQUIRE(log.IsLogEnabled(2) == false);
            REQUIRE(log.IsLogEnabled(3) == false);

            //Functions should throw if provided with invalid argumnts
            REQUIRE_THROWS(log.DoLoggingLevels(true, 3, 2, log.MaxLevelIndex + 1, 0));
            //Non invalid levels SHOULD NOT be set - read explanation under Logger<>::DoLoggingLevels
            REQUIRE(log.IsLogEnabled(0) == false);
            REQUIRE(log.IsLogEnabled(2) == false);
            REQUIRE(log.IsLogEnabled(3) == false);
        }
        TEST_CASE("Getting and Setting Log levels using templates", "[StaticLogger]")
        {
            Logger<TestingLogger1> log;

            //All should be on by default
            REQUIRE(log.IsLogEnabled<0>() == true);
            REQUIRE(log.IsLogEnabled<1>() == true);
            REQUIRE(log.IsLogEnabled<2>() == true);
            REQUIRE(log.IsLogEnabled<3>() == true);
            REQUIRE(log.IsLogEnabled<4>() == true);
            //...
            REQUIRE(log.IsLogEnabled<log.MaxLevelIndex>() == true);

            //All are impacted by the DoLog function
            log.DisableLog();
            REQUIRE(log.IsLogEnabled<0>() == false);
            REQUIRE(log.IsLogEnabled<1>() == false);
            REQUIRE(log.IsLogEnabled<2>() == false);
            REQUIRE(log.IsLogEnabled<3>() == false);
            REQUIRE(log.IsLogEnabled<4>() == false);
            //...
            REQUIRE(log.IsLogEnabled<log.MaxLevelIndex>() == false);

            //Global switch should beat individual levels
            log.DoLoggingLevels(true, 1, 2, 3, 4);
            REQUIRE(log.IsLogEnabled<0>() == false);
            REQUIRE(log.IsLogEnabled<1>() == false);
            REQUIRE(log.IsLogEnabled<2>() == false);
            REQUIRE(log.IsLogEnabled<3>() == false);
            REQUIRE(log.IsLogEnabled<4>() == false);
            //...
            REQUIRE(log.IsLogEnabled<log.MaxLevelIndex>() == false);

            log.EnableLog();

            //Setting and setting multiple of the same argument
            log.DoLoggingLevels<0, 2, 3, 3>(false);
            REQUIRE(log.IsLogEnabled<0>() == false);
            REQUIRE(log.IsLogEnabled<2>() == false);
            REQUIRE(log.IsLogEnabled<3>() == false);

            log.DoLoggingLevels<0, 2, 3, 3>(true);
            REQUIRE(log.IsLogEnabled<0>() == true);
            REQUIRE(log.IsLogEnabled<2>() == true);
            REQUIRE(log.IsLogEnabled<3>() == true);

            //Order shouldnt matter
            log.DoLoggingLevels<3, 2, 0, 0>(false);
            REQUIRE(log.IsLogEnabled<0>() == false);
            REQUIRE(log.IsLogEnabled<2>() == false);
            REQUIRE(log.IsLogEnabled<3>() == false);


            //Switching the global logging status shoudlnt reset the levels
            log.DisableLog();
            log.EnableLog();
            REQUIRE(log.IsLogEnabled<0>() == false);
            REQUIRE(log.IsLogEnabled<2>() == false);
            REQUIRE(log.IsLogEnabled<3>() == false);

            //Functions should not compile if provided with invalid argumnts
            //log.DoLoggingLevels<3, 2, log.MaxLevelIndex + 1, 0>(true);
        }
    }

    namespace DefaultDataInterpretTesting
    {

        template<typename T, typename valType>
        void TestConversion(valType val, std::string PASS_REF output, DefaultDataInterpret PASS_REF interpret, std::string expectedResult)
        {
            interpret.InterpretArg(static_cast<T>(val), output);
            REQUIRE(output == expectedResult);
        }

        TEST_CASE("Conversions", "[DefaultDataInterpret]")
        {
            DefaultDataInterpret interpret;
            std::string output;
            output.reserve(512);

            SECTION("Converting from int")
            {
                TestConversion<int>(132, output, interpret, "132");
                TestConversion<int>(-132, output, interpret, "-132");
                TestConversion<int>(0.3, output, interpret, "0");
                TestConversion<int>(-0.3, output, interpret, "0");
            }

            SECTION("Converting from long")
            {
                TestConversion<long>(132, output, interpret, "132");
                TestConversion<long>(-132, output, interpret, "-132");
                TestConversion<long>(0.3, output, interpret, "0");
                TestConversion<long>(-0.3, output, interpret, "0");
            }

            SECTION("Converting from long long")
            {
                TestConversion<long long>(132, output, interpret, "132");
                TestConversion<long long>(-132, output, interpret, "-132");
                TestConversion<long long>(0.3, output, interpret, "0");
                TestConversion<long long>(-0.3, output, interpret, "0");
            }

            SECTION("Converting from unsigned")
            {
                TestConversion<unsigned>(132, output, interpret, "132");
                TestConversion<unsigned>(0.3, output, interpret, "0");
                TestConversion<unsigned>(-0.3, output, interpret, "0");
            }

            SECTION("Converting from unsigned long")
            {
                TestConversion<unsigned long>(132, output, interpret, "132");
                TestConversion<unsigned long>(0.3, output, interpret, "0");
                TestConversion<unsigned long>(-0.3, output, interpret, "0");
            }

            SECTION("Converting from unsigned long long")
            {
                TestConversion<unsigned long long>(132, output, interpret, "132");
                TestConversion<unsigned long long>(0.3, output, interpret, "0");
                TestConversion<unsigned long long>(-0.3, output, interpret, "0");
            }

            SECTION("Converting from float")
            {
                TestConversion<float>(132, output, interpret, "132.000000");
                TestConversion<float>(-132, output, interpret, "-132.000000");
                TestConversion<float>(0.3, output, interpret, "0.300000");
                TestConversion<float>(-0.3, output, interpret, "-0.300000");
            }

            SECTION("Converting from double")
            {
                TestConversion<double>(132, output, interpret, "132.000000");
                TestConversion<double>(-132, output, interpret, "-132.000000");
                TestConversion<double>(0.3, output, interpret, "0.300000");
                TestConversion<double>(-0.3, output, interpret, "-0.300000");
            }

            SECTION("Converting from long double")
            {
                TestConversion<long double>(132, output, interpret, "132.0000000000000000");
                TestConversion<long double>(-132, output, interpret, "-132.0000000000000000");
                TestConversion<long double>(0.3, output, interpret, "0.3000000000000000");
                TestConversion<long double>(-0.3, output, interpret, "-0.3000000000000000");
            }

            SECTION("Converting from pointer")
            {
                void POINTER ptr;
                ptr = reinterpret_cast<void POINTER>(0x0);
                TestConversion<void POINTER>(ptr, output, interpret, "0x0");
                ptr = reinterpret_cast<void POINTER>(0xf);
                TestConversion<void POINTER>(ptr, output, interpret, "0xf");
                ptr = reinterpret_cast<void POINTER>(0xf00f);
                TestConversion<void POINTER>(ptr, output, interpret, "0xf00f");
            }

            SECTION("Converting from bool")
            {
                TestConversion<bool>(true, output, interpret, "true");
                TestConversion<bool>(false, output, interpret, "false");
            }

            SECTION("Converting from char *")
            {
                const char * myCStr1 = "cstr1";
                const char * myCStr2 = "CSTR2";
                const char * myCStr3 = "";
                TestConversion<const char *>(myCStr1, output, interpret, "cstr1");
                TestConversion<const char *>(myCStr2, output, interpret, "CSTR2");
                TestConversion<const char *>(myCStr3, output, interpret, "");
            }

            SECTION("Converting from std::string")
            {
                const std::string myStr1 = "str1";
                const std::string myStr2 = "STR2";
                const std::string myStr3 = "";
                TestConversion<std::string>(myStr1, output, interpret, "str1");
                TestConversion<std::string>(myStr2, output, interpret, "STR2");
                TestConversion<std::string>(myStr3, output, interpret, "");
            }

            SECTION("Converting from std::string_view")
            {
                TestConversion<std::string_view>("str1", output, interpret, "str1");
                TestConversion<std::string_view>("STR2", output, interpret, "STR2");
                TestConversion<std::string_view>("", output, interpret, "");
            }
        }
    }

    namespace DefaultLogWritterPackageTesting
    {
        TEST_CASE("DefaultLogWritterPackage Construction and Assignment", "[DefaultLogWritterPackage]")
        {
            DefaultLogWriterPackageTester Tester1("My file");
            DefaultLogWriterPackageTester Tester2("My file");

            SECTION("Default construction member inicialisation")
            {
                REQUIRE(Tester1.Formater.size() == 0);
                REQUIRE(Tester1.CollectionString.capacity() >= Tester1.MinimumCollectionStringSize);
                REQUIRE(Tester1.TempString.capacity() >= Tester1.MinimumTempStringSize);
                REQUIRE(Tester1.Messages.capacity() >= Tester1.MinimumMessagesSize);
                REQUIRE(Tester1.Variables.capacity() >= Tester1.MinimumVariablesSize);
                REQUIRE(Tester1.Tags.capacity() >= Tester1.MinimumTagsSize);
                REQUIRE(Tester1.Source.capacity() >= Tester1.MinimumSourceSize);
                REQUIRE(Tester1.EntryIndex == 0);
            }

            Tester1.CollectionString = "Collection string";
            Tester1.TempString = "Temp string";
            Tester1.Messages = "Messages";
            Tester1.Variables = "Variables";
            Tester1.Tags = "Tags";
            Tester1.Source = "Source";
            Tester1.EntryIndex = 1;

            SECTION("Move assignment")
            {
                Tester2 = std::move(Tester1);
                REQUIRE(Tester2.CollectionString == "Collection string");
                REQUIRE(Tester2.TempString == "Temp string");
                REQUIRE(Tester2.Messages == "Messages");
                REQUIRE(Tester2.Variables == "Variables");
                REQUIRE(Tester2.Tags == "Tags");
                REQUIRE(Tester2.Source == "Source");
                REQUIRE(Tester2.EntryIndex == 1);

                REQUIRE(Tester1.CollectionString == "");
                REQUIRE(Tester1.TempString == "");
            }

            SECTION("Move constructor")
            {
                DefaultLogWriterPackageTester Tester3(std::move(Tester1));
                REQUIRE(Tester3.CollectionString == "Collection string");
                REQUIRE(Tester3.TempString == "Temp string");
                REQUIRE(Tester3.Messages == "Messages");
                REQUIRE(Tester3.Variables == "Variables");
                REQUIRE(Tester3.Tags == "Tags");
                REQUIRE(Tester3.Source == "Source");
                REQUIRE(Tester3.EntryIndex == 1);

                REQUIRE(Tester2.CollectionString == "");
                REQUIRE(Tester2.TempString == "");
            }
        }

    }

    namespace DefaultLogWritterTesting
    {
        TEST_CASE("Construction DefaultLogWritter", "[DefaultLogWritter]")
        {
            GIVEN("Two logWriters with move constructors and assignments")
            {
                DefaultLogWriterPackage dataPackage("FilePackage");
                GetDefPackageTester(dataPackage).CollectionString   = "PackageCollectionString";
                GetDefPackageTester(dataPackage).TempString         = "PackageTempString";
                GetDefPackageTester(dataPackage).Messages           = "PackageMessages";
                GetDefPackageTester(dataPackage).Variables          = "PackageVariables";
                GetDefPackageTester(dataPackage).Tags               = "PackageTags";
                GetDefPackageTester(dataPackage).Source             = "PackageSource";
                GetDefPackageTester(dataPackage).EntryIndex         = 33;

                Logger<DefaultLogWriter> logWriter1("File1");
                Logger<DefaultLogWriter> logWriter2("File2");

                GetDefWriterTester(logWriter1).CollectionString   = "Logger1CollectionString";
                GetDefWriterTester(logWriter1).TempString         = "Logger1TempString";
                GetDefWriterTester(logWriter1).Messages           = "Logger1Messages";
                GetDefWriterTester(logWriter1).Variables          = "Logger1Variables";
                GetDefWriterTester(logWriter1).Tags               = "Logger1Tags";
                GetDefWriterTester(logWriter1).Source             = "Logger1Source";
                GetDefWriterTester(logWriter1).EntryIndex         = 11;

                logWriter1.DoLog(false);
                logWriter1.DoLoggingLevels<1, 3>(false);

                WHEN("Move constructing from package")
                {
                    Logger<DefaultLogWriter> fromPackage(std::move(dataPackage));
                    THEN("The contents will be swaped with empty package but then reset as a result of OnConstruction")
                    {
                        //All strings except TempSTring get reset as result of pushing out the OnConstruction message
                        REQUIRE(GetDefWriterTester(fromPackage).CollectionString    == "");
                        //REQUIRE(GetDefWriterTester(fromPackage).TempString          == "");
                        REQUIRE(GetDefWriterTester(fromPackage).Messages            == "");
                        REQUIRE(GetDefWriterTester(fromPackage).Variables           == "");
                        REQUIRE(GetDefWriterTester(fromPackage).Tags                == "");
                        REQUIRE(GetDefWriterTester(fromPackage).Source              == "");
                        REQUIRE(GetDefWriterTester(fromPackage).EntryIndex          == 34); //1 is added cause of the OnConstruction msg

                        REQUIRE(GetDefPackageTester(dataPackage).CollectionString   == "");
                        REQUIRE(GetDefPackageTester(dataPackage).TempString         == "");
                        REQUIRE(GetDefPackageTester(dataPackage).Messages           == "");
                        REQUIRE(GetDefPackageTester(dataPackage).Variables          == "");
                        REQUIRE(GetDefPackageTester(dataPackage).Tags               == "");
                        REQUIRE(GetDefPackageTester(dataPackage).Source             == "");
                        REQUIRE(GetDefPackageTester(dataPackage).EntryIndex         == 33); //trivially constructible types are just copied
                    }
                }

                WHEN("Move constructing from logWriter")
                {
                    Logger<DefaultLogWriter> moveConstructed(std::move(logWriter1));
                    THEN("The contents will be swaped with empty package but then reset as a result of OnConstruction")
                    {
                        //All strings except TempSTring get reset as result of pushing out the OnConstruction message
                        REQUIRE(GetDefWriterTester(moveConstructed).CollectionString    == "");
                        //REQUIRE(GetDefWriterTester(moveConstructed).TempString          == "");
                        REQUIRE(GetDefWriterTester(moveConstructed).Messages            == "");
                        REQUIRE(GetDefWriterTester(moveConstructed).Variables           == "");
                        REQUIRE(GetDefWriterTester(moveConstructed).Tags                == "");
                        REQUIRE(GetDefWriterTester(moveConstructed).Source              == "");
                        REQUIRE(GetDefWriterTester(moveConstructed).EntryIndex          == 12); //1 is added cause of the OnConstruction msg

                        REQUIRE(GetDefWriterTester(logWriter1).CollectionString    == "");
                        REQUIRE(GetDefWriterTester(logWriter1).TempString          == "");
                        REQUIRE(GetDefWriterTester(logWriter1).Messages            == "");
                        REQUIRE(GetDefWriterTester(logWriter1).Variables           == "");
                        REQUIRE(GetDefWriterTester(logWriter1).Tags                == "");
                        REQUIRE(GetDefWriterTester(logWriter1).Source              == "");
                        REQUIRE(GetDefWriterTester(logWriter1).EntryIndex          == 11); //trivially constructible types are just copied

                        //Status level array is just copied
                        REQUIRE(moveConstructed.IsLogEnabled() == false);
                        REQUIRE(moveConstructed.IsLogEnabled(1) == false);
                        REQUIRE(moveConstructed.IsLogEnabled(3) == false);

                        REQUIRE(logWriter1.IsLogEnabled() == false);
                        REQUIRE(logWriter1.IsLogEnabled(1) == false);
                        REQUIRE(logWriter1.IsLogEnabled(3) == false);
                    }
                }

                WHEN("Move assigning from logWriter")
                {
                    logWriter2 = std::move(logWriter1);
                    THEN("The contents will be swaped or set equal to the other logWriter")
                    {
                        //No OnConstruction function is called when move assigning therefore the values are either swapped or set equal
                        REQUIRE(GetDefWriterTester(logWriter2).CollectionString    == "Logger1CollectionString");
                        REQUIRE(GetDefWriterTester(logWriter2).TempString          == "Logger1TempString");
                        REQUIRE(GetDefWriterTester(logWriter2).Messages            == "Logger1Messages");
                        REQUIRE(GetDefWriterTester(logWriter2).Variables           == "Logger1Variables");
                        REQUIRE(GetDefWriterTester(logWriter2).Tags                == "Logger1Tags");
                        REQUIRE(GetDefWriterTester(logWriter2).Source              == "Logger1Source");
                        REQUIRE(GetDefWriterTester(logWriter2).EntryIndex          == 11);

                        REQUIRE(GetDefWriterTester(logWriter1).CollectionString    == "");
                        REQUIRE(GetDefWriterTester(logWriter1).TempString          == "");
                        REQUIRE(GetDefWriterTester(logWriter1).Messages            == "");
                        REQUIRE(GetDefWriterTester(logWriter1).Variables           == "");
                        REQUIRE(GetDefWriterTester(logWriter1).Tags                == "");
                        REQUIRE(GetDefWriterTester(logWriter1).Source              == "");
                        REQUIRE(GetDefWriterTester(logWriter1).EntryIndex          == 11); //trivially constructible types are just copied

                        //Status level array is just copied
                        REQUIRE(logWriter2.IsLogEnabled() == false);
                        REQUIRE(logWriter2.IsLogEnabled(1) == false);
                        REQUIRE(logWriter2.IsLogEnabled(3) == false);

                        REQUIRE(logWriter1.IsLogEnabled() == false);
                        REQUIRE(logWriter1.IsLogEnabled(1) == false);
                        REQUIRE(logWriter1.IsLogEnabled(3) == false);
                    }
                }
            }
        }

        TEST_CASE("Using constant adding functions", "[DefaultLogWritter]")
        {
            GIVEN("A LogWriter and a output string and formater")
            {
                Logger<DefaultLogWriter> writer("File1");
                std::string output;
                fmt::format_int formater;
                output.reserve(256);

                SECTION("ResetString")
                {
                    GetDefWriterTester(writer).ResetString(output);
                    REQUIRE(output.empty() == true);
                    output = "Hello";
                    GetDefWriterTester(writer).ResetString(output);
                    REQUIRE(output.empty() == true);
                }

                SECTION("PutNumIntoCharcterField")
                {
                    GetDefWriterTester(writer).PutNumIntoCharcterField(32, 8, formater, output);
                    REQUIRE(output == "00000032");
                    GetDefWriterTester(writer).PutNumIntoCharcterField(1111, 8, formater, output);
                    REQUIRE(output == "0000003200001111");
                    output.clear();

                    GetDefWriterTester(writer).PutNumIntoCharcterField(1111, 3, formater, output);
                    REQUIRE(output == "1111");
                    output.clear();

                    GetDefWriterTester(writer).PutNumIntoCharcterField(1111, 0, formater, output);
                    REQUIRE(output == "1111");
                    output.clear();

                    GetDefWriterTester(writer).PutNumIntoCharcterField(0, 8, formater, output);
                    REQUIRE(output == "00000000");
                    output.clear();
                }

                SECTION("AddNewline")
                {
                    GetDefWriterTester(writer).AddNewline(output);
                    REQUIRE(output[0] == '\n');
                    GetDefWriterTester(writer).AddNewline(output);
                    GetDefWriterTester(writer).AddNewline(output);
                    REQUIRE(output[0] == '\n');
                    REQUIRE(output[2] == '\n');
                }

                SECTION("AddSeparator")
                {
                    constexpr char separator = DefaultLogWriterTester::Separator;
                    GetDefWriterTester(writer).AddSeparator(output);
                    REQUIRE(output[0] == separator);
                    GetDefWriterTester(writer).AddSeparator(output);
                    GetDefWriterTester(writer).AddSeparator(output);
                    REQUIRE(output[0] == separator);
                    REQUIRE(output[2] == separator);
                }

                SECTION("AddFormatedVariable")
                {
                    GetDefWriterTester(writer).AddFormatedVariable("Name", "Value", output);
                    REQUIRE(output == "Name = Value");
                    GetDefWriterTester(writer).AddFormatedVariable("Name2", "Value2", output);
                    REQUIRE(output == "Name = ValueName2 = Value2");
                    output.clear();
                    GetDefWriterTester(writer).AddFormatedVariable("32", "64", output);
                    REQUIRE(output == "32 = 64");
                    output.clear();
                    GetDefWriterTester(writer).AddFormatedVariable("\n", "\t", output);
                    REQUIRE(output == "\n = \t");
                }

                SECTION("AddFormatedTag")
                {
                    GetDefWriterTester(writer).AddFormatedTag("tag", output);
                    REQUIRE(output == "<tag>");
                    GetDefWriterTester(writer).AddFormatedTag("tag2", output);
                    REQUIRE(output == "<tag><tag2>");
                    output.clear();
                    GetDefWriterTester(writer).AddFormatedTag("\n", output);
                    REQUIRE(output == "<\n>");
                }

                SECTION("AddUnformatedMsgPart")
                {
                    GetDefWriterTester(writer).AddUnformatedMsgPart("msg", output);
                    REQUIRE(output == "msg");
                    GetDefWriterTester(writer).AddUnformatedMsgPart("msg2", output);
                    REQUIRE(output == "msgmsg2");
                    output.clear();
                    GetDefWriterTester(writer).AddUnformatedMsgPart("\n", output);
                    REQUIRE(output == "\n");
                }

                SECTION("AddFormatedMsg")
                {
                    GetDefWriterTester(writer).AddFormatedMsg("msg", output);
                    REQUIRE(output == "msg = msg");
                    GetDefWriterTester(writer).AddFormatedMsg("msg2", output);
                    REQUIRE(output == "msg = msgmsg = msg2");
                    output.clear();
                    GetDefWriterTester(writer).AddFormatedMsg("\n", output);
                    REQUIRE(output == "msg = \n");
                }

                SECTION("AddFormatedLevel")
                {
                    GetDefWriterTester(writer).AddFormatedLevel(32, formater, output);
                    REQUIRE(output == "<32>");
                    GetDefWriterTester(writer).AddFormatedLevel(64, formater, output);
                    REQUIRE(output == "<32><64>");
                    output.clear();
                    GetDefWriterTester(writer).AddFormatedLevel(0, formater, output);
                    REQUIRE(output == "<0>");
                }

                SECTION("AddFormatedIterationCount")
                {
                    GetDefWriterTester(writer).AddFormatedIterationCount(32, formater, output);
                    REQUIRE(output == "00000032");
                    GetDefWriterTester(writer).AddFormatedIterationCount(64, formater, output);
                    REQUIRE(output == "0000003200000064");
                    output.clear();
                    GetDefWriterTester(writer).AddFormatedIterationCount(0, formater, output);
                    REQUIRE(output == "00000000");
                }

                SECTION("AddFormatedSource")
                {
                    GetDefWriterTester(writer).AddFormatedSource("file1", 32, formater, output);
                    REQUIRE(output == "file1 [32]");
                    GetDefWriterTester(writer).AddFormatedSource("file2", 64, formater, output);
                    REQUIRE(output == "file1 [32]file2 [64]");
                    output.clear();
                    GetDefWriterTester(writer).AddFormatedSource("\n", 0, formater, output);
                    REQUIRE(output == "\n [0]");
                }

                SECTION("AddFormatedTime")
                {
                    time_t rawTime;
                    time(&rawTime);
                    unsigned long long lenght;
                    GetDefWriterTester(writer).AddFormatedTime(formater, rawTime, clock(), output);
                    REQUIRE(output.empty() == false);
                    lenght = output.size();
                    GetDefWriterTester(writer).AddFormatedTime(formater, rawTime, clock(), output);
                    REQUIRE(output.empty() == false);
                    REQUIRE(output.size() > lenght);
                    output.clear();

                    GetDefWriterTester(writer).AddFormatedTime(formater, 0, 0, output);
                    REQUIRE(output.empty() == false);
                    lenght = output.size();
                    output.clear();

                    GetDefWriterTester(writer).AddFormatedTime(formater, std::numeric_limits<unsigned long long>::max(), std::numeric_limits<unsigned long>::max(), output);
                    REQUIRE(output.empty() == false);
                    lenght = output.size();
                }

            }
        }
    }
}

#endif // STATICLOG2_H
