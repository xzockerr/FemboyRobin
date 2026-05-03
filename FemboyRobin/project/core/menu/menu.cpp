#include <stdafx.hpp>

namespace
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_menu_background{};
	int g_menu_background_width{};
	int g_menu_background_height{};

	void try_load_menu_background( )
	{
		if ( g_menu_background )
		{
			return;
		}

		const std::array<std::string, 6> relative_paths
		{
			"assets/femboy.png",
			"../assets/femboy.png",
			"../../assets/femboy.png",
			"../../../assets/femboy.png",
			"FemboyRobin/assets/femboy.png",
			"../FemboyRobin/assets/femboy.png"
		};

		for ( const auto& p : relative_paths )
		{
			g_menu_background = zdraw::load_texture_from_file( p, &g_menu_background_width, &g_menu_background_height );
			if ( g_menu_background )
			{
				return;
			}
		}

		char module_path[MAX_PATH]{};
		const auto len = ::GetModuleFileNameA( nullptr, module_path, MAX_PATH );
		if ( len == 0 || len >= MAX_PATH )
		{
			return;
		}

		std::string base{ module_path, module_path + len };
		const auto pos = base.find_last_of( "\\/" );
		if ( pos == std::string::npos )
		{
			return;
		}

		base.resize( pos + 1 );
		for ( const auto& p : relative_paths )
		{
			g_menu_background = zdraw::load_texture_from_file( base + p, &g_menu_background_width, &g_menu_background_height );
			if ( g_menu_background )
			{
				return;
			}
		}
	}
}

void menu::draw( )
{
	if ( GetAsyncKeyState( VK_INSERT ) & 1 )
	{
		this->m_open = !this->m_open;
	}

	if ( !this->m_open )
	{
		return;
	}

	static auto x{ 200.0f };
	static auto y{ 150.0f };
	static auto w{ 680.0f };
	static auto h{ 510.0f };

	zui::begin( );

	if ( zui::begin_window( "Femboy Robin##main", this->m_x, this->m_y, this->m_w, this->m_h, true, 580.0f, 440.0f ) )
	{
		const auto [avail_w, avail_h] = zui::get_content_region_avail( );

		if ( zui::begin_nested_window( "##inner", avail_w, avail_h ) )
		{
			constexpr auto header_h{ 28.0f };
			constexpr auto padding{ 6.0f };

			zui::set_cursor_pos( padding, padding );
			this->draw_header( avail_w - padding * 2.0f, header_h );

			zui::set_cursor_pos( padding, padding + header_h + padding );
			this->draw_content( avail_w - padding * 2.0f, avail_h - header_h - padding * 3.0f );

			if ( const auto win = zui::detail::get_current_window( ) )
			{
				this->draw_accent_lines( win->bounds );
			}

			zui::end_nested_window( );
		}

		zui::end_window( );
	}

	zui::end( );
}

void menu::draw_header( float width, float height )
{
	if ( !zui::begin_nested_window( "##header", width, height ) )
	{
		return;
	}

	const auto current = zui::detail::get_current_window( );
	if ( !current )
	{
		zui::end_nested_window( );
		return;
	}

	const auto& style = zui::get_style( );
	const auto dt = zdraw::get_delta_time( );
	const auto bx = current->bounds.x;
	const auto by = current->bounds.y;
	const auto bw = current->bounds.w;
	const auto bh = current->bounds.h;

	zdraw::rect_filled( bx, by, bw, bh, zdraw::rgba{ 24, 16, 24, 130 } );
	zdraw::rect( bx, by, bw, bh, zdraw::rgba{ 82, 52, 88, 180 } );

	{
		constexpr auto title{ "Femboy Robin" };
		auto [tw, th] = zdraw::measure_text( title );
		zdraw::text( bx + 10.0f, by + ( bh - th ) * 0.5f, title, style.accent );
	}

	static constexpr std::pair<const char*, tab> tabs[ ]
	{
		{ "combat", tab::combat },
		{ "esp",    tab::esp    },
		{ "misc",   tab::misc   },
	};

	constexpr auto tab_count = static_cast< int >( std::size( tabs ) );
	constexpr auto tab_spacing{ 16.0f };

	struct tab_anim { float v{ 0.0f }; };
	static std::array<tab_anim, tab_count> anims{};

	auto cursor_x = bx + bw - 10.0f;

	for ( int i = tab_count - 1; i >= 0; --i )
	{
		const auto& t = tabs[ i ];
		const auto is_sel = ( this->m_tab == t.second );
		auto [tw, th] = zdraw::measure_text( t.first );

		cursor_x -= tw;

		const auto tab_rect = zui::rect{ cursor_x, by, tw, bh };
		const auto hovered = zui::detail::mouse_hovered( tab_rect ) && !zui::detail::overlay_blocking_input( );

		if ( hovered && zui::detail::mouse_clicked( ) )
		{
			this->m_tab = t.second;
		}

		auto& anim = anims[ i ];
		anim.v += ( ( is_sel ? 1.0f : 0.0f ) - anim.v ) * std::min( 10.0f * dt, 1.0f );

		const auto text_y = by + ( bh - th ) * 0.5f;
		const auto col = is_sel ? zui::lighten( style.accent, 1.0f + 0.1f * anim.v ) : zui::lerp( zdraw::rgba{ 156, 120, 160, 255 }, style.text, hovered ? 1.0f : 0.0f );

		zdraw::text( cursor_x, text_y, t.first, col );

		cursor_x -= tab_spacing;
	}

	zui::end_nested_window( );
}

