#pragma once

namespace settings {

	struct combat
	{
		struct aimbot
		{
			bool enabled{ true };
			int key{ VK_XBUTTON2 };
			int type{ 0 };

			int fov{ 5 };
			int smoothing{ 5 };

			bool autowall{ true };
			float min_damage{ 90.0f };

			bool head_only{ true };
			bool wall_check{ true };
			bool sticky_target{ true };
			int switch_delay_ms{ 85 };

			bool draw_fov{ true };
			zdraw::rgba fov_color{ 236, 146, 219, 125 };

			bool predictive{ true };
		};

		struct triggerbot
		{
			bool enabled{ true };
			int key{ VK_XBUTTON2 };

			float hitchance{ 75.0f };
			int delay{ 10 };

			bool wall_check{ true };
			bool autowall{ true };
			float min_damage{ 90.0f };

			bool autostop{ false };
			bool early_autostop{ false };

			bool predictive{ true };
		};

		struct group_config
		{
			aimbot aimbot{};
			triggerbot triggerbot{};
		};

		static constexpr std::uint32_t k_group_count{ 6 };

		std::array<group_config, k_group_count> groups{};

		group_config& get( std::uint32_t weapon_type )
		{
			const auto idx = weapon_type - cstypes::pistol;
			return this->groups[ idx < k_group_count ? idx : 2 ];
		}

		const group_config& get( std::uint32_t weapon_type ) const
		{
			const auto idx = weapon_type - cstypes::pistol;
			return this->groups[ idx < k_group_count ? idx : 2 ];
		}
	};

	struct esp
	{
		struct player
		{
			bool enabled{ true };

			struct radar
			{
				bool enabled{ true };
				bool minimap_sync{ true };
				float pos_x{ 18.0f };
				float pos_y{ 120.0f };
				float size{ 170.0f };
				float range{ 1800.0f };
				bool rotate{ true };
				float spotted_hold{ 2.2f };

				zdraw::rgba background{ 18, 12, 20, 150 };
				zdraw::rgba border{ 96, 56, 104, 220 };
				zdraw::rgba crosshair{ 132, 95, 140, 200 };
				zdraw::rgba self_color{ 238, 214, 243, 245 };
				zdraw::rgba enemy_color{ 255, 70, 70, 245 };
				zdraw::rgba enemy_faded{ 190, 60, 60, 170 };
			} m_radar{};

			struct sound
			{
				bool enabled{ true };
				float max_distance{ 45.0f };
				float duration{ 1.2f };
				float min_speed{ 85.0f };
				float ring_size{ 12.0f };
				bool show_text{ true };

				zdraw::rgba color{ 238, 144, 220, 210 };
			} m_sound{};

			struct box
			{
				enum class style0 : std::uint8_t { full, cornered };

				bool enabled{ true };
				style0 style{ style0::cornered };
				bool fill{ true };
				bool outline{ true };
				float corner_length{ 10.0f };

				zdraw::rgba visible_color{ 236, 136, 215, 255 };
				zdraw::rgba occluded_color{ 158, 94, 150, 180 };
			} m_box{};

			struct skeleton
			{
				bool enabled{ true };
				float thickness{ 1.0f };
				bool head_dot{ true };
				float head_dot_radius{ 2.4f };

				zdraw::rgba visible_color{ 233, 160, 224, 255 };
				zdraw::rgba occluded_color{ 165, 104, 158, 180 };
				zdraw::rgba head_dot_color{ 255, 90, 90, 240 };
			} m_skeleton{};

			struct hitboxes
			{
				bool enabled{ false };

				zdraw::rgba visible_color{ 236, 146, 221, 10 };
				zdraw::rgba occluded_color{ 155, 93, 149, 10 };

				bool fill{ true };
				bool outline{ true };
			} m_hitboxes{};

			struct health_bar
			{
				enum class position : std::uint8_t { left, top, bottom };

				bool enabled{ true };
				position position{ position::left };
				bool outline{ true };
				bool gradient{ true };
				bool show_value{ true };

				zdraw::rgba full_color{ 236, 136, 215, 255 };
				zdraw::rgba low_color{ 166, 92, 171, 255 };
				zdraw::rgba background_color{ 15, 16, 22, 150 };
				zdraw::rgba outline_color{ 15, 16, 22, 255 };
				zdraw::rgba text_color{ 236, 220, 238, 255 };
			} m_health_bar{};

			struct ammo_bar
			{
				enum class position : std::uint8_t { left, top, bottom };

				bool enabled{ true };
				position position{ position::bottom };
				bool outline{ true };
				bool gradient{ true };
				bool show_value{ false };

