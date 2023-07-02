#include "gui.h"
#include "../interfaces/interfaces.h"
#include "compressedfonts.h"
#include "../utilities/iconparser.h"

void gui::Initialize( IDirect3DDevice9* m_pDevice, unsigned int m_uFontFlags )
{
    ImGui::CreateContext( );

    // setup platform and renderer bindings
    ImGui_ImplWin32_Init( InputHandler::m_hWindow );
    ImGui_ImplDX9_Init( m_pDevice );

    // setup styles
#pragma region draw_style
    ImGuiStyle& style = ImGui::GetStyle( );
    style.Alpha = 1.0f;
    style.WindowPadding = ImVec2( 8, 8 );
    style.WindowRounding = 7.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2( 32, 32 );
    style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
    style.ChildRounding = 7.f;
    style.ChildBorderSize = 0.5f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2( 4, 2 );
    style.FrameRounding = 0.f;
    style.FrameBorderSize = 1.0f;
    style.ItemSpacing = ImVec2( 8, 8 );
    style.ItemInnerSpacing = ImVec2( 4, 4 );
    style.IndentSpacing = 6.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 6.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 0.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 0.f;
    style.TabBorderSize = 1.0f;
    style.ButtonTextAlign = ImVec2( 0.5f, 0.5f );
    style.SelectableTextAlign = ImVec2( 0.0f, 0.5f );
    style.MouseCursorScale = 1.f;
    //style.AntiAliasedFill = false;
    //style.AntiAliasedLines = false;
#pragma endregion

#pragma region draw_style_color

    style.Colors[ ImGuiCol_Text ] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f ); // primtv 0
    style.Colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.75f, 0.75f, 0.75f, 0.85f ); // primtv 2
    style.Colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.1f, 0.1f, 0.1f, 0.85f );    // accent 1

    style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );   // primtv 3
    style.Colors[ ImGuiCol_ChildBg ] = ImVec4( 0.09f, 0.09f, 0.09f, 0.60f );   // primtv 1
    style.Colors[ ImGuiCol_PopupBg ] = ImVec4( 0.025f, 0.025f, 0.025f, 0.5f ); // primtv 1

    style.Colors[ ImGuiCol_Border ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.10f ); // primtv 4
    style.Colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f ); // clear

    style.Colors[ ImGuiCol_FrameBg ] = ImVec4( 0.1f, 0.1f, 0.1f, 1.00f ); // primtv 3
    style.Colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.1f, 0.1f, 0.1f, 0.1f );  // accent 1
    style.Colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.21f, 0.f, 1.f, 1.f );    // accent 0

    style.Colors[ ImGuiCol_ControlBg ] = ImVec4( 0.2f, 0.2f, 0.2f, 0.2f );     // primtv 3
    style.Colors[ ImGuiCol_ControlBgHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f ); // primtv 5
    style.Colors[ ImGuiCol_ControlBgActive ] = ImVec4( 0.75f, 0.75f, 0.75f, 0.10f ); // primtv 2

    style.Colors[ ImGuiCol_TitleBg ] = ImVec4( 0.1f, 0.1f, 0.1f, 1.00f ); // accent 0
    style.Colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.1f, 0.1f, 0.1f, 1.00f ); // accent 1
    style.Colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.1f, 0.1f, 0.1f, 1.00f ); // accent 1

    style.Colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.3f, 0.3f, 0.3f, 0.1f ); // primtv 1

    style.Colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.1f, 0.1f, 0.1f, 1.00f );                  // primtv 3
    style.Colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0 / 255.f, 158 / 255.f, 100 / 255.f, 1.f ); // accent 3
    style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 0.90f );               // primtv 5
    style.Colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.75f, 0.75f, 0.75f, 0.10f );               // primtv 2

    style.Colors[ ImGuiCol_CheckMark ] = ImVec4( 0 / 255.f, 110 / 255.f, 62 / 255.f, 1.f ); // accent 0

    style.Colors[ ImGuiCol_SliderGrab ] = ImVec4( 0 / 255.f, 158 / 255.f, 100 / 255.f, 1.f );  // accent 0
    style.Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0 / 255.f, 110 / 255.f, 62 / 255.f, 1.00f ); // accent 1

    style.Colors[ ImGuiCol_Button ] = ImVec4( 0.1f, 0.1f, 0.1f, 1.00f );    // primtv 3
    style.Colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f ); // primtv 5
    style.Colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.21f, 0.f, 1.f, 1.f );       // accent 0

    style.Colors[ ImGuiCol_Header ] = ImVec4( 0.35f, 0.35f, 0.35f, 1.f );   // accent 0
    style.Colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f ); // primtv 5
    style.Colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.1f, 0.1f, 0.1f, 1.00f );    // primtv 3

    style.Colors[ ImGuiCol_Separator ] = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f ); // primtv 3
    style.Colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f ); // primtv 5
    style.Colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.21f, 0.f, 1.f, 1.f );       // accent 0

    style.Colors[ ImGuiCol_ResizeGrip ] = ImVec4( 0.21f, 0.f, 1.f, 1.f );       // accent 0
    style.Colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 0.70f ); // primtv 5
    style.Colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.1f, 0.1f, 0.1f, 1.00f );    // accent 1

    style.Colors[ ImGuiCol_Tab ] = ImVec4( 0.f, 0.f, 0.f, 1.f );         // primtv 1
    style.Colors[ ImGuiCol_TabHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 0.80f ); // primtv 5
    style.Colors[ ImGuiCol_TabActive ] = ImVec4( 0.21f, 0.f, 1.f, 1.f );       // accent 0
    style.Colors[ ImGuiCol_TabUnfocused ] = ImVec4( 0.30f, 0.30f, 0.30f, 0.70f ); // primtv 5
    style.Colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.21f, 0.f, 1.f, 1.f );       // accent 0

    style.Colors[ ImGuiCol_PlotLines ] = ImVec4( 0.f, 0.43f, 0.25f, 1.f ); // accent 0
    style.Colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 0.f, 0.43f, 0.25f, 1.f ); // accent 0
    style.Colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.f, 0.43f, 0.25f, 1.f ); // accent 0
    style.Colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 0.f, 0.43f, 0.25f, 1.f ); // accent 0

    style.Colors[ ImGuiCol_DragDropTarget ] = ImVec4( 0.21f, 0.f, 1.f, 1.f ); // accent 3
    style.Colors[ ImGuiCol_Triangle ] = ImVec4( 0.21f, 0.f, 1.f, 1.f ); // accent 0

    style.Colors[ ImGuiCol_ModalWindowDimBg ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.25f ); // primtv 4

    style.Colors[ ImGuiCol_Mouse ] = ImVec4( 1.f, 1.f, 1.f, 1.f ); // accent 0