void menu::draw_content( float width, float height )
{
	try_load_menu_background( );

	zui::push_style_var( zui::style_var::window_padding_x, 10.0f );
	zui::push_style_var( zui::style_var::window_padding_y, 10.0f );

	if ( !zui::begin_nested_window( "##content", width, height ) )
	{
		zui::pop_style_var( 2 );
		return;
	}

	if ( const auto win = zui::detail::get_current_window( ) )
	{
		if ( g_menu_background && g_menu_background_width > 0 && g_menu_background_height > 0 )
		{
			const auto pad = 12.0f;
			const auto max_h = win->bounds.h - pad * 2.0f;
			const auto aspect = static_cast< float >( g_menu_background_width ) / static_cast< float >( g_menu_background_height );
			auto img_h = std::max( 20.0f, max_h * 0.9f );
			auto img_w = img_h * aspect;

			const auto max_w = win->bounds.w * 0.55f;
			if ( img_w > max_w )
			{
				img_w = max_w;
				img_h = img_w / aspect;
			}

			const auto img_x = win->bounds.right( ) - img_w - pad;
			const auto img_y = win->bounds.bottom( ) - img_h - pad;
			zdraw::rect_textured( img_x, img_y, img_w, img_h, g_menu_background.Get( ), 0.0f, 0.0f, 1.0f, 1.0f, zdraw::rgba{ 255, 255, 255, 88 } );
		}

		this->draw_accent_lines( win->bounds );
	}

	switch ( this->m_tab )
	{
	case tab::combat: this->draw_combat( ); break;
	case tab::esp:    this->draw_esp( );    break;
	case tab::misc:   this->draw_misc( );   break;
	default: break;
	}

	zui::pop_style_var( 2 );
	zui::end_nested_window( );
}

void menu::draw_accent_lines( const zui::rect& bounds, float fade_ratio )
{
	const auto ix = bounds.x + 1.0f;
	const auto iw = bounds.w - 2.0f;
	const auto top_y = bounds.y + 1.0f;
	const auto bot_y = bounds.y + bounds.h - 2.0f;
	const auto accent = zui::get_accent_color( );
	const auto trans = zdraw::rgba{ accent.r, accent.g, accent.b, 0 };
	const auto fade_w = iw * fade_ratio;
	const auto solid_w = iw - fade_w * 2.0f;

	for ( const auto ly : { top_y, bot_y } )
	{
		zdraw::rect_filled_multi_color( ix, ly, fade_w, 1.0f, trans, accent, accent, trans );
		zdraw::rect_filled( ix + fade_w, ly, solid_w, 1.0f, accent );
		zdraw::rect_filled_multi_color( ix + fade_w + solid_w, ly, fade_w, 1.0f, accent, trans, trans, accent );
	}
}

