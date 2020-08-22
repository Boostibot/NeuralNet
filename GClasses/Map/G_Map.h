#ifndef MAP_H
#define MAP_H

#include "G_Common.h"
//A map entry compare function class
//All functions must be overriden
template<typename KeyType>
class MapEntryCompare_Base
{
    public:
        //Returns true if both values are equal
        static inline bool IsEqual(KeyType key1, KeyType key2);
        //Returns true if key1 > key2
        static inline bool IsBigger(KeyType key1, KeyType key2);
        //Returns true if key1 < key2
        static inline bool IsSmaller(KeyType key1, KeyType key2);
};

//A defaultly used map entry compare function class
template<typename KeyType>
class MapEntryCompare_Standart : MapEntryCompare_Base<KeyType>
{
    public:
        //Returns true if both values are equal
        static inline bool IsEqual(KeyType key1, KeyType key2)
        {
            return IS_EQUAL(key1, key2);
        }
        //Returns true if key1 > key2
        static inline bool IsBigger(KeyType key1, KeyType key2)
        {
            return IS_BIGGER(key1, key2);
        }
        //Returns true if key1 < key2
        static inline bool IsSmaller(KeyType key1, KeyType key2)
        {
            return IS_SMALLER(key1, key2);
        }
};

template<typename KeyTypeArg, typename ValueTypeArg, typename EntryCompareType = MapEntryCompare_Standart<KeyTypeArg>>
class MapEntry
{
        ///Using
    public:
        using KeyType = KeyTypeArg;
        using ValueType = ValueTypeArg;
    private:
        using ThisType = MapEntry<KeyType, ValueType>;

        ///Members
    public:
        KeyType Key;
        ValueType Value;


        ///Member functions
    public:
        //Getting members
        inline KeyType RETURN_REF          GetKeyRef()
        {
            return this->Key;
        }
        inline ValueType RETURN_REF        GetValueRef()
        {
            return this->Value;
        }
        inline KeyType                     GetKey()
        {
            return this->GetKeyRef();
        }
        inline ValueType                   GetValue()
        {
            return this->GetValueRef();
        }
        inline KeyType RETURN_POINTER      GetKeyPtr()
        {
            return ADDRESS(this->GetKeyRef());
        }
        inline ValueType RETURN_POINTER    GetValuePtr()
        {
            return ADDRESS(this->GetValueRef());
        }

    public:
        //Setting members
        inline void SetKey(KeyType key)
        {
            this->GetKeyRef() = key;
        }
        inline void SetValue(ValueType value)
        {
            this->GetValueRef() = value;
        }
        inline void SetKeyRef(KeyType PASS_REF key)
        {
            this->GetKeyRef() = key;
        }
        inline void SetValueRef(ValueType PASS_REF value)
        {
            this->GetValueRef() = value;
        }

        inline void Set(KeyType key, ValueType value)
        {
            this->SetKey(key);
            this->SetValue(value);
        }
        inline void Set(ThisType PASS_REF parrent)
        {
            this->SetKey(parrent.GetKey());
            this->SetValue(parrent.GetValue());
        }

        inline void NullSelf()
        {
            NullType(POINTER_VALUE(this));
        }


        ///Info functions
    public:
        //Returns true if key of this == key
        inline bool IsEqual(KeyType key)
        {
            return EntryCompareType::IsEqual(this->GetKey(), key);
        }
        //Returns true if key of this > key
        inline bool IsBigger(KeyType key)
        {
            return EntryCompareType::IsBigger(this->GetKey(), key);
        }
        //Returns true if key of this < key
        inline bool IsSmaller(KeyType key)
        {
            return EntryCompareType::IsSmaller(this->GetKey(), key);
        }


        ///Constructor and destructor
    public:
        MapEntry()
        {

        }
        MapEntry(KeyType key, ValueType value)
        {
            this->Set(key, value);
        }
        MapEntry(const ThisType PASS_REF parrent)
        {
            this->Set(const_cast<ThisType PASS_REF>(parrent));
        }
        ~MapEntry() {}
};

