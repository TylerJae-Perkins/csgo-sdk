#include "widgets.h"
#include "../utilities/iconparser.h"

struct circle_button
{
    float  alpha;
    float  radius;
    bool   active;
    ImVec2 pos;
    bool   pos_set;
};

bool ImGui::TabButton( const char* label, const char* label_id, const ImVec2& size_arg, bool m_bDisableAnimation, bool use_icon, ImFont* icon_font, const char* icon_label, bool isActive, ImVec4 m_vecColor, bool m_bDrawArrow )
{
    ImGuiWindow* window = ImGui::GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID     id = window->GetID( label_id );
    const ImVec2      label_size = ImGui::CalcTextSize( label, NULL, true );

    if ( use_icon && icon_font )
        ImGui::PushFont( icon_font );

    const ImVec2 icon_size = use_icon ? ImGui::CalcTextSize( icon_label, NULL, true ) : ImVec2( 0, 0 );

    if ( use_icon && icon_font )
        ImGui::PopFont( );

    auto flags = 0;

    ImVec2 pos = window->DC.CursorPos;
    if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = ImGui::CalcItemSize( size_arg, ImGui::CalcItemWidth( ), label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ImGui::ItemSize( size, style.FramePadding.y );
    if ( !ImGui::ItemAdd( bb, id ) )
        return false;

    float deltatime = 1.5f * ImGui::GetIO( ).DeltaTime;

    if ( window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat )
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior( bb, id, &hovered, &held, flags );

    const ImVec4 text_dis = ImVec4( 202 / 255.f, 202 / 255.f, 202 / 255.f, 1.f );
    const ImVec4 text_act = m_vecColor;
    const ImVec4 text_hov = m_vecColor;
    const ImVec4 icon_act = m_vecColor;

    static std::map<ImGuiID, float> hover_animation;
    auto                            it_hover = hover_animation.find( id );
    if ( it_hover == hover_animation.end( ) )
    {
        hover_animation.insert( { id, 0.f } );
        it_hover = hover_animation.find( id );
    }
    it_hover->second = std::clamp( it_hover->second + ( 3.f * ImGui::GetIO( ).DeltaTime * ( hovered ? 1.f : -1.f ) ), 0.0f, 1.f );

    static std::unordered_map<ImGuiID, ImVec4> text_animation;
    auto                                       it_text = text_animation.find( id );
    if ( it_text == text_animation.end( ) )
    {
        text_animation.insert( { id, text_dis } );
        it_text = text_animation.find( id );
    }
    if ( hovered || isActive )
    {
        ImVec4 to = isActive ? text_act : text_hov;
        if ( it_text->second.x != to.x )
        {
            if ( it_text->second.x < to.x )
                it_text->second.x = ImMin( it_text->second.x + deltatime, to.x );
            else if ( it_text->second.x > to.x )
                it_text->second.x = ImMax( to.x, it_text->second.x - deltatime );
        }

        if ( it_text->second.y != to.y )
        {
            if ( it_text->second.y < to.y )
                it_text->second.y = ImMin( it_text->second.y + deltatime, to.y );
            else if ( it_text->second.y > to.y )
                it_text->second.y = ImMax( to.y, it_text->second.y - deltatime );
        }

        if ( it_text->second.z != to.z )
        {
            if ( it_text->second.z < to.z )
                it_text->second.z = ImMin( it_text->second.z + deltatime, to.z );
            else if ( it_text->second.z > to.z )
                it_text->second.z = ImMax( to.z, it_text->second.z - deltatime );
        }
    }
    else
    {
        ImVec4 to = text_dis;
        if ( it_text->second.x != to.x )
        {
            if ( it_text->second.x < to.x )
                it_text->second.x = ImMin( it_text->second.x + deltatime, to.x );
            else if ( it_text->second.x > to.x )
                it_text->second.x = ImMax( to.x, it_text->second.x - deltatime );
        }

        if ( it_text->second.y != to.y )
        {
            if ( it_text->second.y < to.y )
                it_text->second.y = ImMin( it_text->second.y + deltatime, to.y );
            else if ( it_text->second.y > to.y )
                it_text->second.y = ImMax( to.y, it_text->second.y - deltatime );
        }

        if ( it_text->second.z != to.z )
        {
            if ( it_text->second.z < to.z )
                it_text->second.z = ImMin( it_text->second.z + deltatime, to.z );
            else if ( it_text->second.z > to.z )
                it_text->second.z = ImMax( to.z, it_text->second.z - deltatime );
        }
    }

    static std::map<ImGuiID, circle_button> circle_animation;
    auto                                    it_circle = circle_animation.find( id );
    if ( it_circle == circle_animation.end( ) )
    {
        circle_animation.insert( { id, {0.8f, 0.f, false, ImVec2( 0, 0 ), false} } );
        it_circle = circle_animation.find( id );
    }
    
    if ( pressed || it_circle->second.active )
    {
        if ( !it_circle->second.pos_set )
        {
            it_circle->second.pos = ImGui::GetIO( ).MousePos;
            it_circle->second.pos_set = true;
        }
        it_circle->second.active = true;
        it_circle->second.alpha = std::clamp( it_circle->second.alpha - ( 2.f * ImGui::GetIO( ).DeltaTime ), 0.0f, 0.8f );
        it_circle->second.radius = std::clamp( it_circle->second.radius + ( 240.f * ImGui::GetIO( ).DeltaTime ), 0.0f, 400.f );

        if ( it_circle->second.alpha <= 0.f )
            it_circle->second.active = false;
    }
    else
    {
        it_circle->second.alpha = 0.8f;
        it_circle->second.radius = 0.f;
        it_circle->second.pos = ImVec2( 0, 0 );
        it_circle->second.pos_set = false;
    }
   

    // Render
    const ImU32 col = ImGui::GetColorU32( ImGuiCol_Button );

    if ( !m_bDisableAnimation )
    {
        window->DrawList->PushClipRect( bb.Min, bb.Max, false );
        ImGui::GetWindowDrawList( )->AddCircleFilled( it_circle->second.pos, it_circle->second.radius, ImGui::GetColorU32( ImVec4( m_vecColor.x, m_vecColor.y, m_vecColor.z, std::min( m_vecColor.w, it_circle->second.alpha ) ) ), 60 );
        window->DrawList->PopClipRect( );
    }

    //ImGui::GetWindowDrawList()->AddRect(bb.Min, bb.Max, GetColorU32(ImVec4(0 / 255.f, 158 / 255.f, 100 / 255.f, it_hover->second)), 2.5f);

    if ( m_bDrawArrow )
    {
        ImGui::PushStyleColor( ImGuiCol_Text, it_text->second );

        ImGui::PushFont( icon_font );
        ImGui::RenderTextClipped( bb.Min + ImVec2( 20.f, 0 ), bb.Max, icon_label, NULL, &icon_size, ImVec2( 0.0f, 0.5f ), &bb );
        ImGui::PopFont( );

        ImGui::PopStyleColor( );

        if ( isActive )
        {
            window->DrawList->AddRectFilled( ImVec2( bb.Max.x - 2.f, bb.Min.y + 3.f ), ImVec2( bb.Max.x, bb.Max.y - 3.f ), ImGui::GetColorU32( it_text->second ) );
        }
    }

    ImGui::PushStyleColor( ImGuiCol_Text, it_text->second );
    ImGui::RenderTextClipped( bb.Min, bb.Max, label, NULL, &label_size, ImVec2( 0.5f, 0.5f ), &bb );
    ImGui::PopStyleColor( );

    // Automatically close popups
    if ( pressed && !( flags & ImGuiButtonFlags_DontClosePopups ) && ( window->Flags & ImGuiWindowFlags_Popup ) )
        ImGui::CloseCurrentPopup( );

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, window->DC.LastItemStatusFlags );
    return pressed;
}

