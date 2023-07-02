#pragma once
// used: vector
#include "vector.h"

using matrix3x3_t = float[ 3 ][ 3 ];
struct matrix3x4_t
{
	matrix3x4_t( ) = default;

	constexpr matrix3x4_t(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23 )
	{
		m_arrData[ 0 ][ 0 ] = m00; m_arrData[ 0 ][ 1 ] = m01; m_arrData[ 0 ][ 2 ] = m02; m_arrData[ 0 ][ 3 ] = m03;
		m_arrData[ 1 ][ 0 ] = m10; m_arrData[ 1 ][ 1 ] = m11; m_arrData[ 1 ][ 2 ] = m12; m_arrData[ 1 ][ 3 ] = m13;
		m_arrData[ 2 ][ 0 ] = m20; m_arrData[ 2 ][ 1 ] = m21; m_arrData[ 2 ][ 2 ] = m22; m_arrData[ 2 ][ 3 ] = m23;
	}

	constexpr matrix3x4_t( const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin )
	{
		Init( xAxis, yAxis, zAxis, vecOrigin );
	}

	constexpr void Init( const Vector& vecForward, const Vector& vecLeft, const Vector& vecUp, const Vector& vecOrigin )
	{
		SetForward( vecForward );
		SetLeft( vecLeft );
		SetUp( vecUp );
		SetOrigin( vecOrigin );
	}

	constexpr void SetForward( const Vector& vecForward )
	{
		this->m_arrData[ 0 ][ 0 ] = vecForward.x;
		this->m_arrData[ 1 ][ 0 ] = vecForward.y;
		this->m_arrData[ 2 ][ 0 ] = vecForward.z;
	}

	constexpr void SetLeft( const Vector& vecLeft )
	{
		this->m_arrData[ 0 ][ 1 ] = vecLeft.x;
		this->m_arrData[ 1 ][ 1 ] = vecLeft.y;
		this->m_arrData[ 2 ][ 1 ] = vecLeft.z;
	}

	constexpr void SetUp( const Vector& vecUp )
	{
		this->m_arrData[ 0 ][ 2 ] = vecUp.x;
		this->m_arrData[ 1 ][ 2 ] = vecUp.y;
		this->m_arrData[ 2 ][ 2 ] = vecUp.z;
	}

	constexpr void SetOrigin( const Vector& vecOrigin )
	{
		this->m_arrData[ 0 ][ 3 ] = vecOrigin.x;
		this->m_arrData[ 1 ][ 3 ] = vecOrigin.y;
		this->m_arrData[ 2 ][ 3 ] = vecOrigin.z;
	}

	constexpr void Invalidate( )
	{
		for ( auto& arrSubData : m_arrData )
		{
			for ( auto& flData : arrSubData )
				flData = std::numeric_limits<float>::infinity( );
		}
	}

	float* operator[]( const int nIndex )
	{
		return m_arrData[ nIndex ];
	}

	const float* operator[]( const int nIndex ) const
	{
		return m_arrData[ nIndex ];
	}

	[[nodiscard]] constexpr Vector at( const int nIndex ) const
	{
		return Vector( m_arrData[ 0 ][ nIndex ], m_arrData[ 1 ][ nIndex ], m_arrData[ 2 ][ nIndex ] );
	}

	float* Base( )
	{
		return &m_arrData[ 0 ][ 0 ];
	}

	[[nodiscard]] const float* Base( ) const
	{
		return &m_arrData[ 0 ][ 0 ];
	}

	float m_arrData[ 3 ][ 4 ] = { };
};

__declspec( align( 16 ) ) class matrix3x4a_t : public matrix3x4_t
{
public:
	matrix3x4a_t& operator=( const matrix3x4_t& matSource )
	{
		std::copy_n( matSource.Base( ), sizeof( float ) * 3U * 4U, this->Base( ) );
		return *this;
	}
};

