#pragma once
// used: matrix
#include "../memory/datatypes/matrix.h"
#include "../memory/datatypes/utlvector.h"
#include "../memory/datatypes/qangle.h"

using RadianEuler = float[ 3 ];
using Quaternion = float[ 4 ];

#pragma region studio_definitions
#define MAXSTUDIOBONECTRLS			4
#define MAXSTUDIOPOSEPARAM			24
#define MAXSTUDIOSKINS				32		// total textures
#define MAXSTUDIOFLEXCTRL			96		// maximum number of flexcontrollers (input sliders)
#define MAXSTUDIOBONES				128		// total bones actually used
#define MAXSTUDIOANIMBLOCKS			256
#define MAXSTUDIOFLEXDESC			1024	// maximum number of low level flexes (actual morph targets)

#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01
#define BONE_PHYSICS_PROCEDURAL		0x02
#define BONE_ALWAYS_PROCEDURAL		0x04
#define BONE_SCREEN_ALIGN_SPHERE	0x08
#define BONE_SCREEN_ALIGN_CYLINDER	0x10

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100
#define BONE_USED_BY_ATTACHMENT		0x00000200
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400
#define BONE_USED_BY_VERTEX_LOD1	0x00000800
#define BONE_USED_BY_VERTEX_LOD2	0x00001000
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000
#define BONE_ALWAYS_SETUP			0x00080000

#define BONE_USED_BY_VERTEX_AT_LOD( iLOD ) ( BONE_USED_BY_VERTEX_LOD0 << ( iLOD ) )
#define BONE_USED_BY_ANYTHING_AT_LOD( iLOD ) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD( iLOD ) )

#define MAX_NUM_LODS				8

#define BONE_TYPE_MASK				0x00F00000
#define BONE_FIXED_ALIGNMENT		0x00100000
#define BONE_HAS_SAVEFRAME_POS		0x00200000
#define BONE_HAS_SAVEFRAME_ROT64	0x00400000
#define BONE_HAS_SAVEFRAME_ROT32	0x00800000
#pragma endregion

struct vcollide_t;
struct virtualmodel_t;
struct vertexfileheader_t;

struct studiohdr_t;
class CStudioHdr
{
public:
	studiohdr_t*							m_pStudioHdr;
	virtualmodel_t*							m_pVirtualModel;
	void*									m_pSoftbody;
	mutable CUtlVector<const studiohdr_t*>	m_pStudioHdrCache;
	mutable int								m_mnFrameUnlockCounter;
	int*									m_pFrameUnlockCounter;
	std::byte								pad0[ 0x8 ];
	CUtlVector<int>							m_vecBoneFlags;
	CUtlVector<int>							m_vecBoneParent;
	void*									m_pActivityToSequence;
};

struct mstudiobone_t
{
	int			m_nNameIndex;
	int			m_iParent;			// parent bone
	int			m_iBoneController[ 6 ]; // bone controller index, -1 == none

	// default values
	Vector		m_vecPosition;
	Quaternion	m_qWorld;
	RadianEuler m_radRotation;

	// compression scale
	Vector		m_vecPositionScale;
	Vector		m_vecEulerScale;

	matrix3x4_t m_matPoseToBone;
	Quaternion	m_qAlignment;
	int			m_iFlags;
	int			m_iProcedureType;
	int			m_nProcedureIndex;	// procedural rule
	mutable int m_miPhysicsBone;		// index into physically simulated bone
	int			m_nSurfacePropIndex;	// index into string table for property name
	int			m_iContents;			// see bspflags.h for the contents flags
	int			m_iSurfaceProp;		// this index must be cached by the loader, not saved in the file
	std::byte	pad0[ 0x1C ];

	inline const char* GetName( )
	{
		if ( !m_nNameIndex ) return nullptr;
		return ( char* )this + m_nNameIndex;
	}

	inline void* GetProcedure( ) const
	{
		if ( !m_nProcedureIndex ) return nullptr;
		return ( std::uint8_t* )this + m_nProcedureIndex;
	}

	inline const char* GetSurfacePropName( ) const
	{
		if ( !m_nSurfacePropIndex ) return nullptr;
		return ( char* )this + m_nSurfacePropIndex;
	}
};

