/*
	NOTE:
	This file was autogenerated by CppGen, changes may be overwritten and forever lost!
	Modify at your own risk!
	
	[ Generated on 2023.06.11 00:02:54 ]
*/

#include "Scripts.hpp"

namespace CppProject
{
	void window_draw_startup(ScopeAny self)
	{
		sVar(content_x) = IntType(0);
		sVar(content_y) = IntType(0);
		sVar(content_width) = sReal(window_width);
		sVar(content_height) = sReal(window_height);
		sBool(content_mouseon) = app_mouse_box(self, sVar(content_x), sVar(content_y), sVar(content_width), sVar(content_height)) && !sBool(popup_mouseon) && !sBool(toast_mouseon) && !sBool(context_menu_mouseon);
		RealType headersize = IntType(144);
		draw_clear_alpha(global::c_level_middle, 1.0);
		draw_pattern(IntType(0), headersize, sReal(window_width), sReal(window_height) - headersize);
		draw_box(IntType(0), IntType(0), sReal(window_width), headersize, false, global::c_level_top, IntType(1));
		draw_divide(IntType(0), headersize, sReal(window_width));
		draw_sprite(ID_spr_logo, IntType(0), (IntType)(sReal(window_width) / 2.0), (IntType)(headersize / 2.0));
		StringType trial = ((global::trial_version > 0) ? /*" "*/ STR(17) + text_get({ /*"startuptrial"*/ STR(1920) }) : /*""*/ STR(0));
		draw_button_text(self, { text_get({ /*"startupversion"*/ STR(3868), mineimator_version_full + trial }), (sReal(window_width) / 2.0) + IntType(259), floor((headersize / 2.0) + ((RealType)sprite_get_height(ID_spr_logo) / 2.0)) + IntType(3), ID_popup_switch, sInt(popup_about) });
		sVar(dy) = headersize + IntType(48);
		sVar(dw) = min({ sReal(window_width) - IntType(48), IntType(1008) });
		if (ds_list_size(sInt(recent_list)) == IntType(0))
		{
			draw_label(self, { text_get({ /*"recentnone"*/ STR(3869) }), sReal(window_width) / 2.0, sVar(dy), fa_center, fa_middle, global::c_accent, IntType(1), sInt(font_heading_big) });
			sVar(dy) += IntType(48);
		}
		draw_set_font(sInt(font_button));
		RealType newprojectwidth, browsewidth, centerx;
		newprojectwidth = string_width(text_get({ /*"startupnewproject"*/ STR(3870) })) + button_icon_padding;
		browsewidth = string_width(text_get({ /*"startupbrowse"*/ STR(3871) })) + button_icon_padding;
		centerx = round((sReal(window_width) / 2.0) - ((browsewidth + newprojectwidth) / 2.0));
		if (ds_list_size(sInt(recent_list)) > IntType(0))
			sVar(dx) = (sReal(window_width) / 2.0) + ((RealType)sVar(dw) / 2.0);
		else
			sVar(dx) = centerx + (browsewidth + IntType(24) + newprojectwidth);
		
		sVar(dx) -= newprojectwidth;
		if (draw_button_label(self, /*"startupnewproject"*/ STR(3870), sVar(dx), sVar(dy), null_, icons_FILE))
		{
			popup_newproject_clear(self);
			popup_switch(self, sInt(popup_newproject));
		}
		if (sReal(recent_list_amount) > IntType(0))
			sVar(dx) -= 12.0 + browsewidth;
		else
			sVar(dx) = centerx;
		
		if (draw_button_label(self, /*"startupbrowse"*/ STR(3871), sVar(dx), sVar(dy), null_, icons_FOLDER, e_button_SECONDARY))
		{
			if (project_load(self))
				sStr(window_state) = /*""*/ STR(0);
		}
		if (sReal(recent_list_amount) > IntType(0))
		{
			if (draw_button_icon(self, { /*"startuprecentdisplay"*/ STR(3872), sVar(dx) - IntType(24) - IntType(8), sVar(dy) + IntType(4), IntType(24), IntType(24), false, sStr(recent_display_mode) == /*"grid"*/ STR(613) ? icons_VIEW_LIST : icons_VIEW_GRID, null_, false, sStr(recent_display_mode) == /*"grid"*/ STR(613) ? /*"tooltipviewlist"*/ STR(3873) : /*"tooltipviewgrid"*/ STR(3874) }))
			{
				if (sStr(recent_display_mode) == /*"list"*/ STR(1068))
					sStr(recent_display_mode) = /*"grid"*/ STR(613);
				else
					sStr(recent_display_mode) = /*"list"*/ STR(1068);
				
			}
		}
		if (sReal(recent_list_amount) > IntType(0))
		{
			sVar(dx) -= (12.0 + 28.0);
			sVar(dx) = (sReal(window_width) / 2.0) - ((RealType)sVar(dw) / 2.0);
			draw_set_font(sInt(font_heading));
			draw_label(self, { text_get({ /*"startuprecentprojects"*/ STR(3875) }), sVar(dx), sVar(dy) + IntType(16), fa_left, fa_middle, global::c_accent, IntType(1) });
			IntType labelwid = string_width(text_get({ /*"startuprecentprojects"*/ STR(3875) }));
			if (draw_button_label(self, /*"startupsortby"*/ STR(1450), sVar(dx) + labelwid + IntType(16), sVar(dy), null_, icons_SORT_DOWN, e_button_TERTIARY))
				menu_settings_set(self, sVar(dx) + labelwid + IntType(16), sVar(dy), /*"startupsortby"*/ STR(1450), IntType(32));
			if (sStr(settings_menu_name) == /*"startupsortby"*/ STR(1450) && sStr(settings_menu_ani_type) != /*"hide"*/ STR(719))
				ObjType(value_animation, ObjType(micro_animation, global::current_microani)->active)->value = true;
			sVar(dy) += IntType(72);
			RealType listheight;
			if (sStr(recent_display_mode) == /*"list"*/ STR(1068))
				listheight = IntType(28) + min({ sReal(window_height) - sVar(dy), (min({ ds_list_size(sInt(recent_list)), IntType(8) }) * IntType(44)) });
			else
				listheight = min({ sReal(window_height) - sVar(dy), ceil(sReal(recent_list_amount) / 4.0) * IntType(256) });
			
			tab_control(self, listheight);
			draw_recent(self, { sVar(dx), sVar(dy), sVar(dw), listheight });
			tab_next(self);
		}
		else
		{
			VarType midx, midy;
			midx = snap(sReal(window_width) / 2.0, IntType(2));
			midy = snap(headersize + ((sReal(window_height) - headersize) / 1.75), IntType(2));
			if ((midy + ((RealType)sprite_get_height(ID_spr_jonathan_splash) / 1.75)) < (sReal(window_height) - IntType(50)))
				draw_image({ ID_spr_jonathan_splash, IntType(0), midx, midy });
		}
		
	}
	
