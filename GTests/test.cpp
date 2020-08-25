#include "pch.h"
#include "../GClasses/Common/Log.h"

/*
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}


TEST(Logger, )
{
	EXPECT_EQ(1, 1);
}
*/

TEST(Logger, DoLogginglevels) 
{
	Logger log;

	//
	EXPECT_TRUE(log.IsLoggingLevelEnabled<3>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled(3));

	EXPECT_ANY_THROW(log.IsLoggingLevelEnabled(Logger<>::LevelArraySize + 1));
	//EXPECT_ANY_THROW(log.IsLoggingLevelEnabled<Logger<>::LevelArraySize + 1>()); //will produce static_assertion thus works

	//
	log.DoLoggingLevel(3, false);

	EXPECT_FALSE(log.IsLoggingLevelEnabled<3>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled(3));

	log.DoLoggingLevel(3, true);

	EXPECT_TRUE(log.IsLoggingLevelEnabled<3>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled(3));

	log.DoLoggingLevel<3>(false);

	EXPECT_FALSE(log.IsLoggingLevelEnabled<3>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled(3));

	log.DoLoggingLevel<3>(true);

	EXPECT_TRUE(log.IsLoggingLevelEnabled<3>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled(3));

	EXPECT_ANY_THROW(log.DoLoggingLevel(Logger<>::LevelArraySize + 1, true));
	//EXPECT_ANY_THROW(log.DoLoggingLevel<Logger<>::LevelArraySize + 1>(true)); //will produce static_assertion thus works

	//
	log.DoLoggingLevels(false, 3, 2, 1);
	EXPECT_FALSE(log.IsLoggingLevelEnabled<3>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled<2>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled<1>());

	log.DoLoggingLevels(false, 1, 3, 2);
	EXPECT_FALSE(log.IsLoggingLevelEnabled<3>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled<2>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled<1>());

	log.DoLoggingLevels(true, 3, 2, 3, 2);
	EXPECT_TRUE(log.IsLoggingLevelEnabled<3>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled<2>());

	log.DoLoggingLevels<3, 2, 1>(false);
	EXPECT_FALSE(log.IsLoggingLevelEnabled<3>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled<2>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled<1>());

	log.DoLoggingLevels<1, 3, 2>(false);
	EXPECT_FALSE(log.IsLoggingLevelEnabled<3>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled<2>());
	EXPECT_FALSE(log.IsLoggingLevelEnabled<1>());

	log.DoLoggingLevels<3, 2, 3, 2>(true);
	EXPECT_TRUE(log.IsLoggingLevelEnabled<3>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled<2>());

	EXPECT_ANY_THROW(log.DoLoggingLevels(true, Logger<>::LevelArraySize + 1, 3, 2));
	//log.DoLoggingLevels<Logger<>::LevelArraySize + 1, 3, 2>(true); //will produce static_assertion thus works
}

TEST(Logger, DoLog)
{
	Logger log;
	
	EXPECT_TRUE(log.IsLoggingEnabled());
	EXPECT_TRUE(log.IsLoggingEnabled());

	log.DisableLog();
	EXPECT_FALSE(log.IsLoggingEnabled());

	log.DisableLog();
	EXPECT_FALSE(log.IsLoggingEnabled());

	log.EnableLog();
	EXPECT_TRUE(log.IsLoggingEnabled());

	log.DoLog(false);
	EXPECT_FALSE(log.IsLoggingEnabled());

	log.DoLog(true);
	EXPECT_TRUE(log.IsLoggingEnabled());
}

TEST(Logger, Construction)
{
	Logger<> log;

	EXPECT_TRUE(log.IsLoggingEnabled());

	EXPECT_TRUE(log.GetOperativeLogWriter() != nullptr);
	EXPECT_TRUE(log.GetPassiveLogWriter() != nullptr);

	EXPECT_FALSE(log.GetOperativeLogWriter()->IsPassiveLogger());
	EXPECT_TRUE(log.GetPassiveLogWriter()->IsPassiveLogger());

	for (u32 i = 0; i < Logger<>::LevelArraySize; i++)
	{
		EXPECT_TRUE(log.IsLoggingLevelEnabled(i));
		EXPECT_TRUE(log.GetLevelPointer(i) != nullptr);
	}

	EXPECT_TRUE(log.IsLoggingLevelEnabled<1>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled<2>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled<3>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled<4>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled<5>());
	EXPECT_TRUE(log.IsLoggingLevelEnabled<14>());
	//EXPECT_ANY_THROW(log.IsLoggingLevelEnabled<Logger<>::LevelArraySize + 1>());

	EXPECT_TRUE(log.GetLevelPointer<1>() != nullptr);
	EXPECT_TRUE(log.GetLevelPointer<2>() != nullptr);
	EXPECT_TRUE(log.GetLevelPointer<3>() != nullptr);
	EXPECT_TRUE(log.GetLevelPointer<14>() != nullptr);
	//EXPECT_ANY_THROW(log.GetLevelPointer<Logger<>::LevelArraySize + 1>()); //will produce static_assertion thus works

}