bool ImGui::CustomButton( const char* label, const char* label_id, const ImVec2& size_arg, ImVec4 m_vecColor, bool m_bDisableAnimation, bool use_icon, ImFont* icon_font, const char* icon_label, bool isActive, bool closePopups )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID     id = window->GetID( label_id );
    const ImVec2      label_size = ImGui::CalcTextSize( label, NULL, true );

    if ( use_icon && icon_font )
        ImGui::PushFont( icon_font );

    const ImVec2 icon_size = use_icon ? ImGui::CalcTextSize( icon_label, NULL, true ) : ImVec2( 0, 0 );

    if ( use_icon && icon_font )
        ImGui::PopFont( );

    auto flags = 0;

    ImVec2 pos = window->DC.CursorPos;
    if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = ImGui::CalcItemSize( size_arg, ImGui::CalcItemWidth( ) + 98.f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ImGui::ItemSize( size, style.FramePadding.y );
    if ( !ImGui::ItemAdd( bb, id ) )
        return false;

    float deltatime = 1.5f * ImGui::GetIO( ).DeltaTime;

    if ( window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat )
        flags |= ImGuiButtonFlags_Repeat;
    if ( !closePopups )
        flags |= ImGuiButtonFlags_DontClosePopups;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior( bb, id, &hovered, &held, flags );

    const ImVec4 text_dis = ImVec4( 202 / 255.f, 202 / 255.f, 202 / 255.f, 1.f );
    const ImVec4 text_act = m_vecColor;
    const ImVec4 text_hov = m_vecColor;
    const ImVec4 icon_act = m_vecColor;

    static std::map<ImGuiID, float> hover_animation;
    auto                            it_hover = hover_animation.find( id );
    if ( it_hover == hover_animation.end( ) )
    {
        hover_animation.insert( { id, 0.f } );
        it_hover = hover_animation.find( id );
    }
    it_hover->second = std::clamp( it_hover->second + ( 3.f * ImGui::GetIO( ).DeltaTime * ( hovered ? 1.f : -1.f ) ), 0.0f, 1.f );

    static std::unordered_map<ImGuiID, ImVec4> text_animation;
    auto                                       it_text = text_animation.find( id );
    if ( it_text == text_animation.end( ) )
    {
        text_animation.insert( { id, text_dis } );
        it_text = text_animation.find( id );
    }
    if ( hovered || isActive )
    {
        ImVec4 to = isActive ? text_act : text_hov;
        if ( it_text->second.x != to.x )
        {
            if ( it_text->second.x < to.x )
                it_text->second.x = ImMin( it_text->second.x + deltatime, to.x );
            else if ( it_text->second.x > to.x )
                it_text->second.x = ImMax( to.x, it_text->second.x - deltatime );
        }

        if ( it_text->second.y != to.y )
        {
            if ( it_text->second.y < to.y )
                it_text->second.y = ImMin( it_text->second.y + deltatime, to.y );
            else if ( it_text->second.y > to.y )
                it_text->second.y = ImMax( to.y, it_text->second.y - deltatime );
        }

        if ( it_text->second.z != to.z )
        {
            if ( it_text->second.z < to.z )
                it_text->second.z = ImMin( it_text->second.z + deltatime, to.z );
            else if ( it_text->second.z > to.z )
                it_text->second.z = ImMax( to.z, it_text->second.z - deltatime );
        }
    }
    else
    {
        ImVec4 to = text_dis;
        if ( it_text->second.x != to.x )
        {
            if ( it_text->second.x < to.x )
                it_text->second.x = ImMin( it_text->second.x + deltatime, to.x );
            else if ( it_text->second.x > to.x )
                it_text->second.x = ImMax( to.x, it_text->second.x - deltatime );
        }

        if ( it_text->second.y != to.y )
        {
            if ( it_text->second.y < to.y )
                it_text->second.y = ImMin( it_text->second.y + deltatime, to.y );
            else if ( it_text->second.y > to.y )
                it_text->second.y = ImMax( to.y, it_text->second.y - deltatime );
        }

        if ( it_text->second.z != to.z )
        {
            if ( it_text->second.z < to.z )
                it_text->second.z = ImMin( it_text->second.z + deltatime, to.z );
            else if ( it_text->second.z > to.z )
                it_text->second.z = ImMax( to.z, it_text->second.z - deltatime );
        }
    }

    static std::map<ImGuiID, circle_button> circle_animation;
    auto                                    it_circle = circle_animation.find( id );
    if ( it_circle == circle_animation.end( ) )
    {
        circle_animation.insert( { id, {0.8f, 0.f, false, ImVec2( 0, 0 ), false} } );
        it_circle = circle_animation.find( id );
    }
    if ( strcmp( "Save config", label ) != 0 && strcmp( "Remove config", label ) != 0 && strcmp( "Save & Close", label ) != 0 )
    {
        if ( pressed || it_circle->second.active )
        {
            if ( !it_circle->second.pos_set )
            {
                it_circle->second.pos = GetIO( ).MousePos;
                it_circle->second.pos_set = true;
            }
            it_circle->second.active = true;
            it_circle->second.alpha = std::clamp( it_circle->second.alpha - ( 2.f * ImGui::GetIO( ).DeltaTime ), 0.0f, 0.8f );
            it_circle->second.radius = std::clamp( it_circle->second.radius + ( 240.f * ImGui::GetIO( ).DeltaTime ), 0.0f, 400.f );

            if ( it_circle->second.alpha <= 0.f )
                it_circle->second.active = false;
        }
        else
        {
            it_circle->second.alpha = 0.8f;
            it_circle->second.radius = 0.f;
            it_circle->second.pos = ImVec2( 0, 0 );
            it_circle->second.pos_set = false;
        }
    }

    // Render
    const ImU32 col = GetColorU32( ImGuiCol_Button );

    ImGui::RenderFrame( bb.Min, bb.Max, col, true, 2.5f );

    if ( !m_bDisableAnimation )
    {
        window->DrawList->PushClipRect( bb.Min, bb.Max, false );
        ImGui::GetWindowDrawList( )->AddCircleFilled( it_circle->second.pos, it_circle->second.radius, GetColorU32( ImVec4( m_vecColor.x, m_vecColor.y, m_vecColor.z, std::min( m_vecColor.w, it_circle->second.alpha ) ) ), 60 );
        window->DrawList->PopClipRect( );
    }


    ImGui::GetWindowDrawList( )->AddRect( bb.Min, bb.Max, GetColorU32( ImVec4( m_vecColor.x, m_vecColor.y, m_vecColor.z, std::min( m_vecColor.w, it_hover->second ) ) ), 2.5f );

    if ( use_icon )
    {
        ImGui::PushStyleColor( ImGuiCol_Text, icon_act );

        ImGui::PushFont( icon_font );
        ImGui::RenderTextClipped( bb.Min + ImVec2( 20.f, 0 ), bb.Max, icon_label, NULL, &icon_size, ImVec2( 0.0f, 0.5f ), &bb );
        ImGui::PopFont( );

        ImGui::PopStyleColor( );
    }

    ImGui::PushStyleColor( ImGuiCol_Text, it_text->second );
    ImGui::RenderTextClipped( bb.Min, bb.Max, label, NULL, &label_size, ImVec2( 0.5f, 0.5f ), &bb );
    ImGui::PopStyleColor( );

    // Automatically close popups
    if ( pressed && !( flags & ImGuiButtonFlags_DontClosePopups ) && ( window->Flags & ImGuiWindowFlags_Popup ) )
        ImGui::CloseCurrentPopup( );

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, window->DC.LastItemStatusFlags );
    return pressed;
}

