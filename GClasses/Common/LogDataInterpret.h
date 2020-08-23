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
class CustomConversionDataInterpret : public DataInterpretInterface<DefaultDataInterpret, DefaultDataInterpretDataPackage>
{
    public:
        using ThisPackageType = DefaultDataInterpretDataPackage;

        //The obligatory function overloads
    public:

        inline void InetrpretArg (int val, std::string PASS_REF outputString)                  {outputString += std::to_string(val);}
        inline void InetrpretArg (long val, std::string PASS_REF outputString)                 {outputString += std::to_string(val);}
        inline void InetrpretArg (long long val, std::string PASS_REF outputString)            {outputString += std::to_string(val);}
        inline void InetrpretArg (unsigned val, std::string PASS_REF outputString)             {outputString += std::to_string(val);}
        inline void InetrpretArg (unsigned long val, std::string PASS_REF outputString)        {outputString += std::to_string(val);}
        inline void InetrpretArg (unsigned long long val, std::string PASS_REF outputString)   {outputString += std::to_string(val);}
        inline void InetrpretArg (float val, std::string PASS_REF outputString)                {outputString += std::to_string(val);}
        inline void InetrpretArg (double val, std::string PASS_REF outputString)               {outputString += std::to_string(val);}
        inline void InetrpretArg (long double val, std::string PASS_REF outputString)          {outputString += std::to_string(val);}

        inline void InetrpretArg (void POINTER val, std::string PASS_REF outputString)         {outputString += std::to_string(reinterpret_cast<unsigned long long>(val));}
        inline void InetrpretArg (bool val, std::string PASS_REF outputString)
        {
            if(val)
                outputString += "true";
            else
                outputString += "false";
        }

    private:
        //TODO - implement templated pointer
        static inline void ConvertAsNumToString(const void POINTER ptr, const std::size_t size, std::string PASS_REF outputString)
        {

        }
};
/*
 *
 *
 * ostream& operator<< (bool val);
ostream& operator<< (short val);
ostream& operator<< (unsigned short val);
ostream& operator<< (int val);
ostream& operator<< (unsigned int val);
ostream& operator<< (long val);
ostream& operator<< (unsigned long val);
ostream& operator<< (float val);
ostream& operator<< (double val);
ostream& operator<< (long double val);
ostream& operator<< (void* val);

string to_string (int val);
string to_string (long val);
string to_string (long long val);
string to_string (unsigned val);
string to_string (unsigned long val);
string to_string (unsigned long long val);
string to_string (float val);
string to_string (double val);
string to_string (long double val);
*/

#endif // LOGDATAINTERPRET_H