template<typename KeyTypeArg, KeyTypeArg InitialKey, typename ValueTypeArg, ValueTypeArg InitialValue, typename EntryCompareTypeArg = MapEntryCompare_Standart<KeyTypeArg>>
class MapEntryFixed : public MapEntry<KeyTypeArg, ValueTypeArg, EntryCompareTypeArg>
{
        ///Using
    public:
        using KeyType = KeyTypeArg;
        using ValueType = ValueTypeArg;
        using EntryCompareType = EntryCompareTypeArg;
        using MapEntryType = MapEntry<KeyType, ValueType, EntryCompareType>;

    private:
        using ThisType = MapEntryFixed<KeyType, InitialKey, ValueType, InitialValue>;

        //CHANGE -ADD CONVERT CONSTRUCTOR TO THE MapENtry

    public:
        inline operator MapEntryType()
        {
            return this->GetMapEntry();
        }


        ///Constructor and destructor
    public:
        MapEntryFixed() {this->Initialise();}
        MapEntryFixed(const ThisType PASS_REF parrent)
        {
            this->Set(const_cast<ThisType PASS_REF>(parrent));
        }
        MapEntryFixed(const MapEntryType PASS_REF parrent)
        {
            this->Set(const_cast<MapEntryType PASS_REF>(parrent));
        }
        ~MapEntryFixed() {}

        ///Main functions
    public:
        inline void Initialise()
        {
            this->Set(InitialKey, InitialValue);
        }
        inline MapEntryType RETURN_REF GetMapEntry()
        {
            return POINTER_VALUE(static_cast<MapEntryType POINTER>(this));
        }
};

//CHANGE - NO ArgumentPack
template<typename KeyTypeArg, typename ValueTypeArg, typename ... EntryTypes>
class MapStaticLocalCore
{
        ///Using
    public:
        using KeyType = KeyTypeArg;
        using ValueType = ValueTypeArg;
    private:
        using ThisType = MapStaticLocalCore<KeyType, ValueType, EntryTypes...>;

    public:
        using MapEntryType = MapEntryFixed<KeyType, NULL, ValueType, NULL>;
        using SizeType = size_t;
        using IndexType = SizeType;


        ///Constexpr functions
    public:
        //Returns the count of Entries in this Map
        constexpr static SizeType GetEntryCount()
        {
            return sizeof... (EntryTypes);
        }
        //Returns the maximum index in this Map
        constexpr static SizeType GetMaxIndex()
        {
            //If the size is 0 returns zero (should never happen)
            if(ThisType::GetEntryCount() == 0)
                return 0;

            //Else returns the value of size - 1
            return ThisType::GetEntryCount() - 1;
        }


        ///Members
    public:
        MapEntryType EntryArray[ThisType::GetEntryCount()];


        ///Member functions
    public:
        inline MapEntryType RETURN_REF GetEntryRef(IndexType index)
        {
            return this->EntryArray[index];
        }
};

template<typename KeyTypeArg, typename ValueTypeArg, typename ... EntryTypes>
class MapStaticGlobalCore
{
        ///Using
    public:
        using KeyType = KeyTypeArg;
        using ValueType = ValueTypeArg;
    private:
        using ThisType = MapStaticLocalCore<KeyType, ValueType, EntryTypes...>;

    public:
        using MapEntryType = MapEntryFixed<KeyType, NULL, ValueType, NULL>;
        using SizeType = size_t;
        using IndexType = SizeType;


        ///Constexpr functions
    public:
        //Returns the count of Entries in this Map
        constexpr static SizeType GetEntryCount()
        {
            return sizeof... (EntryTypes);
        }
        //Returns the maximum index in this Map
        constexpr static SizeType GetMaxIndex()
        {
            //If the size is 0 returns zero (should never happen)
            if(ThisType::GetEntryCount() == 0)
                return 0;

            //Else returns the value of size - 1
            return ThisType::GetEntryCount() - 1;
        }


        ///Members
    public:
        static MapEntryType EntryArray[ThisType::GetEntryCount()];


        ///Member functions
    public:
        inline MapEntryType RETURN_REF GetEntryRef(IndexType index)
        {
            return this->EntryArray[index];
        }
};

