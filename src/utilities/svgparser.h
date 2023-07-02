#pragma once
#include "../common.h"
#include "../interfaces/interfaces.h"

/* calculates the address of the function that the E8 call opcode calls */
std::uintptr_t FollowCall( std::uintptr_t pAddress )
{
	std::uintptr_t pBase = pAddress + 1;
	std::int32_t relAddress = *reinterpret_cast< std::int32_t* >( pBase );
	return pAddress + 5 + relAddress;
}

class PanoramaImageLoader
{
public:
	PanoramaImageLoader( size_t size )
	{
		std::memset( data, 0x0, 0x2C );

		static std::uintptr_t constructorEnd = Memory::FindPattern( PANORAMA_DLL, XorStr( "E8 ? ? ? ? 83 7D 20 00" ) );
		*( std::uintptr_t* )( std::uintptr_t( this ) + 0x24 ) = *( std::uintptr_t* )( constructorEnd - 12 );
		*( std::uintptr_t* )( std::uintptr_t( this ) + 0x28 ) = *( std::uintptr_t* )( constructorEnd - 4 );

		static auto fnInit = reinterpret_cast< void( __thiscall* )( void*, size_t ) >( FollowCall( constructorEnd ) );
		fnInit( this, 4 * size );
	}

	~PanoramaImageLoader( ) { Release( ); }

	void Release( )
	{
		if ( *reinterpret_cast< int* >( std::uintptr_t( data ) + 0x8 ) >= 0 )
		{
			if ( GetImageData( ) )
			{
				Memory::CallVFunc<void>( Interfaces::m_pMemAlloc, 5, GetImageData( ) );
				GetImageData( ) = nullptr;
			}

			GetImageDataSize( ) = 0;
		}
	}

	unsigned char*& GetImageData( )
	{
		return *reinterpret_cast< unsigned char** >( data );
	}

	size_t& GetImageDataSize( )
	{
		return *reinterpret_cast< size_t* >( std::uintptr_t( data ) + 0x4 );
	}

	bool ParseSVG( const std::string& svg, int* ptrWidth, int* ptrHeight, float scale )
	{
		static std::uintptr_t fnGetImageDataRGBASVG = Memory::FindPattern( PANORAMA_DLL, XorStr( "55 8B EC 81 EC ? ? ? ? 53 8B 5D 0C" ) );

		const char* svgText = svg.c_str( );
		size_t svgLength = svg.size( );

		bool res = false;
		__asm
		{
			push 0
			push scale
			push ptrHeight
			push ptrWidth
			push this
			mov ecx, svgText
			mov edx, svgLength
			call fnGetImageDataRGBASVG
			add esp, 0x14
			mov res, al
		}

		return res;
	}

private:
	char data[ 0x2C ];
};