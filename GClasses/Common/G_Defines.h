#ifndef G_DEFINES_H
#define G_DEFINES_H

#include <chrono>
#include <type_traits>
#include <memory>

///General defines
#ifndef NULL
#define NULL 0
#endif // NULL

#define NULLPTR (nullptr)

///Conditions
#ifndef FALSE
#define FALSE                   false
#endif
#ifndef TRUE
#define TRUE                    true
#endif
#define NOT(expression)         !(expression)

///Pointer readability defines
#define REFERENCE               &
#define REF                     REFERENCE
#define PASS_BY_REFERENCE       REFERENCE
#define PASS_REF                PASS_BY_REFERENCE
#define PASS_RVALUE_REFERENCE   &&
#define PASS_RVALUE_REF         PASS_RVALUE_REFERENCE

///Function readability defines
#define RETURN_REFERENCE        &
#define RETURN_REF              RETURN_REFERENCE
#define RETURN_POINTER          *
#define RETURN_PTR              RETURN_POINTER
#define MUST_OVERRIDE           0


//Is used to mark that a retunr type is a pointer; Has no actual impact; Mostly used in template functions with unspecified argument types to mark the type
#define CLASSIFIED_AS_POINTER

#define DEREFERENCE_TYPE(Type)   std::remove_pointer<Type>::type

///Pointer readablity define functions
#define ADDRESS(item)           (std::addressof(item))
#define POINTER                 *
#define POINTER_VALUE(pointer)  (*pointer)

///Info define funtions
#define IS_NULL(pointer)        (((pointer)==(NULLPTR)) ? (true) : (false))
#define IS_NOT_NULL(pointer)    (((pointer)==(NULLPTR)) ? (false) : (true))

#define IS_EQUAL(a,b)           (((a) == (b)) ? (true) : (false))
#define IS_NOT_EQUAL(a,b)       (((a) == (b)) ? (false) : (true))
#define IS_BIGGER(a,b)          (((a)>(b)) ? (true) : (false))
#define IS_SMALLER(a,b)         (((a)>(b)) ? (false): (true))

#define GET_BIGGER(a,b)         (((a)>(b)) ? (a):(b))
#define GET_SMALLER(a,b)        (((a)<(b)) ? (a):(b))
#define MAX(a,b)                GET_BIGGER(a,b)
#define MIN(a,b)                GET_SMALLER(a,b)


///Getting the Biggest and the Smallest
#define GET_BIGGEST_OF_1(a)                     (a)
#define GET_BIGGEST_OF_2(a,b)                   GET_BIGGER(GET_BIGGEST_OF_1(a), b)
#define GET_BIGGEST_OF_3(a,b,c)                 GET_BIGGER(GET_BIGGEST_OF_2(a,b), c)
#define GET_BIGGEST_OF_4(a,b,c,d)               GET_BIGGER(GET_BIGGEST_OF_3(a,b,c), d)
#define GET_BIGGEST_OF_5(a,b,c,d,e)             GET_BIGGER(GET_BIGGEST_OF_4(a,b,c,d), e)
#define GET_BIGGEST_OF_6(a,b,c,d,e,f)           GET_BIGGER(GET_BIGGEST_OF_5(a,b,c,d,e), f)
#define GET_BIGGEST_OF_7(a,b,c,d,e,f,g)         GET_BIGGER(GET_BIGGEST_OF_6(a,b,c,d,e,f), g)
#define GET_BIGGEST_OF_8(a,b,c,d,e,f,g,h)       GET_BIGGER(GET_BIGGEST_OF_7(a,b,c,d,e,f,g), h)
#define GET_BIGGEST_OF_9(a,b,c,d,e,f,g,h,i)     GET_BIGGER(GET_BIGGEST_OF_8(a,b,c,d,e,f,g,h), i)
#define GET_BIGGEST_OF_10(a,b,c,d,e,f,g,h,i,j)  GET_BIGGER(GET_BIGGEST_OF_9(a,b,c,d,e,f,g,h,i), j)