#pragma endregion

    // create fonts
    ImGuiIO& io = ImGui::GetIO( );

    //Montserrat
    ImFontConfig imMontserratConfig = {};
    imMontserratConfig.RasterizerFlags = ImGuiFreeType::LightHinting;
    Fonts::Montserrat = io.Fonts->AddFontFromMemoryCompressedTTF( montserrat_compressed_data, montserrat_compressed_size, 16.f, &imMontserratConfig, io.Fonts->GetGlyphRangesCyrillic( ) );
    Fonts::MontserratS = io.Fonts->AddFontFromMemoryCompressedTTF( montserrat_compressed_data, montserrat_compressed_size, 14.f, &imMontserratConfig, io.Fonts->GetGlyphRangesCyrillic( ) );
    Fonts::MontserratM = io.Fonts->AddFontFromMemoryCompressedTTF( montserrat_compressed_data, montserrat_compressed_size, 18.f, &imMontserratConfig, io.Fonts->GetGlyphRangesCyrillic( ) );
    Fonts::MontserratL = io.Fonts->AddFontFromMemoryCompressedTTF( montserrat_compressed_data, montserrat_compressed_size, 22.f, &imMontserratConfig, io.Fonts->GetGlyphRangesCyrillic( ) );
    Fonts::MontserratXL = io.Fonts->AddFontFromMemoryCompressedTTF( montserrat_compressed_data, montserrat_compressed_size, 30.f, &imMontserratConfig, io.Fonts->GetGlyphRangesCyrillic( ) );

    //Verdana
    ImFontConfig imVerdanaConfig = {};
    imVerdanaConfig.RasterizerFlags = ImGuiFreeType::LightHinting | ImGuiFreeType::Bold;
    Fonts::Verdana = io.Fonts->AddFontFromFileTTF( XorStr( "C:\\Windows\\Fonts\\Verdana.ttf" ), 16.f, &imVerdanaConfig, io.Fonts->GetGlyphRangesCyrillic( ) );

    //Icons
    ImFontConfig imIconsConfig = {};
    imIconsConfig.RasterizerFlags = ImGuiFreeType::LightHinting;
    constexpr ImWchar wIconRanges[ ] =
    {
        0xE000, 0xF8FF, // Private Use Area
        0
    };

    Fonts::Icons = io.Fonts->AddFontFromMemoryCompressedTTF( iconfont_compressed_data, iconfont_compressed_size, 17.f, &imIconsConfig, io.Fonts->GetGlyphRangesDefault( ) );
    Fonts::IconsS = io.Fonts->AddFontFromMemoryCompressedTTF( iconfont_compressed_data, iconfont_compressed_size, 14.f, &imIconsConfig, io.Fonts->GetGlyphRangesDefault( ) );
    Fonts::IconsL = io.Fonts->AddFontFromMemoryCompressedTTF( iconfont_compressed_data, iconfont_compressed_size, 20.f, &imIconsConfig, io.Fonts->GetGlyphRangesDefault( ) );

    m_bInitialized = ImGuiFreeType::BuildFontAtlas( io.Fonts, m_uFontFlags );
}

