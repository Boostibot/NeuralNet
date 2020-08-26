#include "pch.h"
#include "../GClasses/Common/StaticLog.h"

namespace StaticLogger
{
	using namespace StaticLogTesting;
	TEST(Logger, CrtpOverloading)
	{
		//StaticLogger::Logger<StaticLogger::TestingLogger1>  logWriter1;
		Logger<TestingLogger1> logWriter1;
		EXPECT_EQ(logWriter1.Identifier, "TestingDataPackage");
		EXPECT_EQ(logWriter1.LastFunctionCalled.empty(), false); //- not true cause of TestingLogger2::OnCOnstruction

		Logger<TestingLogger2> logWriter2;
		EXPECT_EQ(logWriter2.Identifier, "TestingDataPackage2");
		EXPECT_EQ(logWriter2.LastFunctionCalled.empty(), false); //- not true cause of TestingLogger2::OnCOnstruction
		EXPECT_EQ(logWriter2.MyInt1, 1);
		EXPECT_EQ(logWriter2.MyInt2, 2);
		EXPECT_EQ(logWriter2.MyInt3, 3);


		Logger<TestingLogger2> eqWriter1;
		Logger<TestingLogger2, TestingDataPackage2> eqWriter2;
		//EXPECT_EQ(eqWriter1, eqWriter2);

		//Logger<TestingLogger2, TestingDataPackage1 > invalidWriter; //Should not compile!!!

	}
	
	namespace LoggerFunctionCalling
	{

		TEST(Logger, WriteMsgs)
		{
			Logger<TestingLogger1> logWriter1;
			logWriter1.WriteMsgs<2>("Msg1", "Msg2");
			EXPECT_EQ(logWriter1.LastFunctionCalled, "WriteMsgs");
		}

		TEST(Logger, WriteVars)
		{

			Logger<TestingLogger1> logWriter1;
			logWriter1.WriteVars<2, int, int>(1, 2);
			EXPECT_EQ(logWriter1.LastFunctionCalled, "WriteVars");
		}

		TEST(Logger, AppendSource)
		{

			Logger<TestingLogger1> logWriter1;
			StringType file = "File";
			u32 lineNum = 32;
			//logWriter1.AppendSource(file, 32, "Value"); //Compiler error - no function overload
			logWriter1.AppendSource<2>(file, 32);
			EXPECT_EQ(logWriter1.LastFunctionCalled, "AppendSource");
			EXPECT_EQ(logWriter1.FileName, file);
			EXPECT_EQ(logWriter1.LineNum, lineNum);
		}

		TEST(Logger, AppendTags)
		{

			Logger<TestingLogger1> logWriter;
			logWriter.AppendTags<2>("tag", "tag1");
			EXPECT_EQ(logWriter.LastFunctionCalled, "AppendTags");
		}

		TEST(Logger, OpenStream)
		{
			Logger<TestingLogger1> logWriter;
			//logWriter.OpenStream("path", "path"); //Compiler error - no function overload
			EXPECT_EQ(logWriter.StreamState, false);
			logWriter.OpenStream("path");
			EXPECT_EQ(logWriter.LastFunctionCalled, "OpenStream");
			EXPECT_EQ(logWriter.StreamState, true);
		}

		TEST(Logger, CloseStream)
		{

			Logger<TestingLogger1> logWriter;
			EXPECT_EQ(logWriter.StreamState, false);
			logWriter.CloseStream();
			EXPECT_EQ(logWriter.LastFunctionCalled, "CloseStream");
			EXPECT_EQ(logWriter.StreamState, false);
		}

		TEST(Logger, IsStreamOpen)
		{

			Logger<TestingLogger1> logWriter;
			logWriter.IsStreamOpen();
			EXPECT_EQ(logWriter.IsStreamOpen(), false);
			EXPECT_EQ(logWriter.LastFunctionCalled, "IsStreamOpen");

			logWriter.OpenStream("path");
			EXPECT_EQ(logWriter.IsStreamOpen(), true);
			logWriter.CloseStream();
			EXPECT_EQ(logWriter.IsStreamOpen(), false);
		}

