// MinUIEditor.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "MinUIEditor.h"
#include "../common_files/minui_interfaces.hpp"
#include "../common_files/stools.hpp"
#include "simple_fs.hpp"

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

	using pair_t = std::pair<uint32_t, minui::array_reference>;
	auto write_umap_of_vector = [&](auto& umap) {
		std::vector<pair_t> temp;
		temp.resize(umap.m_values.size());
		for(size_t i = 0; i < umap.m_values.size(); ++i) {
			temp[i].first = umap.m_values[i].first;
			temp[i].second.count = uint32_t(umap.m_values[i].second.size());
		}
		auto base_addr = buf.get_data_position() + offsetof(pair_t, second) + offsetof(minui::array_reference, file_offset);
		buf.write_fixed(temp.data(), umap.m_values.size());

		for(size_t i = 0; i < umap.m_values.size(); ++i) {
			buf.write_relocation(base_addr, [i, &umap](serialization::out_buffer& b) {
				b.write_fixed(umap.m_values[i].second.data(), uint32_t(umap.m_values[i].second.size()));
			});
			base_addr += sizeof(pair_t);
		}
		buf.write_fixed(umap.m_buckets, umap.bucket_count());
	};

	write_umap_of_vector(d_fixed_children);
	write_umap_of_vector(d_window_children);

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

	write_umap_of_vector(d_on_update_raw);
	write_umap_of_vector(d_on_gain_focus_raw);
	write_umap_of_vector(d_on_lose_focus_raw);
	write_umap_of_vector(d_on_visible_raw);
	write_umap_of_vector(d_on_hide_raw);
	write_umap_of_vector(d_on_create_raw);
	write_umap_of_vector(d_user_fn_a_raw);
	write_umap_of_vector(d_user_fn_b_raw);
	write_umap_of_vector(d_user_mouse_fn_a_raw);

	buf.finalize();

	std::wstring fname{ file_name };
	HANDLE file_handle = CreateFileW(fname.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
	if(file_handle != INVALID_HANDLE_VALUE) {
		WriteFile(file_handle, buf.data(), DWORD(buf.size()), nullptr, nullptr);
		SetEndOfFile(file_handle);
		CloseHandle(file_handle);
	}
}

