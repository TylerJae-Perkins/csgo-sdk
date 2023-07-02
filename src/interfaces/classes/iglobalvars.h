#pragma once
#include "../../definitions.h"

class IGlobalVarsBase
{
public:
	float			m_flRealTime;					//0x00
	int				m_iFrameCount;				//0x04
	float			m_flAbsFrameTime;				//0x08
	float			m_flAbsFrameStartTime;		//0x0C
	float			m_flCurrentTime;				//0x10
	float			m_flFrameTime;				//0x14
	int				m_nMaxClients;				//0x18
	int				m_iTickCount;					//0x1C
	float			m_flIntervalPerTick;			//0x20
	float			m_flInterpolationAmount;		//0x24
	int				m_nFrameSimulationTicks;		//0x28
	int				m_iNetworkProtocol;			//0x2C
	void*			m_pSaveData;					//0x30
	bool			m_bClient;					//0x34
	bool		    m_bRemoteClient;				//0x35
	int				m_iTimestampNetworkingBase;	//0x36
	int				m_iTimestampRandomizeWindow;	//0x3A
}; // Size: 0x3E

class IGlobalVars : public IGlobalVarsBase
{
public:
	string_t		m_szMapName;					//0x3E
	string_t		m_szMapGroupName;				//0x42
	int				m_iMapVersion;				//0x46
	string_t		m_szStartSpot;				//0x4A
	int				m_nLoadType;					//0x4E
	bool			m_bMapLoadFailed;				//0x52
	bool			m_bDeathmatch;				//0x53
	bool			m_bCooperative;				//0x54
	bool			m_bTeamplay;					//0x55
	int				m_nMaxEntities;				//0x56
	int				m_nServerCount;				//0x5A
	void*			m_pEdicts;					//0x5E
}; // Size: 0x62