		TEST(Logger, FlushStream)
		{
			Logger<TestingLogger1> logWriter;
			logWriter.FlushStream();
			EXPECT_EQ(logWriter.LastFunctionCalled, "FlushStream");
		}

		TEST(Logger, OnConstruction)
		{
			Logger<TestingLogger1> logWriter;
			//OnConstruction gets called automatically 
			EXPECT_EQ(logWriter.LastFunctionCalled, "OnConstruction");
		}

		TEST(Logger, OnDestruction)
		{
			//Used to save the msg - the data from the logger will get destroyed 
			//  along with it so its necessary to write them somwhre else
			StringType onDestructionMsg;
			{
				Logger<TestingLogger1> logWriter;
				logWriter.OnDestructionWriteLocation = &(onDestructionMsg);
			}
			EXPECT_EQ(onDestructionMsg, "OnDestruction");
		}

	}

	namespace LoggerLogEnabling
	{

		TEST(Logger, IsLevelInRange)
		{
			Logger<TestingLogger1> log;
			EXPECT_EQ(log.IsLevelInRange(0), true);
			EXPECT_EQ(log.IsLevelInRange(log.LevelCount), false);
			EXPECT_EQ(log.IsLevelInRange(log.LevelCount - 1), true);
		}
		TEST(Logger, IsLogEnabled)
		{
			Logger<TestingLogger1> log;
			EXPECT_EQ(log.IsLogEnabled(), true);
			log.DoLog(false);
			EXPECT_EQ(log.IsLogEnabled(), false);
		}
		TEST(Logger, DoLog)
		{
			Logger<TestingLogger1> log;
			EXPECT_EQ(log.IsLogEnabled(), true);
			log.DoLog(false);
			EXPECT_EQ(log.IsLogEnabled(), false);
			log.DoLog(true);
			EXPECT_EQ(log.IsLogEnabled(), true);
		}
		TEST(Logger, EnableLog)
		{
			Logger<TestingLogger1> log;
			EXPECT_EQ(log.IsLogEnabled(), true);
			log.DoLog(false);
			EXPECT_EQ(log.IsLogEnabled(), false);
			log.EnableLog();
			EXPECT_EQ(log.IsLogEnabled(), true);
			log.EnableLog();
			EXPECT_EQ(log.IsLogEnabled(), true);
		}
		TEST(Logger, DisableLog)
		{
			Logger<TestingLogger1> log;
			EXPECT_EQ(log.IsLogEnabled(), true);
			log.DisableLog();
			EXPECT_EQ(log.IsLogEnabled(), false);
			log.DisableLog();
			EXPECT_EQ(log.IsLogEnabled(), false);
		}

