#pragma once
#include "../memory/datatypes/vector.h"
#include "../memory/datatypes/qangle.h"
#include "../memory/datatypes/matrix.h"
#include "../memory/memory.h"
#include "../definitions.h"
#include "../common.h"

#define CSGO_ANIM_LOWER_CATCHUP_IDLE	100.0f
#define CSGO_ANIM_AIM_NARROW_WALK	0.8f
#define CSGO_ANIM_AIM_NARROW_RUN	0.5f
#define CSGO_ANIM_AIM_NARROW_CROUCHMOVING	0.5f
#define CSGO_ANIM_LOWER_CATCHUP_WITHIN	3.0f
#define CSGO_ANIM_LOWER_REALIGN_DELAY	1.1f
#define CSGO_ANIM_READJUST_THRESHOLD	120.0f
#define EIGHT_WAY_WIDTH 22.5f

#define FIRSTPERSON_TO_THIRDPERSON_VERTICAL_TOLERANCE_MIN 4.0f
#define FIRSTPERSON_TO_THIRDPERSON_VERTICAL_TOLERANCE_MAX 10.0f

#define CSGO_ANIM_WALK_TO_RUN_TRANSITION_SPEED 2.0f
#define CSGO_ANIM_ONGROUND_FUZZY_APPROACH 8.0f
#define CSGO_ANIM_ONGROUND_FUZZY_APPROACH_CROUCH 16.0f
#define CSGO_ANIM_LADDER_CLIMB_COVERAGE 100.0f
#define CSGO_ANIM_RUN_ANIM_PLAYBACK_MULTIPLIER 0.85f
#define MAX_ANIMSTATE_ANIMNAME_CHARS 64

#define ANIM_TRANSITION_WALK_TO_RUN 0
#define ANIM_TRANSITION_RUN_TO_WALK 1

#define CS_PLAYER_SPEED_DUCK_MODIFIER 0.34f
#define CS_PLAYER_SPEED_WALK_MODIFIER 0.52f
#define CS_PLAYER_SPEED_CLIMB_MODIFIER 0.34f
#define CS_PLAYER_HEAVYARMOR_FLINCH_MODIFIER 0.5f
#define CS_PLAYER_SPEED_RUN 260.0f

#define MOVESTATE_IDLE	0
#define MOVESTATE_WALK	1
#define MOVESTATE_RUN	2

/* max animation layers */
#define MAXOVERLAYS 15

enum ESequenceActivity : int
{
	ACT_CSGO_NULL = 957,
	ACT_CSGO_DEFUSE,
	ACT_CSGO_DEFUSE_WITH_KIT,
	ACT_CSGO_FLASHBANG_REACTION,
	ACT_CSGO_FIRE_PRIMARY,
	ACT_CSGO_FIRE_PRIMARY_OPT_1,
	ACT_CSGO_FIRE_PRIMARY_OPT_2,
	ACT_CSGO_FIRE_SECONDARY,
	ACT_CSGO_FIRE_SECONDARY_OPT_1,
	ACT_CSGO_FIRE_SECONDARY_OPT_2,
	ACT_CSGO_RELOAD,
	ACT_CSGO_RELOAD_START,
	ACT_CSGO_RELOAD_LOOP,
	ACT_CSGO_RELOAD_END,
	ACT_CSGO_OPERATE,
	ACT_CSGO_DEPLOY,
	ACT_CSGO_CATCH,
	ACT_CSGO_SILENCER_DETACH,
	ACT_CSGO_SILENCER_ATTACH,
	ACT_CSGO_TWITCH,
	ACT_CSGO_TWITCH_BUYZONE,
	ACT_CSGO_PLANT_BOMB,
	ACT_CSGO_IDLE_TURN_BALANCEADJUST,
	ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING,
	ACT_CSGO_ALIVE_LOOP,
	ACT_CSGO_FLINCH,
	ACT_CSGO_FLINCH_HEAD,
	ACT_CSGO_FLINCH_MOLOTOV,
	ACT_CSGO_JUMP,
	ACT_CSGO_FALL,
	ACT_CSGO_CLIMB_LADDER,
	ACT_CSGO_LAND_LIGHT,
	ACT_CSGO_LAND_HEAVY,
	ACT_CSGO_EXIT_LADDER_TOP,
	ACT_CSGO_EXIT_LADDER_BOTTOM
};

