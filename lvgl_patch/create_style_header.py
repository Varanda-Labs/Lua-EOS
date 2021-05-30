#
#
# recreate prototypes for style get/set
#
# Part of Lua EOS
#
# Copyrights Varanda Labs Inc.
#


"""
#define _OBJ_GET_STYLE(prop_name, func_name, value_type, style_type)                                \
    static inline value_type lv_obj_get_style_##func_name(const lv_obj_t * obj, uint8_t part)       \
    {                                                                                               \
        return (value_type)_lv_obj_get_style##style_type(obj, part, LV_STYLE_##prop_name);          \
    }
#else
#define _OBJ_GET_STYLE(prop_name, func_name, value_type, style_type)                                \
    static inline value_type lv_obj_get_style_##func_name(const lv_obj_t * obj, uint8_t part)       \
    {                                                                                               \
        return _lv_obj_get_style##style_type(obj, part, LV_STYLE_##prop_name);                      \
    }
#endif

#define _OBJ_SET_STYLE_LOCAL(prop_name, func_name, value_type, style_type)                                                      \
    static inline void lv_obj_set_style_local_##func_name(lv_obj_t * obj, uint8_t part, lv_state_t state, value_type value)     \
    {                                                                                                                           \
        _lv_obj_set_style_local##style_type(obj, part, LV_STYLE_##prop_name | (state << LV_STYLE_STATE_POS), value);            \
    }

#define _OBJ_SET_STYLE(prop_name, func_name, value_type, style_type)                                                     \
    static inline void lv_style_set_##func_name(lv_style_t * style, lv_state_t state, value_type value)                  \
    {                                                                                                                    \
        _lv_style_set##style_type(style, LV_STYLE_##prop_name | (state << LV_STYLE_STATE_POS), value);                   \
    }

#define _LV_OBJ_STYLE_SET_GET_DECLARE(prop_name, func_name, value_type, style_type)                                      \
    _OBJ_GET_STYLE(prop_name, func_name, value_type, style_type)                                                         \
    _OBJ_SET_STYLE_LOCAL(prop_name, func_name, value_type, style_type)                                                   \
    _OBJ_SET_STYLE(prop_name, func_name, value_type, style_type)
"""

body = ""

def  _OBJ_GET_STYLE(prop_name, func_name, value_type, style_type):
  global body
  # value_type lv_obj_get_style_##func_name(const lv_obj_t * obj, uint8_t part)
  body = body + value_type + " lv_obj_get_style_" + func_name + "(const lv_obj_t * obj, uint8_t part);\n"
  
  
def  _OBJ_SET_STYLE(prop_name, func_name, value_type, style_type):
  global body
  # void lv_style_set_##func_name(lv_style_t * style, lv_state_t state, value_type value)
  body = body + "void lv_style_set_" + func_name + "(lv_style_t * style, lv_state_t state, " + value_type + " value);\n"

def _LV_OBJ_STYLE_SET_GET_DECLARE(prop_name, func_name, value_type, style_type):
  _OBJ_GET_STYLE(prop_name, func_name, value_type, style_type)
  _OBJ_SET_STYLE(prop_name, func_name, value_type, style_type)


