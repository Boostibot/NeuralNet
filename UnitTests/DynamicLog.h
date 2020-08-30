#ifndef DYNAMICLOG_H
#define DYNAMICLOG_H

#include "Catch2/Catch.hpp"
#include "GClasses/Common/Log.h"

#include "Libraries/Fmt/fmt/os.h"
//#include "Libraries/Fmt/fmt/core.h"
//#include "Libraries/Fmt/fmt/compile.h"
namespace DynamicLogger
{
    TEST_CASE("Setting and Getting logging levels", "[DynamicLogger]")
    {
        Logger log;

        //
        REQUIRE(log.IsLoggingLevelEnabled<3>() == true);
        REQUIRE(log.IsLoggingLevelEnabled(3) == true);

        REQUIRE_THROWS(log.IsLoggingLevelEnabled(Logger<>::LevelArraySize + 1));
        //REQUIRE_THROWS(log.IsLoggingLevelEnabled<Logger<>::LevelArraySize + 1>()); //will produce static_assertion thus works

        //
        log.DoLoggingLevel(3, false);

        REQUIRE(log.IsLoggingLevelEnabled<3>() == false);
        REQUIRE(log.IsLoggingLevelEnabled(3) == false);

        log.DoLoggingLevel(3, true);

        REQUIRE(log.IsLoggingLevelEnabled<3>() == true);
        REQUIRE(log.IsLoggingLevelEnabled(3) == true);

        log.DoLoggingLevel<3>(false);

        REQUIRE(log.IsLoggingLevelEnabled<3>() == false);
        REQUIRE(log.IsLoggingLevelEnabled(3) == false);

        log.DoLoggingLevel<3>(true);

        REQUIRE(log.IsLoggingLevelEnabled<3>() == true);
        REQUIRE(log.IsLoggingLevelEnabled(3) == true);

        REQUIRE_THROWS(log.DoLoggingLevel(Logger<>::LevelArraySize + 1, true));
        //REQUIRE_THROWS(log.DoLoggingLevel<Logger<>::LevelArraySize + 1>(true)); //will produce static_assertion thus works

        //
        log.DoLoggingLevels(false, 3, 2, 1);
        REQUIRE(log.IsLoggingLevelEnabled<3>() == false);
        REQUIRE(log.IsLoggingLevelEnabled<2>() == false);
        REQUIRE(log.IsLoggingLevelEnabled<1>() == false);

        log.DoLoggingLevels(false, 1, 3, 2);
        REQUIRE(log.IsLoggingLevelEnabled<3>() == false);
        REQUIRE(log.IsLoggingLevelEnabled<2>() == false);
        REQUIRE(log.IsLoggingLevelEnabled<1>() == false);

        log.DoLoggingLevels(true, 3, 2, 3, 2);
        REQUIRE(log.IsLoggingLevelEnabled<3>() == true);
        REQUIRE(log.IsLoggingLevelEnabled<2>() == true);

        log.DoLoggingLevels<3, 2, 1>(false);
        REQUIRE(log.IsLoggingLevelEnabled<3>() == false);
        REQUIRE(log.IsLoggingLevelEnabled<2>() == false);
        REQUIRE(log.IsLoggingLevelEnabled<1>() == false);

        log.DoLoggingLevels<1, 3, 2>(false);
        REQUIRE(log.IsLoggingLevelEnabled<3>() == false);
        REQUIRE(log.IsLoggingLevelEnabled<2>() == false);
        REQUIRE(log.IsLoggingLevelEnabled<1>() == false);

        log.DoLoggingLevels<3, 2, 3, 2>(true);
        REQUIRE(log.IsLoggingLevelEnabled<3>() == true);
        REQUIRE(log.IsLoggingLevelEnabled<2>() == true);

        REQUIRE_THROWS(log.DoLoggingLevels(true, Logger<>::LevelArraySize + 1, 3, 2));
        //log.DoLoggingLevels<Logger<>::LevelArraySize + 1, 3, 2>(true); //will produce static_assertion thus works
    }

    TEST_CASE("Setting and Getting logging status", "[DynamicLogger]")
    {
        Logger log;

        REQUIRE(log.IsLoggingEnabled() == true);
        REQUIRE(log.IsLoggingEnabled() == true);

        log.DisableLog();
        REQUIRE(log.IsLoggingEnabled() == false);

        log.DisableLog();
        REQUIRE(log.IsLoggingEnabled() == false);

        log.EnableLog();
        REQUIRE(log.IsLoggingEnabled() == true);

        log.DoLog(false);
        REQUIRE(log.IsLoggingEnabled() == false);

        log.DoLog(true);
        REQUIRE(log.IsLoggingEnabled() == true);
    }