void gui::RenderGui( IDirect3DDevice9* m_pDevice )
{
    if ( Interfaces::m_pEngine->IsTakingScreenshot( ) || Interfaces::m_pEngine->IsDrawingLoadingImage( ) )
        return;

    m_pCurrentDevice = m_pDevice;
    ImGuiIO& m_io = ImGui::GetIO( );
    const ImVec2 m_vecScreenSize = m_io.DisplaySize;

    const ImGuiStyle& m_style = ImGui::GetStyle( );
    ImDrawList* m_pForegroundDrawList = ImGui::GetForegroundDrawList( );
    ImDrawList* m_pBackgroundDrawList = ImGui::GetBackgroundDrawList( );
 
    m_io.MouseDrawCursor = gui::m_bIsOpen;
    if ( gui::m_bIsOpen )
    {
        int x, y;
        Interfaces::m_pInputSystem->GetCursorPosition( &x, &y );
        // set imgui mouse position
        m_io.MousePos = ImVec2( static_cast< float >( x ), static_cast< float >( y ) );
    }

    static float m_flAlpha = 0.0002f;
    m_flAlpha = std::clamp( m_flAlpha + ( 3.f * ImGui::GetIO( ).DeltaTime * ( gui::m_bIsOpen ? 1.f : -1.f ) ), 0.0001f, 1.f );
    const ImVec4 m_veMenuTheme = ImVec4( Variables::m_colMenuTheme.Base<COLOR_R>( ), Variables::m_colMenuTheme.Base<COLOR_G>( ), Variables::m_colMenuTheme.Base<COLOR_B>( ), std::min( m_flAlpha, Variables::m_colMenuTheme.Base<COLOR_A>( ) ) );

    gui::RenderWatermark( m_pForegroundDrawList, m_vecScreenSize, m_veMenuTheme );

    // render menu
    if ( m_flAlpha > 0.0001f )
    {
        // add background
        m_pBackgroundDrawList->AddRectFilled( ImVec2( 0.f, 0.f ), m_vecScreenSize, ImColor( 0.01f, 0.01f, 0.01f, m_flAlpha * 0.5f ) );

        RenderMainWindow( m_vecScreenSize, m_veMenuTheme, m_flAlpha );
    } 
}

void gui::RenderWatermark( ImDrawList* m_pDrawList, const ImVec2 m_vecScreenSize, ImVec4 m_vecMenuTheme )
{
    ImGui::PushFont( Fonts::MontserratL );

    const std::string m_strTime = std::vformat( XorStr( "{:%d-%m-%Y %X} " ), std::make_format_args( std::chrono::system_clock::now( ) ) );
    const std::string m_strFirstPart = XorStr( " pulse" );
    const std::string m_strSecondPart = XorStr( "fire | " ) + m_strTime;
    const std::string m_strWatermarkLength = m_strFirstPart + m_strSecondPart;
    const ImVec2 m_vecTextSizeFirstPart = ImGui::CalcTextSize( m_strFirstPart.c_str( ) );
    const ImVec2 m_vecTextSize = ImGui::CalcTextSize( m_strWatermarkLength.c_str( ) );
    
    m_pDrawList->AddRectFilled( ImVec2( m_vecScreenSize.x - m_vecTextSize.x, 1.f ), ImVec2( m_vecScreenSize.x - 1.f, m_vecTextSize.y + 7.f ), ImColor( 0.09f, 0.09f, 0.09f, 1.f ), 4.f );
    m_pDrawList->AddRect( ImVec2( m_vecScreenSize.x - m_vecTextSize.x - 1.f, 0.f ), ImVec2( m_vecScreenSize.x, m_vecTextSize.y + 7.f + 1.f ), ImColor( m_vecMenuTheme.x, m_vecMenuTheme.y, m_vecMenuTheme.z, 1.f ), 4.f );
    m_pDrawList->AddText( ImVec2( m_vecScreenSize.x - m_vecTextSize.x, 4.f ), ImColor( m_vecMenuTheme.x, m_vecMenuTheme.y, m_vecMenuTheme.z, 1.f ), m_strFirstPart.c_str( ) );
    m_pDrawList->AddText( ImVec2( ( m_vecScreenSize.x - m_vecTextSize.x ) + m_vecTextSizeFirstPart.x, 4.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), m_strSecondPart.c_str( ) );

    ImGui::PopFont( );
}

