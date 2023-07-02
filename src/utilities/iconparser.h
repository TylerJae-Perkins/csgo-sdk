#pragma once
#include <unordered_map>
#include "../common.h"
#include "../valve/entity.h"

struct IconData
{
	LPDIRECT3DTEXTURE9 m_texture = nullptr;
	int m_iWidth = 0;
	int m_iHeight = 0;
	bool GenerateWeaponIconTexture( int iWeaponID );

	inline bool HasTexture( )
	{
		if ( this->m_texture != nullptr )
			return true;

		return false;
	}
};

class CIconParser : public CSingleton<CIconParser>
{
public:
	IconData* GetIcon( int m_iIndex );
	std::vector<std::uint8_t> GetFileData( const char* pFile );

	std::unordered_map<int, IconData> icons;
};