    TEST_CASE("Constructing Logger", "[DynamicLogger]")
    {
        Logger<> log;

        REQUIRE(log.IsLoggingEnabled() == true);

        REQUIRE(log.GetOperativeLogWriter() != nullptr);
        REQUIRE(log.GetPassiveLogWriter() != nullptr);

        REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == false);
        REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == true);

        for (u32 i = 0; i < Logger<>::LevelArraySize; i++)
        {
            REQUIRE(log.IsLoggingLevelEnabled(i) == true);
            REQUIRE(log.GetLevelPointer(i) != nullptr);
        }

        REQUIRE(log.IsLoggingLevelEnabled<1>() == true);
        REQUIRE(log.IsLoggingLevelEnabled<2>() == true);
        REQUIRE(log.IsLoggingLevelEnabled<3>() == true);
        REQUIRE(log.IsLoggingLevelEnabled<4>() == true);
        REQUIRE(log.IsLoggingLevelEnabled<5>() == true);
        REQUIRE(log.IsLoggingLevelEnabled<14>() == true);
        //REQUIRE_THROWS(log.IsLoggingLevelEnabled<Logger<>::LevelArraySize + 1>());

        REQUIRE(log.GetLevelPointer<1>() != nullptr);
        REQUIRE(log.GetLevelPointer<2>() != nullptr);
        REQUIRE(log.GetLevelPointer<3>() != nullptr);
        REQUIRE(log.GetLevelPointer<14>() != nullptr);
        //REQUIRE_THROWS(log.GetLevelPointer<Logger<>::LevelArraySize + 1>()); //will produce static_assertion thus works

    }

    TEST_CASE("Static function calling", "[DynamicLogger]")
    {
        std::unique_ptr<LogWriterInterface> temp1, temp2, temp3, temp4;

        temp1 = Logger<>::GetNewLogWritter<DefaultOperativeLogWriter>();
        REQUIRE(temp1.get()->IsPassiveLogger() == false);
        REQUIRE(temp1.get() != nullptr);
        temp1 = Logger<>::GetNewLogWritter<DefaultPassiveLogWriter>();
        REQUIRE(temp1.get()->IsPassiveLogger() == true);
        REQUIRE(temp1.get() != nullptr);

        temp3 = Logger<>::GetNewLogWritter<DefaultOperativeLogWriter>();
        temp4 = Logger<>::GetNewLogWritter<DefaultOperativeLogWriter>();
        REQUIRE(temp3 != temp4);

        temp2 = Logger<>::GetNewDefaultOperativeWriter();
        REQUIRE(temp2.get()->IsPassiveLogger() == false);
        REQUIRE(temp2.get() != nullptr);
        temp2 = Logger<>::GetNewDefaultPassiveWriter();
        REQUIRE(temp2.get()->IsPassiveLogger() == true);
        REQUIRE(temp2.get() != nullptr);

        temp3 = Logger<>::GetNewDefaultOperativeWriter();
        temp4 = Logger<>::GetNewDefaultOperativeWriter();
        REQUIRE(temp3 != temp4);

        REQUIRE(Logger<>::IsIndexInLevelArrayRange(0) == true);
        REQUIRE(Logger<>::IsIndexInLevelArrayRange(Logger<>::LevelArraySize - 1) == true);
        REQUIRE(Logger<>::IsIndexInLevelArrayRange(Logger<>::LevelArraySize + 1) == false);
    }

    TEST_CASE("Setting and Getting LogWriter Pointers", "[DynamicLogger]")
    {
        //TODO
        //- SPLIT
        //- ADD CHECKING FOR REBUILDING THE POINTER ARRAY
        //- ADD CHECKING FOR SETTING TO EMPTY POINTER AND THE POINTER ARRAY!!
        Logger<> log;

        REQUIRE(log.GetOperativeLogWriter() != nullptr);
        REQUIRE(log.GetPassiveLogWriter() != nullptr);

        REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == false);
        REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == true);

        std::unique_ptr<int> ptr;

        std::unique_ptr<LogWriterInterface> operative = Logger<>::GetNewDefaultOperativeWriter();
        std::unique_ptr<LogWriterInterface> passive = Logger<>::GetNewDefaultOperativeWriter();
        LogWriterInterface POINTER location1 = operative.get();
        LogWriterInterface POINTER location2 = log.GetOperativeLogWriter();

        REQUIRE(location1 != location2);
        {
            log.ChangeOperativeWriter(std::move(operative));
            REQUIRE(operative.get() == nullptr);
            REQUIRE(location1 == log.GetOperativeLogWriter());
            REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == false);

            log.ChangeOperativeWriter(std::move(passive));
            REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == false);
        }

        operative = Logger<>::GetNewDefaultOperativeWriter();
        passive = Logger<>::GetNewDefaultPassiveWriter();
        {
            log.ChangePassiveWriter(std::move(operative));
            REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == false);

            log.ChangePassiveWriter(std::move(passive));
            REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == true);
        }

        {
            log.ChangePassiveWriter<DefaultOperativeLogWriter>();
            REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == false);

            log.ChangePassiveWriter<DefaultPassiveLogWriter>();
            REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == true);
        }

        {
            log.ChangeOperativeWriter<DefaultOperativeLogWriter>();
            REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == false);

            log.ChangeOperativeWriter<DefaultPassiveLogWriter>();
            REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == true);
        }

        operative = Logger<>::GetNewDefaultOperativeWriter();
        passive = Logger<>::GetNewDefaultPassiveWriter();
        {
            log.ChangeWriters(std::move(operative), std::move(passive));
            REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == false);
            REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == true);
        }

        {
            log.ChangeWriters<DefaultOperativeLogWriter, DefaultPassiveLogWriter>();
            REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == false);
            REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == true);
        }

        {
            log.ChangeWriters<DefaultPassiveLogWriter, DefaultOperativeLogWriter>();
            log.ChangeToDefaultWriters();
            REQUIRE(log.GetOperativeLogWriter()->IsPassiveLogger() == false);
            REQUIRE(log.GetPassiveLogWriter()->IsPassiveLogger() == true);
        }
    }
}
#endif // DYNAMICLOG_H
