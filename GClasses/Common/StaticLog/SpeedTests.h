#ifndef SPEEDTESTS_H
#define SPEEDTESTS_H

#include <iostream>

#include "StaticLog.h"
#include "TesterClasses.h"
#include "math.h"

using MicrosecondsChronoDurations = std::chrono::duration<double, std::ratio<1, 1000000>>;
u64 GetAverage(std::vector<MicrosecondsChronoDurations> REF durations)
{
    u64 returnValue = 0;
    for(auto i = durations.size(); --i > 0; )
    {
        returnValue += durations[i].count();
    }
    return returnValue / durations.size();
}

void AllocationTests()
{

    auto tempTimePoint = std::chrono::high_resolution_clock::now();
    constexpr u32 rotations = 10;
    constexpr u32 iterations = 50000;

    std::vector<MicrosecondsChronoDurations> staticLog(40);
    u32 currentSet = 0;

    StaticLog::DefaultLogger<> log("My file");

    //Set1
    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            log.LogMsgs<1>("pow");
        }
        staticLog[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    currentSet += 10;
    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            log.LogMsgs<1>("pow");
        }
        staticLog[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    currentSet += 10;
    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            log.LogMsgs<1>("pow");
        }
        staticLog[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    currentSet += 10;
    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            log.LogMsgs<1>("pow");
        }
        staticLog[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    std::cout << "================================================================\n";
    std::cout << "  StaticLog: " << GetAverage(staticLog) << "ms" << std::endl;
}

void SpeedTests()
{

    auto tempTimePoint = std::chrono::high_resolution_clock::now();
    constexpr u32 rotations = 10;
    constexpr u32 iterations = 50000;

    std::vector<MicrosecondsChronoDurations> cout(40);
    std::vector<MicrosecondsChronoDurations> staticLog(40);
    std::vector<MicrosecondsChronoDurations> control(10);

    double myResult = 0;
    double toPow = 5.3;
    (void)myResult;

    u32 currentSet = 0;

    StaticLog::DefaultLogger<> log("My file");

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            myResult = pow(i, toPow);
        }
        control[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    //Set1
    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            std::cout << "pow\n";
        }
        cout[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            log.LogMsgs<1>("pow");
        }
        staticLog[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    currentSet += 10;

    //Set2
    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            std::cout << "pow\n";
        }
        cout[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            log.LogMsgs<1>("pow");
        }
        staticLog[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    currentSet += 10;

    //Set3
    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            std::cout << "pow\n";
        }
        cout[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            log.LogMsgs<1>("pow");
        }
        staticLog[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    currentSet += 10;

    //Set4
    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            std::cout << "pow\n";
        }
        cout[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            log.LogMsgs<1>("pow");
        }
        staticLog[currentSet + rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }


    std::cout << "================================================================\n";
    std::cout << "  Cout: " << GetAverage(cout) << "ms\n";
    std::cout << "  StaticLog: " << GetAverage(staticLog) << "ms\n";
    std::cout << "  Control: " << GetAverage(control) << "ms\n";
}

#include <cstdio>

void LibSpeedTests()
{

    auto tempTimePoint = std::chrono::high_resolution_clock::now();
    constexpr u32 rotations = 10;
    constexpr u32 iterations = 10000;

    std::vector<MicrosecondsChronoDurations> Cout(10);
    std::vector<MicrosecondsChronoDurations> CoutWrite(10);
    std::vector<MicrosecondsChronoDurations> Puts(10);
    std::vector<MicrosecondsChronoDurations> Printf(10);
    std::vector<MicrosecondsChronoDurations> FmtPrint(10);
    std::vector<MicrosecondsChronoDurations> Control(10);

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            std::cout << "pow\n";
        }
        Cout[rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            std::cout.write("pow\n", 4);
        }
        CoutWrite[rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            puts("pow");
        }
        Puts[rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            printf("pow\n");
        }
        Printf[rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            fmt::print("pow\n");
        }
        FmtPrint[rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    for(u32 rotation = 0; rotation < rotations; rotation ++)
    {
        tempTimePoint = std::chrono::high_resolution_clock::now();
        for(u32 i = 0; i < iterations; i++)
        {
            (void)i;
        }
        Control[rotation] = std::chrono::high_resolution_clock::now() - tempTimePoint;
    }

    std::cout << "================================================================\n";
    std::cout << "  Cout: " << GetAverage(Cout) << "ms\n";
    std::cout << "  CoutWrite: " << GetAverage(CoutWrite) << "ms\n";
    std::cout << "  Puts: " << GetAverage(Puts) << "ms\n";
    std::cout << "  Printf: " << GetAverage(Printf) << "ms\n";
    std::cout << "  FmtPrint: " << GetAverage(FmtPrint) << "ms\n";
    std::cout << "  Control: " << GetAverage(Control) << "ms\n";
}

#endif // SPEEDTESTS_H