bool ImGui::FormattedCombo( const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items )
{
    ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 2 );
    //ImGui::Indent( 17.f );
    ImGui::Text( label );
    ImGui::SetCursorPosY( GetCursorPosY( ) - 5 );
    bool returnvalue = ImGui::Combo( label, current_item, items_separated_by_zeros, height_in_items );
    //ImGui::Unindent( 17.f );

    return returnvalue;
}

bool ImGui::MultiCombo( const char* szLabel, const char** szDisplayName, std::vector<bool>& v, int nHeightInItems )
{
    ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 2 );
    ImGui::Text( szLabel );
    ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 5 );
    bool returnvalue = MultiComboInternal( szLabel, szDisplayName, v, nHeightInItems );

    return returnvalue;
}

static float CalcMaxPopupHeightFromItemCount( int items_count )
{
    ImGuiContext& g = *GImGui;
    if ( items_count <= 0 )
        return FLT_MAX;
    return ( g.FontSize + g.Style.ItemSpacing.y ) * items_count - g.Style.ItemSpacing.y + ( g.Style.WindowPadding.y * 2 );
}

bool ImGui::MultiComboInternal( const char* szLabel, const char** szDisplayName, std::vector<bool>& v, int nHeightInItems )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* pWindow = g.CurrentWindow;
    if ( pWindow->SkipItems )
        return false;

    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;


    if ( nHeightInItems != -1 && !( g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint ) )
        ImGui::SetNextWindowSizeConstraints( ImVec2( 0, 0 ), ImVec2( FLT_MAX, CalcMaxPopupHeightFromItemCount( nHeightInItems ) ) );

    std::string  szBuffer;
    const ImVec2 vecLabelSize = ImGui::CalcTextSize( szLabel );
    float        flActiveWidth = ImGui::CalcItemWidth( ) - ( style.ItemInnerSpacing.x + ImGui::GetFrameHeight( ) ) - 10.f;

    for ( int i = 0; i < nHeightInItems; i++ )
    {
        if ( v[ i ] )
        {
            ImVec2 vecTextSize = ImGui::CalcTextSize( szBuffer.c_str( ) );

            if ( szBuffer.empty( ) )
                szBuffer.assign( szDisplayName[ i ] );
            else
                szBuffer.append( ", " ).append( szDisplayName[ i ] );

            if ( vecTextSize.x > flActiveWidth )
                szBuffer.erase( szBuffer.find_last_of( "," ) ).append( "..." );
        }
    }

    if ( szBuffer.empty( ) )
        szBuffer.assign( "-" );

    bool bValueChanged = false;
    if ( ImGui::BeginCombo( szLabel, szBuffer.c_str( ) ) )
    {
        for ( int i = 0; i < nHeightInItems; i++ )
        {
            if ( ImGui::Selectable( szDisplayName[ i ], v[ i ], ImGuiSelectableFlags_DontClosePopups ) )
            {
                v[ i ] = !v[ i ];
                bValueChanged = true;
            }
        }

        ImGui::EndCombo( );
    }

    return bValueChanged;
}