TEST(Logger, StaticFunctions)
{
	std::unique_ptr<LogWriterInterface> temp1, temp2, temp3, temp4;

	temp1 = Logger<>::GetNewLogWritter<DefaultOperativeLogWriter>();
	EXPECT_FALSE(temp1.get()->IsPassiveLogger());
	EXPECT_TRUE(temp1.get() != nullptr);
	temp1 = Logger<>::GetNewLogWritter<DefaultPassiveLogWriter>();
	EXPECT_TRUE(temp1.get()->IsPassiveLogger());
	EXPECT_TRUE(temp1.get() != nullptr);

	temp3 = Logger<>::GetNewLogWritter<DefaultOperativeLogWriter>();
	temp4 = Logger<>::GetNewLogWritter<DefaultOperativeLogWriter>();
	EXPECT_TRUE(temp3 != temp4);

	temp2 = Logger<>::GetNewDefaultOperativeWriter();
	EXPECT_FALSE(temp2.get()->IsPassiveLogger());
	EXPECT_TRUE(temp2.get() != nullptr);
	temp2 = Logger<>::GetNewDefaultPassiveWriter();
	EXPECT_TRUE(temp2.get()->IsPassiveLogger());
	EXPECT_TRUE(temp2.get() != nullptr);

	temp3 = Logger<>::GetNewDefaultOperativeWriter();
	temp4 = Logger<>::GetNewDefaultOperativeWriter();
	EXPECT_TRUE(temp3 != temp4);

	EXPECT_TRUE(Logger<>::IsIndexInLevelArrayRange(0));
	EXPECT_TRUE(Logger<>::IsIndexInLevelArrayRange(Logger<>::LevelArraySize - 1));
	EXPECT_FALSE(Logger<>::IsIndexInLevelArrayRange(Logger<>::LevelArraySize + 1));
}

TEST(Logger, PointerGettingSetting)
{
	//TODO 
	//- SPLIT 
	//- ADD CHECKING FOR REBUILDING THE POINTER ARRAY
	//- ADD CHECKING FOR SETTING TO EMPTY POINTER AND THE POINTER ARRAY!!
	Logger<> log;

	EXPECT_TRUE(log.GetOperativeLogWriter() != nullptr);
	EXPECT_TRUE(log.GetPassiveLogWriter() != nullptr);

	EXPECT_FALSE(log.GetOperativeLogWriter()->IsPassiveLogger());
	EXPECT_TRUE(log.GetPassiveLogWriter()->IsPassiveLogger());

	std::unique_ptr<int> ptr;

	std::unique_ptr<LogWriterInterface> operative = Logger<>::GetNewDefaultOperativeWriter();
	std::unique_ptr<LogWriterInterface> passive = Logger<>::GetNewDefaultOperativeWriter();
	LogWriterInterface POINTER location1 = operative.get();
	LogWriterInterface POINTER location2 = log.GetOperativeLogWriter();

	EXPECT_TRUE(location1 != location2);
	{
		log.ChangeOperativeWriter(std::move(operative));
		EXPECT_TRUE(operative.get() == nullptr);
		EXPECT_TRUE(location1 == log.GetOperativeLogWriter());
		EXPECT_FALSE(log.GetOperativeLogWriter()->IsPassiveLogger());

		log.ChangeOperativeWriter(std::move(passive));
		EXPECT_FALSE(log.GetOperativeLogWriter()->IsPassiveLogger());
	}

	operative = Logger<>::GetNewDefaultOperativeWriter();
	passive = Logger<>::GetNewDefaultPassiveWriter();
	{
		log.ChangePassiveWriter(std::move(operative));
		EXPECT_FALSE(log.GetPassiveLogWriter()->IsPassiveLogger());

		log.ChangePassiveWriter(std::move(passive));
		EXPECT_TRUE(log.GetPassiveLogWriter()->IsPassiveLogger());
	}

	{
		log.ChangePassiveWriter<DefaultOperativeLogWriter>();
		EXPECT_FALSE(log.GetPassiveLogWriter()->IsPassiveLogger());

		log.ChangePassiveWriter<DefaultPassiveLogWriter>();
		EXPECT_TRUE(log.GetPassiveLogWriter()->IsPassiveLogger());
	}

	{
		log.ChangeOperativeWriter<DefaultOperativeLogWriter>();
		EXPECT_FALSE(log.GetOperativeLogWriter()->IsPassiveLogger());

		log.ChangeOperativeWriter<DefaultPassiveLogWriter>();
		EXPECT_TRUE(log.GetOperativeLogWriter()->IsPassiveLogger());
	}

	operative = Logger<>::GetNewDefaultOperativeWriter();
	passive = Logger<>::GetNewDefaultPassiveWriter();
	{
		log.ChangeWriters(std::move(operative), std::move(passive));
		EXPECT_FALSE(log.GetOperativeLogWriter()->IsPassiveLogger());
		EXPECT_TRUE(log.GetPassiveLogWriter()->IsPassiveLogger());
	}

	{
		log.ChangeWriters<DefaultOperativeLogWriter, DefaultPassiveLogWriter>();
		EXPECT_FALSE(log.GetOperativeLogWriter()->IsPassiveLogger());
		EXPECT_TRUE(log.GetPassiveLogWriter()->IsPassiveLogger());
	}

	{
		log.ChangeWriters<DefaultPassiveLogWriter, DefaultOperativeLogWriter>();
		log.ChangeToDefaultWriters();
		EXPECT_FALSE(log.GetOperativeLogWriter()->IsPassiveLogger());
		EXPECT_TRUE(log.GetPassiveLogWriter()->IsPassiveLogger());
	}
}

/*
TEST(Logger, )
{
	EXPECT_EQ(1, 1);
}
*/
