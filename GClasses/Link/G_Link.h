#ifndef LINK_H
#define LINK_H

#include "G_Common.h"



template<typename ObjectType>
class PrivateLinkCore
{

        ///Using
    private:
        using ThisType = PrivateLinkCore<ObjectType>;
    public:
        using ObjectPointerType = ObjectType POINTER;


        ///Members
    public:
        //Is defaultly set to NULL so that class can auto update on constructor after being reallocated
        ObjectPointerType ObjectPointer = NULLPTR;


        ///Constructors
    public:
        //CHANGE
        PrivateLinkCore()
        {

        }
        ~PrivateLinkCore()
        {

        }


        ///Info functions
    public:
        inline bool IsSet() {return IS_NOT_NULL(this->ObjectPointer);}
        inline bool IsNotSet() {return IS_NULL(this->ObjectPointer);}


        ///Member functions
    public:
        ObjectPointerType             GetObjectPointer()                      {return this->ObjectPointer;}
        ObjectType RETURN_REF   GetObject()                             {return POINTER_VALUE(this->GetObjectPointer());}
        template<typename PointerType,
                 std::enable_if_t<std::is_pointer<PointerType>::value, int> = 0>
        void SetObjectPointer(PointerType CLASSIFIED_AS_POINTER pointer)
        {
            this->ObjectPointer = reinterpret_cast<ObjectType POINTER>(pointer);
        }
        void SetObject(ObjectType const PASS_REF object)            {this->SetObjectPointer(ADDRESS(object));}

};

#define LINK_RESPOND_UNIT_DEFAULT_SIZE 3
#define LINK_RESPOND_UNIT_INCREMENT_SIZE 3

template<size_t InitialSize = 3>
class LinkRespondStaticArray
{
    ///Using
    private:
        using ThisType = LinkRespondStaticArray<InitialSize>;
    public:
        using SizeType = size_t;
        using IlustrativeObjectType = u32;
        using LinkCoreType = PrivateLinkCore<IlustrativeObjectType>;
        using LinkCorePointerType = LinkCoreType POINTER;


    ///Members
    public:
        SizeType Size = InitialSize;
        LinkCorePointerType LinkArray[InitialSize];


    ///Constructors and destructors
    public:
        LinkRespondStaticArray() {this->NullLinkPointerArray();}
        ~LinkRespondStaticArray() {}


    ///Member functions
    public:
        inline SizeType GetSize()               {return this->Size;}
        inline void     SetSize(SizeType size)  {this->Size = size;}

        inline LinkCorePointerType  GetLinkPointer(SizeType index)                          {return this->LinkArray[index];}
        inline void             SetLinkPointer(SizeType index, LinkCorePointerType value)   {this->LinkArray[index] = value;}
        inline void             NullLinkPointer(SizeType index)                         {this->SetLinkPointer(index, NULLPTR);}

        inline void NullLinkPointerArray() {NullMemory(this, this->GetByteSize());}


    ///Size
    public:
        static inline size_t GetArrayByteSize(SizeType arraySize)
        {
            return arraySize * sizeof(LinkCoreType);
        }
        inline size_t GetArrayByteSize()
        {
            return ThisType::GetArrayByteSize(this->GetSize());
        }

        static inline size_t GetByteSize(SizeType arraySize)
        {
            return sizeof(LinkRespondStaticArray) - sizeof(LinkCoreType) + ThisType::GetArrayByteSize(arraySize);
        }
        inline size_t GetByteSize()
        {
            return ThisType::GetByteSize(this->GetSize());
        }


