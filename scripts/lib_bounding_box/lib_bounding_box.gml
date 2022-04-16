/// lib_bbox()
/// @desc Functions library for making and editing bounding boxes

function bbox() constructor {
	
	frustum_state = e_frustum_state.INIT
	frustum_state_prev = frustum_state
	changed = false
	start_pos = [no_limit, no_limit, no_limit]
	end_pos = [-no_limit, -no_limit, -no_limit]
	points = array_create(8, vec3(0)) // Non axis-aligned points
	size = no_limit*2
	center = [0, 0, 0]
	
	static updatePoints = function()
	{
		// Convert points to cube
		center[X] = start_pos[X] + ((end_pos[X] - start_pos[X]) * .5)
		center[Y] = start_pos[Y] + ((end_pos[Y] - start_pos[Y]) * .5)
		center[Z] = start_pos[Z] + ((end_pos[Z] - start_pos[Z]) * .5)
		size = point_distance_3d(start_pos[X], start_pos[Y], start_pos[Z], center[X], center[Y], center[Z])
	}
	
	static reset = function()
	{
		changed = false
		start_pos = point3D(no_limit, no_limit, no_limit)
		end_pos = point3D(-no_limit, -no_limit, -no_limit)
		points = array_create(8, vec3(0))
		
		self.updatePoints()
	}
	
	static mul_matrix = function(mat)
	{
		var p, np, i, corner, xx, yy, zz;
		p = array_create(8, [0, 0, 0])
		np = array_create(8, [0, 0, 0])
		p[0] = [start_pos[X], start_pos[Y], start_pos[Z]]
		p[1] = [start_pos[X], end_pos[Y],   start_pos[Z]]
		p[2] = [end_pos[X],   end_pos[Y],   start_pos[Z]]
		p[3] = [end_pos[X],   start_pos[Y], start_pos[Z]]
		
		p[4] = [start_pos[X], start_pos[Y], end_pos[Z]]
		p[5] = [start_pos[X], end_pos[Y],   end_pos[Z]]
		p[6] = [end_pos[X],   end_pos[Y],   end_pos[Z]]
		p[7] = [end_pos[X],   start_pos[Y], end_pos[Z]]
		
		// Apply matrix
		i = 0
		repeat (8)
		{
			xx = p[i][X]
			yy = p[i][Y]
			zz = p[i][Z]
			
			np[i][X] = mat[@ 0] * xx + mat[@ 4] * yy + mat[@ 8]  * zz + mat[@ 12]
			np[i][Y] = mat[@ 1] * xx + mat[@ 5] * yy + mat[@ 9]  * zz + mat[@ 13]
			np[i][Z] = mat[@ 2] * xx + mat[@ 6] * yy + mat[@ 10] * zz + mat[@ 14]
			
			i++
		}
		
		start_pos = point3D(no_limit, no_limit, no_limit)
		end_pos = point3D(-no_limit, -no_limit, -no_limit)
		
		points[0] = np[0]
		points[1] = np[4]
		points[2] = np[1]
		points[3] = np[5]
		points[4] = np[3]
		points[5] = np[7]
		points[6] = np[2]
		points[7] = np[6]
		
		i = 0
		repeat (8)
		{
			corner = points[i]
			
			start_pos[X] = start_pos[X] < corner[X] ? start_pos[X] : corner[X]
			start_pos[Y] = start_pos[Y] < corner[Y] ? start_pos[Y] : corner[Y]
			start_pos[Z] = start_pos[Z] < corner[Z] ? start_pos[Z] : corner[Z]
			
			end_pos[X] = end_pos[X] > corner[X] ? end_pos[X] : corner[X]
			end_pos[Y] = end_pos[Y] > corner[Y] ? end_pos[Y] : corner[Y]
			end_pos[Z] = end_pos[Z] > corner[Z] ? end_pos[Z] : corner[Z]
			
			i++
		}
		
		self.updatePoints()
	}
	
	static merge = function(box)
	{
		if (!box.changed)
			return 0
		
		start_pos[X] = (start_pos[X] < box.start_pos[X] ? start_pos[X] : box.start_pos[X])//min(start_pos[X], box.start_pos[X], box.end_pos[X])
		start_pos[Y] = (start_pos[Y] < box.start_pos[Y] ? start_pos[Y] : box.start_pos[Y])//min(start_pos[Y], box.start_pos[Y], box.end_pos[Y])
		start_pos[Z] = (start_pos[Z] < box.start_pos[Z] ? start_pos[Z] : box.start_pos[Z])//min(start_pos[Z], box.start_pos[Z], box.end_pos[Z])
		
		end_pos[X] = (end_pos[X] > box.end_pos[X] ? end_pos[X] : box.end_pos[X])
		end_pos[Y] = (end_pos[Y] > box.end_pos[Y] ? end_pos[Y] : box.end_pos[Y])
		end_pos[Z] = (end_pos[Z] > box.end_pos[Z] ? end_pos[Z] : box.end_pos[Z])
		
		points[0] = [start_pos[X], start_pos[Y], start_pos[Z]]
		points[1] = [start_pos[X], start_pos[Y],   end_pos[Z]]
		points[2] = [start_pos[X],   end_pos[Y],   start_pos[Z]]
		points[3] = [start_pos[X],   end_pos[Y],   end_pos[Z]]
		points[4] = [end_pos[X], start_pos[Y], start_pos[Z]]
		points[5] = [end_pos[X], start_pos[Y],   end_pos[Z]]
		points[6] = [end_pos[X],   end_pos[Y],   start_pos[Z]]
		points[7] = [end_pos[X],   end_pos[Y], end_pos[Z]]
		
		changed = true
		self.updatePoints()
	}
	
	static copy_vbuffer = function(update)
	{
		start_pos = [vbuffer_xmin, vbuffer_ymin, vbuffer_zmin]
		end_pos = [vbuffer_xmax, vbuffer_ymax, vbuffer_zmax]
		
		changed = true
		
		if (update = undefined || update)
			self.updatePoints()
	}
	
	static set_vbuffer = function()
	{
		vbuffer_xmin = start_pos[X]
		vbuffer_ymin = start_pos[Y]
		vbuffer_zmin = start_pos[Z]
		
		vbuffer_xmax = end_pos[X]
		vbuffer_ymax = end_pos[Y]
		vbuffer_zmax = end_pos[Z]
	}
	
	static copy = function(box)
	{
		start_pos = array_copy_1d(box.start_pos)
		end_pos = array_copy_1d(box.end_pos)
		points = array_copy_1d(box.points)
		changed = box.changed
		
		self.updatePoints()
	}
	
	static updateFrustumState = function()
	{
		frustum_state_prev = frustum_state
		
		if (frustum_state = e_frustum_state.INIT)
		{
			frustum_state = e_frustum_state.PARTIAL
			return 0
		}
		
		if (size > no_limit)
			return 0
		
		var i, side, distance;
		
		i = 0
		repeat (6)
		{
			side = render_frustum.p[i]

			distance = ((side[X] * center[X]) + (side[Y] * center[Y]) + (side[Z] * center[Z]) + side[W])
			
			if (abs(distance) <= size)
			{
				frustum_state = e_frustum_state.PARTIAL
				return 0
			}
			else if (distance < -size)
			{
				frustum_state = e_frustum_state.HIDDEN
				return 0
			}
			
			i++
		}
		
		frustum_state = e_frustum_state.VISIBLE
	}
}

