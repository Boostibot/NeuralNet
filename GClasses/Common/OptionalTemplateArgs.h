#ifndef OPTIONALTEMPLATEARGUMENTS_H
#define OPTIONALTEMPLATEARGUMENTS_H

#include "GClasses/Common/G_Common.h"

namespace MetaPrograming
{
    namespace OptionalArgDetail
    {
            template<typename, typename>
            constexpr bool is_same_type_template{false};


            template<   template<typename...> class T, //typename T in C++17
                        typename... A,
                        typename... B>
            constexpr bool is_same_type_template<
                T<A...>,
                T<B...>>
            {true};

            template<typename LookingForTemplate>
            constexpr bool IsTypeTemplatePresent() {return false;}
            template<typename LookingForTemplate, typename FirstTemplate, typename ... Templates>
            constexpr bool IsTypeTemplatePresent()
            {
                if constexpr (is_same_type_template<LookingForTemplate, FirstTemplate>)
                    return true;
                else
                    return IsTypeTemplatePresent<LookingForTemplate, Templates...>();
            }

            template<size_t currentIndex,
                     typename LookingForTemplate>
            constexpr size_t GetTypeTemplateIndexHelper() {return currentIndex;}

            template<size_t currentIndex,
                     typename LookingForTemplate, typename FirstTemplate, typename ... Templates>
            constexpr size_t GetTypeTemplateIndexHelper()
            {
                if constexpr (is_same_type_template<LookingForTemplate, FirstTemplate>)
                    return currentIndex;
                else
                    return GetTypeTemplateIndexHelper<currentIndex + 1, LookingForTemplate, Templates...>();
            }


            template<typename LookingForTemplate, typename FirstTemplate, typename ... Templates>
            constexpr size_t GetTypeTemplateIndex() {return GetTypeTemplateIndexHelper<0, LookingForTemplate, FirstTemplate, Templates...>();}


            template <int N, typename... T>
            struct tuple_element;

            template <typename T0, typename... T>
            struct tuple_element<0, T0, T...> {
                typedef T0 type;
            };
            template <int N, typename T0, typename... T>
            struct tuple_element<N, T0, T...> {
                typedef typename tuple_element<N-1, T...>::type type;
            };

            template<int index, typename... Types>
            using GetTypeOnIndex = typename tuple_element<index, Types...>::type;


            //Type
            template<typename LookingForType, typename OrType, typename ... Types,
                     std::enable_if_t<IsTypePresent<LookingForType, Types...>::value, int> = 0>
            constexpr LookingForType GetTypeIfPresentOr() {}

            template<typename LookingForType, typename OrType, typename ... Types,
                     std::enable_if_t<!IsTypePresent<LookingForType, Types...>::value, int> = 0>
            constexpr OrType GetTypeIfPresentOr() {}


            //Type template
            template<typename LookingForType, typename OrType, typename ... Types,
                     std::enable_if_t<IsTypeTemplatePresent<LookingForType, Types...>(), int> = 0>
            constexpr auto GetTypeTemplateIfPresentOr() -> GetTypeOnIndex<GetTypeTemplateIndex<LookingForType, Types...>(), Types...> {}

            template<typename LookingForType, typename OrType, typename ... Types,
                     std::enable_if_t<!IsTypeTemplatePresent<LookingForType, Types...>(), int> = 0>
            constexpr auto GetTypeTemplateIfPresentOr() -> OrType {}

            //Retrieve
            template<typename LookingForType, typename OrType, typename ... Types>
            using RetrieveType = decltype(GetTypeIfPresentOr<LookingForType, OrType, Types...>());

            template<typename LookingForTemplate, typename OrType, typename ... Types>
            using RetrieveTypeTemplate = decltype(GetTypeTemplateIfPresentOr<LookingForTemplate, OrType, Types...>());
    };

    template<typename... OptionalTypes>
    struct OptionalArgs
    {
            template<typename LookingForType, typename OrType>
            using RetrieveType = decltype(OptionalArgDetail::GetTypeIfPresentOr<LookingForType, OrType, OptionalTypes...>());

            template<typename LookingForTemplate, typename OrType>
            using RetrieveTypeTemplate = decltype(OptionalArgDetail::GetTypeTemplateIfPresentOr<LookingForTemplate, OrType, OptionalTypes...>());
    };

    namespace OptionalArgExample
    {

        template <typename T>
        struct DoSourceLog
        {
                using ValueType = T;
        };

        template <typename T>
        struct DoAppOutputLog
        {
                using ValueType = T;
        };

        template <typename T>
        struct InterpretType
        {
                using ValueType = T;
        };

        template <typename T>
        struct IterationFielSize
        {
                using ValueType = T;
        };

        template <typename T>
        struct ClockFielSize
        {
                using ValueType = T;
        };

        struct DefaultDataInterpret {int a = 0;};

        //Specialised options
        //Used to hold the metaprograming code
        template <typename ... OptionTypes>
        struct SpecialisedOptions
        {
                //Specialisation for conviniencec
                using ThisOptions = MetaPrograming::OptionalArgs<OptionTypes...>;
                using DefaultInterpretType = DefaultDataInterpret;

                //getting the individual types for convinience

                //MetaPrograming::OptionalArgs<>::RetrieveTypeTemplate<[1st type template], [2nd type template]>  use:
                //[1st type template]   - The type template the function is trying to retrieve
                //                      -
                using DoSourceLogType       = typename ThisOptions::template RetrieveTypeTemplate< DoSourceLog<BoolVal<true>>,              DoSourceLog<BoolVal<true>>          >;
                using DoAppOutputLogType    = typename ThisOptions::template RetrieveTypeTemplate< DoAppOutputLog<BoolVal<true>>,           DoAppOutputLog<BoolVal<true>>       >;
                using IterationFielSizeType = typename ThisOptions::template RetrieveTypeTemplate< IterationFielSize<UintVal<8>>,           IterationFielSize<UintVal<8>>       >;
                using ClockFielSizeType     = typename ThisOptions::template RetrieveTypeTemplate< ClockFielSize<UintVal<8>>,               ClockFielSize<UintVal<8>>           >;
                using InterpretType         = typename ThisOptions::template RetrieveTypeTemplate< InterpretType<DefaultInterpretType>,     InterpretType<DefaultInterpretType> >;

                static constexpr bool   DoSourceLog         = DoSourceLogType::ValueType::Value;
                static constexpr bool   DoAppOutputLog        = DoAppOutputLogType::ValueType::Value;
                static constexpr u32    IterationFielSize   = IterationFielSizeType::ValueType::Value;
                static constexpr u32    ClockFielSize       = ClockFielSizeType::ValueType::Value;
                InterpretType Interpret;
        };

        template <typename ... OptionTypes>
        struct ClassUsingOptions : SpecialisedOptions<OptionTypes...>
        {
            //Notmal class declaration
        };

        void TestOptionalArg()
        {
            bool look = false;
            u32 lookU = 0;

            ClassUsingOptions<> test1;
            look = test1.DoSourceLog;
            lookU = test1.ClockFielSize;

            ClassUsingOptions<DoSourceLog<BoolVal<false>>> test2;
            look = test2.DoSourceLog;
            lookU = test2.ClockFielSize;

            ClassUsingOptions<
                    DoSourceLog<BoolVal<false>>,
                    DoAppOutputLog<BoolVal<false>>,
                    ClockFielSize<UintVal<1234>>> test3;
            look = test3.DoAppOutputLog;
            lookU = test3.ClockFielSize;

            (void)look;
            (void)lookU;
        }

    }

}

#endif // OPTIONALTEMPLATEARGUMENTS_H