struct mstudiobonecontroller_t
{
	int			m_iBone; // -1 == 0
	int			m_nType; // X, Y, Z, XR, YR, ZR, M
	float		m_flStart;
	float		m_flEnd;
	int			m_iRest; // byte index value at rest
	int			m_iInputField; // 0-3 user set controller, 4 mouth
	std::byte	pad0[ 0x40 ];
};

struct mstudiobbox_t
{
	int			m_iBone;
	int			m_iGroup; // intersection group
	Vector		m_vecBBMin; // bounding box
	Vector		m_vecBBMax;
	int			m_nHitboxNameIndex; // offset to the name of the hitbox
	QAngle		m_angOffsetOrientation;
	float		m_flRadius;
	std::byte	pad0[ 0x10 ];

	inline const char* GetHitboxName( ) const
	{
		if ( !m_nHitboxNameIndex ) return nullptr;
		return ( const char* )this + m_nHitboxNameIndex;
	}
};

struct mstudiohitboxset_t
{
	int m_nNameIndex;
	int m_nHitboxes;
	int m_nHitboxIndex;

	inline const char* GetName( ) const
	{
		if ( !m_nNameIndex ) return nullptr;
		return ( char* )this + m_nNameIndex;
	}

	inline mstudiobbox_t* GetHitbox( int iHitBox ) const
	{
		if ( iHitBox < 0 || iHitBox >= m_nHitboxes ) return nullptr;
		return ( mstudiobbox_t* )( ( std::uint8_t* )this + m_nHitboxIndex ) + iHitBox;
	}
};

class virtualgroup_t
{
public:
	void*			m_pCache;
	CUtlVector<int> m_vecBoneMap;
	CUtlVector<int> m_vecMasterBone;
	CUtlVector<int> m_vecMasterSequence;
	CUtlVector<int> m_vecMasterAnim;
	CUtlVector<int> m_vecMasterAttachment;
	CUtlVector<int> m_vecMasterPose;
	CUtlVector<int> m_vecMasterNode;
};

struct virtualsequence_t
{
	int	m_nFlags;
	int m_iActivity;
	int m_iGroup;
	int m_nIndex;
};

struct virtualgeneric_t
{
	int m_iGroup;
	int m_nIndex;
};

struct virtualmodel_t
{
	inline virtualgroup_t* GetAnimGroup( const int iAnimation )
	{
		// Note: user must manage mutex for this
		return &m_vecGroup[ m_vecAnim[ iAnimation ].m_iGroup ];
	}

	inline virtualgroup_t* GetSequenceGroup( const int iSequence )
	{
		// Note: user must manage mutex for this
		return &m_vecGroup[ m_vecSequence[ iSequence ].m_iGroup ];
	}

	std::byte						pad0[ 0x8 ]; // CThreadFastMutex
	CUtlVector<virtualsequence_t>	m_vecSequence;
	CUtlVector<virtualgeneric_t>	m_vecAnim;
	CUtlVector<virtualgeneric_t>	m_vecAttachment;
	CUtlVector<virtualgeneric_t>	m_vecPose;
	CUtlVector<virtualgroup_t>		m_vecGroup;
	CUtlVector<virtualgeneric_t>	m_vecNode;
	CUtlVector<virtualgeneric_t>	m_vecIKLock;
	CUtlVector<unsigned short>		m_vecAutoplaySequences;
};

struct studiohdr_t;
struct mstudioseqdesc_t
{
	int m_nBaseStudio;
	inline studiohdr_t* GetStudioHdr( ) const
	{
		return ( studiohdr_t* )( ( std::uint8_t* )this + m_nBaseStudio );
	}

	int	m_nLabelIndex;
	inline const char* GetLabel( ) const
	{
		return ( char* )this + m_nLabelIndex;
	}

	int	m_nActivityNameIndex;
	inline const char* GetActivityName( ) const
	{
		return ( char* )this + m_nActivityNameIndex;
	}

	int	m_iFlags;

	int	m_iActivity;
	int	m_iActivityWeight;

	int m_nEvents;
	int m_nEventIndex;
	inline void* GetEvent( int iEvent ) const
	{
		if ( iEvent < 0 || iEvent >= m_nEvents ) return nullptr;
		return ( ( std::uint8_t* )this + m_nEventIndex ) + iEvent;
	}

	Vector m_vecBBMin;
	Vector m_vecBBMax;