	RealType window_draw_timeline_move(ScopeAny self)
	{
		if (sVar(window_busy) != /*"timelinemove"*/ STR(48))
			return IntType(0);
		sVar(content_x) = IntType(0);
		sVar(content_y) = IntType(0);
		sVar(content_width) = sReal(window_width);
		sVar(content_height) = sReal(window_height);
		sVar(dx) = gmlGlobal::mouse_x + sReal(timeline_move_off_x);
		sVar(dy) = gmlGlobal::mouse_y + sReal(timeline_move_off_y);
		window_draw_timeline_move_tree(self, sInt(timeline_move_obj));
		return 0.0;
	}
	
	void window_draw_timeline_move_tree(ScopeAny self, VarType par)
	{
		RealType itemh, indent, iconalpha, namealpha;
		VarType iconcolor, namecolor;
		itemh = ((sReal(setting_timeline_compact) > 0) ? IntType(20) : IntType(24));
		indent = IntType(20);
		for (IntType t = IntType(0); t < ds_list_size(idInt(par, tree_list)); t++)
		{
			VarType tl, px;
			RealType xoff;
			tl = DsList(idInt(par, tree_list)).Value(t);
			px = sVar(dx);
			xoff = IntType(0);
			if (!tl_update_list_filter(tl))
			{
				if (ObjType(obj_timeline, tl)->tree_extend > 0)
					window_draw_timeline_move_tree(self, tl);
				continue;
			}
			sVar(dx) += IntType(4);
			if (ds_list_size(ObjType(obj_timeline, tl)->tree_list) > IntType(0))
			{
				draw_image({ ID_spr_icons, (ObjType(obj_timeline, tl)->tree_extend > 0) ? icons_CHEVRON_DOWN_TINY : icons_CHEVRON_RIGHT_TINY, sVar(dx) + IntType(8), sVar(dy) + itemh / 2.0, IntType(1), IntType(1), global::c_text_secondary, global::a_text_secondary });
				if (sReal(setting_timeline_compact) > 0)
					sVar(dx) += IntType(20);
			}
			else
				if (sReal(setting_timeline_compact) > 0)
					sVar(dx) += IntType(5);
			
			if (!(sReal(setting_timeline_compact) > 0))
				sVar(dx) += IntType(20);
			if (ObjType(obj_timeline, tl)->selected > 0)
			{
				iconcolor = global::c_accent;
				iconalpha = IntType(1);
				namecolor = global::c_accent;
				namealpha = IntType(1);
			}
			else
			{
				iconcolor = global::c_text_tertiary;
				iconalpha = global::a_text_tertiary;
				namecolor = global::c_text_main;
				namealpha = global::a_text_main;
			}
			
			if (ObjType(obj_timeline, tl)->type != null_ && !(sReal(setting_timeline_compact) > 0))
			{
				draw_image({ ID_spr_icons, DsList(global::timeline_icon_list).Value(ObjType(obj_timeline, tl)->type), sVar(dx) + IntType(10), sVar(dy) + (itemh / 2.0), IntType(1), IntType(1), iconcolor, iconalpha });
				sVar(dx) += IntType(28);
			}
			draw_label(self, { string_remove_newline(ObjType(obj_timeline, tl)->display_name), sVar(dx), sVar(dy) + itemh / 2.0, fa_left, fa_middle, namecolor, namealpha, sInt(font_value) });
			sVar(dx) = px + indent;
			sVar(dy) += itemh;
			if (ObjType(obj_timeline, tl)->tree_extend > 0)
				window_draw_timeline_move_tree(self, tl);
			sVar(dx) = px;
		}
	}
	
