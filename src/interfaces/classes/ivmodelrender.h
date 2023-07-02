#pragma once
#include "../../memory/memory.h"
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/ivmodelrender.h
using ModelInstanceHandle_t = std::uint16_t;

struct ModelRenderInfo_t
{
	Vector					m_vecOrigin;
	QAngle					m_angAngles;
	std::byte				pad0[ 0x4 ];
	IClientRenderable*		m_pRenderable;
	const Model_t*			m_pModel;
	const matrix3x4_t*		m_pModelToWorld;
	const matrix3x4_t*		m_pLightingOffset;
	const Vector*			m_pLightingOrigin;
	int						m_iFlags;
	int						m_nEntityIndex;
	int						m_iSkin;
	int						m_iBody;
	int						m_iHitboxSet;
	ModelInstanceHandle_t	m_hInstance;
};

struct RenderableInfo_t
{
	IClientRenderable*		m_pRenderable;			// 0x00
	void*					m_pAlphaProperty;		// 0x04 | @FIX ME should be: IClientAlphaProperty*
	int						m_iEnumCount;			// 0x08
	int						m_nRenderFrame;			// 0x0C
	unsigned short			m_uFirstShadow;			// 0x10
	unsigned short			m_uLeafList;			// 0x12
	short					m_sArea;				// 0x14
	std::uint16_t			m_uFlags;				// 0x16
	std::uint16_t			m_uFlags2;				// 0x18
	Vector					m_vecBloatedAbsMins;	// 0x1A
	Vector					m_vecBloatedAbsMaxs;	// 0x26
	Vector					m_vecAbsMins;			// 0x32
	Vector					m_vecAbsMaxs;			// 0x3E
	std::byte				pad0[ 0x4 ];			// 0x4A
}; // Size: 0x4E

struct studiohwdata_t;
struct StudioDecalHandle_t;
struct DrawModelState_t
{
	studiohdr_t*			m_pStudioHdr;
	studiohwdata_t*			m_pStudioHWData;
	IClientRenderable*		m_pRenderable;
	const matrix3x4_t*		m_pModelToWorld;
	StudioDecalHandle_t*	m_pDecals;
	int						m_iDrawFlags;
	int						m_iLOD;
};

class IVModelRender
{
public:
	void ForcedMaterialOverride( IMaterial* pMaterial, EOverrideType overrideType = OVERRIDE_NORMAL, int nOverrides = 0 )
	{
		return Memory::CallVFunc<void>( this, 1, pMaterial, overrideType, nOverrides );
	}

	bool IsForcedMaterialOverride( )
	{
		return Memory::CallVFunc<bool>( this, 2 );
	}
};