class CBoneAccessor
{
public:
	matrix3x4_t* GetBoneArrayForWrite( )
	{
		return m_matBones;
	}

	void SetBoneArrayForWrite( matrix3x4_t* bone_array )
	{
		m_matBones = bone_array;
	}

	int GetReadableBones( )
	{
		return m_nReadableBones;
	}

	void SetReadableBones( int flags )
	{
		m_nReadableBones = flags;
	}

	int GetWritableBones( )
	{
		return m_nWritableBones;
	}

	void SetWritableBones( int flags )
	{
		m_nWritableBones = flags;
	}

	matrix3x4_t*	m_matBones;
	int				m_nReadableBones;
	int				m_nWritableBones;
};

class CAnimationLayer
{
public:
	float			m_flAnimationTime;		//0x00
	float			m_flFadeOut;				//0x04
	void*			m_pStudioHdr;				//0x08
	int				m_nDispatchedSrc;			//0x0C
	int				m_nDispatchedDst;			//0x10
	int				m_iOrder;					//0x14
	std::uintptr_t  m_nSequence;				//0x18
	float			m_flPrevCycle;			//0x1C
	float			m_flWeight;				//0x20
	float			m_flWeightDeltaRate;		//0x24
	float			m_flPlaybackRate;			//0x28
	float			m_flCycle;				//0x2C
	void*			m_pOwner;					//0x30
	int				m_nInvalidatePhysicsBits;	//0x34
}; // Size: 0x38

#pragma pack(push, 1)
struct CAimLayer
{
	float flUnknown0;
	float m_flTotalTime;
	float flUnknown1;
	float flUnknown2;
	float m_flWeight;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CAimLayers
{
	CAimLayer m_layers[ 3 ];
};
#pragma pack(pop)

struct AnimstatePose
{
	bool		m_bInitialized;
	int			m_nIndex;
	const char* m_szName;

	AnimstatePose( )
	{
		m_bInitialized = false;
		m_nIndex = -1;
		m_szName = "";
	}
};

struct ProceduralFoot
{
	Vector	m_vecPosAnim;
	Vector	m_vecPosAnimLast;
	Vector	m_vecPosPlant;
	Vector	m_vecPlantVel;
	float	m_flLockAmount;
	float	m_flLastPlantTime;
};

class CBaseEntity;
class CBaseCombatWeapon;
class CCSGOPlayerAnimState
{
public:
	void CreateAnimationState( CBaseEntity* pEntity )
	{
		using CreateAnimationStateFn = void( __thiscall* )( void*, CBaseEntity* );
		static auto oCreateAnimationState = reinterpret_cast< CreateAnimationStateFn >( Memory::FindPattern( CLIENT_DLL, XorStr( "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" ) ) ); // @xref: "ggprogressive_player_levelup"

		if ( oCreateAnimationState == nullptr )
			return;

		oCreateAnimationState( this, pEntity );
	}