void menu::draw_combat( )
{
	const auto [avail_w, avail_h] = zui::get_content_region_avail( );
	const auto col_w = ( avail_w - 8.0f ) * 0.5f;
	const auto& style = zui::get_style( );

	{
		const auto win = zui::detail::get_current_window( );
		if ( win )
		{
			constexpr auto group_spacing{ 12.0f };
			constexpr auto bar_h{ 22.0f };
			auto gx = win->bounds.x + style.window_padding_x;
			const auto gy = win->bounds.y + win->cursor_y;

			for ( int i = 0; i < 6; ++i )
			{
				auto [tw, th] = zdraw::measure_text( k_weapon_groups[ i ] );
				const auto gr = zui::rect{ gx, gy, tw, bar_h };
				const auto hov = zui::detail::mouse_hovered( gr ) && !zui::detail::overlay_blocking_input( );

				if ( hov && zui::detail::mouse_clicked( ) )
				{
					this->m_weapon_group = i;
				}

				const auto sel = ( this->m_weapon_group == i );
				const auto col = sel ? zui::get_accent_color( ) : zui::lerp( zdraw::rgba{ 100, 100, 100, 255 }, style.text, hov ? 1.0f : 0.0f );

				zdraw::text( gx, gy + ( bar_h - th ) * 0.5f, k_weapon_groups[ i ], col );

				if ( sel )
				{
					const auto accent = zui::get_accent_color( );
					const auto trans = zdraw::rgba{ accent.r, accent.g, accent.b, 0 };
					const auto fade = tw * 0.3f;
					zdraw::rect_filled_multi_color( gx, gy + bar_h - 2.0f, fade, 1.0f, trans, accent, accent, trans );
					zdraw::rect_filled( gx + fade, gy + bar_h - 2.0f, tw - fade * 2.0f, 1.0f, accent );
					zdraw::rect_filled_multi_color( gx + tw - fade, gy + bar_h - 2.0f, fade, 1.0f, accent, trans, trans, accent );
				}

				gx += tw + group_spacing;
			}

			win->cursor_y += bar_h + style.item_spacing_y;
			win->line_height = 0.0f;
		}
	}

	auto& cfg = settings::g_combat.groups[ this->m_weapon_group ];

	if ( zui::begin_group_box( "aimbot", col_w ) )
	{
		zui::checkbox( "enabled##ab", cfg.aimbot.enabled );
		if ( zui::keybind( "key##ab", cfg.aimbot.key ) )
		{
			for ( auto& group : settings::g_combat.groups )
			{
				group.aimbot.key = cfg.aimbot.key;
			}
		}
		zui::slider_int( "fov##ab", cfg.aimbot.fov, 1, 45 );
		zui::slider_int( "smoothing##ab", cfg.aimbot.smoothing, 0, 50 );
		zui::checkbox( "sticky target##ab", cfg.aimbot.sticky_target );
		zui::slider_int( "switch delay##ab", cfg.aimbot.switch_delay_ms, 0, 250, "%d ms" );
		zui::checkbox( "head only##ab", cfg.aimbot.head_only );
		zui::checkbox( "wall check##ab", cfg.aimbot.wall_check );

		if ( cfg.aimbot.wall_check )
		{
			zui::checkbox( "autowall##ab", cfg.aimbot.autowall );

			if ( cfg.aimbot.autowall )
			{
				zui::slider_float( "min damage##ab", cfg.aimbot.min_damage, 1.0f, 100.0f, "%.0f" );
			}
		}

		zui::checkbox( "predictive##ab", cfg.aimbot.predictive );

		zui::separator( );
		zui::checkbox( "draw fov##ab", cfg.aimbot.draw_fov );

		if ( cfg.aimbot.draw_fov )
		{
			zui::color_picker( "fov color##ab", cfg.aimbot.fov_color );
		}

		zui::end_group_box( );
	}

	zui::same_line( );

	if ( zui::begin_group_box( "triggerbot", col_w ) )
	{
		zui::checkbox( "enabled##tb", cfg.triggerbot.enabled );
		if ( zui::keybind( "key##tb", cfg.triggerbot.key ) )
		{
			for ( auto& group : settings::g_combat.groups )
			{
				group.triggerbot.key = cfg.triggerbot.key;
			}
		}
		zui::slider_float( "hitchance##tb", cfg.triggerbot.hitchance, 0.0f, 100.0f, "%.0f%%" );
		zui::slider_int( "delay (ms)##tb", cfg.triggerbot.delay, 0, 500 );
		zui::checkbox( "wall check##tb", cfg.triggerbot.wall_check );

		if ( cfg.triggerbot.wall_check )
		{
			zui::checkbox( "autowall##tb", cfg.triggerbot.autowall );

			if ( cfg.triggerbot.autowall )
			{
				zui::slider_float( "min damage##tb", cfg.triggerbot.min_damage, 1.0f, 100.0f, "%.0f" );
			}
		}

		zui::checkbox( "autostop##tb", cfg.triggerbot.autostop );

		if ( cfg.triggerbot.autostop )
		{
			zui::checkbox( "early autostop##tb", cfg.triggerbot.early_autostop );
		}

		zui::checkbox( "predictive##tb", cfg.triggerbot.predictive );
		zui::end_group_box( );
	}
}

