#pragma once
#include "../../memory/datatypes/qangle.h"
#include "../../memory/datatypes/vector.h"

#pragma region clientmode_definitions
#define SIGNONSTATE_NONE		0	// no state yet, about to connect
#define SIGNONSTATE_CHALLENGE	1	// client challenging server, all OOB packets
#define SIGNONSTATE_CONNECTED	2	// client is connected to server, netchans ready
#define SIGNONSTATE_NEW			3	// just got serverinfo and string tables
#define SIGNONSTATE_PRESPAWN	4	// received signon buffers
#define SIGNONSTATE_SPAWN		5	// ready to receive entity packets
#define SIGNONSTATE_FULL		6	// we are fully connected, first non-delta packet received (in-game check)
#define SIGNONSTATE_CHANGELEVEL	7	// server is changing level, please wait
#pragma endregion

class CViewSetup
{
public:
	int			m_iX;
	int			m_iUnscaledX;
	int			m_iY;
	int			m_iUnscaledY;
	int			m_iWidth;
	int			m_iUnscaledWidth;
	int			m_iHeight;
	int			m_iUnscaledHeight;
	bool		m_bOrtho;
	float		m_flOrthoLeft;
	float		m_flOrthoTop;
	float		m_flOrthoRight;
	float		m_flOrthoBottom;
	std::byte	pad0[ 0x7C ];
	float		m_flFOV;
	float		m_flViewModelFOV;
	Vector		m_vecOrigin;
	QAngle		m_angView;
	float		m_flNearZ;
	float		m_flFarZ;
	float		m_flNearViewmodelZ;
	float		m_flFarViewmodelZ;
	float		m_flAspectRatio;
	float		m_flNearBlurDepth;
	float		m_flNearFocusDepth;
	float		m_flFarFocusDepth;
	float		m_flFarBlurDepth;
	float		m_flNearBlurRadius;
	float		m_flFarBlurRadius;
	float		m_flDoFQuality;
	int			m_nMotionBlurMode;
	float		m_flShutterTime;
	Vector		m_vecShutterOpenPosition;
	QAngle		m_vecShutterOpenAngles;
	Vector		m_vecShutterClosePosition;
	QAngle		m_vecShutterCloseAngles;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;
	bool		m_bOffCenter : 1;
	bool		m_bRenderToSubrectOfLargerScreen : 1;
	bool		m_bDoBloomAndToneMapping : 1;
	bool		m_bDoDepthOfField : 1;
	bool		m_bHDRTarget : 1;
	bool		m_bDrawWorldNormal : 1;
	bool		m_bCullFontFaces : 1;
	bool		m_bCacheFullSceneState : 1;
	bool		m_bCSMView : 1;
};

class IHudChat;
class IClientModeShared
{
public:
	std::byte	pad0[ 0x1B ];
	void*		m_pViewport;
	IHudChat*	m_pChatElement;
	HCursor		m_hCursorNone;
	void*		m_pWeaponSelection;
	int			m_nRootSize[ 2 ];
};

class IAppSystem
{
private:
	virtual void function0( ) = 0;
	virtual void function1( ) = 0;
	virtual void function2( ) = 0;
	virtual void function3( ) = 0;
	virtual void function4( ) = 0;
	virtual void function5( ) = 0;
	virtual void function6( ) = 0;
	virtual void function7( ) = 0;
	virtual void function8( ) = 0;
};

class CEventInfo
{
public:
	enum
	{
		EVENT_INDEX_BITS = 8,
		EVENT_DATA_LEN_BITS = 11,
		MAX_EVENT_DATA = 192,
	};

	inline CEventInfo( )
	{
		m_iClassID = 0;
		m_flFireDelay = 0.0f;
		m_nFlags = 0;
		m_pSendTable = 0;
		m_pClientClass = 0;
		m_iPacked = 0;
	}

	short				m_iClassID;
	short				pad;
	float				m_flFireDelay;
	const void*			m_pSendTable;
	const CBaseClient*	m_pClientClass;
	int					m_iPacked;
	int					m_nFlags;
	int					m_aFilters[ 8 ];
	CEventInfo*			m_pNext;
};

class CClockDriftMgr
{
public:
	float		m_flClockOffsets[ 17 ];	//0x0000
	uint32_t	m_iCurClockOffset;	//0x0044
	uint32_t	m_nServerTick;		//0x0048
	uint32_t	m_nClientTick;		//0x004C
}; //Size: 0x0050

class INetChannel;
class IClientState
{
public:
	std::byte		pad0[ 0x9C ];				// 0x0000
	INetChannel*	m_pNetChannel;			// 0x009C
	int				m_iChallengeNr;			// 0x00A0
	std::byte		pad1[ 0x64 ];				// 0x00A4
	int				m_iSignonState;			// 0x0108
	std::byte		pad2[ 0x8 ];				// 0x010C
	float			m_flNextCmdTime;			// 0x0114
	int				m_nServerCount;			// 0x0118
	int				m_iCurrentSequence;		// 0x011C
	std::byte		pad3[ 0x4 ];				// 0x0120
	CClockDriftMgr  m_pClockDriftMgr;			//0x0128
	int				m_iDeltaTick;				// 0x0174
	bool			m_bPaused;				// 0x0178
	std::byte		pad4[ 0x7 ];				// 0x0179
	int				m_iViewEntity;			// 0x0180
	int				m_iPlayerSlot;			// 0x0184
	char			m_szLevelName[ 260 ];	// 0x0188
	char			m_szLevelNameShort[ 80 ];	// 0x028C
	char			m_szMapGroupName[ 80 ];		// 0x02DC
	char			m_szLastLevelNameShort[ 80 ]; // 0x032C
	std::byte		pad5[ 0xC ];				// 0x037C
	int				m_nMaxClients;			// 0x0388 
	std::byte		pad6[ 0x498C ];			// 0x038C
	float			m_flLastServerTickTime;	// 0x4D18
	bool			m_bInSimulation;			// 0x4D1C
	std::byte		pad7[ 0x3 ];				// 0x4D1D
	int				m_iOldTickcount;			// 0x4D20
	float			m_flTickRemainder;		// 0x4D24
	float			m_flFrameTime;			// 0x4D28
	int				m_iLastOutgoingCommand;	// 0x4D2C
	int				m_nChokedCommands;		// 0x4D30
	int				m_iLastCommandAck;		// 0x4D34
	int				m_iCommandAck;			// 0x4D38
	int				m_iSoundSequence;			// 0x4D3C
	std::byte		pad8[ 0x50 ];				// 0x4D40
	QAngle			m_angViewPoint;			// 0x4D90
	std::byte		pad9[ 0xD0 ];				// 0x4D9C
	CEventInfo*		m_pEvents;				// 0x4E6C
}; // Size: 0x4E70