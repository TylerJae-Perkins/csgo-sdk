#pragma once
#include "../../memory/datatypes/vector.h"
#include "../../memory/datatypes/matrix.h"
#include "../../memory/memory.h"
#include "irefcount.h"
#include "imaterial.h"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/mathlib/lightdesc.h
enum ELightType : int
{
	MATERIAL_LIGHT_DISABLE = 0,
	MATERIAL_LIGHT_POINT,
	MATERIAL_LIGHT_DIRECTIONAL,
	MATERIAL_LIGHT_SPOT
};

enum ELightTypeOptimizationFlags
{
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION0 = 1,
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION1 = 2,
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION2 = 4,
	LIGHTTYPE_OPTIMIZATIONFLAGS_DERIVED_VALUES_CALCED = 8
};

struct LightDesc_t
{
	void InitDirectional( const Vector& vecDirection, const Vector& vecColor )
	{
		this->m_nType = MATERIAL_LIGHT_DIRECTIONAL;
		this->m_vecColor = vecColor;
		this->m_vecDirection = vecDirection;
		this->m_flRange = 0.0f;
		this->m_flAttenuation0 = 1.0f;
		this->m_flAttenuation1 = 0.f;
		this->m_flAttenuation2 = 0.f;

		m_fFlags = LIGHTTYPE_OPTIMIZATIONFLAGS_DERIVED_VALUES_CALCED;

		if ( m_flAttenuation0 )
			m_fFlags |= LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION0;
		if ( m_flAttenuation1 )
			m_fFlags |= LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION1;
		if ( m_flAttenuation2 )
			m_fFlags |= LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION2;

		if ( m_nType == MATERIAL_LIGHT_SPOT )
		{
			m_flThetaDot = std::cosf( m_flTheta );
			m_flPhiDot = std::cosf( m_flPhi );

			if ( const float m_flSpread = m_flThetaDot - m_flPhiDot; m_flSpread > 1.0e-10f )
				// note - this quantity is very sensitive to round off error. the sse reciprocal approximation won't cut it here.
				m_flOneOverThetaDotMinusPhiDot = 1.0f / m_flSpread;
			else
				// hard falloff instead of divide by zero
				m_flOneOverThetaDotMinusPhiDot = 1.0f;
		}
		else if ( m_nType == MATERIAL_LIGHT_DIRECTIONAL )
		{
			// set position to be real far away in the right direction
			m_vecPosition = vecDirection;
			m_vecPosition *= 2.0e6;
		}

		m_flRangeSquared = m_flRange * m_flRange;
	}

	ELightType	m_nType;
	Vector		m_vecColor;
	Vector		m_vecPosition;
	Vector		m_vecDirection;
	float		m_flRange;
	float		m_flFalloff;
	float		m_flAttenuation0;
	float		m_flAttenuation1;
	float		m_flAttenuation2;
	float		m_flTheta;
	float		m_flPhi;
	float		m_flThetaDot;
	float		m_flPhiDot;
	float		m_flOneOverThetaDotMinusPhiDot;
	std::uint32_t m_fFlags;
protected:
	float		m_flRangeSquared;
};

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/client/glow_outline_effect.cpp
enum EStencilOperation : int
{
	STENCILOPERATION_KEEP = 1,
	STENCILOPERATION_ZERO = 2,
	STENCILOPERATION_REPLACE = 3,
	STENCILOPERATION_INCRSAT = 4,
	STENCILOPERATION_DECRSAT = 5,
	STENCILOPERATION_INVERT = 6,
	STENCILOPERATION_INCR = 7,
	STENCILOPERATION_DECR = 8,
	STENCILOPERATION_FORCE_DWORD = 0x7FFFFFFF
};

enum EStencilComparisonFunction : int
{
	STENCILCOMPARISONFUNCTION_NEVER = 1,
	STENCILCOMPARISONFUNCTION_LESS = 2,
	STENCILCOMPARISONFUNCTION_EQUAL = 3,
	STENCILCOMPARISONFUNCTION_LESSEQUAL = 4,
	STENCILCOMPARISONFUNCTION_GREATER = 5,
	STENCILCOMPARISONFUNCTION_NOTEQUAL = 6,
	STENCILCOMPARISONFUNCTION_GREATEREQUAL = 7,
	STENCILCOMPARISONFUNCTION_ALWAYS = 8,
	STENCILCOMPARISONFUNCTION_FORCE_DWORD = 0x7FFFFFFF
};

struct ShaderStencilState_t
{
	ShaderStencilState_t( )
	{
		m_bEnable = false;
		m_PassOperation = m_FailOperation = m_ZFailOperation = STENCILOPERATION_KEEP;
		m_CompareFunction = STENCILCOMPARISONFUNCTION_ALWAYS;
		m_nReferenceValue = 0;
		m_uTestMask = m_uWriteMask = 0xFFFFFFFF;
	}