    ///Allocating
    public:
        static inline ThisType POINTER AllocateRespondArray(SizeType newArraySize)
        {
            return static_cast<ThisType POINTER>(AllocateMemoryNull(ThisType::GetByteSize(newArraySize)));
        }
        template<typename RespondArrayPointerType,
                 std::enable_if_t<std::is_pointer<RespondArrayPointerType>::value, int> = 0>
        static inline ThisType POINTER ReallocateRespondArray(RespondArrayPointerType CLASSIFIED_AS_POINTER oldRespondArray, SizeType newArraySize)
        {
            //If old array is null = just create a new array
            //Calling GetSize() would result in a crash
            ThisType POINTER respondArrayPtr;
            if(oldRespondArray == NULLPTR)
            {
                respondArrayPtr = static_cast<ThisType POINTER>(ReallocateMemory(oldRespondArray, ThisType::GetByteSize(newArraySize)));

                //If reallocation failed returns the respondArrayPtr
                if(respondArrayPtr == NULLPTR)
                    return respondArrayPtr;

                //Formats the respondArrayPtr
                ThisType::NullRespondArray(respondArrayPtr, newArraySize);

            }
            //If the new size is bigger then the old size
            else if(oldRespondArray->GetSize() < newArraySize)
            {
                respondArrayPtr = static_cast<ThisType POINTER>(ReallocateMemory(oldRespondArray, ThisType::GetByteSize(newArraySize)));

                if(respondArrayPtr == NULLPTR)
                    return respondArrayPtr;

                SizeType oldArraySize = respondArrayPtr->GetSize();
                ThisType::NullRespondArray(respondArrayPtr, oldArraySize, newArraySize - oldArraySize);
            }
            else
            {
                //Else is shrinking
                respondArrayPtr = static_cast<ThisType POINTER>(ReallocateMemory(oldRespondArray, ThisType::GetByteSize(newArraySize)));

                //If reallocation failed returns the respondArrayPtr
                if(respondArrayPtr == NULLPTR)
                    return respondArrayPtr;
            }

            //Formats the respondArrayPtr
            respondArrayPtr->SetSize(newArraySize);

            return respondArrayPtr;

        }
        template<typename RespondArrayPointerType,
                 std::enable_if_t<std::is_pointer<RespondArrayPointerType>::value, int> = 0>
        static inline void DeallocateRespondArray(RespondArrayPointerType CLASSIFIED_AS_POINTER oldRespondArray)
        {
            Deallocate(oldRespondArray);
        }

        static inline void NullRespondArray(ThisType POINTER respondArray, SizeType arraySize)
        {
            NullMemory(respondArray->LinkArray,  ThisType::GetArrayByteSize(arraySize));
        }
        static inline void NullRespondArray(ThisType POINTER respondArray, SizeType index, SizeType count)
        {
            NullMemory(ADDRESS(respondArray->LinkArray[index]),  ThisType::GetArrayByteSize(count));
        }


    ///Info
    public:
        inline bool IsIndexInBounds(SizeType index)
        {
            if(this->GetSize() > index)
                return TRUE;
            return FALSE;
        }
        inline bool IsIndexNotInBounds(SizeType index) {return NOT(this->IsIndexInBounds(index));}

        inline bool IsIndexOfValue(SizeType index, LinkCorePointerType value) {return IS_EQUAL(this->GetLinkPointer(index), value);}
        inline bool IsIndexNotOfValue(SizeType index, LinkCorePointerType value) {return NOT(this->IsIndexOfValue(index, value));}

        inline bool IsIndexOpen(SizeType index) {return IS_NULL(this->GetLinkPointer(index));}
        inline bool IsIndexNotOpen(SizeType index) {return NOT(this->IsIndexOpen(index));}

        inline SizeType GetMaxIndex()           {return this->GetSize() - 1;}
        inline SizeType GetMinIndex()           {return 0;}


    ///Main functions
    public:
        SizeType GetFirstIndexOf(LinkCorePointerType value)
        {
            //Cycles while the index is in range
            for(SizeType i = 0; this->IsIndexInBounds(i); i++)
            {
                //If the current index has the desired value returns it
                if(this->IsIndexOfValue(i, value))
                    return i;
            }

            //Returns the size to indicate out ouf bounds error
            return this->GetSize();
        }
        SizeType GetFirstIndexOf(LinkCorePointerType value, SizeType startingIndex)
        {
            //Cycles while the index is in range
            for(SizeType i = startingIndex; this->IsIndexInBounds(i); i++)
            {
                //If the current index has the desired value returns it
                if(this->IsIndexOfValue(i, value))
                    return i;
            }

            //Returns the size to indicate out ouf bounds error
            return this->GetSize();
        }

        inline SizeType GetFirstOpenIndex() {return this->GetFirstIndexOf(NULLPTR);}
        inline SizeType GetFirstOpenIndex(SizeType startingIndex) {return this->GetFirstIndexOf(NULLPTR, startingIndex);}
};


class LinkRespondUnit
{
    ///Using
    private:
        using ThisType = LinkRespondUnit;
    public:
        //Inherited
        using SizeType = LinkRespondStaticArray<>::SizeType;
        using LinkCoreType = LinkRespondStaticArray<>::LinkCoreType;
        using LinkCorePointerType = LinkRespondStaticArray<>::LinkCorePointerType;
        //New
        using ArrayType = LinkRespondStaticArray<>;
        using ArrayPointerType = ArrayType POINTER;


