#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/istudiorender.h
#include "../../memory/memory.h"
#include "../../memory/datatypes/vector.h"
#include "../../valve/studio.h"
#include "ivmodelinfo.h"

#pragma region studiorender_enumerations
enum
{
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,
	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,
	STUDIORENDER_DRAW_ACCURATETIME = 0x10,
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,
	STUDIORENDER_DRAW_WIREFRAME = 0x80,
	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,
	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,
	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,
	STUDIORENDER_GENERATE_STATS = 0x8000,
};

enum EOverrideType : int
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE
};
#pragma endregion

struct MaterialLightingState_t
{
	Vector			m_vecAmbientCube[ 6 ];
	Vector			m_vecLightingOrigin;
	int				m_nLocalLightCount;
	LightDesc_t		m_localLightDesc[ 4 ];
};

struct DrawModelResults_t;
struct ColorMeshInfo_t;
struct StudioDecalHandle_t { int m_iUnused; };
struct DrawModelInfo_t
{
	studiohdr_t*			m_pStudioHdr;
	studiohwdata_t*			m_pHardwareData;
	StudioDecalHandle_t		m_hDecals;
	int						m_iSkin;
	int						m_iBody;
	int						m_iHitboxSet;
	IClientRenderable*		m_pClientEntity;
	int						m_iLOD;
	ColorMeshInfo_t*		m_pColorMeshes;
	bool					m_bStaticLighting;
	MaterialLightingState_t	m_lightingState;
};

class IStudioRender
{
public:
	void SetColorModulation( float const* arrColor )
	{
		Memory::CallVFunc<void>( this, 27, arrColor );
	}

	void SetAlphaModulation( float flAlpha )
	{
		Memory::CallVFunc<void>( this, 28, flAlpha );
	}

	void ForcedMaterialOverride( IMaterial* pMaterial, EOverrideType nOverrideType = OVERRIDE_NORMAL, int nOverrides = 0 )
	{
		Memory::CallVFunc<void>( this, 33, pMaterial, nOverrideType, nOverrides );
	}
};