static inline ImU32 AlphaBlend( ImU32 col_a, ImU32 col_b )
{
    float t = ( ( col_b >> IM_COL32_A_SHIFT ) & 0xFF ) / 255.f;
    int   r = ImLerp( ( int )( col_a >> IM_COL32_R_SHIFT ) & 0xFF, ( int )( col_b >> IM_COL32_R_SHIFT ) & 0xFF, t );
    int   g = ImLerp( ( int )( col_a >> IM_COL32_G_SHIFT ) & 0xFF, ( int )( col_b >> IM_COL32_G_SHIFT ) & 0xFF, t );
    int   b = ImLerp( ( int )( col_a >> IM_COL32_B_SHIFT ) & 0xFF, ( int )( col_b >> IM_COL32_B_SHIFT ) & 0xFF, t );
    return IM_COL32( r, g, b, 0xFF );
}

bool ImGui::CustomColorButton( const char* desc_id, Color* col, ImGuiColorEditFlags flags, ImVec2 size, ImVec4 m_vecColor )
{
    ImGuiWindow* window = ImGui::GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiID id = window->GetID( desc_id );
    float         default_size = ImGui::GetFrameHeight( );
    if ( size.x == 0.0f )
        size.x = default_size;
    if ( size.y == 0.0f )
        size.y = default_size;
    const ImRect bb( window->DC.CursorPos, window->DC.CursorPos + size );
    ImGui::ItemSize( bb, ( size.y >= default_size ) ? g.Style.FramePadding.y : 0.0f );
    if ( !ImGui::ItemAdd( bb, id ) )
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior( bb, id, &hovered, &held );

    const bool bLeftClicked = hovered && io.MouseClicked[ 1 ];

    std::string popupLabel = std::to_string( id ) + std::to_string( *desc_id ) + "popItUpCuuh";

    if ( bLeftClicked && !( g.ActiveId == id ) )
    {
        ImGui::OpenPopup( popupLabel.c_str( ) );
    }

    std::string       buttonLabel1 = "Copy##" + popupLabel + "btnCopy";
    std::string       buttonLabel2 = "Paste##" + popupLabel + "btnPaste";
    static int         mode = 0;

    if ( ImGui::BeginPopup( popupLabel.c_str( ) ) )
    {
        if ( ImGui::CustomButton( buttonLabel1.c_str( ), buttonLabel1.c_str( ), ImVec2( 70, 25 ), m_vecColor, true, false, nullptr, nullptr, false, true ) )
            gui::m_pColorClipboard = col;
        
        if ( ImGui::CustomButton( buttonLabel2.c_str( ), buttonLabel2.c_str( ), ImVec2( 70, 25 ), m_vecColor, true, false, nullptr, nullptr, false, true ) && gui::m_pColorClipboard )
            col->Set( gui::m_pColorClipboard->R( ), gui::m_pColorClipboard->G( ), gui::m_pColorClipboard->B( ), gui::m_pColorClipboard->A( ) );
            //col = gui::m_pColorClipboard;

        ImGui::EndPopup( );
    }

    if ( flags & ImGuiColorEditFlags_NoAlpha )
        flags &= ~( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf );

    ImVec4 col_rgb = ImVec4{ col->Base<COLOR_R>( ), col->Base<COLOR_G>( ), col->Base<COLOR_B>( ), col->Base<COLOR_A>( ) };

    if ( flags & ImGuiColorEditFlags_InputHSV )
        ImGui::ColorConvertHSVtoRGB( col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z );

    static std::unordered_map<ImGuiID, float> hover_animation;
    auto                                      it_hover = hover_animation.find( id );
    if ( it_hover == hover_animation.end( ) )
    {
        hover_animation.insert( { id, 0.f } );
        it_hover = hover_animation.find( id );
    }
    it_hover->second = ImClamp( it_hover->second + ( 3.f * ImGui::GetIO( ).DeltaTime * ( hovered ? 1.f : -1.f ) ), 0.0f, 1.f );

    ImVec4 col_rgb_without_alpha( col_rgb.x, col_rgb.y, col_rgb.z, 1.0f );
    ImRect bb_inner = bb;
    float  off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
    bb_inner.Expand( off );
    float mid_x = IM_ROUND( ( bb_inner.Min.x + bb_inner.Max.x ) * 0.5f );
    float mid_y = IM_ROUND( ( bb_inner.Min.y + bb_inner.Max.y ) * 0.5f );

    if ( ( flags & ImGuiColorEditFlags_AlphaPreviewHalf ) && col_rgb.w < 1.0f )
        window->DrawList->AddCircleFilled( ImVec2( mid_x, mid_y ), size.x / 4, ImGui::GetColorU32( col_rgb_without_alpha ), 64 );
    else
    {
        ImVec4 col_source = ( flags & ImGuiColorEditFlags_AlphaPreview ) ? col_rgb : col_rgb_without_alpha;
        window->DrawList->AddCircleFilled( ImVec2( mid_x, mid_y ), size.x / 4, ImGui::GetColorU32( col_rgb ), 64 );
    }
    window->DrawList->AddCircle( ImVec2( mid_x, mid_y ), ( size.x / 4 ) + 1.f, ImGui::GetColorU32( ImVec4( m_vecColor.x, m_vecColor.y, m_vecColor.z, col_rgb.w <= 0.09f ? 1.f : std::min( m_vecColor.z, it_hover->second ) ) ), 64 );

    ImGui::RenderNavHighlight( bb, id );
    return pressed;
}