void ui_definitions::save_to_project_file(std::wstring_view file_name) {
	serialization::out_buffer buf;

	buf.write(uint32_t(sounds.size()));
	for(auto& s : sounds) {
		buf.write(s.file_name);
	}

	buf.write(uint32_t(brushes.size()));
	for(auto& b : brushes) {
		buf.write(b.main_is_image);
		buf.write(b.m_color);
		buf.write(b.m_file);
		buf.write(b.disabled_is_image);
		buf.write(b.d_color);
		buf.write(b.d_file);
		buf.write(b.line_shading);
		buf.write(b.highlight_shading);
		buf.write(b.line_highlight_shading);
	}

	buf.write(uint32_t(icons.size()));
	for(auto& i : icons) {
		buf.write(i.xsize);
		buf.write(i.ysize);
		buf.write(uint32_t(i.sub_slots.size()));
		for(auto& s : i.sub_slots) {
			buf.write(s.is_svg);
			buf.write(s.file);
		}
	}

	buf.write(uint32_t(images.size()));
	for(auto& i : images) {
		buf.write(i.xsize);
		buf.write(i.ysize);
		buf.write(uint32_t(i.sub_slots.size()));
		for(auto& s : i.sub_slots) {
			buf.write(s.file);
		}
	}

	uint32_t num_elements = uint32_t(d_icon_position.size());
	buf.write(num_elements);
	buf.write_fixed(d_icon_position.data(), num_elements);
	buf.write_fixed(d_default_position.data(), num_elements);
	buf.write_fixed(d_interactable_definition.data(), num_elements);
	buf.write_fixed(d_icon.data(), num_elements);
	buf.write_fixed(d_class.data(), num_elements);
	buf.write_fixed(d_standard_flags.data(), num_elements);
	buf.write_fixed(d_foreground_brush.data(), num_elements);
	buf.write_fixed(d_background_brush.data(), num_elements);
	buf.write_fixed(d_highlight_brush.data(), num_elements);
	buf.write_fixed(d_info_brush.data(), num_elements);

	auto write_v_umap = [&buf](auto& u) { 
		buf.write(uint32_t(u.size()));
		for(auto& p : u) {
			buf.write(uint32_t(p.first));
			buf.write_variable(p.second.data(), p.second.size());
		}
	};

	write_v_umap(d_fixed_children);
	write_v_umap(d_window_children);

	for(uint32_t i = 0; i < num_elements; ++i) {
		buf.write_variable(d_variable_definition[i].data(), d_variable_definition[i].size());
	}

	buf.write_fixed(d_total_variable_size.data(), num_elements);
	buf.write_fixed(d_background_definition.data(), num_elements);

	auto write_umap = [&buf](auto& u) {
		buf.write(uint32_t(u.size()));
		for(auto& p : u) {
			buf.write(uint32_t(p.first));
			buf.write(p.second);
		}
	};

	write_umap(d_divider_index);
	write_umap(d_horizontal_orientation);
	write_umap(d_column_properties);
	write_umap(d_page_ui_definitions);
	write_umap(d_text_information);
	write_umap(d_interaction_sound);
	write_umap(d_image_information);

	write_umap(d_on_update_raw);
	write_umap(d_on_gain_focus_raw);
	write_umap(d_on_lose_focus_raw);
	write_umap(d_on_visible_raw);
	write_umap(d_on_hide_raw);
	write_umap(d_on_create_raw);
	write_umap(d_user_fn_a_raw);
	write_umap(d_user_fn_b_raw);
	write_umap(d_user_mouse_fn_a_raw);
	
	buf.finalize();

	std::wstring final_file_name = std::wstring{ file_name } + L".mui";
	DWORD dwAttrib = GetFileAttributesW(final_file_name.c_str());
	if(dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
		// file exists
		std::wstring temp_file_name = std::wstring{ file_name } + L".mui.tmp";

		HANDLE file_handle = CreateFileW(temp_file_name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
		if(file_handle != INVALID_HANDLE_VALUE) {
			WriteFile(file_handle, buf.data(), DWORD(buf.size()), nullptr, nullptr);
			SetEndOfFile(file_handle);
			CloseHandle(file_handle);
		}

		ReplaceFileW(final_file_name.c_str(), temp_file_name.c_str(), nullptr, REPLACEFILE_IGNORE_MERGE_ERRORS | REPLACEFILE_IGNORE_ACL_ERRORS, nullptr, nullptr);
	} else {
		// file does not exist
		HANDLE file_handle = CreateFileW(final_file_name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
		if(file_handle != INVALID_HANDLE_VALUE) {
			WriteFile(file_handle, buf.data(), DWORD(buf.size()), nullptr, nullptr);
			SetEndOfFile(file_handle);
			CloseHandle(file_handle);
		}
	}
}
void ui_definitions::load_from_project_file(std::wstring_view file_name) {
	simple_fs::file f{ std::wstring{ file_name } };
	auto fcontent = simple_fs::view_contents(f);
	serialization::in_buffer buf{ fcontent.data, fcontent.file_size };

	auto num_sounds = buf.read<uint32_t>();
	sounds.resize(num_sounds);
	for(uint32_t i = 0; i < num_sounds; ++i) {
		sounds[i].file_name = std::wstring{ buf.read<std::wstring_view>() };
	}

	auto num_brush = buf.read<uint32_t>();
	brushes.resize(num_brush);
	for(uint32_t i = 0; i < num_brush; ++i) {
		brushes[i].main_is_image = buf.read<bool>();
		brushes[i].m_color = buf.read<minui::brush_color>();
		brushes[i].m_file = std::wstring{ buf.read<std::wstring_view>() };
		brushes[i].disabled_is_image = buf.read<bool>();
		brushes[i].d_color = buf.read<minui::brush_color>();
		brushes[i].d_file = std::wstring{ buf.read<std::wstring_view>() };
		brushes[i].line_shading = buf.read<float>();
		brushes[i].highlight_shading = buf.read<float>();
		brushes[i].line_highlight_shading = buf.read<float>();
	}

	auto num_icons = buf.read<uint32_t>();
	icons.resize(num_icons);
	for(uint32_t i = 0; i < num_icons; ++i) {
		icons[i].xsize = buf.read<minui::em>();
		icons[i].ysize = buf.read<minui::em>();
		uint32_t sslots = buf.read<uint32_t>();
		icons[i].sub_slots.resize(sslots);

		for(uint32_t j = 0; j < sslots; ++j) {
			icons[i].sub_slots[j].is_svg = buf.read<bool>();
			icons[i].sub_slots[j].file = std::wstring{ buf.read<std::wstring_view>() };
		}
	}

	auto num_image = buf.read<uint32_t>();
	images.resize(num_image);
	for(uint32_t i = 0; i < num_image; ++i) {
		images[i].xsize = buf.read<minui::em>();
		images[i].ysize = buf.read<minui::em>();
		uint32_t sslots = buf.read<uint32_t>();
		images[i].sub_slots.resize(sslots);

		for(uint32_t j = 0; j < sslots; ++j) {
			images[i].sub_slots[j].file = std::wstring{ buf.read<std::wstring_view>() };
		}
	}

	auto span_to_vector = [](auto const& span) { return std::vector<std::remove_cvref_t<decltype(*(span.begin()))>>{ span.begin(), span.end() }; };
	auto num_elements = buf.read<uint32_t>();
	d_icon_position = span_to_vector(buf.read_fixed<minui::layout_position>(num_elements));
	d_default_position = span_to_vector(buf.read_fixed<minui::layout_rect>(num_elements));
	d_interactable_definition = span_to_vector(buf.read_fixed<minui::interactable_definition>(num_elements));
	d_icon = span_to_vector(buf.read_fixed<minui::icon_handle>(num_elements));
	d_class = span_to_vector(buf.read_fixed<uint16_t>(num_elements));
	d_standard_flags = span_to_vector(buf.read_fixed<uint32_t>(num_elements));
	d_foreground_brush = span_to_vector(buf.read_fixed<uint16_t>(num_elements));
	d_background_brush = span_to_vector(buf.read_fixed<uint16_t>(num_elements));
	d_highlight_brush = span_to_vector(buf.read_fixed<uint16_t>(num_elements));
	d_info_brush = span_to_vector(buf.read_fixed<uint16_t>(num_elements));

	auto numfc = buf.read<uint32_t>();
	d_fixed_children.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_fixed_children.insert_or_assign(key, span_to_vector(buf.read_variable<uint16_t>()));
	}

	numfc = buf.read<uint32_t>();
	d_window_children.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_window_children.insert_or_assign(key, span_to_vector(buf.read_variable<minui::relative_child_def>()));
	}

	d_variable_definition.resize(num_elements);
	for(uint32_t i = 0; i < num_elements; ++i) {
		d_variable_definition[i] = span_to_vector(buf.read_variable<minui::variable_definition>());
	}
	d_total_variable_size = span_to_vector(buf.read_fixed<uint16_t>(num_elements));
	d_background_definition = span_to_vector(buf.read_fixed<minui::background_definition>(num_elements));

	numfc = buf.read<uint32_t>();
	d_divider_index.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_divider_index.insert_or_assign(key, buf.read<int32_t>());
	}
	numfc = buf.read<uint32_t>();
	d_horizontal_orientation.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_horizontal_orientation.insert_or_assign(key, buf.read<bool>());
	}
	numfc = buf.read<uint32_t>();
	d_column_properties.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_column_properties.insert_or_assign(key, buf.read<minui::column_properties>());
	}
	numfc = buf.read<uint32_t>();
	d_page_ui_definitions.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_page_ui_definitions.insert_or_assign(key, buf.read<minui::page_ui_definitions>());
	}
	numfc = buf.read<uint32_t>();
	d_text_information.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_text_information.insert_or_assign(key, buf.read<minui::text_information>());
	}
	numfc = buf.read<uint32_t>();
	d_interaction_sound.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_interaction_sound.insert_or_assign(key, buf.read<minui::sound_handle>());
	}
	numfc = buf.read<uint32_t>();
	d_image_information.clear();
	for(uint32_t i = 0; i < numfc; ++i) {
		auto key = buf.read<uint32_t>();
		d_image_information.insert_or_assign(key, buf.read<minui::image_information>());
	}

	auto read_smap = [&buf](auto& map) { 
		map.clear();
		auto count = buf.read<uint32_t>();
		for(uint32_t i = 0; i < count; ++i) {
			auto key = buf.read<uint32_t>();
			map.insert_or_assign(key, std::wstring{ buf.read<std::wstring_view>() });
		}
	};
	read_smap(d_on_update_raw);
	read_smap(d_on_gain_focus_raw);
	read_smap(d_on_lose_focus_raw);
	read_smap(d_on_visible_raw);
	read_smap(d_on_hide_raw);
	read_smap(d_on_create_raw);
	read_smap(d_user_fn_a_raw);
	read_smap(d_user_fn_b_raw);
	read_smap(d_user_mouse_fn_a_raw);

}
