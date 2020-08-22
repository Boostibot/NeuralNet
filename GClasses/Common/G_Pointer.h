#ifndef G_POINTER_H
#define G_POINTER_H

#include "G_Defines.h"
#include "G_GeneralFunctions.h"
#include <type_traits>

//Is part of the G namespace
namespace G
{
    //Anabling code defines
    #define G_POINTER_NO_PADDING            true    //Enables/Disables padding for this class; Shoudl have no effect since this class can only be 8B long
    #define G_POINTER_SHORT_FUNC_SYNONIMS   true    //Enables/Disables functions: Set() (Equivalent to SetPtr()), Get (Equivalent to GetPtr())
    #define G_POINTER_MSVC_COMPILER         true    //Enables/Disables MSVC specific forced inline expansion
    #define G_POINTER_GCC_COMPILER          false   //Enables/Disables GCC specific forced inline expansion

    #if G_POINTER_NO_PADDING == true
        #pragma pack(push, 1) //Should have no effect; Is here just to be sure
    #endif //G_POINTER_NO_PADDING


    template<typename PointerType>
    class Pointer
    {
        ///Using
        protected:
            using ThisType = Pointer<PointerType>;

        ///Members
        //protected:
        public:
            PointerType* InternalPointer;                                                                               //Pointer which is a core of the class; Defaultly is set to NULL; Can be set with this->SetPtr(...) or this->SetRef(...)


        ///Operators
        public:
            ///CHANGE - Check if other class is of G::Pointer Type
            //Set operators
            template<typename PtrT,
                     std::enable_if_t<std::is_pointer<PtrT>::value || is_same_type_template<PtrT, ThisType>, int> = 0>   //Function argument must be either pointer type or of G::Pointer type
            inline void         operator =      (PtrT gPointer) const {this->SetPtr(gPointer);}                         //

            //Get operators
            template<typename PtrT = PointerType*,
                     std::enable_if_t<std::is_pointer<PtrT>::value, int> = 0>
            inline PointerType* operator ->     () const                        {return this->GetPtr<PtrT>();}              //

            //Info operators
            template<typename PtrT,
                     std::enable_if_t<std::is_pointer<PtrT>::value || is_same_type_template<PtrT, ThisType>, int> = 0>   //Function argument must be either pointer type or of G::Pointer type
            inline bool         operator ==     (PtrT gPointer) const {return this->IsEqual(gPointer);}           //
            template<typename PtrT,
                     std::enable_if_t<std::is_pointer<PtrT>::value || is_same_type_template<PtrT, ThisType>, int> = 0>   //Function argument must be either pointer type or of G::Pointer type
            inline bool         operator !=     (PtrT gPointer) const {return this->IsNotEqual(gPointer);}        //

            //Conversion operators
            template<typename OtherGPointerT,
                     std::enable_if_t<is_same_type_template<OtherGPointerT, ThisType>, int> = 0> //Checks if OtherGPointer is derived from the G::Pointer type
            inline operator OtherGPointerT() const {return POINTER_VALUE(reinterpret_cast<OtherGPointerT POINTER>(this));}

        ///Constructors
        public:
            inline Pointer<PointerType>         ()              {this->Reset();}                                        //

            ///CHANGE
            template<typename PtrT CLASSIFIED_AS_POINTER,
                     std::enable_if_t<std::is_pointer<PtrT>::value, int> = 0>
            inline Pointer<PointerType>         (PtrT ptr)      {this->SetPtr(ptr);}                                    //


        ///Destructors
        public:
            inline ~Pointer () {}                                                                                       //


        ///Getting functions
        public:            
            template<typename PtrT = PointerType*, //Default return type is set to this class pointer
                     std::enable_if_t<std::is_pointer<PtrT>::value, int> = 0>
            inline PtrT CLASSIFIED_AS_POINTER const GetPtr() const {return reinterpret_cast<PtrT>(InternalPointer);}                       //Returns this->InternalPointer int the given type

            template<typename ValT = PointerType>   //Default return type is set to this class
            inline ValT RETURN_REF                  GetRef() const {return static_cast<ValT RETURN_REF>(POINTER_VALUE(this->GetPtr()));}   //Gets the value pointer using this->Get() then dereferences it and tries to cast it onto the given type; Returns a reference

        #if G_POINTER_SHORT_FUNC_SYNONIMS == true
            template<typename PtrT = PointerType*,
                     std::enable_if_t<std::is_pointer<PtrT>::value, int> = 0>
            inline PtrT CLASSIFIED_AS_POINTER const  Get() const {return this->GetPtr<PtrT>();} //Same as this->GetPtr()
        #endif


