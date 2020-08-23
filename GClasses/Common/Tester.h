#ifndef TESTER_H
#define TESTER_H

#include "LogDefines.h"


#define UNIQUE_NAME(name) CONCAT(name, __COUNTER__)

#include <map>
#include <vector>
#include <string>
#include "G_Common.h"

/*
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "Catch/Catch.hpp"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
*/

#define G_UNIQUE_NAME(name) CONCAT(name, __COUNTER__)

#define G_INTERNAL_TEST_CASE(name, description)  \
struct name \
{ \
     \
    using ThisType = name; \
    static constexpr char Description [] = description; \
    void Execute();\
}; \
/* Insert into tester class here */    \
void name::Execute()


#define G_INTERNAL_ASSERT(name, description, expr) std::cout << STRINGIFY(name) << " " << description << " " << STRINGIFY(expr) << std::endl

#define G_TEST_CASE(description) G_INTERNAL_TEST_CASE(G_UNIQUE_NAME(TestCase), description)

#define G_ASSERT(expr) \
    G_INTERNAL_ASSERT(ThisType, ThisType::Description, expr)

/*
 * G_TEST_CASE("Vector size testing")
 * {
 *      vector<int> vec(3);
 *      G_ASSERT(vec.size() == 3);
 * }};
 *
 *
 *
 *
 *
 *
 */

/*
#define INTERNAL_CATCH_TESTCASE2( TestName, ... ) \
    static void TestName(); \
    CATCH_INTERNAL_START_WARNINGS_SUPPRESSION \
    CATCH_INTERNAL_SUPPRESS_GLOBALS_WARNINGS \
    namespace
{
        Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar )( Catch::makeTestInvoker( &TestName ),
                                                                    CATCH_INTERNAL_LINEINFO,
                                                                    Catch::StringRef(),
                                                                    Catch::NameAndTags{ __VA_ARGS__ } );
}  \
    CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION \
    static void TestName()
*/

#endif // TESTER_H