				zdraw::rgba full_color{ 236, 136, 215, 255 };
				zdraw::rgba low_color{ 166, 92, 171, 255 };
				zdraw::rgba background_color{ 15, 16, 22, 150 };
				zdraw::rgba outline_color{ 15, 16, 22, 255 };
				zdraw::rgba text_color{ 236, 220, 238, 255 };
			} m_ammo_bar{};

			struct info_flags
			{
				enum flag : std::uint8_t
				{
					none = 0,
					money = 1 << 0,
					armor = 1 << 1,
					kit = 1 << 2,
					scoped = 1 << 3,
					defusing = 1 << 4,
					flashed = 1 << 5,
					ping = 1 << 6,
					distance = 1 << 7
				};

				bool enabled{ true };
				std::uint8_t flags{ flag::money | flag::armor | flag::kit | flag::scoped | flag::defusing | flag::flashed | flag::ping };

				zdraw::rgba money_color{ 120, 230, 160, 255 };
				zdraw::rgba armor_color{ 236, 220, 238, 255 };
				zdraw::rgba kit_color{ 236, 136, 215, 255 };
				zdraw::rgba scoped_color{ 236, 220, 238, 255 };
				zdraw::rgba defusing_color{ 236, 136, 215, 255 };
				zdraw::rgba flashed_color{ 255, 210, 120, 255 };
				zdraw::rgba distance_color{ 146, 108, 150, 255 };

				[[nodiscard]] bool has( flag f ) const { return this->flags & f; }
			} m_info_flags{};

			struct name
			{
				bool enabled{ true };
				zdraw::rgba color{ 236, 220, 238, 230 };
			} m_name{};

			struct weapon
			{
				enum class display_type : std::uint8_t { text, icon, text_and_icon };

				bool enabled{ true };
				display_type display{ display_type::icon };

				zdraw::rgba text_color{ 236, 220, 238, 210 };
				zdraw::rgba icon_color{ 236, 220, 238, 230 };
			} m_weapon{};
		} m_player{};

		struct item
		{
			bool enabled{ true };
			float max_distance{ 40.0f };

			struct icon
			{
				bool enabled{ true };
				zdraw::rgba color{ 236, 220, 238, 200 };
			} m_icon{};

			struct name
			{
				bool enabled{ false };
				zdraw::rgba color{ 236, 220, 238, 180 };
			} m_name{};

			struct ammo
			{
				bool enabled{ true };
				zdraw::rgba color{ 236, 136, 215, 200 };
				zdraw::rgba empty_color{ 180, 80, 80, 200 };
			} m_ammo{};

			struct filters
			{
				bool rifles{ true };
				bool smgs{ true };
				bool shotguns{ true };
				bool snipers{ true };
				bool pistols{ true };
				bool heavy{ true };
				bool grenades{ true };
				bool utility{ true };
			} m_filters{};
		} m_item{};

		struct projectile
		{
			bool enabled{ true };

			bool show_icon{ true };
			bool show_name{ true };
			bool show_timer_bar{ true };
			bool show_inferno_bounds{ true };

			zdraw::rgba default_color{ 236, 220, 238, 200 };
			zdraw::rgba color_he{ 220, 150, 150, 220 };
			zdraw::rgba color_flash{ 230, 220, 150, 220 };
			zdraw::rgba color_smoke{ 160, 200, 180, 220 };
			zdraw::rgba color_molotov{ 220, 170, 130, 220 };
			zdraw::rgba color_decoy{ 211, 164, 219, 200 };

			zdraw::rgba timer_high_color{ 236, 136, 215, 255 };
			zdraw::rgba timer_low_color{ 220, 100, 100, 255 };
			zdraw::rgba bar_background{ 15, 16, 22, 150 };
		} m_projectile{};
	};

	struct misc
	{
		struct grenades
		{
			bool enabled{ true };

			zdraw::rgba line_color{ 234, 166, 224, 200 };
			float line_thickness{ 2.0f };
			bool line_gradient{ true };

			bool show_bounces{ true };
			zdraw::rgba bounce_color{ 236, 220, 238, 255 };
			float bounce_size{ 2.0f };

			zdraw::rgba detonate_color{ 140, 150, 235, 255 };
			float detonate_size{ 4.0f };

			bool per_type_colors{ false };
			zdraw::rgba color_he{ 190, 140, 140, 200 };
			zdraw::rgba color_flash{ 200, 195, 150, 200 };
			zdraw::rgba color_smoke{ 150, 185, 165, 200 };
			zdraw::rgba color_molotov{ 195, 155, 130, 200 };
			zdraw::rgba color_decoy{ 160, 165, 185, 200 };

			bool local_only{ true };
			float fade_duration{ 0.3f };
		} m_grenades{};
	};

	inline combat g_combat{};
	inline esp g_esp{};
	inline misc g_misc{};

} // namespace settings