void AlignForWidth( float width, float alignment = 0.5f )
{
    ImGuiStyle& style = ImGui::GetStyle( );
    float avail = ImGui::GetContentRegionAvail( ).x;
    float off = ( avail - width ) * alignment;
    if ( off > 0.0f )
        ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + off );
}

void gui::RenderMainWindow( ImVec2 m_vecScreenSize, ImVec4 m_vecMenuTheme, float m_flAlpha )
{
    static float m_flOldAlpha;
    static ImVec4 m_vecOldMenuTheme;
    static ImGuiStyle& m_style = ImGui::GetStyle( );
    // @NOTE: this is a shit way to update the menu theme, but it works.
    // Please don't beat me for the horrible color check
    if ( m_flAlpha != m_flOldAlpha || 
        m_vecMenuTheme.x != m_vecOldMenuTheme.x || 
        m_vecMenuTheme.y != m_vecOldMenuTheme.y || 
        m_vecMenuTheme.z != m_vecOldMenuTheme.z || 
        m_vecMenuTheme.y != m_vecOldMenuTheme.y )
    { 
        m_style.Alpha = m_flAlpha;
        m_style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 0.09f, 0.09f, 0.09f, m_flAlpha );
        m_style.Colors[ ImGuiCol_Mouse ] = m_vecMenuTheme;
        m_style.Colors[ ImGuiCol_CheckMark ] = m_vecMenuTheme;
        m_style.Colors[ ImGuiCol_SliderGrab ] = m_vecMenuTheme;
        m_style.Colors[ ImGuiCol_FrameBg ] = m_vecMenuTheme;
        m_style.Colors[ ImGuiCol_BorderWindow ] = m_vecMenuTheme;
        m_flOldAlpha = m_flAlpha;
        m_vecOldMenuTheme = m_vecMenuTheme;
    }

    static int  m_iTab = Tabs::m_iActiveTab;
    static bool m_bFadePreview = false;

    if ( m_iTab != Tabs::m_iActiveTab || m_bFadePreview )
    {
        if ( !m_bFadePreview )
        {
            if ( Tabs::m_flPreviousAlpha == 1.f ) //-V550
                m_bFadePreview = true;

            Tabs::m_flPreviousAlpha = std::clamp( Tabs::m_flPreviousAlpha + ( 4.f * ImGui::GetIO( ).DeltaTime ), 0.01f, 1.f );
        }
        else
        {
            m_iTab = Tabs::m_iActiveTab;
            if ( Tabs::m_flPreviousAlpha == 0.01f ) //-V550
            {
                m_bFadePreview = false;
            }

            Tabs::m_flPreviousAlpha = std::clamp( Tabs::m_flPreviousAlpha - ( 4.f * ImGui::GetIO( ).DeltaTime ), 0.01f, 1.f );
        }
    }
    else
        Tabs::m_flPreviousAlpha = std::clamp( Tabs::m_flPreviousAlpha - ( 4.f * ImGui::GetIO( ).DeltaTime ), 0.0f, 1.f );

    ImGui::PushStyleColor( ImGuiCol_ChildBg, ImGui::GetColorU32( ImGuiCol_WindowBg ) );
    ImGui::PushStyleColor( ImGuiCol_Button, ImGui::GetColorU32( ImGuiCol_WindowBg ) );
    
    ImGui::PushStyleVar( ImGuiStyleVar_PopupRounding, 4.5f );
    ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 4.5f );
    ImGui::PushStyleVar( ImGuiStyleVar_Alpha, m_flAlpha );
    
    ImGui::PushFont( Fonts::MontserratM );

    ImGui::SetNextWindowPos( ImVec2( m_vecScreenSize.x * 0.5f, m_vecScreenSize.y * 0.5f ), ImGuiCond_Once, ImVec2( 0.5f, 0.5f ) );
    ImGui::SetNextWindowSize( ImVec2( 1024.f , 576.f ), ImGuiCond_Always );
    ImGui::Begin( XorStr( "csgo-cheat" ), &m_bIsOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
          const ImVec2 m_vecTextSize = ImGui::CalcTextSize( XorStr( "csgo-cheat" ) );
          ImGui::SetCursorPos( ImVec2( ImGui::GetContentRegionMax().x * 0.5f - ( m_vecTextSize.x * 0.5f ), -1.f ) );
          ImGui::PushFont( Fonts::MontserratM );
          ImGui::TextColored( m_vecMenuTheme, XorStr( "pulse" ) );
          ImGui::SameLine( 0.f, 0.f );
          ImGui::TextColored( ImVec4( 1.f, 1.f, 1.f, m_flAlpha ), XorStr( "fire" ) );
          ImGui::PopFont( );
          ImGui::EndMenuBar( );
        }

        ImGui::BeginChild( XorStr( "header" ), ImVec2( 0, 25 ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
        {  
            const static ImVec2 m_vecTabButtonSize = ImVec2( 160, 25 );

            ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 5, 0.f ) );
            ImGui::PushStyleVar( ImGuiStyleVar_TabRounding, 2.f );
            
            ImGui::PushFont( Fonts::MontserratM );

            float m_flWidth = 0.0f;
            m_flWidth += m_vecTabButtonSize.x * 6;
            m_flWidth += m_style.ItemSpacing.x * 6;
            AlignForWidth( m_flWidth );

            ImGui::SetCursorPosY( 0.f );
            if ( ImGui::TabButton( XorStr( "Legit" ), XorStr( "firstTab" ), m_vecTabButtonSize, false, true, Fonts::Icons, XorStr( "%" ), m_iTab == 0 && !m_bFadePreview, m_vecMenuTheme ) ) Tabs::m_iActiveTab = 0;
            ImGui::SameLine( );
            if ( ImGui::TabButton( XorStr( "Rage" ), XorStr( "secondTab" ), m_vecTabButtonSize, false, true, Fonts::Icons, XorStr( "&" ), m_iTab == 1 && !m_bFadePreview, m_vecMenuTheme ) ) Tabs::m_iActiveTab = 1;
            ImGui::SameLine( );
            if ( ImGui::TabButton( XorStr( "Anti-Aim" ), XorStr( "thirdTab" ), m_vecTabButtonSize, false, true, Fonts::Icons, XorStr( "(" ), m_iTab == 2 && !m_bFadePreview, m_vecMenuTheme ) ) Tabs::m_iActiveTab = 2;
            ImGui::SameLine( );
            if ( ImGui::TabButton( XorStr( "Visuals" ), XorStr( "fourthTab" ), m_vecTabButtonSize, false, true, Fonts::Icons, XorStr( "#" ), m_iTab == 3 && !m_bFadePreview, m_vecMenuTheme ) ) Tabs::m_iActiveTab = 3;
            ImGui::SameLine( );
            if ( ImGui::TabButton( XorStr( "Misc" ), XorStr( "fithtab" ), m_vecTabButtonSize, false, true, Fonts::Icons, XorStr( "4" ), m_iTab == 4 && !m_bFadePreview, m_vecMenuTheme ) ) Tabs::m_iActiveTab = 4;
            ImGui::SameLine( );
            if ( ImGui::TabButton( XorStr( "Skins" ), XorStr( "sixthtab" ), m_vecTabButtonSize, false, true, Fonts::Icons, XorStr( "4" ), m_iTab == 5 && !m_bFadePreview, m_vecMenuTheme ) ) Tabs::m_iActiveTab = 5;

            ImGui::PopFont( );
            ImGui::PopStyleVar( 2 );

            ImGui::EndChild( );
        }

        ImGui::PopStyleColor(2);

        ImGui::BeginChild( XorStr( "maincontent" ), ImVec2( 0, 0 ), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar );
        {
            ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ( 1.f - Tabs::m_flPreviousAlpha ) * m_flAlpha );
            ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 5, 8.f ) );
            ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( m_style.FramePadding.x, 1.f ) );

            switch ( m_iTab )
            {
            case 0:
                Tabs::LegitBot( m_vecMenuTheme );
                break;
            case 1:
                Tabs::RageBot( m_vecMenuTheme );
                break;
            case 2:
                Tabs::AntiAim( m_vecMenuTheme );
                break;
            case 3:
                Tabs::Visuals( m_vecMenuTheme );
                break;
            case 4:
                Tabs::Miscellaneous( m_vecMenuTheme );
                break;
            case 5:
                Tabs::SkinChanger( m_vecMenuTheme );
                break;
            }

            ImGui::PopStyleVar( 3 );
            ImGui::EndChild( );
        }

        ImGui::PopFont( );
        ImGui::PopStyleVar( 3 );
        ImGui::End( );
    }
}

