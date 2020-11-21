﻿#ifndef OPENMODETESTING_H
#define OPENMODETESTING_H

#include "Catch2/Catch.hpp"
#include "General/File/OpenMode.h"

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
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::IfPossibleNoFlushingToDisk, OpenModeFlag::Append, WindowsOpenModeFlag::IfPossibleNoFlushingToDisk) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Write, OpenModeFlag::MustExist) == true);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::RandomAccessOptimized, OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::RandomAccessOptimized) == true);
            }
            WHEN("Given arguments containing the lookignForFlag multiple times it should return true")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Read, OpenModeFlag::Read) == true);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::Utf8Encoding, OpenModeFlag::Append, WindowsOpenModeFlag::Utf8Encoding, WindowsOpenModeFlag::Utf8Encoding) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, OpenModeFlag::Append, WindowsOpenModeFlag::Utf8Encoding, OpenModeFlag::Write, OpenModeFlag::Write) == true);
            }
            WHEN("Given arguments not containing the lookignForFlag it should return false")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, WindowsOpenModeFlag::IfPossibleNoFlushingToDisk, OpenModeFlag::Append) == false);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::Utf8Encoding, WindowsOpenModeFlag::RandomAccessOptimized, OpenModeFlag::Write) == false);
            }
            WHEN("Given arguments only lookingForType it should return false")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read) == false);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::NotInheritedByChildProcess) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::MustNotExist) == false);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::Text) == false);
            }
        }

        TEST_CASE("[FlagPresence] : the [] operator should return flag presence at index", "[FlagPresence]")
        {
            GIVEN("A FlagPresence constructed with certain flags")
            {
                FlagPresence presence(OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist, WindowsOpenModeFlag::DeleteAfterClose);
                WHEN("A [] operator is used with the value of constructed flag it should be present")
                {
                    REQUIRE(presence[OpenModeFlag::Write] == true);
                    REQUIRE(presence[OpenModeFlag::Append] == true);
                    REQUIRE(presence[OpenModeFlag::MustExist] == true);
                    REQUIRE(presence[WindowsOpenModeFlag::DeleteAfterClose] == true);
                }
                WHEN("A [] operator is used with the value of not constructed flag it should not be present")
                {
                    REQUIRE(presence[OpenModeFlag::Read] == false);
                    REQUIRE(presence[OpenModeFlag::Binary] == false);
                    REQUIRE(presence[OpenModeFlag::MustNotExist] == false);
                    REQUIRE(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == false);
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
                                      OpenModeFlag::Binary,
                                      OpenModeFlag::MustNotExist,
                                      WindowsOpenModeFlag::Text,
                                      WindowsOpenModeFlag::CommitDirectlyToDisk,
                                      WindowsOpenModeFlag::CommitIndirectlyToDisk,
                                      WindowsOpenModeFlag::NotInheritedByChildProcess,
                                      WindowsOpenModeFlag::SequntialAccessOptimized,
                                      WindowsOpenModeFlag::RandomAccessOptimized,
                                      WindowsOpenModeFlag::IfPossibleNoFlushingToDisk,
                                      WindowsOpenModeFlag::DeleteAfterClose,
                                      WindowsOpenModeFlag::UnicodeEncoding,
                                      WindowsOpenModeFlag::Utf8Encoding,
                                      WindowsOpenModeFlag::Utf16Encoding);

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
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::MustExist] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::Text")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::Text);
                        CHECK(presence[OpenModeFlag::Binary] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == true);
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
                        CHECK(presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == false);
                        CHECK(presence[OpenModeFlag::MustNotExist] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::CommitDirectlyToDisk")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::CommitDirectlyToDisk);
                        CHECK(presence[WindowsOpenModeFlag::CommitIndirectlyToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::CommitIndirectlyToDisk")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::CommitIndirectlyToDisk);
                        CHECK(presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == false);
                        CHECK(presence[WindowsOpenModeFlag::CommitIndirectlyToDisk] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::NotInheritedByChildProcess")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::NotInheritedByChildProcess);
                        CHECK(presence[WindowsOpenModeFlag::SequntialAccessOptimized] == false);
                        CHECK(presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::SequntialAccessOptimized")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::SequntialAccessOptimized);
                        CHECK(presence[WindowsOpenModeFlag::RandomAccessOptimized] == false);
                        CHECK(presence[WindowsOpenModeFlag::SequntialAccessOptimized] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::RandomAccessOptimized")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::RandomAccessOptimized);
                        CHECK(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::RandomAccessOptimized] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::IfPossibleNoFlushingToDisk")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::IfPossibleNoFlushingToDisk);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == false);
                        CHECK(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::DeleteAfterClose")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::DeleteAfterClose);
                        CHECK(presence[WindowsOpenModeFlag::UnicodeEncoding] == false);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::UnicodeEncoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::UnicodeEncoding);
                        CHECK(presence[WindowsOpenModeFlag::Utf8Encoding] == false);
                        CHECK(presence[WindowsOpenModeFlag::UnicodeEncoding] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::Utf8Encoding);
                        CHECK(presence[WindowsOpenModeFlag::Utf16Encoding] == false);
                        CHECK(presence[WindowsOpenModeFlag::Utf8Encoding] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[WindowsOpenModeFlag::Utf16Encoding] == true);
                    }
                }
            }

            WHEN("Constructed with multiple arguments")
            {
                THEN("There should be no presence except the one specified")
                {
                    SECTION("Flags OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::DeleteAfterClose, WindowsOpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == true);
                        CHECK(presence[WindowsOpenModeFlag::Utf16Encoding] == true);

                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::RandomAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::Binary, OpenModeFlag::Write, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Append")
                    {
                        FlagPresence presence(OpenModeFlag::Binary, OpenModeFlag::Write, WindowsOpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Append);
                        CHECK(presence[OpenModeFlag::Binary] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == true);
                        CHECK(presence[OpenModeFlag::Append] == true);

                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::SequntialAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, WindowsOpenModeFlag::Utf8Encoding);
                        CHECK(presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == true);
                        CHECK(presence[OpenModeFlag::MustExist] == true);
                        CHECK(presence[WindowsOpenModeFlag::Utf8Encoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == false);

                    }
                    SECTION("Flags Read, Write, DeleteAfterClose, Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::UnicodeEncoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[WindowsOpenModeFlag::Text] == true);
                        CHECK(presence[WindowsOpenModeFlag::UnicodeEncoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Binary] == false);
                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[WindowsOpenModeFlag::CommitIndirectlyToDisk] == false);

                    }
                }
            }

            WHEN("Constructed with multiples of the same argument")
            {
                THEN("There should be no presence except the one specified")
                {
                    SECTION("Flags OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::DeleteAfterClose, WindowsOpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == true);
                        CHECK(presence[WindowsOpenModeFlag::Utf16Encoding] == true);

                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::RandomAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::Binary, OpenModeFlag::Write, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Append")
                    {
                        FlagPresence presence(OpenModeFlag::Binary, OpenModeFlag::Write, WindowsOpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Binary, OpenModeFlag::Append);
                        CHECK(presence[OpenModeFlag::Binary] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == true);
                        CHECK(presence[OpenModeFlag::Append] == true);

                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::SequntialAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::MustExist, WindowsOpenModeFlag::Utf8Encoding);
                        CHECK(presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == true);
                        CHECK(presence[OpenModeFlag::MustExist] == true);
                        CHECK(presence[WindowsOpenModeFlag::Utf8Encoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == false);

                    }
                    SECTION("Flags Read, Write, DeleteAfterClose, Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::UnicodeEncoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[WindowsOpenModeFlag::Text] == true);
                        CHECK(presence[WindowsOpenModeFlag::UnicodeEncoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Binary] == false);
                        CHECK(presence[OpenModeFlag::Append] == false);
                        CHECK(presence[WindowsOpenModeFlag::CommitIndirectlyToDisk] == false);

                    }
                }
            }
        }
    }

    namespace OpenModeConversionStateTesting
    {
        TEMPLATE_TEST_CASE("[OpenModeConversionState] : Constructor should initialise the FlagPresence according to the given paarmeters", "[OpenModeConversionState]", OpenModeTestedTypes)
        {
            using ConversionState = OpenModeConversionState<TestType>;

            GIVEN("A state constructed with certain flags")
            {
                ConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write, WindowsOpenModeFlag::Text, OpenModeFlag::Binary, WindowsOpenModeFlag::UnicodeEncoding);
                THEN("the specified flags should be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::Read] == true);
                    REQUIRE(state.Presence[OpenModeFlag::Write] == true);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::Text] == true);
                    REQUIRE(state.Presence[OpenModeFlag::Binary] == true);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::UnicodeEncoding] == true);
                }
                THEN("the not specified flags should not be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::MustNotExist] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Append] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::SequntialAccessOptimized] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::RandomAccessOptimized] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::Utf16Encoding] == false);
                }
            }
            GIVEN("A state constructed with no flags")
            {
                ConversionState state;
                THEN("no flags should be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::Read] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Write] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Append] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::Text] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Binary] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::UnicodeEncoding] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::Utf16Encoding] == false);
                }
            }
        }

        TEMPLATE_TEST_CASE("[OpenModeConversionState] : AreFlagsConflicting should return true if the flags are logically conflicting", "[OpenModeConversionState]", OpenModeTestedTypes)
        {
            using ConversionState = OpenModeConversionState<TestType>;

            GIVEN("A states constructed with not conflicting flags")
            {
                ConversionState state1(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write);
                ConversionState state2(OpenModeFlag::Read, WindowsOpenModeFlag::UnicodeEncoding, OpenModeFlag::MustExist);
                ConversionState state3;
                THEN("The flags should not conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == false);
                    REQUIRE(state2.AreFlagsConflicting() == false);
                    REQUIRE(state3.AreFlagsConflicting() == false);
                }
            }

            GIVEN("states constructed with MustExist and MustNotExist flags")
            {
                ConversionState state1(OpenModeFlag::MustExist, OpenModeFlag::MustNotExist);
                ConversionState state2(OpenModeFlag::MustExist, OpenModeFlag::MustNotExist, OpenModeFlag::Write);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with Text and Binary flags")
            {
                ConversionState state1(WindowsOpenModeFlag::Text, OpenModeFlag::Binary);
                ConversionState state2(WindowsOpenModeFlag::Text, OpenModeFlag::Read, OpenModeFlag::Binary);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with CommitDirectlyToDisk and CommitIndirectlyToDisk flags")
            {
                ConversionState state1(WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::CommitIndirectlyToDisk);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with SequntialAccessOptimized and RandomAccessOptimized flags")
            {
                ConversionState state1(WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::CommitIndirectlyToDisk);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with at leats two of UnicodeEncoding, Utf8Encoding or Utf16Encoding flags")
            {
                ConversionState state1(WindowsOpenModeFlag::UnicodeEncoding, WindowsOpenModeFlag::Utf8Encoding);
                ConversionState state2(WindowsOpenModeFlag::UnicodeEncoding, WindowsOpenModeFlag::Utf16Encoding);
                ConversionState state3(WindowsOpenModeFlag::Utf8Encoding, WindowsOpenModeFlag::Utf16Encoding);
                ConversionState state4(WindowsOpenModeFlag::UnicodeEncoding, WindowsOpenModeFlag::Utf8Encoding, WindowsOpenModeFlag::Utf8Encoding);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                    REQUIRE(state3.AreFlagsConflicting() == true);
                    REQUIRE(state4.AreFlagsConflicting() == true);
                }
            }
        }

        TEMPLATE_TEST_CASE("[OpenModeConversionState] : SetCOpenModeAndValidty should match the given flags to a COpenMode or set invalid", "[OpenModeConversionState]", OpenModeTestedTypes)
        {
            using ConversionState = OpenModeConversionState<TestType>;
            COpenMode copenMode;
            OpenModeValidity validty;

            WHEN("State is constructed with valid combination of flags the copenMode should be set"
                 " to the appropriate mode and validity should be set to valid")
            {
                SECTION("Flag Read should result in ReadWrite")
                {
                    ConversionState state(OpenModeFlag::Read);
                    state.SetCOpenModeAndValidty(copenMode, validty);

                    CHECK(copenMode == COpenMode::ReadWrite);
                    REQUIRE(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Write should result in Write")
                {
                    ConversionState state(OpenModeFlag::Write);
                    state.SetCOpenModeAndValidty(copenMode, validty);

                    CHECK(copenMode == COpenMode::Write);
                    REQUIRE(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Read, Write, MustExist should result in ReadWriteMustExist")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist);
                    state.SetCOpenModeAndValidty(copenMode, validty);

                    CHECK(copenMode == COpenMode::ReadWriteMustExist);
                    REQUIRE(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Write and Append should result in WriteAppend")
                {
                    ConversionState state(OpenModeFlag::Write, OpenModeFlag::Append);
                    state.SetCOpenModeAndValidty(copenMode, validty);

                    CHECK(copenMode == COpenMode::WriteAppend);
                    REQUIRE(validty == OpenModeValidity::Valid);
                }
            }

            WHEN("State is constructed with additional mode flags should skip them"
                 "and behave like without them")
            {

                SECTION("Flags Read and MustNotExist should result in ReadWrite")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::MustNotExist);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    CHECK(copenMode == COpenMode::ReadWrite);
                    REQUIRE(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Read, Write and Binary should result in ReadWrite")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::Text, OpenModeFlag::Binary);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    CHECK(copenMode == COpenMode::ReadWrite);
                    REQUIRE(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Read, Write, MustExist, CommitDirectlyToDisk should result in ReadWriteMustExist")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, WindowsOpenModeFlag::CommitDirectlyToDisk);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    CHECK(copenMode == COpenMode::ReadWriteMustExist);
                    REQUIRE(validty == OpenModeValidity::Valid);
                }

                SECTION("Flags Write, Append, UnicodeEncoding should result in WriteAppend")
                {
                    ConversionState state(OpenModeFlag::Write, OpenModeFlag::Append, WindowsOpenModeFlag::UnicodeEncoding);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    CHECK(copenMode == COpenMode::WriteAppend);
                    REQUIRE(validty == OpenModeValidity::Valid);
                }
            }

            WHEN("State is constructed with invalid combination of flags the validty it should be set to invalid")
            {
                SECTION("No arguments is insufficient information")
                {
                    ConversionState state;
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Invalid);
                }

                SECTION("Only Append is insufficient information")
                {
                    ConversionState state(OpenModeFlag::Append);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Invalid);
                }

                SECTION("Only MustExist is insufficient information")
                {
                    ConversionState state(OpenModeFlag::MustExist);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Invalid);
                }

                SECTION("Append and MustExist is insufficient information")
                {
                    ConversionState state(OpenModeFlag::Append, OpenModeFlag::MustExist);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Invalid);
                }
            }

            WHEN("State is constructed with both MustExist and Append the validty should be set to unsupported")
            {
                SECTION("Append, MustExist, and Write")
                {
                    ConversionState state(OpenModeFlag::Append, OpenModeFlag::MustExist, OpenModeFlag::Write);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Unsupported);
                }

                SECTION("Append, MustExist, and Read")
                {
                    ConversionState state(OpenModeFlag::Append, OpenModeFlag::MustExist, OpenModeFlag::Read);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Unsupported);
                }

                SECTION("Append, MustExist, Read and Write")
                {
                    ConversionState state(OpenModeFlag::Append, OpenModeFlag::MustExist, OpenModeFlag::Read, OpenModeFlag::Write);
                    state.SetCOpenModeAndValidty(copenMode, validty);
                    REQUIRE(validty == OpenModeValidity::Unsupported);
                }
            }

        }

        TEMPLATE_TEST_CASE("[OpenModeConversionState] : GetCOpenModeStr should return the COpenMode represented in compile time string"
                           "such that if converted to const char * it can be passed into fopen", "[OpenModeConversionState]", OpenModeTestedTypes)
        {
            using ConversionState = OpenModeConversionState<TestType>;
            using OpenModeString = typename ConversionState::OpenModeString;
            using Char8ConstexprStr = typename ConversionState::template ConstexprString<char8>;

            WHEN("Provided with COpenMode::ReadMustExist the output should be \'r\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::ReadMustExist);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::Write the output should be \'w\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::Write);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::WriteAppend the output should be \'a\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::WriteAppend);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteMustExist the output should be \'r+\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::ReadWriteMustExist);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::ReadWrite the output should be \'w+\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::ReadWrite);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteAppend the output should be \'a+\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::ReadWriteAppend);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a+"));
                REQUIRE(output == expected);
            }
        }

        TEMPLATE_TEST_CASE("[OpenModeConversionState] : GetAdditionalModeStr should return additional mode string based on the flags passed into constructor", "[OpenModeConversionState]", OpenModeTestedTypes)
        {
            using ConversionState = OpenModeConversionState<TestType>;
            using OpenModeString = typename ConversionState::OpenModeString;
            using Char8ConstexprStr = typename ConversionState::template ConstexprString<char8>;

            WHEN("Contructed with a single additional flag the additional mode str should contain only"
                 " the according code")
            {
                WHEN("Cosntructed only with OpenModeFlag::Text the output should be \'t\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Text);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("t"));

                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::Binary the output should be \'b\'")
                {
                    ConversionState state(OpenModeFlag::Binary);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("b"));

                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::MustNotExist the output should be \'x\'")
                {
                    ConversionState state(OpenModeFlag::MustNotExist);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("x"));

                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::CommitDirectlyToDisk the output should be \'c\'")
                {
                    ConversionState state(WindowsOpenModeFlag::CommitDirectlyToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("c"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::CommitIndirectlyToDisk the output should be \'n\'")
                {
                    ConversionState state(WindowsOpenModeFlag::CommitIndirectlyToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("n"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::NotInheritedByChildProcess the output should be \'N\'")
                {
                    ConversionState state(WindowsOpenModeFlag::NotInheritedByChildProcess);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("N"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::SequntialAccessOptimized the output should be \'S\'")
                {
                    ConversionState state(WindowsOpenModeFlag::SequntialAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("S"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::RandomAccessOptimized the output should be \'R\'")
                {
                    ConversionState state(WindowsOpenModeFlag::RandomAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("R"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::IfPossibleNoFlushingToDisk the output should be \'T\'")
                {
                    ConversionState state(WindowsOpenModeFlag::IfPossibleNoFlushingToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("T"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::DeleteAfterClose the output should be \'D\'")
                {
                    ConversionState state(WindowsOpenModeFlag::DeleteAfterClose);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("D"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::UnicodeEncoding the output should be \',css=UNICODE\'")
                {
                    ConversionState state(WindowsOpenModeFlag::UnicodeEncoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr(",css=UNICODE"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::Utf8Encoding the output should be \',css=UTF-8\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Utf8Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr(",css=UTF-8"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::Utf16Encoding the output should be \',css=UTF-16LE\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Utf16Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr(",css=UTF-16LE"));
                    REQUIRE(output == expected);
                }
            }

            WHEN("Contructed with a multiple additional flags the additional mode str should contain"
                 " all of the flag codes concatenated")
            {
                WHEN("Cosntructed with flags Text, MustNotExist the output should be \'tx\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Text, OpenModeFlag::MustNotExist);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("tx"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Binary, Utf8Encoding the output should be \'b,css=UTF-8\'")
                {
                    ConversionState state(OpenModeFlag::Binary, WindowsOpenModeFlag::Utf8Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("b,css=UTF-8"));
                    REQUIRE(output == expected);
                }
                //The order shouldnt matter
                WHEN("Cosntructed with flags Utf8Encoding, Binary the output should be \'b,css=UTF-8\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Utf8Encoding, OpenModeFlag::Binary);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("b,css=UTF-8"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags NotInheritedByChildProcess, RandomAccessOptimized the output should be \'NR\'")
                {
                    ConversionState state(WindowsOpenModeFlag::NotInheritedByChildProcess, WindowsOpenModeFlag::RandomAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("NR"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags MustNotExist, Text, CommitDirectlyToDisk, DeleteAfterClose the output should be \'txcD\'")
                {
                    ConversionState state(OpenModeFlag::MustNotExist, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::DeleteAfterClose);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("txcD"));
                    REQUIRE(output == expected);
                }
            }

            WHEN("Contructed with a additional flags and core flags the additional mode str should contain"
                 " only the additional flag codes cocatenated")
            {
                WHEN("Cosntructed with flags Binary, DeleteAfterClose, Read, Write the output should be \'bD\'")
                {
                    ConversionState state(OpenModeFlag::Binary, WindowsOpenModeFlag::DeleteAfterClose, OpenModeFlag::Read, OpenModeFlag::Write);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bD"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Binary, Read, DeleteAfterClose, Append the output should be \'bD\'")
                {
                    ConversionState state(OpenModeFlag::Binary, OpenModeFlag::Read, WindowsOpenModeFlag::DeleteAfterClose, OpenModeFlag::Append);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bD"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Binary, Write, DeleteAfterClose, MustExist the output should be \'bD\'")
                {
                    ConversionState state(OpenModeFlag::Binary, OpenModeFlag::Write, WindowsOpenModeFlag::DeleteAfterClose, OpenModeFlag::MustExist);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bD"));
                    REQUIRE(output == expected);
                }
            }
        }
    }

    namespace OpenModeClassTesting
    {
        TEST_CASE("[OpenMode] : Only char and wchar_t specialisations should compile", "[OpenMode]")
        {
            [[maybe_unused]] BasicOpenMode<char8> mode8;
            [[maybe_unused]] BasicOpenMode<charW> modeW;

            #if OpenMode_TEST_COMPILE_ERROR == true
            //Should not compile
            [[maybe_unused]] BasicOpenMode<int> modeInt;
            [[maybe_unused]] BasicOpenMode<double> modeDouble;
            [[maybe_unused]] BasicOpenMode<short> modeShort;
            [[maybe_unused]] BasicOpenMode<long> modeLing;
            [[maybe_unused]] BasicOpenMode<std::string> modeString;
            [[maybe_unused]] BasicOpenMode<std::wstring> modeWString;
            #endif
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Default constructor should create an openmode with invalid Validty", "[OpenMode]", OpenModeTestedTypes)
       {
            const OpenMode mode;
            REQUIRE(mode.IsValid() == false);
            REQUIRE(mode.IsSupported() == true);
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Constructor with COpenMode should crete always valid openmode with appropriate OpenModeString", "[OpenMode]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;

            WHEN("Provided with COpenMode::ReadMustExist the OpenModeStr should be \'r\'")
            {
                const OpenMode mode(COpenMode::ReadMustExist);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::Write the OpenModeStr should be \'w\'")
            {
                const OpenMode mode(COpenMode::Write);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::WriteAppend the OpenModeStr should be \'a\'")
            {
                const OpenMode mode(COpenMode::WriteAppend);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteMustExist the OpenModeStr should be \'r+\'")
            {
                const OpenMode mode(COpenMode::ReadWriteMustExist);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWrite the OpenModeStr should be \'w+\'")
            {
                const OpenMode mode(COpenMode::ReadWrite);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteAppend the OpenModeStr should be \'a+\'")
            {
                const OpenMode mode(COpenMode::ReadWriteAppend);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : AssignCOpenMode should change the openmode to always valid openmode with appropriate OpenModeString", "[OpenMode]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;

            OpenMode mode;

            WHEN("Provided with COpenMode::ReadMustExist the OpenModeStr should be \'r\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadMustExist);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::Write the OpenModeStr should be \'w\'")
            {
                mode.AssignCOpenMode(COpenMode::Write);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::WriteAppend the OpenModeStr should be \'a\'")
            {
                mode.AssignCOpenMode(COpenMode::WriteAppend);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteMustExist the OpenModeStr should be \'r+\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadWriteMustExist);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWrite the OpenModeStr should be \'w+\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadWrite);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadWriteAppend the OpenModeStr should be \'a+\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadWriteAppend);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Constructor called with opeindividual OpenModeFlags should create a open mode according to the flags passed", "[OpenMode]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;
            using OsStringView = typename OpenMode::OsStringView;

            WHEN("Called with valid flags it should construct a valid openmode")
            {
                WHEN("Provided with flags Read MustExist the OpenModeStr should be \'r\'"
                     " and it should be valid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::MustExist);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r"));

                    const OsStringView outputView = output;
                    const OsStringView expectedView = expected;
                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Read, Write, MustExist, Binary the OpenModeStr should be \'r+b\'"
                     " and it should be valid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+b"));

                    const OsStringView outputView = output;
                    const OsStringView expectedView = expected;
                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, MustNotExist, CommitDirectlyToDisk, SequntialAccessOptimized the OpenModeStr should be \'wxcS\'"
                     " and it should be valid")
                {
                    const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::MustNotExist, WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::SequntialAccessOptimized);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("wxcS"));

                    const OsStringView outputView = output;
                    const OsStringView expectedView = expected;
                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                    CHECK(mode.IsValid() == true);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, MustNotExist, CommitDirectlyToDisk, SequntialAccessOptimized the OpenModeStr should be \'w+ND,css=UTF-8\'"
                     " and it should be valid")
                {
                    const OpenMode mode(OpenModeFlag::Read, WindowsOpenModeFlag::NotInheritedByChildProcess, WindowsOpenModeFlag::DeleteAfterClose, WindowsOpenModeFlag::Utf8Encoding);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+ND,css=UTF-8"));

                    const OsStringView outputView = output;
                    const OsStringView expectedView = expected;
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
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::MustNotExist);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Read, Write, Text, Binary it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::Text, OpenModeFlag::Binary);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, UnicodeEncoding, Utf8Encoding it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Write, WindowsOpenModeFlag::UnicodeEncoding, WindowsOpenModeFlag::Utf8Encoding);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Write, Append, CommitDirectlyToDisk, CommitIndirectlyToDisk it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Append, WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::CommitIndirectlyToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Read, SequntialAccessOptimized, RandomAccessOptimized it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Read, WindowsOpenModeFlag::SequntialAccessOptimized, WindowsOpenModeFlag::RandomAccessOptimized);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
            }

            WHEN("Called with invalid flags (insufficient information) it should construct a invalid openmode")
            {
                WHEN("Provided with no flags it should be invalid")
                {
                    const OpenMode mode;
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags MustNotExist it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::MustNotExist);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Append, Text it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Append, WindowsOpenModeFlag::Text);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags MustExist, UnicodeEncoding, Binary, CommitDirectlyToDisk it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::MustExist, WindowsOpenModeFlag::UnicodeEncoding, OpenModeFlag::Binary, WindowsOpenModeFlag::CommitDirectlyToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
                WHEN("Provided with flags Append, MustExist, DeleteAfterClose, IfPossibleNoFlushingToDisk it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Append, OpenModeFlag::MustExist, WindowsOpenModeFlag::DeleteAfterClose, WindowsOpenModeFlag::IfPossibleNoFlushingToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == true);
                }
            }

            WHEN("Called with usnupported combination of flags (Append, MustExist together alongside Write or Read) it should construct an unsupported openmode")
            {
                WHEN("Provided with flags Read, MustExist, Append, CommitDirectlyToDisk it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Append, WindowsOpenModeFlag::CommitDirectlyToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == false);
                }
                WHEN("Provided with flags Read, MustExist, Append, CommitDirectlyToDisk it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::MustExist, WindowsOpenModeFlag::UnicodeEncoding, OpenModeFlag::Append);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == false);
                }
                WHEN("Provided with flags Read, MustExist, Append, CommitDirectlyToDisk it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write,  OpenModeFlag::MustExist, OpenModeFlag::Append, WindowsOpenModeFlag::IfPossibleNoFlushingToDisk);
                    CHECK(mode.IsValid() == false);
                    CHECK(mode.IsSupported() == false);
                }
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : IsValid shoudl return true if the openmode is valid (fit to open a file)", "[OpenMode]", OpenModeTestedTypes)
        {
            WHEN("A constructed mode is valid IsValid returns true")
            {
                const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                REQUIRE(mode.IsValid() == true);
            }
            WHEN("A constructed mode is invalid IsValid returns false")
            {
                const OpenMode mode(OpenModeFlag::MustExist, WindowsOpenModeFlag::Text, OpenModeFlag::MustNotExist);
                REQUIRE(mode.IsValid() == false);
            }
            WHEN("A constructed mode is unsupported IsValid returns false")
            {
                const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                REQUIRE(mode.IsValid() == false);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : IsSupported should return true if the openmode is supported (does not have to valid)", "[OpenMode]", OpenModeTestedTypes)
        {
            WHEN("A constructed mode is valid IsSupported returns true")
            {
                const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                REQUIRE(mode.IsSupported() == true);
            }
            WHEN("A constructed mode is invalid IsSupported returns true")
            {
                const OpenMode mode(OpenModeFlag::MustExist, WindowsOpenModeFlag::Text, OpenModeFlag::MustNotExist);
                REQUIRE(mode.IsSupported() == true);
            }
            WHEN("A constructed mode is unsupported IsSupported returns false")
            {
                const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                REQUIRE(mode.IsSupported() == false);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : GetOpenModeString should return a copy of the constexpr openmode string", "[OpenMode]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;

            const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
            REQUIRE(mode.IsValid() == true);

            const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+"));
            REQUIRE(mode.GetOpenModeString() == expected);
        }

        TEMPLATE_TEST_CASE("[OpenMode] : operator StringView<OsCharType> should return a stringview conatining just the openmode", "[OpenMode]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using OsStringView = typename OpenMode::OsStringView;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;

            const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
            REQUIRE(mode.IsValid() == true);

            const OsStringView output = mode;
            const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+"));
            const OsStringView expectedView = expected;
            REQUIRE(output == expectedView);
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Copy constructor should construct a copy of the provided open mode", "[OpenMode]", OpenModeTestedTypes)
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
                const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                const OpenMode constructed(mode);

                REQUIRE(constructed.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(constructed.IsValid() == true);
                REQUIRE(constructed.IsSupported() == true);
            }
            WHEN("Constructing from an unsupported openmode the result should be unsupported aswell")
            {
                const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                const OpenMode constructed(mode);

                REQUIRE(constructed.IsValid() == false);
                REQUIRE(constructed.IsSupported() == false);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Move constructor should construct a copy of the provided open mode", "[OpenMode]", OpenModeTestedTypes)
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
                OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
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
                OpenMode mode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
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

        TEMPLATE_TEST_CASE("[OpenMode] : Copy assigning to OpenMode should copy the provided OpenMode", "[OpenMode]", OpenModeTestedTypes)
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
                const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                OpenMode assigned;

                assigned = mode;

                REQUIRE(assigned.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(assigned.IsValid() == true);
                REQUIRE(assigned.IsSupported() == true);
            }
            WHEN("Assigning from an unsupported openmode the result should be unsupported aswell")
            {
                const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
                OpenMode assigned;

                assigned = mode;

                REQUIRE(assigned.IsValid() == false);
                REQUIRE(assigned.IsSupported() == false);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Move assigning to OpenMode should copy the provided OpenMode", "[OpenMode]", OpenModeTestedTypes)
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
                OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
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
                OpenMode mode(OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Append);
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

        TEMPLATE_TEST_CASE("[OpenMode] : ", "[OpenMode]", OpenModeTestedTypes)
        {

        }
    }
}

#endif // OPENMODETESTING_H
