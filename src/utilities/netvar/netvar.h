#pragma once
#include <fstream>
#include <unordered_map>

#include "../../common.h"
#include "../../memory/hash/fnv1a.h"
#include "../../memory/datatypes/datatable.h"
#include "../../memory/datatypes/datamap.h"

#pragma region netvar_definitions
/* add function to get variable with additional offset from netvar offset */
#define N_ADD_VARIABLE_OFFSET( Type, szFunctionName, szNetVar, uAdditional )								\
	[[nodiscard]] std::add_lvalue_reference_t<Type> szFunctionName()										\
	{																										\
		static constexpr FNV1A_t uHash = FNV1A::HashConst(szNetVar);										\
		static std::uintptr_t uOffset = CNetvarManager::Get().m_mapProps[uHash].m_uOffset;					\
		return *(std::add_pointer_t<Type>)(reinterpret_cast<std::uintptr_t>(this) + uOffset + uAdditional);	\
	}

/* add function to get netvar variable */
#define N_ADD_VARIABLE( Type, szFunctionName, szNetVar ) N_ADD_VARIABLE_OFFSET( Type, szFunctionName, szNetVar, 0U )

/* add function to get variable pointer with additional offset from netvar offset */
#define N_ADD_PVARIABLE_OFFSET( Type, szFunctionName, szNetVar, uAdditional )								\
	[[nodiscard]] std::add_pointer_t<Type> szFunctionName()													\
	{																										\
		static constexpr FNV1A_t uHash = FNV1A::HashConst(szNetVar);										\
		static std::uintptr_t uOffset = CNetvarManager::Get().m_mapProps[uHash].m_uOffset;					\
		return (std::add_pointer_t<Type>)(reinterpret_cast<std::uintptr_t>(this) + uOffset + uAdditional);	\
	}

/* add function to get netvar variable pointer */
#define N_ADD_PVARIABLE( Type, szFunctionName, szNetVar ) N_ADD_PVARIABLE_OFFSET( Type, szFunctionName, szNetVar, 0U )

/* add function to get csplayerresource variable for entity by index from netvar offset */
#define N_ADD_RESOURCE_VARIABLE( Type, szFunctionName, szNetVar )														\
	[[nodiscard]] std::add_lvalue_reference_t<Type> szFunctionName(int nIndex)											\
	{																													\
		static constexpr FNV1A_t uHash = FNV1A::HashConst(szNetVar);													\
		static std::uintptr_t uOffset = CNetvarManager::Get().m_mapProps[uHash].m_uOffset;								\
		return *(std::add_pointer_t<Type>)(reinterpret_cast<std::uintptr_t>(this) + uOffset + nIndex * sizeof(Type));	\
	}

/* add function to get datamap variable */
#define N_ADD_DATAFIELD( Type, szFunctionName, pMap, szDataField )											\
	[[nodiscard]] std::add_lvalue_reference_t<Type> szFunctionName()										\
	{																										\
		static constexpr FNV1A_t uHash = FNV1A::HashConst(szDataField);										\
		static std::uintptr_t uOffset = CNetvarManager::Get().FindInDataMap(pMap, uHash);					\
		return *(std::add_pointer_t<Type>)(reinterpret_cast<std::uintptr_t>(this) + uOffset);				\
	}

/* add function to get datamap variable pointer */
#define N_ADD_PDATAFIELD( Type, szFunctionName, pMap, szDataField )											\
	[[nodiscard]] std::add_pointer_t<Type> szFunctionName()													\
	{																										\
		static constexpr FNV1A_t uHash = FNV1A::HashConst(szDataField);										\
		static std::uintptr_t uOffset = CNetvarManager::Get().FindInDataMap(pMap, uHash);					\
		return (std::add_pointer_t<Type>)(reinterpret_cast<std::uintptr_t>(this) + uOffset);				\
	}

/* add function to get variable by offset */
#define N_ADD_OFFSET( Type, szFunctionName, uOffset )														\
	[[nodiscard]] std::add_lvalue_reference_t<Type> szFunctionName()										\
	{																										\
		return *(std::add_pointer_t<Type>)(reinterpret_cast<std::uintptr_t>(this) + uOffset);				\
	}

/* add function to get variable pointer by offset */
#define N_ADD_POFFSET( Type, szFunctionName, uOffset )														\
	[[nodiscard]] std::add_pointer_t<Type> szFunctionName()													\
	{																										\
		return (std::add_pointer_t<Type>)(reinterpret_cast<std::uintptr_t>(this) + uOffset);				\
	}

/* add function to get offset by index  */
#define N_ADD_RESOURCE_OFFSET( Type, szFunctionName, uOffset )															\
	[[nodiscard]] std::add_lvalue_reference_t<Type> szFunctionName(int nIndex)											\
	{																													\
		return *(std::add_pointer_t<Type>)(reinterpret_cast<std::uintptr_t>(this) + uOffset + nIndex * sizeof(Type));	\
	}

#pragma endregion

class CRecvPropHook
{
public:
    CRecvPropHook( RecvProp_t* pRecvProp, const RecvVarProxyFn pNewProxyFn ) :
        m_pRecvProp( pRecvProp ), m_pOriginalFn( pRecvProp->m_oProxyFn )
    {
        SetProxy( pNewProxyFn );
    }

    // Get
    /* replace with our function */
    void Replace( RecvProp_t* pRecvProp )
    {
        this->m_pRecvProp = pRecvProp;
        this->m_pOriginalFn = pRecvProp->m_oProxyFn;
    }

    /* restore original function */
    void Restore( ) const
    {
        if ( this->m_pOriginalFn != nullptr )
            this->m_pRecvProp->m_oProxyFn = this->m_pOriginalFn;
    }

    void SetProxy( const RecvVarProxyFn pNewProxyFn ) const
    {
        this->m_pRecvProp->m_oProxyFn = pNewProxyFn;
    }

    RecvVarProxyFn GetOriginal( ) const
    {
        return this->m_pOriginalFn;
    }

private:
    // Values
    /* in future that is being modified and replace the original prop */
    RecvProp_t*     m_pRecvProp = nullptr;
    /* original proxy function to make able to restore it later */
    RecvVarProxyFn  m_pOriginalFn = nullptr;
};

class CNetvarManager : public CSingleton<CNetvarManager>
{
public:
    struct NetvarObject_t
    {
        RecvProp_t*     m_pRecvProp = nullptr;
        std::uintptr_t  m_uOffset = 0U;
    };

    // Get
    /* fill map with netvars and also dump it to given file */
    bool Setup( const std::string_view szDumpFileName );
    /*
     * stores the variables of objects in the hierarchy
     * used to iterate through an object's data descriptions from data map
     */
    std::uintptr_t FindInDataMap( DataMap_t* pMap, const FNV1A_t uFieldHash );

    // Values
    /* logging counters */
    int m_iStoredProps = 0;
    int m_iStoredTables = 0;
    /* networkable properties map */
    std::unordered_map<FNV1A_t, NetvarObject_t> m_mapProps = {};

private:
    /*
     * recursively stores networked properties info from data tables in our map
     * and also format our dump and write values to file
     */
    void StoreProps( const char* szClassName, RecvTable_t* pRecvTable, const std::uintptr_t uOffset, const int iDepth );

    // Extra
    std::string GetPropertyType( const RecvProp_t* pRecvProp ) const;

    // Values
    /* output file */
    std::ofstream fsDumpFile = {};
};