function bbox_update_visible()
{
	// Update models
	for (var i = 0; i < ds_list_size(app.project_model_list); i++)
	{
		with (app.project_model_list[|i])
		{
			if (type = e_tl_type.MODEL && temp.model != null && temp.model.model_format = e_model_format.BLOCK)
				continue
			
			if (model_timeline_list = null)
				continue
			
			bounding_box_matrix.updateFrustumState()
			bounding_box_update = false
			
			if (bounding_box_matrix.frustum_state = e_frustum_state.PARTIAL)
			{
				for (var j = 0; j < ds_list_size(model_timeline_list); j++)
				{
					var tl = model_timeline_list[|j];
					
					//if (!tl.render_visible)
					//	continue
					
					tl.bounding_box_update = true
				}
			}
			else
			{
				for (var j = 0; j < ds_list_size(model_timeline_list); j++)
				{
					var tl = model_timeline_list[|j];
					
					//if (!tl.render_visible)
					//	continue
					
					tl.bounding_box_update = false
					tl.bounding_box_matrix.frustum_state = bounding_box_matrix.frustum_state
				}
			}
		}
	}
	
	// Update timelines
	with (obj_timeline)
	{
		if (!bounding_box_update || 
			(type = e_tl_type.MODEL && temp.model != null && temp.model.model_format = e_model_format.MIMODEL) ||
			type = e_tl_type.CHARACTER ||
			type = e_tl_type.SPECIAL_BLOCK ||
			type = e_tl_type.FOLDER ||
			type = e_tl_type.BACKGROUND ||
			type = e_tl_type.AUDIO ||
			type = e_tl_type.CAMERA)
			continue
		
		// Objects in scenery don't use template objects
		if (part_root = null && temp != null && temp.object_index = obj_template)
		{
			if (type = e_tl_type.SCENERY && temp.scenery = null)
				continue
			else
			{
				if (type = e_tl_type.SCENERY && temp.scenery != null && !temp.scenery.ready)
					continue
			}
		}
		
		bounding_box_matrix.updateFrustumState()
		
		if ((bounding_box_matrix.frustum_state = e_frustum_state.VISIBLE || bounding_box_matrix.frustum_state = e_frustum_state.HIDDEN))
			continue
		
		if ((bounding_box_matrix.frustum_state_prev = bounding_box_matrix.frustum_state))
			continue
	}
}

