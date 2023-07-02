#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h

#include "../../memory/datatypes/matrix.h"
#include "../../memory/datatypes/qangle.h"
#include "../../memory/hash/crc32.h"

#pragma region engineclient_enumerations
enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	// a network packet is being recieved
	FRAME_NET_UPDATE_START,
	// data has been received and we are going to start calling postdataupdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// data has been received and called postdataupdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// received all packets, we can now do interpolation, prediction, etc
	FRAME_NET_UPDATE_END,
	// start rendering the scene
	FRAME_RENDER_START,
	// finished rendering the scene
	FRAME_RENDER_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

enum ERenderViewInfo : int
{
	RENDERVIEW_UNSPECIFIED =				0,
	RENDERVIEW_DRAWVIEWMODEL =				(1 << 0),
	RENDERVIEW_DRAWHUD =					(1 << 1),
	RENDERVIEW_SUPPRESSMONITORRENDERING =	(1 << 2)
};
#pragma endregion

struct PlayerInfo_t
{
	std::uint64_t	ullVersion = 0ULL;
	union
	{
		std::uint64_t m_ullXuid;
		struct
		{
			std::uint32_t m_nXuidLow;
			std::uint32_t m_nXuidHigh;
		};
	};

	char			m_szName[128];
	int				m_nUserID;
	char			m_szSteamID[33];
	std::uint32_t	m_nFriendsID;
	char			m_szFriendsName[128];
	bool			m_bFakePlayer;
	bool			m_bIsHLTV;
	CRC32_t			m_uCustomFiles[4];
	std::uint8_t	m_dFilesDownloaded;
};

struct AudioState_t
{
	Vector			vecOrigin;
	QAngle			angAngles;
	bool			bIsUnderwater;
};

struct Model_t;
struct SteamAPIContext_t;
struct ClientTextMessage_t;
struct SurfInfo_t;
class CSentence;
class CAudioSource;
class CPhysCollide;
class CGamestatsData;
class ICollideable;
class ISpatialQuery;
class ISPSharedMemory;
class IAchievementMgr;
class IMaterial;
class IMaterialSystem;

class IEngineClient
{
public:
	void GetScreenSize(int& iWidth, int& iHeight)
	{
		Memory::CallVFunc<void>(this, 5, std::ref(iWidth), std::ref(iHeight));
	}

	bool GetPlayerInfo(int nEntityIndex, PlayerInfo_t* pInfo)
	{
		return Memory::CallVFunc<bool>(this, 8, nEntityIndex, pInfo);
	}

	int GetPlayerForUserID(int nUserID)
	{
		return Memory::CallVFunc<int>(this, 9, nUserID);
	}

	bool IsConsoleVisible()
	{
		return Memory::CallVFunc<bool>(this, 11);
	}

	int GetLocalPlayer()
	{
		return Memory::CallVFunc<int>(this, 12);
	}

	float GetLastTimeStamp()
	{
		return Memory::CallVFunc<float>(this, 14);
	}

	void GetViewAngles(QAngle& angView)
	{
		Memory::CallVFunc<void>(this, 18, std::ref(angView));
	}

	void SetViewAngles(QAngle& angView)
	{
		Memory::CallVFunc<void>(this, 19, std::ref(angView));
	}

	int GetMaxClients()
	{
		return Memory::CallVFunc<int>(this, 20);
	}

	// returns true if the player is fully connected and active in game (i.e, not still loading) and for check doesnt need isconnected
	bool IsInGame()
	{
		return Memory::CallVFunc<bool>(this, 26);
	}

	// returns true if the player is connected, but not necessarily active in game (could still be loading)
	bool IsConnected()
	{
		return Memory::CallVFunc<bool>(this, 27);
	}

	bool IsDrawingLoadingImage()
	{
		return Memory::CallVFunc<bool>(this, 28);
	}

	const ViewMatrix_t& WorldToScreenMatrix()
	{
		return Memory::CallVFunc<const ViewMatrix_t&>(this, 37);
	}

	void* GetBSPTreeQuery()
	{
		return Memory::CallVFunc<void*>(this, 43);
	}

	const char* GetLevelName()
	{
		return Memory::CallVFunc<const char*>(this, 52);
	}

	const char* GetLevelNameShort()
	{
		return Memory::CallVFunc<const char*>(this, 53);
	}

	INetChannelInfo* GetNetChannelInfo()
	{
		return Memory::CallVFunc<INetChannelInfo*>(this, 78);
	}

	bool IsPlayingDemo()
	{
		return Memory::CallVFunc<bool>(this, 82);
	}

	bool IsRecordingDemo()
	{
		return Memory::CallVFunc<bool>(this, 83);
	}

	bool IsPlayingTimeDemo()
	{
		return Memory::CallVFunc<bool>(this, 84);
	}

	bool IsTakingScreenshot()
	{
		return Memory::CallVFunc<bool>(this, 92);
	}

	bool IsHLTV()
	{
		return Memory::CallVFunc<bool>(this, 93);
	}

	unsigned int GetEngineBuildNumber()
	{
		return Memory::CallVFunc<unsigned int>(this, 104);
	}

	const char* GetProductVersionString()
	{
		return Memory::CallVFunc<const char*>(this, 105);
	}

	void ExecuteClientCmd(const char* szCmdString)
	{
		Memory::CallVFunc<void>(this, 108, szCmdString);
	}

	void ClientCmdUnrestricted(const char* szCmdString, bool bFromConsoleOrKeybind = false)
	{
		Memory::CallVFunc<void>(this, 114, szCmdString, bFromConsoleOrKeybind);
	}

	SteamAPIContext_t* GetSteamAPIContext()
	{
		return Memory::CallVFunc<SteamAPIContext_t*>(this, 185);
	}

	bool IsVoiceRecording()
	{
		return Memory::CallVFunc<bool>(this, 224);
	}
};