static int m_iTest = 0;
const char* arrAimingConditionsLegit[ ] =
{
    "Aim through wall",
    "Aim through smoke",
    "Aim while flashed"
};

static std::vector<bool>TestVecBool( 3 );
static Color testcol = Color( 255, 255, 255, 255 );
static int testkey = 0;

void Tabs::LegitBot( ImVec4 m_vecMenuTheme )
{
    ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 0.07f, 0.07f, 0.07f, 1.f ) );
    ImGui::BeginChild( XorStr( "MainSettings" ), ImVec2( ImGui::GetContentRegionAvail( ).x * 0.5f, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushFont( Fonts::MontserratM );

            ImGui::Text( XorStr( "Legit-Bot" ) );

            ImGui::PopFont( );
            ImGui::EndMenuBar( );
        }

        ImGui::Checkbox( "Enable", &Variables::m_bLegitbotEnabled );

        ImGui::PushDisabled( !Variables::m_bLegitbotEnabled );

        ImGui::Text( "Test Text" );
        
        ImGui::FormattedCombo( "Test combo", &m_iTest, "Disabled\0Enabled\0I'm gay\0\0" );
        
        ImGui::MultiCombo( "Test multi combo", arrAimingConditionsLegit, TestVecBool, IM_ARRAYSIZE( arrAimingConditionsLegit ) );
        
        ImGui::CustomButton( "Test button", "TestButton", ImVec2(0, 0), m_vecMenuTheme );
        
        ImGui::Text( "Test colorpicker" );
        ImGui::ColorPickerBox( XorStr( "##coltest" ), &testcol, 45.f, true, m_vecMenuTheme );

        ImGui::Text( "test hotkey" );
        ImGui::HotKey( "test key", &testkey, m_vecMenuTheme );

        ImGui::PopDisabled( !Variables::m_bLegitbotEnabled );

        ImGui::EndChild( );
    }

    ImGui::SameLine( );
    ImVec2 m_vecSavedPosition = ImVec2( ImGui::GetCursorPos( ).x, ImGui::GetCursorPos( ).y + ImGui::GetContentRegionAvail( ).y / 2 + ImGui::GetStyle( ).WindowPadding.y + 2.f );

    ImGui::BeginChild( XorStr( "WeaponConfig" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y * 0.5f ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        ImGui::PushDisabled( !Variables::m_bLegitbotEnabled );

        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Weapons" ) );

            const char* m_szItems[ ] = { XorStr( "Pistols" ), XorStr( "Deagle" ), XorStr( "SMGs" ), XorStr( "Rifles" ), XorStr( "Shotguns" ), XorStr( "Snipers" ), XorStr( "LMGs" )};
            const int m_iWeapon[  ] = { WEAPON_USP_SILENCER, WEAPON_DEAGLE, WEAPON_UMP45, WEAPON_AK47, WEAPON_NOVA, WEAPON_AWP, WEAPON_M249 };
            
            ImGui::SetCursorPosX( ImGui::GetWindowContentRegionWidth() - static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iLegitBotWeapon ] )->m_iWidth ) );
            if ( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iLegitBotWeapon ] )->HasTexture( ) )
                ImGui::ImageButton( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iLegitBotWeapon ] )->m_texture,
                    "weaponselectlegit",
                    ImVec2( static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iLegitBotWeapon ] )->m_iWidth ), static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iLegitBotWeapon ] )->m_iHeight ) ) );

            if ( Variables::m_iLegitBotWeapon < 0 || Variables::m_iLegitBotWeapon > 7 )
                Variables::m_iLegitBotWeapon = 0;

            if ( ImGui::IsItemActive( ) )
                ImGui::OpenPopup( XorStr( "##piepopuplegit" ) );

            ImVec2 m_vecPieCenter = ImGui::GetIO( ).MouseClickedPos[ 0 ];
            int n = ImGui::PiePopupSelectMenuWeapons( m_vecPieCenter, XorStr( "##piepopuplegit" ), m_szItems, 7, &Variables::m_iLegitBotWeapon, m_vecMenuTheme, m_iWeapon );


            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::PushFont( Fonts::MontserratM );

        static bool yes = false;
        ImGui::Checkbox( XorStr("Test"), &yes );

        ImGui::PopFont( );

        ImGui::PopDisabled( !Variables::m_bLegitbotEnabled );
        ImGui::EndChild( );
    }

    ImGui::SetCursorPos( m_vecSavedPosition );

    ImGui::BeginChild( XorStr( "Exploits" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        ImGui::PushDisabled( !Variables::m_bLegitbotEnabled );

        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Exploits" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }



        ImGui::PopDisabled( !Variables::m_bLegitbotEnabled );
        ImGui::EndChild( );
    }
    ImGui::PopStyleColor( 1 );
   
}

