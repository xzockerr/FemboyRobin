#include <stdafx.hpp>

bool offsets::initialize( )
{
	if ( !g::modules.client )
	{
		return false;
	}

	const auto client_base = g::modules.client;

	this->csgo_input = client_base + 0x2340E00;
	this->entity_list = client_base + 0x24D1DF0;
	this->local_player_controller = client_base + 0x230B5D0;
	this->global_vars = client_base + 0x204C5D8;
	this->view_matrix = client_base + 0x2331B30;

	return this->csgo_input && this->entity_list && this->local_player_controller && this->global_vars && this->view_matrix;
}