//CHANGE - add checks
//CHANGE - change the sattic word for a better word that describes the same thing
//       - static can be confusign whith the static keyword
template<typename MapStaticCoreType, typename KeyType, typename ValueType, typename ... EntryTypes>
class MapStatic : public MapStaticCoreType
{
        ///Using
    private:
        using ThisType = MapStatic<MapStaticCoreType, KeyType, ValueType, EntryTypes...>;
    public:
        using MapEntryType = MapEntryFixed<KeyType, NULL, ValueType, NULL>;
        using MapCoreType = MapStaticLocalCore<KeyType, ValueType, EntryTypes...>;
        using SizeType = typename MapCoreType::SizeType;
        using IndexType = typename MapCoreType::IndexType;


        ///Members - inhereted


        ///Member functions
    public:
        inline MapEntryType GetEntry(IndexType index)
        {
            return this->GetEntryRef(index);
        }
        inline MapEntryType RETURN_POINTER GetEntryPtr(IndexType index)
        {
            return ADDRESS(this->GetEntryRef(index));
        }
        inline void SetEntry(MapEntryType value, IndexType index)
        {
            this->GetEntryRef(index) = value;
        }


        ///Constructors and destructors
    public:
        MapStatic()
        {
            this->InitialiseArray();
        }
        ~MapStatic() {}


        ///Info functions
    public:
        constexpr bool IsIndexInBounds(IndexType index)
        {
            //If index is at leats one smaller then the size returns true
            if(index < ThisType::GetEntryCount())
                return TRUE;
            return FALSE;
        }
        constexpr bool IsIndexInNotBounds(IndexType index)
        {
            return NOT(ThisType::IsIndexInBounds(index));
        }


        ///Private helper functions
    private:
        //Sets a single entry (element in the EntryArray) to its appopriate value
        //All give entries need to have the same KeyType and ValueType as thsi class
        template<typename SpecilaisedEntryType>
        inline void InitialiseEntryIncrement(SpecilaisedEntryType entry, IndexType PASS_REF index)
        {
            this->SetEntry(POINTER_VALUE(reinterpret_cast<MapEntryType POINTER>(ADDRESS(entry))), index);
            index++;
            //return TRUE;
        }
        //Sets a single entry (element in the EntryArray) to its appopriate value
        //All give entries need to have the same KeyType and ValueType as thsi class
        //Only initialises the entry if the perform is true
        template<typename SpecilaisedEntryType>
        inline void InitialiseEntryIncrementIf(SpecilaisedEntryType entry, IndexType PASS_REF index, bool perform)
        {
            //If perform is true initialises the entry
            if(perform)
                this->SetEntry(POINTER_VALUE(reinterpret_cast<MapEntryType POINTER>(ADDRESS(entry))), index);
            index++;
            //return TRUE;
        }
        //Loops through the entire array and performs InitialiseEntry on every single one
        template<typename... SpecilaisedEntryTypes>
        inline void InitialiseAllEntries(SpecilaisedEntryTypes... entries)
        {
            IndexType iteratingIndex = 0;
            //Surpresses bugy warning
            (void) iteratingIndex;
            //Applies InitialiseENtry on all entries
            using dummy = int[];
            (void)dummy { 0, (InitialiseEntryIncrement(entries,iteratingIndex), 0)... };
            //( void(this->InitialiseEntry(entries,iteratingIndex)) , ...);
            //this->EmptyFunc(this->InitialiseEntry(entries, iteratingIndex)...);
        }
        //Loops through the entire array and performs InitialiseEntry on every single one
        template<typename... SpecilaisedEntryTypes>
        inline void InitialiseAllEntriesIfIndex(IndexType index, SpecilaisedEntryTypes... entries)
        {
            IndexType iteratingIndex = 0;
            auto t = { (InitialiseEntryIncrementIf(entries,iteratingIndex, IS_EQUAL(iteratingIndex, index)),0)... };
            (void) t;
            //( void(this->InitialiseEntry(entries,iteratingIndex)) , ...);
            //this->EmptyFunc(this->InitialiseEntry(entries, iteratingIndex)...);
        }