struct ViewMatrix_t
{
	ViewMatrix_t( ) = default;

	constexpr ViewMatrix_t(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33 )
	{
		m_arrData[ 0 ][ 0 ] = m00; m_arrData[ 0 ][ 1 ] = m01; m_arrData[ 0 ][ 2 ] = m02; m_arrData[ 0 ][ 3 ] = m03;
		m_arrData[ 1 ][ 0 ] = m10; m_arrData[ 1 ][ 1 ] = m11; m_arrData[ 1 ][ 2 ] = m12; m_arrData[ 1 ][ 3 ] = m13;
		m_arrData[ 2 ][ 0 ] = m20; m_arrData[ 2 ][ 1 ] = m21; m_arrData[ 2 ][ 2 ] = m22; m_arrData[ 2 ][ 3 ] = m23;
		m_arrData[ 3 ][ 0 ] = m30; m_arrData[ 3 ][ 1 ] = m31; m_arrData[ 3 ][ 2 ] = m32; m_arrData[ 3 ][ 3 ] = m33;
	}

	constexpr ViewMatrix_t( const matrix3x4_t& matFrom, const Vector4D& vecAdditionalColumn = { } )
	{
		m_arrData[ 0 ][ 0 ] = matFrom.m_arrData[ 0 ][ 0 ]; m_arrData[ 0 ][ 1 ] = matFrom.m_arrData[ 0 ][ 1 ]; m_arrData[ 0 ][ 2 ] = matFrom.m_arrData[ 0 ][ 2 ]; m_arrData[ 0 ][ 3 ] = matFrom.m_arrData[ 0 ][ 3 ];
		m_arrData[ 1 ][ 0 ] = matFrom.m_arrData[ 1 ][ 0 ]; m_arrData[ 1 ][ 1 ] = matFrom.m_arrData[ 1 ][ 1 ]; m_arrData[ 1 ][ 2 ] = matFrom.m_arrData[ 1 ][ 2 ]; m_arrData[ 1 ][ 3 ] = matFrom.m_arrData[ 1 ][ 3 ];
		m_arrData[ 2 ][ 0 ] = matFrom.m_arrData[ 2 ][ 0 ]; m_arrData[ 2 ][ 1 ] = matFrom.m_arrData[ 2 ][ 1 ]; m_arrData[ 2 ][ 2 ] = matFrom.m_arrData[ 2 ][ 2 ]; m_arrData[ 2 ][ 3 ] = matFrom.m_arrData[ 2 ][ 3 ];
		m_arrData[ 3 ][ 0 ] = vecAdditionalColumn.x; m_arrData[ 3 ][ 1 ] = vecAdditionalColumn.y; m_arrData[ 3 ][ 2 ] = vecAdditionalColumn.z; m_arrData[ 3 ][ 3 ] = vecAdditionalColumn.w;
	}

	float* operator[]( const int nIndex )
	{
		return m_arrData[ nIndex ];
	}

	const float* operator[]( const int nIndex ) const
	{
		return m_arrData[ nIndex ];
	}

	[[nodiscard]] constexpr Vector4D at( const int nIndex ) const
	{
		return Vector4D( m_arrData[ 0 ][ nIndex ], m_arrData[ 1 ][ nIndex ], m_arrData[ 2 ][ nIndex ], m_arrData[ 3 ][ nIndex ] );
	}

	constexpr ViewMatrix_t& operator+=( const ViewMatrix_t& matAdd )
	{
		for ( int i = 0; i < 4; i++ )
		{
			for ( int n = 0; n < 4; n++ )
				this->m_arrData[ i ][ n ] += matAdd[ i ][ n ];
		}

		return *this;
	}

	constexpr ViewMatrix_t& operator-=( const ViewMatrix_t& matSubtract )
	{
		for ( int i = 0; i < 4; i++ )
		{
			for ( int n = 0; n < 4; n++ )
				this->m_arrData[ i ][ n ] -= matSubtract[ i ][ n ];
		}

		return *this;
	}