	void UpdateAnimationState( QAngle angView )
	{
		using UpdateAnimationStateFn = void( __vectorcall* )( void*, void*, float, float, float, void* );
		static auto oUpdateAnimationState = reinterpret_cast< UpdateAnimationStateFn >( Memory::FindPattern( CLIENT_DLL, XorStr( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3" ) ) ); // @xref: "%s_aim"

		if ( oUpdateAnimationState == nullptr )
			return;

		oUpdateAnimationState( this, nullptr, 0.0f, angView.y, angView.x, nullptr );
	}

	void ResetAnimationState( )
	{
		using ResetAnimationStateFn = void( __thiscall* )( void* );
		static auto oResetAnimationState = reinterpret_cast< ResetAnimationStateFn >( Memory::FindPattern( CLIENT_DLL, XorStr( "56 6A 01 68 ? ? ? ? 8B F1" ) ) ); // @xref: "player_spawn"

		if ( oResetAnimationState == nullptr )
			return;

		oResetAnimationState( this );
	}

	void*					pThis;
	bool					m_bIsReset;
	bool					bUnknownClientBoolean;
	char					aSomePad[ 2 ];
	int						m_nTick;
	float					m_flFlashedStartTime;
	float					m_flFlashedEndTime;
	CAimLayers				m_AimLayers;
	int						m_iModelIndex;
	int						iUnknownArray[ 3 ];
	CBaseEntity*			m_pEntity;
	CBaseCombatWeapon*		m_pActiveWeapon;
	CBaseCombatWeapon*		m_pLastActiveWeapon;
	float					m_flLastUpdateTime;
	int						m_iLastUpdateFrame;
	float					m_flLastUpdateIncrement;
	float					m_flEyeYaw;
	float					m_flEyePitch;
	float					m_flGoalFeetYaw;
	float					m_flGoalFeetYawLast;
	float					m_flMoveYaw;
	float					m_flMoveYawIdeal;
	float					m_flMoveYawCurrentToIdeal;
	float					m_flTimeToAlignLowerBody;
	float					m_flFeetCycle;
	float					m_flMoveWeight;
	float					m_flMoveWeightSmoothed;
	float					m_flDuckAmount;
	float					m_flHitGroundCycle;
	float					m_flRecrouchWeight;
	Vector					m_vecPositionCurrent;
	Vector					m_vecPositionLast;
	Vector					m_vecVelocity;
	Vector					m_vecVelocityNormalized;
	Vector					m_vecVelocityNormalizedNonZero;
	float					m_flVelocityLenght2D;
	float					m_flJumpFallVelocity;
	float					m_flSpeedNormalized;
	float					m_flRunningSpeed;
	float					m_flDuckingSpeed;
	float					m_flDurationMoving;
	float					m_flDurationStill;
	bool					m_bOnGround;
	bool					m_bHitGroundAnimation;
	std::byte				pad2[ 0x2 ];
	float					m_flJumpToFall;
	float					m_flDurationInAir;
	float					m_flLeftGroundHeight;
	float					m_flLandAnimMultiplier;
	float					m_flWalkToRunTransition;
	bool					m_bLandedOnGroundThisFrame;
	bool					m_bLeftTheGroundThisFrame;
	float					m_flInAirSmoothValue;
	bool					m_bOnLadder;
	float					m_flLadderWeight;
	float					m_flLadderSpeed;
	bool					m_bWalkToRunTransitionState;
	bool					m_bDefuseStarted;
	bool					m_bPlantAnimStarted;
	bool					m_bTwitchAnimStarted;
	bool					m_bAdjustStarted;
	char					m_ActivityModifiers[ 20 ];
	float					m_flNextTwitchTime;
	float					m_flTimeOfLastKnownInjury;
	float					m_flLastVelocityTestTime;
	Vector					m_vecVelocityLast;
	Vector					m_vecTargetAcceleration;
	Vector					m_vecAcceleration;
	float					m_flAccelerationWeight;
	float					m_flAimMatrixTransition;
	float					m_flAimMatrixTransitionDelay;
	bool					m_bFlashed;
	float					m_flStrafeChangeWeight;
	float					m_flStrafeChangeTargetWeight;
	float					m_flStrafeChangeCycle;
	int						m_nStrafeSequence;
	bool					m_bStrafeChanging;
	float					m_flDurationStrafing;
	float					m_flFootLerp;
	bool					m_bFeetCrossed;
	bool					m_bPlayerIsAccelerating;
	AnimstatePose			m_tPoseParamMappings[ 20 ];
	float					m_flDurationMoveWeightIsTooHigh;
	float					m_flStaticApproachSpeed;
	int						m_nPreviousMoveState;
	float					m_flStutterStep;
	float					m_flActionWeightBiasRemainder;
	ProceduralFoot			m_tFootLeft;
	ProceduralFoot			m_tFootRight;
	float					m_flCameraSmoothHeight;
	bool					m_bSmoothHeightValid;
	float					m_flLastTimeVelocityOverTen;
	float					m_flUnknownFL0;
	float					m_flMinBodyYaw;
	float					m_flMaxBodyYaw;
	float					m_flMinPitch;
	float					m_flMaxPitch;
	int						m_iAnimsetVersion;
}; // Size: 0x344

