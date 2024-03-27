#pragma once

#include "resource.h"
#include "../common_files/unordered_dense.h"
#include "../common_files/minui_interfaces.hpp"

struct sound_def {
	std::wstring file_name;
};

struct brush_def {
	bool main_is_image = false;
	minui::brush_color m_color;
	std::wstring m_file;
	bool disabled_is_image = false;
	minui::brush_color d_color;
	std::wstring d_file;
	float line_shading = 0.0f;
	float highlight_shading = 0.0f;
	float line_highlight_shading = 0.0f;
};

struct sub_icon_def {
	std::wstring file;
	bool is_svg = false;
};

struct icon_def {
	minui::em xsize;
	minui::em ysize;
	std::vector< sub_icon_def> sub_slots;
};

struct sub_image_def {
	std::wstring file;
};

struct image_def {
	minui::em xsize;
	minui::em ysize;
	std::vector< sub_image_def> sub_slots;
};

struct ui_definitions {
	std::vector<sound_def> sounds;
	std::vector<brush_def> brushes;
	std::vector<icon_def> icons;
	std::vector<image_def> images;

	std::vector<minui::layout_position> d_icon_position;
	std::vector<minui::layout_rect> d_default_position;
	std::vector<minui::interactable_definition> d_interactable_definition;
	std::vector<minui::icon_handle> d_icon;
	std::vector<uint16_t> d_class;
	std::vector<uint32_t> d_standard_flags;
	std::vector<uint16_t> d_foreground_brush;
	std::vector<uint16_t> d_background_brush;
	std::vector<uint16_t> d_highlight_brush;
	std::vector<uint16_t> d_info_brush;
	ankerl::unordered_dense::map<uint32_t, std::vector<uint16_t>> d_fixed_children;
	ankerl::unordered_dense::map<uint32_t, std::vector<minui::relative_child_def>> d_window_children;
	std::vector<std::vector<minui::variable_definition>> d_variable_definition;
	std::vector<uint16_t> d_total_variable_size;
	std::vector<minui::background_definition> d_background_definition;
	ankerl::unordered_dense::map<uint32_t, int32_t> d_divider_index;
	ankerl::unordered_dense::map<uint32_t, bool> d_horizontal_orientation;
	ankerl::unordered_dense::map<uint32_t, minui::column_properties> d_column_properties;
	ankerl::unordered_dense::map<uint32_t, minui::page_ui_definitions> d_page_ui_definitions;
	ankerl::unordered_dense::map<uint32_t, minui::text_information> d_text_information;
	ankerl::unordered_dense::map<uint32_t, minui::sound_handle> d_interaction_sound;
	ankerl::unordered_dense::map<uint32_t, minui::image_information> d_image_information;

	ankerl::unordered_dense::map<uint32_t, std::string> d_on_update_raw;
	ankerl::unordered_dense::map<uint32_t, std::string> d_on_gain_focus_raw;
	ankerl::unordered_dense::map<uint32_t, std::string> d_on_lose_focus_raw;
	ankerl::unordered_dense::map<uint32_t, std::string> d_on_visible_raw;
	ankerl::unordered_dense::map<uint32_t, std::string> d_on_hide_raw;
	ankerl::unordered_dense::map<uint32_t, std::string> d_on_create_raw;
	ankerl::unordered_dense::map<uint32_t, std::string> d_user_fn_a_raw;
	ankerl::unordered_dense::map<uint32_t, std::string> d_user_fn_b_raw;
	ankerl::unordered_dense::map<uint32_t, std::string> d_user_mouse_fn_a_raw;

	void save_to_file(std::wstring_view file_name);

	void save_to_project_file(std::wstring_view file_name);
	void load_from_project_file(std::wstring_view file_name);
};