#include "setup.h"
#include "../common.h"
#include "../Interfaces/Interfaces.h"
#include "../hooks/hooks.h"
#include "../config/variables.h"
#include "../utilities/inputhandler.h"
#include "../utilities/netvar/netvar.h"
#include <mmsystem.h>

#pragma comment( lib, "winmm.lib" )

FILE* pStream;

bool AttachConsole(const char* szConsoleTitle)
{
	// allocate memory for console
	if (!AllocConsole())
		return false;

	// attach console to current process
	AttachConsole(ATTACH_PARENT_PROCESS);

	/*
	 * redirect cout stdin - to read / stdout - to write in console window
	 * cuz we doesn't need read anything from console we use only write stream
	 */
	if (freopen_s(&pStream, XorStr("CONOUT$"), XorStr("w"), stdout) != 0)
		return false;

	// set console window title
	if (!SetConsoleTitleA(szConsoleTitle))
		return false;

	return true;
}

void DetachConsole()
{
	// close write console stream
	fclose(pStream);

	// free allocated memory
	FreeConsole();

	// get console window
	if (const auto hConsoleWnd = GetConsoleWindow(); hConsoleWnd != nullptr)
		// close console window
		PostMessageW(hConsoleWnd, WM_CLOSE, 0U, 0L);
}

void Setup::OnStartup( )
{
    // wait for serverbrowser.dll to be loaded
    while ( Memory::GetModuleBaseHandle( SERVERBROWSER_DLL ) == nullptr )
        std::this_thread::sleep_for( 200ms );

    NTFunctions::SetupNTFunctions();

    // initialize logging
	AttachConsole("csgo-cheat");

    // get interface pointers
    if ( !Interfaces::Setup( ) )
        throw std::runtime_error( XorStr( "failed to capture Interfaces" ) );

    if ( !CNetvarManager::Get().Setup( XorStr( "netvars.txt" ) ) )
        throw std::runtime_error( XorStr( "failed to initialize netvars" ) );

    if ( !InputHandler::Setup( ) )
        throw std::runtime_error( XorStr( "failed to set window messages processor" ) );

    Hooks::SetupHooks();

    // print success & version check
    LOG( XorStr( "setup complete" ), LOG_MESSAGE);
}

void Setup::OnShutdown( )
{
    Globals::m_bIsUnloading = true;

    InputHandler::Restore( );

    Hooks::OnShutdown();

	DetachConsole( );
}