function frustum() constructor {
	
	active = true
	vbuffer = null
	mat = null
	corners = null
	
	near = 0
	far = 0
	matView = null
	matProj = null
	matBias = null
	clipEndDepth = 0
	
	self.reset()
	
	static reset = function()
	{
		p[0] = [ 1,  0,  0, 1] // Left
		p[1] = [-1,  0,  0, 1] // Right
		p[2] = [ 0,  1,  0, 1] // Bottom
		p[3] = [ 0, -1,  0, 1] // Top
		p[4] = [ 0,  0,  1, 1] // Behind view
		p[5] = [ 0,  0, -1, 1] // Beyond view
	}
	
	static build = function(matVP, ortho = false)
	{
		mat = matVP
		var matVPt = matrix_transpose(mat);
		self.reset()
		
		for (var i = 0; i < 6; i++)
		{
			var mul = vec4_mul_matrix(p[i], matVPt);
			p[i] = vec4_div(mul, vec3_length(vec3(mul[X], mul[Y], mul[Z])))
		}
		
		var corners_v4, near;
		near = (ortho ? -1 : 0)
		corners_v4 = [
			[-1,  1,    1, 1], // Far
            [ 1,  1,    1, 1],
            [ 1, -1,    1, 1],
            [-1, -1,    1, 1],
            [-1,  1, near, 1], // Near
            [ 1,  1, near, 1],
            [ 1, -1, near, 1],
            [-1, -1, near, 1]
		];
		
		for (var i = 0; i < 8; i++)
			corners[i] = vec4_homogenize(vec4_mul_matrix(corners_v4[i], matrix_inverse(mat)))
	}
	
	static add_triangle = function(corner1, corner2, corner3) {
		vbuffer_add_triangle(corners[corner1], corners[corner2], corners[corner3], [0, 0], [0, 0], [0, 0])
	}
	
	static build_vbuffer = function(color = c_white)
	{
		if (vbuffer != null)
			vbuffer_destroy(vbuffer)
		
		// Generate mesh
		vertex_brightness = 1
		vertex_alpha = 0.5
		
		vbuffer = vbuffer_start()
		
		vertex_rgb = color_multiply(color, c_dkgrey)
		add_triangle(0, 4, 5); add_triangle(5, 1, 0); // X-
		
		vertex_rgb = color_multiply(color, c_ltgrey)
		add_triangle(7, 3, 2); add_triangle(2, 6, 7); // X+
		
		vertex_rgb = color_multiply(color, c_grey)
		add_triangle(2, 1, 5); add_triangle(5, 6, 2); // Y-
		add_triangle(7, 4, 0); add_triangle(0, 3, 7); // Y+
		
		vertex_rgb = color
		add_triangle(0, 1, 2); add_triangle(2, 3, 0); // Z-
		add_triangle(4, 7, 6); add_triangle(6, 5, 4); // Z+
		
		vbuffer_done()
		
		vertex_brightness = 0
		vertex_alpha = 1
		vertex_rgb = c_white
	}
}