#define GET_SMALLEST_OF_1(a)                    (a)
#define GET_SMALLEST_OF_2(a,b)                  GET_SMALLER(GET_SMALLEST_OF_1(a), b)
#define GET_SMALLEST_OF_3(a,b,c)                GET_SMALLER(GET_SMALLEST_OF_2(a,b), c)
#define GET_SMALLEST_OF_4(a,b,c,d)              GET_SMALLER(GET_SMALLEST_OF_3(a,b,c), d)
#define GET_SMALLEST_OF_5(a,b,c,d,e)            GET_SMALLER(GET_SMALLEST_OF_4(a,b,c,d), e)
#define GET_SMALLEST_OF_6(a,b,c,d,e,f)          GET_SMALLER(GET_SMALLEST_OF_5(a,b,c,d,e), f)
#define GET_SMALLEST_OF_7(a,b,c,d,e,f,g)        GET_SMALLER(GET_SMALLEST_OF_6(a,b,c,d,e,f), g)
#define GET_SMALLEST_OF_8(a,b,c,d,e,f,g,h)      GET_SMALLER(GET_SMALLEST_OF_7(a,b,c,d,e,f,g), h)
#define GET_SMALLEST_OF_9(a,b,c,d,e,f,g,h,i)    GET_SMALLER(GET_SMALLEST_OF_8(a,b,c,d,e,f,g,h), i)
#define GET_SMALLEST_OF_10(a,b,c,d,e,f,g,h,i,j) GET_SMALLER(GET_SMALLEST_OF_9(a,b,c,d,e,f,g,h,i), j)


///Class members and functions
//Defines a default value for access (in a class is private)
#define CLASS_DEFAULT_ACCESS private

//Resets the dafault value for the access
//(Is ment to be used after a command like below)
#define RESTORE_CLASS_DEFAULT_ACCESS CLASS_DEFAULT_ACCESS:

//Declares a new Member of the Access access of the given Type and Name
#define CLASS_MEMBER(Type, Name, Access) Access: Type Name; CLASS_DEFAULT_ACCESS:

//Declares a specific Get functions for already declared member
#define CLASS_MEMBER_FUNCTION_GET_PTR(Type, Name, Access)   Access: Type RETURN_POINTER Get##Name##Ptr()    {return ADDRESS(Name);}   RESTORE_CLASS_DEFAULT_ACCESS
#define CLASS_MEMBER_FUNCTION_GET_REF(Type, Name, Access)   Access: Type RETURN_REF     Get##Name##Ref()    {return Name;}            RESTORE_CLASS_DEFAULT_ACCESS
#define CLASS_MEMBER_FUNCTION_GET(Type, Name, Access)       Access: Type                Get##Name()         {return Name;}            RESTORE_CLASS_DEFAULT_ACCESS

//Declares a specific Set functions for already declared member
#define CLASS_MEMBER_FUNCTION_SET_REF(Type, Name, Access)               Access: void Set##Name##Ref(Type PASS_REF otherRef) {Name = otherRef;} RESTORE_CLASS_DEFAULT_ACCESS
#define CLASS_MEMBER_FUNCTION_SET(Type, Name, Access)                   Access: void Set##Name(Type other)                  {Name = other;} RESTORE_CLASS_DEFAULT_ACCESS
#define CLASS_MEMBER_FUNCTION_RESET(Type, Name, DefaultValue, Access)   Access: void Reset##Name(Type other = DefaultValue)   {Name = other;} RESTORE_CLASS_DEFAULT_ACCESS

//Declares a new Member of the Access access of the given Type and Name
//Declares a specific Get and Set functions the declared member
#define CLASS_MEMBER_GET(Type, Name, MemberAccess, FunctionsAccess) \
    CLASS_MEMBER(Type, Name, MemberAccess) \
    CLASS_MEMBER_FUNCTION_GET(Type, Name, FunctionsAccess)

//Declares a new Member of the Access access of the given Type and Name
//Declares a specific Get and Set functions the declared member
#define CLASS_MEMBER_GET_SET(Type, Name, MemberAccess, FunctionsAccess) \
    CLASS_MEMBER_GET(Type, Name, MemberAccess, FunctionsAccess) \
    CLASS_MEMBER_FUNCTION_SET(Type, Name, FunctionsAccess)

//Declares a new Member of the Access access of the given Type and Name
//Declares a specific Get and Reset functions the declared member
#define CLASS_MEMBER_GET_RESET(Type, Name, DefaultValue, MemberAccess, FunctionsAccess) \
    CLASS_MEMBER_GET(Type, Name, MemberAccess, FunctionsAccess) \
    CLASS_MEMBER_FUNCTION_RESET(Type, Name, DefaultValue, FunctionsAccess)

