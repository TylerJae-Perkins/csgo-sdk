#pragma once
#include "../common.h"
#include "config.h"

namespace Variables
{
	#pragma region menu
	inline std::vector<int> m_vecKeyStateTypes(256);

	inline int m_iMenuKey = VK_INSERT;
	inline int m_iUnloadKey = VK_DELETE;

	inline Color m_colMenuTheme = Color( 155, 123, 168, 255);
#pragma endregion

	#pragma region legitbot
	inline bool m_bLegitbotEnabled = false;
	inline int  m_iLegitBotWeapon = 0;
	#pragma endregion

#pragma region ragebot
	inline bool m_bRagebotEnabled = false;
	inline int  m_iRageBotWeapon = 0;
#pragma endregion
}