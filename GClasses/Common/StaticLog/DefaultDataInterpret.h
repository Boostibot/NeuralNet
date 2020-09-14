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
            inline void InterpretArg (const int REF val, std::string REF outputString)                   {Formater.format_signed(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (const long REF val, std::string REF outputString)                  {Formater.format_signed(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (const long long REF val, std::string REF outputString)             {Formater.format_signed(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (const unsigned REF val, std::string REF outputString)              {Formater.format_unsigned(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (const unsigned long REF val, std::string REF outputString)         {Formater.format_unsigned(val); outputString.assign(Formater.data(), Formater.size());}
            inline void InterpretArg (const unsigned long long REF val, std::string REF outputString)    {Formater.format_unsigned(val); outputString.assign(Formater.data(), Formater.size());}

            static inline void InterpretArg (const float REF val, std::string REF outputString)                {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:f}"), val);}
            static inline void InterpretArg (const double REF val, std::string REF outputString)               {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:f}"), val);}
            static inline void InterpretArg (const long double REF val, std::string REF outputString)          {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:.16f}"), val);}
            static inline void InterpretArg (void POINTER val, std::string REF outputString)                   {outputString.clear(); fmt::format_to(std::back_inserter(outputString), FMT_STRING("{:p}"), val);}

            static inline void InterpretArg (const bool REF val, std::string REF outputString)
            {
                if(val)
                    outputString = "true";
                else
                    outputString = "false";
            }
            static inline void InterpretArg (const char* val, std::string REF outputString)
            {
                outputString = val;
            }
            static inline void InterpretArg (std::string RVALUE_REF val, std::string REF outputString)
            {
                outputString = std::forward<std::string>(val);
            }
            static inline void InterpretArg (std::string_view RVALUE_REF val, std::string REF outputString)
            {
                outputString = std::forward<std::string_view>(val);
            }
    };
}
#endif // DATAINTERPRET_H
