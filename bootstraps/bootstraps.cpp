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
		// compiled ui definitions

		ui_definitions defs;
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


		{ // brushes
			// 0
			defs.brushes.push_back(brush_def{
				true,
				minui::brush_color{ 1.0f, 1.0f, 1.0f, 1.0f},
				L"yellowpaper.png",
				true,
				minui::brush_color{ 0.8f, 0.8f, 0.8f, 1.0f },
				L"yellowpaper_bw.png",
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


		defs.save_to_file(L"ui.dat");
	}
}
