#pragma once

#include "../GClasses/Common/StaticLog.h"

namespace TEMP
{

    using StringType = ::std::string;
    using StringViewType = ::std::string_view;

    template <typename Derived,
        typename DerivedDataPackage = typename Derived::ThisPackageType>
        class LogWriterBase : public LogWriterTraits, public DerivedDataPackage
    {
    public:
        using StringType = ::std::string;
        using StringViewType = ::std::string_view;

    public:
        Derived REF GetThisAsDerived() const { return POINTER_VALUE(static_cast<Derived POINTER>(this)); }

        //Overloaded functions
    public:
        template<typename ... MsgTypes>
        void WriteMsgs(MsgTypes ... msgs) { GetThisAsDerived().WriteMsgs(msgs...); }
        template<typename ... NameTypes, typename ... VarTypes>
        void WriteVars(NameTypes ... names, VarTypes ... vars) { GetThisAsDerived().WriteVars(names..., vars...); }

        template<typename ... AdditionalTypes>
        void AppendSource(StringViewType file, const u32 lineNum, AdditionalTypes ... additional) { GetThisAsDerived().AppendSource(file, lineNum, additional...); }
        template<typename ... TagTypes>
        void AppendTags(TagTypes ... tags) { GetThisAsDerived().AppendTags(tags...); }

        template<typename ... PathTypes>
        void OpenStream(PathTypes ... paths) { GetThisAsDerived().OpenStream(paths...); }
        template<typename ... StreamIdentifierTypes>
        void CloseStream(StreamIdentifierTypes ... indetifiers) { GetThisAsDerived().CloseStream(indetifiers...); }
        template<typename ... StreamIdentifierTypes>
        bool IsStreamOpen(StreamIdentifierTypes ... indetifiers) { GetThisAsDerived().IsStreamOpen(indetifiers...); }
        template<typename ... ArgTypes>
        void FlushStream(ArgTypes ... args) { GetThisAsDerived().FlushStream(args...); }

        void OnConstruction() { GetThisAsDerived().OnConstruction(); }
        void OnDestruction() { GetThisAsDerived().OnDestruction(); }

        //Custom
        template<typename ... CustomTypes>
        void WriteCustom(CustomTypes ... customArgs) { GetThisAsDerived().WriteCustom(customArgs...); }
        template<typename ... CustomTypes>
        void WriteCustom() { GetThisAsDerived().template WriteCustom<CustomTypes...>(); }

        template<typename ... TagTypes>
        void AppendCustom(TagTypes ... tags) { GetThisAsDerived().AppendCustom(tags...); }
        template<typename ... TagTypes>
        void AppendCustom() { GetThisAsDerived().template AppendCustom<TagTypes...>(); }

        template<typename ReturnType, typename ... CustomTypes>
        ReturnType DoCustom(CustomTypes ... customs) { GetThisAsDerived().DoCustom(customs...); }
        template<typename ReturnType, typename ... CustomTypes>
        ReturnType DoCustom() { GetThisAsDerived().template DoCustom<CustomTypes...>(); }

    };

    struct TestingDataPackage1
    {
    public:
        StringType LastFunctionCalled;
        StringType Identifier = "TestingDataPackage";
    };

    struct TestingDataPackage2
    {
    public:
        int MyInt1;
        int MyInt2;
        int MyInt3;
        StringType LastFunctionCalled;
        StringType Identifier = "TestingDataPackage2";
    };

    struct TestingLogWriterBase1 : public LogWriterBase<TestingLogWriterBase1, TestingDataPackage1>
    {
    public:
        using ThisPackageType = StaticLogger::TestingDataPackage1;
    };

    struct TestingLogWriterBase2 : public LogWriterBase<TestingLogWriterBase2, TestingDataPackage1>
    {
    public:
        using ThisPackageType = StaticLogger::TestingDataPackage2;
    };
}