        //Sets a single entry (element in the EntryArray) to the providede value
        //All give entries need to have the same KeyType and ValueType as thsi class
        template<typename SpecilaisedEntryType>
        inline void SetEntryIncrement(SpecilaisedEntryType entry, IndexType PASS_REF index)
        {
            this->SetEntry(POINTER_VALUE(reinterpret_cast<MapEntryType POINTER>(ADDRESS(entry))), index);
            index++;
            //return TRUE;
        }
        //Loops through the entire array and performs SetEntry on every single one
        template<typename... SpecilaisedEntryTypes>
        inline void SetAllEntries(SpecilaisedEntryTypes... entries)
        {
            IndexType iteratingIndex = 0;
            auto t = { (SetEntryIncrement(entries,iteratingIndex),0)... };
            (void) t;
            //( void(this->InitialiseEntry(entries,iteratingIndex)) , ...);
            //this->EmptyFunc(this->InitialiseEntry(entries, iteratingIndex)...);
        }


        ///Main functions
    public:
        //Sets a single entry (element in the EntryArray) to its appopriate value
        //All give entries need to have the same KeyType and ValueType as thsi class
        template<typename SpecilaisedEntryType>
        inline void InitialiseEntry(IndexType index)
        {
            InitialiseAllEntriesIfIndex(index, EntryTypes()...);
        }
        //Sets all entries (elements in the EntryArray) to their appopriate values
        inline void InitialiseArray()
        {
            this->InitialiseAllEntries(EntryTypes()...);
        }

        template<typename... SpecilaisedEntryTypes>
        inline void SetArray(SpecilaisedEntryTypes... entries)
        {
            this->SetAllEntries(entries...);
        }

    public:
        //Returns the index of an entry with the specified key
        //If the key is not found returns an index of ThisType::GetEntryCount()
        IndexType FindKey(KeyType key)
        {
            //Loops through the entire array
            for(IndexType i = 0; i < ThisType::GetEntryCount(); i++)
                //If the key of the current entry is equal to the searched for key returns current index
                if(this->GetEntry(i).IsEqual(key))
                    return i;

            //If nothing is found retturns the size (is out of bounds and can be check for)
            return ThisType::GetEntryCount();
        }
        //Returns the the entry assosieted with the given key
        //If the key is not found returns a new MapEntryType
        MapEntryType GetEntryByKey(KeyType key)
        {
            //Gets the index
            IndexType keyIndex = ThisType::FindKey(key);

            //If the index is in bounds returns the value of the associeted entry
            if(ThisType::IsIndexInBounds(keyIndex))
            {
                return this->GetEntry(keyIndex);
            }

            //Else returns a new entry
            return MapEntryType();
        }
        //Returns the value of the entry assosieted with the given key
        inline ValueType GetValueByKey(KeyType key)
        {
            return this->GetEntryByKey(key).GetValue();
        }
};


template</*typename KeyType, typename ValueType,*/ typename ... EntryTypes>
class MapNoData
{

        ///Using
    private:
        using ThisType = MapNoData<EntryTypes...>;
    public:
        template<int N>
        using TypeOfEntry       = G::NthTypeOf<N, EntryTypes...>;

        using KeyType           = typename TypeOfEntry<0>::KeyType;
        using ValueType         = typename TypeOfEntry<0>::ValueType;
        using MapEntryFixedType = MapEntryFixed<KeyType, NULL, ValueType, NULL>;
        using MapEntryType      = MapEntry<KeyType, ValueType>;

        using SizeType          = size_t;
        using IndexType         = SizeType;


        ///Members - NONE


        ///Constructors and destructors
    public:
        MapNoData()   {}
        ~MapNoData()  {}


        ///Constexpr functions
    public:
        //Returns the count of Entries in this Map
        constexpr static SizeType GetEntryCount()
        {
            return sizeof... (EntryTypes);
        }
        //Returns the maximum index in this Map
        constexpr static SizeType GetMaxIndex()
        {
            //If the size is 0 returns zero (should never happen)
            if(ThisType::GetEntryCount() == 0)
                return 0;

            //Else returns the value of size - 1
            return ThisType::GetEntryCount() - 1;
        }


