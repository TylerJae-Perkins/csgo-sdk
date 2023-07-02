#pragma once
// used: bf_read, bf_write for buffer operations
#include "../../valve/bitbuf.h"
#include "../../memory/memory.h"

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/inetchannelinfo.h
class INetChannelInfo
{
public:
	enum
	{
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		TEMPENTS,		// temp entities
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		PAINTMAP,		// paintmap data
		ENCRYPTED,		// encrypted data
		TOTAL			// must be last and is not a real group
	};

	virtual const char* GetName( ) const = 0;			// get channel name
	virtual const char* GetAddress( ) const = 0;			// get channel IP address as string
	virtual float		GetTime( ) const = 0;			// current net time
	virtual float		GetTimeConnected( ) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize( ) const = 0;		// netchannel packet history size
	virtual int			GetDataRate( ) const = 0;		// send data rate in byte/sec

	virtual bool		IsLoopback( ) const = 0;			// true if loopback channel
	virtual bool		IsTimingOut( ) const = 0;		// true if timing out
	virtual bool		IsPlayback( ) const = 0;			// true if demo playback
	virtual float		GetLatency( int iFlow ) const = 0; // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency( int iFlow ) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss( int iFlow ) const = 0; // avg packet loss[0..1]
	virtual float		GetAvgChoke( int iFlow ) const = 0; // avg packet choke[0..1]
	virtual float		GetAvgData( int iFlow ) const = 0; // data flow in bytes/sec
	virtual float		GetAvgPackets( int iFlow ) const = 0; // avg packets/sec
	virtual int			GetTotalData( int iFlow ) const = 0; // total flow in/out in bytes
	virtual int			GetTotalPackets( int iFlow ) const = 0;
	virtual int			GetSequenceNr( int iFlow ) const = 0; // last send seq number
	virtual bool		IsValidPacket( int iFlow, int nFrame ) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime( int iFlow, int nFrame ) const = 0; // time when packet was send
	virtual int			GetPacketBytes( int iFlow, int nFrame, int group ) const = 0; // group size of this packet
	virtual bool		GetStreamProgress( int iFlow, int* piReceived, int* piTotal ) const = 0; // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived( ) const = 0; // get time since last received packet in seconds
	virtual	float		GetCommandInterpolationAmount( int iFlow, int nFrame ) const = 0;
	virtual void		GetPacketResponseLatency( int iFlow, int frame_number, int* pnLatencyMsecs, int* pnChoke ) const = 0;
	virtual void		GetRemoteFramerate( float* pflFrameTime, float* pflFrameTimeStdDeviation, float* pflFrameStartTimeStdDeviation ) const = 0;
	virtual float		GetTimeoutSeconds( ) const = 0;
};

struct CSVCMsg_VoiceData
{
	char		pad_0000[ 8 ]; //0x0000
	int32_t		m_nClient; //0x0008
	int32_t		m_iAudibleMask; //0x000C
	uint32_t	m_uiXuidLow{};
	uint32_t	m_uiXuidHigh{};
	void*		m_pVoideData; //0x0018
	int32_t		m_iProximity; //0x001C
	//int32_t caster; //0x0020
	int32_t		m_iFormat; //0x0020
	int32_t		m_iSequenceBytes; //0x0024
	uint32_t	m_uiSectionNumber; //0x0028
	uint32_t	m_uiUncompressedSampleOffset; //0x002C
};

struct CCLCMsg_VoiceData
{
	uint32_t	INetMessage_Vtable; //0x0000
	char		pad_0004[ 4 ]; //0x0004
	uint32_t	CCLCMsg_VoiceData_Vtable; //0x0008
	char		pad_000C[ 8 ]; //0x000C
	void*		m_pData; //0x0014
	uint32_t	m_uiXuidLow{};
	uint32_t	m_uiXuidHigh{};
	int32_t		m_iFormat; //0x0020
	int32_t		m_iSequenceBytes; //0x0024
	uint32_t	m_uiSectionNumber; //0x0028
	uint32_t	m_uiUncompressedSampleOffset; //0x002C
	int32_t		m_iCachedSize; //0x0030

	uint32_t	m_uiFlags; //0x0034