	ViewMatrix_t operator*( const ViewMatrix_t& matMultiply ) const
	{
		return ViewMatrix_t(
			m_arrData[ 0 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 0 ] + m_arrData[ 0 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 0 ] + m_arrData[ 0 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 0 ] + m_arrData[ 0 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 0 ],
			m_arrData[ 0 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 1 ] + m_arrData[ 0 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 1 ] + m_arrData[ 0 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 1 ] + m_arrData[ 0 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 1 ],
			m_arrData[ 0 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 2 ] + m_arrData[ 0 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 2 ] + m_arrData[ 0 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 2 ] + m_arrData[ 0 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 2 ],
			m_arrData[ 0 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 3 ] + m_arrData[ 0 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 3 ] + m_arrData[ 0 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 3 ] + m_arrData[ 0 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 3 ],

			m_arrData[ 1 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 0 ] + m_arrData[ 1 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 0 ] + m_arrData[ 1 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 0 ] + m_arrData[ 1 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 0 ],
			m_arrData[ 1 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 1 ] + m_arrData[ 1 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 1 ] + m_arrData[ 1 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 1 ] + m_arrData[ 1 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 1 ],
			m_arrData[ 1 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 2 ] + m_arrData[ 1 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 2 ] + m_arrData[ 1 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 2 ] + m_arrData[ 1 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 2 ],
			m_arrData[ 1 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 3 ] + m_arrData[ 1 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 3 ] + m_arrData[ 1 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 3 ] + m_arrData[ 1 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 3 ],

			m_arrData[ 2 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 0 ] + m_arrData[ 2 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 0 ] + m_arrData[ 2 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 0 ] + m_arrData[ 2 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 0 ],
			m_arrData[ 2 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 1 ] + m_arrData[ 2 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 1 ] + m_arrData[ 2 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 1 ] + m_arrData[ 2 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 1 ],
			m_arrData[ 2 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 2 ] + m_arrData[ 2 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 2 ] + m_arrData[ 2 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 2 ] + m_arrData[ 2 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 2 ],
			m_arrData[ 2 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 3 ] + m_arrData[ 2 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 3 ] + m_arrData[ 2 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 3 ] + m_arrData[ 2 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 3 ],

			m_arrData[ 3 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 0 ] + m_arrData[ 3 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 0 ] + m_arrData[ 3 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 0 ] + m_arrData[ 3 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 0 ],
			m_arrData[ 3 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 1 ] + m_arrData[ 3 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 1 ] + m_arrData[ 3 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 1 ] + m_arrData[ 3 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 1 ],
			m_arrData[ 3 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 2 ] + m_arrData[ 3 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 2 ] + m_arrData[ 3 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 2 ] + m_arrData[ 3 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 2 ],
			m_arrData[ 3 ][ 0 ] * matMultiply.m_arrData[ 0 ][ 3 ] + m_arrData[ 3 ][ 1 ] * matMultiply.m_arrData[ 1 ][ 3 ] + m_arrData[ 3 ][ 2 ] * matMultiply.m_arrData[ 2 ][ 3 ] + m_arrData[ 3 ][ 3 ] * matMultiply.m_arrData[ 3 ][ 3 ] );
	}

	constexpr void Identity( )
	{
		for ( int i = 0; i < 4; i++ )
		{
			for ( int n = 0; n < 4; n++ )
				this->m_arrData[ i ][ n ] = i == n ? 1.0f : 0.0f;
		}
	}

	const matrix3x4_t& As3x4( ) const
	{
		return *reinterpret_cast< const matrix3x4_t* >( this );
	}

	matrix3x4_t& As3x4( )
	{
		return *reinterpret_cast< matrix3x4_t* >( this );
	}

	float m_arrData[ 4 ][ 4 ] = { };
};