        ///Member functions
    public:
        //Gets the entry on the index provided by template argument
        template<IndexType index>
        static MapEntryType GetEntry()
        {
            G::NthTypeOf<index, EntryTypes...> entry;
            return POINTER_VALUE(reinterpret_cast<MapEntryFixedType POINTER>(ADDRESS(entry)));
        }
        //Gets the entry on the index provided by function argument
        static MapEntryType GetEntry(IndexType index)
        {
            return ThisType::GetEntryByIndexFromConstructed(index, EntryTypes()...);
        }
        //Saves all entries to the provided array
        static MapEntryType GetAllEntries(MapEntryType returnEntryArray[ThisType::GetEntryCount()])
        {
            return ThisType::GetAllEntriesFromConstructed(returnEntryArray, EntryTypes()...);
        }


        ///Main functions
    public:
        //Returns the index of an entry with the specified key
        //If the key is not found returns an index of ThisType::GetEntryCount()
        static IndexType FindKey(KeyType key)
        {
            return ThisType::FindEntryFromConstructed(key, EntryTypes()...);
        }
        //Returns the the entry assosieted with the given key
        //If the key is not found returns a new MapEntryType
        static bool GetEntryByKey(KeyType key, MapEntryType PASS_REF returnEntry)
        {
            return ThisType::GetEntryByKeyFromConstructed(key, returnEntry, EntryTypes()...);
        }
        //Returns the value of the entry assosieted with the given key
        inline static bool GetValueByKey(KeyType key, ValueType PASS_REF returnValue)
        {
            MapEntryType entry;
            bool status;

            status = ThisType::GetEntryByKey(key, entry);
            returnValue = entry.GetValue();

            return status;
        }


        ///Info functions
    public:
        //Returns true if the provided index is not greater then the maximum index
        constexpr static bool IsIndexInBounds(IndexType index)
        {
            //If index is at leats one smaller then the size returns true
            if(index < ThisType::GetEntryCount())
                return TRUE;
            return FALSE;
        }
        //Returns false if the provided index is not greater then the maximum index
        constexpr static bool IsIndexInNotBounds(IndexType index)
        {
            return NOT(ThisType::IsIndexInBounds(index));
        }


        ///Private helper functions
        ///Private getting an entry by index
    private:
        //Sets the [returnEntry] to [entry] if [targetIndex] is equal to [currentIndex]
        //Increments [currentIndex] by 1
        template<typename SpecilaisedEntryType>
        inline static void ReturnEntryIfIndexIsEqual(SpecilaisedEntryType entry, IndexType targetIndex, IndexType PASS_REF currentIndex, MapEntryType PASS_REF returnEntry)
        {
            //If perform is true initialises the entry
            if(IS_EQUAL(targetIndex, currentIndex))
                returnEntry = POINTER_VALUE(reinterpret_cast<MapEntryType POINTER>(ADDRESS(entry)));
            currentIndex++;
        }
        //Returns the entry whichs index is equal to the provided [index]
        //(Loops through the entire array and performs ReturnEntryIfIndexIsEqual on every single one)
        template<typename... SpecilaisedEntryTypes>
        inline static MapEntryType GetEntryByIndexFromConstructed(IndexType targetIndex, SpecilaisedEntryTypes PASS_RVALUE_REF... entries)
        {
            //Sets the apropriate values to the variables used to cycle throught the index and return value
            MapEntryType returnEntry;
            IndexType iteratingIndex = 0;

            //Applies ReturnIndexIfKeyIsEqual on every single entry from entries
            //(expands parameter pack)
            auto t = { (ThisType::ReturnEntryIfIndexIsEqual(entries, targetIndex, iteratingIndex, returnEntry),0)... };
            (void) t;

            //Returns the entry
            //If the index xis out of boudns returns a new entry
            return returnEntry;
        }


