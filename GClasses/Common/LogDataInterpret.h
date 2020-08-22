#ifndef LOGDATAINTERPRET_H
#define LOGDATAINTERPRET_H

#include "LogWriter.h"

//DataInterpret is a class supposed to convert non string values values to string
//It has to be done statically since templates (which are convinient) cannot be inside virtual classes
//Logger interprets must publically inherit from this class with the template argument of the class itself (ie. myClass :  DataInterpretInterface<myClass, myClassDataPackage>)
template <typename Derived, typename DerivedDataPackage>
class DataInterpretInterface : public DerivedDataPackage
{
        //The following functions are obligatory to be overloaded
    public:
        template <typename Type>
        inline std::string InetrpretArg(Type arg)
        {
            return static_cast<Derived POINTER>(this)->InetrpretArg(arg);
        }

};

//The default logger interpret
struct DefaultDataInterpretDataPackage
{
    public:
        using OStream = std::basic_ostringstream<char>;

    public:
        OStream Stream;
};
class DefaultDataInterpret : public DataInterpretInterface<DefaultDataInterpret, DefaultDataInterpretDataPackage>
{
    public:
        using ThisPackageType = DefaultDataInterpretDataPackage;

        //The obligatory function overloads
    public:
        template <typename Type>
        inline std::string InetrpretArg(Type arg)
        {
            Stream << arg;
            std::string temp = Stream.str();
            Stream.str("");

            return temp;
        }
};

#endif // LOGDATAINTERPRET_H