//Declares a new Member of the Access access of the given Type and Name
//Declares a specific Get and Set functions the declared member
//Has more get and set functions
#define CLASS_MEMBER_RICH_GET_SET(Type, Name, MemberAccess, FunctionsAccess) \
    CLASS_MEMBER_GET_SET(Type, Name, MemberAccess, FunctionsAccess) \
    \
    CLASS_MEMBER_FUNCTION_GET_REF(Type, Name, FunctionsAccess) \
    CLASS_MEMBER_FUNCTION_GET_PTR(Type, Name, FunctionsAccess) \
    \
    CLASS_MEMBER_FUNCTION_SET_REF(Type, Name, FunctionsAccess)



///DELETE
///Speed testing
#define FUNCTION_TIME(func, iterations, time, start_time_point, end_time_point, units)  \
    start_time_point = std::chrono::steady_clock::now(); \
    for(u32 i = 0; i < (iterations); i++) { func; }; \
    end_time_point = std::chrono::steady_clock::now(); \
    time = std::chrono::duration_cast<units>(end_time_point - start_time_point).count();

#define FUNCTION_TIME_MICRO_S(func, iterations, time, start_time_point, end_time_point) \
    FUNCTION_TIME(func, iterations, time, start_time_point, end_time_point, std::chrono::microseconds);

#define FUNCTION_TIME_MILLI_S(func, iterations, time, start_time_point, end_time_point) \
    FUNCTION_TIME(func, iterations, time, start_time_point, end_time_point, std::chrono::milliseconds);

#define FUNCTION_TIME_SECONDS(func, iterations, time, start_time_point, end_time_point) \
    FUNCTION_TIME(func, iterations, time, start_time_point, end_time_point, std::chrono::seconds);

#define FUNCTION_TIME_MINUTES(func, iterations, time, start_time_point, end_time_point) \
    FUNCTION_TIME(func, iterations, time, start_time_point, end_time_point, std::chrono::minutes);


///Testing if a function is defined in scope of a class
//CHANGE

//Creates a check for a specific function
//This check is later needed to test if a function is defined in scope of a class
#define GENERATE_FUNC_DEFINITION_CHECK(FUNC_NAME) \
namespace FuncPosession \
{ \
    template<typename, typename T> \
    struct FUNC_NAME { \
        static_assert( \
            std::integral_constant<T, false>::value, \
            "Second template parameter needs to be of function type."); \
    }; \
    template<typename C, typename Ret, typename... Args> \
    struct FUNC_NAME<C, Ret(Args...)> { \
    private: \
        template<typename T> \
        static constexpr auto check(T*) \
        -> typename \
            std::is_same< \
                decltype( std::declval<T>().FUNC_NAME( std::declval<Args>()... ) ), \
                Ret \
            >::type; \
        template<typename> \
        static constexpr std::false_type check(...); \
        typedef decltype(check<C>(0)) type; \
    public: \
        static constexpr bool value = type::value; \
    }; \
}

//Uses previously created check (FUNC_NAME's must be identical) to check in a class CLASS_TYPE for the specific function
//If function is found returns true
#define IS_FUNC_DEFINED_IN_CLASS(FUNC_NAME, CLASS_TYPE, RET_TYPE, ARGS) FuncPosession::FUNC_NAME<CLASS_TYPE, RET_TYPE(ARGS)>::value

//Example of use:
/*
    struct X
    {
        int serialize(const std::string&) { return 42; }
    };

    struct Y : X {};

    struct Z
    {
        double serialize(const std::string&) { return 42; }
    };

    GENERATE_FUNC_DEFINITION_CHECK(serialize);

    void SomeFunction()
    {
        IS_FUNC_DEFINED_IN_CLASS(serialize, X, int, const std::string&); //Returns TRUE //Function "serialize" is defined in X
        IS_FUNC_DEFINED_IN_CLASS(serialize, Y, int, const std::string&); //Returns TRUE //Function "serialize" is not defined in Y but is inherited
        IS_FUNC_DEFINED_IN_CLASS(serialize, Z, int, const std::string&); //Returns FALSE //Function "serialize" is not defined or inherited in Z
    }
 */
#endif // G_DEFINES_H