        ///Private geting all entries into a provided entry array
    private:
        //Sets the [returnEntry] to [entry]
        //Increments [index] by 1
        template<typename SpecilaisedEntryType>
        inline static void GetEntryIncrement(SpecilaisedEntryType entry, IndexType PASS_REF currentIndex, MapEntryType PASS_REF returnEntry)
        {
            returnEntry = POINTER_VALUE(reinterpret_cast<MapEntryType POINTER>(ADDRESS(entry)));
            currentIndex++;
        }
        //Sets the [returnEntryArray]'s elements equal to the values of [entries...]
        //Loops through the entire array and performs GetEntryIncrement on every single one
        //Output is set to the rovided [returnEntryArray]
        template<typename... SpecilaisedEntryTypes>
        inline static void GetAllEntriesFromConstructed(MapEntryType returnEntryArray[ThisType::GetEntryCount()], SpecilaisedEntryTypes PASS_RVALUE_REF ... entries)
        {
            //Sets the apropriate values to the variables used to cycle throught the index
            IndexType iteratingIndex = 0;

            //Applies ReturnIndexIfKeyIsEqual on every single entry from entries
            //(expands parameter pack)
            auto t = { (ThisType::GetEntryIncrement(entries,iteratingIndex, returnEntryArray[iteratingIndex]),0)... };
            (void) t;
        }


        ///Privtae getting index by Key
    private:
        //Sets the [returnEntry] to [entry] if [entry].IsEqual([key]) is true
        //Increments [index] by 1
        template<typename SpecilaisedEntryType>
        inline static void ReturnIndexIfKeyIsEqual(SpecilaisedEntryType entry, KeyType key, IndexType PASS_REF currentIndex, IndexType PASS_REF returnIndex)
        {
            if(entry.IsEqual(key))
                returnIndex = currentIndex;
            currentIndex++;
        }
        //Returns the index of an entry whichs key is equal to the provided [key]
        //If no such entry is found returns the ThisType::GetEntryCount (greater than the maximum index)
        //(Loops through the entire array and performs ReturnIndexIfKeyIsEqual on every single one)
        template<typename... SpecilaisedEntryTypes>
        inline static IndexType FindEntryFromConstructed(KeyType key, SpecilaisedEntryTypes PASS_RVALUE_REF ... entries)
        {
            //Sets the apropriate values to the variables used to cycle throught the index and return value
            IndexType iteratingIndex = 0;
            IndexType returnIndex = ThisType::GetEntryCount();

            //Applies ReturnIndexIfKeyIsEqual on every single entry from entries
            //(expands parameter pack)
            auto t = { (ThisType::ReturnIndexIfKeyIsEqual(entries, key, iteratingIndex, returnIndex),0)... };
            (void) t;

            //Returns the return index
            //If the index is out of bounds this function was unable to find the key
            return returnIndex;
        }


        ///Private getting entry by Key
    private:
        //Sets the [returnIndex] to [index] if [entry].IsEqual([key]) is true
        //Increments [index] by 1
        template<typename SpecilaisedEntryType>
        inline static void ReturnEntryIfKeyIsEqual(SpecilaisedEntryType entry, KeyType key, IndexType PASS_REF currentIndex, MapEntryType PASS_REF returnEntry, bool PASS_REF status)
        {
            if(entry.IsEqual(key))
            {
                returnEntry = POINTER_VALUE(reinterpret_cast<MapEntryType POINTER>(ADDRESS(entry)));
                status = TRUE;
            }
            currentIndex++;
        }
        //Returns the entry whichs key is equal to the provided [key]
        //If no such entry is found returns a value of new created MapEntryType
        //(Loops through the entire array and performs ReturnEntryIfKeyIsEqual on every single one)
        template<typename... SpecilaisedEntryTypes>
        inline static bool GetEntryByKeyFromConstructed(KeyType key, MapEntryType PASS_REF returnEntry, SpecilaisedEntryTypes PASS_RVALUE_REF... entries)
        {
            //Sets the apropriate values to the variables used to cycle throught the index and return value
            IndexType iteratingIndex = 0;
            bool success = false;

            //Applies ReturnIndexIfKeyIsEqual on every single entry from entries
            //(expands parameter pack)
            auto t = { (ThisType::ReturnEntryIfKeyIsEqual(entries, key, iteratingIndex, returnEntry, success),0)... };
            (void) t;

            //If success is false value wasnt found
            //Returns false
            if(success == false)
                return FALSE;

            //Else returns true
            return TRUE;
        }


};

template<typename KeyType, typename ValueType, typename ... EntryTypes>
using MapStaticLocal = MapStatic<MapStaticLocalCore<KeyType, ValueType, EntryTypes...>, KeyType, ValueType>;

#endif // MAP_H
