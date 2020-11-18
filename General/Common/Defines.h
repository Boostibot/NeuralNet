#ifndef G_DEFINES_H
#define G_DEFINES_H

#include <memory>

//Operator readability
#define NOT                     !
#define REFERENCE               &
#define REF                     REFERENCE
#define RVALUE_REFERENCE        &&
#define RVALUE_REF              RVALUE_REFERENCE
#define MUST_OVERRIDE           0

//Argument intent
#ifdef IN
#undef IN
#endif

#ifdef OUT
#undef OUT
#endif

#define IN                      const REF
#define IN_COPY                 const
#define OUT                     REF
#define INOUT                   REF
#define FORWARD                 RVALUE_REF
#define MOVE                    RVALUE_REF

//Header support
#define HEADER_ONLY             inline

//Logic
#define AND                     &&
#define OR                      ||

//Is used to mark that a retunr type is a pointer; Has no actual impact; Mostly used in template functions with unspecified argument types to mark the type
#define CLASSIFIED_AS_POINTER

//Pointer readablity define functions
#define ADDRESS(item)           (std::addressof(item))
#define POINTER                 *
#define PTR                     POINTER
#define POINTER_VALUE(pointer)  (*pointer)
#define PTR_VAL(pointer)        (*pointer)

//Concatenate arguments
#define CONCAT_HELPER(a, b) a##b
#define CONCAT(a, b) CONCAT_HELPER(a, b)

//Other defines
#define STRINGIFY_HELPER(...) #__VA_ARGS__
#define STRINGIFY(...) STRINGIFY_HELPER(__VA_ARGS__)

//Required for msvc support
#define ECHO(...) __VA_ARGS__
#define PASS_ON(...)
#define CALL(name, args) name args
#define EXPAND(x) x
#define CONSUME(...)

//Debug mode
#ifdef NDEBUG
#define IS_IN_DEBUG_MODE false
#else
#define IS_IN_DEBUG_MODE true
#endif

//Counts arguments
#define COUNT_ARGUMENTS(...) \
    EXPAND(EXPAND(PP_NARG_) ( __VA_ARGS__, PP_RSEQ_N()))

#define PP_NARG_(...) \
         EXPAND(EXPAND(PP_ARG_N)(__VA_ARGS__))
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

//Puts the given arguments if the given condition is met
#define DO_IF_OPTION_true(...) __VA_ARGS__
#define DO_IF_OPTION_false(...)
#define DO_IF(cond, ...) CONCAT(DO_IF_OPTION_, cond)(__VA_ARGS__)

//Switches based on the number given and defineName body (same as DO_IF but more manual)
#define DO_SWITCH(defineName, number, ...) CONCAT(defineName, number)(__VA_ARGS__)


#endif // G_DEFINES_H
