#ifndef LOGTESTS_H
#define LOGTESTS_H

#include <vector>
#include "Log.h"


template<typename Type>
class ConstructionViewer : public Type
{
    public:
        ConstructionViewer() : Type()
        {
            std::cout << "Default Constructed" << std::endl;
        }
        ConstructionViewer(const ConstructionViewer PASS_REF viewer) : Type(reinterpret_cast<Type>(viewer))
        {
            std::cout << "Copy Constructed" << std::endl;
        }
        ConstructionViewer(ConstructionViewer PASS_RVALUE_REF viewer) : Type(std::move(reinterpret_cast<Type>(viewer)))
        {
            std::cout << "Move Constructed" << std::endl;
        }
        ~ConstructionViewer() {std::cout << "Destructed" << std::endl;}
};

template<typename Type>
class ConstructionViewer2
{
    public:
        Type Viewed;
        ConstructionViewer2() : Viewed()
        {
            std::cout << "Default Constructed" << std::endl;
        }
        ConstructionViewer2(const ConstructionViewer2 PASS_REF viewer) : Viewed(viewer.Viewed)
        {
            std::cout << "Copy Constructed" << std::endl;
        }
        ConstructionViewer2(ConstructionViewer2 PASS_RVALUE_REF viewer) : Viewed(std::move(viewer.Viewed))
        {
            std::cout << "Move Constructed" << std::endl;
        }
        ~ConstructionViewer2() {std::cout << "Destructed" << std::endl;}
};

class SpeedTestBase
{
    public:
        virtual void Function5(const int i1, const int i2, const int i3, const int i4, const int i5) = 0;
        virtual void Function4(const int i1, const int i2, const int i3, const int i4) = 0;
        virtual void Function3(const int i1, const int i2, const int i3) = 0;
        virtual void Function2(const int i1, const int i2) = 0;
        virtual void Function1(const int i1) = 0;
};

class SpeedTestOperative : public SpeedTestBase
{
    public:
        int iR = 0;

    public:
        virtual void Function5(const int i1, const int i2, const int i3, const int i4, const int i5) {iR += i1 + i2 + i3 + i4 + i5;}
        virtual void Function4(const int i1, const int i2, const int i3, const int i4) {iR += i1 + i2 + i3 + i4;}
        virtual void Function3(const int i1, const int i2, const int i3) {iR += i1 + i2 + i3;}
        virtual void Function2(const int i1, const int i2) {iR += i1 + i2;}
        virtual void Function1(const int i1) {iR += i1;}
};

class SpeedTestPassive : public SpeedTestBase
{
    public:
        virtual void Function5(const int, const int, const int, const int, const int) {}
        virtual void Function4(const int, const int, const int, const int) {}
        virtual void Function3(const int, const int, const int) {}
        virtual void Function2(const int, const int) {}
        virtual void Function1(const int) {}
};

class SpeedTestSimple
{
    public:
        bool Disabled = true;

    public:
        int iR = 0;

    public:
        void Function5(const int i1, const int i2, const int i3, const int i4, const int i5) {if(Disabled) iR += i1 + i2 + i3 + i4 + i5;}
        void Function4(const int i1, const int i2, const int i3, const int i4) {if(Disabled) iR += i1 + i2 + i3 + i4;}
        void Function3(const int i1, const int i2, const int i3) {if(Disabled) iR += i1 + i2 + i3;}
        void Function2(const int i1, const int i2) {if(Disabled) iR += i1 + i2;}
        void Function1(const int i1) {if(Disabled) iR += i1;}
};


struct Durations
{
        using MicrosecondsChronoDurations = std::chrono::duration<double, std::ratio<1, 1000>>;
    public:
        MicrosecondsChronoDurations VirtualOnDuration;
        MicrosecondsChronoDurations VirtualOffDuration;

        MicrosecondsChronoDurations SimplePtrOnDuration;
        MicrosecondsChronoDurations SimplePtrOffDuration;

        MicrosecondsChronoDurations SimpleOnDuration;
        MicrosecondsChronoDurations SimpleOffDuration;
};

#define TestFuncSpeed(functionNum, nameAndAccess, durationName, ...) \
    TempTimePoint = std::chrono::high_resolution_clock::now(); \
    for(u32 i = 0; i < iterations; i++) \
        nameAndAccess CONCAT(Function, functionNum)(__VA_ARGS__); \
    durations.at(functionNum - 1).durationName = std::chrono::high_resolution_clock::now() - TempTimePoint;



