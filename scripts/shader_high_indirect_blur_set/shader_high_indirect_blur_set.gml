/// shader_high_indirect_blur_set(depthsurface, normalsurface)
/// @arg depthsurface
/// @arg normalsurface

function shader_high_indirect_blur_set(depthsurface, normalsurface)
{
	texture_set_stage(sampler_map[?"uDepthBuffer"], surface_get_texture(depthsurface))
	gpu_set_texrepeat_ext(sampler_map[?"uDepthBuffer"], false)
	
	texture_set_stage(sampler_map[?"uNormalBuffer"], surface_get_texture(normalsurface))
	gpu_set_texrepeat_ext(sampler_map[?"uNormalBuffer"], false)
	
	texture_set_stage(sampler_map[?"uNoiseBuffer"], surface_get_texture(render_sample_noise_texture))
	gpu_set_texfilter_ext(sampler_map[?"uNoiseBuffer"], false)
	
	render_set_uniform("uNoiseSize", render_sample_noise_size)
	render_set_uniform_vec2("uScreenSize", render_width, render_height)
	render_set_uniform("uSamples", render_samples)
	render_set_uniform("uBlurSize", app.project_render_indirect_blur_radius)
}
