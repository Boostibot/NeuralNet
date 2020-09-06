#ifndef STATICLOG_H
#define STATICLOG_H

#include "GClasses/Common/G_Common.h"
#include "GClasses/Common/StaticLog/LoggerInterface.h"
#include "GClasses/Common/StaticLog/DefaultDataInterpret.h"
#include "GClasses/Common/StaticLog/DefaultLogger.h"
#include "GClasses/Common/StaticLog/LogDefines.h"

//TODO - Put this into an example file
/*
template <typename Derived, typename DerivedDataPackage>
class CrtpBase : public DerivedDataPackage
{
        //The following functions are obligatory to be overloaded
    public:
        template <typename Type>
        inline void BaseFunc(Type arg)
        {
            static_cast<Derived POINTER>(this)->OverloadedFunc(arg);
        }

};

//The default logger interpret
struct CrtpDerivedPackage
{
    public:
        int MyInt1;
        int MyInt2;
        int MyInt3;
};
class CrtpDerived : public CrtpBase<CrtpDerived, CrtpDerivedPackage>
{
    public:
        using ThisPackageType = CrtpDerivedPackage;

        //The obligatory function overloads
    private:
        template <typename Type>
        inline void OverloadedFunc(Type arg)
        {
            this->MyInt1 = 1; //Avalible to use
        }
};
*/



#endif // STATICLOG_H
