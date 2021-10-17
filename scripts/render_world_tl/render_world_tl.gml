/// render_world_tl()
/// @desc Renders the 3D model of the timeline instance.

function render_world_tl()
{
	// Frustum culled
	if (render_frustum.active && (bounding_box_matrix.frustum_state = e_frustum_state.HIDDEN))
		return 0
	
	// No 3D representation?
	if (type = e_tl_type.CHARACTER ||
		type = e_tl_type.SPECIAL_BLOCK ||
		type = e_tl_type.FOLDER ||
		type = e_tl_type.BACKGROUND ||
		type = e_tl_type.AUDIO)
		return 0
	
	if (type = e_tl_type.MODEL && (temp.model = null || temp.model.model_format = e_model_format.MIMODEL))
		return 0
	
	// Invisible?
	if (!tl_get_visible())
		return 0
	
	// Only render glow effect?
	if ((glow && only_render_glow) && render_mode != e_render_mode.COLOR_GLOW)
		return 0
	
	// Not registered on shadow depth testing?
	if (!shadows &&
		(render_mode = e_render_mode.HIGH_LIGHT_SUN_DEPTH ||
		 render_mode = e_render_mode.HIGH_LIGHT_SPOT_DEPTH ||
		 render_mode = e_render_mode.HIGH_LIGHT_POINT_DEPTH))
		return 0
	
	// Click mode
	if (render_mode = e_render_mode.CLICK)
	{
		if (selected || lock || !tl_update_list_filter(id)) // Already selected when clicking?
			return 0
		
		render_set_uniform_color("uReplaceColor", id, 1)
	}
	
	if (render_mode = e_render_mode.SCENE_TEST)
		render_set_uniform_color("uReplaceColor", c_white, 1)
	
	// Outlined?
	else if (render_mode = e_render_mode.SELECT && !parent_is_selected && !selected)
		return 0
	
	// Box for clicking
	if (type = e_tl_type.PARTICLE_SPAWNER ||
		type = e_tl_type.SPOT_LIGHT ||
		type = e_tl_type.POINT_LIGHT ||
		type = e_tl_type.CAMERA)
	{
		if (render_mode = e_render_mode.CLICK)
		{
			render_set_texture(shape_texture)
			vbuffer_render(render_click_box, world_pos)
		}
		
		if (type != e_tl_type.PARTICLE_SPAWNER) // Only proceed with rendering for particles
			return 0
	}
	
	if ((value_inherit[e_value.ALPHA] * 1000) = 0)
		return 0
	
	// Set render options
	render_set_culling(!backfaces)
	shader_texture_filter_linear = texture_blur
	shader_texture_filter_mipmap = (app.project_render_texture_filtering && texture_filtering)
	
	shader_blend_color = value_inherit[e_value.RGB_MUL]
	
	if (foliage_tint)
		shader_blend_color = color_multiply(shader_blend_color, app.background_foliage_color)
	
	shader_blend_alpha = value_inherit[e_value.ALPHA]
	render_set_uniform_color("uBlendColor", shader_blend_color, shader_blend_alpha)
	
	if (colors_ext != shader_uniform_color_ext ||
        value_inherit[e_value.RGB_ADD] != shader_uniform_rgb_add ||
		value_inherit[e_value.HSB_ADD] != shader_uniform_hsb_add ||
        value_inherit[e_value.RGB_SUB] != shader_uniform_rgb_sub ||
        value_inherit[e_value.HSB_SUB] != shader_uniform_hsb_sub ||
        value_inherit[e_value.HSB_MUL] != shader_uniform_hsb_mul ||
        value_inherit[e_value.MIX_COLOR] != shader_uniform_mix_color ||
        value_inherit[e_value.MIX_PERCENT] != shader_uniform_mix_percent)
	{
		shader_uniform_color_ext = colors_ext
		shader_uniform_rgb_add = value_inherit[e_value.RGB_ADD]
		shader_uniform_hsb_add = value_inherit[e_value.HSB_ADD]
        shader_uniform_rgb_sub = value_inherit[e_value.RGB_SUB]
        shader_uniform_hsb_sub = value_inherit[e_value.HSB_SUB]
        shader_uniform_hsb_mul = value_inherit[e_value.HSB_MUL]
        shader_uniform_mix_color = value_inherit[e_value.MIX_COLOR]
        shader_uniform_mix_percent = value_inherit[e_value.MIX_PERCENT]

		render_set_uniform_int("uColorsExt", shader_uniform_color_ext)
		render_set_uniform_color("uRGBAdd", shader_uniform_rgb_add, 1)
		render_set_uniform_color("uHSBAdd", shader_uniform_hsb_add, 1)
		render_set_uniform_color("uRGBSub", shader_uniform_rgb_sub, 1)
		render_set_uniform_color("uHSBSub", shader_uniform_hsb_sub, 1)
		render_set_uniform_color("uHSBMul", shader_uniform_hsb_mul, 1)
		render_set_uniform_color("uMixColor", shader_uniform_mix_color, shader_uniform_mix_percent)
	}
	
	if (value_inherit[e_value.BRIGHTNESS] != shader_uniform_brightness)
	{
		shader_uniform_brightness = value_inherit[e_value.BRIGHTNESS]
		render_set_uniform("uBrightness", shader_uniform_brightness)
	}
	
	if (value_inherit[e_value.METALLIC] != shader_uniform_metallic)
	{
		shader_uniform_metallic = value_inherit[e_value.METALLIC]
		render_set_uniform("uMetallic", shader_uniform_metallic)
	}
	
	if (value_inherit[e_value.ROUGHNESS] != shader_uniform_roughness)
	{
		shader_uniform_roughness = value_inherit[e_value.ROUGHNESS]
		render_set_uniform("uRoughness", shader_uniform_roughness)
	}
	
	if (wind != shader_uniform_wind)
	{
		shader_uniform_wind = wind
		render_set_uniform("uWindEnable", shader_uniform_wind)
	}
	
	if (wind_terrain != shader_uniform_wind_terrain)
	{
		shader_uniform_wind_terrain = wind_terrain
		render_set_uniform("uWindTerrain", shader_uniform_wind_terrain)
	}
	
	if (fog != shader_uniform_fog)
	{
		shader_uniform_fog = fog
		render_set_uniform_int("uFogShow", shader_uniform_fog)
	}
	
	if (ssao != shader_uniform_ssao)
    {
        shader_uniform_ssao = ssao
		render_set_uniform_int("uSSAOEnable", shader_uniform_ssao)
    }

	if (value_inherit[e_value.SUBSURFACE] != shader_uniform_sss ||
        value_inherit[e_value.SUBSURFACE_RADIUS_RED] != shader_uniform_sss_red ||
        value_inherit[e_value.SUBSURFACE_RADIUS_GREEN] != shader_uniform_sss_green ||
        value_inherit[e_value.SUBSURFACE_RADIUS_BLUE] != shader_uniform_sss_blue ||
        value_inherit[e_value.SUBSURFACE_COLOR] != shader_uniform_sss_color)
	{
        shader_uniform_sss = value_inherit[e_value.SUBSURFACE]
		shader_uniform_sss_red = value_inherit[e_value.SUBSURFACE_RADIUS_RED]
		shader_uniform_sss_green = value_inherit[e_value.SUBSURFACE_RADIUS_GREEN]
		shader_uniform_sss_blue = value_inherit[e_value.SUBSURFACE_RADIUS_BLUE]
		shader_uniform_sss_color = value_inherit[e_value.SUBSURFACE_COLOR]
		
        if (shader_uniform_sss != 0)
        {
            render_set_uniform("uSSS", shader_uniform_sss)
            render_set_uniform_vec3("uSSSRadius", shader_uniform_sss_red,
                                                    shader_uniform_sss_green,
                                                    shader_uniform_sss_blue)
            render_set_uniform_color("uSSSColor", shader_uniform_sss_color, 1.0) 
        }
        else
        {
            render_set_uniform("uSSS", 0)
            render_set_uniform_vec3("uSSSRadius", 1.0, 1.0, 1.0)
            render_set_uniform_color("uSSSColor", c_white, 0.0)
        }
	}
	
	var prevblend = null;
	
	// Object blend mode
	if (blend_mode != "normal" && (render_mode = e_render_mode.COLOR || render_mode = e_render_mode.COLOR_FOG || render_mode = e_render_mode.COLOR_FOG_LIGHTS || render_mode = e_render_mode.ALPHA_FIX))
	{
		if (render_mode = e_render_mode.ALPHA_FIX)
			return 0
		
		prevblend = gpu_get_blendmode()
		
		var blend = blend_mode_map[? blend_mode];
		if (is_array(blend))
			gpu_set_blendmode_ext(blend[0], blend[1])
		else
			gpu_set_blendmode(blend)
	}
	
	// Glow
	if (glow != shader_uniform_glow ||
        glow_texture != shader_uniform_glow_texture ||
        value_inherit[e_value.GLOW_COLOR] != shader_uniform_glow_color)
	{
        shader_uniform_glow = glow
        shader_uniform_glow_texture = glow_texture
        shader_uniform_glow_color = value_inherit[e_value.GLOW_COLOR]
        
        if (shader_uniform_glow)
        {
            render_set_uniform_int("uGlow", 1)
            render_set_uniform_int("uGlowTexture", glow_texture)
            render_set_uniform_color("uGlowColor", shader_uniform_glow_color, 1)
            
            if (only_render_glow)
            {
                prevblend = gpu_get_blendmode()
                gpu_set_blendmode(bm_add)
            }
        }
        else
        {
            render_set_uniform_int("uGlow", 0)
            render_set_uniform_int("uGlowTexture", 0)
            render_set_uniform_color("uGlowColor", c_black, 0)
        }
	}
	
	// Multiply for sun color
	if (render_mode = e_render_mode.HIGH_LIGHT_SUN_COLOR)
	{
		prevblend = gpu_get_blendmode()
		gpu_set_blendmode_ext(bm_zero, bm_src_color)
	}
	
	// Render
	if (type != e_tl_type.PARTICLE_SPAWNER)
	{
		matrix_set(matrix_world, matrix_render)
		
		// Reset material textures for other timelines
		if (type != e_tl_type.SCENERY && type != e_tl_type.BLOCK)
		{
			render_set_texture(spr_default_material, "Material")
			render_set_texture(spr_default_normal, "Normal")
		}
		
		switch (type)
		{
			case e_tl_type.BODYPART:
			{
				if (model_part = null || render_res_diffuse = null)
					break
				
				render_world_model_part(model_part, render_res_diffuse, temp.model_texture_name_map, model_shape_vbuffer_map, temp.model_color_map, temp.model_shape_hide_list, temp.model_shape_texture_name_map, self)
				break
			}
			
			case e_tl_type.SCENERY:
			case e_tl_type.BLOCK:
			{
				if (type = e_tl_type.BLOCK)
					render_world_block(temp.scenery_chunk_array, [render_res_diffuse, render_res_material, render_res_normal], true, temp.block_repeat_enable ? temp.block_repeat : vec3(1), temp)
				else if (temp.scenery)
					render_world_scenery(temp.scenery, [render_res_diffuse, render_res_material, render_res_normal], temp.block_repeat_enable, temp.block_repeat)
				break
			}
			
			case e_tl_type.ITEM:
			{
				if (item_vbuffer = null)
					render_world_item(temp.item_vbuffer, temp.item_3d, temp.item_face_camera, temp.item_bounce, temp.item_spin, [item_res, item_material_res, item_normal_res])
				else
					render_world_item(item_vbuffer, temp.item_3d, temp.item_face_camera, temp.item_bounce, temp.item_spin, [item_res, item_material_res, item_normal_res])
				break
			}
			
			case e_tl_type.TEXT:
			{
				var font = value[e_value.TEXT_FONT];
				if (font = null)
					font = temp.text_font
				render_world_text(text_vbuffer, text_texture, temp.text_face_camera, text_res)
				break
			}
			
			case e_tl_type.MODEL:
			{
				if (temp.model != null)
				{
					var res = value_inherit[e_value.TEXTURE_OBJ];
					if (res = null)
						res = temp.model_tex
					if (res = null || res.block_sheet_texture = null)
						res = mc_res
					render_world_block(temp.model.block_vbuffer, res)
					
					with (temp)
						res = temp_get_model_texobj(other.value_inherit[e_value.TEXTURE_OBJ])
					render_world_block_map(temp.model.model_block_map, res)
				}
				break
			}
			
			default: // Shapes
			{
				var tex, matres, texmat, normtex;
				with (temp)
				{
					tex = temp_get_shape_tex(temp_get_shape_texobj(other.value_inherit[e_value.TEXTURE_OBJ]))
					
					matres = temp_get_shape_tex_material_obj(other.value_inherit[e_value.TEXTURE_MATERIAL_OBJ])
					texmat = temp_get_shape_tex(matres, spr_default_material)
					
					if (matres != null)
						render_set_uniform_int("uMaterialUseGlossiness", matres.material_uses_glossiness)
					else
						render_set_uniform_int("uMaterialUseGlossiness", 0)
					
					normtex = temp_get_shape_tex(temp_get_shape_tex_normal_obj(other.value_inherit[e_value.TEXTURE_NORMAL_OBJ]), spr_default_normal)
				}
				
				render_world_shape(temp.type, temp.shape_vbuffer, temp.shape_face_camera, [tex, texmat, normtex])
				break
			}
		}
	} 
	else if (render_particles) 
	{
		for (var p = 0; p < ds_list_size(particle_list); p++)
			with (particle_list[|p])
				render_world_particle()
	}
	
	matrix_world_reset()
	shader_texture_surface = false
	
	if (prevblend != null)
		gpu_set_blendmode(prevblend)
}
