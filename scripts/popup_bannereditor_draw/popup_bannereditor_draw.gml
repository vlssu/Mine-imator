/// popup_bannereditor_draw()

dy += 8
dx += 16
dw -= 32
dh -= 16
	
// Caption
draw_label(text_get(popup.name + "caption"), dx, dy + 16, fa_left, fa_middle, c_accent, 1, font_heading)

dy += 44

dx_start = dx

// Banner preview
popup.preview.update = true
setting_wind_enable = false
preview_draw(popup.preview, dx, dy, 240, 375)
setting_wind_enable = true

dx += 240 + 8

draw_separator_vertical(dx, dy + 4, 375 - 8)
dx += 8

var listy, listw;
listy = dy 
listw = dw - (dx - dx_start)

// Add layer
if (draw_button_secondary("bannereditoraddlayer", dx, dy, listw, null, icons.ADD))
{
	ds_list_add(popup.pattern_list_edit, minecraft_banner_pattern_list[|1])
	ds_list_add(popup.pattern_color_list_edit, c_minecraft_black)
	popup_bannereditor.update = true
}

listy += (36 + 8)

// Draw layers
var listystart, listh, insertpos;
listystart = listy
listh = 48 * 7
insertpos = popup.layer_move

scrollbar_draw(popup.layer_scrollbar, e_scroll.VERTICAL, dx + listw - 6, listy, listh, (ds_list_size(popup.pattern_list_edit) + 1) * 48)

if (popup.layer_scrollbar.needed)
{
	listw -= 16
	listy -= popup.layer_scrollbar.value
	
	window_scroll_focus = string(popup.layer_scrollbar)
}

scissor_start(dx, listystart, listw, listh)

// Top layer
if (mouse_y < listy && popup.layer_move != null)
{
	insertpos = ds_list_size(popup.pattern_list_edit)
	draw_box(dx, listy, listw, 48, false, c_background_secondary, 1)
	listy += 48
}

for (var i = ds_list_size(popup.pattern_list_edit) - 1; i >= 0; i--)
{
	// Skip moving layer
	if (popup.layer_move != null && popup.layer_move = i)
		continue
				
	if ((listy < listystart + listh) && (listy + 48 > listystart))
	{
		if (popup.layer_move = null)
		{
			popup_bannereditor_draw_layer(dx, listy, listw, 48, i, false)
		}
		else
		{
			if (mouse_y >= listy && mouse_y <= listy + 48)
			{
				if (mouse_y >= listy)
				{
					insertpos = i + 1
					draw_box(dx, listy, listw, 48, false, c_background_secondary, 1)
					
					listy += 48
					
					popup_bannereditor_draw_layer(dx, listy, listw, 48, i, false)
					listy += 48
					
					continue
				}
			}
			else
				popup_bannereditor_draw_layer(dx, listy, listw, 48, i, false)
		}
	}
	
	listy += 48
}

// Bottom layer
if (mouse_y >= listy && popup.layer_move != null)
{
	insertpos = 0
	draw_box(dx, listy, listw, 48, false, c_background_secondary, 1)
	listy += 48
}

if ((listy < listystart + listh) && (listy + 48 > listystart))
	popup_bannereditor_draw_layer(dx, listy, listw, 48, -1, true)

scissor_done()

draw_set_font(font_button)
var buttonx = string_width(text_get("bannereditordone")) + 28;

// Done
if (draw_button_primary("bannereditordone", dx_start + dw - buttonx, dy_start + dh - 28, null, null))
{
	if (popup.banner_edit = temp_edit)
	{
		properties.library.preview.update = true
		action_lib_model_banner(popup.banner_edit_preview.banner_base_color, ds_list_create_array(popup.pattern_list_edit), ds_list_create_array(popup.pattern_color_list_edit))
	}
	else if (popup.banner_edit.object_index = obj_bench_settings)
	{
		popup.banner_edit.banner_pattern_list = ds_list_create_array(popup.pattern_list_edit)
		popup.banner_edit.banner_color_list = ds_list_create_array(popup.pattern_color_list_edit)
		popup.banner_edit.banner_base_color = popup.banner_edit_preview.banner_base_color
		
		array_add(banner_update, popup.banner_edit)
		
		bench_settings.preview.update = true
	}
	
	instance_destroy(popup.banner_edit_preview)
	popup.banner_edit_preview = null
	
	popup_close()
}

buttonx += 10 + (string_width(text_get("bannereditorcancel")) + 28)

// Cancel
if (draw_button_secondary("bannereditorcancel", dx_start + dw - buttonx, dy_start + dh - 28, null, null))
{
	instance_destroy(popup.banner_edit_preview)
	popup.banner_edit_preview = null
	
	popup_close()
}

if (popup.layer_remove != null)
{
	ds_list_delete(popup.pattern_list_edit, popup.layer_remove)
	ds_list_delete(popup.pattern_color_list_edit, popup.layer_remove)
	
	popup.layer_remove = null
	popup_bannereditor.update = true
}

if (popup.update)
{
	if (popup.banner_edit_preview.banner_skin)
		sprite_delete(popup.banner_edit_preview.banner_skin)
	
	popup.banner_edit_preview.banner_skin = minecraft_banner_generate(popup.banner_edit_preview.banner_base_color, ds_list_create_array(popup.pattern_list_edit), ds_list_create_array(popup.pattern_color_list_edit))
	
	popup.update = false
}

// Draw moving layer
if (popup.layer_move != null)
{
	content_x = 0
	content_y = 0
	content_width = window_width
	content_height = window_height
	
	mouse_cursor = cr_size_all
	
	draw_dropshadow(mouse_x + popup.layer_move_x, mouse_y + popup.layer_move_y, listw, 48, c_black, 1)
	draw_box(mouse_x + popup.layer_move_x, mouse_y + popup.layer_move_y, listw, 48, false, c_background, 1)
	popup_bannereditor_draw_layer(mouse_x + popup.layer_move_x, mouse_y + popup.layer_move_y, listw, 48, popup.layer_move, false)
	
	// Insert layer
	if (mouse_left_released)
	{
		if (insertpos != popup.layer_move)
		{
			// Insert value
			ds_list_insert(popup.pattern_list_edit, insertpos, popup.pattern_list_edit[|popup.layer_move])
			ds_list_insert(popup.pattern_color_list_edit, insertpos, popup.pattern_color_list_edit[|popup.layer_move])
			
			// Delete old position
			var pos = popup.layer_move;
			
			if (insertpos <= popup.layer_move)
				pos++
			
			ds_list_delete(popup.pattern_list_edit, pos)
			ds_list_delete(popup.pattern_color_list_edit, pos)
		}
		
		popup.layer_move = null
		popup.update = true
		window_busy = "popup" + popup.name
	}
	
	if (mouse_y < listystart)
		popup.layer_scrollbar.value_goal -= 30
	else if (mouse_y > listystart + listh)
		popup.layer_scrollbar.value_goal += 30
}
