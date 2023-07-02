#include <thread>

#include "inputhandler.h"
#include "../gui/gui.h"
#include "../hooks/hooks.h"

bool InputHandler::Setup( )
{
    D3DDEVICE_CREATION_PARAMETERS creationParameters = {};
    while ( FAILED( Interfaces::m_pDirectDevice->GetCreationParameters( &creationParameters ) ) )
        std::this_thread::sleep_for( 200ms );

    m_hWindow = creationParameters.hFocusWindow;

    if ( m_hWindow == nullptr )
        return false;

    m_pOldWndProc = reinterpret_cast< WNDPROC >( SetWindowLongPtrW( m_hWindow, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( Hooks::hkWndProc ) ) );

    if ( m_pOldWndProc == nullptr )
        return false;

    LOG( XorStr( "setup input system" ), LOG_MESSAGE );
    return true;
}

void InputHandler::Restore( )
{
    if ( m_pOldWndProc != nullptr )
    {
        SetWindowLongPtrW( m_hWindow, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( m_pOldWndProc ) );
        m_pOldWndProc = nullptr;
    }

    // reset input state
    Interfaces::m_pInputSystem->EnableInput( true );
}

bool InputHandler::Process( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // prevent process when e.g. binding something in-menu
    if ( gui::m_bIsOpen && wParam != Variables::m_iMenuKey && wParam != Variables::m_iUnloadKey )
        return false;

    // current active key
    int nKey = 0;
    // current active key state
    auto state = EKeyState::NONE;

    switch ( uMsg )
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if ( wParam < 256U )
        {
            nKey = wParam;
            state = EKeyState::DOWN;
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        if ( wParam < 256U )
        {
            nKey = wParam;
            state = EKeyState::UP;
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
        nKey = VK_LBUTTON;
        state = uMsg == WM_LBUTTONUP ? EKeyState::UP : EKeyState::DOWN;
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
        nKey = VK_RBUTTON;
        state = uMsg == WM_RBUTTONUP ? EKeyState::UP : EKeyState::DOWN;
        break;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
        nKey = VK_MBUTTON;
        state = uMsg == WM_MBUTTONUP ? EKeyState::UP : EKeyState::DOWN;
        break;
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_XBUTTONDBLCLK:
        nKey = ( GET_XBUTTON_WPARAM( wParam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
        state = uMsg == WM_XBUTTONUP ? EKeyState::UP : EKeyState::DOWN;
        break;
    default:
        return false;
    }

    // save key states
    if ( state == EKeyState::UP && m_arrKeyState.at( nKey ) == EKeyState::DOWN ) // if swap states it will be pressed state
        m_arrKeyState.at( nKey ) = EKeyState::RELEASED;
    else
        m_arrKeyState.at( nKey ) = state;

    return true;
}