    ///Members
    public:
        ArrayPointerType LinkRespondArray;


    ///Operators
    public:
        void operator = (ThisType PASS_REF other)
        {
            this->InheritResponsesFrom(other);
        }


    ///Constructors and destructors
    public:
        LinkRespondUnit()
        {
            this->ResetRespondArray();
        }
        LinkRespondUnit(SizeType size)
        {
            this->ResetRespondArray();
            this->ResizeArray(size);
        }
        LinkRespondUnit(ThisType PASS_REF other)
        {
            this->InheritResponsesFrom(other);
        }
        ~LinkRespondUnit()
        {
            this->RemoveAllResponses();
            this->DeleteArray();
        }


    ///Member functions
    public:
        inline ArrayPointerType GetRespondArray()               {return this->LinkRespondArray;}
        inline void     ResetRespondArray(ArrayPointerType ptr = NULLPTR)  {this->LinkRespondArray = ptr;}
        inline void     SetFormatArray(ArrayPointerType arrayPtr, SizeType size)
        {
            this->ResetRespondArray(arrayPtr);
            this->GetRespondArray()->SetSize(size);
        }


    ///Info functions
    public:
        inline bool IsResponseArrayHeld() {return IS_NOT_NULL(this->GetRespondArray());}
        inline bool IsResponseArrayNotHeld() {return NOT(IsResponseArrayHeld());}

        inline SizeType GetResponseArraySize()
        {
            //If array is not set returns zero
            if(IsResponseArrayNotHeld())
                return NULL;

            //Respond the size
            return this->GetRespondArray()->GetSize();
        }

    ///Creating and deleting
    public:
        bool ResizeArray(SizeType newSize)
        {
            //Reallocates an Array of its appropriate size
            ArrayPointerType ptr = ArrayType::ReallocateRespondArray(this->GetRespondArray(), newSize);

            //If the pointer is NULL returns false (something went wrong)
            if(IS_NULL(ptr))
                return FALSE;

            //Else sets the new pointer
            this->SetFormatArray(ptr, newSize);
            return TRUE;
        }
        void DeleteArray()
        {
            //If no array is held returns
            if(this->IsResponseArrayNotHeld())
                return;

            //Deallocates the array held
            ArrayType::DeallocateRespondArray(this->GetRespondArray());
        }
    private:
        inline bool ExpandArrayNoChecks(SizeType expandSize) { return this->ResizeArray(this->GetRespondArray()->GetSize() + expandSize);}
        inline bool ExpandArrayNoChecks() { return this->ResizeArray(this->GetRespondArray()->GetSize() + LINK_RESPOND_UNIT_INCREMENT_SIZE);}
    public:
        inline bool ExpandArray(SizeType expandSize = LINK_RESPOND_UNIT_INCREMENT_SIZE)
        {
            if(this->IsResponseArrayNotHeld())
                return this->ResizeArray(expandSize);

            return ExpandArrayNoChecks(expandSize);
        }

        ///Private interal functions
    private:
        inline void SetResponseNoChecks(SizeType index, LinkCorePointerType caller)
        {
            caller->SetObjectPointer(this);
            this->GetRespondArray()->SetLinkPointer(index, caller);
        }
        inline void NullResponseNoChecks(SizeType index)
        {
            this->GetRespondArray()->GetLinkPointer(index)->SetObjectPointer(static_cast<ThisType POINTER>(NULLPTR));
            this->GetRespondArray()->NullLinkPointer(index);
        }
        inline void UpdateResponseNoChecks(SizeType index)
        {
            this->GetRespondArray()->GetLinkPointer(index)->SetObjectPointer(this);
        }