	int	m_nBlends;
	int	m_nBlendIndex;

	inline int GetAnimValue( int x, int y ) const
	{
		if ( x >= m_iGroupSize[ 0 ] )
			x = m_iGroupSize[ 0 ] - 1;

		if ( y >= m_iGroupSize[ 1 ] )
			y = m_iGroupSize[ 1 ] - 1;

		int iOffset = y * m_iGroupSize[ 0 ] + x;
		short* arrBlends = ( short* )( ( std::uint8_t* )this + m_nBlendIndex );
		return ( int )arrBlends[ iOffset ];
	}

	int	m_nMovementIndex;		// [blend] float array for blended movement
	int m_iGroupSize[ 2 ];
	int m_nParamIndex[ 2 ];		// X, Y, Z, XR, YR, ZR
	float m_flParamStart[ 2 ];	// local (0..1) starting value
	float m_flParamEnd[ 2 ];	// local (0..1) ending value
	int	m_iParamParent;

	float m_flFadeInTime;		// ideal cross fate in time (0.2 default)
	float m_flFadeOutTime;	// ideal cross fade out time (0.2 default)

	int	m_iEntryNode;			// transition node at entry
	int	m_iExitNode;			// transition node at exit
	int	m_iNodeFlags;			// transition rules

	float m_flEntryPhase;		// used to match entry gait
	float m_flExitPhase;		// used to match exit gait

	float m_flLastFrame;		// frame that should generation EndOfSequence

	int	m_iNextSequence;		// auto advancing sequences
	int	m_iPose;				// index of delta animation between end and nextseq

	int	m_nIKRules;

	int	m_nAutoLayers;
	int	m_nAutoLayerIndex;
	inline void* GetAutoLayer( int iAutoLayer ) const
	{
		if ( iAutoLayer < 0 || iAutoLayer >= m_nAutoLayers ) return nullptr;
		return ( ( std::uint8_t* )this + m_nAutoLayerIndex ) + iAutoLayer;
	}

	int	m_nWeightListIndex;
	inline float* GetBoneWeight( int iBone ) const
	{
		return ( ( float* )( ( std::uint8_t* )this + m_nWeightListIndex ) + iBone );
	}

	int	m_nPoseKeyIndex;
	inline float* GetPoseKey( int iParameter, int iAnimation ) const
	{
		return ( float* )( ( std::uint8_t* )this + m_nPoseKeyIndex ) + iParameter * m_iGroupSize[ 0 ] + iAnimation;
	}

	int	m_nIKLocks;
	int	m_nIKLockIndex;
	inline void* GetIKLock( int iIK ) const
	{
		if ( iIK < 0 || iIK >= m_nIKLocks ) return nullptr;
		return ( ( std::uint8_t* )this + m_nIKLockIndex ) + iIK;
	}

	int	m_nKeyValueIndex;
	int	m_iKeyValueSize;
	inline const char* KeyValueText( ) const
	{
		if ( !m_iKeyValueSize ) return nullptr;
		return ( char* )this + m_nKeyValueIndex;
	}

	int	m_nCyclePoseIndex;

	int m_nActivityModifierIndex;
	int	m_nActivityModifiers;

	int	m_nAnimTagIndex;
	int	m_nAnimTags;

	int	nRootDriverIndex;
	std::byte pad[ 0x8 ];
};

struct mstudioposeparamdesc_t
{
	int		m_nNameIndex;
	int		m_nFlags;
	float	m_flStart; // starting value
	float	m_flEnd; // ending value
	float	m_flLoop; // looping range, 0 for no looping, 360 for rotations, etc.

	inline const char* GetName( ) const
	{
		return ( char* )this + m_nNameIndex;
	}
};

struct studiohwdata_t;
struct studiohdr_t
{
	int		m_nID;
	int		m_iVersion;
	int		m_iChecksum;
	char	m_szName[ 64 ];
	int		m_iLength;

	Vector	m_vecEyePosition;
	Vector	m_vecIllumPosition;
	Vector	m_vecHullMin;
	Vector	m_vecHullMax;
	Vector	m_vecViewBBMin;
	Vector	m_vecViewBBMax;

	int		m_iFlags;