	void window_draw_toasts(ScopeAny self)
	{
		IntType toast;
		VarType busy;
		sBool(toast_mouseon) = false;
		busy = sVar(window_busy);
		if (sInt(popup) > 0 && busy == /*"popup"*/ STR(1566) + idVar(sInt(popup), name))
			sVar(window_busy) = /*""*/ STR(0);
		for (RealType i = sInt(toast_amount) - IntType(1); i >= IntType(0); i--)
		{
			toast = DsList(sInt(toast_list)).Value(i);
			draw_set_alpha(ease(/*"easeoutcirc"*/ STR(557), ObjType(obj_toast, toast)->remove_alpha));
			toast_draw(self, toast);
			draw_set_alpha(1.0);
		}
		sVar(window_busy) = busy;
	}
	
	VarType window_maximize()
	{
		return lib_window_maximize(window_handle());
	}
	
	VarType window_set_focus()
	{
		return lib_window_set_focus(window_handle());
	}
	
	void app_startup_interface_world_import()
	{
		global::_app->world_import_settings_popup = new_popup({ /*"worldsettings"*/ STR(3876), ID_popup_worldsettings_draw, IntType(600), IntType(532), true, false, false, true, ID_world_import_apply_settings });
		global::_app->world_import_surface = null_;
		global::_app->world_import_add_tl = false;
		global::_app->world_import_temp = null_;
		global::_app->world_import_settings_block_select = null_;
		global::_app->world_import_settings_block_list = (new obj_sortlist)->id;
		ObjType(obj_sortlist, global::_app->world_import_settings_block_list)->can_deselect = true;
		ObjType(obj_sortlist, global::_app->world_import_settings_block_list)->script = ID_action_world_import_settings_block_select;
		sortlist_column_add(global::_app->world_import_settings_block_list, /*"blockfilter"*/ STR(2888), IntType(0));
		for (IntType b = IntType(0); b < ds_list_size(ObjType(obj_minecraft_assets, global::mc_assets)->block_list); b++)
			if (ds_list_find_index(global::_app->setting_world_import_filter_list, b) < IntType(0))
				sortlist_add(global::_app->world_import_settings_block_list, b);
		global::_app->world_import_settings_filter_select = null_;
		global::_app->world_import_settings_filter_list = (new obj_sortlist)->id;
		ObjType(obj_sortlist, global::_app->world_import_settings_filter_list)->can_deselect = true;
		ObjType(obj_sortlist, global::_app->world_import_settings_filter_list)->script = ID_action_world_import_settings_filter_select;
		sortlist_column_add(global::_app->world_import_settings_filter_list, /*"blockfilter"*/ STR(2888), IntType(0));
		for (IntType i = IntType(0); i < ds_list_size(global::_app->setting_world_import_filter_list); i++)
			sortlist_add(global::_app->world_import_settings_filter_list, DsList(global::_app->setting_world_import_filter_list).Value(i));
		world_import_startup();
	}
	
