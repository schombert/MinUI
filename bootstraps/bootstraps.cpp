#include "Windows.h"
#include "../common_files/stools.hpp"
#include "../common_files/minui_interfaces.hpp"
#include "../common_files/minui_text_impl.hpp"
#include "../MinUIEditor/MinUIEditor.h"
#include "../MinUIEditor/MinUIEditor.cpp"

int main() {
	{
		// locale file

		serialization::out_buffer b;
		b.write(true);
		b.write(std::wstring(L"English (US)"));

		b.finalize();

		std::wstring fname{ L"locale.dat" };
		HANDLE file_handle = CreateFileW(fname.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
		if(file_handle != INVALID_HANDLE_VALUE) {
			WriteFile(file_handle, b.data(), DWORD(b.size()), nullptr, nullptr);
			SetEndOfFile(file_handle);
			CloseHandle(file_handle);
		}
	}

	{
		// fonts file


		serialization::out_buffer b;

		b.write(uint32_t(1)); // number of fonts
		{ // per font
			b.write(std::wstring(L"Source Serif 4"));
			b.write(100.0f); // span
			b.write(int32_t(400)); // weight
			b.write(int32_t(0)); // top lead
			b.write(int32_t(0)); // bottom lead
			b.write(1.0f); // scale
			b.write(minui::text::font_type::roman);
			b.write(false); // oblique
			b.write(uint32_t(0)); // number of fallbacks
		}
		b.finalize();

		std::wstring fname{ L"fonts.dat" };
		HANDLE file_handle = CreateFileW(fname.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
		if(file_handle != INVALID_HANDLE_VALUE) {
			WriteFile(file_handle, b.data(), DWORD(b.size()), nullptr, nullptr);
			SetEndOfFile(file_handle);
			CloseHandle(file_handle);
		}
	}

	{
		// compiled ui definitions
		ui_definitions defs;
	
		{ // fonts
			
		}
		{ // brushes
			// 0
			defs.brushes.push_back(brush_def{
				true,
				minui::brush_color{ 1.0f, 1.0f, 1.0f, 1.0f},
				L"assets\\yellowpaper.png",
				true,
				minui::brush_color{ 0.8f, 0.8f, 0.8f, 1.0f },
				L"assets\\yellowpaper_bw.png",
				-0.1f, - 0.05f, -0.15f
			});
			// 1
			defs.brushes.push_back(brush_def{
				false,
				minui::brush_color{ 0.0f, 0.0f, 0.0f, 0.0f },
				L"",
				false,
				minui::brush_color{ 0.6f, 0.6f, 0.6f, 1.0f },
				L"",
				0.1f, 0.05f, 0.15f
			});
			// 2
			defs.brushes.push_back(brush_def{
				false,
				minui::brush_color{ 0.811764f, 0.184313f, 0.086274f, 1.0f },
				L"",
				false,
				minui::brush_color{ 0.5, 0.5, 0.5, 1.0f },
				L"",
				0.1f, 0.05f, 0.15f
			});
			// 2
			defs.brushes.push_back(brush_def{
				false,
				minui::brush_color{ 0.811764f, 0.184313f, 0.086274f, 1.0f },
				L"",
				false,
				minui::brush_color{ 0.5, 0.5, 0.5, 1.0f },
				L"",
				0.1f, 0.05f, 0.15f
			});
			// 3
			defs.brushes.push_back(brush_def{
				false,
				minui::brush_color{ 0.086274f, 0.184313f, 0.811764f, 1.0f },
				L"",
				false,
				minui::brush_color{ 0.5, 0.5, 0.5, 1.0f },
				L"",
				0.1f, 0.05f, 0.15f
			});
		}
		{ // icons
			// 0 -- labels and groups
			defs.icons.emplace_back();
			defs.icons[0].xsize = minui::em{ 100 };
			defs.icons[0].ysize = minui::em{ 100 };
			defs.icons[0].sub_slots.resize(8);

			for(uint32_t i = 0; i < 8; ++i)
				defs.icons[0].sub_slots[i].is_svg = true;
			
			defs.icons[0].sub_slots[uint32_t(minui::interactable_orientation::left)].file = L"assets\\left_select_n.svg";
			defs.icons[0].sub_slots[uint32_t(minui::interactable_orientation::right)].file = L"assets\\right_select_n.svg";
			defs.icons[0].sub_slots[uint32_t(minui::interactable_orientation::above)].file = L"assets\\top_select_n.svg";
			defs.icons[0].sub_slots[uint32_t(minui::interactable_orientation::below)].file = L"assets\\bottom_select_n.svg";
			defs.icons[0].sub_slots[uint32_t(minui::interactable_orientation::left) + 4].file = L"assets\\group_select_n.svg";
			defs.icons[0].sub_slots[uint32_t(minui::interactable_orientation::right) + 4].file = L"assets\\group_select_n.svg";
			defs.icons[0].sub_slots[uint32_t(minui::interactable_orientation::above) + 4].file = L"assets\\group_select_n.svg";
			defs.icons[0].sub_slots[uint32_t(minui::interactable_orientation::below) + 4].file = L"assets\\group_select_n.svg";

			// 1 - page button
			defs.icons.emplace_back();
			defs.icons[1].xsize = minui::em{ 100 };
			defs.icons[1].ysize = minui::em{ 100 };
			defs.icons[1].sub_slots.resize(1);
			defs.icons[1].sub_slots[0].is_svg = true;
			defs.icons[1].sub_slots[0].file = L"assets\\control_prev_prev.svg";

			// 2 - page button
			defs.icons.emplace_back();
			defs.icons[2].xsize = minui::em{ 100 };
			defs.icons[2].ysize = minui::em{ 100 };
			defs.icons[2].sub_slots.resize(1);
			defs.icons[2].sub_slots[0].is_svg = true;
			defs.icons[2].sub_slots[0].file = L"assets\\control_prev.svg";

			// 3 - page button
			defs.icons.emplace_back();
			defs.icons[3].xsize = minui::em{ 100 };
			defs.icons[3].ysize = minui::em{ 100 };
			defs.icons[3].sub_slots.resize(1);
			defs.icons[3].sub_slots[0].is_svg = true;
			defs.icons[3].sub_slots[0].file = L"assets\\control_next.svg";

			// 4 - page button
			defs.icons.emplace_back();
			defs.icons[4].xsize = minui::em{ 100 };
			defs.icons[4].ysize = minui::em{ 100 };
			defs.icons[4].sub_slots.resize(1);
			defs.icons[4].sub_slots[0].is_svg = true;
			defs.icons[4].sub_slots[0].file = L"assets\\control_next_next.svg";

			// 5 - list button
			defs.icons.emplace_back();
			defs.icons[5].xsize = minui::em{ 100 };
			defs.icons[5].ysize = minui::em{ 100 };
			defs.icons[5].sub_slots.resize(1);
			defs.icons[5].sub_slots[0].is_svg = true;
			defs.icons[5].sub_slots[0].file = L"assets\\control_list.svg";

			// 6 - pages
			defs.icons.emplace_back();
			defs.icons[6].xsize = minui::em{ 100 };
			defs.icons[6].ysize = minui::em{ 100 };
			defs.icons[6].sub_slots.resize(1);
			defs.icons[6].sub_slots[0].is_svg = true;
			defs.icons[6].sub_slots[0].file = L"assets\\control_pages.svg";
		}

		constexpr auto num_elements = 6;

		defs.d_icon_position.resize(num_elements);
		defs.d_default_position.resize(num_elements);
		defs.d_interactable_definition.resize(num_elements);
		defs.d_icon.resize(num_elements);
		defs.d_class.resize(num_elements);
		defs.d_standard_flags.resize(num_elements);
		defs.d_foreground_brush.resize(num_elements);
		defs.d_background_brush.resize(num_elements);
		defs.d_highlight_brush.resize(num_elements);
		defs.d_info_brush.resize(num_elements);
		defs.d_variable_definition.resize(num_elements);
		defs.d_total_variable_size.resize(num_elements);
		defs.d_background_definition.resize(num_elements);

		{ // main window
			defs.d_icon_position[0] = minui::layout_position{ minui::em{ 0 }, minui::em{ 0 } };
			defs.d_default_position[0] = minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 1000 }, minui::em{ 1000 } };
			defs.d_interactable_definition[0] = minui::interactable_definition{ minui::interactable_orientation::left, minui::interactable_placement::internal };
			defs.d_icon[0] = minui::icon_handle{ -1 };
			defs.d_class[0] = 1;
			defs.d_standard_flags[0] = minui::behavior::transparent_to_focus;
			defs.d_foreground_brush[0] = 1;
			defs.d_background_brush[0] = 0;
			defs.d_highlight_brush[0] = 2;
			defs.d_info_brush[0] = 3;
			defs.d_variable_definition[0].resize(1);
			defs.d_variable_definition[0][0] = minui::variable_definition{ 0 /*variable*/, 0 /*data type -- ptr to info*/, 0 /*offset*/, { char(0) } };
			defs.d_total_variable_size[0] = 1;
			defs.d_background_definition[0] = minui::background_definition{ 
				minui::image_handle{ -1 },
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // ext. edge offsets
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // interior region
				0, // brush
				0, // left border
				0, //right border
				0, //top border
				0 //bottom border
			};
			// TODO: children
		}

		{ // generic page control left left
			defs.d_icon_position[1] = minui::layout_position{ minui::em{ 0 }, minui::em{ 0 } };
			defs.d_default_position[1] = minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 100 }, minui::em{ 100 } };
			defs.d_interactable_definition[1] = minui::interactable_definition{ minui::interactable_orientation::left, minui::interactable_placement::internal };
			defs.d_icon[1] = minui::icon_handle{ 1 };
			defs.d_class[1] = 13;
			defs.d_standard_flags[1] = minui::behavior::interaction_command | minui::behavior::visually_interactable;
			defs.d_foreground_brush[1] = 1;
			defs.d_background_brush[1] = 0;
			defs.d_highlight_brush[1] = 2;
			defs.d_info_brush[1] = 3;
			defs.d_total_variable_size[1] = 0;
			defs.d_background_definition[1] = minui::background_definition{
				minui::image_handle{ -1 },
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // ext. edge offsets
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // interior region
				uint16_t(-1), // brush
				0, // left border
				0, //right border
				0, //top border
				0 //bottom border
			};
		}
		{ // generic page control left
			defs.d_icon_position[2] = minui::layout_position{ minui::em{ 0 }, minui::em{ 0 } };
			defs.d_default_position[2] = minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 100 }, minui::em{ 100 } };
			defs.d_interactable_definition[2] = minui::interactable_definition{ minui::interactable_orientation::left, minui::interactable_placement::internal };
			defs.d_icon[2] = minui::icon_handle{ 2 };
			defs.d_class[2] = 13;
			defs.d_standard_flags[2] = minui::behavior::interaction_command | minui::behavior::visually_interactable;
			defs.d_foreground_brush[2] = 1;
			defs.d_background_brush[2] = 0;
			defs.d_highlight_brush[2] = 2;
			defs.d_info_brush[2] = 3;
			defs.d_total_variable_size[2] = 0;
			defs.d_background_definition[2] = minui::background_definition{
				minui::image_handle{ -1 },
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // ext. edge offsets
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // interior region
				uint16_t(-1), // brush
				0, // left border
				0, //right border
				0, //top border
				0 //bottom border
			};
		}
		{ // generic page control right
			defs.d_icon_position[3] = minui::layout_position{ minui::em{ 0 }, minui::em{ 0 } };
			defs.d_default_position[3] = minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 100 }, minui::em{ 100 } };
			defs.d_interactable_definition[3] = minui::interactable_definition{ minui::interactable_orientation::left, minui::interactable_placement::internal };
			defs.d_icon[3] = minui::icon_handle{ 3 };
			defs.d_class[3] = 13;
			defs.d_standard_flags[3] = minui::behavior::interaction_command | minui::behavior::visually_interactable;
			defs.d_foreground_brush[3] = 1;
			defs.d_background_brush[3] = 0;
			defs.d_highlight_brush[3] = 2;
			defs.d_info_brush[3] = 3;
			defs.d_total_variable_size[3] = 0;
			defs.d_background_definition[3] = minui::background_definition{
				minui::image_handle{ -1 },
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // ext. edge offsets
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // interior region
				uint16_t(-1), // brush
				0, // left border
				0, //right border
				0, //top border
				0 //bottom border
			};
		}
		{ // generic page control right right
			defs.d_icon_position[4] = minui::layout_position{ minui::em{ 0 }, minui::em{ 0 } };
			defs.d_default_position[4] = minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 100 }, minui::em{ 100 } };
			defs.d_interactable_definition[4] = minui::interactable_definition{ minui::interactable_orientation::left, minui::interactable_placement::internal };
			defs.d_icon[4] = minui::icon_handle{ 4 };
			defs.d_class[4] = 13;
			defs.d_standard_flags[4] = minui::behavior::interaction_command | minui::behavior::visually_interactable;
			defs.d_foreground_brush[4] = 1;
			defs.d_background_brush[4] = 0;
			defs.d_highlight_brush[4] = 2;
			defs.d_info_brush[4] = 3;
			defs.d_total_variable_size[4] = 0;
			defs.d_background_definition[4] = minui::background_definition{
				minui::image_handle{ -1 },
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // ext. edge offsets
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // interior region
				uint16_t(-1), // brush
				0, // left border
				0, //right border
				0, //top border
				0 //bottom border
			};
		}
		{ // generic page control text
			defs.d_icon_position[5] = minui::layout_position{ minui::em{ 0 }, minui::em{ 0 } };
			defs.d_default_position[5] = minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 100 }, minui::em{ 100 } };
			defs.d_interactable_definition[5] = minui::interactable_definition{ minui::interactable_orientation::left, minui::interactable_placement::internal };
			defs.d_icon[5] = minui::icon_handle{ -1 };
			defs.d_class[5] = 14;
			defs.d_standard_flags[5] = 0;
			defs.d_foreground_brush[5] = 1;
			defs.d_background_brush[5] = 0;
			defs.d_highlight_brush[5] = 2;
			defs.d_info_brush[5] = 3;
			defs.d_total_variable_size[5] = 0;
			defs.d_background_definition[5] = minui::background_definition{
				minui::image_handle{ -1 },
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // ext. edge offsets
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // interior region
				uint16_t(-1), // brush
				0, // left border
				0, //right border
				0, //top border
				0 //bottom border
			};

			defs.d_text_information.insert_or_assign(5, text_information_def{
				std::string(""),
				minui::layout_rect{ minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 }, minui::em{ 0 } }, // margins
				minui::text::font_handle{ 0 },
				minui::text::content_alignment::centered,
				minui::em{ 100 }, // min space
				true // multiline
			});
		}

		defs.save_to_file(L"ui.dat");
	}
}
