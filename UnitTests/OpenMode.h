#ifndef OPENMODETESTING_H
#define OPENMODETESTING_H

#include "Catch2/Catch.hpp"
#include "General/File/NewOpenMode.h"

namespace CIo::OpenModeTesting
{
    using namespace OpenModeHelpers;
}

#define OpenMode_TEST_COMPILE_ERROR false
#define OpenModeTestedTypes char8, charW

namespace CIo::OpenModeTesting
{
    namespace FlagPresenceTesting
    {
        TEST_CASE("[FlagPresence] : IsFlagPresent should return true if the looking for flag is present in the arguments", "[FlagPresence]")
        {

            WHEN("Given arguments containing the lookignForFlag it should return true")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Read) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::IfPossibleNoFlushingToDisk, OpenModeFlag::Append, OpenModeFlag::IfPossibleNoFlushingToDisk) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Write, OpenModeFlag::MustExist) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::RandomAccessOptimized, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::RandomAccessOptimized) == true);
            }
            WHEN("Given arguments containing the lookignForFlag multiple times it should return true")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Read, OpenModeFlag::Read) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Utf8Encoding, OpenModeFlag::Append, OpenModeFlag::Utf8Encoding, OpenModeFlag::Utf8Encoding) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Utf8Encoding, OpenModeFlag::Write, OpenModeFlag::Write) == true);
            }
            WHEN("Given arguments not containing the lookignForFlag it should return false")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, OpenModeFlag::IfPossibleNoFlushingToDisk, OpenModeFlag::Append) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Utf8Encoding, OpenModeFlag::RandomAccessOptimized, OpenModeFlag::Write) == false);
            }
            WHEN("Given arguments only lookingForType it should return false")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::NotInheritedByChildProcess) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::MustNotExist) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Text) == false);
            }
        }

        TEST_CASE("[FlagPresence] : the [] operator should return flag presence at index", "[FlagPresence]")
        {
            GIVEN("A FlagPresence constructed with certain flags")
            {
                FlagPresence presence(OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist, OpenModeFlag::DeleteAfterClose);
                WHEN("A [] operator is used with the value of constructed flag it should be present")
                {
                    REQUIRE(presence[OpenModeFlag::Write] == true);
                    REQUIRE(presence[OpenModeFlag::Append] == true);
                    REQUIRE(presence[OpenModeFlag::MustExist] == true);
                    REQUIRE(presence[OpenModeFlag::DeleteAfterClose] == true);
                }
                WHEN("A [] operator is used with the value of not constructed flag it should not be present")
                {
                    REQUIRE(presence[OpenModeFlag::Read] == false);
                    REQUIRE(presence[OpenModeFlag::Binary] == false);
                    REQUIRE(presence[OpenModeFlag::MustNotExist] == false);
                    REQUIRE(presence[OpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                }
            }
        }

        TEST_CASE("[FlagPresence] : Constructing FlagPresence should fill the presence array accordingly", "[FlagPresence]")
        {
            WHEN("Constructed with No flags")
            {
                FlagPresence presence;
                THEN("There should be no presence noted")
                {
                    for(OpenModeEnumsBaseType i = 0; i < OpenModeFlagsCount; i++)
                        CHECK(presence[static_cast<OpenModeFlag>(i)] == false);
                }
            }

            WHEN("Constructed with All flags")
            {
                FlagPresence presence(OpenModeFlag::Read,
                                      OpenModeFlag::Write,
                                      OpenModeFlag::Append,
                                      OpenModeFlag::MustExist,
                                      OpenModeFlag::Text,
                                      OpenModeFlag::Binary,
                                      OpenModeFlag::MustNotExist,
                                      OpenModeFlag::CommitDirectlyToDisk,
                                      OpenModeFlag::CommitIndirectlyToDisk,
                                      OpenModeFlag::NotInheritedByChildProcess,
                                      OpenModeFlag::SequntialAccessOptimized,
                                      OpenModeFlag::RandomAccessOptimized,
                                      OpenModeFlag::IfPossibleNoFlushingToDisk,
                                      OpenModeFlag::DeleteAfterClose,
                                      OpenModeFlag::UnicodeEncoding,
                                      OpenModeFlag::Utf8Encoding,
                                      OpenModeFlag::Utf16Encoding);

                THEN("All flags should be present")
                {
                    for(OpenModeEnumsBaseType i = 0; i < OpenModeFlagsCount; i++)
                        CHECK(presence[static_cast<OpenModeFlag>(i)] == true);
                }
            }

            WHEN("Constructed with a single argument")
            {
                THEN("There should be no presence except the one specified")
                {
                    SECTION("Flag OpenModeFlag::Read")
                    {
                        FlagPresence presence(OpenModeFlag::Read);
                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Read] == true);
                    }
                    SECTION("Flag OpenModeFlag::Write")
                    {
                        FlagPresence presence(OpenModeFlag::Write);
                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[OpenModeFlag::Write] == true);
                    }
                    SECTION("Flag OpenModeFlag::Append")
                    {
                        FlagPresence presence(OpenModeFlag::Append);
                        CHECK(presence[OpenModeFlag::MustExist] == false);
                        CHECK(presence[OpenModeFlag::Append] == true);
                    }
                    SECTION("Flag OpenModeFlag::MustExist")
                    {
                        FlagPresence presence(OpenModeFlag::MustExist);
                        CHECK(presence[OpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::MustExist] == true);
                    }
                    SECTION("Flag OpenModeFlag::Text")
                    {
                        FlagPresence presence(OpenModeFlag::Text);
                        CHECK(presence[OpenModeFlag::Binary] == false);
                        CHECK(presence[OpenModeFlag::Text] == true);
                    }
                    SECTION("Flag OpenModeFlag::Binary")
                    {
                        FlagPresence presence(OpenModeFlag::Binary);
                        CHECK(presence[OpenModeFlag::MustNotExist] == false);
                        CHECK(presence[OpenModeFlag::Binary] == true);
                    }
                    SECTION("Flag OpenModeFlag::MustNotExist")
                    {
                        FlagPresence presence(OpenModeFlag::MustNotExist);
                        CHECK(presence[OpenModeFlag::CommitDirectlyToDisk] == false);
                        CHECK(presence[OpenModeFlag::MustNotExist] == true);
                    }
                    SECTION("Flag OpenModeFlag::CommitDirectlyToDisk")
                    {
                        FlagPresence presence(OpenModeFlag::CommitDirectlyToDisk);
                        CHECK(presence[OpenModeFlag::CommitIndirectlyToDisk] == false);
                        CHECK(presence[OpenModeFlag::CommitDirectlyToDisk] == true);
                    }
                    SECTION("Flag OpenModeFlag::CommitIndirectlyToDisk")
                    {
                        FlagPresence presence(OpenModeFlag::CommitIndirectlyToDisk);
                        CHECK(presence[OpenModeFlag::NotInheritedByChildProcess] == false);
                        CHECK(presence[OpenModeFlag::CommitIndirectlyToDisk] == true);
                    }
                    SECTION("Flag OpenModeFlag::NotInheritedByChildProcess")
                    {
                        FlagPresence presence(OpenModeFlag::NotInheritedByChildProcess);
                        CHECK(presence[OpenModeFlag::SequntialAccessOptimized] == false);
                        CHECK(presence[OpenModeFlag::NotInheritedByChildProcess] == true);
                    }
                    SECTION("Flag OpenModeFlag::SequntialAccessOptimized")
                    {
                        FlagPresence presence(OpenModeFlag::SequntialAccessOptimized);
                        CHECK(presence[OpenModeFlag::RandomAccessOptimized] == false);
                        CHECK(presence[OpenModeFlag::SequntialAccessOptimized] == true);
                    }
                    SECTION("Flag OpenModeFlag::RandomAccessOptimized")
                    {
                        FlagPresence presence(OpenModeFlag::RandomAccessOptimized);
                        CHECK(presence[OpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[OpenModeFlag::RandomAccessOptimized] == true);
                    }
                    SECTION("Flag OpenModeFlag::IfPossibleNoFlushingToDisk")
                    {
                        FlagPresence presence(OpenModeFlag::IfPossibleNoFlushingToDisk);
                        CHECK(presence[OpenModeFlag::DeleteAfterClose] == false);
                        CHECK(presence[OpenModeFlag::IfPossibleNoFlushingToDisk] == true);
                    }
                    SECTION("Flag OpenModeFlag::DeleteAfterClose")
                    {
                        FlagPresence presence(OpenModeFlag::DeleteAfterClose);
                        CHECK(presence[OpenModeFlag::UnicodeEncoding] == false);
                        CHECK(presence[OpenModeFlag::DeleteAfterClose] == true);
                    }
                    SECTION("Flag OpenModeFlag::UnicodeEncoding")
                    {
                        FlagPresence presence(OpenModeFlag::UnicodeEncoding);
                        CHECK(presence[OpenModeFlag::Utf8Encoding] == false);
                        CHECK(presence[OpenModeFlag::UnicodeEncoding] == true);
                    }
                    SECTION("Flag OpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Utf8Encoding);
                        CHECK(presence[OpenModeFlag::Utf16Encoding] == false);
                        CHECK(presence[OpenModeFlag::Utf8Encoding] == true);
                    }
                    SECTION("Flag OpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[OpenModeFlag::Utf16Encoding] == true);
                    }
                }
            }

            WHEN("Constructed with multiple arguments")
            {
                THEN("There should be no presence except the one specified")
                {
                    SECTION("Flags OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[OpenModeFlag::DeleteAfterClose] == true);
                        CHECK(presence[OpenModeFlag::Utf16Encoding] == true);

                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[OpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::RandomAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::Binary, OpenModeFlag::Write, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Append")
                    {
                        FlagPresence presence(OpenModeFlag::Binary, OpenModeFlag::Write, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Append);
                        CHECK(presence[OpenModeFlag::Binary] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[OpenModeFlag::CommitDirectlyToDisk] == true);
                        CHECK(presence[OpenModeFlag::Append] == true);

                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[OpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::SequntialAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::Utf8Encoding);
                        CHECK(presence[OpenModeFlag::NotInheritedByChildProcess] == true);
                        CHECK(presence[OpenModeFlag::MustExist] == true);
                        CHECK(presence[OpenModeFlag::Utf8Encoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[OpenModeFlag::DeleteAfterClose] == false);

                    }
                    SECTION("Flags Read, Write, DeleteAfterClose, Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, OpenModeFlag::Text, OpenModeFlag::UnicodeEncoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[OpenModeFlag::Text] == true);
                        CHECK(presence[OpenModeFlag::UnicodeEncoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Binary] == false);
                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[OpenModeFlag::CommitIndirectlyToDisk] == false);

                    }
                }
            }

            WHEN("Constructed with multiples of the same argument")
            {
                THEN("There should be no presence except the one specified")
                {
                    SECTION("Flags OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[OpenModeFlag::DeleteAfterClose] == true);
                        CHECK(presence[OpenModeFlag::Utf16Encoding] == true);

                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[OpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::RandomAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::Binary, OpenModeFlag::Write, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Append")
                    {
                        FlagPresence presence(OpenModeFlag::Binary, OpenModeFlag::Write, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Binary, OpenModeFlag::Append);
                        CHECK(presence[OpenModeFlag::Binary] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[OpenModeFlag::CommitDirectlyToDisk] == true);
                        CHECK(presence[OpenModeFlag::Append] == true);

                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[OpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::SequntialAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::MustExist, OpenModeFlag::Utf8Encoding);
                        CHECK(presence[OpenModeFlag::NotInheritedByChildProcess] == true);
                        CHECK(presence[OpenModeFlag::MustExist] == true);
                        CHECK(presence[OpenModeFlag::Utf8Encoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[OpenModeFlag::DeleteAfterClose] == false);

                    }
                    SECTION("Flags Read, Write, DeleteAfterClose, Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, OpenModeFlag::Text, OpenModeFlag::Text, OpenModeFlag::Text, OpenModeFlag::UnicodeEncoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[OpenModeFlag::Text] == true);
                        CHECK(presence[OpenModeFlag::UnicodeEncoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Binary] == false);
                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[OpenModeFlag::CommitIndirectlyToDisk] == false);

                    }
                }
            }
        }
    }

    namespace OpenModeConversionStateTesting
    {
        TEST_CASE("[OpenModeConversionState] : Constructor should initialise the FlagPresence according to the given paarmeters", "[OpenModeConversionState]")
        {


            GIVEN("A state constructed with certain flags")
            {
                OpenModeConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write, OpenModeFlag::Text, OpenModeFlag::Binary, OpenModeFlag::UnicodeEncoding);
                THEN("the specified flags should be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::Read] == true);
                    REQUIRE(state.Presence[OpenModeFlag::Write] == true);
                    REQUIRE(state.Presence[OpenModeFlag::Text] == true);
                    REQUIRE(state.Presence[OpenModeFlag::Binary] == true);
                    REQUIRE(state.Presence[OpenModeFlag::UnicodeEncoding] == true);
                }
                THEN("the not specified flags should not be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::MustNotExist] == false);
                    REQUIRE(state.Presence[OpenModeFlag::NotInheritedByChildProcess] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Append] == false);
                    REQUIRE(state.Presence[OpenModeFlag::SequntialAccessOptimized] == false);
                    REQUIRE(state.Presence[OpenModeFlag::RandomAccessOptimized] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Utf16Encoding] == false);
                }
            }
            GIVEN("A state constructed with no flags")
            {
                OpenModeConversionState state;
                THEN("no flags should be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::Read] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Write] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Append] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Text] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Binary] == false);
                    REQUIRE(state.Presence[OpenModeFlag::CommitDirectlyToDisk] == false);
                    REQUIRE(state.Presence[OpenModeFlag::UnicodeEncoding] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Utf16Encoding] == false);
                }
            }
        }

        TEST_CASE("[OpenModeConversionState] : AreFlagsConflicting should return true if the flags are logically conflicting", "[OpenModeConversionState]")
        {
            GIVEN("A states constructed with not conflicting flags")
            {
                OpenModeConversionState state1(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write);
                OpenModeConversionState state2(OpenModeFlag::Read, OpenModeFlag::UnicodeEncoding, OpenModeFlag::MustExist);
                OpenModeConversionState state3;
                THEN("The flags should not conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == false);
                    REQUIRE(state2.AreFlagsConflicting() == false);
                    REQUIRE(state3.AreFlagsConflicting() == false);
                }
            }

            GIVEN("states constructed with MustExist and MustNotExist flags")
            {
                OpenModeConversionState state1(OpenModeFlag::MustExist, OpenModeFlag::MustNotExist);
                OpenModeConversionState state2(OpenModeFlag::MustExist, OpenModeFlag::MustNotExist, OpenModeFlag::Write);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with Text and Binary flags")
            {
                OpenModeConversionState state1(OpenModeFlag::Text, OpenModeFlag::Binary);
                OpenModeConversionState state2(OpenModeFlag::Text, OpenModeFlag::Read, OpenModeFlag::Binary);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with CommitDirectlyToDisk and CommitIndirectlyToDisk flags")
            {
                OpenModeConversionState state1(OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::CommitIndirectlyToDisk);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with SequntialAccessOptimized and RandomAccessOptimized flags")
            {
                OpenModeConversionState state1(OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::CommitIndirectlyToDisk);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with at leats two of UnicodeEncoding, Utf8Encoding or Utf16Encoding flags")
            {
                OpenModeConversionState state1(OpenModeFlag::UnicodeEncoding, OpenModeFlag::Utf8Encoding);
                OpenModeConversionState state2(OpenModeFlag::UnicodeEncoding, OpenModeFlag::Utf16Encoding);
                OpenModeConversionState state3(OpenModeFlag::Utf8Encoding, OpenModeFlag::Utf16Encoding);
                OpenModeConversionState state4(OpenModeFlag::UnicodeEncoding, OpenModeFlag::Utf8Encoding, OpenModeFlag::Utf8Encoding);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                    REQUIRE(state3.AreFlagsConflicting() == true);
                    REQUIRE(state4.AreFlagsConflicting() == true);
                }
            }
        }

        TEST_CASE("[OpenModeConversionState] : SetCOpenModeAndValidty should match the given flags to a COpenMode or set invalid", "[OpenModeConversionState]")
        {

            COpenMode copenMode;
            OpenModeValidity validty;

            WHEN("State is constructed with valid combination of flags the copenMode should be set"
                 " to the appropriate mode and validity should be set to valid")
            {
                SECTION("Flag Read should result in ReadWrite")
                {
                    OpenModeConversionState state(OpenModeFlag::Read);
                    state.SetCOpenModeAndValidty(copenMode, validty);

                    CHECK(copenMode == COpenMode::ReadWrite);
                    CHECK(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Write should result in Write")
                {
                    OpenModeConversionState state(OpenModeFlag::Write);
                    state.SetCOpenModeAndValidty(copenMode, validty);

                    CHECK(copenMode == COpenMode::Write);
                    CHECK(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Read, Write, MustExist should result in ReadWriteMustExist")
                {
                    OpenModeConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist);
                    state.SetCOpenModeAndValidty(copenMode, validty);

                    CHECK(copenMode == COpenMode::ReadWriteMustExist);
                    CHECK(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Write and Append should result in WriteAppend")
                {
                    OpenModeConversionState state(OpenModeFlag::Write, OpenModeFlag::Append);
                    state.SetCOpenModeAndValidty(copenMode, validty);

                    CHECK(copenMode == COpenMode::WriteAppend);
                    CHECK(validty == OpenModeValidity::Valid);
                }
            }

            WHEN("State is constructed with additional mode flags should skip them"
                 "and behave like without them")
            {

                SECTION("Flags Read and MustNotExist should result in ReadWrite")
                {
                    OpenModeConversionState state(OpenModeFlag::Read, OpenModeFlag::MustNotExist);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    CHECK(copenMode == COpenMode::ReadWrite);
                    CHECK(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Read, Write and Binary should result in ReadWrite")
                {
                    OpenModeConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Text, OpenModeFlag::Binary);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    CHECK(copenMode == COpenMode::ReadWrite);
                    CHECK(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Read, Write, MustExist, CommitDirectlyToDisk should result in ReadWriteMustExist")
                {
                    OpenModeConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::CommitDirectlyToDisk);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    CHECK(copenMode == COpenMode::ReadWriteMustExist);
                    CHECK(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Write, Append, UnicodeEncoding should result in WriteAppend")
                {
                    OpenModeConversionState state(OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::UnicodeEncoding);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    CHECK(copenMode == COpenMode::WriteAppend);
                    CHECK(validty == OpenModeValidity::Valid);
                }
            }

            WHEN("State is constructed with invalid combination of flags the validty it should be set to invalid")
            {
                SECTION("No arguments is insufficient information")
                {
                    OpenModeConversionState state;
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Invalid);
                }

                SECTION("Only Append is insufficient information")
                {
                    OpenModeConversionState state(OpenModeFlag::Append);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Invalid);
                }

                SECTION("Only MustExist is insufficient information")
                {
                    OpenModeConversionState state(OpenModeFlag::MustExist);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Invalid);
                }

                SECTION("Append and MustExist is insufficient information")
                {
                    OpenModeConversionState state(OpenModeFlag::Append, OpenModeFlag::MustExist);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Invalid);
                }
            }

            WHEN("State is constructed with both MustExist and Append the validty should be set to unsupported")
            {
                SECTION("Append, MustExist, and Write")
                {
                    OpenModeConversionState state(OpenModeFlag::Append, OpenModeFlag::MustExist, OpenModeFlag::Write);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Unsupported);
                }

                SECTION("Append, MustExist, and Read")
                {
                    OpenModeConversionState state(OpenModeFlag::Append, OpenModeFlag::MustExist, OpenModeFlag::Read);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Unsupported);
                }

                SECTION("Append, MustExist, Read and Write")
                {
                    OpenModeConversionState state(OpenModeFlag::Append, OpenModeFlag::MustExist, OpenModeFlag::Read, OpenModeFlag::Write);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Unsupported);
                }
            }

        }

        TEST_CASE("[OpenModeConversionState] : GetCOpenModeStr should return the COpenMode represented in compile time string"
                           "such that if converted to const char * it can be passed into fopen", "[OpenModeConversionState]")
        {
            using OpenModeString = OpenModeConversionState::OpenModeString;

            WHEN("Provided with COpenMode::ReadMustExist the output should be \'r\'")
            {
                const OpenModeString output = OpenModeConversionState::GetCOpenModeStr(COpenMode::ReadMustExist);
                const OpenModeString expected = "r";
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::Write the output should be \'w\'")
            {
                const OpenModeString output = OpenModeConversionState::GetCOpenModeStr(COpenMode::Write);
                const OpenModeString expected = "w";
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::WriteAppend the output should be \'a\'")
            {
                const OpenModeString output = OpenModeConversionState::GetCOpenModeStr(COpenMode::WriteAppend);
                const OpenModeString expected = "a";
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteMustExist the output should be \'r+\'")
            {
                const OpenModeString output = OpenModeConversionState::GetCOpenModeStr(COpenMode::ReadWriteMustExist);
                const OpenModeString expected = "r+";
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::ReadWrite the output should be \'w+\'")
            {
                const OpenModeString output = OpenModeConversionState::GetCOpenModeStr(COpenMode::ReadWrite);
                const OpenModeString expected = "w+";
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteAppend the output should be \'a+\'")
            {
                const OpenModeString output = OpenModeConversionState::GetCOpenModeStr(COpenMode::ReadWriteAppend);
                const OpenModeString expected = "a+";
                REQUIRE(output == expected);
            }
        }

        TEST_CASE("[OpenModeConversionState] : ConvertString should convert compile time string of smaller char lenght to string of bigger char lenght", "[OpenModeConversionState]")
        {
            WHEN("Converting from char8 to char16 the string should be converted as expected")
            {
                WHEN("Converting normal string")
                {
                    const MetaPrograming::ConstexprStr<8, char8> input = "Hello";
                    const MetaPrograming::ConstexprStr<8, char16> expected = u"Hello";
                    const MetaPrograming::ConstexprStr<8, char16> converted = OpenModeConversionState::template ConvertString<8, char8, char16>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting max sized string")
                {
                    const MetaPrograming::ConstexprStr<5, char8> input = "Hello";
                    const MetaPrograming::ConstexprStr<5, char16> expected = u"Hello";
                    const MetaPrograming::ConstexprStr<5, char16> converted = OpenModeConversionState::template ConvertString<5, char8, char16>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting single char string")
                {
                    const MetaPrograming::ConstexprStr<5, char8> input = "a";
                    const MetaPrograming::ConstexprStr<5, char16> expected = u"a";
                    const MetaPrograming::ConstexprStr<5, char16> converted = OpenModeConversionState::template ConvertString<5, char8, char16>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting empty string")
                {
                    const MetaPrograming::ConstexprStr<5, char8> input = "";
                    const MetaPrograming::ConstexprStr<5, char16> expected = u"";
                    const MetaPrograming::ConstexprStr<5, char16> converted = OpenModeConversionState::template ConvertString<5, char8, char16>(input);
                    REQUIRE(converted == expected);
                }
            }
            WHEN("Converting from char16 to char32 the string should be converted as expected")
            {
                WHEN("Converting normal string")
                {
                    const MetaPrograming::ConstexprStr<8, char16> input = u"Hello";
                    const MetaPrograming::ConstexprStr<8, char32> expected = U"Hello";
                    const MetaPrograming::ConstexprStr<8, char32> converted = OpenModeConversionState::template ConvertString<8, char16, char32>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting max sized string")
                {
                    const MetaPrograming::ConstexprStr<5, char16> input = u"Hello";
                    const MetaPrograming::ConstexprStr<5, char32> expected = U"Hello";
                    const MetaPrograming::ConstexprStr<5, char32> converted = OpenModeConversionState::template ConvertString<5, char16, char32>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting single char string")
                {
                    const MetaPrograming::ConstexprStr<5, char16> input = u"a";
                    const MetaPrograming::ConstexprStr<5, char32> expected = U"a";
                    const MetaPrograming::ConstexprStr<5, char32> converted = OpenModeConversionState::template ConvertString<5, char16, char32>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting empty string")
                {
                    const MetaPrograming::ConstexprStr<5, char16> input = u"";
                    const MetaPrograming::ConstexprStr<5, char32> expected = U"";
                    const MetaPrograming::ConstexprStr<5, char32> converted = OpenModeConversionState::template ConvertString<5, char16, char32>(input);
                    REQUIRE(converted == expected);
                }
            }
            WHEN("Converting from char8 to char32 the string should be converted as expected")
            {
                WHEN("Converting normal string")
                {
                    const MetaPrograming::ConstexprStr<8, char8> input = "Hello";
                    const MetaPrograming::ConstexprStr<8, char32> expected = U"Hello";
                    const MetaPrograming::ConstexprStr<8, char32> converted = OpenModeConversionState::template ConvertString<8, char8, char32>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting max sized string")
                {
                    const MetaPrograming::ConstexprStr<5, char8> input = "Hello";
                    const MetaPrograming::ConstexprStr<5, char32> expected = U"Hello";
                    const MetaPrograming::ConstexprStr<5, char32> converted = OpenModeConversionState::template ConvertString<5, char8, char32>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting single char string")
                {
                    const MetaPrograming::ConstexprStr<5, char8> input = "a";
                    const MetaPrograming::ConstexprStr<5, char32> expected = U"a";
                    const MetaPrograming::ConstexprStr<5, char32> converted = OpenModeConversionState::template ConvertString<5, char8, char32>(input);
                    REQUIRE(converted == expected);
                }
                WHEN("Converting empty string")
                {
                    const MetaPrograming::ConstexprStr<5, char8> input = "";
                    const MetaPrograming::ConstexprStr<5, char32> expected = U"";
                    const MetaPrograming::ConstexprStr<5, char32> converted = OpenModeConversionState::template ConvertString<5, char8, char32>(input);
                    REQUIRE(converted == expected);
                }
            }
        }

        TEST_CASE("[OpenModeConversionState] : GetAdditionalModeStr should return additional mode string based on the flags passed into constructor", "[OpenModeConversionState]")
        {
            using OpenModeString = OpenModeConversionState::OpenModeString;
            using BaseStringView = OpenModeConversionState::BaseStringView;

            WHEN("Contructed with a single additional flag the additional mode str should contain only"
                 " the according code")
            {
                WHEN("Cosntructed only with OpenModeFlag::Text the output should be \'t\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Text);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "t";

                    BaseStringView outputView = output;
                    BaseStringView expectedView = expected;

                    CHECK(outputView == expectedView);
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::Binary the output should be \'b\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Binary);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "b";

                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::MustNotExist the output should be \'x\'")
                {
                    OpenModeConversionState state(OpenModeFlag::MustNotExist);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "x";

                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::CommitDirectlyToDisk the output should be \'c\'")
                {
                    OpenModeConversionState state(OpenModeFlag::CommitDirectlyToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "c";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::CommitIndirectlyToDisk the output should be \'n\'")
                {
                    OpenModeConversionState state(OpenModeFlag::CommitIndirectlyToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "n";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::NotInheritedByChildProcess the output should be \'N\'")
                {
                    OpenModeConversionState state(OpenModeFlag::NotInheritedByChildProcess);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "N";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::SequntialAccessOptimized the output should be \'S\'")
                {
                    OpenModeConversionState state(OpenModeFlag::SequntialAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "S";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::RandomAccessOptimized the output should be \'R\'")
                {
                    OpenModeConversionState state(OpenModeFlag::RandomAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "R";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::IfPossibleNoFlushingToDisk the output should be \'T\'")
                {
                    OpenModeConversionState state(OpenModeFlag::IfPossibleNoFlushingToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "T";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::DeleteAfterClose the output should be \'D\'")
                {
                    OpenModeConversionState state(OpenModeFlag::DeleteAfterClose);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "D";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::UnicodeEncoding the output should be \',css=UNICODE\'")
                {
                    OpenModeConversionState state(OpenModeFlag::UnicodeEncoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = ",css=UNICODE";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::Utf8Encoding the output should be \',css=UTF-8\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Utf8Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = ",css=UTF-8";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::Utf16Encoding the output should be \',css=UTF-16LE\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Utf16Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = ",css=UTF-16LE";
                    REQUIRE(output == expected);
                }
            }

            WHEN("Contructed with a multiple additional flags the additional mode str should contain"
                 " all of the flag codes concatenated")
            {
                WHEN("Cosntructed with flags Text, MustNotExist the output should be \'tx\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Text, OpenModeFlag::MustNotExist);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "tx";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Binary, Utf8Encoding the output should be \'b,css=UTF-8\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Binary, OpenModeFlag::Utf8Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "b,css=UTF-8";
                    REQUIRE(output == expected);
                }
                //The order shouldnt matter
                WHEN("Cosntructed with flags Utf8Encoding, Binary the output should be \'b,css=UTF-8\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Utf8Encoding, OpenModeFlag::Binary);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "b,css=UTF-8";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags NotInheritedByChildProcess, RandomAccessOptimized the output should be \'NR\'")
                {
                    OpenModeConversionState state(OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::RandomAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "NR";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags MustNotExist, Text, CommitDirectlyToDisk, DeleteAfterClose the output should be \'txcD\'")
                {
                    OpenModeConversionState state(OpenModeFlag::MustNotExist, OpenModeFlag::Text, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::DeleteAfterClose);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "txcD";
                    REQUIRE(output == expected);
                }
            }

            WHEN("Contructed with a additional flags and core flags the additional mode str should contain"
                 " only the additional flag codes cocatenated")
            {
                WHEN("Cosntructed with flags Binary, DeleteAfterClose, Read, Write the output should be \'bD\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Binary, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Read, OpenModeFlag::Write);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "bD";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Binary, Read, DeleteAfterClose, Append the output should be \'bD\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Binary, OpenModeFlag::Read, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Append);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "bD";
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Binary, Write, DeleteAfterClose, MustExist the output should be \'bD\'")
                {
                    OpenModeConversionState state(OpenModeFlag::Binary, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::MustExist);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = "bD";
                    REQUIRE(output == expected);
                }
            }
        }
    }

    namespace OpenModeClassTesting
    {
        TEST_CASE("[OpenMode] : Default constructor should create an openmode with invalid Validty", "[OpenMode]")
       {
            const OpenMode mode;
            REQUIRE(mode.IsValid() == false);
            REQUIRE(mode.IsSupported() == true);
        }

        TEST_CASE("[OpenMode] : Constructor with COpenMode should crete always valid openmode with appropriate OpenModeString", "[OpenMode]")
        {
            using OpenModeString = OpenModeConversionState::OpenModeString;

            WHEN("Provided with COpenMode::ReadMustExist the OpenModeStr should be \'r\'")
            {
                const OpenMode mode(COpenMode::ReadMustExist);
                const OpenModeString expected = "r";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::Write the OpenModeStr should be \'w\'")
            {
                const OpenMode mode(COpenMode::Write);
                const OpenModeString expected = "w";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::WriteAppend the OpenModeStr should be \'a\'")
            {
                const OpenMode mode(COpenMode::WriteAppend);
                const OpenModeString expected = "a";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteMustExist the OpenModeStr should be \'r+\'")
            {
                const OpenMode mode(COpenMode::ReadWriteMustExist);
                const OpenModeString expected = "r+";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWrite the OpenModeStr should be \'w+\'")
            {
                const OpenMode mode(COpenMode::ReadWrite);
                const OpenModeString expected = "w+";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteAppend the OpenModeStr should be \'a+\'")
            {
                const OpenMode mode(COpenMode::ReadWriteAppend);
                const OpenModeString expected = "a+";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
        }

        TEST_CASE("[OpenMode] : AssignCOpenMode should change the openmode to always valid openmode with appropriate OpenModeString", "[OpenMode]")
        {
            using OpenModeString = OpenModeConversionState::OpenModeString;
            OpenMode mode;

            WHEN("Provided with COpenMode::ReadMustExist the OpenModeStr should be \'r\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadMustExist);
                const OpenModeString expected = "r";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::Write the OpenModeStr should be \'w\'")
            {
                mode.AssignCOpenMode(COpenMode::Write);
                const OpenModeString expected = "w";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::WriteAppend the OpenModeStr should be \'a\'")
            {
                mode.AssignCOpenMode(COpenMode::WriteAppend);
                const OpenModeString expected = "a";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteMustExist the OpenModeStr should be \'r+\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadWriteMustExist);
                const OpenModeString expected = "r+";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWrite the OpenModeStr should be \'w+\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadWrite);
                const OpenModeString expected = "w+";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteAppend the OpenModeStr should be \'a+\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadWriteAppend);
                const OpenModeString expected = "a+";

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
        }

        TEST_CASE("[OpenMode] : CreateOpenMode should create a open mode according to the flags passed", "[OpenMode]")
        {
            using OpenModeString = OpenModeConversionState::OpenModeString;
            using BaseStringView = typename OpenMode::BaseStringView;

            WHEN("Called with valid flags it should construct a valid openmode")
            {
                WHEN("Provided with flags Read MustExist the OpenModeStr should be \'r\'"
                     " and it should be valid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::MustExist);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = "r";

                    const BaseStringView outputView = output;
                    const BaseStringView expectedView = expected;
                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Read, Write, MustExist, Binary the OpenModeStr should be \'r+b\'"
                     " and it should be valid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = "r+b";

                    const BaseStringView outputView = output;
                    const BaseStringView expectedView = expected;
                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, MustNotExist, CommitDirectlyToDisk, SequntialAccessOptimized the OpenModeStr should be \'wxcS\'"
                     " and it should be valid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::MustNotExist, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::SequntialAccessOptimized);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = "wxcS";

                    const BaseStringView outputView = output;
                    const BaseStringView expectedView = expected;
                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, MustNotExist, CommitDirectlyToDisk, SequntialAccessOptimized the OpenModeStr should be \'w+ND,css=UTF-8\'"
                     " and it should be valid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf8Encoding);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = "w+ND,css=UTF-8";

                    const BaseStringView outputView = output;
                    const BaseStringView expectedView = expected;
                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
            }

            WHEN("Called with conflicting flags (according to OpenModeConversionState::AreAreFlagsConflicting) it should construct a invalid openmode")
            {
                WHEN("Provided with flags Read, MustExist, MustNotExist it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::MustNotExist);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Read, Write, Text, Binary it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Text, OpenModeFlag::Binary);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, UnicodeEncoding, Utf8Encoding it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::UnicodeEncoding, OpenModeFlag::Utf8Encoding);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, Append, CommitDirectlyToDisk, CommitIndirectlyToDisk it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::CommitIndirectlyToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Read, SequntialAccessOptimized, RandomAccessOptimized it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::SequntialAccessOptimized, OpenModeFlag::RandomAccessOptimized);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
            }

            WHEN("Called with invalid flags (insufficient information) it should construct a invalid openmode")
            {
                WHEN("Provided with no flags it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode();
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags MustNotExist it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::MustNotExist);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Append, Text it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Append, OpenModeFlag::Text);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags MustExist, UnicodeEncoding, Binary, CommitDirectlyToDisk it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::MustExist, OpenModeFlag::UnicodeEncoding, OpenModeFlag::Binary, OpenModeFlag::CommitDirectlyToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Append, MustExist, DeleteAfterClose, IfPossibleNoFlushingToDisk it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Append, OpenModeFlag::MustExist, OpenModeFlag::DeleteAfterClose, OpenModeFlag::IfPossibleNoFlushingToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
            }

            WHEN("Called with usnupported combination of flags (Append, MustExist together alongside Write or Read) it should construct an unsupported openmode")
            {
                WHEN("Provided with flags Read, MustExist, Append, CommitDirectlyToDisk it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Append, OpenModeFlag::CommitDirectlyToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == false);
                }
                WHEN("Provided with flags Read, MustExist, Append, CommitDirectlyToDisk it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::UnicodeEncoding, OpenModeFlag::Append);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == false);
                }
                WHEN("Provided with flags Read, MustExist, Append, CommitDirectlyToDisk it should be invalid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write,  OpenModeFlag::MustExist, OpenModeFlag::Append, OpenModeFlag::IfPossibleNoFlushingToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == false);
                }
            }
        }

        TEST_CASE("[OpenMode] : CreateOpenMode (passing flags into template) should create a open mode according to the flags passed", "[OpenMode]")
        {
            using OpenModeString = OpenModeConversionState::OpenModeString;

            WHEN("Called with valid flags it should construct a valid openmode")
            {
                WHEN("Provided with flags Read MustExist the OpenModeStr should be \'r\'"
                     " and it should be valid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode<OpenModeFlag::Read, OpenModeFlag::MustExist>();

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = "r";

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Read, Write, MustExist, Binary the OpenModeStr should be \'r+b\'"
                     " and it should be valid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode<OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary>();

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = "r+b";

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, MustNotExist, CommitDirectlyToDisk, SequntialAccessOptimized the OpenModeStr should be \'wxcS\'"
                     " and it should be valid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode<OpenModeFlag::Write, OpenModeFlag::MustNotExist, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::SequntialAccessOptimized>();

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = "wxcS";

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, MustNotExist, CommitDirectlyToDisk, SequntialAccessOptimized the OpenModeStr should be \'w+ND,css=UTF-8\'"
                     " and it should be valid")
                {
                    const OpenMode mode = OpenMode::CreateOpenMode<OpenModeFlag::Read, OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf8Encoding>();

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = "w+ND,css=UTF-8";

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
            }

            WHEN("Called with conflicting flags (according to OpenModeConversionState::AreAreFlagsConflicting) it should not compile and assert the invlaid message")
            {
                #if OpenMode_TEST_COMPILE_ERROR == true
                constexpr OpenMode mode1 = OpenMode::CreateOpenMode<OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Text, OpenModeFlag::Binary>();
                constexpr OpenMode mode2 = OpenMode::CreateOpenMode<OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::CommitIndirectlyToDisk>();
                #endif
            }

            WHEN("Called with invalid flags (insufficient information) it should not compile")
            {

                #if OpenMode_TEST_COMPILE_ERROR == true
                constexpr OpenMode mode1 = OpenMode::CreateOpenMode<>();
                constexpr OpenMode mode2 = OpenMode::CreateOpenMode<OpenModeFlag::MustNotExist>();
                constexpr OpenMode mode3 = OpenMode::CreateOpenMode<OpenModeFlag::MustExist, OpenModeFlag::UnicodeEncoding, OpenModeFlag::MustNotExist, OpenModeFlag::CommitDirectlyToDisk>();
                #endif
            }

            WHEN("Called with usnupported combination of flags (Append, MustExist together alongside Write or Read) it should not compile and assert the unsupported message")
            {
                #if OpenMode_TEST_COMPILE_ERROR == true
                constexpr OpenMode mode1 = OpenMode::CreateOpenMode<OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Append, OpenModeFlag::CommitDirectlyToDisk>();
                constexpr OpenMode mode2 = OpenMode::CreateOpenMode<OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::UnicodeEncoding, OpenModeFlag::Append>();
                constexpr OpenMode mode3 = OpenMode::CreateOpenMode<OpenModeFlag::Read, OpenModeFlag::Write,  OpenModeFlag::MustExist, OpenModeFlag::Append, OpenModeFlag::IfPossibleNoFlushingToDisk>();
                #endif
            }
        }

        TEST_CASE("[OpenMode] : IsValid shoudl return true if the openmode is valid (fit to open a file)", "[OpenMode]")
        {
            WHEN("A constructed mode is valid IsValid returns true")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write);
                REQUIRE(mode.IsValid() == true);
            }
            WHEN("A constructed mode is invalid IsValid returns false")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::MustExist, OpenModeFlag::Text, OpenModeFlag::MustNotExist);
                REQUIRE(mode.IsValid() == false);
            }
            WHEN("A constructed mode is unsupported IsValid returns false")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                REQUIRE(mode.IsValid() == false);
            }
        }

        TEST_CASE("[OpenMode] : IsSupported should return true if the openmode is supported (does not have to valid)", "[OpenMode]")
        {
            WHEN("A constructed mode is valid IsSupported returns true")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write);
                REQUIRE(mode.IsSupported() == true);
            }
            WHEN("A constructed mode is invalid IsSupported returns true")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::MustExist, OpenModeFlag::Text, OpenModeFlag::MustNotExist);
                REQUIRE(mode.IsSupported() == true);
            }
            WHEN("A constructed mode is unsupported IsSupported returns false")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                REQUIRE(mode.IsSupported() == false);
            }
        }

        #define OpenModeTesting_AllValidCharTypes char8, char16, char32, charW
        TEMPLATE_TEST_CASE("[OpenMode] : GetOpenModeString should return a copy of the constexpr openmode string in the char type specified", "[OpenMode]", OpenModeTesting_AllValidCharTypes)
        {
            const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write);
            REQUIRE(mode.IsValid() == true);

            OpenMode::ConstexprString<TestType> expected = OpenModeConversionState::ConvertToOsString<TestType>("w+");
            REQUIRE(mode.GetOpenModeString<TestType>() == expected);
        }

        TEMPLATE_TEST_CASE("[OpenMode] : operator StringView<OsCharType> should return a stringview conatining just the openmode in the char type specified", "[OpenMode]", OpenModeTesting_AllValidCharTypes)
        {
            const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write);
            REQUIRE(mode.IsValid() == true);

            OpenMode::StringView<TestType> output = mode;
            OpenMode::StringView<TestType> expected = OpenModeConversionState::ConvertToOsString<TestType>("w+");
            REQUIRE(output == expected);
        }
        #undef OpenModeTesting_AllValidCharTypes

        TEST_CASE("[OpenMode] : Copy constructor should construct a copy of the provided open mode", "[OpenMode]")
        {
            WHEN("Constructing from an empty invalid openmode the result should be empty and invalid aswell")
            {
                const OpenMode mode;
                const OpenMode constructed(mode);

                REQUIRE(constructed.IsValid() == false);
                REQUIRE(constructed.IsSupported() == true);
            }
            WHEN("Constructing from an valid openmode the result should be valid and have the same OpenModeStr")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write);
                const OpenMode constructed(mode);

                REQUIRE(constructed.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(constructed.IsValid() == true);
                REQUIRE(constructed.IsSupported() == true);
            }
            WHEN("Constructing from an unsupported openmode the result should be unsupported aswell")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                const OpenMode constructed(mode);

                REQUIRE(constructed.IsValid() == false);
                REQUIRE(constructed.IsSupported() == false);
            }
        }

        TEST_CASE("[OpenMode] : Move constructor should construct a copy of the provided open mode", "[OpenMode]")
        {
            WHEN("Constructing from an empty invalid openmode the result should be empty and invalid aswell")
            {
                OpenMode mode;
                const auto beforeStr = mode.GetOpenModeString();
                const OpenMode constructed(std::move(mode));

                REQUIRE(constructed.IsValid() == false);
                REQUIRE(constructed.IsSupported() == true);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == false);
                REQUIRE(mode.IsSupported() == true);
            }
            WHEN("Constructing from an valid openmode the result should be valid and have the same OpenModeStr")
            {
                OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write);
                const auto beforeStr = mode.GetOpenModeString();
                const OpenMode constructed(std::move(mode));

                REQUIRE(constructed.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(constructed.IsValid() == true);
                REQUIRE(constructed.IsSupported() == true);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == true);
                REQUIRE(mode.IsSupported() == true);
            }
            WHEN("Constructing from an unsupported openmode the result should be unsupported aswell")
            {
                OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                const auto beforeStr = mode.GetOpenModeString();
                const OpenMode constructed(std::move(mode));

                REQUIRE(constructed.IsValid() == false);
                REQUIRE(constructed.IsSupported() == false);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == false);
                REQUIRE(mode.IsSupported() == false);
            }
        }

        TEST_CASE("[OpenMode] : Copy assigning to OpenMode should copy the provided OpenMode", "[OpenMode]")
        {
            WHEN("Assigning from an empty invalid openmode the result should be empty and invalid aswell")
            {
                const OpenMode mode;
                OpenMode assigned;

                assigned = mode;

                REQUIRE(assigned.IsValid() == false);
                REQUIRE(assigned.IsSupported() == true);
            }
            WHEN("Assigning from an valid openmode the result should be valid and have the same OpenModeStr")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write);
                OpenMode assigned;

                assigned = mode;

                REQUIRE(assigned.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(assigned.IsValid() == true);
                REQUIRE(assigned.IsSupported() == true);
            }
            WHEN("Assigning from an unsupported openmode the result should be unsupported aswell")
            {
                const OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                OpenMode assigned;

                assigned = mode;

                REQUIRE(assigned.IsValid() == false);
                REQUIRE(assigned.IsSupported() == false);
            }
        }

        TEST_CASE("[OpenMode] : Move assigning to OpenMode should copy the provided OpenMode", "[OpenMode]")
        {
            WHEN("Assigning from an empty invalid openmode the result should be empty and invalid aswell")
            {
                OpenMode mode;
                OpenMode assigned;
                const auto beforeStr = mode.GetOpenModeString();

                assigned = std::move(mode);

                REQUIRE(assigned.IsValid() == false);
                REQUIRE(assigned.IsSupported() == true);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == false);
                REQUIRE(mode.IsSupported() == true);
            }
            WHEN("Assigning from an valid openmode the result should be valid and have the same OpenModeStr")
            {
                OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Read, OpenModeFlag::Write);
                OpenMode assigned;
                const auto beforeStr = mode.GetOpenModeString();

                assigned = std::move(mode);

                REQUIRE(assigned.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(assigned.IsValid() == true);
                REQUIRE(assigned.IsSupported() == true);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == true);
                REQUIRE(mode.IsSupported() == true);
            }
            WHEN("Assigning from an unsupported openmode the result should be unsupported aswell")
            {
                OpenMode mode = OpenMode::CreateOpenMode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                OpenMode assigned;
                const auto beforeStr = mode.GetOpenModeString();

                assigned = std::move(mode);

                REQUIRE(assigned.IsValid() == false);
                REQUIRE(assigned.IsSupported() == false);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == false);
                REQUIRE(mode.IsSupported() == false);
            }

        }

        TEST_CASE("[OpenMode] : ", "[OpenMode]")
        {

        }
    }
}

#endif // OPENMODETESTING_H
