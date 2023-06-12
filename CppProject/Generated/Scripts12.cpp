/*
	NOTE:
	This file was autogenerated by CppGen, changes may be overwritten and forever lost!
	Modify at your own risk!
	
	[ Generated on 2023.06.11 00:02:52 ]
*/

#include "Scripts.hpp"

namespace CppProject
{
	void action_tl_keyframes_remove(ScopeAny self)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
			{
				history_restore_keyframes(self);
				history_restore_tl_select(self);
			}
			
		}
		else
		{
			if (!(sReal(history_redo) > 0))
			{
				withOne (obj_history, history_set(self, ID_action_tl_keyframes_remove), self->id)
				{
					history_save_keyframes(self);
					history_save_tl_select(ScopeAny(self));
				}
				
			}
			tl_keyframes_remove();
		}
		
		withAll (obj_timeline, self->id)
			tl_update_values(ScopeAny(self));
		
		tl_update_matrix(self);
		tl_update_length();
		app_update_tl_edit(self);
	}
	
	void action_tl_keyframes_select_area(ScopeAny self, RealType stl, RealType etl, RealType spos, RealType epos)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
				history_restore_tl_select(self);
			
		}
		else
			if (sReal(history_redo) > 0)
			{
				withOne (obj_history, global::history_data, self->id)
					history_restore_tl_select_new(self);
				
			}
			else
			{
				IntType hobj;
				BoolType ctrl;
				hobj = history_set(self, ID_action_tl_keyframes_select_area);
				ctrl = keyboard_check(vk_control);
				withOne (obj_history, hobj, self->id)
					history_save_tl_select(ScopeAny(self));
				
				for (RealType t = stl; t <= etl; t++)
				{
					VarType tl = DsList(sInt(tree_visible_list)).Value(t);
					if (idVar(tl, lock) > 0)
						continue;
					for (IntType k = IntType(0); k < ds_list_size(idInt(tl, keyframe_list)); k++)
					{
						VarType kf;
						RealType len;
						kf = DsList(idInt(tl, keyframe_list)).Value(k);
						len = tl_keyframe_length(kf);
						if ((!ctrl && ObjType(obj_keyframe, kf)->selected > 0) || ObjType(obj_keyframe, kf)->position + len < spos)
							continue;
						if (ObjType(obj_keyframe, kf)->position > epos)
							break;
						if (ctrl)
							tl_keyframe_deselect(kf);
						else
							tl_keyframe_select(kf);
						
					}
				}
				withOne (obj_history, hobj, self->id)
					history_save_tl_select_new(ScopeAny(self));
				
			}
		
		
		app_update_tl_edit(self);
	}
	
	void action_tl_keyframes_sound_resize(ScopeAny self)
	{
		RealType movex = sVar(timeline_mouse_pos) - sVar(timeline_sound_resize_mouse_pos);
		withAll (obj_keyframe, self->id)
		{
			if (!(self->selected > 0) || self->sound_resize_index < IntType(0))
				continue;
			self->new_position = max({ IntType(0), self->sound_resize_pos + movex });
			self->new_start = self->sound_resize_start + movex / global::_app->project_tempo;
			if (self->new_start < IntType(0) || self->position == self->new_position)
				continue;
			ds_list_delete_value(idInt(self->timeline, keyframe_list), self->id);
		}
		
		withAll (obj_keyframe, self->id)
		{
			if (!(self->selected > 0) || self->sound_resize_index < IntType(0) || self->new_start < IntType(0) || self->position == self->new_position)
				continue;
			self->value[e_value_SOUND_START] = self->new_start;
			withOne (Object, self->timeline, self->id)
			{
				tl_keyframe_add(self, ObjType(obj_keyframe, self.otherId)->new_position, self.otherId);
				sBool(update_matrix) = true;
			}
			
		}
		
	}
	
	void action_tl_keyframes_sound_resize_done(ScopeAny self)
	{
		withAll (obj_timeline, self->id)
			self->update_values = false;
		
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
			{
				for (IntType k = IntType(0); k < self->kf_resize_amount; k++)
				{
					withOne (obj_keyframe, DsList(idInt(save_id_find(self->kf_resize_tl_save_id.Value(k)), keyframe_list)).Value(sArr(kf_resize_new_index).Value(k)), self->id)
					{
						self->new_position = ObjType(obj_history, self.otherId)->kf_resize_old_pos.Value(k);
						if (self->position == self->new_position)
							continue;
						self->value[e_value_SOUND_START] = ObjType(obj_history, self.otherId)->kf_resize_old_start.Value(k);
						for (IntType a = IntType(0); a < ObjType(obj_history, self.otherId)->kf_resize_amount; a++)
							if (save_id_find(ObjType(obj_history, self.otherId)->kf_resize_tl_save_id.Value(a)) == self->timeline && ObjType(obj_history, self.otherId)->kf_resize_new_index.Value(a) > ds_list_find_index(idInt(self->timeline, keyframe_list), self->id))
								ObjType(obj_history, self.otherId)->kf_resize_new_index[a]--;
						ds_list_delete_value(idInt(self->timeline, keyframe_list), self->id);
						idBool(self->timeline, update_values) = true;
					}
					
				}
			}
			
		}
		else
			if (sReal(history_redo) > 0)
			{
				withOne (obj_history, global::history_data, self->id)
				{
					for (IntType k = IntType(0); k < self->kf_resize_amount; k++)
					{
						withOne (obj_keyframe, DsList(idInt(save_id_find(self->kf_resize_tl_save_id.Value(k)), keyframe_list)).Value(sArr(kf_resize_old_index).Value(k)), self->id)
						{
							self->new_position = ObjType(obj_history, self.otherId)->kf_resize_new_pos.Value(k);
							if (self->position == self->new_position)
								continue;
							self->value[e_value_SOUND_START] = ObjType(obj_history, self.otherId)->kf_resize_new_start.Value(k);
							for (IntType a = IntType(0); a < ObjType(obj_history, self.otherId)->kf_resize_amount; a++)
								if (save_id_find(ObjType(obj_history, self.otherId)->kf_resize_tl_save_id.Value(a)) == self->timeline && ObjType(obj_history, self.otherId)->kf_resize_old_index.Value(a) > ds_list_find_index(idInt(self->timeline, keyframe_list), self->id))
									ObjType(obj_history, self.otherId)->kf_resize_old_index[a]--;
							ds_list_delete_value(idInt(self->timeline, keyframe_list), self->id);
							idBool(self->timeline, update_values) = true;
						}
						
					}
				}
				
			}
			else
			{
				withOne (obj_history, history_set(self, ID_action_tl_keyframes_sound_resize_done), self->id)
				{
					self->kf_resize_amount = IntType(0);
					withAll (obj_keyframe, self->id)
					{
						if (!(self->selected > 0) || self->sound_resize_index < IntType(0))
							continue;
						ObjType(obj_history, self.otherId)->kf_resize_tl_save_id[ObjType(obj_history, self.otherId)->kf_resize_amount] = save_id_get(self->timeline);
						ObjType(obj_history, self.otherId)->kf_resize_old_index[ObjType(obj_history, self.otherId)->kf_resize_amount] = self->sound_resize_index;
						ObjType(obj_history, self.otherId)->kf_resize_old_pos[ObjType(obj_history, self.otherId)->kf_resize_amount] = self->sound_resize_pos;
						ObjType(obj_history, self.otherId)->kf_resize_old_start[ObjType(obj_history, self.otherId)->kf_resize_amount] = self->sound_resize_start;
						ObjType(obj_history, self.otherId)->kf_resize_new_index[ObjType(obj_history, self.otherId)->kf_resize_amount] = ds_list_find_index(idInt(self->timeline, keyframe_list), self->id);
						ObjType(obj_history, self.otherId)->kf_resize_new_pos[ObjType(obj_history, self.otherId)->kf_resize_amount] = self->position;
						ObjType(obj_history, self.otherId)->kf_resize_new_start[ObjType(obj_history, self.otherId)->kf_resize_amount] = self->value.Value(e_value_SOUND_START);
						ObjType(obj_history, self.otherId)->kf_resize_amount++;
						idBool(self->timeline, update_values) = true;
					}
					
				}
				
				sVar(window_busy) = /*""*/ STR(0);
			}
		
		
		if (sReal(history_undo) > 0 || sReal(history_redo) > 0)
		{
			withAll (obj_keyframe, self->id)
			{
				if (!(self->selected > 0) || self->position == self->new_position)
					continue;
				withOne (Object, self->timeline, self->id)
					tl_keyframe_add(self, ObjType(obj_keyframe, self.otherId)->new_position, self.otherId);
				
			}
			
		}
		tl_update_length();
		withAll (obj_timeline, self->id)
			if (self->update_values)
				tl_update_values(ScopeAny(self));
		
	}
	
	void action_tl_keyframes_sound_resize_start(ScopeAny self)
	{
		withAll (obj_keyframe, self->id)
		{
			if (!(self->selected > 0))
				continue;
			if (idVar(self->timeline, type) != e_tl_type_AUDIO || self->value.Value(e_value_SOUND_OBJ) == null_ || !idBool(self->value.Value(e_value_SOUND_OBJ), ready))
			{
				self->sound_resize_index = null_;
				continue;
			}
			self->sound_resize_index = ds_list_find_index(idInt(self->timeline, keyframe_list), self->id);
			self->sound_resize_pos = self->position;
			self->sound_resize_start = self->value.Value(e_value_SOUND_START);
		}
		
		sVar(timeline_sound_resize_mouse_pos) = sVar(timeline_mouse_pos);
		sVar(window_busy) = /*"timelineresizesounds"*/ STR(46);
	}
	
	void action_tl_keyframe_create(ScopeAny self, VarType timeline, VarType position)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
				history_restore_tl_select(self);
			
			withOne (Object, save_id_find(ObjType(obj_history, global::history_data)->tl_save_id), self->id)
			{
				withOne (obj_keyframe, DsList(sInt(keyframe_list)).Value(ObjType(obj_history, global::history_data)->kf_index), self->id)
					instance_destroy(ScopeAny(self));
				
				tl_update_values(self);
				sBool(update_matrix) = true;
				tl_update_matrix(self);
			}
			
		}
		else
		{
			VarType tl, pos;
			IntType kf, hobj;
			if (sReal(history_redo) > 0)
			{
				tl = save_id_find(ObjType(obj_history, global::history_data)->tl_save_id);
				pos = ObjType(obj_history, global::history_data)->position;
			}
			else
			{
				tl = timeline;
				pos = position;
				hobj = history_set(self, ID_action_tl_keyframe_create);
				withOne (obj_history, hobj, self->id)
				{
					ObjType(obj_history, self->id)->tl_save_id = save_id_get(tl);
					ObjType(obj_history, self->id)->position = pos;
					history_save_tl_select(ScopeAny(self));
				}
				
			}
			
			tl_deselect_all();
			withOne (Object, tl, self->id)
			{
				tl_select(self);
				kf = tl_keyframe_add(self, pos);
				sBool(update_matrix) = true;
				tl_update_matrix(self);
			}
			
			if (!(sReal(history_redo) > 0))
				ObjType(obj_history, hobj)->kf_index = ds_list_find_index(idInt(tl, keyframe_list), kf);
		}
		
		app_update_tl_edit(self);
		tl_update_length();
	}
	
	RealType action_tl_keyframe_deselect(ScopeAny self, VarType timeline, IntType keyframe)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (Object, save_id_find(ObjType(obj_history, global::history_data)->tl_save_id), self->id)
				tl_keyframe_select(DsList(sInt(keyframe_list)).Value(ObjType(obj_history, global::history_data)->kf_index));
			
		}
		else
		{
			VarType tl;
			IntType kf;
			if (sReal(history_redo) > 0)
			{
				tl = save_id_find(ObjType(obj_history, global::history_data)->tl_save_id);
				kf = DsList(idInt(tl, keyframe_list)).Value(ObjType(obj_history, global::history_data)->kf_index);
			}
			else
			{
				tl = timeline;
				kf = keyframe;
				if (!(ObjType(obj_keyframe, kf)->selected > 0))
					return IntType(0);
				withOne (obj_history, history_set(self, ID_action_tl_keyframe_deselect), self->id)
				{
					self->tl_save_id = save_id_get(tl);
					self->kf_index = ds_list_find_index(idInt(tl, keyframe_list), kf);
				}
				
			}
			
			tl_keyframe_deselect(kf);
		}
		
		app_update_tl_edit(self);
		return 0.0;
	}
	
	void action_tl_keyframe_next(ScopeAny self)
	{
		VarType pos = no_limit;
		withAll (obj_timeline, self->id)
		{
			if (!(self->selected > 0) && global::tl_edit_amount > IntType(0))
				continue;
			if (self->keyframe_next != null_ && (ObjType(obj_keyframe, self->keyframe_next)->position > global::_app->timeline_marker))
				pos = min({ pos, ObjType(obj_keyframe, self->keyframe_next)->position });
		}
		
		if (pos != no_limit)
			sVar(timeline_marker) = pos;
	}
	
	void action_tl_keyframe_previous(ScopeAny self)
	{
		VarType pos, prevpos;
		IntType index;
		pos = IntType(0);
		prevpos = IntType(0);
		withAll (obj_timeline, self->id)
		{
			if (!(self->selected > 0) && global::tl_edit_amount > IntType(0))
				continue;
			if (self->keyframe_current != null_ && (ObjType(obj_keyframe, self->keyframe_current)->position != global::_app->timeline_marker))
				pos = max({ pos, ObjType(obj_keyframe, self->keyframe_current)->position });
		}
		
		withAll (obj_timeline, self->id)
		{
			if (!(self->selected > 0) && global::tl_edit_amount > IntType(0))
				continue;
			if (self->keyframe_current != null_)
			{
				index = ds_list_find_index(self->keyframe_list, self->keyframe_current);
				if (index > IntType(0))
					prevpos = max({ prevpos, ObjType(obj_keyframe, DsList(self->keyframe_list).Value(index - IntType(1)))->position });
			}
		}
		
		if (prevpos > pos)
			pos = prevpos;
		sVar(timeline_marker) = pos;
	}
	
	RealType action_tl_keyframe_select(ScopeAny self, VarType timeline, IntType keyframe)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (Object, save_id_find(ObjType(obj_history, global::history_data)->tl_save_id), self->id)
				tl_keyframe_deselect(DsList(sInt(keyframe_list)).Value(ObjType(obj_history, global::history_data)->kf_index));
			
		}
		else
		{
			VarType tl;
			IntType kf;
			if (sReal(history_redo) > 0)
			{
				tl = save_id_find(ObjType(obj_history, global::history_data)->tl_save_id);
				kf = DsList(idInt(tl, keyframe_list)).Value(ObjType(obj_history, global::history_data)->kf_index);
			}
			else
			{
				tl = timeline;
				kf = keyframe;
				if (ObjType(obj_keyframe, kf)->selected > 0)
					return IntType(0);
				withOne (obj_history, history_set(self, ID_action_tl_keyframe_select), self->id)
				{
					self->tl_save_id = save_id_get(tl);
					self->kf_index = ds_list_find_index(idInt(tl, keyframe_list), kf);
				}
				
			}
			
			tl_keyframe_select(kf);
		}
		
		app_update_tl_edit(self);
		return 0.0;
	}
	
	void action_tl_keyframe_select_single(ScopeAny self, VarType timeline, IntType keyframe)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
				history_restore_tl_select(self);
			
		}
		else
		{
			VarType tl;
			IntType kf;
			if (sReal(history_redo) > 0)
			{
				withOne (obj_history, global::history_data, self->id)
				{
					tl = save_id_find(ObjType(obj_history, global::history_data)->tl_save_id);
					kf = DsList(idInt(tl, keyframe_list)).Value(ObjType(obj_history, global::history_data)->kf_index);
				}
				
			}
			else
			{
				tl = timeline;
				kf = keyframe;
				withOne (obj_history, history_set(self, ID_action_tl_keyframe_select_single), self->id)
				{
					self->tl_save_id = save_id_get(tl);
					self->kf_index = ds_list_find_index(idInt(tl, keyframe_list), kf);
					history_save_tl_select(ScopeAny(self));
				}
				
			}
			
			tl_deselect_all();
			tl_keyframe_select(kf);
		}
		
		app_update_tl_edit(self);
	}
	
	void action_tl_left()
	{
		global::_app->timeline_marker_move -= (RealType)global::_app->project_tempo / gmlGlobal::room_speed;
		if (global::_app->setting_timeline_frame_snap > 0 || keyboard_check(vk_control))
			global::_app->timeline_marker = round(global::_app->timeline_marker_move);
		else
			global::_app->timeline_marker = global::_app->timeline_marker_move;
		
		if (global::_app->timeline_repeat > 0)
		{
			if (global::_app->timeline_region_end != null_)
			{
				if (global::_app->timeline_marker < global::_app->timeline_region_start)
					global::_app->timeline_marker = global::_app->timeline_region_end;
			}
			else
				if (global::_app->timeline_marker < IntType(0))
					global::_app->timeline_marker = global::_app->timeline_length;
			
		}
		global::_app->timeline_marker = max({ IntType(0), global::_app->timeline_marker });
		global::_app->timeline_marker_move = max({ IntType(0), global::_app->timeline_marker_move });
	}
	
	RealType action_tl_load_loop(ScopeAny self, VarType fn)
	{
		if (sReal(history_undo) > 0)
		{
			tl_keyframes_remove();
			withOne (obj_history, global::history_data, self->id)
			{
				history_destroy_loaded(self);
				history_restore_tl_select(self);
			}
			
			tl_update_list(self);
			withAll (obj_timeline, self->id)
				tl_update_values(ScopeAny(self));
			
			tl_update_matrix(self);
		}
		else
		{
			IntType hobj;
			hobj = null_;
			if (!(sReal(history_redo) > 0))
			{
				if (!file_exists_lib(fn))
					return IntType(0);
				hobj = history_set(self, ID_action_tl_load_loop);
				withOne (obj_history, hobj, self->id)
				{
					self->filename = fn;
					history_save_tl_select(ScopeAny(self));
				}
				
			}
			else
			{
				fn = ObjType(obj_history, global::history_data)->filename;
				if (!file_exists_lib(fn))
					return IntType(0);
			}
			
			VarType tladd = sInt(timeline_settings_import_loop_tl);
			if (idVar(tladd, part_of) != null_)
				tladd = idVar(tladd, part_of);
			VarType insertpos, goalpos;
			insertpos = idVar(idVar(tladd, keyframe_select), position);
			goalpos = ObjType(obj_keyframe, DsList(idInt(tladd, keyframe_list)).Value(ds_list_find_index(idInt(tladd, keyframe_list), idVar(tladd, keyframe_select)) + IntType(1)))->position;
			tl_deselect_all();
			for (IntType p = IntType(0); p < ds_list_size(idInt(tladd, part_list)); p++)
			{
				withOne (Object, DsList(idInt(tladd, part_list)).Value(p), self->id)
				{
					tl_keyframe_select(tl_keyframe_add(self, insertpos));
					tl_keyframe_select(tl_keyframe_add(self, goalpos));
				}
				
			}
			while (insertpos < goalpos)
				insertpos = action_tl_keyframes_load_file(self, fn, tladd, insertpos, goalpos - insertpos);
			
			withAll (obj_keyframe, self->id)
				if (self->timeline == tladd && self->selected > 0)
					instance_destroy(ScopeAny(self));
			
			withOne (Object, tladd, self->id)
			{
				tl_deselect(self);
				sBool(update_matrix) = true;
			}
			
			project_load_update(self);
			withOne (obj_history, hobj, self->id)
				history_save_loaded(self);
			
		}
		
		app_update_tl_edit(self);
		return 0.0;
	}
	
	void action_tl_lock(ScopeAny self, VarType timeline)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
				for (IntType t = IntType(0); t < self->save_var_amount; t++)
					withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
						sVar(lock) = ObjType(obj_history, self.otherId)->save_var_old_value.Value(t);
			
			
		}
		else
			if (sReal(history_redo) > 0)
			{
				withOne (obj_history, global::history_data, self->id)
					for (IntType t = IntType(0); t < self->save_var_amount; t++)
						withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
							sVar(lock) = ObjType(obj_history, self.otherId)->save_var_new_value.Value(t);
				
				
			}
			else
			{
				VarType hobj = history_save_var_start(self, ID_action_tl_lock, false);
				action_tl_lock_tree(timeline, !(idVar(timeline, lock) > 0), hobj);
			}
		
		
	}
	
	void action_tl_lock_bend(ScopeAny self, VarType enable)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
			{
				for (IntType t = IntType(0); t < self->save_var_amount; t++)
				{
					withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
					{
						sVar(lock_bend) = ObjType(obj_history, self.otherId)->save_var_old_value.Value(t);
						sBool(update_matrix) = true;
					}
					
				}
			}
			
		}
		else
			if (sReal(history_redo) > 0)
			{
				withOne (obj_history, global::history_data, self->id)
				{
					for (IntType t = IntType(0); t < self->save_var_amount; t++)
					{
						withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
						{
							sVar(lock_bend) = ObjType(obj_history, self.otherId)->save_var_new_value.Value(t);
							sBool(update_matrix) = true;
						}
						
					}
				}
				
			}
			else
			{
				VarType hobj = history_save_var_start(self, ID_action_tl_lock_bend, false);
				withAll (obj_timeline, self->id)
				{
					if (!(self->selected > 0))
						continue;
					withOne (obj_history, hobj, self->id)
						history_save_var(self, self.otherId, ObjType(obj_timeline, self.otherId)->lock_bend, enable);
					
					self->lock_bend = enable;
					self->update_matrix = true;
				}
				
			}
		
		
		tl_update_matrix(self);
	}
	
	void action_tl_lock_tree(VarType tl, BoolType nval, VarType hobj)
	{
		withOne (obj_history, hobj, noone)
			history_save_var(self, tl, idVar(tl, lock), nval);
		
		idVar(tl, lock) = nval;
		for (IntType i = IntType(0); i < ds_list_size(idInt(tl, tree_list)); i++)
			action_tl_lock_tree(DsList(idInt(tl, tree_list)).Value(i), nval, hobj);
	}
	
	void action_tl_lq_hiding(ScopeAny self, VarType enable)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
				for (IntType t = IntType(0); t < self->save_var_amount; t++)
					withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
						sVar(lq_hiding) = ObjType(obj_history, self.otherId)->save_var_old_value.Value(t);
			
			
		}
		else
			if (sReal(history_redo) > 0)
			{
				withOne (obj_history, global::history_data, self->id)
					for (IntType t = IntType(0); t < self->save_var_amount; t++)
						withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
							sVar(lq_hiding) = ObjType(obj_history, self.otherId)->save_var_new_value.Value(t);
				
				
			}
			else
			{
				VarType hobj = history_save_var_start(self, ID_action_tl_lq_hiding, false);
				withAll (obj_timeline, self->id)
					if (self->selected > 0)
						action_tl_lq_hiding_tree(self->id, enable, hobj);
				
			}
		
		
	}
	
	void action_tl_lq_hiding_tree(VarType tl, VarType nval, VarType hobj)
	{
		withOne (obj_history, hobj, noone)
			history_save_var(self, tl, ObjType(obj_timeline, tl)->lq_hiding, nval);
		
		ObjType(obj_timeline, tl)->lq_hiding = nval;
		for (IntType i = IntType(0); i < ds_list_size(ObjType(obj_timeline, tl)->tree_list); i++)
			if (!(ObjType(obj_timeline, DsList(ObjType(obj_timeline, tl)->tree_list).Value(i))->selected > 0))
				action_tl_lq_hiding_tree(DsList(ObjType(obj_timeline, tl)->tree_list).Value(i), nval, hobj);
	}
	
	void action_tl_marker_color(ScopeAny self, VarType color)
	{
		action_tl_marker_edit(self, idVar(sVar(timeline_marker_edit), name), color);
	}
	
	void action_tl_marker_delete(ScopeAny self)
	{
		VarType marker;
		if (sReal(history_undo) > 0)
		{
			marker = (new obj_marker)->id;
			idVar(marker, save_id) = ObjType(obj_history, global::history_data)->marker_save_id;
			idVar(marker, name) = ObjType(obj_history, global::history_data)->marker_name;
			idVar(marker, color) = ObjType(obj_history, global::history_data)->marker_color;
			idVar(marker, pos) = ObjType(obj_history, global::history_data)->marker_pos;
			ds_list_add({ sInt(timeline_marker_list), marker });
		}
		else
		{
			VarType hobj;
			marker = global::list_item_value;
			if (!(sReal(history_redo) > 0))
			{
				hobj = history_set(self, ID_action_tl_marker_delete);
				withOne (obj_history, hobj, self->id)
				{
					self->marker_save_id = idVar(marker, save_id);
					self->marker_name = idVar(marker, name);
					self->marker_color = idVar(marker, color);
					self->marker_pos = idVar(marker, pos);
				}
				
			}
			else
				hobj = global::history_data;
			
			instance_destroy(self, { save_id_find(ObjType(obj_history, hobj)->marker_save_id) });
		}
		
		marker_list_sort(self);
	}
	
	RealType action_tl_marker_edit(ScopeAny self, VarType name, VarType color)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (Object, save_id_find(ObjType(obj_history, global::history_data)->marker_save_id), self->id)
			{
				color = ObjType(obj_history, global::history_data)->marker_color_prev;
				name = ObjType(obj_history, global::history_data)->marker_name_prev;
			}
			
		}
		else
		{
			VarType hobj, marker;
			if (sReal(history_amount) > IntType(0) && (ObjType(obj_history, sArr(history).Value(IntType(0)))->script == ID_action_tl_marker_new) && (ObjType(obj_history, sArr(history).Value(IntType(0)))->marker_save_id == save_id_get(sVar(timeline_marker_edit))))
			{
				ObjType(obj_history, sArr(history)[IntType(0)])->marker_name = name;
				ObjType(obj_history, sArr(history)[IntType(0)])->marker_color = color;
				marker = save_id_find(ObjType(obj_history, sArr(history).Value(IntType(0)))->marker_save_id);
				idVar(marker, name) = name;
				idVar(marker, color) = color;
				return IntType(0);
			}
			if (!(sReal(history_redo) > 0))
			{
				marker = sVar(timeline_marker_edit);
				if (sReal(history_amount) > IntType(0) && (ObjType(obj_history, sArr(history).Value(IntType(0)))->script == ID_action_tl_marker_edit) && (ObjType(obj_history, sArr(history).Value(IntType(0)))->marker_save_id == save_id_get(marker)))
				{
					hobj = sArr(history).Value(IntType(0));
					ObjType(obj_history, hobj)->marker_name_new = name;
					ObjType(obj_history, hobj)->marker_color_new = color;
				}
				else
				{
					hobj = history_set(self, ID_action_tl_marker_edit);
					withOne (obj_history, hobj, self->id)
					{
						self->marker_save_id = save_id_get(marker);
						self->marker_name_prev = idVar(marker, name);
						self->marker_color_prev = idVar(marker, color);
						self->marker_name_new = name;
						self->marker_color_new = color;
					}
					
				}
				
				idVar(marker, name) = ObjType(obj_history, hobj)->marker_name_new;
				idVar(marker, color) = ObjType(obj_history, hobj)->marker_color_new;
			}
			else
			{
				withOne (Object, save_id_find(ObjType(obj_history, global::history_data)->marker_save_id), self->id)
				{
					name = ObjType(obj_history, global::history_data)->marker_name_new;
					color = ObjType(obj_history, global::history_data)->marker_color_new;
				}
				
			}
			
		}
		
		return 0.0;
	}
	
	void action_tl_marker_editor(ScopeAny self, IntType marker)
	{
		menu_settings_set(self, gmlGlobal::mouse_x, gmlGlobal::mouse_y, /*"timelinemarkernew"*/ STR(47), IntType(0));
		sInt(settings_menu_script) = ID_marker_editor_draw;
		sVar(timeline_marker_edit) = marker;
		sBool(settings_menu_above) = true;
	}
	
	void action_tl_marker_new(ScopeAny self)
	{
		if (sReal(history_undo) > 0)
		{
			instance_destroy(self, { save_id_find(ObjType(obj_history, global::history_data)->marker_save_id) });
		}
		else
		{
			IntType hobj, marker;
			hobj = null_;
			if (!(sReal(history_redo) > 0))
			{
				hobj = history_set(self, ID_action_tl_marker_new);
				marker = (new obj_marker)->id;
				withOne (obj_history, hobj, self->id)
				{
					self->marker_save_id = save_id_get(marker);
					self->marker_pos = ObjType(obj_marker, marker)->pos;
					self->marker_color = ObjType(obj_marker, marker)->color;
					self->marker_name = ObjType(obj_marker, marker)->name;
				}
				
				action_tl_marker_editor(self, marker);
			}
			else
			{
				marker = (new obj_marker)->id;
				ObjType(obj_marker, marker)->pos = ObjType(obj_history, global::history_data)->marker_pos;
				ObjType(obj_marker, marker)->color = ObjType(obj_history, global::history_data)->marker_color;
				ObjType(obj_marker, marker)->name = ObjType(obj_history, global::history_data)->marker_name;
			}
			
			ds_list_add({ sInt(timeline_marker_list), marker });
		}
		
		marker_list_sort(self);
	}
	
	void action_tl_marker_pos(ScopeAny self)
	{
		VarType marker;
		if (sReal(history_undo) > 0)
		{
			marker = save_id_find(ObjType(obj_history, global::history_data)->marker_save_id);
			idVar(marker, pos) = ObjType(obj_history, global::history_data)->marker_pos_prev;
		}
		else
		{
			IntType hobj;
			if (!(sReal(history_redo) > 0))
			{
				marker = sVar(timeline_marker_edit);
				hobj = history_set(self, ID_action_tl_marker_pos);
				withOne (obj_history, hobj, self->id)
				{
					self->marker_save_id = save_id_get(marker);
					self->marker_pos_prev = idVar(marker, edit_pos);
					self->marker_pos_new = idVar(marker, pos);
				}
				
				idVar(marker, pos) = ObjType(obj_history, hobj)->marker_pos_new;
			}
			else
			{
				withOne (Object, save_id_find(ObjType(obj_history, global::history_data)->marker_save_id), self->id)
					sVar(pos) = ObjType(obj_history, global::history_data)->marker_pos_new;
				
			}
			
		}
		
		marker_list_sort(self);
	}
	
	void action_tl_move_done(ScopeAny self, VarType par, IntType pos)
	{
		if (par == null_)
		{
			par = global::_app->id;
			pos = null_;
		}
		action_tl_parent(self, par, pos);
		withOne (obj_data, sInt(timeline_move_obj), self->id)
		{
			ds_list_destroy(self->tree_list);
			instance_destroy(ScopeAny(self));
		}
		
		sInt(timeline_move_obj) = null_;
		sVar(window_busy) = /*""*/ STR(0);
	}
	
	void action_tl_move_start(ScopeAny self)
	{
		sVar(window_busy) = /*"timelinemove"*/ STR(48);
		sInt(timeline_move_obj) = (new obj_data)->id;
		ObjType(obj_data, sInt(timeline_move_obj))->tree_list = ds_list_create();
		ObjType(obj_data, sInt(timeline_move_obj))->tree_list_filter = ds_list_create();
		action_tl_move_start_tree(self);
		tl_update_list(self);
	}
	
	void action_tl_move_start_tree(ScopeAny self)
	{
		for (IntType t = IntType(0); t < ds_list_size(sInt(tree_list)); t++)
		{
			withOne (obj_timeline, DsList(sInt(tree_list)).Value(t), self->id)
			{
				if (self->selected > 0 && self->part_of == null_)
				{
					self->move_parent = self->parent;
					self->move_parent_tree_index = ds_list_find_index(idInt(self->parent, tree_list), self->id);
					tl_set_parent(ScopeAny(self), { global::_app->timeline_move_obj });
					t--;
				}
				action_tl_move_start_tree(ScopeAny(self));
			}
			
		}
	}
	
	void action_tl_name(ScopeAny self, VarType name)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
			{
				for (IntType t = IntType(0); t < self->save_var_amount; t++)
				{
					withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
					{
						idVar(self->id, name) = ObjType(obj_history, self.otherId)->save_var_old_value.Value(t);
						tl_update_display_name(self);
					}
					
				}
			}
			
		}
		else
			if (sReal(history_redo) > 0)
			{
				withOne (obj_history, global::history_data, self->id)
				{
					for (IntType t = IntType(0); t < self->save_var_amount; t++)
					{
						withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
						{
							idVar(self->id, name) = ObjType(obj_history, self.otherId)->save_var_new_value.Value(t);
							tl_update_display_name(self);
						}
						
					}
				}
				
			}
			else
			{
				VarType hobj = history_save_var_start(self, ID_action_tl_name, true);
				withAll (obj_timeline, self->id)
				{
					if (!(self->selected > 0))
						continue;
					withOne (obj_history, hobj, self->id)
						history_save_var(self, self.otherId, ObjType(obj_timeline, self.otherId)->name, name);
					
					ObjType(obj_timeline, self->id)->name = name;
					tl_update_display_name(ScopeAny(self));
				}
				
			}
		
		
	}
	
	void action_tl_name_single(ScopeAny self, VarType name)
	{
		if (sReal(history_undo) > 0)
		{
			withOne (obj_history, global::history_data, self->id)
			{
				for (IntType t = IntType(0); t < self->save_var_amount; t++)
				{
					withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
					{
						idVar(self->id, name) = ObjType(obj_history, self.otherId)->save_var_old_value.Value(t);
						tl_update_display_name(self);
					}
					
				}
			}
			
		}
		else
			if (sReal(history_redo) > 0)
			{
				withOne (obj_history, global::history_data, self->id)
				{
					for (IntType t = IntType(0); t < self->save_var_amount; t++)
					{
						withOne (Object, save_id_find(self->save_var_save_id.Value(t)), self->id)
						{
							idVar(self->id, name) = ObjType(obj_history, self.otherId)->save_var_new_value.Value(t);
							tl_update_display_name(self);
						}
						
					}
				}
				
			}
			else
			{
				VarType hobj = history_save_var_start(self, ID_action_tl_name_single, true);
				withOne (Object, sVar(timeline_rename), self->id)
				{
					withOne (obj_history, hobj, self->id)
						history_save_var(self, self.otherId, idVar(self.otherId, name), name);
					
					idVar(self->id, name) = name;
					tl_update_display_name(self);
				}
				
			}
		
		
	}
	
}