void menu::draw_esp( )
{
	const auto [avail_w, avail_h] = zui::get_content_region_avail( );
	const auto col_w = ( avail_w - 8.0f ) * 0.5f;
	auto& p = settings::g_esp.m_player;

	if ( zui::begin_group_box( "box", col_w ) )
	{
		zui::checkbox( "enabled##bx", p.m_box.enabled );

		constexpr const char* box_styles[ ]{ "full", "cornered" };
		auto bs = static_cast< int >( p.m_box.style );

		if ( zui::combo( "style##bx", bs, box_styles, 2 ) )
		{
			p.m_box.style = static_cast< settings::esp::player::box::style0 >( bs );
		}

		zui::checkbox( "fill##bx", p.m_box.fill );
		zui::checkbox( "outline##bx", p.m_box.outline );

		if ( p.m_box.style == settings::esp::player::box::style0::cornered )
		{
			zui::slider_float( "corner len##bx", p.m_box.corner_length, 4.0f, 30.0f, "%.0f" );
		}

		zui::color_picker( "visible##bx", p.m_box.visible_color );
		zui::color_picker( "occluded##bx", p.m_box.occluded_color );
		zui::end_group_box( );
	}

	if ( zui::begin_group_box( "skeleton", col_w ) )
	{
		zui::checkbox( "enabled##sk", p.m_skeleton.enabled );
		zui::slider_float( "thickness##sk", p.m_skeleton.thickness, 0.5f, 4.0f, "%.1f" );
		zui::checkbox( "head dot##sk", p.m_skeleton.head_dot );

		if ( p.m_skeleton.head_dot )
		{
			zui::slider_float( "head dot size##sk", p.m_skeleton.head_dot_radius, 1.0f, 6.0f, "%.1f" );
			zui::color_picker( "head dot color##sk", p.m_skeleton.head_dot_color );
		}

		zui::color_picker( "visible##sk", p.m_skeleton.visible_color );
		zui::color_picker( "occluded##sk", p.m_skeleton.occluded_color );
		zui::end_group_box( );
	}

	if ( zui::begin_group_box( "health bar", col_w ) )
	{
		zui::checkbox( "enabled##hb", p.m_health_bar.enabled );
		zui::checkbox( "outline##hb", p.m_health_bar.outline );
		zui::checkbox( "gradient##hb", p.m_health_bar.gradient );
		zui::checkbox( "show value##hb", p.m_health_bar.show_value );
		zui::color_picker( "full##hb", p.m_health_bar.full_color );
		zui::color_picker( "low##hb", p.m_health_bar.low_color );
		zui::end_group_box( );
	}

	if ( zui::begin_group_box( "items", col_w ) )
	{
		auto& it = settings::g_esp.m_item;
		zui::checkbox( "enabled##it", it.enabled );
		zui::slider_float( "max dist##it", it.max_distance, 5.0f, 150.0f, "%.0fm" );
		zui::checkbox( "icon##it", it.m_icon.enabled );
		zui::color_picker( "icon color##it", it.m_icon.color );
		zui::checkbox( "name##it", it.m_name.enabled );
		zui::color_picker( "name color##it", it.m_name.color );
		zui::checkbox( "ammo##it", it.m_ammo.enabled );
		zui::color_picker( "ammo color##it", it.m_ammo.color );
		zui::color_picker( "empty color##it", it.m_ammo.empty_color );
		zui::end_group_box( );
	}

	const auto [cx, cy] = zui::get_cursor_pos( );
	zui::set_cursor_pos( col_w + 8.0f + zui::get_style( ).window_padding_x, zui::get_style( ).window_padding_y );

	if ( zui::begin_group_box( "name / weapon", col_w ) )
	{
		zui::checkbox( "name##nm", p.m_name.enabled );
		zui::color_picker( "name color##nm", p.m_name.color );
		zui::separator( );
		zui::checkbox( "weapon##wp", p.m_weapon.enabled );

		constexpr const char* disp_types[ ]{ "text", "icon", "text + icon" };
		auto dt = static_cast< int >( p.m_weapon.display );

		if ( zui::combo( "display##wp", dt, disp_types, 3 ) )
		{
			p.m_weapon.display = static_cast< settings::esp::player::weapon::display_type >( dt );
		}

		zui::color_picker( "text color##wp", p.m_weapon.text_color );
		zui::color_picker( "icon color##wp", p.m_weapon.icon_color );
		zui::end_group_box( );
	}

	if ( zui::begin_group_box( "ammo bar", col_w ) )
	{
		zui::checkbox( "enabled##amb", p.m_ammo_bar.enabled );
		zui::checkbox( "outline##amb", p.m_ammo_bar.outline );
		zui::checkbox( "gradient##amb", p.m_ammo_bar.gradient );
		zui::checkbox( "show value##amb", p.m_ammo_bar.show_value );
		zui::color_picker( "full##amb", p.m_ammo_bar.full_color );
		zui::color_picker( "low##amb", p.m_ammo_bar.low_color );
		zui::end_group_box( );
	}

	if ( zui::begin_group_box( "projectiles", col_w ) )
	{
		auto& pr = settings::g_esp.m_projectile;
		zui::checkbox( "enabled##pr", pr.enabled );
		zui::checkbox( "icon##pr", pr.show_icon );
		zui::checkbox( "name##pr", pr.show_name );
		zui::checkbox( "timer bar##pr", pr.show_timer_bar );
		zui::checkbox( "inferno bounds##pr", pr.show_inferno_bounds );
		zui::separator( );
		zui::color_picker( "he##pr", pr.color_he );
		zui::color_picker( "flash##pr", pr.color_flash );
		zui::color_picker( "smoke##pr", pr.color_smoke );
		zui::color_picker( "molotov##pr", pr.color_molotov );
		zui::color_picker( "decoy##pr", pr.color_decoy );
		zui::end_group_box( );
	}

	if ( zui::begin_group_box( "sound esp", col_w ) )
	{
		auto& s = p.m_sound;
		zui::checkbox( "enabled##snd", s.enabled );
		zui::slider_float( "max dist##snd", s.max_distance, 5.0f, 120.0f, "%.0fm" );
		zui::slider_float( "duration##snd", s.duration, 0.2f, 4.0f, "%.1fs" );
		zui::slider_float( "min speed##snd", s.min_speed, 5.0f, 220.0f, "%.0f" );
		zui::slider_float( "ring size##snd", s.ring_size, 6.0f, 40.0f, "%.0f" );
		zui::checkbox( "show text##snd", s.show_text );
		zui::color_picker( "color##snd", s.color );
		zui::end_group_box( );
	}

	if ( zui::begin_group_box( "item filters", col_w ) )
	{
		auto& f = settings::g_esp.m_item.m_filters;
		zui::checkbox( "rifles##f", f.rifles );
		zui::checkbox( "smgs##f", f.smgs );
		zui::checkbox( "shotguns##f", f.shotguns );
		zui::checkbox( "snipers##f", f.snipers );
		zui::checkbox( "pistols##f", f.pistols );
		zui::checkbox( "heavy##f", f.heavy );
		zui::checkbox( "grenades##f", f.grenades );
		zui::checkbox( "utility##f", f.utility );
		zui::end_group_box( );
	}
}