bool ImGui::ColorPickerBox( const char* picker_idname, Color* col_n, float paddingExclusion, bool alpha, ImVec4 m_vecColor )
{
    bool valuechanged = false;
    auto colthing = col_n->GetU32( );

    auto vecColor = ImVec4{ col_n->Base<COLOR_R>( ), col_n->Base<COLOR_G>( ), col_n->Base<COLOR_B>( ), col_n->Base<COLOR_A>( ) };

    ImGui::SameLine( );
    ImGui::SetCursorPosX( ImGui::GetContentRegionMax( ).x - 22.5f );
    bool open_popup = ImGui::CustomColorButton( picker_idname, col_n, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview /*| ImGuiColorEditFlags_AlphaPreview */| ImGuiColorEditFlags_NoTooltip, ImVec2( 32, 0 ), m_vecColor );
    if ( open_popup )
    {
        ImGui::OpenPopup( picker_idname );
    }

    float col[ 4 ] = { col_n->Base<COLOR_R>( ), col_n->Base<COLOR_G>( ), col_n->Base<COLOR_B>( ), col_n->Base<COLOR_A>( ) };

    if ( ImGui::BeginPopup( picker_idname ) )
    {
        std::string id_new = picker_idname;
        id_new += "##pickeritself_";

        if ( ImGui::ColorPicker4( id_new.c_str( ), col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | ( alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0 ) ) )
        {
            col_n->Set( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] );
            valuechanged = true;
        }

        ImGui::EndPopup( );
    }
    return valuechanged;
}

static constexpr std::array<const char*, 166U> m_arrKeyNames =
{
    "",
    "mouse 1", "mouse 2", "cancel", "mouse 3", "mouse 4", "mouse 5", "",
    "backspace", "tab", "", "", "clear", "enter", "", "",
    "shift", "control", "alt", "pause", "caps", "", "", "", "", "", "",
    "escape", "", "", "", "", "space", "page up", "page down",
    "end", "home", "left", "up", "right", "down", "", "", "",
    "print", "insert", "delete", "",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "", "", "", "", "", "", "",
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
    "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
    "v", "w", "x", "y", "z", "lwin", "rwin", "", "", "",
    "num0", "num1", "num2", "num3", "num4", "num5",
    "num6", "num7", "num8", "num9",
    "*", "+", "", "-", ".", "/",
    "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8",
    "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16",
    "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24",
    "", "", "", "", "", "", "", "",
    "num lock", "scroll lock",
    "", "", "", "", "", "", "",
    "", "", "", "", "", "", "",
    "lshift", "rshift", "lctrl",
    "rctrl", "lmenu", "rmenu"
};