	void world_import_begin(ScopeAny self, BoolType addtl, VarType temp)
	{
		sStr(window_state) = /*"world_import"*/ STR(2664);
		sStr(popup_ani_type) = /*""*/ STR(0);
		sStr(world_import_world_root) = /*""*/ STR(0);
		sVar(world_import_world_name) = text_get({ /*"worldimportnoworld"*/ STR(3877) });
		sStr(world_import_dimension) = /*"overworld"*/ STR(3878);
		sBool(world_import_add_tl) = addtl;
		sVar(world_import_temp) = temp;
	}
	
	void world_import_select_dimension(ScopeAny self, VarType name)
	{
		world_import_select_world(self, global::_app->world_import_world_root, name);
	}
	
	void world_import_confirm_done(ScopeAny self)
	{
		sStr(window_state) = /*""*/ STR(0);
	}
	
	void world_import_dimension_menu_init()
	{
		menu_add_item({ /*"overworld"*/ STR(3878), text_get({ /*"worldimportoverworld"*/ STR(3888) }) });
		if (world_import_has_dimension(/*"nether"*/ STR(3889)))
			menu_add_item({ /*"nether"*/ STR(3889), text_get({ /*"worldimportnether"*/ STR(3890) }) });
		if (world_import_has_dimension(/*"end"*/ STR(3891)))
			menu_add_item({ /*"end"*/ STR(3891), text_get({ /*"worldimportend"*/ STR(3892) }) });
	}
	
