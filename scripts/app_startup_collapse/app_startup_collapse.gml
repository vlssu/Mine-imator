/// app_startup_collapse()

function app_startup_collapse()
{
	globalvar collapse_map, collapse_ani, collapse_groups;
	collapse_ani = 1
	collapse_groups = 0
	
	collapse_map = ds_map_create()
	
	collapse_register("backup")
	collapse_register("watermark")
	
	collapse_register("file")
	collapse_register("edit")
	collapse_register("tools")
	collapse_register("viewport")
	collapse_register("timeline")
	collapse_register("camera")
	
	collapse_register("texfilter")
	
	collapse_register("ssao")
	collapse_register("shadows")
	collapse_register("subsurface")
	collapse_register("indirect")
	collapse_register("reflections")
	collapse_register("glow")
	collapse_register("glow_falloff")
	collapse_register("aa")
	collapse_register("camera_effects")
	collapse_register("models_scenery")
	
	collapse_register("clouds")
	collapse_register("ground")
	collapse_register("volumetricfog")
	collapse_register("fog")
	collapse_register("fog_ambience")
	collapse_register("fog_noise")
	collapse_register("wind")
	
	collapse_register("rotatepoint")
	collapse_register("camshake")
	collapse_register("dof")
	collapse_register("dof_fringe")
	collapse_register("bloom")
	collapse_register("lensdirt")
	collapse_register("clrcor")
	collapse_register("grain")
	collapse_register("vignette")
	collapse_register("ca")
	collapse_register("distort")
	
	collapse_register("itemslot")
	
	collapse_register("material_texture")
	collapse_register("material_subsurface")
}