	int		m_nBones;
	int		m_nBoneIndex;
	inline mstudiobone_t* GetBone( int iBone ) const
	{
		if ( iBone < 0 || iBone >= m_nBones ) return nullptr;
		return ( mstudiobone_t* )( ( std::uint8_t* )this + m_nBoneIndex ) + iBone;
	}

	int		m_nBoneControllers;
	int		m_nBoneControllerIndex;
	inline void* GetBoneController( int iBoneController ) const
	{
		if ( iBoneController < 0 || iBoneController >= m_nBoneControllers ) return nullptr;
		return ( ( std::uint8_t* )this + m_nBoneControllerIndex ) + iBoneController;
	}

	int		m_nHitboxSets;
	int		m_nHitboxSetIndex;
	inline mstudiohitboxset_t* GetHitboxSet( int iSet ) const
	{
		if ( iSet < 0 || iSet >= m_nHitboxSets ) return nullptr;
		return ( mstudiohitboxset_t* )( ( std::uint8_t* )this + m_nHitboxSetIndex ) + iSet;
	}

	inline mstudiobbox_t* GetHitbox( int iHitBox, int iSet ) const
	{
		const mstudiohitboxset_t* pHitboxSet = GetHitboxSet( iSet );

		if ( pHitboxSet == nullptr )
			return nullptr;

		return pHitboxSet->GetHitbox( iHitBox );
	}

	inline int GetHitboxCount( int iSet ) const
	{
		const mstudiohitboxset_t* pHitboxSet = GetHitboxSet( iSet );

		if ( pHitboxSet == nullptr )
			return 0;

		return pHitboxSet->m_nHitboxes;
	}

	int		m_nLocalAnimations;
	int		m_nLocalAnimationIndex;
	inline void* GetAnimDescription( int iAnimation ) const
	{
		if ( iAnimation < 0 || iAnimation >= m_nLocalAnimations ) iAnimation = 0;
		return ( ( std::uint8_t* )this + m_nLocalAnimationIndex ) + iAnimation;
	}

	int		m_nLocalSequences;
	int		m_nLocalSequenceIndex;
	inline mstudioseqdesc_t* GetLocalSequenceDescription( int iSequence ) const
	{
		if ( iSequence < 0 || iSequence >= m_nLocalSequences ) iSequence = 0;
		return ( mstudioseqdesc_t* )( ( std::uint8_t* )this + m_nLocalSequenceIndex ) + iSequence;
	}

	inline mstudioseqdesc_t& GetSequenceDescription( int iSequence, virtualmodel_t* pVirtualModel = nullptr, const studiohdr_t* pStudioHdr = nullptr ) const
	{
		if ( m_nIncludeModels == 0 || pVirtualModel == nullptr || pStudioHdr == nullptr )
			return *GetLocalSequenceDescription( iSequence );

		return *pStudioHdr->GetLocalSequenceDescription( pVirtualModel->m_vecSequence[ iSequence ].m_nIndex );
	}

	mutable int	m_miActivityListVersion;
	mutable int m_miEventsIndexed;