void SpeedTest()
{
    std::unique_ptr<SpeedTestBase> virtualOn = std::make_unique<SpeedTestOperative>();
    std::unique_ptr<SpeedTestBase> virtualOff = std::make_unique<SpeedTestPassive>();
    std::unique_ptr<SpeedTestSimple> SimplePtr = std::make_unique<SpeedTestSimple>();
    SpeedTestSimple Simple;
    std::vector<Durations> durations(5);
    std::chrono::time_point<std::chrono::high_resolution_clock> TempTimePoint;

    constexpr u32 iterations = 100000000;

    TestFuncSpeed(1, virtualOn->, VirtualOnDuration, 1);
    TestFuncSpeed(2, virtualOn->, VirtualOnDuration, 1, 1);
    TestFuncSpeed(3, virtualOn->, VirtualOnDuration, 1, 1, 1);
    TestFuncSpeed(4, virtualOn->, VirtualOnDuration, 1, 1, 1, 1);
    TestFuncSpeed(5, virtualOn->, VirtualOnDuration, 1, 1, 1, 1, 1);

    TestFuncSpeed(1, virtualOff->, VirtualOffDuration, 1);
    TestFuncSpeed(2, virtualOff->, VirtualOffDuration, 1, 1);
    TestFuncSpeed(3, virtualOff->, VirtualOffDuration, 1, 1, 1);
    TestFuncSpeed(4, virtualOff->, VirtualOffDuration, 1, 1, 1, 1);
    TestFuncSpeed(5, virtualOff->, VirtualOffDuration, 1, 1, 1, 1, 1);

    SimplePtr->Disabled = false;
    TestFuncSpeed(1, SimplePtr->, SimplePtrOnDuration, 1);
    TestFuncSpeed(2, SimplePtr->, SimplePtrOnDuration, 1, 1);
    TestFuncSpeed(3, SimplePtr->, SimplePtrOnDuration, 1, 1, 1);
    TestFuncSpeed(4, SimplePtr->, SimplePtrOnDuration, 1, 1, 1, 1);
    TestFuncSpeed(5, SimplePtr->, SimplePtrOnDuration, 1, 1, 1, 1, 1);

    SimplePtr->Disabled = true;
    TestFuncSpeed(1, SimplePtr->, SimplePtrOffDuration, 1);
    TestFuncSpeed(2, SimplePtr->, SimplePtrOffDuration, 1, 1);
    TestFuncSpeed(3, SimplePtr->, SimplePtrOffDuration, 1, 1, 1);
    TestFuncSpeed(4, SimplePtr->, SimplePtrOffDuration, 1, 1, 1, 1);
    TestFuncSpeed(5, SimplePtr->, SimplePtrOffDuration, 1, 1, 1, 1, 1);

    Simple.Disabled = false;
    TestFuncSpeed(1, Simple., SimpleOnDuration, 1);
    TestFuncSpeed(2, Simple., SimpleOnDuration, 1, 1);
    TestFuncSpeed(3, Simple., SimpleOnDuration, 1, 1, 1);
    TestFuncSpeed(4, Simple., SimpleOnDuration, 1, 1, 1, 1);
    TestFuncSpeed(5, Simple., SimpleOnDuration, 1, 1, 1, 1, 1);

    Simple.Disabled = true;
    TestFuncSpeed(1, Simple., SimpleOffDuration, 1);
    TestFuncSpeed(2, Simple., SimpleOffDuration, 1, 1);
    TestFuncSpeed(3, Simple., SimpleOffDuration, 1, 1, 1);
    TestFuncSpeed(4, Simple., SimpleOffDuration, 1, 1, 1, 1);
    TestFuncSpeed(5, Simple., SimpleOffDuration, 1, 1, 1, 1, 1);


    Simple.Disabled = false;
    for(u32 i = 0; i < iterations; i++)
        Simple.Function1(1);

    for(int i = 0; i < 5; i++)
    {
        std::cout << std::endl;
        std::cout << "Durations for function with " << i + 1 << " arguments:" << std::endl;
        std::cout << "Virtual On:     " << durations[i].VirtualOnDuration.count() << " mikro s" << std::endl;
        std::cout << "Simple Ptr On:  " << durations[i].SimplePtrOnDuration.count() << " mikro s" << std::endl;
        std::cout << "Simple On:      " << durations[i].SimpleOnDuration.count() << " mikro s" << std::endl;

        std::cout << "Virtual Off:    " << durations[i].VirtualOffDuration.count() << " mikro s" << std::endl;
        std::cout << "Simple Ptr Off: " << durations[i].SimplePtrOffDuration.count() << " mikro s" << std::endl;
        std::cout << "Simple Off:     " << durations[i].SimpleOffDuration.count() << " mikro s" << std::endl;
    }
}

#endif