	uint8_t		m_uiNoStackOverflow[ 0xFF ];
};

class INetChannel
{
public:
	std::byte	pad0[ 0x14 ];				//0x0000
	bool		m_bProcessingMessages;	//0x0014
	bool		m_bShouldDelete;			//0x0015
	bool		m_bStopProcessing;		//0x0016
	std::byte	pad1[ 0x1 ];				//0x0017
	int			m_iOutSequenceNr;			//0x0018 last send outgoing sequence number
	int			m_iInSequenceNr;			//0x001C last received incoming sequence number
	int			m_iOutSequenceNrAck;		//0x0020 last received acknowledge outgoing sequence number
	int			m_iOutReliableState;		//0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int			m_iInReliableState;		//0x0028 state of incoming reliable data
	int			m_iChokedPackets;			//0x002C number of choked packets
	std::byte	pad2[ 0x414 ];			//0x0030

	bool SendNetMessage( void* pMessage, bool bOnlyReliable = false, bool bVoice = false )
	{
		return Memory::CallVFunc<bool>( this, 40, pMessage, bOnlyReliable, bVoice );
	}

	int	SendDatagram( bf_write* pDatagram )
	{
		return Memory::CallVFunc<int>( this, 46, pDatagram );
	}

	bool Transmit( bool bOnlyReliable = false )
	{
		return Memory::CallVFunc<bool>( this, 49, bOnlyReliable );
	}
}; // Size: 0x0444

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/inetmessage.h
class INetMessage
{
public:
	virtual					~INetMessage( ) { }
	virtual void			SetNetChannel( void* pNetChannel ) = 0;
	virtual void			SetReliable( bool bState ) = 0;
	virtual bool			Process( ) = 0;
	virtual	bool			ReadFromBuffer( bf_read& buffer ) = 0;
	virtual	bool			WriteToBuffer( bf_write& buffer ) = 0;
	virtual bool			IsReliable( ) const = 0;
	virtual int				GetType( ) const = 0;
	virtual int				GetGroup( ) const = 0;
	virtual const char*		GetName( ) const = 0;
	virtual INetChannel*	GetNetChannel( ) const = 0;
	virtual const char*		ToString( ) const = 0;
	virtual std::size_t		GetSize( ) const = 0;
};

class CCLCMsg_Move
{
private:
	std::byte pad0[ 0x8 ];
public:
	int				m_nBackupCommands;
	int				m_nNewCommands;
	std::string*	m_szData;   // 0x10 std::string
	void*			m_DataIn;      // 0x14
	bf_write		m_DataOut;    // 0x38
};

template <typename T>
class CNetMessagePB : public INetMessage, public T { };
using CCLCMsg_Move_t = CNetMessagePB<CCLCMsg_Move>;

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/inetmsghandler.h
class INetChannelHandler
{
public:
	virtual			~INetChannelHandler( ) { }
	virtual void	ConnectionStart( INetChannel* pChannel ) = 0; // called first time network channel is established
	virtual void	ConnectionStop( ) = 0; // called first time network channel is established
	virtual void	ConnectionClosing( const char* szReason ) = 0; // network channel is being closed by remote site
	virtual void	ConnectionCrashed( const char* szReason ) = 0; // network error occured
	virtual void	PacketStart( int iIncomingSequence, int iOutgoingAcknowledged ) = 0;	// called each time a new packet arrived
	virtual void	PacketEnd( ) = 0; // all messages has been parsed
	virtual void	FileRequested( const char* szFileName, unsigned int uTransferID, bool bReplayDemoFile ) = 0; // other side request a file for download
	virtual void	FileReceived( const char* szFileName, unsigned int uTransferID, bool bReplayDemoFile ) = 0; // we received a file
	virtual void	FileDenied( const char* szFileName, unsigned int uTransferID, bool bReplayDemoFile ) = 0; // a file request was denied by other side
	virtual void	FileSent( const char* szFileName, unsigned int uTransferID, bool bReplayDemoFile ) = 0; // we sent a file
	virtual bool	ChangeSplitscreenUser( int nSplitScreenUserSlot ) = 0; // interleaved networking used by SS system is changing the SS player slot that the subsequent messages pertain to
};