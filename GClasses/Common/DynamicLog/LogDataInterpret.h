#ifndef LOGDATAINTERPRET_H
#define LOGDATAINTERPRET_H

#include "LogWriter.h"

/*
void * operator new(size_t size)
{
    std::cout<< "Overloading new operator with size: " << size << std::endl;

    return malloc(size);
}

void operator delete(void * p)
{
    std::cout<< "Overloading delete operator " << std::endl;
    free(p);
}
*/

//DataInterpret is a class supposed to convert non string values values to string
//It has to be done statically since templates (which are convinient) cannot be inside virtual classes
//Logger interprets must publically inherit from this class with the template argument of the class itself (ie. myClass :  DataInterpretInterface<myClass, myClassDataPackage>)
template <typename Derived, typename DerivedDataPackage>
class DataInterpretInterface : public DerivedDataPackage
{
        //The following functions are obligatory to be overloaded
    public:
        template <typename Type>
        inline void InetrpretArg(Type arg, std::string REF outputString)
        {
            static_cast<Derived POINTER>(this)->InetrpretArg(arg, outputString);
        }

};

//The default logger interpret
struct OStreamDataInterpretDataPackage
{
    public:
        using OStream = std::basic_ostringstream<char>;

    public:
        OStream Stream;
};
class OStreamDataInterpret : public DataInterpretInterface<OStreamDataInterpret, OStreamDataInterpretDataPackage>
{
    public:
        using ThisPackageType = OStreamDataInterpretDataPackage;

        //The obligatory function overloads
    public:
        template <typename Type>
        inline void InetrpretArg(Type arg, std::string REF outputString)
        {
            Stream << arg;
            outputString = Stream.str();
            Stream.str("");
        }
};

struct DefaultDataInterpretDataPackage {};
class DefaultDataInterpret : public DataInterpretInterface<DefaultDataInterpret, DefaultDataInterpretDataPackage>
{
    public:
        using ThisPackageType = DefaultDataInterpretDataPackage;

        //The obligatory function overloads
    public:

        inline void InetrpretArg (int val, std::string REF outputString)                  {outputString = std::to_string(val);}
        inline void InetrpretArg (long val, std::string REF outputString)                 {outputString = std::to_string(val);}
        inline void InetrpretArg (long long val, std::string REF outputString)            {outputString = std::to_string(val);}
        inline void InetrpretArg (unsigned val, std::string REF outputString)             {outputString = std::to_string(val);}
        inline void InetrpretArg (unsigned long val, std::string REF outputString)        {outputString = std::to_string(val);}
        inline void InetrpretArg (unsigned long long val, std::string REF outputString)   {outputString = std::to_string(val);}
        inline void InetrpretArg (float val, std::string REF outputString)                {outputString = std::to_string(val);}
        inline void InetrpretArg (double val, std::string REF outputString)               {outputString = std::to_string(val);}
        inline void InetrpretArg (long double val, std::string REF outputString)          {outputString = std::to_string(val);}

        inline void InetrpretArg (void POINTER val, std::string REF outputString)         {outputString = std::to_string(reinterpret_cast<unsigned long long>(val));}
        inline void InetrpretArg (bool val, std::string REF outputString)
        {
            if(val)
                outputString = "true";
            else
                outputString = "false";
        }
};

#endif // LOGDATAINTERPRET_H