	void window_draw_world_import(ScopeAny self)
	{
		RealType spacing, capwid, surfaceh;
		BoolType hasselection;
		VarType surfacey;
		spacing = IntType(12);
		sVar(content_x) = IntType(0);
		sVar(content_y) = sReal(toolbar_size);
		sVar(content_width) = sReal(window_width);
		sVar(content_height) = IntType(40);
		sVar(dx) = sVar(content_x) + IntType(12);
		sVar(dy) = sVar(content_y) + IntType(8);
		sVar(dw) = sVar(content_width);
		sVar(dh) = sVar(content_height);
		draw_clear(global::c_level_bottom);
		RealType confirmx, confirmy, confirmw, confirmh;
		hasselection = world_import_has_selection();
		confirmh = IntType(64);
		if (hasselection)
		{
			draw_set_font(sInt(font_heading_big));
			confirmw = string_width(text_get({ /*"worldimportconfirm"*/ STR(3893) })) + IntType(70);
		}
		else
			confirmw = IntType(0);
		
		confirmx = sReal(window_width) / 2.0 - confirmw / 2.0;
		confirmy = sReal(window_height) - IntType(60) - confirmh - ((IntType)sBool(setting_show_shortcuts_bar) * IntType(28));
		surfacey = sVar(content_y) + sVar(content_height);
		surfaceh = sReal(window_height) - surfacey;
		if (sBool(setting_show_shortcuts_bar))
			surfaceh -= 28.0;
		sVar(world_import_surface) = surface_require(sVar(world_import_surface), sReal(window_width), surfaceh);
		world_import_update_surface(IntType(0), surfacey, sReal(window_width), surfaceh, confirmx, confirmy, confirmw, confirmh);
		draw_set_color(c_ltgray);
		draw_rectangle(IntType(0), (IntType)(surfacey), (IntType)(sReal(window_width)), (IntType)(surfacey + surfaceh), false);
		draw_surface((IntType)(sVar(world_import_surface)), IntType(0), (IntType)(surfacey));
		draw_box(sVar(content_x), sVar(content_y), sVar(content_width), sVar(content_height), false, global::c_level_middle, IntType(1));
		draw_divide(sVar(content_x), sVar(content_y) + sVar(content_height), sVar(content_width));
		draw_gradient(sVar(content_x), sVar(content_y) + sVar(content_height), sVar(content_width), shadow_size, c_black, shadow_alpha, shadow_alpha, IntType(0), IntType(0));
		sBool(content_mouseon) = true;
		draw_set_font(sInt(font_button));
		sVar(dw) = string_width(text_get({ /*"worldimportcancel"*/ STR(3894) })) + IntType(24);
		if (draw_button_label(self, /*"worldimportcancel"*/ STR(3894), sVar(dx), sVar(content_y) + IntType(4), null_, null_, e_button_SECONDARY, null_, e_anchor_LEFT))
			world_import_cancel();
		sVar(dx) += sVar(dw);
		sVar(dx) += IntType(12);
		draw_divide_vertical(sVar(dx), sVar(content_y) + IntType(6), sVar(content_height) - IntType(12));
		sVar(dx) += IntType(12);
		sVar(dw) = IntType(256);
		capwid = IntType(50);
		draw_button_menu(self, { /*"worldimportworld"*/ STR(1462), e_menu_LIST, sVar(dx), sVar(dy), sVar(dw), IntType(24), sStr(world_import_world_root), sVar(world_import_world_name), ID_world_import_select_world, false, null_, null_, /*""*/ STR(0), null_, null_, capwid });
		sVar(dx) += sVar(dw) + spacing;
		sVar(dw) = IntType(208);
		capwid = IntType(80);
		draw_button_menu(self, { /*"worldimportdimension"*/ STR(1463), e_menu_LIST, sVar(dx), sVar(dy), sVar(dw), IntType(24), sStr(world_import_dimension), text_get({ /*"worldimport"*/ STR(2880) + sStr(world_import_dimension) }), ID_world_import_select_dimension, false, null_, null_, /*""*/ STR(0), null_, null_, capwid });
		sVar(dx) += sVar(dw);
		sVar(dx) += IntType(12);
		draw_divide_vertical(sVar(dx), sVar(content_y) + IntType(6), sVar(content_height) - IntType(12));
		sVar(dx) += IntType(12);
		sVar(dw) = IntType(24);
		spacing = IntType(4);
		if (draw_button_icon(self, { /*"worldimportbrowse"*/ STR(3895), sVar(dx), sVar(dy), sVar(dw), sVar(dw), false, icons_FOLDER, null_, false, /*"worldimportbrowsetip"*/ STR(3896) }))
		{
			StringType leveldat = file_dialog_open(text_get({ /*"worldimportbrowseworlds"*/ STR(3897) }) + /*" (level.dat)|level.dat;"*/ STR(3898), /*""*/ STR(0), world_import_get_saves_dir(), text_get({ /*"worldimportbrowsecaption"*/ STR(3899) }));
			if (file_exists_lib(leveldat))
				world_import_select_world(self, filename_dir(leveldat));
		}
		sVar(dx) += sVar(dw) + spacing;
		if (draw_button_icon(self, { /*"worldimportsettings"*/ STR(3900), sVar(dx), sVar(dy), sVar(dw), sVar(dw), false, icons_SETTINGS, null_, false, /*"worldimportsettingstip"*/ STR(3901) }))
			popup_show(self, sInt(world_import_settings_popup));
		sVar(dx) += sVar(dw) + spacing;
		BoolType worldpicked = sStr(world_import_world_root) != /*""*/ STR(0);
		if (draw_button_icon(self, { /*"worldimportreload"*/ STR(3902), sVar(dx), sVar(dy), sVar(dw), sVar(dw), false, icons_REFRESH, null_, !worldpicked, /*"worldimportreloadtip"*/ STR(3903) }))
			world_import_select_world(self, sStr(world_import_world_root), sStr(world_import_dimension));
		sVar(dx) += sVar(dw) + spacing;
		if (draw_button_icon(self, { /*"worldimportgotoplayer"*/ STR(3904), sVar(dx), sVar(dy), sVar(dw), sVar(dw), false, icons_PATH_POINT, null_, !worldpicked, /*"worldimportgotoplayertip"*/ STR(3905) }))
			world_import_go_to_player();
		sVar(dx) += sVar(dw);
		sVar(dx) += IntType(12);
		draw_divide_vertical(sVar(dx), sVar(content_y) + IntType(6), sVar(content_height) - IntType(12));
		sVar(dx) += IntType(12);
		sVar(dy) = sVar(content_y) + IntType(4);
		draw_set_font(sInt(font_label));
		spacing = IntType(12);
		sVar(dw) = string_width(text_get({ /*"worldimportselection"*/ STR(2883) }) + /*":"*/ STR(755));
		draw_label(self, { text_get({ /*"worldimportselection"*/ STR(2883) }) + /*":"*/ STR(755), sVar(dx), sVar(content_y) + (RealType)sVar(content_height) / 2.0, fa_left, fa_middle, global::c_text_secondary, global::a_text_secondary });
		sVar(dx) += sVar(dw) + spacing;
		draw_set_font(sInt(font_button));
		sVar(dw) = string_width(text_get({ /*"worldimportselectionsmall"*/ STR(3906) })) + IntType(24);
		if (draw_button_label(self, /*"worldimportselectionsmall"*/ STR(3906), sVar(dx), sVar(dy), null_, null_, e_button_SECONDARY, null_, e_anchor_LEFT, !worldpicked))
			world_import_set_selection(/*"small"*/ STR(3907));
		sVar(dx) += sVar(dw) + spacing;
		sVar(dw) = string_width(text_get({ /*"worldimportselectionmedium"*/ STR(3908) })) + IntType(24);
		if (draw_button_label(self, /*"worldimportselectionmedium"*/ STR(3908), sVar(dx), sVar(dy), null_, null_, e_button_SECONDARY, null_, e_anchor_LEFT, !worldpicked))
			world_import_set_selection(/*"medium"*/ STR(581));
		sVar(dx) += sVar(dw) + spacing;
		sVar(dw) = string_width(text_get({ /*"worldimportselectionlarge"*/ STR(3909) })) + IntType(24);
		if (draw_button_label(self, /*"worldimportselectionlarge"*/ STR(3909), sVar(dx), sVar(dy), null_, null_, e_button_SECONDARY, null_, e_anchor_LEFT, !worldpicked))
			world_import_set_selection(/*"large"*/ STR(3910));
		if (world_import_has_selection())
		{
			sVar(dx) += sVar(dw) + IntType(20);
			VecType size = world_import_get_selection_size();
			draw_label(self, { text_get({ /*"worldimportblocks"*/ STR(3911), size.Real(X_), size.Real(Y_), size.Real(Z_) }), sVar(dx), sVar(content_y) + (RealType)sVar(content_height) / 2.0, fa_left, fa_middle, global::c_text_main, global::a_text_main, sInt(font_value) });
		}
		sVar(content_x) = IntType(0);
		sVar(content_y) = IntType(0);
		sVar(content_width) = sReal(window_width);
		sVar(content_height) = sReal(window_height);
		if (hasselection)
			if (draw_button_label(self, /*"worldimportconfirm"*/ STR(3893), confirmx, confirmy, confirmw, null_, e_button_BIG, null_, e_anchor_LEFT))
				world_import_confirm();
		if (worldpicked && sVar(setting_world_import_filter_enabled) > 0 && ds_list_size(sInt(setting_world_import_filter_list)) > IntType(0))
		{
			VarType filtertext = text_get({ /*"worldimportfilteractive"*/ STR(3912) });
			draw_label(self, { filtertext, confirmx + confirmw / 2.0 + IntType(1), confirmy + IntType(80) + IntType(1), fa_center, fa_top, c_black, IntType(1), sInt(font_heading_big) });
			draw_label(self, { filtertext, confirmx + confirmw / 2.0, confirmy + IntType(80), fa_center, fa_top, c_yellow, IntType(1), sInt(font_heading_big) });
		}
	}
	