        ///Setting functions
        public:
            template <typename PtrT,
                      std::enable_if_t<std::is_pointer<PtrT>::value, int> = 0>                              //Function argument can only be of a pointer type
            inline void     SetPtr  (PtrT ptr)  {InternalPointer = const_cast<PointerType POINTER>(ptr);}   //Sets this->InternalPointer to the given pointer

            template <typename OtherGPointerT,
                      std::enable_if_t<is_same_type_template<OtherGPointerT, ThisType>, int> = 0>                                       //Checks if OtherGPointer is derived from the G::Pointer type
            inline void     SetPtr  (OtherGPointerT gPointer)  {InternalPointer = const_cast<PointerType POINTER>(gPointer.GetPtr());}  //Sets this->InternalPointer to the value of gPointer.GetPtr()

            ///CHANGE
            template <typename RefT>
            inline void     SetRef  (RefT const PASS_BY_REFERENCE ref)    {this->SetPtr(ADDRESS(ref));}     //Sets this->InternalPointer to the adDress of the given argument

        #if G_POINTER_SHORT_FUNC_SYNONIMS == true
            template <typename PtrT,
                      std::enable_if_t<std::is_pointer<PtrT>::value || is_same_type_template<PtrT, ThisType>, int> = 0>   //Function argument must be either pointer type or of G::Pointer type
            inline void     Set     (PtrT ptr)                      {this->SetPtr(ptr);}
        #endif


        ///Info functions
        public:
            inline bool IsNull          () const {return IS_NULL(this->GetPtr());}                                       //Returns true if this->InetrnalPointer is NULL otherwise false
            inline bool IsNotNull       () const {return IS_NOT_NULL(this->GetPtr());}                                   //Returns false if this->InetrnalPointer is NULL otherwise true
            inline bool IsPointing      () const {return this->IsNotNull();}                                             //Returns true if this->InetrnalPointer is NULL otherwise false
            inline bool IsNotPointing   () const {return this->IsNull();}                                                //Returns false if this->InetrnalPointer is NULL otherwise true

            template <typename PtrT,
                      std::enable_if_t<std::is_pointer<PtrT>::value, int> = 0>                      //Function argument can only be of a pointer type
            inline bool IsEqual     (PtrT ptr) const {return IS_EQUAL(this->GetPtr(), ptr);}        //Returns true if a this->InternalPointer is equal to gPointer.InternalPointer otherwise false
            template <typename PtrT,
                      std::enable_if_t<std::is_pointer<PtrT>::value, int> = 0>                      //Function argument can only be of a pointer type
            inline bool IsNotEqual  (PtrT ptr) const {return IS_NOT_EQUAL(this->GetPtr(), ptr);}    //Returns false if a this->InternalPointer is equal to gPointer.InternalPointer otherwise true

            template <typename OtherGPointerT,
                      std::enable_if_t<is_same_type_template<OtherGPointerT, ThisType>, int> = 0>                               //Checks if OtherGPointer is derived from the G::Pointer type
            inline bool IsEqual     (OtherGPointerT gPointer) const {return IS_EQUAL(this->GetPtr(), gPointer.GetPtr());}       //Returns true if a this->InternalPointer is equal to pgPointer.InternalPointer otherwise false
            template <typename OtherGPointerT,
                      std::enable_if_t<is_same_type_template<OtherGPointerT, ThisType>, int> = 0>                               //Checks if OtherGPointer is derived from the G::Pointer type
            inline bool IsNotEqual  (OtherGPointerT gPointer) const {return IS_NOT_EQUAL(this->GetPtr(), gPointer.GetPtr());}   //Returns false if a this->InternalPointer is equal to pgPointer.InternalPointer otherwise true


        ///Other
        public:
            inline void Null        () {InternalPointer = NULLPTR;}                                                     //Sets this->InternalPointer to NULL (default value signaling the pointer is not in use)
            inline void Reset       () {this->Null();}                                                                  //Sets this->InternalPointer to NULL (default value signaling the pointer is not in use)

        ///CHANGE
        ///Static
        public:
            inline static const ThisType Create              ()                              {ThisType pointer; return pointer;}                //An empty Pointer<...> value is equal to NULL
            template <typename PtrT>
            inline static const ThisType Create              (PtrT ptr)                      {ThisType pointer(ptr); return pointer;}           //
            template <typename RefT>
            inline static const ThisType CreateFromRef       (RefT PASS_BY_REFERENCE ref)    {ThisType pointer(ADDRESS(ref)); return pointer;}   //

    };

    #if G_POINTER_NO_PADDING == true
        #pragma pack(pop) //Stops no padding (from this point on padding is normal)
    #endif //G_POINTER_NO_PADDING

}

#endif // G_POINTER_H
