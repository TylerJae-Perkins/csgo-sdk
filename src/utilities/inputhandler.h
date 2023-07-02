#pragma once
#include <array>

#include "../common.h"

enum class EKeyState : int
{
	NONE,
	DOWN,
	UP,
	RELEASED
};

enum EKeyInputType : int
{
	NONE,
	HOLD,
	TOGGLE,
	ALWAYS_ON
};

namespace InputHandler
{
	// Values
	/* current window */
	inline HWND	m_hWindow = nullptr;
	/* saved window messages handler */
	inline WNDPROC m_pOldWndProc = nullptr;
	/* last processed key states */
	inline std::array<EKeyState, 256U> m_arrKeyState = { };
	inline bool m_arrKeyBooleans[ 256 ];

	// Get
	/* set our window messages proccesor */
	bool Setup( );
	/* restore window messages processor and clear saved pointer */
	void Restore( );
	/* process input window message and save keys states in array */
	bool Process( UINT uMsg, WPARAM wParam, LPARAM lParam );

	/* is given key being held */
	inline bool IsKeyDown( const std::uint32_t uButtonCode )
	{
		return m_arrKeyState.at( uButtonCode ) == EKeyState::DOWN;
	}

	/* was given key released */
	inline bool IsKeyReleased( const std::uint32_t uButtonCode )
	{
		if ( m_arrKeyState.at( uButtonCode ) == EKeyState::RELEASED )
		{
			m_arrKeyState.at( uButtonCode ) = EKeyState::UP;
			return true;
		}

		return false;
	}

	inline bool HandleInput( const std::uint32_t uButtonCode )
	{
		if ( uButtonCode == 0 )
			return false;

		if ( Variables::m_vecKeyStateTypes.at( uButtonCode ) == HOLD ) {
			return IsKeyDown( uButtonCode );
		}
		else if ( Variables::m_vecKeyStateTypes.at( uButtonCode ) == TOGGLE ) {

			if ( IsKeyReleased( uButtonCode ) )
				m_arrKeyBooleans[ uButtonCode ] = !m_arrKeyBooleans[ uButtonCode ];

			return m_arrKeyBooleans[ uButtonCode ];
		}
		else if ( Variables::m_vecKeyStateTypes.at( uButtonCode ) == ALWAYS_ON ) {
			return true;
		}

		return false;

	}
}