def print_prototypes():
  _LV_OBJ_STYLE_SET_GET_DECLARE("RADIUS", "radius", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("CLIP_CORNER", "clip_corner", "bool", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SIZE", "size", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSFORM_WIDTH", "transform_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSFORM_HEIGHT", "transform_height", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSFORM_ANGLE", "transform_angle", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSFORM_ZOOM", "transform_zoom", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("OPA_SCALE", "opa_scale", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PAD_TOP", "pad_top", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PAD_BOTTOM", "pad_bottom", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PAD_LEFT", "pad_left", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PAD_RIGHT", "pad_right", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PAD_INNER", "pad_inner", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("MARGIN_TOP", "margin_top", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("MARGIN_BOTTOM", "margin_bottom", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("MARGIN_LEFT", "margin_left", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("MARGIN_RIGHT", "margin_right", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BG_BLEND_MODE", "bg_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BG_MAIN_STOP", "bg_main_stop", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BG_GRAD_STOP", "bg_grad_stop", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BG_GRAD_DIR", "bg_grad_dir", "lv_grad_dir_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BG_COLOR", "bg_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BG_GRAD_COLOR", "bg_grad_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BG_OPA", "bg_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BORDER_WIDTH", "border_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BORDER_SIDE", "border_side", "lv_border_side_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BORDER_BLEND_MODE", "border_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BORDER_POST", "border_post", "bool", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BORDER_COLOR", "border_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("BORDER_OPA", "border_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("OUTLINE_WIDTH", "outline_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("OUTLINE_PAD", "outline_pad", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("OUTLINE_BLEND_MODE", "outline_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("OUTLINE_COLOR", "outline_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("OUTLINE_OPA", "outline_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SHADOW_WIDTH", "shadow_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SHADOW_OFS_X", "shadow_ofs_x", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SHADOW_OFS_Y", "shadow_ofs_y", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SHADOW_SPREAD", "shadow_spread", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SHADOW_BLEND_MODE", "shadow_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SHADOW_COLOR", "shadow_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SHADOW_OPA", "shadow_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PATTERN_REPEAT", "pattern_repeat", "bool", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PATTERN_BLEND_MODE", "pattern_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PATTERN_RECOLOR", "pattern_recolor", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PATTERN_OPA", "pattern_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PATTERN_RECOLOR_OPA", "pattern_recolor_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("PATTERN_IMAGE", "pattern_image", "const void *", "_ptr")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_LETTER_SPACE", "value_letter_space", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_LINE_SPACE", "value_line_space", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_BLEND_MODE", "value_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_OFS_X", "value_ofs_x", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_OFS_Y", "value_ofs_y", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_ALIGN", "value_align", "lv_align_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_COLOR", "value_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_OPA", "value_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_FONT", "value_font", "const lv_font_t *", "_ptr")
  _LV_OBJ_STYLE_SET_GET_DECLARE("VALUE_STR", "value_str", "const char *", "_ptr")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_LETTER_SPACE", "text_letter_space", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_LINE_SPACE", "text_line_space", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_DECOR", "text_decor", "lv_text_decor_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_BLEND_MODE", "text_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_COLOR", "text_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_SEL_COLOR", "text_sel_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_SEL_BG_COLOR", "text_sel_bg_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_OPA", "text_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TEXT_FONT", "text_font", "const lv_font_t *", "_ptr")
  _LV_OBJ_STYLE_SET_GET_DECLARE("LINE_WIDTH", "line_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("LINE_BLEND_MODE", "line_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("LINE_DASH_WIDTH", "line_dash_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("LINE_DASH_GAP", "line_dash_gap", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("LINE_ROUNDED", "line_rounded", "bool", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("LINE_COLOR", "line_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("LINE_OPA", "line_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("IMAGE_BLEND_MODE", "image_blend_mode", "lv_blend_mode_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("IMAGE_RECOLOR", "image_recolor", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("IMAGE_OPA", "image_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("IMAGE_RECOLOR_OPA", "image_recolor_opa", "lv_opa_t", "_opa")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_TIME", "transition_time", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_DELAY", "transition_delay", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_PROP_", "transition_prop_1", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_PROP_2", "transition_prop_2", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_PROP_3", "transition_prop_3", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_PROP_4", "transition_prop_4", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_PROP_5", "transition_prop_5", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_PROP_6", "transition_prop_6", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("TRANSITION_PATH", "transition_path", "const lv_anim_path_t *", "_ptr")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SCALE_WIDTH", "scale_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SCALE_BORDER_WIDTH", "scale_border_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SCALE_END_BORDER_WIDTH", "scale_end_border_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SCALE_END_LINE_WIDTH", "scale_end_line_width", "lv_style_int_t", "_int")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SCALE_GRAD_COLOR", "scale_grad_color", "lv_color_t", "_color")
  _LV_OBJ_STYLE_SET_GET_DECLARE("SCALE_END_COLOR", "scale_end_color", "lv_color_t", "_color")
  
  print(body)

if __name__ == "__main__":
  print_prototypes()