void Tabs::RageBot( ImVec4 m_vecMenuTheme )
{
    ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 0.07f, 0.07f, 0.07f, 1.f ) );
    ImGui::BeginChild( XorStr( "MainSettings" ), ImVec2( ImGui::GetContentRegionAvail( ).x * 0.5f, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "General" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::Checkbox( "Enable", &Variables::m_bRagebotEnabled );

        ImGui::PushDisabled( !Variables::m_bRagebotEnabled );

        ImGui::Text( "Test Text" );

        ImGui::PopDisabled( !Variables::m_bRagebotEnabled );

        ImGui::EndChild( );
    }

    ImGui::SameLine( );
    ImVec2 m_vecSavedPosition = ImVec2( ImGui::GetCursorPos( ).x, ImGui::GetCursorPos( ).y + ImGui::GetContentRegionAvail( ).y / 2 + ImGui::GetStyle( ).WindowPadding.y + 2.f );

    ImGui::BeginChild( XorStr( "WeaponConfig" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y * 0.5f ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        ImGui::PushDisabled( !Variables::m_bRagebotEnabled );

        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Weapons" ) );

            const char* m_szItems[ ] = { XorStr( "Pistols" ), XorStr( "Deagle" ), XorStr( "Scout" ), XorStr( "AWP" ), XorStr( "Auto" ), XorStr( "Other" )};
            const int m_iWeapon[ ] = { WEAPON_USP_SILENCER, WEAPON_DEAGLE, WEAPON_SSG08, WEAPON_AWP, WEAPON_SCAR20, WEAPON_AK47 };

            ImGui::SetCursorPosX( ImGui::GetWindowContentRegionWidth( ) - static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iRageBotWeapon ] )->m_iWidth ) );
            if ( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iRageBotWeapon ] )->HasTexture( ) )
                ImGui::ImageButton( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iRageBotWeapon ] )->m_texture,
                    "weaponselectrage",
                    ImVec2( static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iRageBotWeapon ] )->m_iWidth ), static_cast< float >( CIconParser::Get( ).GetIcon( m_iWeapon[ Variables::m_iRageBotWeapon ] )->m_iHeight ) ) );

            if ( Variables::m_iRageBotWeapon < 0 || Variables::m_iRageBotWeapon > 6 )
                Variables::m_iRageBotWeapon = 0;

            if ( ImGui::IsItemActive( ) )
                ImGui::OpenPopup( XorStr( "##piepopuprage" ) );

            ImVec2 m_vecPieCenter = ImGui::GetIO( ).MouseClickedPos[ 0 ];
            int n = ImGui::PiePopupSelectMenuWeapons( m_vecPieCenter, XorStr( "##piepopuprage" ), m_szItems, 6, &Variables::m_iRageBotWeapon, m_vecMenuTheme, m_iWeapon );


            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::PopDisabled( !Variables::m_bRagebotEnabled );
        ImGui::EndChild( );
    }

    ImGui::SetCursorPos( m_vecSavedPosition );

    ImGui::BeginChild( XorStr( "Exploits" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        ImGui::PushDisabled( !Variables::m_bRagebotEnabled );

        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Exploits" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::PopDisabled( !Variables::m_bRagebotEnabled );
        ImGui::EndChild( );
    }
    ImGui::PopStyleColor( 1 );
}