	bool						m_bEnable;
	EStencilOperation			m_FailOperation;
	EStencilOperation			m_ZFailOperation;
	EStencilOperation			m_PassOperation;
	EStencilComparisonFunction	m_CompareFunction;
	int							m_nReferenceValue;
	std::uint32_t				m_uTestMask;
	std::uint32_t				m_uWriteMask;
};

enum MaterialMatrixMode_t
{
	MATERIAL_VIEW = 0,
	MATERIAL_PROJECTION,

	MATERIAL_MATRIX_UNUSED0,
	MATERIAL_MATRIX_UNUSED1,
	MATERIAL_MATRIX_UNUSED2,
	MATERIAL_MATRIX_UNUSED3,
	MATERIAL_MATRIX_UNUSED4,
	MATERIAL_MATRIX_UNUSED5,
	MATERIAL_MATRIX_UNUSED6,
	MATERIAL_MATRIX_UNUSED7,

	MATERIAL_MODEL,

	// Total number of matrices
	NUM_MATRIX_MODES = MATERIAL_MODEL + 1,
};

class VMatrix;

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/materialsystem/imaterialsystem.h
class IMatRenderContext : public IRefCounted
{
public:
	void BeginRender( )
	{
		Memory::CallVFunc<void>( this, 2 );
	}

	void EndRender( )
	{
		Memory::CallVFunc<void>( this, 3 );
	}

	void BindLocalCubemap( ITexture* pTexture )
	{
		Memory::CallVFunc<void>( this, 5, pTexture );
	}

	void SetRenderTarget( ITexture* pTexture )
	{
		Memory::CallVFunc<void>( this, 6, pTexture );
	}

	ITexture* GetRenderTarget( )
	{
		return Memory::CallVFunc<ITexture*>( this, 7 );
	}

	void ClearBuffers( bool bClearColor, bool bClearDepth, bool bClearStencil = false )
	{
		Memory::CallVFunc<void>( this, 12, bClearColor, bClearDepth, bClearStencil );
	}

	void SetLights( int nCount, const LightDesc_t* pLights )
	{
		Memory::CallVFunc<void>( this, 17, nCount, pLights );
	}

	void SetAmbientLightCube( Vector4D vecCube[ 6 ] )
	{
		Memory::CallVFunc<void>( this, 18, vecCube );
	}

	void GetMatrix( MaterialMatrixMode_t matrixMode, VMatrix* matrix )
	{
		Memory::CallVFunc<void>( this, 31, matrixMode, matrix );
	}

	void GetMatrix( MaterialMatrixMode_t matrixMode, matrix3x4_t* matrix )
	{
		Memory::CallVFunc<void>( this, 32, matrixMode, matrix );
	}

	void Viewport( int x, int y, int iWidth, int iHeight )
	{
		Memory::CallVFunc<void>( this, 40, x, y, iWidth, iHeight );
	}

	void GetViewport( int& x, int& y, int& iWidth, int& iHeight )
	{
		Memory::CallVFunc<void, int&, int&, int&, int&>( this, 41, x, y, iWidth, iHeight );
	}

	void ClearColor3ub( unsigned char r, unsigned char g, unsigned char b )
	{
		Memory::CallVFunc<void>( this, 78, r, g, b );
	}

	void ClearColor4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
	{
		Memory::CallVFunc<void>( this, 79, r, g, b, a );
	}

	void DrawScreenSpaceRectangle( IMaterial* pMaterial, int iDestX, int iDestY, int iWidth, int iHeight, float flTextureX0, float flTextureY0, float flTextureX1, float flTextureY1, int iTextureWidth, int iTextureHeight, void* pClientRenderable = nullptr, int nXDice = 1, int nYDice = 1 )
	{
		Memory::CallVFunc<void>( this, 114, pMaterial, iDestX, iDestY, iWidth, iHeight, flTextureX0, flTextureY0, flTextureX1, flTextureY1, iTextureWidth, iTextureHeight, pClientRenderable, nXDice, nYDice );
	}

	void PushRenderTargetAndViewport( )
	{
		Memory::CallVFunc<void>( this, 119 );
	}

	void PopRenderTargetAndViewport( )
	{
		Memory::CallVFunc<void>( this, 120 );
	}

	void SetIntRenderingParameter( int int1, int int2 )
	{
		Memory::CallVFunc<void>( this, 126, int1, int2 );
	}

	void SetLightingOrigin( Vector vecLightingOrigin )
	{
		Memory::CallVFunc<void>( this, 158, vecLightingOrigin.x, vecLightingOrigin.y, vecLightingOrigin.z );
	}

	void SetScissorRect( const int nLeft, const int nTop, const int nRight, const int nBottom, const bool bEnableScissor = true )
	{
		Memory::CallVFunc<void>( this, 159, nLeft, nTop, nRight, nBottom, bEnableScissor );
	}

	void PopScissorRect( )
	{
		Memory::CallVFunc<void>( this, 160 );
	}
};