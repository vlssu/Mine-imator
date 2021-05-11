/// shader_high_indirect_set(depthsurface, normalsurface, normalsurface2, diffusesurf, shadowsurf, brightnesssurf)
/// @arg depthsurface
/// @arg normalsurface
/// @arg normalsurface2
/// @arg diffusesurf
/// @arg shadowsurf
/// @arg brightnesssurf

function shader_high_indirect_set(depthsurface, normalsurface, normalsurface2, diffusesurf, shadowsurf, brightnesssurf)
{
	texture_set_stage(sampler_map[?"uDepthBuffer"], surface_get_texture(depthsurface))
	texture_set_stage(sampler_map[?"uNormalBuffer"], surface_get_texture(normalsurface))
	texture_set_stage(sampler_map[?"uNormalBufferExp"], surface_get_texture(normalsurface2))
	texture_set_stage(sampler_map[?"uDiffuseBuffer"], surface_get_texture(diffusesurf))
	texture_set_stage(sampler_map[?"uLightingBuffer"], surface_get_texture(shadowsurf))
	texture_set_stage(sampler_map[?"uBrightnessBuffer"], surface_get_texture(brightnesssurf))
	texture_set_stage(sampler_map[?"uIndirectBuffer"], surface_get_texture(render_surface_indirect))
	
	render_set_uniform("uNear", cam_near)
	render_set_uniform("uFar", cam_far)
	render_set_uniform("uProjMatrix", proj_matrix)
	render_set_uniform("uProjMatrixInv", matrix_inverse(proj_matrix))
	render_set_uniform("uViewMatrix", view_proj_matrix)
	render_set_uniform("uViewMatrixInv", matrix_inverse(view_proj_matrix))
	
	render_set_uniform("uKernel", render_indirect_kernel)
	render_set_uniform("uOffset", render_indirect_offset)
	
	render_set_uniform("uEmissionRange", app.setting_render_indirect_range)
	render_set_uniform("uStepSize", 10)
	render_set_uniform_int("uStepAmount", 48)
	render_set_uniform_int("uRays", 2 + (app.setting_render_indirect_quality * 2))
}
