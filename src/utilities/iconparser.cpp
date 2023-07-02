#include "iconparser.h"
#include "../interfaces/interfaces.h"
#include "../utilities/svgparser.h"

IconData* CIconParser::GetIcon( int m_iIndex )
{
    if ( m_iIndex != 0 )
    {
        auto it = icons.find( m_iIndex );
        if ( it == icons.end( ) )
        {
            icons[ m_iIndex ] = IconData( );

            if ( !icons[ m_iIndex ].GenerateWeaponIconTexture( m_iIndex ) )
                return nullptr;

            return &icons[ m_iIndex ];
        }

        if ( !it->second.HasTexture( ) )
            return nullptr;

        return &it->second;
    }
    return nullptr;
}

std::vector<std::uint8_t> CIconParser::GetFileData( const char* pFile )
{
    void* pOpen = Interfaces::m_pFileSystem->Open( pFile, "r", "GAME" );
    if ( !pOpen )
        return std::vector<std::uint8_t>( );

    std::vector<uint8_t> vecData;
    vecData.resize( Interfaces::m_pFileSystem->Size( pOpen ) );

    Interfaces::m_pFileSystem->Read( vecData.data( ), vecData.size( ), pOpen );

    Interfaces::m_pFileSystem->Close( pOpen );

    return vecData;
}

bool IconData::GenerateWeaponIconTexture( int iWeaponID )
{
    std::string strWepName = Interfaces::m_pWeaponSystem->GetWeaponData( iWeaponID )->m_szConsoleName;
    strWepName.erase( 0, 7 );

    std::string strWepPath = "materials/panorama/images/icons/equipment/" + strWepName + ".svg";

    std::vector<uint8_t> vecPngData = CIconParser::Get( ).GetFileData( strWepPath.c_str( ) );
    if ( !vecPngData.empty( ) )
    {
        PanoramaImageLoader imageLoader( this->m_iWidth * this->m_iHeight );
        std::string strSvgData( vecPngData.begin( ), vecPngData.end( ) );
        bool bSuccess = imageLoader.ParseSVG( strSvgData, &this->m_iWidth, &this->m_iHeight, 0.6f );

        if ( bSuccess )
        {
            HRESULT hres = Interfaces::m_pDirectDevice->CreateTexture( this->m_iWidth,
                this->m_iHeight,
                1,
                D3DUSAGE_DYNAMIC,
                D3DFMT_A8R8G8B8,
                D3DPOOL_DEFAULT,
                &this->m_texture,
                nullptr );

            if ( hres != D3D_OK )
                return false;

            D3DLOCKED_RECT rect;
            hres = m_texture->LockRect( 0, &rect, nullptr, D3DLOCK_DISCARD );
            if ( hres != D3D_OK )
                return false;

            unsigned char* src = imageLoader.GetImageData( );
            uint8_t* dst = reinterpret_cast< uint8_t* >( rect.pBits );
            for ( int y = 0; y < this->m_iHeight; ++y )
            {
                std::copy( src, src + ( this->m_iWidth * 4 ), dst );

                src += this->m_iWidth * 4;
                dst += rect.Pitch;
            }

            hres = m_texture->UnlockRect( 0 );
            if ( hres != D3D_OK )
                return false;
        }
    }

    return true;
}