	int		m_nTextures;
	int		m_nTextureIndex;
	inline void* GetTexture( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nTextures ) return nullptr;
		return ( ( std::uint8_t* )this + m_nTextureIndex ) + nIndex;
	}

	int		m_nCdTextures;
	int		m_nCdTextureIndex;
	inline char* GetCdTexture( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nCdTextures ) return nullptr;
		return ( ( ( char* )this ) + *( ( int* )( ( std::uint8_t* )this + m_nCdTextureIndex ) + nIndex ) );
	}

	int		m_nSkinRefs;
	int		m_nSkinFamilies;
	int		m_nSkinIndex;
	inline short* GetSkinRef( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nSkinRefs ) return nullptr;
		return ( short* )( ( std::uint8_t* )this + m_nSkinIndex ) + nIndex;
	}

	int		m_nBodyParts;
	int		m_nBodyPartIndex;
	inline void* GetBodyPart( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nBodyParts ) return nullptr;
		return ( ( std::uint8_t* )this + m_nBodyPartIndex ) + nIndex;
	}

	int		m_nAttachments;
	int		m_nAttachmentIndex;
	inline void* GetAttachment( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nAttachments ) return nullptr;
		return ( ( std::uint8_t* )this + m_nAttachmentIndex ) + nIndex;
	}

	int		m_nTransitions;
	int		m_nTransitionIndex;
	int		m_nTransitionNameIndex;
	inline std::uint8_t* GetTransition( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= ( m_nTransitions * m_nTransitions ) ) return nullptr;
		return ( std::uint8_t* )( ( std::uint8_t* )this + m_nTransitionIndex ) + nIndex;
	}

	int		m_nFlexDescs;
	int		m_nFlexDescIndex;
	inline void* GetFlexDescription( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nFlexDescs ) return nullptr;
		return ( ( std::uint8_t* )this + m_nFlexDescIndex ) + nIndex;
	}

	int		m_nFlexControllers;
	int		m_nFlexControllerIndex;
	inline void* GetFlexController( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nFlexControllers ) return nullptr;
		return ( ( std::uint8_t* )this + m_nFlexControllerIndex ) + nIndex;
	}

	int		m_nFlexRules;
	int		m_nFlexRuleIndex;
	inline void* GetFlexRule( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nFlexRules ) return nullptr;
		return ( ( std::uint8_t* )this + m_nFlexRuleIndex ) + nIndex;
	}

	int		m_nIkChains;
	int		m_nIkChainIndex;
	inline void* GetIKChain( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nIkChains ) return nullptr;
		return ( ( std::uint8_t* )this + m_nIkChainIndex ) + nIndex;
	}

	int		m_nMouths;
	int		m_nMouthIndex;
	inline void* GetMouth( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nMouths ) return nullptr;
		return ( ( std::uint8_t* )this + m_nMouthIndex ) + nIndex;
	}

	int		m_nPoseParameters;
	int		m_nPoseParameterIndex;
	inline void* GetPoseParameter( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nPoseParameters ) return nullptr;
		return ( ( std::uint8_t* )this + m_nPoseParameterIndex ) + nIndex;
	}

	int		m_nSurfacePropIndex;
	inline const char* GetSurfaceProp( ) const
	{
		if ( !m_nSurfacePropIndex ) return nullptr;
		return ( char* )this + m_nSurfacePropIndex;
	}

	int		m_nKeyValueIndex;
	int		m_nKeyValueSize;
	inline const char* KeyValueText( ) const
	{
		if ( !m_nKeyValueSize ) return nullptr;
		return ( char* )this + m_nKeyValueIndex;
	}

	int		m_nIkAutoplayLocks;
	int		m_nIkAutoplayLockIndex;
	inline void* GetIKAutoplayLock( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nIkAutoplayLocks ) return nullptr;
		return ( ( std::uint8_t* )this + m_nIkAutoplayLockIndex ) + nIndex;
	}

	float	m_flMass;
	int		m_iContents;

	int		m_nIncludeModels;
	int		m_nIncludeModelIndex;
	inline void* GetModelGroup( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nIncludeModels ) return nullptr;
		return ( ( std::uint8_t* )this + m_nIncludeModelIndex ) + nIndex;
	}

	int		m_iVirtualModel;

	int		m_nAnimBlockNameIndex;
	inline const char* GetAnimBlockName( ) const
	{
		return ( ( char* )this ) + m_nAnimBlockNameIndex;
	}

	int		m_nAnimBlocks;
	int		m_nAnimBlockIndex;
	inline void* GetAnimBlock( int nIndex ) const
	{
		if ( nIndex < 0 || nIndex >= m_nAnimBlocks ) return nullptr;
		return ( ( ( std::uint8_t* )this ) + m_nAnimBlockIndex ) + nIndex;
	}

	int		m_iAnimBlockModel;

	int		m_nBoneTableByNameIndex;
	inline const std::uint8_t* GetBoneTableSortedByName( ) const
	{
		return ( std::uint8_t* )this + m_nBoneTableByNameIndex;
	}

	int		m_iVertexBase;
	int		m_nIndexBase;

	std::uint8_t m_uDirectionalLightDot;
	std::uint8_t m_uRootLOD;
	std::uint8_t m_nAllowedRootLODs;
	std::byte	 pad0[ 0x5 ];

	int		m_nFlexControllerUI;
	int		m_nFlexcontrollerUIIndex;

	float	m_flVertAnimFixedPointScale;
	mutable int m_miSurfacePropLookup;
	int		m_nStudioHdr2Index;

	std::byte pad1[ 0x4 ];
};