void Tabs::AntiAim( ImVec4 m_vecMenuTheme )
{
    ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 0.07f, 0.07f, 0.07f, 1.f ) );
    ImGui::BeginChild( XorStr( "MainSettings" ), ImVec2( ImGui::GetContentRegionAvail( ).x * 0.5f, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Anti-Aim" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::EndChild( );
    }

    ImGui::SameLine( );

    ImGui::BeginChild( XorStr( "Fakelag" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Fakelag" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::EndChild( );
    }
    ImGui::PopStyleColor( 1 );
}

void Tabs::Visuals( ImVec4 m_vecMenuTheme )
{
    ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 0.07f, 0.07f, 0.07f, 1.f ) );
    ImGui::BeginChild( XorStr( "Main settings" ), ImVec2( ImGui::GetContentRegionAvail( ).x * 0.5f, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::IconsS );
            ImGui::Text( XorStr( "4" ) );
            ImGui::PopFont( );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "ESP" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::EndChild( );
    }

    ImGui::SameLine( );
    ImVec2 m_vecSavedPosition = ImVec2( ImGui::GetCursorPos( ).x, ImGui::GetCursorPos( ).y + ImGui::GetContentRegionAvail( ).y / 2 + ImGui::GetStyle( ).WindowPadding.y + 2.f );

    ImGui::BeginChild( XorStr( "Models" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y * 0.5f ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::IconsS );
            ImGui::Text( XorStr( "'" ) );
            ImGui::PopFont( );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Models" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::EndChild( );
    }

    ImGui::SetCursorPos( m_vecSavedPosition );

    ImGui::BeginChild( XorStr( "Other" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushFont( Fonts::IconsS );
            ImGui::Text( XorStr( "|" ) );
            ImGui::PopFont( );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Misc" ) );
            ImGui::PopFont( );

            ImGui::EndMenuBar( );
        }

        ImGui::PushFont( Fonts::MontserratM );


        ImGui::PopFont( );

        ImGui::EndChild( );
    }
    ImGui::PopStyleColor( 1 );

}

