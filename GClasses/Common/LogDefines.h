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

///General macro logic
//Counts arguments
#define COUNT_ARGUMENTS(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

//Concatenate arguments
#define CONCAT_HELPER(a, b) a##b
#define CONCAT(a, b) CONCAT_HELPER(a, b)

#define STRINGIFY_HELPER(...) #__VA_ARGS__
#define STRINGIFY(...) STRINGIFY_HELPER(__VA_ARGS__)

//Puts the given arguments if the given condition is met
#define DO_IF_OPTION_true(...) __VA_ARGS__
#define DO_IF_OPTION_false(...)
#define DO_IF(cond, ...) CONCAT(DO_IF_OPTION_, cond)(__VA_ARGS__)

//Switches based on the number given and defineName body (same as DO_IF but more manual)
#define DO_SWITCH(defineName, number, ...) CONCAT(defineName, number)(__VA_ARGS__)


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

#define PP_HAS_ARGS_IMPL2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define PP_HAS_ARGS_SOURCE() 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define PP_HAS_ARGS_IMPL(...) PP_HAS_ARGS_IMPL2(__VA_ARGS__)
#define PP_HAS_ARGS(...)      PP_HAS_ARGS_IMPL(__VA_ARGS__, PP_HAS_ARGS_SOURCE())

#define GLOG_INTERNAL_LOG_VARS_DISAMBIGUATE2(level, logger, has_args, ...) GLOG_INTERNAL_LOG_VARS_ ## has_args (level, logger, __VA_ARGS__)
#define GLOG_INTERNAL_LOG_VARS_DISAMBIGUATE(level, logger, has_args, ...) GLOG_INTERNAL_LOG_VARS_DISAMBIGUATE2(level, logger, has_args, __VA_ARGS__)
#define GLOG_INTERNAL_LOG_VARS(level, logger, ...) GLOG_INTERNAL_LOG_VARS_DISAMBIGUATE(level, logger, PP_HAS_ARGS(__VA_ARGS__), __VA_ARGS__)

//Final definitions
#define LOG_VARS(level, logger, ...)    GLOG_INTERNAL_LOG_VARS(level, logger, __VA_ARGS__)
#define LOGV(level, logger, ...)        GLOG_INTERNAL_LOG_VARS(level, logger, __VA_ARGS__)
#define LOG_MSG(level, logger, ...)     GLOG_INTERNAL_LOG_MSG(level, logger, __VA_ARGS__)
#define LOG(level, logger, ...)         GLOG_INTERNAL_LOG_MSG(level, logger, __VA_ARGS__)

//Custom logging support
#define DO_IF_LOG_LOGIC(level, ...) GLOG_INTERNAL_LOG_LOGIC_NO_SOURCE(level, __VA_ARGS__)


#endif // LOGDEFINES_H
