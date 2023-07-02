#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/dt_common.h
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/dt_recv.h

enum ESendPropType : int
{
	DPT_INT = 0,
	DPT_FLOAT,
	DPT_VECTOR,
	DPT_VECTOR2D,
	DPT_STRING,
	DPT_ARRAY,
	DPT_DATATABLE,
	DPT_INT64,
	DPT_SENDPROPTYPEMAX
};

struct RecvProp_t;
struct DataVariant_t
{
	union
	{
		float	m_Float;
		long	m_Int;
		char*	m_String;
		void*	m_Data;
		float	m_Vector[ 3 ];
		int64_t	m_Int64;
	};

	ESendPropType m_iType;
};

class CRecvProxyData
{
public:
	const RecvProp_t*	m_pRecvProp;	// the property it's receiving
	DataVariant_t		m_Value;		// the value given to you to store
	int					m_iElement;	// which array element you're getting
	int					m_nObjectID;	// the object being referred to
};

using RecvVarProxyFn = void( __cdecl* )( const CRecvProxyData*, void*, void* );
using ArrayLengthProxyFn = void( __cdecl* )( void*, int, int );
using DataTableProxyFn = void( __cdecl* )( const RecvProp_t*, void**, void*, int );

class CStandartRecvProxies
{
public:
	RecvVarProxyFn m_pInt32ToInt8;
	RecvVarProxyFn m_pInt32ToInt16;
	RecvVarProxyFn m_pInt32ToInt32;
	RecvVarProxyFn m_pInt64ToInt64;
	RecvVarProxyFn m_pFloatToFloat;
	RecvVarProxyFn m_pVectorToVector;
};

// receive data table
struct RecvTable_t
{
	RecvProp_t*		m_pProps;
	int				m_nProps;
	void*			m_pDecoder;
	char*			m_szNetTableName;
	bool			m_bInitialized;
	bool			m_bInMainList;
};

// receive prop comes from the receive data table
struct RecvProp_t
{
	char*				m_szVarName;
	ESendPropType		m_iRecvType;
	int					m_iFlags;
	int					m_nStringBufferSize;
	bool				m_bInsideArray;
	const void*			m_pExtraData;
	RecvProp_t*			m_pArrayProp;
	ArrayLengthProxyFn* m_oArrayLengthProxyFn;
	RecvVarProxyFn		m_oProxyFn;
	DataTableProxyFn*	m_oDataTableProxyFn;
	RecvTable_t*		m_pDataTable;
	int					m_iOffset;
	int					m_iElementStride;
	int					m_iElements;
	const char*			m_szParentArrayPropName;
};
