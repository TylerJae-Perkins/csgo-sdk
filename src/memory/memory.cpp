#include "memory.h"

std::uintptr_t Memory::FindPattern( const std::string_view szModuleName, const std::string_view szPattern )
{
	void* hModule = GetModuleBaseHandle( szModuleName );

	if ( hModule == nullptr )
		throw std::runtime_error( std::vformat( XorStr( "failed to get handle for: {}" ), std::make_format_args( szModuleName ) ) );

	const std::uint8_t* uModuleAddress = static_cast< std::uint8_t* >( hModule );
	const IMAGE_DOS_HEADER* pDosHeader = static_cast< IMAGE_DOS_HEADER* >( hModule );
	const IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast< const IMAGE_NT_HEADERS* >( uModuleAddress + pDosHeader->e_lfanew );

	return FindPattern( uModuleAddress, pNtHeaders->OptionalHeader.SizeOfImage, szPattern );
}

std::uintptr_t Memory::FindPattern( const std::uint8_t* uRegionStart, const std::uintptr_t uRegionSize, const std::string_view szPattern )
{
	const std::vector<std::optional<std::uint8_t>> vecBytes = PatternToBytes( szPattern );

	// check for bytes sequence match
	for ( std::uintptr_t i = 0U; i < uRegionSize - vecBytes.size( ); ++i )
	{
		bool bByteFound = true;

		for ( std::uintptr_t s = 0U; s < vecBytes.size( ); ++s )
		{
			// compare byte and skip if wildcard
			if ( vecBytes[ s ].has_value( ) && uRegionStart[ i + s ] != vecBytes[ s ].value( ) )
			{
				bByteFound = false;
				break;
			}
		}

		// return valid address
		if ( bByteFound )
			return reinterpret_cast< std::uintptr_t >( &uRegionStart[ i ] );
	}

	LOG( std::format( "[error] pattern not found: [{}]" , szPattern), LOG_ERROR );
	return 0U;
}

std::vector<std::optional<std::uint8_t>> Memory::PatternToBytes( const std::string_view szPattern )
{
	std::vector<std::optional<std::uint8_t>> vecBytes = { };
	auto itBegin = szPattern.cbegin( );
	const auto itEnd = szPattern.cend( );

	while ( itBegin < itEnd )
	{
		// check is current byte a wildcard
		if ( *itBegin == '?' )
		{
			// check is two-character wildcard
			if ( ++itBegin; itBegin < itEnd && *itBegin == '?' )
				++itBegin;

			// iterate
			if ( itBegin < itEnd )
				++itBegin;

			// ignore that
			vecBytes.emplace_back( std::nullopt );
			continue;
		}
		// check is not space
		else if ( *itBegin != ' ' )
		{
			// convert current 4 bits to hex
			std::uint8_t uByte = static_cast< std::uint8_t >( ( ( *itBegin >= 'A' ? ( ( ( *itBegin - 'A' ) & ( ~( 'a' ^ 'A' ) ) ) + 10 ) : ( *itBegin <= '9' ? *itBegin - '0' : 0x0 ) ) | 0xF0 ) << 4 );

			// convert next 4 bits to hex and assign to byte
			if ( ++itBegin; *itBegin != ' ' )
				uByte |= static_cast< std::uint8_t >( *itBegin >= 'A' ? ( ( ( *itBegin - 'A' ) & ( ~( 'a' ^ 'A' ) ) ) + 10 ) : ( *itBegin <= '9' ? *itBegin - '0' : 0x0 ) );

			vecBytes.emplace_back( uByte );
		}

		++itBegin;
	}

	return vecBytes;
}

void* Memory::GetModuleBaseHandle( const std::string_view szModuleName )
{
	const _PEB32* pPEB = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) ); // mov eax, fs:[0x30]

	if ( szModuleName.empty( ) )
		return pPEB->ImageBaseAddress;

	const std::wstring wszModuleName( szModuleName.begin( ), szModuleName.end( ) );

	for ( LIST_ENTRY* pListEntry = pPEB->Ldr->InLoadOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InLoadOrderModuleList; pListEntry = pListEntry->Flink )
	{
		const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD( pListEntry, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

		if ( pEntry->BaseDllName.Buffer && wszModuleName.compare( pEntry->BaseDllName.Buffer ) == 0 )
			return pEntry->DllBase;
	}

	LOG( std::format( "[error] module base not found: [{}]", szModuleName), LOG_ERROR );
	return nullptr;
}

// @todo: GetImportAddress also
void* Memory::GetExportAddress( const void* pModuleBase, const std::string_view szProcedureName )
{
	const std::uint8_t* pAddress = static_cast< const std::uint8_t* >( pModuleBase );
	const IMAGE_DOS_HEADER* pDosHeader = static_cast< const IMAGE_DOS_HEADER* >( pModuleBase );
	const IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast< const IMAGE_NT_HEADERS* >( pAddress + pDosHeader->e_lfanew );
	const IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNtHeaders->OptionalHeader;

	const std::uintptr_t uExportSize = pOptionalHeader->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
	const std::uintptr_t uExportAddress = pOptionalHeader->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;

	if ( uExportSize <= 0U )
		return nullptr;

	const IMAGE_EXPORT_DIRECTORY* pExportDirectory = reinterpret_cast< const IMAGE_EXPORT_DIRECTORY* >( pAddress + uExportAddress );
	const std::uintptr_t* pNamesRVA = reinterpret_cast< const std::uintptr_t* >( pAddress + pExportDirectory->AddressOfNames );
	const std::uintptr_t* pFunctionsRVA = reinterpret_cast< const std::uintptr_t* >( pAddress + pExportDirectory->AddressOfFunctions );
	const std::uint16_t* pNameOrdinals = reinterpret_cast< const std::uint16_t* >( pAddress + pExportDirectory->AddressOfNameOrdinals );

	// perform binary search
	std::uintptr_t uRight = pExportDirectory->NumberOfNames;
	std::uintptr_t uLeft = 0;

	while ( uRight != uLeft )
	{
		const std::uintptr_t uMiddle = uLeft + ( ( uRight - uLeft ) >> 1U );
		const int iResult = szProcedureName.compare( reinterpret_cast< const char* >( pAddress + pNamesRVA[ uMiddle ] ) );

		if ( iResult == 0 )
			return const_cast< void* >( static_cast< const void* >( pAddress + pFunctionsRVA[ pNameOrdinals[ uMiddle ] ] ) );

		if ( iResult > 0 )
			uLeft = uMiddle;
		else
			uRight = uMiddle;
	}

	LOG( XorStr( "[error] export not found: [{}]" , szProcedureName), LOG_ERROR );
	return nullptr;
}