bool ImGui::HotKey( const char* szLabel, int* v, ImVec4 m_vecColor )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* pWindow = g.CurrentWindow;
    if ( pWindow->SkipItems )
        return false;

    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;
    const ImGuiID nIndex = pWindow->GetID( szLabel );

    const float flWidth = CalcTextSize( "mouse 32", nullptr, true ).x;
    const ImVec2 vecLabelSize = CalcTextSize( szLabel, nullptr, true );

    SameLine( GetContentRegionMax( ).x - flWidth + 2.5f );
    SetCursorPosY( GetCursorPosY( ) - ( ( GetFrameHeight( ) * 0.9f ) / 14 ) );

    const ImRect frame_bb( pWindow->DC.CursorPos, pWindow->DC.CursorPos + ImVec2( flWidth, GetFrameHeight( ) /** 1.5f*/ ) );
    const ImRect total_bb( frame_bb.Min, frame_bb.Max );

    const ImU32 frame_col = GetColorU32( g.HoveredId == nIndex ? ImGuiCol_ButtonHovered : ImGuiCol_Button );
    RenderNavHighlight( frame_bb, nIndex );
    RenderFrame( frame_bb.Min, frame_bb.Max, frame_col, true, 4.5f );

    ItemSize( total_bb, style.FramePadding.y );
    if ( !ItemAdd( total_bb, nIndex, &frame_bb ) )
        return false;

    const bool bHovered = ItemHoverable( frame_bb, nIndex );
    if ( bHovered )
    {
        SetHoveredID( nIndex );
        g.MouseCursor = ImGuiMouseCursor_TextInput;

        if ( *v != 0 )
            SetTooltip( "Left-click to set an input mode" );
    }

    const bool bFocusRequested = FocusableItemRegister( pWindow, nIndex );
    const bool bClicked = bHovered && io.MouseClicked[ 0 ];
    const bool bLeftClicked = bHovered && io.MouseClicked[ 1 ];
    const bool bDoubleClicked = bHovered && g.IO.MouseDoubleClicked[ 0 ];
    if ( bFocusRequested || bClicked || bDoubleClicked )
    {
        if ( g.ActiveId != nIndex )
        {
            memset( io.MouseDown, 0, sizeof( io.MouseDown ) );
            memset( io.KeysDown, 0, sizeof( io.KeysDown ) );
            *v = 0;
        }

        SetActiveID( nIndex, pWindow );
        FocusWindow( pWindow );
    }

    if ( bLeftClicked && !( g.ActiveId == nIndex ) && *v != 0 ) {
        OpenPopup( szLabel );
    }

    auto buttonLabel1 = std::to_string( *szLabel ) + "btnHold";
    auto buttonLabel2 = std::to_string( *szLabel ) + "btnToggle";
    auto buttonLabel3 = std::to_string( *szLabel ) + "btnAlwayson";
    static int mode = 0;


    if ( ImGui::BeginPopup( szLabel ) )
    {
        if ( ImGui::TabButton( "Hold", buttonLabel1.c_str( ), ImVec2( 70, 15 ), true, true, Fonts::Icons, "", Variables::m_vecKeyStateTypes.at( *v ) == EKeyInputType::HOLD, m_vecColor, true ) ) Variables::m_vecKeyStateTypes.at( *v ) = EKeyInputType::HOLD;
        if ( ImGui::TabButton( "Toggle", buttonLabel2.c_str( ), ImVec2( 70, 15 ), true, true, Fonts::Icons, "", Variables::m_vecKeyStateTypes.at( *v ) == EKeyInputType::TOGGLE, m_vecColor, true ) ) Variables::m_vecKeyStateTypes.at( *v ) = EKeyInputType::TOGGLE;
        if ( ImGui::TabButton( "Always-on", buttonLabel3.c_str( ), ImVec2( 70, 15 ), true, true, Fonts::Icons, "", Variables::m_vecKeyStateTypes.at( *v ) == EKeyInputType::ALWAYS_ON, m_vecColor, true ) ) Variables::m_vecKeyStateTypes.at( *v ) = EKeyInputType::ALWAYS_ON;

        ImGui::EndPopup( );
    }

    bool bPressed = false;
    if ( int nKey = *v; g.ActiveId == nIndex )
    {
        for ( int n = 0; n < IM_ARRAYSIZE( io.MouseDown ); n++ )
        {
            if ( IsMouseDown( n ) )
            {
                switch ( n )
                {
                case 0:
                    nKey = VK_LBUTTON;
                    break;
                case 1:
                    nKey = VK_RBUTTON;
                    break;
                case 2:
                    nKey = VK_MBUTTON;
                    break;
                case 3:
                    nKey = VK_XBUTTON1;
                    break;
                case 4:
                    nKey = VK_XBUTTON2;
                    break;
                }

                bPressed = true;
                ClearActiveID( );
            }
        }

        if ( !bPressed )
        {
            for ( int n = VK_BACK; n <= VK_RMENU; n++ )
            {
                if ( IsKeyDown( n ) )
                {
                    nKey = n;
                    bPressed = true;
                    ClearActiveID( );
                }
            }
        }

        if ( IsKeyPressed( io.KeyMap[ ImGuiKey_Escape ] ) )
        {
            *v = 0;
            ClearActiveID( );
        }
        else
            *v = nKey;
    }

    const ImVec4 text_dis = ImVec4( 202 / 255.f, 202 / 255.f, 202 / 255.f, 1.f );
    const ImVec4 text_act = m_vecColor;
    const ImVec4 text_hov = m_vecColor;

    float deltatime = 1.5f * ImGui::GetIO( ).DeltaTime;

    static std::unordered_map<ImGuiID, ImVec4> text_animation;
    auto it_text = text_animation.find( nIndex );
    if ( it_text == text_animation.end( ) )
    {
        text_animation.insert( { nIndex, text_dis } );
        it_text = text_animation.find( nIndex );
    }
    if ( bHovered || g.ActiveId == nIndex )
    {
        ImVec4 to = g.ActiveId == nIndex ? text_act : text_hov;
        if ( it_text->second.x != to.x )
        {
            if ( it_text->second.x < to.x )
                it_text->second.x = ImMin( it_text->second.x + deltatime, to.x );
            else if ( it_text->second.x > to.x )
                it_text->second.x = ImMax( to.x, it_text->second.x - deltatime );
        }

        if ( it_text->second.y != to.y )
        {
            if ( it_text->second.y < to.y )
                it_text->second.y = ImMin( it_text->second.y + deltatime, to.y );
            else if ( it_text->second.y > to.y )
                it_text->second.y = ImMax( to.y, it_text->second.y - deltatime );
        }

        if ( it_text->second.z != to.z )
        {
            if ( it_text->second.z < to.z )
                it_text->second.z = ImMin( it_text->second.z + deltatime, to.z );
            else if ( it_text->second.z > to.z )
                it_text->second.z = ImMax( to.z, it_text->second.z - deltatime );
        }
    }
    else
    {
        ImVec4 to = text_dis;
        if ( it_text->second.x != to.x )
        {
            if ( it_text->second.x < to.x )
                it_text->second.x = ImMin( it_text->second.x + deltatime, to.x );
            else if ( it_text->second.x > to.x )
                it_text->second.x = ImMax( to.x, it_text->second.x - deltatime );
        }

        if ( it_text->second.y != to.y )
        {
            if ( it_text->second.y < to.y )
                it_text->second.y = ImMin( it_text->second.y + deltatime, to.y );
            else if ( it_text->second.y > to.y )
                it_text->second.y = ImMax( to.y, it_text->second.y - deltatime );
        }

        if ( it_text->second.z != to.z )
        {
            if ( it_text->second.z < to.z )
                it_text->second.z = ImMin( it_text->second.z + deltatime, to.z );
            else if ( it_text->second.z > to.z )
                it_text->second.z = ImMax( to.z, it_text->second.z - deltatime );
        }
    }

    static std::map<ImGuiID, float> hover_animation;
    auto it_hover = hover_animation.find( nIndex );
    if ( it_hover == hover_animation.end( ) )
    {
        hover_animation.insert( { nIndex, 0.f } );
        it_hover = hover_animation.find( nIndex );
    }
    it_hover->second = std::clamp( it_hover->second + ( 3.f * ImGui::GetIO( ).DeltaTime * ( bHovered ? 1.f : -1.f ) ), 0.0f, 1.f );

    ImGui::GetWindowDrawList( )->AddRect( frame_bb.Min, frame_bb.Max, GetColorU32( ImVec4( m_vecColor.x, m_vecColor.y, m_vecColor.z, std::min( m_vecColor.z, it_hover->second ) ) ), 4.5f );

    char chBuffer[ 64 ] = { };
    sprintf_s( chBuffer, sizeof( chBuffer ), XorStr( "%s" ), *v != 0 && g.ActiveId != nIndex ? m_arrKeyNames.at( *v ) : g.ActiveId == nIndex ? XorStr( "..." ) : XorStr( "key" ) );

    float oldFrameHeight = GetFrameHeight( ) * 1.5f;
    // modified by qo0
    ImGui::PushFont( Fonts::MontserratS );
    ImVec2 bufTxtSize = CalcTextSize( chBuffer );
    PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( style.FramePadding.x, -1 ) );
    pWindow->DrawList->AddText( ImVec2( total_bb.Min.x + ( flWidth / 2 ) - ( bufTxtSize.x / 2 ), total_bb.Max.y - ( bufTxtSize.y * 1.3f ) ), GetColorU32( it_text->second ), chBuffer );
    ImGui::PopFont( );
    //if (vecLabelSize.x > 0.f)
        //RenderText(ImVec2(total_bb.Min.x, total_bb.Min.y + style.FramePadding.y), szLabel);

    PopStyleVar( );
    return bPressed;
}