		TEST(Logger, LogLevels_LevelArgument)
		{
			Logger<TestingLogger1> log;

			//All should be on by default
			for (auto i = log.LevelCount; i-- > 0; )
				EXPECT_EQ(log.IsLogEnabled(i), true);

			//All are impacted by the DoLog function
			log.DisableLog();
			for (auto i = log.LevelCount; i-- > 0; )
				EXPECT_EQ(log.IsLogEnabled(i), false);

			//Global switch should beat individual levels
			log.DoLoggingLevels(true, 1, 2, 3, 4);
			for (auto i = log.LevelCount; i-- > 0; )
				EXPECT_EQ(log.IsLogEnabled(i), false);

			log.EnableLog();

			//Setting and multiple of the same argument
			log.DoLoggingLevels(false, 0, 2, 3, 3);
			EXPECT_EQ(log.IsLogEnabled(0), false);
			EXPECT_EQ(log.IsLogEnabled(2), false);
			EXPECT_EQ(log.IsLogEnabled(3), false);

			log.DoLoggingLevels(true, 0, 2, 3, 3);
			EXPECT_EQ(log.IsLogEnabled(0), true);
			EXPECT_EQ(log.IsLogEnabled(2), true);
			EXPECT_EQ(log.IsLogEnabled(3), true);

			//Order shouldnt matter
			log.DoLoggingLevels(false, 3, 2, 0, 0);
			EXPECT_EQ(log.IsLogEnabled(0), false);
			EXPECT_EQ(log.IsLogEnabled(2), false);
			EXPECT_EQ(log.IsLogEnabled(3), false);

			//Switching the global logging status shoudlnt reset the levels
			log.DisableLog();
			log.EnableLog();
			EXPECT_EQ(log.IsLogEnabled(0), false);
			EXPECT_EQ(log.IsLogEnabled(2), false);
			EXPECT_EQ(log.IsLogEnabled(3), false);

			//Functions should throw if provided with invalid argumnts
			EXPECT_ANY_THROW(log.DoLoggingLevels(true, 3, 2, log.MaxLevelIndex + 1, 0)); 
			//Non invalid levels SHOULD NOT be set - read explanation under Logger<>::DoLoggingLevels
			EXPECT_EQ(log.IsLogEnabled(0), false);
			EXPECT_EQ(log.IsLogEnabled(2), false);
			EXPECT_EQ(log.IsLogEnabled(3), false);
		}
		TEST(Logger, LogLevels_LevelTemplate)
		{
			Logger<TestingLogger1> log;

			//All should be on by default
			EXPECT_EQ(log.IsLogEnabled<0>(), true);
			EXPECT_EQ(log.IsLogEnabled<1>(), true);
			EXPECT_EQ(log.IsLogEnabled<2>(), true);
			EXPECT_EQ(log.IsLogEnabled<3>(), true);
			EXPECT_EQ(log.IsLogEnabled<4>(), true);
			//...
			EXPECT_EQ(log.IsLogEnabled<log.MaxLevelIndex>(), true);

			//All are impacted by the DoLog function
			log.DisableLog();
			EXPECT_EQ(log.IsLogEnabled<0>(), false);
			EXPECT_EQ(log.IsLogEnabled<1>(), false);
			EXPECT_EQ(log.IsLogEnabled<2>(), false);
			EXPECT_EQ(log.IsLogEnabled<3>(), false);
			EXPECT_EQ(log.IsLogEnabled<4>(), false);
			//...
			EXPECT_EQ(log.IsLogEnabled<log.MaxLevelIndex>(), false);

			//Global switch should beat individual levels
			log.DoLoggingLevels(true, 1, 2, 3, 4);
			EXPECT_EQ(log.IsLogEnabled<0>(), false);
			EXPECT_EQ(log.IsLogEnabled<1>(), false);
			EXPECT_EQ(log.IsLogEnabled<2>(), false);
			EXPECT_EQ(log.IsLogEnabled<3>(), false);
			EXPECT_EQ(log.IsLogEnabled<4>(), false);
			//...
			EXPECT_EQ(log.IsLogEnabled<log.MaxLevelIndex>(), false);

			log.EnableLog();

			//Setting and setting multiple of the same argument
			log.DoLoggingLevels<0, 2, 3, 3>(false);
			EXPECT_EQ(log.IsLogEnabled<0>(), false);
			EXPECT_EQ(log.IsLogEnabled<2>(), false);
			EXPECT_EQ(log.IsLogEnabled<3>(), false);

			log.DoLoggingLevels<0, 2, 3, 3>(true);
			EXPECT_EQ(log.IsLogEnabled<0>(), true);
			EXPECT_EQ(log.IsLogEnabled<2>(), true);
			EXPECT_EQ(log.IsLogEnabled<3>(), true);

			//Order shouldnt matter
			log.DoLoggingLevels<3, 2, 0, 0>(false);
			EXPECT_EQ(log.IsLogEnabled<0>(), false);
			EXPECT_EQ(log.IsLogEnabled<2>(), false);
			EXPECT_EQ(log.IsLogEnabled<3>(), false);


			//Switching the global logging status shoudlnt reset the levels
			log.DisableLog();
			log.EnableLog();
			EXPECT_EQ(log.IsLogEnabled<0>(), false);
			EXPECT_EQ(log.IsLogEnabled<2>(), false);
			EXPECT_EQ(log.IsLogEnabled<3>(), false);

			//Functions should not compile if provided with invalid argumnts
			//log.DoLoggingLevels<3, 2, log.MaxLevelIndex + 1, 0>(true);
		}
	}
}