#ifndef LOGDEFINES_H
#define LOGDEFINES_H

#include "Log.h"

#define DO_LOG true
#define DO_SOURCE_LOG true

#define DO_LOG_LEVEL_0 true
#define DO_LOG_LEVEL_1 true
#define DO_LOG_LEVEL_2 true
#define DO_LOG_LEVEL_3 true
#define DO_LOG_LEVEL_4 true
#define DO_LOG_LEVEL_5 true
#define DO_LOG_LEVEL_6 true
#define DO_LOG_LEVEL_7 true
#define DO_LOG_LEVEL_8 true
#define DO_LOG_LEVEL_9 true
#define DO_LOG_LEVEL_10 true
#define DO_LOG_LEVEL_11 true
#define DO_LOG_LEVEL_12 true
#define DO_LOG_LEVEL_13 true
#define DO_LOG_LEVEL_14 true
#define DO_LOG_LEVEL_15 true

///Log logic
//Checks DO_IF with the concatenated version of conditionVar and number
#define CHECK_IF_SWITCH(conditionVar, number, ...) DO_IF(CONCAT(conditionVar, number), __VA_ARGS__)

//goes throuh all of the logic of logging and calls indetifier##true/false with the argument pack
#define GLOG_INTERNAL_LOG_LOGIC(level, identifier, ...)  DO_IF(DO_LOG, \
                                                      CHECK_IF_SWITCH(DO_LOG_LEVEL_, level,  \
                                                                      CONCAT(identifier, DO_SOURCE_LOG)(__VA_ARGS__) \
                                                                     ) \
                                                      )
#define GLOG_INTERNAL_LOG_LOGIC_NO_SOURCE(level, ...)  DO_IF(DO_LOG, \
                                                      CHECK_IF_SWITCH(DO_LOG_LEVEL_, level,  \
                                                                      __VA_ARGS__ \
                                                                     ) \
                                                      )

#define GLOG_INTERNAL_LOG_MSG_DO_SOURCE_OPTION_true(level, logger, ...) logger.LogMsgSource<level>(__FILE__, __LINE__, __VA_ARGS__)
#define GLOG_INTERNAL_LOG_MSG_DO_SOURCE_OPTION_false(level, logger, ...) logger.LogMsg<level>(__VA_ARGS__)
#define GLOG_INTERNAL_LOG_MSG(level, logger, ...) GLOG_INTERNAL_LOG_LOGIC(level, GLOG_INTERNAL_LOG_MSG_DO_SOURCE_OPTION_, level, logger, __VA_ARGS__)

#define GLOG_INTERNAL_LOG_VARS_DO_SOURCE_OPTION_true(level, logger, ...) logger.LogVarsSource<level>(__FILE__, __LINE__, __VA_ARGS__)
#define GLOG_INTERNAL_LOG_VARS_DO_SOURCE_OPTION_false(level, logger, ...) logger.LogVars<level>(__VA_ARGS__)
#define GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, ...) GLOG_INTERNAL_LOG_LOGIC(level, GLOG_INTERNAL_LOG_VARS_DO_SOURCE_OPTION_, level, logger, __VA_ARGS__)


//Log vars
#define GLOG_INTERNAL_LOG_VARS_0(level, logger) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger)
#define GLOG_INTERNAL_LOG_VARS_1(level, logger, var1) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1)
#define GLOG_INTERNAL_LOG_VARS_2(level, logger, var1, var2) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2)
#define GLOG_INTERNAL_LOG_VARS_3(level, logger, var1, var2, var3) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3)
#define GLOG_INTERNAL_LOG_VARS_4(level, logger, var1, var2, var3, var4) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4)
#define GLOG_INTERNAL_LOG_VARS_5(level, logger, var1, var2, var3, var4, var5) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5)
#define GLOG_INTERNAL_LOG_VARS_6(level, logger, var1, var2, var3, var4, var5, var6) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6)
#define GLOG_INTERNAL_LOG_VARS_7(level, logger, var1, var2, var3, var4, var5, var6, var7) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7)
#define GLOG_INTERNAL_LOG_VARS_8(level, logger, var1, var2, var3, var4, var5, var6, var7, var8) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8)
#define GLOG_INTERNAL_LOG_VARS_9(level, logger, var1, var2, var3, var4, var5, var6, var7, var8, var9) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9)
#define GLOG_INTERNAL_LOG_VARS_10(level, logger, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9, #var10, var10)
#define GLOG_INTERNAL_LOG_VARS_11(level, logger, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9, #var10, var10, #var11, var11)
#define GLOG_INTERNAL_LOG_VARS_12(level, logger, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9, #var10, var10, #var11, var11, #var12, var12)
#define GLOG_INTERNAL_LOG_VARS_13(level, logger, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9, #var10, var10, #var11, var11, #var12, var12, #var13, var13)
#define GLOG_INTERNAL_LOG_VARS_14(level, logger, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9, #var10, var10, #var11, var11, #var12, var12, #var13, var13, #var14, var14)
#define GLOG_INTERNAL_LOG_VARS_15(level, logger, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14, var15) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9, #var10, var10, #var11, var11, #var12, var12, #var13, var13, #var14, var14, #var15, var15)
#define GLOG_INTERNAL_LOG_VARS_16(level, logger, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14, var15, var16) GLOG_INTERNAL_LOG_VARS_LOGIC(level, logger, #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9, #var10, var10, #var11, var11, #var12, var12, #var13, var13, #var14, var14, #var15, var15, #var16, var16)
//pattern: #var1, var1, #var2, var2, #var3, var3, #var4, var4, #var5, var5, #var6, var6, #var7, var7, #var8, var8, #var9, var9, #var10, var10, #var11, var11, #var12, var12, #var13, var13, #var14, var14, #var15, var15, #var16, var16

#define GLOG_INTERNAL_LOG_VARS_CALL(level, logger, argumentCount, ...) EXPAND(CONCAT(GLOG_INTERNAL_LOG_VARS_, argumentCount)(level, logger, __VA_ARGS__))
#define GLOG_INTERNAL_LOG_VARS(level, logger, ...) EXPAND(EXPAND(GLOG_INTERNAL_LOG_VARS_CALL)(level, logger, COUNT_ARGUMENTS(__VA_ARGS__), __VA_ARGS__))

//Final definitions
#define LOG_VARS(level, logger, ...)    GLOG_INTERNAL_LOG_VARS(level, logger, __VA_ARGS__)
#define LOGV(level, logger, ...)        GLOG_INTERNAL_LOG_VARS(level, logger, __VA_ARGS__)
#define LOG_MSG(level, logger, ...)     GLOG_INTERNAL_LOG_MSG(level, logger, __VA_ARGS__)
#define LOG(level, logger, ...)         GLOG_INTERNAL_LOG_MSG(level, logger, __VA_ARGS__)

//Custom logging support
#define DO_IF_LOG(...)              DO_IF(DO_LOG, __VA_ARGS__)
#define DO_IF_LOG_LEVEL(level, ...) GLOG_INTERNAL_LOG_LOGIC_NO_SOURCE(level, __VA_ARGS__)
#define DO_IF_LOG_SOURCE(...)       DO_IF(DO_LOG, DO_IF(DO_SOURCE_LOG, __VA_ARGS__))

//Debug suppport
#define DO_IF_DEBUG(...)            DO_IF(IS_IN_DEBUG_MODE, __VA_ARGS__)


#endif // LOGDEFINES_H
