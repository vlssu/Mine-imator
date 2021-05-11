/// render_high_ssao()

function render_high_ssao()
{
	var depthsurf, normalsurf, brightnesssurf;
	
	// Get depth and normal information
	render_surface[1] = surface_require(render_surface[1], render_width, render_height)
	render_surface[2] = surface_require(render_surface[2], render_width, render_height)
	render_surface[3] = surface_require(render_surface[3], render_width, render_height)
	depthsurf = render_surface[1]
	normalsurf = render_surface[2]
	brightnesssurf = render_surface[3]
	surface_set_target_ext(0, depthsurf)
	surface_set_target_ext(1, normalsurf)
	surface_set_target_ext(2, brightnesssurf)
	{
		gpu_set_blendmode_ext(bm_one, bm_zero)
		
		draw_clear_alpha(c_black, 0)
		render_world_start(5000)
		render_world(e_render_mode.HIGH_SSAO_DEPTH_NORMAL)
		render_world_done()
		
		gpu_set_blendmode(bm_normal)
	}
	surface_reset_target()
	
	// Noise texture
	if (!surface_exists(render_ssao_noise))
		render_ssao_noise = render_generate_noise(4, 4)
	
	// Calculate SSAO
	render_surface_ssao = surface_require(render_surface_ssao, render_width, render_height)
	surface_set_target(render_surface_ssao)
	{
		gpu_set_texrepeat(false)
		draw_clear(c_white)
		render_shader_obj = shader_map[?shader_high_ssao]
		with (render_shader_obj)
		{
			shader_set(shader)
			shader_high_ssao_set(depthsurf, normalsurf, brightnesssurf)
		}
		draw_blank(0, 0, render_width, render_height) // Blank quad
		with (render_shader_obj)
			shader_clear()
		gpu_set_texrepeat(true)
	}
	surface_reset_target()
	
	// Blur
	repeat (setting_render_ssao_blur_passes)
	{
		var ssaosurftemp;
		render_surface[3] = surface_require(render_surface[3], render_width, render_height)
		ssaosurftemp = render_surface[3]
		
		render_shader_obj = shader_map[?shader_high_ssao_blur]
		with (render_shader_obj)
			shader_set(shader)
		
		// Horizontal
		surface_set_target(ssaosurftemp)
		{
			with (render_shader_obj)
				shader_high_ssao_blur_set(depthsurf, normalsurf, 1, 0)
			draw_surface_exists(render_surface_ssao, 0, 0)
		}
		surface_reset_target()
		
		// Vertical
		surface_set_target(render_surface_ssao)
		{
			with (render_shader_obj)
				shader_high_ssao_blur_set(depthsurf, normalsurf, 0, 1)
			draw_surface_exists(ssaosurftemp, 0, 0)
		}
		surface_reset_target()
		
		with (render_shader_obj)
			shader_clear()
	}
	gpu_set_texrepeat(true)
	
	if (setting_render_pass = e_render_pass.AO)
		render_pass_surf = surface_duplicate(render_surface_ssao)
	
	if (setting_render_pass = e_render_pass.DEPTH_U24)
		render_pass_surf = surface_duplicate(depthsurf)
	
	if (setting_render_pass = e_render_pass.NORMAL)
		render_pass_surf = surface_duplicate(normalsurf)
	
	return render_surface_ssao
}