	void action_world_import_settings_filter_enabled(ScopeAny self, VarType value)
	{
		sVar(setting_world_import_filter_enabled) = value;
	}
	
	void action_world_import_settings_filter_mode(ScopeAny self, VarType value)
	{
		sVar(setting_world_import_filter_mode) = value;
	}
	
	void action_world_import_settings_block_select(ScopeAny self, VarType value)
	{
		sInt(world_import_settings_block_select) = value;
	}
	
	void action_world_import_settings_filter_select(ScopeAny self, VarType value)
	{
		sInt(world_import_settings_filter_select) = value;
	}
	
	void action_world_import_settings_unload_regions(ScopeAny self, VarType value)
	{
		sVar(setting_world_import_unload_regions) = value;
	}
	
	void popup_worldsettings_draw(ScopeAny self)
	{
		sBool(content_mouseon) = true;
		sVar(dw) = (RealType)sVar(dw) / 2.0;
		tab_control_switch(self);
		draw_switch(self, /*"worldsettingsunloadregions"*/ STR(3913), sVar(dx), sVar(dy), sVar(setting_world_import_unload_regions), ID_action_world_import_settings_unload_regions, /*"worldsettingsunloadregionstip"*/ STR(3914));
		tab_next(self);
		tab_control_switch(self);
		draw_switch(self, /*"worldsettingsfilterenabled"*/ STR(3915), sVar(dx), sVar(dy), sVar(setting_world_import_filter_enabled), ID_action_world_import_settings_filter_enabled);
		tab_next(self);
		if (sVar(setting_world_import_filter_enabled) > 0)
		{
			tab_control_togglebutton(self);
			togglebutton_add(self, { /*"worldsettingsfilterremove"*/ STR(3916), null_, IntType(0), sVar(setting_world_import_filter_mode) == IntType(0), ID_action_world_import_settings_filter_mode });
			togglebutton_add(self, { /*"worldsettingsfilterkeep"*/ STR(3917), null_, IntType(1), sVar(setting_world_import_filter_mode) == IntType(1), ID_action_world_import_settings_filter_mode });
			draw_togglebutton(self, /*"worldsettingsfiltermode"*/ STR(3918), sVar(dx), sVar(dy));
			tab_next(self);
			RealType listdw = sVar(content_width) - IntType(24);
			sVar(dy) += IntType(8);
			tab_control_sortlist(self, IntType(10));
			sortlist_draw(self, sInt(world_import_settings_block_list), sVar(dx), sVar(dy), listdw / 2.0 - IntType(20), sReal(tab_control_h), sInt(world_import_settings_block_select), false, text_get({ /*"worldsettingsfilterblocks"*/ STR(3919) }));
			sortlist_draw(self, sInt(world_import_settings_filter_list), sVar(dx) + listdw / 2.0 + IntType(20), sVar(dy), listdw / 2.0 - IntType(20), sReal(tab_control_h), sInt(world_import_settings_filter_select), false, text_get({ /*"worldsettingsfilterfiltered"*/ STR(3920) }));
			if (draw_button_icon(self, { /*"worldsettingsfilterright"*/ STR(3921), sVar(dx) + listdw / 2.0 - IntType(12), sVar(dy) + (sReal(tab_control_h) / 2.0) - IntType(16), IntType(24), IntType(24), false, icons_CHEVRON_RIGHT_TINY, null_, sInt(world_import_settings_block_select) == null_ }))
			{
				sortlist_add(sInt(world_import_settings_filter_list), sInt(world_import_settings_block_select));
				sortlist_remove(sInt(world_import_settings_block_list), sInt(world_import_settings_block_select));
				sortlist_update(self, sInt(world_import_settings_filter_list));
				sortlist_update(self, sInt(world_import_settings_block_list));
				ds_list_add({ sInt(setting_world_import_filter_list), sInt(world_import_settings_block_select) });
				sInt(world_import_settings_block_select) = null_;
			}
			if (draw_button_icon(self, { /*"worldsettingsfilterleft"*/ STR(3922), sVar(dx) + listdw / 2.0 - IntType(12), sVar(dy) + (sReal(tab_control_h) / 2.0) + IntType(16), IntType(24), IntType(24), false, icons_CHEVRON_LEFT_TINY, null_, sInt(world_import_settings_filter_select) == null_ }))
			{
				IntType blocklist = ObjType(obj_sortlist, sInt(world_import_settings_block_list))->list;
				IntType index;
				for (index = IntType(0); index < ds_list_size(blocklist); index++)
					if (DsList(blocklist).Value(index) > sInt(world_import_settings_filter_select))
						break;
				sortlist_add(sInt(world_import_settings_block_list), sInt(world_import_settings_filter_select), index);
				sortlist_remove(sInt(world_import_settings_filter_list), sInt(world_import_settings_filter_select));
				sortlist_update(self, sInt(world_import_settings_block_list));
				sortlist_update(self, sInt(world_import_settings_filter_list));
				ds_list_delete_value(sInt(setting_world_import_filter_list), sInt(world_import_settings_filter_select));
				sInt(world_import_settings_filter_select) = null_;
			}
			tab_next(self);
			sVar(dw) = sVar(content_width) - IntType(24);
			sVar(dy) += IntType(8);
			draw_tooltip_label(self, /*"worldsettingsfilterhelp"*/ STR(3923), icons_INFO, e_toast_INFO);
		}
	}
	
	StringType yesno(RealType val)
	{
		return ((val > 0) ? /*"yes"*/ STR(3924) : /*"no"*/ STR(3925));
	}
	
}
