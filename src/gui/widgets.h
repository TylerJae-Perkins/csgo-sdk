#pragma once
#include "../common.h"
#include <unordered_map>
#include "../memory/datatypes/color.h"

namespace ImGui
{
	// buttons
    bool TabButton( const char* label, const char* label_id, const ImVec2& size_arg, bool m_bDisableAnimation = false, bool use_icon = false, ImFont* icon_font = nullptr, const char* icon_label = nullptr, bool isActive = false, ImVec4 m_vecColor = ImVec4( 1.f, 1.f, 1.f, 1.f ), bool m_bDrawArrow = false );
	bool CustomButton( const char* label, const char* id, const ImVec2& size_arg = ImVec2( 0, 0 ), ImVec4 m_vecColor = ImVec4( 1.f, 1.f, 1.f, 1.f ), bool m_bDisableAnimation = false, bool use_icon = false, ImFont* icon_font = nullptr, const char* icon_label = nullptr, bool isActive = false, bool closePopups = true );

    // combo
	bool FormattedCombo( const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1 );
	bool MultiCombo( const char* szLabel, const char** szDisplayName, std::vector<bool>& v, int nHeightInItems );
	bool MultiComboInternal( const char* szLabel, const char** szDisplayName, std::vector<bool>& v, int nHeightInItems );
	
	// color picker
	bool CustomColorButton( const char* desc_id, Color* col, ImGuiColorEditFlags flags, ImVec2 size, ImVec4 m_vecColor );
	bool ColorPickerBox( const char* picker_idname, Color* col_n, float paddingExclusion, bool alpha = true, ImVec4 m_vecColor = ImVec4( 1.f, 1.f, 1.f, 1.f ) );

	// hotkey
	bool HotKey( const char* szLabel, int* v, ImVec4 m_vecColor );

	// pie pop-up menus
	int PiePopupSelectMenuWeapons( const ImVec2& center, const char* popup_id, const char** items, int items_count, int* p_selected, ImVec4 m_vecColor, const int* m_iWeapon );
	
	// helpers
	void PushDisabled( bool disable );
	void PopDisabled( bool disable );
}