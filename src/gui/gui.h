#pragma once
#include <map>

#include "../common.h"
#include "widgets.h"
#include "../memory/datatypes/color.h"
#include "../config/variables.h"
#include "../utilities/inputhandler.h"

namespace Fonts
{
	inline ImFont* Montserrat;
	inline ImFont* MontserratS;
	inline ImFont* MontserratM;
	inline ImFont* MontserratL;
	inline ImFont* MontserratXL;

	inline ImFont* Verdana;

	inline ImFont* Icons;
	inline ImFont* IconsS;
	inline ImFont* IconsL;
}

namespace Tabs
{
	inline int		m_iActiveTab = 0;
	inline float	m_flPreviousAlpha = 1.f;

	// Tabs
	void LegitBot( ImVec4 m_vecMenuTheme );
	void RageBot( ImVec4 m_vecMenuTheme );
	void AntiAim( ImVec4 m_vecMenuTheme );
	void Visuals( ImVec4 m_vecMenuTheme );
	void Miscellaneous( ImVec4 m_vecMenuTheme );
	void SkinChanger( ImVec4 m_vecMenuTheme );
}

namespace gui
{
	// initialize
	void Initialize( IDirect3DDevice9* m_pDevice, unsigned int m_uFontFlags = 0x0 );
	// render watermark
	void RenderWatermark( ImDrawList* m_pDrawList, const ImVec2 m_vecScreenSize, ImVec4 m_vecMenuTheme );
	// render menu
	void RenderMainWindow( ImVec2 m_vecScreenSize, ImVec4 m_vecMenuTheme, float m_flAlpha );
	// render 
	void RenderGui( IDirect3DDevice9* m_pDevice );

	inline bool m_bInitialized = false;
	inline bool m_bIsOpen = false;
	inline IDirect3DDevice9* m_pCurrentDevice = nullptr;
	inline Color* m_pColorClipboard;
}