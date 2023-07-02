#pragma once
#include "../../memory/memory.h"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h

class IClientNetworkable;
using CreateClientClassFn = IClientNetworkable * ( __cdecl* )( int, int );
using CreateEventFn = IClientNetworkable * ( __cdecl* )( );

class CStandartRecvProxies;
struct RecvTable_t;
enum class EClassIndex;

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/client_class.h
class CBaseClient
{
public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn			m_pCreateEventFn;
	char*					m_szNetworkName;
	RecvTable_t*			m_pRecvTable;
	CBaseClient*			m_pNext;
	EClassIndex				m_nClassID;
};

using CreateInterfaceFn = void* ( __cdecl* )( const char*, int* );
using InstantiateInterfaceFn = void* ( __cdecl* )( );
using DemoCustomDataCallbackFn = void( __cdecl* )( unsigned char*, std::size_t );

class IBaseClientDll
{
public:
	CBaseClient* GetAllClasses( )
	{
		return Memory::CallVFunc<CBaseClient*>( this, 8 );
	}

	bool DispatchUserMessage( int iMessageType, unsigned int nFlags, int iSize, const void* pMessageData )
	{
		return Memory::CallVFunc<bool>( this, 38, iMessageType, nFlags, iSize, pMessageData );
	}

	CStandartRecvProxies* GetStandardRecvProxies( )
	{
		return Memory::CallVFunc<CStandartRecvProxies*>( this, 48 );
	}
};