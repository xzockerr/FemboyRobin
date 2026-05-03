#pragma once
#include <cstdint>

class offsets
{
public:
	bool initialize( );

	std::uintptr_t csgo_input{ 0x2340E00 };
	std::uintptr_t entity_list{ 0x24D1DF0 };
	std::uintptr_t game_entity_system{ 0x24D1DF0 };
	std::uintptr_t game_entity_system_highest_entity_index{ 0x2090 };
	std::uintptr_t game_rules{ 0x19F0A48 };
	std::uintptr_t global_vars{ 0x204C5D8 };
	std::uintptr_t glow_manager{ 0x2328DB0 };
	std::uintptr_t local_player_controller{ 0x230B5D0 };
	std::uintptr_t local_player_pawn{ 0x2057720 };
	std::uintptr_t planted_c4{ 0x2339AC8 };
	std::uintptr_t prediction{ 0x2057630 };
	std::uintptr_t sensitivity{ 0x23298C8 };
	std::uintptr_t sensitivity_sensitivity{ 0x58 };
	std::uintptr_t view_angles{ 0x2341488 };
	std::uintptr_t view_matrix{ 0x2331B30 };
	std::uintptr_t view_render{ 0x2330D38 };
	std::uintptr_t weapon_c4{ 0x22A9D58 };
	std::uintptr_t force_jump{ 0x204DE30 };

	std::uintptr_t m_pGameSceneNode{ 0x330 };
	std::uintptr_t m_modelState{ 0x150 };
	std::uintptr_t m_hPlayerPawn{ 0x904 };
	std::uintptr_t bone_array{ 0x80 };
};