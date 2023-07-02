#include "netvar.h"
#include "../../interfaces/interfaces.h"
#include "../logging.h"

//@TODO: Remake this from the latest version as i believe they've changed a few things - 18-06-2023
bool CNetvarManager::Setup( const std::string_view szDumpFileName )
{
    // clear values
    m_mapProps.clear( );
    m_iStoredProps = 0;
    m_iStoredTables = 0;

    // format time
    const std::string szTime = std::vformat( XorStr( "[{:%d-%m-%Y %X}] " ), std::make_format_args( std::chrono::system_clock::now( ) ) );

    for ( auto pClass = Interfaces::m_pClientDll->GetAllClasses( ); pClass != nullptr; pClass = pClass->m_pNext )
    {
        if ( pClass->m_pRecvTable == nullptr )
            continue;

        StoreProps( pClass->m_szNetworkName, pClass->m_pRecvTable, 0U, 0 );
    }

    LOG( std::format( "found [{:d}] props in [{:d}] tables", CNetvarManager::Get().m_iStoredProps, CNetvarManager::Get( ).m_iStoredTables), LOG_MESSAGE );
    return !m_mapProps.empty( );
}

void CNetvarManager::StoreProps( const char* szClassName, RecvTable_t* pRecvTable, const std::uintptr_t uOffset, const int iDepth )
{
#ifdef _DEBUG
    std::string szDepth = {};

    for ( int i = 0; i < iDepth; i++ )
        szDepth.append( "\t" );

    if ( fsDumpFile.good( ) )
        fsDumpFile << std::format( "{0}[{1}]\n", szDepth, pRecvTable->m_szNetTableName );
#endif

    const FNV1A_t uClassHash = FNV1A::Hash( szClassName );
    const FNV1A_t uDelimiterHash = FNV1A::Hash( "->", uClassHash );
    const FNV1A_t uBaseClassHash = FNV1A::Hash( "baseclass", uDelimiterHash );


    for ( int i = 0; i < pRecvTable->m_nProps; ++i )
    {
        const auto pCurrentProp = &pRecvTable->m_pProps[ i ];

        // base tables filter
        if ( pCurrentProp == nullptr || isdigit( pCurrentProp->m_szVarName[ 0 ] ) )
            continue;

        // concat to our netvar format just by hash
        const FNV1A_t uTotalHash = FNV1A::Hash( pCurrentProp->m_szVarName, uDelimiterHash );

        // skip baseclass
        if ( uTotalHash == uBaseClassHash )
            continue;

        // has child table
        if ( const auto pChildTable = pCurrentProp->m_pDataTable; pChildTable != nullptr &&
            // has props
            pChildTable->m_nProps > 0 &&
            // first char is 'D' ("DT" - "DataTable")
            pChildTable->m_szNetTableName[ 0 ] == 'D' &&
            // type is data table
            pCurrentProp->m_iRecvType == DPT_DATATABLE )
            // recursively get props in all child tables
            StoreProps( szClassName, pChildTable, static_cast< std::uintptr_t >( pCurrentProp->m_iOffset ) + uOffset, iDepth + 1 );

        // make own netvar storing format
        const FNV1A_t        uHash = FNV1A::Hash( std::format( "{}->{}", szClassName, pCurrentProp->m_szVarName ).c_str( ) );
        const std::uintptr_t uTotalOffset = static_cast< std::uintptr_t >( pCurrentProp->m_iOffset ) + uOffset;

        // check if we have already grabbed property
        if ( !m_mapProps[ uTotalHash ].m_uOffset )
        {
#ifdef _DEBUG
            if ( fsDumpFile.good( ) )
                fsDumpFile << std::format( "{0}\t{1} {2} = 0x{3:04X};\n", szDepth, GetPropertyType( pCurrentProp ), pCurrentProp->m_szVarName,
                    uTotalOffset );
#endif

            // write values to map entry
            m_mapProps[ uTotalHash ] = { pCurrentProp, uTotalOffset };

            // count total stored props
            m_iStoredProps++;
        }
    }

    // count total stored tables
    m_iStoredTables++;
}

std::string CNetvarManager::GetPropertyType( const RecvProp_t* pRecvProp ) const
{
    static CStandartRecvProxies* pStandartRecvProxies = Interfaces::m_pClientDll->GetStandardRecvProxies( );

    if ( pRecvProp == nullptr )
        return "";

    RecvVarProxyFn pProxyFn = pRecvProp->m_oProxyFn;

    switch ( pRecvProp->m_iRecvType )
    {
    case DPT_INT:
        // @credits: hinnie
        if ( pProxyFn == pStandartRecvProxies->m_pInt32ToInt8 )
            return XorStr( "byte" );
        else if ( pProxyFn == pStandartRecvProxies->m_pInt32ToInt16 )
            return XorStr( "short" );

        return XorStr( "int" );
    case DPT_FLOAT:
        return XorStr( "float" );
    case DPT_VECTOR:
        return XorStr( "Vector" );
    case DPT_VECTOR2D:
        return XorStr( "Vector2D" );
    case DPT_STRING:
        return std::vformat( XorStr( "char[{:d}]" ), std::make_format_args( pRecvProp->m_nStringBufferSize ) );
    case DPT_ARRAY:
        return std::vformat( XorStr( "array[{:d}]" ), std::make_format_args( pRecvProp->m_iElements ) );
    case DPT_DATATABLE:
        return XorStr( "void*" );
    case DPT_INT64:
        return XorStr( "std::int64_t" );
    default:
        break;
    }

    return "";
}

std::uintptr_t CNetvarManager::FindInDataMap( DataMap_t* m_pMap, const FNV1A_t m_uFieldHash )
{
    while ( m_pMap != nullptr )
    {
        for ( int i = 0; i < m_pMap->m_nDataFields; i++ )
        {
            if ( m_pMap->m_pDataDesc[ i ].m_szFieldName == nullptr )
                continue;

            if ( FNV1A::Hash( m_pMap->m_pDataDesc[ i ].m_szFieldName ) == m_uFieldHash )
                return m_pMap->m_pDataDesc[ i ].m_iFieldOffset[ TD_OFFSET_NORMAL ];

            if ( m_pMap->m_pDataDesc[ i ].m_iFieldType == FIELD_EMBEDDED )
            {
                if ( m_pMap->m_pDataDesc[ i ].m_pTypeDescription != nullptr )
                {
                    if ( const auto m_uOffset = FindInDataMap( m_pMap->m_pDataDesc[ i ].m_pTypeDescription, m_uFieldHash ); m_uOffset != 0U )
                        return m_uOffset;
                }
            }
        }

        m_pMap = m_pMap->m_pBaseMap;
    }

    return 0U;
}