void menu::draw_misc( )
{
	const auto [avail_w, avail_h] = zui::get_content_region_avail( );
	const auto col_w = ( avail_w - 8.0f ) * 0.5f;
	auto& gr = settings::g_misc.m_grenades;

	if ( zui::begin_group_box( "grenade prediction", col_w ) )
	{
		zui::checkbox( "enabled##gr", gr.enabled );
		zui::checkbox( "local only##gr", gr.local_only );
		zui::slider_float( "line thickness##gr", gr.line_thickness, 0.5f, 5.0f, "%.1f" );
		zui::checkbox( "gradient line##gr", gr.line_gradient );
		zui::color_picker( "line color##gr", gr.line_color );
		zui::separator( );
		zui::checkbox( "show bounces##gr", gr.show_bounces );
		zui::color_picker( "bounce color##gr", gr.bounce_color );
		zui::slider_float( "bounce size##gr", gr.bounce_size, 1.0f, 8.0f, "%.1f" );
		zui::separator( );
		zui::color_picker( "detonate color##gr", gr.detonate_color );
		zui::slider_float( "detonate size##gr", gr.detonate_size, 1.0f, 10.0f, "%.1f" );
		zui::slider_float( "fade duration##gr", gr.fade_duration, 0.0f, 2.0f, "%.2f" );
		zui::end_group_box( );
	}

	zui::same_line( );

	if ( zui::begin_group_box( "per type colors", col_w ) )
	{
		zui::checkbox( "enabled##ptc", gr.per_type_colors );
		if ( gr.per_type_colors )
		{
			zui::color_picker( "he##ptc", gr.color_he );
			zui::color_picker( "flash##ptc", gr.color_flash );
			zui::color_picker( "smoke##ptc", gr.color_smoke );
			zui::color_picker( "molotov##ptc", gr.color_molotov );
			zui::color_picker( "decoy##ptc", gr.color_decoy );
		}

		zui::end_group_box( );
	}
}