    ///Main functions
    public:
        bool AddResponse(LinkCorePointerType caller)
        {            
            //If caller is already set returns false
            //Should be solved from outside the class
            if(caller->IsSet())
                return FALSE;

            SizeType index;

            //If the reponse array is not held creates a new one
            if(this->IsResponseArrayNotHeld())
            {
                //Resizes the array to the
                if(this->ResizeArray(LINK_RESPOND_UNIT_INCREMENT_SIZE) == false)
                    return FALSE;

                //Sets index to 0 - first open index
                index = NULL;
            }
            //Else checcks for open index a
            else
            {
                //Finds the first open index
                index = this->GetRespondArray()->GetFirstOpenIndex();

                //If index is out of bounds resizzes the array
                if(this->GetRespondArray()->IsIndexNotInBounds(index))
                {
                    //Index is one higher then the maximum index

                    //Resizes the array to the current size + LINK_RESPOND_UNIT_INCREMENT_SIZE
                    if(this->ExpandArrayNoChecks() == false)
                        return FALSE;

                    //Index is now in bounds again
                }
            }

            //Creates a response
            this->SetResponseNoChecks(index, caller);
            return TRUE;
        }
        bool RemoveResponse(LinkCorePointerType caller)
        {
            //If caller is already set returns false
            //Should be solved from outside the class
            if(caller->IsNotSet())
                return FALSE;

            //If the reponse array is not held returns false
            if(this->IsResponseArrayNotHeld())
                return FALSE;

            //Removes the link pointer and returns the status
            SizeType index = this->GetRespondArray()->GetFirstIndexOf(caller);

            //If index is out of bounds returns error
            if(this->GetRespondArray()->IsIndexNotInBounds(index))
                return FALSE;

            this->NullResponseNoChecks(index);
            return TRUE;
        }
        void RemoveAllResponses()
        {
            //If the Response array is not created the returned size is 0 so the loop exits emidetely
            for(SizeType i = this->GetResponseArraySize(); i-- > 0; )
            {
                //If the current index is not empty nulls it
                if(this->GetRespondArray()->GetLinkPointer(i)->IsSet())
                    this->NullResponseNoChecks(i);
            }
        }
        void UpdateAllResponses()
        {
            //If the Response array is not created the returned size is 0 so the loop exits emidetely
            for(SizeType i = this->GetResponseArraySize(); i-- > 0; )
            {
                //If the current index is not empty sets it to point to this
                if(this->GetRespondArray()->IsIndexNotOpen(i))
                    this->UpdateResponseNoChecks(i);
            }
        }

        bool IsResponseSet(LinkCorePointerType caller)
        {
            //If the Response array is not created the returned size is 0 so the loop exits emidetely
            for(SizeType i = this->GetResponseArraySize(); i-- > 0; )
            {
                //If the current index is same as caller returns true
                if(this->GetRespondArray()->GetLinkPointer(i) == caller)
                    return TRUE;
            }

            //No index is same returns false
            return FALSE;
        }

        void InheritResponsesFrom(ThisType PASS_REF other)
        {
            //std::swap(this->LinkRespondArray, other.LinkRespondArray);
            this->ResetRespondArray(other.GetRespondArray());
            other.ResetRespondArray();
            this->UpdateAllResponses();
        }

};

template<typename ClassType, typename LinkRespondUnitType = LinkRespondUnit>
class PrivateLinkableObject : public ClassType, LinkRespondUnitType
{
        ///Using
    private:
        using ThisType = PrivateLinkableObject<ClassType, LinkRespondUnit>;


        ///Operators
    public:
        template< std::enable_if_t<!std::is_same<ClassType, void>::value, int> = 0>
        inline void operator = (ThisType PASS_REF other)
        {
            this->SetClass(other.GetClass());
            this->RespondUnit.InheritResponsesFrom(other.RespondUnit);
        }

        template< std::enable_if_t<std::is_same<ClassType, void>::value, int> = 0>
        inline void operator = (ThisType PASS_REF other)
        {
            this->RespondUnit.InheritResponsesFrom(other.RespondUnit);
        }

        //Returns the wrapped around class Class
        template< std::enable_if_t<!std::is_same<ClassType, void>::value, int> = 0>
        inline operator ClassType RETURN_REF()
        {
            return this->GetClass();
        }


        ///Constructors
    public:
        PrivateLinkableObject() : ClassType(), LinkRespondUnitType()
        {

        }
        ~PrivateLinkableObject()
        {

        }


        ///Main functions
    public:
        template< std::enable_if_t<!std::is_same<ClassType, void>::value, int> = 0>
        inline ClassType RETURN_REF GetClass()
        {
            return POINTER_VALUE(static_cast<ClassType POINTER>(this));
        }
        template< std::enable_if_t<!std::is_same<ClassType, void>::value, int> = 0>
        inline void SetClass(ClassType PASS_REF otherClass)
        {
            this->GetClass() = otherClass;
        }


        ///Using inheritance
    public:
        using LinkRespondUnitType::IsResponseSet;
        using LinkRespondUnitType::IsResponseArrayHeld;
        using LinkRespondUnitType::IsResponseArrayNotHeld;
};

template<typename ClassType, typename LinkRespondUnitType = LinkRespondUnit>
class LinkableObject : private PrivateLinkableObject<ClassType, LinkRespondUnitType>
{
    private:
        using PrivateLinkableObjectType = PrivateLinkableObject<ClassType, LinkRespondUnitType>;

