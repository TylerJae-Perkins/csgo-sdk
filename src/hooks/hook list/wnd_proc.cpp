#include "../hooks.h"
#include "../../gui/gui.h"
#include "../../utilities/inputhandler.h"
#include "../../config/variables.h"

long CALLBACK Hooks::hkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // process keys
    InputHandler::Process( uMsg, wParam, lParam );

    // switch window state on key click
    if ( Variables::m_iMenuKey > 0 && InputHandler::IsKeyReleased( Variables::m_iMenuKey ) )
        gui::m_bIsOpen = !gui::m_bIsOpen;

    if ( gui::m_bIsOpen )
    {
        ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam );

        if ( wParam != 'R' && wParam != '1' && wParam != '2' && wParam != '3' && wParam != '4' && wParam != '5' && wParam != 'W' && wParam != 'A' && wParam != 'S' && wParam != 'D' && wParam != VK_SHIFT && wParam != VK_CONTROL && wParam != VK_TAB && wParam != VK_SPACE || ImGui::GetIO( ).WantTextInput )
            return true;
    }

    if ( gui::m_bIsOpen && ( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_MOUSEMOVE ) )
        return false;


    //We should really call this somewhere else as this runs outside the games main thread and therefore could cause random crashes \
    if we use it and the game hasnt created the memory address for it or similar issues, however it should be fine considering its only \
    used once and not again :) 
    static bool m_bOnce = false;
    if ( !m_bOnce )
    {
        std::ostringstream m_szWelcomeMsg;
        m_szWelcomeMsg << "Open menu by pressing \"";
        m_szWelcomeMsg << Interfaces::m_pInputSystem->VirtualKeyToString( Variables::m_iMenuKey );
        m_szWelcomeMsg << "\" on your keyboard.\n";
       // m_szWelcomeMsg << "Configs and other resource files are stored in x.\n";
        Interfaces::m_pGameUI->CreateCommandMsgBox( "Welcome to csgo-cheat", 
            m_szWelcomeMsg.str( ).c_str( ), 
            true, 
            false, 
            nullptr, 
            nullptr, 
            nullptr, 
            nullptr, 
            nullptr );

        m_bOnce = true;
    }

    // disable game input when menu is opened
    //I::InputSystem->EnableInput( !W::bMainOpened );

    /*
     * @note: we can use imgui input handler to our binds if remove menu state check
     * with ImGui::IsKeyDown, ImGui::IsKeyPressed, etc functions
     * but imgui api's keys down durations doesnt have forward compatibility
     * and i dont want spend a lot of time on recode it
     */
    if ( gui::m_bInitialized && gui::m_bIsOpen && ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam ) )
        return 1L;

    // return input controls to the game
    return CallWindowProcW( InputHandler::m_pOldWndProc, hWnd, uMsg, wParam, lParam );
}
