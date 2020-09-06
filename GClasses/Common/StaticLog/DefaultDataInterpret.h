#ifndef DATAINTERPRET_H
#define DATAINTERPRET_H

#include "LoggerInterface.h"

#include "Libraries/Fmt/fmt/compile.h"
#include "Libraries/Fmt/fmt/core.h"
#include "Libraries/Fmt/fmt/os.h"

#include <array>
namespace StaticLog
{
    class DefaultDataInterpret
    {
        public:
            fmt::format_int Formater;

        public:
            inline void InterpretArg (int val, std::string PASS_REF outputString)                   {Formater.format_signed(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (long val, std::string PASS_REF outputString)                  {Formater.format_signed(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (long long val, std::string PASS_REF outputString)             {Formater.format_signed(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (unsigned val, std::string PASS_REF outputString)              {Formater.format_unsigned(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (unsigned long val, std::string PASS_REF outputString)         {Formater.format_unsigned(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (unsigned long long val, std::string PASS_REF outputString)    {Formater.format_unsigned(val); outputString.assign(Formater.data(), Formater.size());}
            /*
            inline void InterpretArg (int val, std::string PASS_REF outputString)                  {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:-d}"), val);}
            inline void InterpretArg (long val, std::string PASS_REF outputString)                 {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:-d}"), val);}
            inline void InterpretArg (long long val, std::string PASS_REF outputString)            {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:-d}"), val);}
            inline void InterpretArg (unsigned val, std::string PASS_REF outputString)             {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:d}"), val);}
            inline void InterpretArg (unsigned long val, std::string PASS_REF outputString)        {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:d}"), val);}
            inline void InterpretArg (unsigned long long val, std::string PASS_REF outputString)   {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:d}"), val);}
            */
            static inline void InterpretArg (float val, std::string PASS_REF outputString)                {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:f}"), val);}
            static inline void InterpretArg (double val, std::string PASS_REF outputString)               {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:f}"), val);}
            static inline void InterpretArg (long double val, std::string PASS_REF outputString)          {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:.16f}"), val);}
            static inline void InterpretArg (void POINTER val, std::string PASS_REF outputString)         {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:p}"), val);}

            static inline void InterpretArg (bool val, std::string PASS_REF outputString)
            {
                if(val)
                    outputString = "true";
                else
                    outputString = "false";
            }
            static inline void InterpretArg (const char* val, std::string PASS_REF outputString)
            {
                outputString = val;
            }
            static inline void InterpretArg (std::string val, std::string PASS_REF outputString)
            {
                outputString = val;
            }
            static inline void InterpretArg (std::string_view val, std::string PASS_REF outputString)
            {
                outputString = val;
            }
    };
}
#endif // DATAINTERPRET_H