int ImGui::PiePopupSelectMenuWeapons( const ImVec2& center, const char* popup_id, const char** items, int items_count, int* p_selected, ImVec4 m_vecColor, const int* m_iWeapon )
{
    int ret = -1;

    // FIXME: Missing a call to query if Popup is open so we can move the PushStyleColor inside the BeginPopupBlock (e.g. IsPopupOpen() in imgui.cpp)
    // FIXME: Our PathFill function only handle convex polygons, so we can't have items spanning an arc too large else inner concave edge artifact is too visible, hence the ImMax(7,items_count)
    PushStyleColor( ImGuiCol_PopupBg, ImVec4( 0, 0, 0, 0 ) );
    PushStyleColor( ImGuiCol_Border, ImVec4( 0, 0, 0, 0 ) );
    if ( BeginPopup( popup_id ) )
    {
        const auto  drag_delta = ImVec2( GetIO( ).MousePos.x - center.x, GetIO( ).MousePos.y - center.y );
        const float drag_dist2 = drag_delta.x * drag_delta.x + drag_delta.y * drag_delta.y;

        const ImGuiStyle& style = GetStyle( );
        const float       RADIUS_MIN = 20.0f;
        const float       RADIUS_MAX = 150.f;
        const float       RADIUS_INTERACT_MIN = 0.0f;
        const int         ITEMS_MIN = 6;

        ImDrawList* draw_list = GetWindowDrawList( );
        draw_list->PushClipRectFullScreen( );
        draw_list->PathArcTo( center, ( RADIUS_MIN + RADIUS_MAX ) * 0.5f, 0.0f, IM_PI * 2.0f * 0.99f, 128 ); // FIXME: 0.99f look like full arc with closed thick stroke has a bug now
        draw_list->PathStroke( ImColor( 0.07f, 0.07f, 0.07f, 1.f ), true, RADIUS_MAX - RADIUS_MIN );

        const float item_arc_span = 2 * IM_PI / ImMax( ITEMS_MIN, items_count );
        float       drag_angle = atan2f( drag_delta.y, drag_delta.x );
        if ( drag_angle < -0.5f * item_arc_span )
            drag_angle += 2.0f * IM_PI;
        //Text("%f", drag_angle);    // [Debug]

        int item_hovered = -1;
        for ( int item_n = 0; item_n < items_count; item_n++ )
        {
            const char* item_label = items[ item_n ];
            const float inner_spacing = style.ItemInnerSpacing.x / RADIUS_MIN / 2;
            const float item_inner_ang_min = item_arc_span * ( item_n - 0.5f + inner_spacing );
            const float item_inner_ang_max = item_arc_span * ( item_n + 0.5f - inner_spacing );
            const float item_outer_ang_min = item_arc_span * ( item_n - 0.5f + inner_spacing * ( RADIUS_MIN / RADIUS_MAX ) );
            const float item_outer_ang_max = item_arc_span * ( item_n + 0.5f - inner_spacing * ( RADIUS_MIN / RADIUS_MAX ) );

            bool hovered = false;
            if ( drag_dist2 >= RADIUS_INTERACT_MIN * RADIUS_INTERACT_MIN )
            {
                if ( drag_angle >= item_inner_ang_min && drag_angle < item_inner_ang_max )
                    hovered = true;
            }
            bool selected = p_selected && ( *p_selected == item_n );

            static std::map<ImGuiID, float> hover_animation;
            auto                            it_hover = hover_animation.find( item_n );
            if ( it_hover == hover_animation.end( ) )
            {
                hover_animation.insert( { item_n, 0.f } );
                it_hover = hover_animation.find( item_n );
            }
            it_hover->second = ImClamp( it_hover->second + ( 3.f * GetIO( ).DeltaTime * ( hovered ? 1.f : -1.f ) ), 0.0f, 1.f );

            int arc_segments = ( int )( 32 * item_arc_span / ( 2 * IM_PI ) ) + 1;
            draw_list->PathArcTo( center, RADIUS_MAX - style.ItemInnerSpacing.x, item_outer_ang_min, item_outer_ang_max, arc_segments );
            draw_list->PathArcTo( center, RADIUS_MIN + style.ItemInnerSpacing.x, item_inner_ang_max, item_inner_ang_min, arc_segments );

            draw_list->PathFillConvex( hovered ? GetColorU32( ImVec4( m_vecColor.x, m_vecColor.y, m_vecColor.z, std::min( m_vecColor.w, it_hover->second ) ) ) : selected ? GetColorU32( ImVec4( m_vecColor.x, m_vecColor.y, m_vecColor.z, std::min( m_vecColor.w,  1.f ) ) ) : ImGuiCol_ChildBg );

            if ( CIconParser::Get( ).GetIcon( m_iWeapon[ item_n ] )->HasTexture( ) )
            {
                auto   text_pos = ImVec2(
                    center.x + cosf( ( item_inner_ang_min + item_inner_ang_max ) * 0.5f ) * ( RADIUS_MIN + RADIUS_MAX ) * 0.5f - static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ item_n ] )->m_iWidth * 0.5f ),
                    center.y + sinf( ( item_inner_ang_min + item_inner_ang_max ) * 0.5f ) * ( RADIUS_MIN + RADIUS_MAX ) * 0.5f - static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ item_n ] )->m_iHeight * 0.5f ) );

                draw_list->AddImage( CIconParser::Get( ).GetIcon( m_iWeapon[ item_n ] )->m_texture,
                    ImVec2( text_pos.x, text_pos.y ),
                    ImVec2( text_pos.x + static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ item_n ] )->m_iWidth ), text_pos.y + static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ item_n ] )->m_iHeight ) ) );
            }

            if ( hovered )
                item_hovered = item_n;
        }
        draw_list->PopClipRect( );

        if ( IsMouseReleased( 0 ) )
        {
            CloseCurrentPopup( );
            ret = item_hovered;
            if ( p_selected )
                *p_selected = item_hovered;
        }
        EndPopup( );
    }
    PopStyleColor( 2 );
    return ret;
}

void ImGui::PushDisabled( bool disable ) {
    PushItemFlag( ImGuiItemFlags_Disabled, disable );

    if ( disable )
        PushStyleVar( ImGuiStyleVar_Alpha, ImGui::GetStyle( ).Alpha * 0.5f );
}

void ImGui::PopDisabled( bool disable )
{
    if ( disable )
        PopStyleVar( );

    PopItemFlag( );
}