void Tabs::Miscellaneous( ImVec4 m_vecMenuTheme )
{
    ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 0.07f, 0.07f, 0.07f, 1.f ) );
    ImGui::BeginChild( XorStr( "MainSettings" ), ImVec2( ImGui::GetContentRegionAvail( ).x * 0.5f, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::IconsS );
            ImGui::Text( XorStr( "4" ) );
            ImGui::PopFont( );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "General" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::EndChild( );
    }

    ImGui::SameLine( );
    ImVec2 m_vecSavedPosition = ImVec2( ImGui::GetCursorPos( ).x, ImGui::GetCursorPos( ).y + ImGui::GetContentRegionAvail( ).y / 2 + ImGui::GetStyle( ).WindowPadding.y + 2.f );

    ImGui::BeginChild( XorStr( "Movement" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y * 0.5f ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.5f, 0.5f ) );
            ImGui::PushFont( Fonts::IconsS );
            ImGui::Text( XorStr( "'" ) );
            ImGui::PopFont( );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Movement" ) );
            ImGui::PopFont( );
            ImGui::PopStyleVar( );
            ImGui::EndMenuBar( );
        }

        ImGui::EndChild( );
    }

    ImGui::SetCursorPos( m_vecSavedPosition );

    ImGui::BeginChild( XorStr( "Configs" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y ), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar );
    {
        if ( ImGui::BeginMenuBar( ) )
        {
            ImGui::PushFont( Fonts::IconsS );
            ImGui::SetCursorPosY( 5.f );
            ImGui::Text( XorStr( "|" ) );
            ImGui::PopFont( );
            ImGui::PushFont( Fonts::MontserratM );
            ImGui::Text( XorStr( "Configs" ) );
            ImGui::PopFont( );

            ImGui::EndMenuBar( );
        }

        ImGui::PushFont( Fonts::MontserratM );
        
        ImGui::Text( "Menu theme" );
        ImGui::ColorPickerBox( XorStr( "##colMenuTheme" ), &Variables::m_colMenuTheme, 45.f, false, m_vecMenuTheme );

        ImGui::Text( "Menu key" );
        ImGui::HotKey( "Menu key" , &Variables::m_iMenuKey, m_vecMenuTheme );

        ImGui::Text( "Unload key" );
        ImGui::HotKey( "Unload key", &Variables::m_iUnloadKey, m_vecMenuTheme );

        ImGui::PopFont( );

        ImGui::EndChild( );
    }
    ImGui::PopStyleColor( 1 );

}

void Tabs::SkinChanger( ImVec4 m_vecMenuTheme )
{


}