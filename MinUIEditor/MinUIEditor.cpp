// MinUIEditor.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "MinUIEditor.h"
#include "../common_files/minui_interfaces.hpp"
#include "../common_files/stools.hpp"

void ui_definitions::save_to_file(std::wstring_view file_name) {
	serialization::out_buffer buf;

	buf.write(uint16_t(sounds.size()));
	for(auto& s : sounds) {
		buf.write(s.file_name);
	}

	buf.write(uint16_t(brushes.size()));
	for(auto& b : brushes) {
		buf.write(!b.main_is_image);
		buf.write(b.m_color);
		if(b.main_is_image) {
			buf.write(b.m_file);
		}

		buf.write(!b.disabled_is_image);
		buf.write(b.d_color);
		if(b.disabled_is_image) {
			buf.write(b.d_file);
		}

		buf.write(b.line_shading);
		buf.write(b.highlight_shading);
		buf.write(b.line_highlight_shading);
	}

	for(size_t i = 0; i < icons.size(); ++i) {
		for(size_t j = 0; j < icons[i].sub_slots.size(); ++j) {
			buf.write(int16_t(i));
			buf.write(int16_t(j));
			buf.write(icons[i].xsize);
			buf.write(icons[i].ysize);
			buf.write(icons[i].sub_slots[j].file);
			buf.write(icons[i].sub_slots[j].is_svg);
		}
	}

	for(size_t i = 0; i < images.size(); ++i) {
		for(size_t j = 0; j < images[i].sub_slots.size(); ++j) {
			buf.write(int16_t(i));
			buf.write(int16_t(j));
			buf.write(images[i].xsize);
			buf.write(images[i].ysize);
			buf.write(images[i].sub_slots[j].file);
		}
	}

	auto elem_count = d_icon_position.size(); // stands for the number of defined elements in general
	buf.write(uint32_t(elem_count));
	buf.write(uint32_t(d_fixed_children.bucket_count()));
	buf.write(uint32_t(d_fixed_children.size()));
	buf.write(uint32_t(d_window_children.bucket_count()));
	buf.write(uint32_t(d_window_children.size()));
	buf.write(uint32_t(d_divider_index.bucket_count()));
	buf.write(uint32_t(d_divider_index.size()));
	buf.write(uint32_t(d_horizontal_orientation.bucket_count()));
	buf.write(uint32_t(d_horizontal_orientation.size()));
	buf.write(uint32_t(d_column_properties.bucket_count()));
	buf.write(uint32_t(d_column_properties.size()));
	buf.write(uint32_t(d_page_ui_definitions.bucket_count()));
	buf.write(uint32_t(d_page_ui_definitions.size()));
	buf.write(uint32_t(d_text_information.bucket_count()));
	buf.write(uint32_t(d_text_information.size()));
	buf.write(uint32_t(d_interaction_sound.bucket_count()));
	buf.write(uint32_t(d_interaction_sound.size()));
	buf.write(uint32_t(d_image_information.bucket_count()));
	buf.write(uint32_t(d_image_information.size()));
	buf.write(uint32_t(d_on_update_raw.bucket_count()));
	buf.write(uint32_t(d_on_update_raw.size()));
	buf.write(uint32_t(d_on_gain_focus_raw.bucket_count()));
	buf.write(uint32_t(d_on_gain_focus_raw.size()));
	buf.write(uint32_t(d_on_lose_focus_raw.bucket_count()));
	buf.write(uint32_t(d_on_lose_focus_raw.size()));
	buf.write(uint32_t(d_on_visible_raw.bucket_count()));
	buf.write(uint32_t(d_on_visible_raw.size()));
	buf.write(uint32_t(d_on_hide_raw.bucket_count()));
	buf.write(uint32_t(d_on_hide_raw.size()));
	buf.write(uint32_t(d_on_create_raw.bucket_count()));
	buf.write(uint32_t(d_on_create_raw.size()));
	buf.write(uint32_t(d_user_fn_a_raw.bucket_count()));
	buf.write(uint32_t(d_user_fn_a_raw.size()));
	buf.write(uint32_t(d_user_fn_b_raw.bucket_count()));
	buf.write(uint32_t(d_user_fn_b_raw.size()));
	buf.write(uint32_t(d_user_mouse_fn_a_raw.bucket_count()));
	buf.write(uint32_t(d_user_mouse_fn_a_raw.size()));

	buf.write_fixed(d_icon_position.data(), elem_count);
	buf.write_fixed(d_default_position.data(), elem_count);
	buf.write_fixed(d_interactable_definition.data(), elem_count);
	buf.write_fixed(d_icon.data(), elem_count);
	buf.write_fixed(d_class.data(), elem_count);
	buf.write_fixed(d_standard_flags.data(), elem_count);
	buf.write_fixed(d_foreground_brush.data(), elem_count);
	buf.write_fixed(d_background_brush.data(), elem_count);
	buf.write_fixed(d_highlight_brush.data(), elem_count);
	buf.write_fixed(d_info_brush.data(), elem_count);

	// NEED FIXING
	buf.write_fixed(d_fixed_children.m_values.data(), d_fixed_children.size());
	buf.write_fixed(d_fixed_children.m_buckets, d_fixed_children.bucket_count());
	buf.write_fixed(d_window_children.m_values.data(), d_window_children.size());
	buf.write_fixed(d_window_children.m_buckets, d_window_children.bucket_count());
	//

	{
		std::vector<minui::array_reference> temp;
		temp.resize(elem_count);
		for(size_t i = 0; i < elem_count; ++i) {
			temp[i].count = uint32_t(d_variable_definition[i].size());
		}
		auto base_addr = buf.get_data_position() + offsetof(minui::array_reference, file_offset);
		buf.write_fixed(temp.data(), elem_count);

		for(size_t i = 0; i < elem_count; ++i) {
			buf.write_relocation(base_addr, [i, this](serialization::out_buffer& b) {
				b.write_fixed(d_variable_definition[i].data(), uint32_t(d_variable_definition[i].size()));
			});
			base_addr += sizeof(minui::array_reference);
		}
	}

	buf.write_fixed(d_total_variable_size.data(), elem_count);
	buf.write_fixed(d_background_definition.data(), elem_count);

	buf.write_fixed(d_divider_index.m_values.data(), d_divider_index.size());
	buf.write_fixed(d_divider_index.m_buckets, d_divider_index.bucket_count());
	buf.write_fixed(d_horizontal_orientation.m_values.data(), d_horizontal_orientation.size());
	buf.write_fixed(d_horizontal_orientation.m_buckets, d_horizontal_orientation.bucket_count());
	buf.write_fixed(d_column_properties.m_values.data(), d_column_properties.size());
	buf.write_fixed(d_column_properties.m_buckets, d_column_properties.bucket_count());
	buf.write_fixed(d_page_ui_definitions.m_values.data(), d_page_ui_definitions.size());
	buf.write_fixed(d_page_ui_definitions.m_buckets, d_page_ui_definitions.bucket_count());
	buf.write_fixed(d_text_information.m_values.data(), d_text_information.size());
	buf.write_fixed(d_text_information.m_buckets, d_text_information.bucket_count());
	buf.write_fixed(d_interaction_sound.m_values.data(), d_interaction_sound.size());
	buf.write_fixed(d_interaction_sound.m_buckets, d_interaction_sound.bucket_count());
	buf.write_fixed(d_image_information.m_values.data(), d_image_information.size());
	buf.write_fixed(d_image_information.m_buckets, d_image_information.bucket_count());
	/*
	{
		auto cspan = buf.read_fixed<decltype(d_on_update_raw)::value_container_type>(d_on_update_raw_content);
		auto bspan = buf.read_fixed<decltype(d_on_update_raw)::bucket_type>(d_on_update_raw_buckets);
		d_on_update_raw = decltype(d_on_update_raw)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_on_gain_focus_raw)::value_container_type>(d_on_gain_focus_raw_content);
		auto bspan = buf.read_fixed<decltype(d_on_gain_focus_raw)::bucket_type>(d_on_gain_focus_raw_buckets);
		d_on_gain_focus_raw = decltype(d_on_gain_focus_raw)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_on_lose_focus_raw)::value_container_type>(d_on_lose_focus_raw_content);
		auto bspan = buf.read_fixed<decltype(d_on_lose_focus_raw)::bucket_type>(d_on_lose_focus_raw_buckets);
		d_on_lose_focus_raw = decltype(d_on_lose_focus_raw)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_on_visible_raw)::value_container_type>(d_on_visible_raw_content);
		auto bspan = buf.read_fixed<decltype(d_on_visible_raw)::bucket_type>(d_on_visible_raw_buckets);
		d_on_visible_raw = decltype(d_on_visible_raw)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_on_hide_raw)::value_container_type>(d_on_hide_raw_content);
		auto bspan = buf.read_fixed<decltype(d_on_hide_raw)::bucket_type>(d_on_hide_raw_buckets);
		d_on_hide_raw = decltype(d_on_hide_raw)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_on_create_raw)::value_container_type>(d_on_create_raw_content);
		auto bspan = buf.read_fixed<decltype(d_on_create_raw)::bucket_type>(d_on_create_raw_buckets);
		d_on_create_raw = decltype(d_on_create_raw)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_user_fn_a_raw)::value_container_type>(d_user_fn_a_raw_content);
		auto bspan = buf.read_fixed<decltype(d_user_fn_a_raw)::bucket_type>(d_user_fn_a_raw_buckets);
		d_user_fn_a_raw = decltype(d_user_fn_a_raw)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_user_fn_b_raw)::value_container_type>(d_user_fn_b_raw_content);
		auto bspan = buf.read_fixed<decltype(d_user_fn_b_raw)::bucket_type>(d_user_fn_b_raw_buckets);
		d_user_fn_b_raw = decltype(d_user_fn_b_raw)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_user_mouse_fn_a_raw)::value_container_type>(d_user_mouse_fn_a_raw_content);
		auto bspan = buf.read_fixed<decltype(d_user_mouse_fn_a_raw)::bucket_type>(d_user_mouse_fn_a_raw_buckets);
		d_user_mouse_fn_a_raw = decltype(d_user_mouse_fn_a_raw)(cspan, bspan);
	}
	*/

	buf.finalize();
}
