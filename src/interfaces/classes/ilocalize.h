#pragma once
#include "../../memory/memory.h"

class ILocalize
{
public:
	wchar_t* Find( const char* szTokenName )
	{
		return Memory::CallVFunc<wchar_t*>( this, 11, szTokenName );
	}

	const wchar_t* FindSafe( const char* szTokenName )
	{
		return Memory::CallVFunc<wchar_t*>( this, 12, szTokenName );
	}
};

struct IBaseFileSystem
{

	int Read( void* pOutput, int size, void* file )
	{
		return Memory::CallVFunc<int>( this, 0, pOutput, size, file );
	}

	// if pathID is NULL, all paths will be searched for the file
	void* Open( const char* pFileName, const char* pOptions, const char* pathID = nullptr )
	{
		return Memory::CallVFunc<void*>( this, 2, pFileName, pOptions, pathID );
	}

	void Close( void* file )
	{
		return Memory::CallVFunc<void>( this, 3, file );
	}

	unsigned int Size( void* file )
	{
		return Memory::CallVFunc<unsigned int>( this, 7, file );
	}
};