    public:
        using PrivateLinkableObjectType::operator=;
        using PrivateLinkableObjectType::operator();

        using PrivateLinkableObjectType::IsResponseSet;
        using PrivateLinkableObjectType::IsResponseArrayHeld;
        using PrivateLinkableObjectType::IsResponseArrayNotHeld;
};


template<typename LinkableObjectType>
class PrivateLinkToInhereted : public PrivateLinkCore<LinkableObjectType>
{
    ///Using
    private:
        using ThisType = PrivateLinkToInhereted<LinkableObjectType>;
    public:
        using LinkRespondType = LinkRespondUnit;
        using ThisCoreType = LinkRespondUnit::LinkCoreType;


    ///Internal private functions
    private:
        template<typename PointerType,
                 std::enable_if_t<std::is_pointer<PointerType>::value, int> = 0>
        inline PointerType GetObjectPtrInType()
        {
            return reinterpret_cast<PointerType>(this->GetObjectPointer());
        }
        template<typename PointerType,
                 std::enable_if_t<std::is_pointer<PointerType>::value, int> = 0>
        inline PointerType GetThisInType()
        {
            return reinterpret_cast<PointerType>(this);
        }


    ///Main functions
    public:
        template <typename LinkableObjectType>
        bool Link(LinkableObjectType PASS_REF object)
        {
            //Delinks the current link
            this->Delink();

            //Links to a new object
            LinkRespondType POINTER objPtr = reinterpret_cast<LinkRespondType POINTER>(ADDRESS(object));
            return objPtr->AddResponse(this->GetThisInType<ThisCoreType POINTER>());
        }

        bool Delink()
        {
            //If is not set returns false
            if(this->IsNotSet())
                return FALSE;

            //returns the status from removing the response
            return this->GetObjectPtrInType<LinkRespondType POINTER>()->RemoveResponse(this->GetThisInType<ThisCoreType POINTER>());
        }

        inline bool IsLinked()
        {
            //If the link is not set returns false
            if(this->IsNotSet())
                return FALSE;

            //returns the status from IsResponseSet()
            return this->GetObjectPtrInType<LinkRespondType POINTER>()->IsResponseSet(this->GetThisInType<ThisCoreType POINTER>());
        }
        inline bool IsNotLinked()
        {
            return NOT(this->IsLinked());
        }
};


template<typename ClassType/*, std::enable_if_t<G::is_same_type_template<ObjectClassType, LinkableObject<u32>>, int> = 0*/>
class PrivateLink : public PrivateLinkCore<ClassType>
{
    ///Using
    private:
        using ThisType = PrivateLink<ClassType>;
    public:
        using LinkRespondType = LinkRespondUnit;
        using LinkableObjectType = LinkableObject<ClassType>;
        using ThisCoreType = LinkRespondUnit::LinkCoreType;


    ///Internal private functions
    private:
        template<typename PointerType,
                 std::enable_if_t<std::is_pointer<PointerType>::value, int> = 0>
        inline PointerType GetObjectPtrInType()
        {
            return reinterpret_cast<PointerType>(this->GetObjectPointer());
        }
        template<typename PointerType,
                 std::enable_if_t<std::is_pointer<PointerType>::value, int> = 0>
        inline PointerType GetThisInType()
        {
            return reinterpret_cast<PointerType>(this);
        }


    ///Main functions
    public:
        bool Link(LinkableObjectType PASS_REF object)
        {
            //Delinks the current link
            this->Delink();

            //Links to a new object
            LinkableObjectType POINTER objPtr = this->GetObjectPtrInType();
            return objPtr->AddResponse(this->GetThisInType<ThisCoreType POINTER>());
        }

        bool Delink()
        {
            //If is not set returns false
            if(this->IsNotSet())
                return FALSE;

            //returns the status from removing the response
            return this->GetObjectPtrInType<LinkRespondType POINTER>()->RemoveResponse(this->GetThisInType<ThisCoreType POINTER>());
        }

        inline bool IsLinked()
        {
            //If the link is not set returns false
            if(this->IsNotSet())
                return FALSE;

            //returns the status from IsResponseSet()
            return this->GetObjectPtrInType<LinkRespondType POINTER>()->IsResponseSet(this->GetThisInType<ThisCoreType POINTER>());
        }
        inline bool IsNotLinked()
        {
            return NOT(this->IsLinked());
        }
};


template<typename ObjectClassType>
class Link : private PrivateLinkCore<ObjectClassType>
{

};

#endif // LINK_H
