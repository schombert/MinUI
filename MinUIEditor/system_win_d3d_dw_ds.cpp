#include "system_win_d3d_dw_ds.hpp"
#include "../common_files/minui_text_impl.cpp"
#include "../common_files/minui_core_impl.cpp"
#include <windowsx.h>
#include <chrono>
#include <Shlobj.h>
#include <dxgi1_2.h>
#include <d3d11_3.h>
#include <DirectXMath.h>
#include <d2d1_3.h>
#include <d2d1helper.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <shlwapi.h>
#include <ole2.h>
#include <UIAutomation.h>
#include <Shlobj.h>
#include <wchar.h>
#include <usp10.h>
#include <initguid.h>
#include <inputscope.h>
#include <Textstor.h>
#include <tsattrs.h>
#include <msctf.h>
#include <olectl.h>
#include <icu.h>

#define XINPUT_ON_GAMEINPUT_NO_XINPUTENABLE

#include "xinput.h"
#include <hidsdi.h>
#include <hidpi.h>

#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Xinput9_1_0.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Windowscodecs.lib")
#pragma comment(lib, "Usp10.lib")
#pragma comment(lib, "icu.lib")
#pragma comment(lib, "Dwrite.lib")

namespace minui {

locale_data_s::locale_data_s() {
	parse_into_cardinality_fns(cardinal_functions, L"am as bn doi fa gu hi kn pcm zu", text::card_2am);
	parse_into_cardinality_fns(cardinal_functions, L"ff hy kab", text::card_2ff);
	parse_into_cardinality_fns(cardinal_functions, L"ast de en et fi fy gl ia io ji lij nl sc scn sv sw ur yi", text::card_2ast);
	cardinal_functions.insert_or_assign(L"si", text::card_2si);
	parse_into_cardinality_fns(cardinal_functions, L"ak bho guw ln mg nso pa ti wa", text::card_2ak);
	cardinal_functions.insert_or_assign(L"tzm", text::card_2tzm);
	parse_into_cardinality_fns(cardinal_functions, L"af an asa az bal bem bez bg brx ce cgg chr ckb dv ee el eo eu fo fur gsw ha haw hu jgo jmc ka kaj kcg kk kkj kl ks ksb ku ky lb lg mas mgo ml mn mr nah nb nd ne nn nnh no nr ny nyn om or os pap ps rm rof rwk saq sd sdh seh sn so sq ss ssy st syr ta te teo tig tk tn tr ts ug uz ve vo vun wae xh xog", text::card_2af);
	cardinal_functions.insert_or_assign(L"da", text::card_2da);
	cardinal_functions.insert_or_assign(L"is", text::card_2is);
	cardinal_functions.insert_or_assign(L"mk", text::card_2mk);
	parse_into_cardinality_fns(cardinal_functions, L"ceb fil tl", text::card_2ceb);

	parse_into_cardinality_fns(cardinal_functions, L"lv prg", text::card_3lv);
	cardinal_functions.insert_or_assign(L"lag", text::card_3lag);
	cardinal_functions.insert_or_assign(L"ksh", text::card_3ksh);
	parse_into_cardinality_fns(cardinal_functions, L"he iw", text::card_3he);
	parse_into_cardinality_fns(cardinal_functions, L"iu naq sat se sma smi smj smn sms", text::card_3iu);

	cardinal_functions.insert_or_assign(L"shi", text::card_3shi);
	parse_into_cardinality_fns(cardinal_functions, L"mo ro", text::card_3mo);
	parse_into_cardinality_fns(cardinal_functions, L"bs hr sh sr", text::card_3bs);

	cardinal_functions.insert_or_assign(L"fr", text::card_3fr);
	cardinal_functions.insert_or_assign(L"pt", text::card_3pt);
	parse_into_cardinality_fns(cardinal_functions, L"ca it pt-PT vec", text::card_3ca);
	cardinal_functions.insert_or_assign(L"es", text::card_3es);

	cardinal_functions.insert_or_assign(L"gd", text::card_4gd);
	cardinal_functions.insert_or_assign(L"sl", text::card_4sl);
	parse_into_cardinality_fns(cardinal_functions, L"dsb hsb", text::card_4dsb);

	parse_into_cardinality_fns(cardinal_functions, L"cs sk", text::card_4cs);
	cardinal_functions.insert_or_assign(L"pl", text::card_4pl);
	cardinal_functions.insert_or_assign(L"be", text::card_4be);
	cardinal_functions.insert_or_assign(L"lt", text::card_4lt);
	parse_into_cardinality_fns(cardinal_functions, L"ru uk", text::card_4ru);

	cardinal_functions.insert_or_assign(L"br", text::card_5br);
	cardinal_functions.insert_or_assign(L"mt", text::card_5mt);
	cardinal_functions.insert_or_assign(L"ga", text::card_5ga);
	cardinal_functions.insert_or_assign(L"gv", text::card_5gv);

	cardinal_functions.insert_or_assign(L"kw", text::card_6kw);
	parse_into_cardinality_fns(cardinal_functions, L"ar ars", text::card_6ar);
	cardinal_functions.insert_or_assign(L"cy", text::card_6cy);


	ordinal_functions.insert_or_assign(L"sv", text::ord_2sv);
	parse_into_ordinal_fns(ordinal_functions, L"bal fil fr ga hy lo mo ms ro tl vi", text::ord_2bal);
	ordinal_functions.insert_or_assign(L"hu", text::ord_2hu);
	ordinal_functions.insert_or_assign(L"ne", text::ord_2ne);

	ordinal_functions.insert_or_assign(L"be", text::ord_2be);
	ordinal_functions.insert_or_assign(L"uk", text::ord_2uk);
	ordinal_functions.insert_or_assign(L"tk", text::ord_2tk);

	ordinal_functions.insert_or_assign(L"kk", text::ord_2kk);
	parse_into_ordinal_fns(ordinal_functions, L"it sc scn vec", text::ord_2it);
	ordinal_functions.insert_or_assign(L"lij", text::ord_2lij);

	ordinal_functions.insert_or_assign(L"ka", text::ord_3ka);
	ordinal_functions.insert_or_assign(L"sq", text::ord_3sq);
	ordinal_functions.insert_or_assign(L"kw", text::ord_3kw);

	ordinal_functions.insert_or_assign(L"en", text::ord_4en);
	ordinal_functions.insert_or_assign(L"mr", text::ord_4mr);
	ordinal_functions.insert_or_assign(L"gd", text::ord_4gd);
	ordinal_functions.insert_or_assign(L"ca", text::ord_4ca);

	ordinal_functions.insert_or_assign(L"mk", text::ord_4mk);

	ordinal_functions.insert_or_assign(L"az", text::ord_4az);

	parse_into_ordinal_fns(ordinal_functions, L"gu hi", text::ord_5gu);
	parse_into_ordinal_fns(ordinal_functions, L"as bn", text::ord_5as);
	ordinal_functions.insert_or_assign(L"or", text::ord_5or);

	ordinal_functions.insert_or_assign(L"cy", text::ord_6cy);
}

void parse_into_cardinality_fns(ankerl::unordered_dense::map<std::wstring_view, cardinal_plural_fn>& map, std::wstring_view list, cardinal_plural_fn fn) {

	for(size_t pos = 0; pos < list.length(); pos = list.find(' ', pos)) {
		if(pos != 0)
			++pos;
		auto next = std::min(list.find(L' ', pos), list.length());
		map.insert_or_assign(list.substr(pos, next - pos), fn);
	}
}

void parse_into_ordinal_fns(ankerl::unordered_dense::map<std::wstring_view, ordinal_plural_fn>& map, std::wstring_view list, ordinal_plural_fn fn) {

	for(size_t pos = 0; pos < list.length(); pos = list.find(' ', pos)) {
		if(pos != 0)
			++pos;
		auto next = std::min(list.find(L' ', pos), list.length());
		map.insert_or_assign(list.substr(pos, next - pos), fn);
	}
}

void win_d2d_dw_ds::add_font_file_to_collection(native_string_view file_name) {
	std::wstring fname = std::wstring(file_name);
	dw_font_collection_builder->AddFontFile(fname.c_str());
}
void win_d2d_dw_ds::resize_text() {
	++font_generation;

	for(auto& f : font_collection) {
		DWRITE_FONT_AXIS_VALUE fax[] = {
			DWRITE_FONT_AXIS_VALUE{ DWRITE_FONT_AXIS_TAG_WEIGHT, float(f.info.weight) },
			DWRITE_FONT_AXIS_VALUE{ DWRITE_FONT_AXIS_TAG_WIDTH, f.info.span },
			DWRITE_FONT_AXIS_VALUE{ DWRITE_FONT_AXIS_TAG_OPTICAL_SIZE, f.info.font_size * 96.0f / dpi } };

		// METRICS
		IDWriteFont3* fon = nullptr;
		IDWriteFontList2* fl;

		dw_font_collection->GetMatchingFonts(f.info.name.c_str(), fax, 2, &fl);
		fl->GetFont(0, &fon);
		safe_release(fl);

		float global_multiplier = 1.0f;

		DWRITE_FONT_METRICS metrics;
		fon->GetMetrics(&metrics);
		f.info.line_spacing = float(pixels_per_em) * f.info.scale;

		auto top_lead = std::floor(f.info.top_leading * global_size_multiplier * dpi / 96.0f);
		auto bottom_lead = std::floor(f.info.bottom_leading * global_size_multiplier * dpi / 96.0f);

		// try to get cap height an integral number of pixels
		auto temp_font_size = float(metrics.designUnitsPerEm) * (pixels_per_em - (top_lead + bottom_lead)) / float(metrics.ascent + metrics.descent + metrics.lineGap);
		auto temp_cap_height = float(metrics.capHeight) * temp_font_size / (float(metrics.designUnitsPerEm));
		auto desired_cap_height = std::round(temp_cap_height);
		auto adjusted_size = (metrics.capHeight > 0 && metrics.capHeight <= metrics.ascent) ? desired_cap_height * float(metrics.designUnitsPerEm) / float(metrics.capHeight) : temp_font_size;
		{
			//	auto temp_x_height = float(metrics.xHeight) * temp_font_size / (float(metrics.designUnitsPerEm));
			//	auto desired_x_height = std::round(temp_x_height);
			//	adjusted_size = (metrics.xHeight > 0 && metrics.xHeight < metrics.ascent) ? desired_x_height * float(metrics.designUnitsPerEm) / float(metrics.xHeight) : adjusted_size;
		}

		f.info.font_size = adjusted_size;
		f.info.baseline = std::round(top_lead + float(metrics.ascent + metrics.lineGap / 2) * temp_font_size / (float(metrics.designUnitsPerEm)));

		f.info.descender = float(metrics.descent) * f.info.font_size / (float(metrics.designUnitsPerEm));

		IDWriteTextAnalyzer* analyzer_base = nullptr;
		IDWriteTextAnalyzer1* analyzer = nullptr;
		auto hr = dwrite_factory->CreateTextAnalyzer(&analyzer_base);

		f.info.vertical_baseline = std::round(pixels_per_em / 2.0f);

		if(SUCCEEDED(hr)) {
			hr = analyzer_base->QueryInterface(IID_PPV_ARGS(&analyzer));
		}
		if(SUCCEEDED(hr)) {
			IDWriteFontFace* face = nullptr;
			fon->CreateFontFace(&face);

			BOOL vert_exists_out = FALSE;
			int32_t vert_base_out = 0;
			analyzer->GetBaseline(face, DWRITE_BASELINE_CENTRAL, TRUE, TRUE, DWRITE_SCRIPT_ANALYSIS{ }, locale_data.app_locale.c_str(), &vert_base_out, &vert_exists_out);

			f.info.vertical_baseline = std::round(top_lead + float(vert_base_out) * f.info.font_size / (float(metrics.designUnitsPerEm)));

			safe_release(face);
		}
		safe_release(analyzer);
		safe_release(analyzer_base);

		safe_release(fon);

		//CREATE FORMAT
		safe_release(f.format);

		dwrite_factory->CreateTextFormat(f.info.name.c_str(), dw_font_collection, fax, 3, f.info.font_size, locale_data.app_locale.c_str(), &f.format);
		f.format->SetFontFallback(f.fallback);
		f.format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		f.format->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, f.info.line_spacing, f.info.baseline);
		f.format->SetAutomaticFontAxes(DWRITE_AUTOMATIC_FONT_AXES_OPTICAL_SIZE);
	}
}
void win_d2d_dw_ds::finalize_font_collection() {
	IDWriteFontSet* sysfs = nullptr;
	dwrite_factory->GetSystemFontSet(&sysfs);
	if(sysfs) dw_font_collection_builder->AddFontSet(sysfs);

	IDWriteFontSet* fs = nullptr;
	dw_font_collection_builder->CreateFontSet(&fs);
	dwrite_factory->CreateFontCollectionFromFontSet(fs, DWRITE_FONT_FAMILY_MODEL_TYPOGRAPHIC, &dw_font_collection);

	safe_release(fs);
	safe_release(sysfs);
	safe_release(dw_font_collection_builder);

	IDWriteFontFallback* system_fallback;
	dwrite_factory->GetSystemFontFallback(&system_fallback);

	for(auto& f : font_collection) {
		// CREATE FALLBACKS
		safe_release(f.fallback);
		IDWriteFontFallbackBuilder* bldr = nullptr;
		dwrite_factory->CreateFontFallbackBuilder(&bldr);

		for(auto& fb : f.fallbacks) {
			WCHAR const* fn[] = { fb.name.c_str() };
			std::vector<DWRITE_UNICODE_RANGE> ranges;
			for(auto& r : fb.ranges) {
				ranges.push_back(DWRITE_UNICODE_RANGE{ r.start, r.end });
			}

			bldr->AddMapping(ranges.data(), uint32_t(ranges.size()), fn, 1, dw_font_collection, nullptr, nullptr, fb.scale);
			
		}

		bldr->AddMappings(system_fallback);

		bldr->CreateFontFallback(&f.fallback);
		safe_release(bldr);
	}

	safe_release(system_fallback);
}
void win_d2d_dw_ds::make_font_definition(text::font_handle slot, text::font&& font_object) {
	if(slot.id >= font_collection.size()) {
		font_collection.resize(slot.id + 1);
	}
	font_collection[slot.id].info = std::move(font_object);
}
void win_d2d_dw_ds::add_font_fallback(text::font_handle slot, text::font_fallback&& font_object) {
	font_collection[slot.id].fallbacks.push_back(std::move(font_object));
}
void win_d2d_dw_ds::add_localization_file(native_string_view file_name) {

}

UINT GetGrouping(WCHAR const* locale) {
	WCHAR wszGrouping[32];
	if(!GetLocaleInfoEx(locale, LOCALE_SGROUPING, wszGrouping, ARRAYSIZE(wszGrouping)))
		return 3;

	UINT nGrouping = 0;
	PWSTR pwsz = wszGrouping;
	for(;;) {
		if(*pwsz == '0') {
			break;
		} else if(iswdigit(*pwsz)) {
			nGrouping = nGrouping * 10 + (UINT)(*pwsz - '0');
		} else if(*pwsz == 0) {
			nGrouping = nGrouping * 10;
			break;
		}
		++pwsz;
	}
	return nGrouping;
}

void win_d2d_dw_ds::set_locale(native_string_view id) {
	++language_generation;

	// clear all locale related resources
	font_collection.clear();
	safe_release(dw_font_collection);
	safe_release(dw_font_collection_builder);
	text::reset(text_data);

	dwrite_factory->CreateFontSetBuilder(&dw_font_collection_builder);

	std::wstring app_lang = std::wstring(id.substr(0, id.find(L'-', 0)));
	locale_data.app_locale = std::wstring(id);

	if(id.length() > 0 && IsValidLocaleName(locale_data.app_locale.c_str())) {
		locale_data.os_locale_is_default = false;
	} else {
		WCHAR buffer[LOCALE_NAME_MAX_LENGTH] = { 0 };
		GetUserDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		locale_data.app_locale = buffer;
		locale_data.os_locale_is_default = true;
	}

	if(auto it = locale_data.cardinal_functions.find(locale_data.app_locale); it != cardinal_functions.end()) {
		locale_data.cardinal_classification = it->second;
	} else if(auto itb = cardinal_functions.find(app_lang); itb != cardinal_functions.end()) {
		locale_data.cardinal_classification = itb->second;
	} else {
		locale_data.cardinal_classification = [](int64_t, int64_t, int32_t) { return text::attribute_type::other; };
	}

	if(auto it = locale_data.ordinal_functions.find(locale_data.app_locale); it != ordinal_functions.end()) {
		locale_data.ordinal_classification = it->second;
	} else if(auto itb = ordinal_functions.find(app_lang); itb != ordinal_functions.end()) {
		locale_data.ordinal_classification = itb->second;
	} else {
		locale_data.ordinal_classification = [](int64_t) { return text::attribute_type::ord_other; };
	}


	{
		DWORD temp = 0;
		GetLocaleInfoEx(locale_data.os_locale_is_default ? LOCALE_NAME_USER_DEFAULT : locale_data.app_locale.c_str(), LOCALE_ILZERO | LOCALE_RETURN_NUMBER, (LPWSTR)&temp, sizeof(temp) / sizeof(WCHAR));
		locale_data.leading_zero = temp;
	}
	{
		DWORD temp = 0;
		GetLocaleInfoEx(locale_data.os_locale_is_default ? LOCALE_NAME_USER_DEFAULT : locale_data.app_locale.c_str(), LOCALE_INEGNUMBER | LOCALE_RETURN_NUMBER, (LPWSTR)&temp, sizeof(temp) / sizeof(WCHAR));
		locale_data.negative_order = temp;
	}

	locale_data.grouping = GetGrouping(locale_data.os_locale_is_default ? LOCALE_NAME_USER_DEFAULT : locale_data.app_locale.c_str());

	GetLocaleInfoEx(locale_data.os_locale_is_default ? LOCALE_NAME_USER_DEFAULT : locale_data.app_locale.c_str(), LOCALE_SDECIMAL, locale_data.decimal_sep, 5);
	GetLocaleInfoEx(locale_data.os_locale_is_default ? LOCALE_NAME_USER_DEFAULT : locale_data.app_locale.c_str(), LOCALE_STHOUSAND, locale_data.thousand_sep, 5);

	locale_data.lcid = locale_data.os_locale_is_default ? LocaleNameToLCID(LOCALE_NAME_USER_DEFAULT, 0) : LocaleNameToLCID(locale_data.app_locale.c_str(), LOCALE_ALLOW_NEUTRAL_NAMES);

	//set_window_title();
}
void win_d2d_dw_ds::set_ltr_mode(bool is_ltr) {
	left_to_right = is_ltr;
}
native_string_view win_d2d_dw_ds::get_locale_name() {
	return locale_data.display_name;
}

native_string_view win_d2d_dw_ds::get_locale() {
	return locale_data.app_locale;
}

void win_d2d_dw_ds::on_dpi_change(float new_dpi) {
	dpi = new_dpi;

	pixels_per_em = int32_t(std::round(float(base_layout_size) * dpi / 96.0f));
	window_border_size = int32_t(std::round(float(base_border_size) * dpi / 96.0f));

	stop_ui_animations();

	recreate_dpi_dependent_resource();
	create_window_size_resources();
	set_ltr_mode(left_to_right);
}

void win_d2d_dw_ds::recreate_dpi_dependent_resource() {
	resize_text();

	safe_release(rendering_params);
	assert(m_hwnd);

	IDWriteRenderingParams* rparams = nullptr;
	auto monitor_handle = MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTOPRIMARY);
	dwrite_factory->CreateMonitorRenderingParams(monitor_handle, &rparams);
	if(rparams) {
		float default_gamma = rparams->GetGamma();
		float default_contrast = rparams->GetEnhancedContrast();

		IDWriteRenderingParams3* rparams3 = nullptr;
		rparams->QueryInterface(IID_PPV_ARGS(&rparams3));

		float default_gs_contrast = rparams3 ? rparams3->GetGrayscaleEnhancedContrast() : 1.0f;
		auto grid_fit = rparams3 ? rparams3->GetGridFitMode() : DWRITE_GRID_FIT_MODE_DEFAULT;

		dwrite_factory->CreateCustomRenderingParams(
			default_gamma,
			0.0f,
			0.0f,
			0.5f,
			DWRITE_PIXEL_GEOMETRY_FLAT,
			DWRITE_RENDERING_MODE1_NATURAL_SYMMETRIC,
			grid_fit,
			&rendering_params);

		safe_release(rparams3);
		safe_release(rparams);
	}
}
void win_d2d_dw_ds::create_window_size_resources() {

}
void win_d2d_dw_ds::d2dsetup() {
	// TODO

	d2d_device_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &solid_brush);
}

void win_d2d_dw_ds::reshow_cursor() {
	if(cursor_is_visible) {
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
	} else {
		SetCursor(nullptr);
	}
}

layout_position win_d2d_dw_ds::get_workspace() const {
	auto monitor_handle = MonitorFromWindow((HWND)(m_hwnd), MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(monitor_handle, &mi);

	return layout_position{ to_ui_space((mi.rcWork.right - mi.rcWork.left)  - window_border_size), to_ui_space((mi.rcWork.bottom - mi.rcWork.top) - window_border_size), };
}
void win_d2d_dw_ds::hide_mouse_cursor() {
	SetCursor(nullptr);
	cursor_is_visible = false;
}
void win_d2d_dw_ds::show_mouse_cursor() {
	SetCursor(LoadCursor(nullptr, IDC_ARROW));
	cursor_is_visible = true;
}
bool win_d2d_dw_ds::is_mouse_cursor_visible() const {
	return cursor_is_visible;
}
uint32_t win_d2d_dw_ds::get_window_dpi() const {
	return GetDpiForWindow((HWND)(m_hwnd));
}
bool win_d2d_dw_ds::is_maximized() const {
	return IsZoomed((HWND)(m_hwnd));
}
bool win_d2d_dw_ds::is_minimized() const {
	return IsIconic((HWND)(m_hwnd));
}
void win_d2d_dw_ds::maximize() {
	ShowWindow((HWND)(m_hwnd), SW_MAXIMIZE);
	//win.accessibility_interface.notify_window_state_change(resize_type::maximize);
}
void win_d2d_dw_ds::minimize() {
	ShowWindow((HWND)(m_hwnd), SW_MINIMIZE);
	//win.accessibility_interface.notify_window_state_change(resize_type::minimize);
}
void win_d2d_dw_ds::restore() {
	ShowWindow((HWND)(m_hwnd), SW_SHOWNORMAL);
	//win.accessibility_interface.notify_window_state_change(resize_type::resize);
}
void win_d2d_dw_ds::close() {
	//win.accessibility_interface.notify_window_closed();
	PostMessage((HWND)(m_hwnd), WM_CLOSE, 0, 0);
}
void win_d2d_dw_ds::set_window_title(native_char const* t) {
	SetWindowText((HWND)(m_hwnd), t);
}
bool win_d2d_dw_ds::window_has_focus() const {
	return GetForegroundWindow() == (HWND)(m_hwnd);
}

struct controller_button_state {
	uint32_t val = 0;

	constexpr static uint32_t button_y = 0x0001;
	constexpr static uint32_t button_x = 0x0002;
	constexpr static uint32_t button_b = 0x0004;
	constexpr static uint32_t button_a = 0x0008;
	constexpr static uint32_t button_lb = 0x0010;
	constexpr static uint32_t button_rb = 0x0020;
	constexpr static uint32_t button_start = 0x0040;
	constexpr static uint32_t button_back = 0x0080;

	constexpr static uint32_t thumb_left = 0x0100;
	constexpr static uint32_t thumb_right = 0x0200;

	constexpr static uint32_t dpad_up = 0x0400;
	constexpr static uint32_t dpad_down = 0x0800;
	constexpr static uint32_t dpad_left = 0x1000;
	constexpr static uint32_t dpad_right = 0x2000;
};

uint32_t to_bitmask(controller_button b) {
	switch(b) {
		case controller_button::y:
			return controller_button_state::button_y;
		case controller_button::x:
			return controller_button_state::button_x;
		case controller_button::b:
			return controller_button_state::button_b;
		case controller_button::a:
			return controller_button_state::button_a;
		case controller_button::lb:
			return controller_button_state::button_lb;
		case controller_button::rb:
			return controller_button_state::button_rb;
		case controller_button::start:
			return controller_button_state::button_start;
		case controller_button::back:
			return controller_button_state::button_back;
		case controller_button::tleft:
			return controller_button_state::thumb_left;
		case controller_button::tright:
			return controller_button_state::thumb_right;
		case controller_button::dup:
			return controller_button_state::dpad_up;
		case controller_button::ddown:
			return controller_button_state::dpad_down;
		case controller_button::dleft:
			return controller_button_state::dpad_left;
		case controller_button::dright:
			return controller_button_state::dpad_right;
		case controller_button::no_button:
			return 0;
		default:
			return 0;
	}
}

uint32_t to_xinput_bitmask(controller_button b) {
	switch(b) {
		case controller_button::y:
			return XINPUT_GAMEPAD_Y;
		case controller_button::x:
			return XINPUT_GAMEPAD_X;
		case controller_button::b:
			return XINPUT_GAMEPAD_B;
		case controller_button::a:
			return XINPUT_GAMEPAD_A;
		case controller_button::lb:
			return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case controller_button::rb:
			return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		case controller_button::start:
			return XINPUT_GAMEPAD_START;
		case controller_button::back:
			return XINPUT_GAMEPAD_BACK;
		case controller_button::tleft:
			return XINPUT_GAMEPAD_LEFT_THUMB;
		case controller_button::tright:
			return XINPUT_GAMEPAD_RIGHT_THUMB;
		case controller_button::dup:
			return XINPUT_GAMEPAD_DPAD_UP;
		case controller_button::ddown:
			return XINPUT_GAMEPAD_DPAD_DOWN;
		case controller_button::dleft:
			return XINPUT_GAMEPAD_DPAD_LEFT;
		case controller_button::dright:
			return XINPUT_GAMEPAD_DPAD_RIGHT;
		case controller_button::no_button:
			return 0;
		default:
			return 0;
	}
}
void test_for_button(uint32_t& new_buttons_pressed, uint32_t& new_buttons_released, uint32_t& button_state, WORD gamepadstate, controller_button b) {
	if((gamepadstate & to_xinput_bitmask(b)) != 0) {
		if((button_state & to_bitmask(b)) == 0) {
			button_state |= to_bitmask(b);
			new_buttons_pressed |= to_bitmask(b);
		}
	} else {
		if((button_state & to_bitmask(b)) != 0) {
			button_state &= ~to_bitmask(b);
			new_buttons_released |= to_bitmask(b);
		}
	}
}

void test_for_all_buttons(uint32_t& buttons_pressed, uint32_t& buttons_released, uint32_t& button_state, WORD gamepadstate) {
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::lb);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::rb);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::y);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::x);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::b);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::a);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::tleft);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::tright);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::start);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::back);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::dleft);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::dright);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::dup);
	test_for_button(buttons_pressed, buttons_released, button_state, gamepadstate, controller_button::ddown);
}
void win_d2d_dw_ds::on_controller_input() {
	// end capture test

	if(imode == input_mode::follow_input) {
		// switch below
	} else if(imode != input_mode::controller_only && imode != input_mode::controller_with_pointer) {
		return;
	}

	for(int32_t i = 0; i < 4; ++i) {
		if(((1 << i) & controller_number_plugged_in) != 0) {
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			if(XInputGetState(i, &state) == ERROR_SUCCESS) {
				// Controller is connected


				if(imode == input_mode::controller_with_pointer) {
					double LX = std::max(state.Gamepad.sThumbLX, state.Gamepad.sThumbRX);
					double LY = std::max(state.Gamepad.sThumbLY, state.Gamepad.sThumbRY);

					//determine how far the controller is pushed
					double magnitude = sqrt(LX * LX + LY * LY);
					double norm_lx = LX / magnitude;
					double norm_ly = -LY / magnitude;
					magnitude = std::min(magnitude, 32767.0) / 32767.0;

					//check if the controller is outside a circular dead zone
					if(magnitude >= controller_deadzone) {
						if(controller_mouse_running) {
							controller_mouse_start = std::chrono::steady_clock::now();
							controller_mouse_running = true;

							POINT p{ };
							GetCursorPos(&p);

							controller_mouse_current_x = p.x;
							controller_mouse_current_y = p.y;
						} else {
							auto new_time = std::chrono::steady_clock::now();
							auto duration = new_time - controller_mouse_start;
							controller_mouse_start = new_time;
							auto in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

							auto scaled_magnitude = (magnitude * magnitude * magnitude) * 1.5;
							controller_mouse_current_x += double(controller_sensitivity * in_ms.count()) * norm_lx * scaled_magnitude;
							controller_mouse_current_y += double(controller_sensitivity * in_ms.count()) * norm_ly * scaled_magnitude;

							RECT rcClient;
							GetWindowRect((HWND)(m_hwnd), &rcClient);

							auto temp_x = long(controller_mouse_current_x);
							auto temp_y = long(controller_mouse_current_y);
							temp_x = std::clamp(temp_x, rcClient.left + window_border_size, rcClient.right - window_border_size);
							temp_y = std::clamp(temp_y, rcClient.top + window_border_size, rcClient.bottom - window_border_size);

							SetCursorPos(temp_x, temp_y);
						}


					} else {
						controller_mouse_running = false;
					}
				}

				uint32_t buttons_pressed = 0;
				uint32_t buttons_released = 0;

				test_for_all_buttons(buttons_pressed, buttons_released, controller_buttons_val, state.Gamepad.wButtons);

				if(!minui_root)
					continue;

				if(buttons_pressed || buttons_released) {
					if(imode == input_mode::follow_input) {
						if(minui_root && minui_root->pmode != prompt_mode::controller) {
							hide_mouse_cursor();
							minui_root->set_prompt_mode(prompt_mode::controller);
							minui_root->set_window_focus(focus_tracker{ nullptr, -1, -1 });
							return; // eat the key so that we don't do something bad
						}
					}

					if((buttons_pressed & to_bitmask(controller_button::lb)) != 0 || (buttons_pressed & to_bitmask(controller_button::rb)) != 0) {
						if((controller_buttons_val & to_bitmask(controller_button::lb)) != 0 && (controller_buttons_val & to_bitmask(controller_button::rb)) != 0)
							minui_root->on_key_down(100, VK_ESCAPE, false);

						controller_buttons_val &= ~to_bitmask(controller_button::x);
						controller_buttons_val &= ~to_bitmask(controller_button::y);
						controller_buttons_val &= ~to_bitmask(controller_button::a);
						controller_buttons_val &= ~to_bitmask(controller_button::b);
					}

					if((buttons_released & to_bitmask(controller_button::lb)) != 0) {
						if((buttons_released & to_bitmask(controller_button::rb)) != 0 || (controller_buttons_val & to_bitmask(controller_button::rb)) != 0) {
							minui_root->on_key_up(0x0001, VK_ESCAPE);
						} else { // clear button down state
							if((controller_buttons_val & to_bitmask(controller_button::x)) != 0) {
								minui_root->on_key_up(0x10, 0x51);
								controller_buttons_val &= ~to_bitmask(controller_button::x);
							}
							if((controller_buttons_val & to_bitmask(controller_button::y)) != 0) {
								minui_root->on_key_up(0x11, 0x57);
								controller_buttons_val &= ~to_bitmask(controller_button::y);
							}
							if((controller_buttons_val & to_bitmask(controller_button::a)) != 0) {
								minui_root->on_key_up(0x12, 0x45);
								controller_buttons_val &= ~to_bitmask(controller_button::a);
							}
							if((controller_buttons_val & to_bitmask(controller_button::b)) != 0) {
								minui_root->on_key_up(0x13, 0x52);
								controller_buttons_val &= ~to_bitmask(controller_button::b);
							}
						}
					}
					if((buttons_released & to_bitmask(controller_button::rb)) != 0) {
						if((buttons_released & to_bitmask(controller_button::lb)) != 0 || (controller_buttons_val & to_bitmask(controller_button::lb)) != 0) {
							minui_root->on_key_up(0x0001, VK_ESCAPE);
						} else { // clear button down state
							if((controller_buttons_val & to_bitmask(controller_button::x)) != 0) {
								minui_root->on_key_up(0x2C, 0x5A);
								controller_buttons_val &= ~to_bitmask(controller_button::x);
							}
							if((controller_buttons_val & to_bitmask(controller_button::y)) != 0) {
								minui_root->on_key_up(0x2D, 0x58);
								controller_buttons_val &= ~to_bitmask(controller_button::y);
							}
							if((controller_buttons_val & to_bitmask(controller_button::a)) != 0) {
								minui_root->on_key_up(0x2E, 0x43);
								controller_buttons_val &= ~to_bitmask(controller_button::a);
							}
							if((controller_buttons_val & to_bitmask(controller_button::b)) != 0) {
								minui_root->on_key_up(0x2F, 0x56);
								controller_buttons_val &= ~to_bitmask(controller_button::b);
							}
						}
					}

					if((buttons_pressed & to_bitmask(controller_button::dup)) != 0) {
						if(sticky_modifier_keys) {
							if(shift_down) 
								minui_root->on_key_up(0x2A, VK_SHIFT);
							else
								minui_root->on_key_down(0x2A, VK_SHIFT, false);
							shift_down = !shift_down;
						} else {
							minui_root->on_key_down(0x2A, VK_SHIFT, false);
						}
					}
					if((buttons_released & to_bitmask(controller_button::dup)) != 0) {
						if(!sticky_modifier_keys) {
							minui_root->on_key_up(0x2A, VK_SHIFT);
						}
					}
					if((buttons_pressed & to_bitmask(controller_button::ddown)) != 0) {
						if(sticky_modifier_keys) {
							if(alt_down)
								minui_root->on_key_up(0x00E2, VK_MENU);
							else
								minui_root->on_key_down(0x00E2, VK_MENU, false);
							alt_down = !alt_down;
						} else {
							minui_root->on_key_down(0x00E2, VK_MENU, false);
						}
					}
					if((buttons_released & to_bitmask(controller_button::ddown)) != 0) {
						if(!sticky_modifier_keys) {
							minui_root->on_key_up(0x00E2, VK_MENU);
						}
					}

					if((buttons_pressed & to_bitmask(controller_button::x)) != 0) {
						if((controller_buttons_val & to_bitmask(controller_button::lb)) != 0) {
							minui_root->on_key_down(0x10, 0x51, false);
						} else if((controller_buttons_val & to_bitmask(controller_button::rb)) != 0) {
							minui_root->on_key_down(0x2C, 0x5A, false);
						} else {
							minui_root->on_key_down(0x1E, 0x41, false);
						}
					}
					if((buttons_released & to_bitmask(controller_button::x)) != 0) {
						minui_root->on_key_up(0x10, 0x51);
						minui_root->on_key_up(0x2C, 0x5A);
						minui_root->on_key_up(0x1E, 0x41);
					}
					if((buttons_pressed & to_bitmask(controller_button::y)) != 0) {
						if((controller_buttons_val & to_bitmask(controller_button::lb)) != 0) {
							minui_root->on_key_down(0x11, 0x57, false);
						} else if((controller_buttons_val & to_bitmask(controller_button::rb)) != 0) {
							minui_root->on_key_down(0x2D, 0x58, false);
						} else {
							minui_root->on_key_down(0x1F, 0x53, false);
						}
					}
					if((buttons_released & to_bitmask(controller_button::y)) != 0) {
						minui_root->on_key_up(0x11, 0x57);
						minui_root->on_key_up(0x2D, 0x58);
						minui_root->on_key_up(0x1F, 0x53);
					}
					if((buttons_pressed & to_bitmask(controller_button::a)) != 0) {
						if((controller_buttons_val & to_bitmask(controller_button::lb)) != 0) {
							minui_root->on_key_down(0x12, 0x45, false);
						} else if((controller_buttons_val & to_bitmask(controller_button::rb)) != 0) {
							minui_root->on_key_down(0x2E, 0x43, false);
						} else {
							minui_root->on_key_down(0x20, 0x44, false);
						}
					}
					if((buttons_released & to_bitmask(controller_button::a)) != 0) {
						minui_root->on_key_up(0x12, 0x45);
						minui_root->on_key_up(0x2E, 0x43);
						minui_root->on_key_up(0x20, 0x44);
					}
					if((buttons_pressed & to_bitmask(controller_button::b)) != 0) {
						if((controller_buttons_val & to_bitmask(controller_button::lb)) != 0) {
							minui_root->on_key_down(0x13, 0x52, false);
						} else if((controller_buttons_val & to_bitmask(controller_button::rb)) != 0) {
							minui_root->on_key_down(0x2F, 0x56, false);
						} else {
							minui_root->on_key_down(0x21, 0x46, false);
						}
					}
					if((buttons_released & to_bitmask(controller_button::b)) != 0) {
						minui_root->on_key_up(0x13, 0x52);
						minui_root->on_key_up(0x2F, 0x56);
						minui_root->on_key_up(0x21, 0x46);
					}

					return; // only process one controller
				} // end if some button was pressed
			} else { // else -- controller could not be read, stop trying it
				controller_number_plugged_in &= ~(1 << i);
			}
		}
	} // end loop over xinput controllers
}
void win_d2d_dw_ds::on_device_change(WPARAM, HANDLE) {
	auto old_number = controller_number_plugged_in;
	controller_number_plugged_in = 0;

	for(uint32_t i = 0; i < 4; ++i) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if(XInputGetState(i, &state) == ERROR_SUCCESS) {
			controller_number_plugged_in |= (1 << i);
		}
	}
	if(controller_number_plugged_in != old_number && minui_root) {
		minui_root->on_update();
	}
}

uint32_t map_scancode(uint32_t in, keyboard_type t) {
	if(t == keyboard_type::left_hand)
		return in;
	if(t == keyboard_type::left_hand) {
		switch(in) {
			case 0x16:
				return 0x10;
			case 0x17:
				return 0x11;
			case 0x18:
				return 0x12;
			case 0x19:
				return 0x13;
			case 0x24:
				return 0x1E;
			case 0x25:
				return 0x1F;
			case 0x26:
				return 0x20;
			case 0x27:
				return 0x21;
			case 0x32:
				return 0x2C;
			case 0x33:
				return 0x2D;
			case 0x34:
				return 0x2E;
			case 0x35:
				return 0x2F;
		}
		return in;
	}
	if(t == keyboard_type::right_hand_tilted) {
		switch(in) {
			case 0x17:
				return 0x10;
			case 0x18:
				return 0x11;
			case 0x19:
				return 0x12;
			case 0x1A:
				return 0x13;
			case 0x24:
				return 0x1E;
			case 0x25:
				return 0x1F;
			case 0x26:
				return 0x20;
			case 0x27:
				return 0x21;
			case 0x31:
				return 0x2C;
			case 0x32:
				return 0x2D;
			case 0x33:
				return 0x2E;
			case 0x34:
				return 0x2F;
		}
		return in;
	}
	return in;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if(message == WM_CREATE) {
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		win_d2d_dw_ds* app = (win_d2d_dw_ds*)pcs->lpCreateParams;

		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));

		return 1;
	} else {
		win_d2d_dw_ds* app = reinterpret_cast<win_d2d_dw_ds*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));

		if(app) {
			switch(message) {
				case WM_DPICHANGED:
				{
					app->on_dpi_change( float(HIWORD(wParam)) );
					auto lprcNewScale = reinterpret_cast<RECT*>(lParam);

					SetWindowPos(hwnd, nullptr, lprcNewScale->left, lprcNewScale->top,
						lprcNewScale->right - lprcNewScale->left, lprcNewScale->bottom - lprcNewScale->top,
						SWP_NOZORDER | SWP_NOACTIVATE);
					break;
				}
				case WM_SETCURSOR:
				{
					if(LOWORD(lParam) == HTCLIENT) {
						app->reshow_cursor();
						return TRUE;
					}
					break;
				}
				case WM_NCMOUSEMOVE:
				{
					RECT rcWindow;
					GetWindowRect(hwnd, &rcWindow);
					auto x = GET_X_LPARAM(lParam);
					auto y = GET_Y_LPARAM(lParam);

					POINTS adj{ SHORT(x - rcWindow.left), SHORT(y - rcWindow.top) };
					memcpy(&lParam, &adj, sizeof(LPARAM));
				} // fallthrough; keep above WM_MOUSEMOVE
				case WM_MOUSEMOVE:
				{
					if(app->selecting_edit_text != edit_selection_mode::none) {
						auto f = (app->minui_root && !app->minui_root->focus_stack.empty()) ? app->minui_root->focus_stack.back().l_interface : nullptr;
						if(f) {
							auto ei = static_cast<ieditable_text*>(f->get_interface(iface::editable_text));
							if(ei) {
								auto control_pos = app->to_screen_space(f->position);
								control_pos.x += app->window_border_size;
								control_pos.y += app->window_border_size;
								ei->move_cursor_by_screen_pt(*app, screen_space_point{ GET_X_LPARAM(lParam) - control_pos.x, GET_Y_LPARAM(lParam) - control_pos.y }, true);

								if(app->minui_root->under_mouse.type_array[size_t(mouse_interactivity::button)].node == f)
									ei->consume_mouse_event(*app, GET_X_LPARAM(lParam) - control_pos.x, GET_Y_LPARAM(lParam) - control_pos.y, uint32_t(wParam));

								return 0;
							}
						}
					}

					if(app->imode == input_mode::follow_input) {
						if(!app->is_mouse_cursor_visible()) {
							app->show_mouse_cursor();
							if(app->minui_root)
								app->minui_root->set_prompt_mode(prompt_mode::hidden);
						}
					} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard && app->imode != input_mode::controller_with_pointer) {
						 // do nothing
						break;
					}
					if(app->minui_root && app->minui_root->on_mouse_move(layout_position{ app->to_ui_space(float(GET_X_LPARAM(lParam) - app->window_border_size)), app->to_ui_space(float(GET_Y_LPARAM(lParam) - app->window_border_size)) })) {
						return 0;
					}
					break;
				}
				case WM_SYSKEYDOWN:
				case WM_KEYDOWN:
				{
					if(app->minui_root && app->sticky_modifier_keys && (wParam == VK_MENU || wParam == VK_SHIFT)) {
						if((HIWORD(lParam) & KF_REPEAT) != 0)
							return 0;

						if(wParam == VK_MENU) {
							if(app->alt_down)
								app->minui_root->on_key_up(0x00E2, VK_MENU);
							else
								app->minui_root->on_key_down(0x00E2, VK_MENU, false);
							app->alt_down = !app->alt_down;
						} else {
							if(app->shift_down)
								app->minui_root->on_key_up(0x2A, VK_SHIFT);
							else
								app->minui_root->on_key_down(0x2A, VK_SHIFT, false);
							app->shift_down = !app->shift_down;
						}
						return 0;
					}
					if(wParam != VK_MENU && wParam != VK_SHIFT && wParam != VK_ESCAPE) {
						if(app->imode == input_mode::follow_input && app->minui_root) {
							 if(app->minui_root->pmode == prompt_mode::controller) {
								app->minui_root->set_prompt_mode(prompt_mode::keyboard);
								return 0; // eat the key so that we don't do something bad
							} else if(app->is_mouse_cursor_visible()) {
								app->hide_mouse_cursor();
								app->minui_root->set_prompt_mode(prompt_mode::keyboard);
								app->minui_root->set_window_focus(focus_tracker{ nullptr, -1, -1 });
								return 0; // eat the key so that we don't do something bad
							}
						} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard) {
							// do nothing
							break;
						}
					}
					if(app->minui_root && app->minui_root->on_key_down(map_scancode(0xFF & MapVirtualKey(UINT(wParam), MAPVK_VK_TO_VSC), app->kb_type), UINT(wParam), (HIWORD(lParam) & KF_REPEAT) != 0)) {
						return 0;
					}
					break;
				}
				case WM_SYSKEYUP:
				case WM_KEYUP:
				{
					if(wParam != VK_MENU && wParam != VK_SHIFT && wParam != VK_ESCAPE) {
						if(app->imode == input_mode::follow_input && app->minui_root) {
							if(app->minui_root->pmode == prompt_mode::controller) {
								app->minui_root->set_prompt_mode(prompt_mode::keyboard);
								return 0; // eat the key so that we don't do something bad
							} else if(app->is_mouse_cursor_visible()) {
								app->hide_mouse_cursor();
								app->minui_root->set_prompt_mode(prompt_mode::keyboard);
								app->minui_root->set_window_focus(focus_tracker{ nullptr, -1, -1 });
								return 0; // eat the key so that we don't do something bad
							}
						} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard) {
							// do nothing
							break;
						}
					}
					if(app->sticky_modifier_keys && (wParam == VK_MENU || wParam == VK_SHIFT)) {
						return 0;
					}
					if(app->minui_root && app->minui_root->on_key_up(map_scancode(0xFF & MapVirtualKey(UINT(wParam), MAPVK_VK_TO_VSC), app->kb_type), UINT(wParam))) {
						return 0;
					}
					break;
				}
				case WM_CHAR:
				{
					if(app->minui_root && wParam >= 0x20 && app->minui_root->on_char(wchar_t(wParam))) {
						// key consumed
					} else {
						// key not handled
					}
					return 0;
				}
				case WM_RBUTTONDBLCLK:
				case WM_RBUTTONDOWN:
				{
					if(app->imode == input_mode::follow_input) {
						if(!app->is_mouse_cursor_visible() || app->minui_root->pmode == prompt_mode::controller) {
							app->show_mouse_cursor();
							if(app->minui_root)
								app->minui_root->set_prompt_mode(prompt_mode::hidden);
						}
					} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard) {
						// do nothing
						break;
					}
					if(app->minui_root && app->minui_root->on_mouse_rbutton()) {
						auto f = app->minui_root->under_mouse.type_array[size_t(mouse_interactivity::button)].node;
						if(f) {
							auto ei = static_cast<ieditable_text*>(f->get_interface(iface::editable_text));
							if(ei) {
								auto control_pos = app->to_screen_space(f->position);
								control_pos.x += app->window_border_size;
								control_pos.y += app->window_border_size;
								ei->consume_mouse_event(*app, GET_X_LPARAM(lParam) - control_pos.x, GET_Y_LPARAM(lParam) - control_pos.y, uint32_t(wParam));
							}
						}
						return 0;
					}
					break;
				}
				case WM_RBUTTONUP:
				{
					if(app->imode == input_mode::follow_input) {
						if(!app->is_mouse_cursor_visible() || app->minui_root->pmode == prompt_mode::controller) {
							app->show_mouse_cursor();
							if(app->minui_root)
								app->minui_root->set_prompt_mode(prompt_mode::hidden);
						}
					} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard) {
						// do nothing
						break;
					}
					if(app->minui_root && app->minui_root->on_mouse_rbutton_up()) {
						auto f = app->minui_root->under_mouse.type_array[size_t(mouse_interactivity::button)].node;
						if(f) {
							auto ei = static_cast<ieditable_text*>(f->get_interface(iface::editable_text));
							if(ei) {
								auto control_pos = app->to_screen_space(f->position);
								control_pos.x += app->window_border_size;
								control_pos.y += app->window_border_size;
								ei->consume_mouse_event(*app, GET_X_LPARAM(lParam) - control_pos.x, GET_Y_LPARAM(lParam) - control_pos.y, uint32_t(wParam));
							}
						}
						return 0;
					}
					break;
				}
				case WM_LBUTTONDOWN:
				{
					if(app->imode == input_mode::follow_input) {
						if(!app->is_mouse_cursor_visible() || app->minui_root->pmode == prompt_mode::controller) {
							app->show_mouse_cursor();
							if(app->minui_root)
								app->minui_root->set_prompt_mode(prompt_mode::hidden);
						}
					} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard) {
						// do nothing
						break;
					}

					SetCapture(hwnd);

					auto duration = std::chrono::steady_clock::now() - app->last_double_click;
					auto in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

					if(app->minui_root && app->minui_root->on_mouse_lbutton( in_ms.count() <= app->double_click_ms ? click_type::triplec : click_type::singlec)) {
						auto f = app->minui_root->under_mouse.type_array[size_t(mouse_interactivity::button)].node;
						if(f) {
							auto ei = static_cast<ieditable_text*>(f->get_interface(iface::editable_text));
							if(ei) {
								auto control_pos = app->to_screen_space(f->position);
								control_pos.x += app->window_border_size;
								control_pos.y += app->window_border_size;
								ei->consume_mouse_event(*app, GET_X_LPARAM(lParam) - control_pos.x, GET_Y_LPARAM(lParam) - control_pos.y, uint32_t(wParam));

								if(in_ms.count() <= app->double_click_ms)
									app->selecting_edit_text = edit_selection_mode::line;
								else
									app->selecting_edit_text = edit_selection_mode::standard;
							}
						}
						return 0;
					}
				}
				case WM_LBUTTONDBLCLK:
				{
					if(app->imode == input_mode::follow_input) {
						if(!app->is_mouse_cursor_visible() || app->minui_root->pmode == prompt_mode::controller) {
							app->show_mouse_cursor();
							if(app->minui_root)
								app->minui_root->set_prompt_mode(prompt_mode::hidden);
						}
					} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard) {
						// do nothing
						break;
					}

					app->last_double_click = std::chrono::steady_clock::now();
					if(app->minui_root && app->minui_root->on_mouse_lbutton(click_type::doublec)) {
						auto f = app->minui_root->under_mouse.type_array[size_t(mouse_interactivity::button)].node;
						if(f) {
							auto ei = static_cast<ieditable_text*>(f->get_interface(iface::editable_text));
							if(ei) {
								auto control_pos = app->to_screen_space(f->position);
								control_pos.x += app->window_border_size;
								control_pos.y += app->window_border_size;
								ei->consume_mouse_event(*app, GET_X_LPARAM(lParam) - control_pos.x, GET_Y_LPARAM(lParam) - control_pos.y, uint32_t(wParam));
								app->selecting_edit_text = edit_selection_mode::word;
							}
						}
						return 0;
					}
				}
				case WM_LBUTTONUP:
					app->selecting_edit_text = edit_selection_mode::none;

					if(app->imode == input_mode::follow_input) {
						if(!app->is_mouse_cursor_visible() || app->minui_root->pmode == prompt_mode::controller) {
							app->show_mouse_cursor();
							if(app->minui_root)
								app->minui_root->set_prompt_mode(prompt_mode::hidden);
						}
					} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard) {
						// do nothing
						break;
					}

					ReleaseCapture();
					if(app->minui_root && app->minui_root->on_mouse_lbutton_up()) {
						auto f = app->minui_root->under_mouse.type_array[size_t(mouse_interactivity::button)].node;
						if(f) {
							auto ei = static_cast<ieditable_text*>(f->get_interface(iface::editable_text));
							if(ei) {
								auto control_pos = app->to_screen_space(f->position);
								control_pos.x += app->window_border_size;
								control_pos.y += app->window_border_size;
								ei->consume_mouse_event(*app, GET_X_LPARAM(lParam) - control_pos.x, GET_Y_LPARAM(lParam) - control_pos.y, uint32_t(wParam));
							}
						}

						return 0;
					}
					break;
				case WM_MOUSEWHEEL:
					if(app->imode == input_mode::follow_input) {
						if(!app->is_mouse_cursor_visible() || app->minui_root->pmode == prompt_mode::controller) {
							app->show_mouse_cursor();
							if(app->minui_root)
								app->minui_root->set_prompt_mode(prompt_mode::hidden);
						}
					} else if(app->imode != input_mode::mouse_only && app->imode != input_mode::mouse_and_keyboard) {
						// do nothing
						break;
					}

					if(app->minui_root && app->minui_root->on_mouse_scroll(float(GET_WHEEL_DELTA_WPARAM(wParam)) / 120.0f)) {
						return 0;
					} else {
						break;
					}
				case WM_NCCALCSIZE:
					if(app->hide_window_elements && wParam == TRUE)
						return 0;
					break;
				case WM_NCHITTEST:
				{
					if(!app->hide_window_elements)
						break;

					POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
					RECT rcWindow;
					GetWindowRect(hwnd, &rcWindow);

					USHORT uRow = 1;
					USHORT uCol = 1;

					if(ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + 5) {
						uRow = 0;
					} else if(ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - 5) {
						uRow = 2;
					}

					// Determine if the point is at the left or right of the window.
					if(ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + 5) {
						uCol = 0; // left side
					} else if(ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - 5) {
						uCol = 2; // right side
					}

					// Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
					LRESULT hitTests[3][3] =
					{
						{ HTTOPLEFT, HTTOP, HTTOPRIGHT },
						{ HTLEFT, HTNOWHERE, HTRIGHT },
						{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
					};

					if(hitTests[uRow][uCol] == HTNOWHERE) {
						// test for desired caption area and return
						//return HTCAPTION;
					
					} else {
						return hitTests[uRow][uCol];
					}
					break;
				}
				case WM_ENTERSIZEMOVE:
					return 0;
				case WM_EXITSIZEMOVE:
					return 0;
				case WM_SIZE:
				{
					resize_type t = resize_type::normal;

					if(wParam == SIZE_MAXIMIZED) {
						t = resize_type::maximize;
					} else if(wParam == SIZE_MINIMIZED) {
						t = resize_type::minimize;
					} else if(wParam == SIZE_RESTORED) {
						t = resize_type::normal;
					} else {
						break;
					}

					app->client_x = LOWORD(lParam);
					app->client_y = HIWORD(lParam);

					if(app->minui_root) {
						app->minui_root->on_workspace_resized(t, layout_position{ app->to_ui_space(float(LOWORD(lParam))), app->to_ui_space(float(HIWORD(lParam))) });
						return 0;
					}
					break;
				}
				case WM_GETMINMAXINFO:
				{
					if(app->minui_root) {
						RECT temp;
						temp.left = 0;
						temp.right = app->to_screen_space(app->minui_root->minimum_width()) + app->window_border_size;
						temp.top = 0;
						temp.bottom = app->to_screen_space(app->minui_root->minimum_height()) + app->window_border_size;
						AdjustWindowRectExForDpi(&temp, app->window_style, FALSE, app->window_style_ex, UINT(app->dpi));
						MINMAXINFO* pMinMaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
						pMinMaxInfo->ptMinTrackSize.x = static_cast<UINT>(temp.right - temp.left);
						pMinMaxInfo->ptMinTrackSize.y = static_cast<UINT>(temp.bottom - temp.top);
						return 0;
					}
					break;
				}
				case WM_PAINT:
				case WM_DISPLAYCHANGE:
				{
					PAINTSTRUCT ps;
					BeginPaint(hwnd, &ps);
					if(app->minui_root) {
						app->minui_root->render();
					}
					EndPaint(hwnd, &ps);
					return 0;
				}
				case WM_DESTROY:
					//app->accessibility_interface.release_root_provider();
					PostQuitMessage(0);
					return 1;
				/*
				// TODO: accessibility
				case WM_GETOBJECT:
				{

					//if(InSendMessageEx(nullptr) != 0)

					if(static_cast<long>(lParam) == static_cast<long>(UiaRootObjectId)) {
						return UiaReturnRawElementProvider(hwnd, wParam, lParam,
							app->accessibility_interface.get_root_window_provider());
					}
					break;
				}
				*/
				case WM_ACTIVATEAPP:
					if(wParam == TRUE) {
						// being activated
						// XInputEnable(TRUE);
					} else {
						// being deactivated
						// XInputEnable(FALSE);
					}
					return 0;
				case WM_INPUT:
					app->on_controller_input();
					return 0;
				case WM_INPUT_DEVICE_CHANGE:
					app->on_device_change(wParam, (HANDLE)lParam);
					return 0;
				case WM_APPCOMMAND:
				{
					auto cmd = GET_APPCOMMAND_LPARAM(lParam);
					if(cmd == APPCOMMAND_COPY) {
						if(app->minui_root)
							app->minui_root->on_direct_edit_command(ieditable_text::edit_command::copy);
						return TRUE;
					} else if(cmd == APPCOMMAND_CUT) {
						if(app->minui_root)
							app->minui_root->on_direct_edit_command(ieditable_text::edit_command::cut);
						return TRUE;
					} else if(cmd == APPCOMMAND_PASTE) {
						if(app->minui_root)
							app->minui_root->on_direct_edit_command(ieditable_text::edit_command::paste);
						return TRUE;
					} else if(cmd == APPCOMMAND_REDO) {
						if(app->minui_root)
							app->minui_root->on_direct_edit_command(ieditable_text::edit_command::redo);
						return TRUE;
					} else if(cmd == APPCOMMAND_UNDO) {
						if(app->minui_root)
							app->minui_root->on_direct_edit_command(ieditable_text::edit_command::undo);
						return TRUE;
					}
					break;
				}
				case WM_KILLFOCUS:
					app->selecting_edit_text = edit_selection_mode::none;
					if(app->minui_root)
						app->minui_root->on_window_kill_focus();
					return 0;
				case WM_SETFOCUS:
					if(app->minui_root)
						app->minui_root->on_window_focus();
					return 0;
				default:
					break;
			}
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}


/*
using formatting_content = std::variant<std::monostate, variable, variable_attributes, inline_icon_id, substitution_mark, extra_formatting, match_set_reference, match_set_terminal_reference, color, color_end>;

struct format_marker {
	uint16_t position;
	formatting_content format;

	bool operator<(const format_marker& o) const noexcept {
		return position < o.position;
	}
};
*/

void apply_default_vertical_options(IDWriteTypography* t) {
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_GLYPH_COMPOSITION_DECOMPOSITION, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_LOCALIZED_FORMS, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_MARK_POSITIONING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_MARK_TO_MARK_POSITIONING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_REQUIRED_LIGATURES, 1 });

	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_VERTICAL_ALTERNATES_AND_ROTATION, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_VERTICAL_WRITING, 1 });
}
void apply_default_ltr_options(IDWriteTypography* t) {
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_GLYPH_COMPOSITION_DECOMPOSITION, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_LOCALIZED_FORMS, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_MARK_POSITIONING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_MARK_TO_MARK_POSITIONING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_REQUIRED_LIGATURES, 1 });

	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG(DWRITE_MAKE_OPENTYPE_TAG('l', 't', 'r', 'a')), 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG(DWRITE_MAKE_OPENTYPE_TAG('l', 't', 'r', 'm')), 1 });

	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_ALTERNATES, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_LIGATURES, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_CURSIVE_POSITIONING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_KERNING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_STANDARD_LIGATURES, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG(DWRITE_MAKE_OPENTYPE_TAG('r', 'c', 'l', 't')), 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG(DWRITE_MAKE_OPENTYPE_TAG('d', 'i', 's', 't')), 1 });
}
void apply_default_rtl_options(IDWriteTypography* t) {
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_GLYPH_COMPOSITION_DECOMPOSITION, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_LOCALIZED_FORMS, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_MARK_POSITIONING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_MARK_TO_MARK_POSITIONING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_REQUIRED_LIGATURES, 1 });

	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG(DWRITE_MAKE_OPENTYPE_TAG('r', 't', 'l', 'a')), 1 });

	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_ALTERNATES, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_LIGATURES, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_CURSIVE_POSITIONING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_KERNING, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_STANDARD_LIGATURES, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG(DWRITE_MAKE_OPENTYPE_TAG('r', 'c', 'l', 't')), 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG(DWRITE_MAKE_OPENTYPE_TAG('d', 'i', 's', 't')), 1 });
}

void apply_old_style_figures_options(IDWriteTypography* t) {
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_OLD_STYLE_FIGURES, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_FIGURES, 1 });
}
void apply_lining_figures_options(IDWriteTypography* t) {
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_CASE_SENSITIVE_FORMS, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_LINING_FIGURES, 1 });
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_TABULAR_FIGURES, 1 });
}

void apply_small_caps_options(IDWriteTypography* t) {
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_SMALL_CAPITALS, 1 });
}

void apply_superscript_options(IDWriteTypography* t) {
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_SUPERSCRIPT, 1 });
}
void apply_subscript_options(IDWriteTypography* t) {
	t->AddFontFeature(DWRITE_FONT_FEATURE{ DWRITE_FONT_FEATURE_TAG_SUBSCRIPT, 1 });
}

struct twrapper {
	IDWriteTypography* p = nullptr;

	template<typename F>
	twrapper(IDWriteFactory6* dwrite_factory, F&& f) {
		dwrite_factory->CreateTypography(&p);
		f(p);
	}
	~twrapper() {
		safe_release(p);
	}
};

void win_d2d_dw_ds::apply_formatting(IDWriteTextLayout* target, text::format_marker const* formatting, uint32_t format_count) {

	struct open_format {
		uint32_t start_pos = 0;
		text::extra_formatting format;
	};
	
	std::vector<open_format> format_stack;

	bool bold_is_applied = false;
	static twrapper lsmcap(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_ltr_options(t);
		apply_small_caps_options(t);
	});
	static twrapper rsmcap(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_rtl_options(t);
		apply_small_caps_options(t);
	});
	static twrapper lsup(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_ltr_options(t);
		apply_superscript_options(t);
	});
	static twrapper rsup(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_rtl_options(t);
		apply_superscript_options(t);
	});
	static twrapper lsub(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_ltr_options(t);
		apply_subscript_options(t);
	});
	static twrapper rsub(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_rtl_options(t);
		apply_subscript_options(t);
	});
	static twrapper llin(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_ltr_options(t);
		apply_lining_figures_options(t);
	});
	static twrapper rlin(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_rtl_options(t);
		apply_lining_figures_options(t);
	});
	static twrapper lold(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_ltr_options(t);
		apply_old_style_figures_options(t);
	});
	static twrapper rold(dwrite_factory, [](IDWriteTypography* t) {
		apply_default_rtl_options(t);
		apply_old_style_figures_options(t);
	});

	for(uint32_t n = 0; n < format_count; ++n) {
		auto& f = formatting[n];
		if(std::holds_alternative<text::extra_formatting>(f.format)) {
			if(std::get<text::extra_formatting>(f.format) != text::extra_formatting::close_formatting) {
				format_stack.push_back(open_format{ f.position, std::get<text::extra_formatting>(f.format) });
			} else if(!format_stack.empty()) {
				switch(format_stack.back().format) {
					case text::extra_formatting::bold:
						target->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, DWRITE_TEXT_RANGE{ format_stack.back().start_pos, f.position - format_stack.back().start_pos });
						break;
					case text::extra_formatting::italic:
						target->SetFontStyle(DWRITE_FONT_STYLE_ITALIC, DWRITE_TEXT_RANGE{ format_stack.back().start_pos, f.position - format_stack.back().start_pos });
						break;
					case text::extra_formatting::superscript:
						target->SetTypography(target->GetReadingDirection() == DWRITE_READING_DIRECTION_LEFT_TO_RIGHT ? lsup.p : rsup.p, DWRITE_TEXT_RANGE{ format_stack.back().start_pos, f.position - format_stack.back().start_pos });
						break;
					case text::extra_formatting::subscript:
						target->SetTypography(target->GetReadingDirection() == DWRITE_READING_DIRECTION_LEFT_TO_RIGHT ? lsub.p : rsub.p, DWRITE_TEXT_RANGE{ format_stack.back().start_pos, f.position - format_stack.back().start_pos });
						break;
					case text::extra_formatting::small_caps:
						target->SetTypography(target->GetReadingDirection() == DWRITE_READING_DIRECTION_LEFT_TO_RIGHT ? lsmcap.p : rsmcap.p, DWRITE_TEXT_RANGE{ format_stack.back().start_pos, f.position - format_stack.back().start_pos });
						break;
					case text::extra_formatting::old_numbers:
						target->SetTypography(target->GetReadingDirection() == DWRITE_READING_DIRECTION_LEFT_TO_RIGHT ? lold.p : rold.p, DWRITE_TEXT_RANGE{ format_stack.back().start_pos, f.position - format_stack.back().start_pos });
						break;
					case text::extra_formatting::tabular_numbers:
						target->SetTypography(target->GetReadingDirection() == DWRITE_READING_DIRECTION_LEFT_TO_RIGHT ? llin.p : rlin.p, DWRITE_TEXT_RANGE{ format_stack.back().start_pos, f.position - format_stack.back().start_pos });
						break;
					default:
						break;
				}
				format_stack.pop_back();
			}
		}
	}
}

uint32_t content_alignment_to_text_alignment(text::content_alignment align) {
	switch(align) {
		case text::content_alignment::leading:
			return DWRITE_TEXT_ALIGNMENT_LEADING;
		case text::content_alignment::trailing:
			return DWRITE_TEXT_ALIGNMENT_TRAILING;
		case text::content_alignment::centered:
			return DWRITE_TEXT_ALIGNMENT_CENTER;
		case text::content_alignment::justified:
			return DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
	}
	return DWRITE_TEXT_ALIGNMENT_LEADING;
}

arrangement_result win_d2d_dw_ds::create_text_arragement(text::formatted_text_reference text, text::content_alignment text_alignment, text::font_handle font, bool single_line, int32_t max_width) {

	IDWriteTextLayout* formatted_text = nullptr;
	arrangement_result result;

	auto text_format = font_collection[font.id].format;

	text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	text_format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	text_format->SetReadingDirection(left_to_right ? DWRITE_READING_DIRECTION_LEFT_TO_RIGHT : DWRITE_READING_DIRECTION_RIGHT_TO_LEFT);
	text_format->SetFlowDirection(DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM);
	text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	text_format->SetOpticalAlignment(DWRITE_OPTICAL_ALIGNMENT_NO_SIDE_BEARINGS);
	text_format->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, font_collection[font.id].info.line_spacing, font_collection[font.id].info.baseline);
	

	dwrite_factory->CreateTextLayout(text.text_content, text.text_length, text_format, max_width, font_collection[font.id].info.line_spacing, &formatted_text);

	if(font_collection[font.id].info.is_oblique)
		formatted_text->SetFontStyle(DWRITE_FONT_STYLE_ITALIC, DWRITE_TEXT_RANGE{ 0, text.text_length });
	
	apply_formatting(formatted_text, text.formatting_content, text.formatting_length);

	DWRITE_TEXT_METRICS text_metrics;
	formatted_text->GetMetrics(&text_metrics);


	if(single_line || text_metrics.width <= max_width) {
		formatted_text->SetMaxWidth(max_width);
		formatted_text->SetMaxHeight(float(font_collection[font.id].info.line_spacing));
		formatted_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(content_alignment_to_text_alignment(text_alignment)));
		formatted_text->SetWordWrapping(DWRITE_WORD_WRAPPING_EMERGENCY_BREAK);
		result.lines_used = 1;
	} else {
		formatted_text->SetMaxWidth(float(max_width));
		formatted_text->SetMaxHeight(float(1000 * font_collection[font.id].info.line_spacing));
		formatted_text->SetWordWrapping(DWRITE_WORD_WRAPPING_EMERGENCY_BREAK);

		formatted_text->GetMetrics(&text_metrics);
		result.lines_used = text_metrics.lineCount;
		formatted_text->SetMaxHeight(float(text_metrics.lineCount * font_collection[font.id].info.line_spacing));
		formatted_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(content_alignment_to_text_alignment(text_alignment)));
	}

	result.ptr = formatted_text;
	return result;
}

void dw_static_text_provider::set_text(system_interface&, text::formatted_text&& t) {
	if(internal_text.text_content != t.text_content) {
		internal_text = std::move(t);
		requires_update = true;
	}
}
void dw_static_text_provider::set_alignment(system_interface&, text::content_alignment a) {
	alignment = a;
	requires_update = true;
}
void dw_static_text_provider::set_font(system_interface&, text::font_handle f) {
	font = f;
	requires_update = true;
}
void dw_static_text_provider::set_is_multiline(system_interface&, bool m) {
	multiline = m;
	requires_update = true;
}
int32_t dw_static_text_provider::get_number_of_displayed_lines(system_interface& s) {
	return displayed_y_size / int32_t(static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing);
}
int32_t dw_static_text_provider::get_number_of_text_lines(system_interface&) {
	return lines_used;
}
int32_t dw_static_text_provider::get_starting_display_line() {
	return starting_line;
}
void dw_static_text_provider::set_starting_display_line(int32_t v) {
	starting_line = int16_t(v);
	requires_update = true;
}
void dw_static_text_provider::render(system_interface& s, layout_rect r, uint16_t brush, rendering_modifiers display_flags, bool in_focus) {
	auto node_size = s.to_screen_space(r);
	if(!cached_text || (node_size.width + 10) != displayed_x_size || node_size.height != displayed_y_size) {
		// resize image_target
		safe_release(cached_text);

		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->CreateBitmap(
			D2D1_SIZE_U{ uint32_t(node_size.width + 10), uint32_t(node_size.height) },
			nullptr,
			0,
			D2D1_BITMAP_PROPERTIES1{
				D2D1::PixelFormat(
					DXGI_FORMAT_A8_UNORM,
					D2D1_ALPHA_MODE_PREMULTIPLIED),
				static_cast<win_d2d_dw_ds&>(s).dpi, static_cast<win_d2d_dw_ds&>(s).dpi,
				D2D1_BITMAP_OPTIONS_TARGET,
				nullptr },
			&cached_text);

		requires_update = true;
		displayed_x_size = node_size.width + 10;
		displayed_y_size = node_size.height;
	}
	if(language_generation != static_cast<win_d2d_dw_ds&>(s).language_generation) {
		attached.on_reload(*(static_cast<win_d2d_dw_ds&>(s).minui_root));
		language_generation = static_cast<win_d2d_dw_ds&>(s).language_generation;
		font_generation = static_cast<win_d2d_dw_ds&>(s).font_generation;
		requires_update = true;
	} else if(font_generation != static_cast<win_d2d_dw_ds&>(s).font_generation) {
		font_generation = static_cast<win_d2d_dw_ds&>(s).font_generation;
		requires_update = true;
	}

	if(requires_update) {
		auto res = static_cast<win_d2d_dw_ds&>(s).create_text_arragement(internal_text, alignment, font, !multiline, node_size.width);
		safe_release(layout);
		layout = res.ptr;
		lines_used = int16_t(res.lines_used);


		ID2D1Image* old_target = nullptr;
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->GetTarget(&old_target);

		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTarget(cached_text);
		//static_cast<win_d2d_dw_ds&>(s).d2d_device_context->BeginDraw();

		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTransform(D2D1::Matrix3x2F::Identity());
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
		if(static_cast<win_d2d_dw_ds&>(s).rendering_params)
			static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTextRenderingParams(static_cast<win_d2d_dw_ds&>(s).rendering_params);
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->DrawTextLayout(D2D1_POINT_2F{ float(5), -static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing * this->starting_line }, layout, static_cast<win_d2d_dw_ds&>(s).solid_brush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		//static_cast<win_d2d_dw_ds&>(s).d2d_device_context->EndDraw();
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTarget(old_target);
		safe_release(old_target);
	}

	requires_update = false;

	// blit cached text to screen
	// TODO: text color

	auto bsize = static_cast<win_d2d_dw_ds&>(s).window_border_size;
	auto base_x = static_cast<win_d2d_dw_ds&>(s).left_to_right ? float(node_size.x + bsize) : float(static_cast<win_d2d_dw_ds&>(s).client_x - (bsize + node_size.width + node_size.x));

	D2D1_RECT_F dest_rect{
		base_x - 5.0f,
		float(node_size.y + bsize),
		base_x + float(node_size.width) + 10.0f,
		float(node_size.y + node_size.height) };
	D2D1_RECT_F source_rect{
		float(0), float(0),
		float(node_size.width + 10), float(node_size.height) };

	static_cast<win_d2d_dw_ds&>(s).d2d_device_context->FillOpacityMask(cached_text, static_cast<win_d2d_dw_ds&>(s).get_brush(brush, display_flags), D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL, dest_rect, source_rect);
}

screen_space_rect dw_static_text_provider::get_character_bounds(system_interface& s, uint32_t position) const {
	if(!layout)
		return screen_space_rect{ 0, 0, 0, 0 };

	DWRITE_HIT_TEST_METRICS r;
	memset(&r, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	float xout = 0;
	float yout = 0;
	layout->HitTestTextPosition(position, FALSE, &xout, &yout, &r);
	
	auto line_offset = static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing * starting_line;
	return screen_space_rect{ int32_t(r.left), int32_t(r.top - line_offset), int32_t(r.width), int32_t(r.height) };
	
}
istatic_text::mouse_test_result dw_static_text_provider::get_position(system_interface& s, screen_space_point pt) {
	if(!layout)
		return istatic_text::mouse_test_result{ 0, 0 };

	auto adj_x = pt.x ;
	auto adj_y = pt.y - static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing * starting_line;


	BOOL is_trailing = FALSE;
	BOOL is_inside = FALSE;
	DWRITE_HIT_TEST_METRICS ht;
	memset(&ht, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	layout->HitTestPoint(float(adj_x), float(adj_y), &is_trailing, &is_inside, &ht);

	//return hit_test_metrics{ text_metrics{ r.textPosition, r.length, r.left, r.top, r.width, r.height, r.bidiLevel },
	//	is_inside == TRUE, is_trailing == TRUE };

	return istatic_text::mouse_test_result{ ht.textPosition, ht.length, is_inside == TRUE, is_trailing == TRUE };
}


void analysis_object::update_analyzed_text(IDWriteTextLayout* formatted_text, std::wstring_view str, bool ltr, win_d2d_dw_ds& tm) {

	if(formatted_text) {
		std::vector< DWRITE_LINE_METRICS> lines;
		uint32_t number_of_lines = 0;
		formatted_text->GetLineMetrics(nullptr, 0, &number_of_lines);
		lines.resize(number_of_lines);
		memset(lines.data(), 0, sizeof(DWRITE_LINE_METRICS) * number_of_lines);
		formatted_text->GetLineMetrics(lines.data(), number_of_lines, &number_of_lines);

		line_breaks.resize(lines.size());
		uint32_t running_total = 0;
		for(uint32_t i = 0; i < lines.size(); ++i) {
			running_total += lines[i].length;
			line_breaks[i] = uint16_t(running_total);
		}
	} else {
		line_breaks.clear();
	}

	int32_t items_got = 0;
	std::vector<SCRIPT_ITEM> processed_items(8);
	int32_t current_size = 8;

	SCRIPT_CONTROL control;
	memset(&control, 0, sizeof(SCRIPT_CONTROL));
	SCRIPT_STATE state;
	memset(&state, 0, sizeof(SCRIPT_STATE));

	control.uDefaultLanguage = LANGIDFROMLCID(tm.get_lcid());
	state.uBidiLevel = ltr ? 0 : 1;
	state.fArabicNumContext = (tm.get_app_locale().starts_with(L"ar-") || tm.get_app_locale() == L"ar") ? 1 : 0;

	while(ScriptItemize(
		str.data(),
		int32_t(str.length()),
		current_size - 1,
		&control,
		&state,
		processed_items.data(),
		&items_got) == E_OUTOFMEMORY) {
		current_size *= 2;
		processed_items.resize(current_size);
	}

	char_attributes.resize(str.length());
	memset(char_attributes.data(), 0, sizeof(SCRIPT_LOGATTR) * str.length());
	for(int32_t i = 0; i < items_got; ++i) {
		auto char_count = processed_items[i + 1].iCharPos - processed_items[i].iCharPos;
		ScriptBreak(str.data() + processed_items[i].iCharPos,
			char_count,
			&(processed_items[i].a),
			char_attributes.data() + processed_items[i].iCharPos);
		for(int32_t j = processed_items[i].iCharPos; j < processed_items[i + 1].iCharPos; ++j) {
			char_attributes[j].fReserved = (processed_items[i].a.s.uBidiLevel & 0x01);
		}
	}

	bool in_numeric_run = false;
	for(uint32_t i = 0; i < str.length(); ++i) {
		if(str[i] >= 0xD800 && str[i] <= 0xDBFF) { // high surrogate
			if(i + 1 < str.length()) {
				auto code_point = text::assemble_codepoint(str[i], str[i + 1]);
				if(text::codepoint32_is_numeric(code_point)) {
					if(!in_numeric_run) {
						char_attributes[i].fWordStop = 1;
						in_numeric_run = true;
					}
				} else {
					if(in_numeric_run) {
						char_attributes[i].fWordStop = 1;
						in_numeric_run = false;
					}
				}
			}
		} else if(str[i] >= 0xDC00 && str[i] <= 0xDFFF) { // low surrogate
			// ignore
		} else {
			if(text::codepoint_is_space(str[i])) {
				//ignore 
			} else if(text::codepoint16_is_numeric(str[i])) {
				if(!in_numeric_run) {
					char_attributes[i].fWordStop = 1;
					in_numeric_run = true;
				}
			} else {
				if(in_numeric_run) {
					char_attributes[i].fWordStop = 1;
					in_numeric_run = false;
				}
			}
		}
	}
}
int32_t analysis_object::number_of_lines() {
	return int32_t(line_breaks.size());
}
int32_t analysis_object::line_of_position(int32_t position) {
	for(uint32_t j = 0; j < line_breaks.size(); ++j) {
		if(uint16_t(position) < line_breaks[j]) {
			return int32_t(j);
		}
	}
	return std::max(int32_t(line_breaks.size()) - 1, 0);
}
int32_t analysis_object::start_of_line(int32_t line) {
	if(line <= 0)
		return 0;
	else if(uint32_t(line) >= line_breaks.size())
		return int32_t(line_breaks.back());
	else
		return int32_t(line_breaks[line - 1]);
}
int32_t analysis_object::end_of_line(int32_t line) {
	if(line < 0)
		return 0;
	else if(uint32_t(line) < line_breaks.size())
		return int32_t(line_breaks[line]);
	else
		return int32_t(line_breaks.back());
}
int32_t analysis_object::left_visual_cursor_position(int32_t position, std::wstring_view str, bool ltr, win_d2d_dw_ds& tm) {
	auto is_ltr = position_is_ltr(position);
	auto in_line = line_of_position(position);
	auto default_pos = is_ltr ? get_previous_cursor_position(position) : get_next_cursor_position(position);

	auto line_begin = start_of_line(in_line);
	auto line_end = end_of_line(in_line);
	if(is_ltr && position == line_begin) {
		return get_previous_cursor_position(end_of_line(in_line - 1));
	}
	if(!is_ltr && default_pos == line_end) {
		return start_of_line(in_line + 1);
	}

	auto default_in_line = line_of_position(default_pos);
	auto default_ltr = position_is_ltr(default_pos);

	if(default_ltr == is_ltr && in_line == default_in_line)
		return default_pos;

	int32_t items_got = 0;
	std::vector<SCRIPT_ITEM> processed_items(8);
	int32_t current_size = 8;

	SCRIPT_CONTROL control;
	memset(&control, 0, sizeof(SCRIPT_CONTROL));
	SCRIPT_STATE state;
	memset(&state, 0, sizeof(SCRIPT_STATE));

	control.uDefaultLanguage = LANGIDFROMLCID(tm.get_lcid());
	state.uBidiLevel = ltr ? 0 : 1;
	state.fArabicNumContext = (tm.get_app_locale().starts_with(L"ar-") || tm.get_app_locale() == L"ar") ? 1 : 0;

	while(ScriptItemize(
		str.data() + line_begin,
		line_end - line_begin,
		current_size - 1,
		&control,
		&state,
		processed_items.data(),
		&items_got) == E_OUTOFMEMORY) {
		current_size *= 2;
		processed_items.resize(current_size);
	}
	int32_t run_position = 0;
	std::vector<BYTE> run_embedding_levels(items_got);
	std::vector<int32_t> visual_to_logical(items_got);
	std::vector<int32_t> logical_to_visual(items_got);

	for(int32_t i = 0; i < items_got; ++i) {
		if(processed_items[i].iCharPos <= (position - line_begin) && (position - line_begin) < processed_items[i + 1].iCharPos) {
			run_position = i;
		}
		run_embedding_levels[i] = processed_items[i].a.s.uBidiLevel;
	}

	ScriptLayout(items_got, run_embedding_levels.data(), visual_to_logical.data(), logical_to_visual.data());
	auto visual_position_of_run = logical_to_visual[run_position];
	if(visual_position_of_run == 0) {
		if(is_ltr) {
			return get_previous_cursor_position(end_of_line(in_line - 1));
		} else {
			return start_of_line(in_line + 1);
		}
	}
	auto logical_position_of_left_run = visual_to_logical[visual_position_of_run - 1];
	auto next_run_is_ltr = (processed_items[logical_position_of_left_run].a.s.uBidiLevel & 0x01) == 0;
	if(next_run_is_ltr) {
		// find rightmost char position by moving back from the run after it
		return get_previous_cursor_position(line_begin + processed_items[logical_position_of_left_run + 1].iCharPos);
	} else {
		// rightmost char position is first char
		return line_begin + processed_items[logical_position_of_left_run].iCharPos;
	}
}
int32_t analysis_object::right_visual_cursor_position(int32_t position, std::wstring_view str, bool ltr, win_d2d_dw_ds& tm) {
	auto is_ltr = position_is_ltr(position);
	auto in_line = line_of_position(position);
	auto default_pos = is_ltr ? get_next_cursor_position(position) : get_previous_cursor_position(position);

	auto line_begin = start_of_line(in_line);
	auto line_end = end_of_line(in_line);
	if(is_ltr && default_pos == line_end) {
		return start_of_line(in_line + 1);
	}
	if(!is_ltr && position == line_begin) {
		return get_previous_cursor_position(end_of_line(in_line - 1));
	}

	auto default_in_line = line_of_position(default_pos);
	auto default_ltr = position_is_ltr(default_pos);

	if(default_ltr == is_ltr && default_in_line == in_line)
		return default_pos;

	int32_t items_got = 0;
	std::vector<SCRIPT_ITEM> processed_items(8);
	int32_t current_size = 8;

	SCRIPT_CONTROL control;
	memset(&control, 0, sizeof(SCRIPT_CONTROL));
	SCRIPT_STATE state;
	memset(&state, 0, sizeof(SCRIPT_STATE));

	control.uDefaultLanguage = LANGIDFROMLCID(tm.get_lcid());
	state.uBidiLevel = ltr ? 0 : 1;
	state.fArabicNumContext = (tm.get_app_locale().starts_with(L"ar-") || tm.get_app_locale() == L"ar") ? 1 : 0;

	while(ScriptItemize(
		str.data() + line_begin,
		line_end - line_begin,
		current_size - 1,
		&control,
		&state,
		processed_items.data(),
		&items_got) == E_OUTOFMEMORY) {
		current_size *= 2;
		processed_items.resize(current_size);
	}
	int32_t run_position = 0;
	std::vector<BYTE> run_embedding_levels(items_got);
	std::vector<int32_t> visual_to_logical(items_got);
	std::vector<int32_t> logical_to_visual(items_got);

	for(int32_t i = 0; i < items_got; ++i) {
		if(processed_items[i].iCharPos <= (position - line_begin) && (position - line_begin) < processed_items[i + 1].iCharPos) {
			run_position = i;
		}
		run_embedding_levels[i] = processed_items[i].a.s.uBidiLevel;
	}

	ScriptLayout(items_got, run_embedding_levels.data(), visual_to_logical.data(), logical_to_visual.data());
	auto visual_position_of_run = logical_to_visual[run_position];
	if(visual_position_of_run == items_got - 1) { // is already rightmost
		if(is_ltr) {
			return start_of_line(in_line + 1);
		} else {
			return get_previous_cursor_position(end_of_line(in_line - 1));
		}
	}
	auto logical_position_of_left_run = visual_to_logical[visual_position_of_run + 1];
	auto next_run_is_ltr = (processed_items[logical_position_of_left_run].a.s.uBidiLevel & 0x01) == 0;
	if(next_run_is_ltr) {
		// leftmost char position is first char
		return processed_items[logical_position_of_left_run].iCharPos + line_begin;
	} else {
		// find leftmost char position by moving back from the run after it
		return get_previous_cursor_position(processed_items[logical_position_of_left_run + 1].iCharPos + line_begin);
	}
}
int32_t analysis_object::number_of_cursor_positions_in_range(int32_t start, int32_t count) {
	int32_t total = 0;
	auto const array_size = char_attributes.size();
	for(int32_t i = 0; i < count && start + i < array_size; ++i) {
		if(char_attributes[start + i].fCharStop)
			++total;
	}
	return total;
}
int32_t analysis_object::get_previous_cursor_position(int32_t position) {
	--position;
	if(position < char_attributes.size()) {
		for(; position > 0; --position) {
			if(char_attributes[position].fCharStop) {
				return position;
			}
		}
	}
	return 0;
}
int32_t analysis_object::get_next_cursor_position(int32_t position) {
	++position;
	auto const array_size = char_attributes.size();
	for(; position < array_size; ++position) {
		if(char_attributes[position].fCharStop) {
			return position;
		}
	}
	return int32_t(array_size);
}
int32_t analysis_object::left_visual_word_position(int32_t position) {
	if(position_is_ltr(position)) {
		return get_previous_word_position(position);
	} else {
		return get_next_word_position(position);
	}
}
int32_t analysis_object::right_visual_word_position(int32_t position) {
	if(position_is_ltr(position)) {
		return get_next_word_position(position);
	} else {
		return get_previous_word_position(position);
	}
}
int32_t analysis_object::get_previous_word_position(int32_t position) {
	--position;
	if(position < char_attributes.size()) {
		for(; position > 0; --position) {
			if(char_attributes[position].fWordStop || char_attributes[position].fSoftBreak) {
				return position;
			}
		}
	}
	return 0;
}
int32_t analysis_object::get_next_word_position(int32_t position) {
	++position;
	auto const array_size = char_attributes.size();
	for(; position < array_size; ++position) {
		if(char_attributes[position].fWordStop || char_attributes[position].fSoftBreak) {
			return position;
		}
	}
	return int32_t(array_size);
}
bool analysis_object::position_is_ltr(int32_t position) {
	if(position < char_attributes.size()) {
		return  char_attributes[position].fReserved == 0;
	} else if(char_attributes.size() > 0) {
		return char_attributes[char_attributes.size() - 1].fReserved == 0;
	} else {
		return false;
	}
}
bool analysis_object::is_cursor_position(int32_t position) {
	bool result = false;
	if(position < char_attributes.size()) {
		result = char_attributes[position].fCharStop != 0;
	}
	return result;
}
bool analysis_object::is_word_position(int32_t position) {
	bool result = false;
	if(position < char_attributes.size()) {
		result = char_attributes[position].fWordStop != 0;
	}
	return result;
}


void dw_editable_text_provider::set_text(system_interface&, text::formatted_text&& t) {
	if(internal_text != t.text_content) {
		internal_text = std::move(t.text_content);
		requires_update = true;
	}
}
void dw_editable_text_provider::set_alignment(system_interface&, text::content_alignment a) {
	alignment = a;
	requires_update = true;
}
void dw_editable_text_provider::set_font(system_interface&, text::font_handle f) {
	font = f;
	requires_update = true;
}
void dw_editable_text_provider::set_is_multiline(system_interface&, bool m) {
	multiline = m;
	requires_update = true;
}
int32_t dw_editable_text_provider::get_number_of_displayed_lines(system_interface& s) {
	return std::max(1, displayed_y_size / int32_t(static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing));
}
int32_t dw_editable_text_provider::get_number_of_text_lines(system_interface&) {
	return lines_used;
}
int32_t dw_editable_text_provider::get_starting_display_line() {
	return starting_line;
}
void dw_editable_text_provider::set_starting_display_line(int32_t v) {
	starting_line = int16_t(v);
	requires_update = true;
}

struct text_metrics {
	uint32_t textPosition;
	uint32_t length;
	float left;
	float top;
	float width;
	float height;
	uint32_t bidiLevel;
};

std::vector<text_metrics> get_metrics_for_range(IDWriteTextLayout* txt, uint32_t position, uint32_t length) {
	IDWriteTextLayout* formatted_text = (IDWriteTextLayout*)txt;

	uint32_t metrics_size = 0;
	formatted_text->HitTestTextRange(position, length, 0, 0, nullptr, 0, &metrics_size);

	std::vector<DWRITE_HIT_TEST_METRICS> mstorage(metrics_size);
	formatted_text->HitTestTextRange(position, length, 0, 0, mstorage.data(), metrics_size, &metrics_size);

	std::vector<text_metrics> result;
	result.reserve(metrics_size);

	for(auto& r : mstorage) {
		result.push_back(text_metrics{ r.textPosition, r.length, r.left, r.top, r.width, r.height, r.bidiLevel });
	}

	return result;
}
text_metrics get_metrics_at_position(IDWriteTextLayout* txt, uint32_t position) {
	IDWriteTextLayout* formatted_text = (IDWriteTextLayout*)txt;

	DWRITE_HIT_TEST_METRICS r;
	memset(&r, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	float xout = 0;
	float yout = 0;
	formatted_text->HitTestTextPosition(position, FALSE, &xout, &yout, &r);
	return text_metrics{ r.textPosition, r.length, r.left, r.top, r.width, r.height, r.bidiLevel };
}

void make_regions_for_range(std::vector<dw_editable_text_provider::selection_run>& cached_selection_region, IDWriteTextLayout* formatted_text, int32_t selection_start, int32_t selection_end, analysis_object& analysis) {

	bool have_seen_end = false;
	bool have_seen_start = false;


	auto mstorage = get_metrics_for_range(formatted_text, selection_start, selection_end - selection_start);

	for(auto& r : mstorage) {
		bool left_to_right_section = (r.bidiLevel % 2) == 0;
		int32_t selection_start_coord =  int32_t(std::round(r.left));
		int32_t selection_end_coord = int32_t(std::round(r.left + r.width));

		if(int32_t(r.textPosition) < selection_start && selection_start < int32_t(r.textPosition + r.length)) {
			//  selection start is strictly within region
			auto curmetrics = get_metrics_at_position(formatted_text, selection_start);

			int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(curmetrics.textPosition, curmetrics.length), 1);
			int32_t num_positions_after_cursor = analysis.number_of_cursor_positions_in_range(selection_start, (curmetrics.textPosition + curmetrics.length) - selection_start);
			float percentage_through_region = float(num_positions_in_metrics - num_positions_after_cursor) / float(num_positions_in_metrics);

			
			selection_start_coord = int32_t(std::round(curmetrics.left + (left_to_right_section ? percentage_through_region * curmetrics.width : (1.0f - percentage_through_region) * curmetrics.width)));
		}

		if(int32_t(r.textPosition) < selection_end && selection_end < int32_t(r.textPosition + r.length)) {
			//  selection end is strictly within region

			auto curmetrics = get_metrics_at_position(formatted_text, selection_end);

			int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(curmetrics.textPosition, curmetrics.length), 1);
			int32_t num_positions_after_cursor = analysis.number_of_cursor_positions_in_range(selection_end, (curmetrics.textPosition + curmetrics.length) - selection_end);
			float percentage_through_region = float(num_positions_in_metrics - num_positions_after_cursor) / float(num_positions_in_metrics);

			
			selection_end_coord = int32_t(std::round(curmetrics.left + (left_to_right_section ? percentage_through_region * curmetrics.width : (1.0f - percentage_through_region) * curmetrics.width)));
		}

		if(int32_t(r.textPosition) <= selection_end - 1 && selection_end - 1 < int32_t(r.textPosition + r.length))
			have_seen_end = true;
		if(int32_t(r.textPosition) <= selection_start && selection_start < int32_t(r.textPosition + r.length))
			have_seen_start = true;

		cached_selection_region.push_back(dw_editable_text_provider::selection_run{ selection_start_coord, selection_end_coord, analysis.line_of_position(r.textPosition) });
	}
	if(!have_seen_end) { // missing end of the selection, (why isn't it in the hittest region???)

		auto curmetrics = get_metrics_at_position(formatted_text, selection_end - 1);

		bool left_to_right_section = (curmetrics.bidiLevel % 2) == 0;
		int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(curmetrics.textPosition, curmetrics.length), 1);
		int32_t num_positions_after_cursor = analysis.number_of_cursor_positions_in_range(selection_end, (curmetrics.textPosition + curmetrics.length) - selection_end);
		float percentage_through_region = float(num_positions_in_metrics - num_positions_after_cursor) / float(num_positions_in_metrics);

		
		if(left_to_right_section) {
			cached_selection_region.push_back(dw_editable_text_provider::selection_run{
				int32_t(std::round(curmetrics.left)),
				int32_t(std::round(curmetrics.left + percentage_through_region * curmetrics.width)),
				analysis.line_of_position(selection_end - 1) });
		} else {
			cached_selection_region.push_back(dw_editable_text_provider::selection_run{
				int32_t(std::round(curmetrics.left + (1.0f - percentage_through_region) * curmetrics.width)),
				int32_t(std::round(curmetrics.left + curmetrics.width)),
				analysis.line_of_position(selection_end - 1) });
		}
	}

	if(!have_seen_start) { // missing end of the selection, (why isn't it in the hittest region???)
		auto curmetrics = get_metrics_at_position(formatted_text, selection_start);

		bool left_to_right_section = (curmetrics.bidiLevel % 2) == 0;
		int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(curmetrics.textPosition, curmetrics.length), 1);
		int32_t num_positions_after_cursor = analysis.number_of_cursor_positions_in_range(selection_start, (curmetrics.textPosition + curmetrics.length) - selection_start);
		float percentage_through_region = float(num_positions_in_metrics - num_positions_after_cursor) / float(num_positions_in_metrics);

	
		if(left_to_right_section) {
			cached_selection_region.push_back(dw_editable_text_provider::selection_run{
				int32_t(std::round(curmetrics.left)),
				int32_t(std::round(curmetrics.left + percentage_through_region * curmetrics.width)),
				analysis.line_of_position(selection_start) });
		} else {
			cached_selection_region.push_back(dw_editable_text_provider::selection_run{
				int32_t(std::round(curmetrics.left + (1.0f - percentage_through_region) * curmetrics.width)),
				int32_t(std::round(curmetrics.left + curmetrics.width)),
				analysis.line_of_position(selection_start) });
		}
	}
}
void dw_editable_text_provider::update_cached_cursor_position() {
	cached_cursor_postion = 0;
	if(layout) {
		bool is_at_text_end = cursor_position == internal_text.length();
		auto curmetrics = get_metrics_at_position(layout, is_at_text_end ? cursor_position - 1 : cursor_position);

		float coff_pos = 0.0f;
		bool left_to_right_section = (curmetrics.bidiLevel % 2) == 0;

		if(is_at_text_end) {
			coff_pos = curmetrics.left + (left_to_right_section ? curmetrics.width : 0.0f);
		} else {
			int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(curmetrics.textPosition, curmetrics.length), 1);
			if(num_positions_in_metrics == 1 || curmetrics.length == 0) {
				coff_pos = curmetrics.left + (left_to_right_section ? 0.0f : curmetrics.width);
			} else {
				int32_t num_positions_after_cursor = analysis.number_of_cursor_positions_in_range(cursor_position, (curmetrics.textPosition + curmetrics.length) - cursor_position);
				float percentage_through_region = float(num_positions_in_metrics - num_positions_after_cursor) / float(num_positions_in_metrics);

				coff_pos = curmetrics.left + (left_to_right_section ? percentage_through_region * curmetrics.width : (1.0f - percentage_through_region) * curmetrics.width);
			}
		}

		cached_cursor_postion = int32_t(std::round(coff_pos));
	}
}

void dw_editable_text_provider::prepare_selection_regions() {
	if(selection_out_of_date) {
		cached_selection_region.clear();

		if(layout) {
			if(anchor_position != cursor_position) {
				auto selection_start = std::min(anchor_position, cursor_position);
				auto selection_end = std::max(anchor_position, cursor_position);

				make_regions_for_range(cached_selection_region, layout, selection_start, selection_end, analysis);
			}
			if(temp_text_length > 0) {
				auto selection_start = temp_text_position;
				auto selection_end = temp_text_position + temp_text_length;

				make_regions_for_range(cached_selection_region, layout, selection_start, selection_end, analysis);
			}
		}

		update_cached_cursor_position();

		selection_out_of_date = false;
	}
}
void dw_editable_text_provider::render(system_interface& s, layout_rect r, uint16_t brush, rendering_modifiers display_flags, bool in_focus) {
	auto node_size = s.to_screen_space(r);
	if(!cached_text || (node_size.width + 10) != displayed_x_size || node_size.height != displayed_y_size) {
		// resize image_target
		safe_release(cached_text);

		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->CreateBitmap(
			D2D1_SIZE_U{ uint32_t(node_size.width + 10), uint32_t(node_size.height) },
			nullptr,
			0,
			D2D1_BITMAP_PROPERTIES1{
				D2D1::PixelFormat(
					DXGI_FORMAT_A8_UNORM,
					D2D1_ALPHA_MODE_PREMULTIPLIED),
			static_cast<win_d2d_dw_ds&>(s).dpi, static_cast<win_d2d_dw_ds&>(s).dpi,
			D2D1_BITMAP_OPTIONS_TARGET,
			nullptr },
			&cached_text);

		requires_update = true;
		displayed_x_size = node_size.width + 10;
		displayed_y_size = node_size.height;
	}
	if(language_generation != static_cast<win_d2d_dw_ds&>(s).language_generation) {
		attached.on_reload(*(static_cast<win_d2d_dw_ds&>(s).minui_root));
		language_generation = static_cast<win_d2d_dw_ds&>(s).language_generation;
		font_generation = static_cast<win_d2d_dw_ds&>(s).font_generation;
		requires_update = true;
	} else if(font_generation != static_cast<win_d2d_dw_ds&>(s).font_generation) {
		font_generation = static_cast<win_d2d_dw_ds&>(s).font_generation;
		requires_update = true;
	}

	prepare_text(static_cast<win_d2d_dw_ds&>(s), node_size.width);

	if(render_out_of_date) {
		ID2D1Image* old_target = nullptr;
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->GetTarget(&old_target);

		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTarget(cached_text);
		//static_cast<win_d2d_dw_ds&>(s).d2d_device_context->BeginDraw();

		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTransform(D2D1::Matrix3x2F::Identity());
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
		if(static_cast<win_d2d_dw_ds&>(s).rendering_params)
			static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTextRenderingParams(static_cast<win_d2d_dw_ds&>(s).rendering_params);
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->DrawTextLayout(D2D1_POINT_2F{ float(5), -static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing * this->starting_line }, layout, static_cast<win_d2d_dw_ds&>(s).solid_brush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		//static_cast<win_d2d_dw_ds&>(s).d2d_device_context->EndDraw();
		static_cast<win_d2d_dw_ds&>(s).d2d_device_context->SetTarget(old_target);
		safe_release(old_target);

		render_out_of_date = false;
	}

	// blit cached text to screen

	auto bsize = static_cast<win_d2d_dw_ds&>(s).window_border_size;
	auto base_x = static_cast<win_d2d_dw_ds&>(s).left_to_right ? float(node_size.x + bsize) : float(static_cast<win_d2d_dw_ds&>(s).client_x - (bsize + node_size.width + node_size.x));

	D2D1_RECT_F dest_rect{
		base_x - 5.0f,
		float(node_size.y + bsize),
		base_x + float(node_size.width) + 10.0f,
		float(node_size.y + node_size.height) };
	D2D1_RECT_F source_rect{
		float(0), float(0),
		float(node_size.width + 10), float(node_size.height) };

	static_cast<win_d2d_dw_ds&>(s).d2d_device_context->FillOpacityMask(cached_text, static_cast<win_d2d_dw_ds&>(s).get_brush(brush, display_flags), D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL, dest_rect, source_rect);

	if(!read_only) {
		prepare_selection_regions();
		auto line_height = static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing;
		ID2D1Brush* fgb = static_cast<win_d2d_dw_ds&>(s).get_brush(static_cast<win_d2d_dw_ds&>(s).minui_root->get_background_brush(attached.type_id), display_flags);
		ID2D1Brush* bgb = static_cast<win_d2d_dw_ds&>(s).get_brush(static_cast<win_d2d_dw_ds&>(s).minui_root->get_foreground_brush(attached.type_id), display_flags);

		auto num_visible_lines = get_number_of_displayed_lines(s);

		// render selection, if any
		if((anchor_position != cursor_position || temp_text_length != 0) && layout) {
			for(auto& rng : cached_selection_region) {
				if(rng.line >= starting_line && (rng.line - starting_line) < num_visible_lines) {
					D2D_RECT_F rect{
						base_x + rng.start,
						float(bsize + node_size.y + (rng.line - starting_line) * line_height),
						base_x + rng.end,
						float(bsize + node_size.y + (1 + rng.line - starting_line) * line_height) };

					static_cast<win_d2d_dw_ds&>(s).d2d_device_context->FillRectangle(rect, bgb);

					D2D1_RECT_F source_rect{
						 rng.start,
						float(bsize + node_size.y + (rng.line - starting_line) * line_height),
						 rng.end,
						float(bsize + node_size.y + (1 + rng.line - starting_line) * line_height) };

					static_cast<win_d2d_dw_ds&>(s).d2d_device_context->FillOpacityMask(cached_text, fgb, D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL, rect, source_rect);
				}
			}
		}

		if(starting_line > 0) {
			auto height = int32_t(2 * static_cast<win_d2d_dw_ds&>(s).global_size_multiplier * static_cast<win_d2d_dw_ds&>(s).dpi / 96.0);

			D2D_RECT_F lr{
				base_x + node_size.width / 2.0f - line_height,
				float(bsize + node_size.y),
				base_x + node_size.width / 2.0f + line_height,
				float(bsize + node_size.y + height) };

			static_cast<win_d2d_dw_ds&>(s).d2d_device_context->FillRectangle(lr, bgb);
		}
		if(analysis.number_of_lines() > num_visible_lines + starting_line) {
			auto height = int32_t(2 * static_cast<win_d2d_dw_ds&>(s).global_size_multiplier * static_cast<win_d2d_dw_ds&>(s).dpi / 96.0);

			D2D_RECT_F lr{
				base_x + node_size.width / 2.0f - line_height,
				float(bsize + node_size.y + node_size.height),
				base_x + node_size.width / 2.0f + line_height,
				float(bsize + node_size.y + node_size.height - height) };

			static_cast<win_d2d_dw_ds&>(s).d2d_device_context->FillRectangle(lr, bgb);
		}

		// render cursor
		auto cursor_line = analysis.line_of_position(cursor_position);

		if(static_cast<win_d2d_dw_ds&>(s).minui_root->contains_focus(&attached) && layout && cursor_line >= starting_line && (cursor_line - starting_line) < num_visible_lines) {
			auto duration = std::chrono::steady_clock::now() - std::chrono::steady_clock::time_point();
			auto in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

			auto ms_in_cycle = in_ms.count() % static_cast<win_d2d_dw_ds&>(s).caret_blink_ms;
			float in_cycle_length = float(ms_in_cycle) * 2.0f * 3.1415f / float(static_cast<win_d2d_dw_ds&>(s).caret_blink_ms);
			float intensity = static_cast<win_d2d_dw_ds&>(s).caret_blinks ? (cos(in_cycle_length) + 1.0f) * 0.5f : 1.0f;

			ID2D1Brush* b = static_cast<win_d2d_dw_ds&>(s).caret_blinks ? static_cast<win_d2d_dw_ds&>(s).get_brush(brush, display_flags) : static_cast<win_d2d_dw_ds&>(s).get_brush(static_cast<win_d2d_dw_ds&>(s).minui_root->get_highlight_brush(attached.type_id), display_flags);

			b->SetOpacity(intensity);
			
			auto cbase_x = static_cast<win_d2d_dw_ds&>(s).left_to_right ?
				float(node_size.x + bsize)
				: float(static_cast<win_d2d_dw_ds&>(s).client_x - (bsize + node_size.width + node_size.x));

			D2D_RECT_F rect{
				cbase_x,
				float(bsize + node_size.y + (cursor_line - starting_line) * line_height),
				cbase_x + float(int32_t(std::ceil(1.0f * static_cast<win_d2d_dw_ds&>(s).global_size_multiplier * static_cast<win_d2d_dw_ds&>(s).dpi / 96.0f))),
				float(bsize + node_size.y + (1 + cursor_line - starting_line) * line_height) };

			static_cast<win_d2d_dw_ds&>(s).d2d_device_context->FillRectangle(rect, b);
			b->SetOpacity(1.0f);
		}
	}
}

screen_space_rect dw_editable_text_provider::get_character_bounds(system_interface& s, uint32_t position) const {
	if(!layout)
		return screen_space_rect{ 0, 0, 0, 0 };

	DWRITE_HIT_TEST_METRICS r;
	memset(&r, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	float xout = 0;
	float yout = 0;
	layout->HitTestTextPosition(position, FALSE, &xout, &yout, &r);

	auto line_offset = static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing * starting_line;
	return screen_space_rect{ int32_t(r.left), int32_t(r.top - line_offset), int32_t(r.width), int32_t(r.height) };

}
istatic_text::mouse_test_result dw_editable_text_provider::get_position(system_interface& s, screen_space_point pt) {
	if(!layout)
		return istatic_text::mouse_test_result{ 0, 0 };

	auto adj_x = pt.x;
	auto adj_y = pt.y - static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing * starting_line;


	BOOL is_trailing = FALSE;
	BOOL is_inside = FALSE;
	DWRITE_HIT_TEST_METRICS ht;
	memset(&ht, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	layout->HitTestPoint(float(adj_x), float(adj_y), &is_trailing, &is_inside, &ht);

	//return hit_test_metrics{ text_metrics{ r.textPosition, r.length, r.left, r.top, r.width, r.height, r.bidiLevel },
	//	is_inside == TRUE, is_trailing == TRUE };

	return istatic_text::mouse_test_result{ ht.textPosition, ht.length, is_inside == TRUE, is_trailing == TRUE };
}
void dw_editable_text_provider::prepare_text(win_d2d_dw_ds& s, int32_t width) {
	if(requires_update) {
		text::formatted_text temp;
		temp.text_content = internal_text;

		// if the text ends in a space, a zero-width non joiner is placed at the end
		// this "fixes" direct-write's problem of ignoring spaces for alignment
		// UNRESOLVED: there may still be problems if the text is left-aligned and begins
		// with a space. A zero width could be prefixed as well, but we would have to adjust
		// for this in the hit-testing logic as well
		if(temp.text_content.length() > 0 && text::codepoint_is_space(temp.text_content.back()))
			temp.text_content += wchar_t(0x200C);

		auto res = static_cast<win_d2d_dw_ds&>(s).create_text_arragement(temp, alignment, font, !multiline, width);
		safe_release(layout);
		layout = res.ptr;
		lines_used = int16_t(res.lines_used);

		analysis.update_analyzed_text(layout, internal_text, static_cast<win_d2d_dw_ds&>(s).left_to_right, static_cast<win_d2d_dw_ds&>(s));

		requires_update = false;
		render_out_of_date = true;
	}
}
void dw_editable_text_provider::make_line_visible(win_d2d_dw_ds& s, int32_t line) {
	auto visible_lines = get_number_of_displayed_lines(s);
	auto total_lines = analysis.number_of_lines();
	if(total_lines <= visible_lines) {
		if(starting_line != 0) {
			starting_line = 0;
		}
		return;
	}
	if(line < starting_line) {
		starting_line = line;
		return;
	}
	if(line >= starting_line + visible_lines) {
		starting_line = line - visible_lines + 1;
		return;
	}
}
void dw_editable_text_provider::on_text_change(win_d2d_dw_ds& win) {
	selection_out_of_date = true;
	changes_made = true;

	prepare_text(win, win.to_screen_space(attached.position).width);

	/*
	// TODO accessibility notifications
	if(acc_obj && win.is_visible(l_id)) {
		win.accessibility_interface.on_text_content_changed(acc_obj);
		if(edit_type != edit_contents::number)
			win.accessibility_interface.on_text_value_changed(acc_obj);
		else
			win.accessibility_interface.on_text_numeric_value_changed(acc_obj);
	}
	*/

	// TODO: feedback notifications to control behavior?
	// on_text_changed(win, text);

	on_selection_change(win);
}
void dw_editable_text_provider::on_selection_change(win_d2d_dw_ds& s) {
	selection_out_of_date = true;
	
	if(s.minui_root->contains_focus(&attached)) {
		auto wslocation = s.minui_root->workspace_placement(attached);
		auto base_size = s.to_screen_space(attached.position);
		auto screen_pos_base = s.to_screen_space(wslocation);

		screen_pos_base.x += s.window_border_size;
		screen_pos_base.y += s.window_border_size;

		if(!s.left_to_right)
			screen_pos_base.x = s.client_x - (screen_pos_base.x + base_size.width);

		auto cursor_line = analysis.line_of_position(cursor_position);
		make_line_visible(s, cursor_line);

		update_cached_cursor_position();
		SetCaretPos(cached_cursor_postion + screen_pos_base.x, screen_pos_base.y + s.font_collection[font.id].info.line_spacing * (cursor_line - starting_line));

		on_selection_change_ts(s);
	}
}
uint32_t dw_editable_text_provider::position_visually_above(win_d2d_dw_ds& s, uint32_t v) {
	auto width = s.to_screen_space(attached.position.width);
	prepare_text(s, width);

	auto current_line = analysis.line_of_position(int32_t(v));
	if(current_line == 0)
		return 0;

	bool is_at_text_end = v == internal_text.length();

	DWRITE_HIT_TEST_METRICS curmetrics;
	memset(&curmetrics, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	float xout = 0;
	float yout = 0;
	layout->HitTestTextPosition(is_at_text_end ? v - 1 : v, FALSE, &xout, &yout, &curmetrics);

	float coff_pos = 0.0f;
	float voff_pos = (curmetrics.top + curmetrics.height / 2.0f);
	{
		bool left_to_right_section = (curmetrics.bidiLevel % 2) == 0;

		if(is_at_text_end) {
			coff_pos = curmetrics.left + (left_to_right_section ? curmetrics.width : 0.0f);
		} else {
			int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(curmetrics.textPosition, curmetrics.length), 1);
			if(num_positions_in_metrics == 1 || curmetrics.length == 0) {
				coff_pos = curmetrics.left + (left_to_right_section ? 0.0f : curmetrics.width);
			} else {
				int32_t num_positions_after_cursor = analysis.number_of_cursor_positions_in_range(v, (curmetrics.textPosition + curmetrics.length) - v);
				float percentage_through_region = float(num_positions_in_metrics - num_positions_after_cursor) / float(num_positions_in_metrics);

				coff_pos = curmetrics.left + (left_to_right_section ? percentage_through_region * curmetrics.width : (1.0f - percentage_through_region) * curmetrics.width);
			}
		}
	}

	
	voff_pos -= s.font_collection[font.id].info.line_spacing;
	
	BOOL is_trailing = FALSE;
	BOOL is_inside = FALSE;
	DWRITE_HIT_TEST_METRICS ht;
	memset(&ht, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	layout->HitTestPoint(coff_pos, voff_pos, &is_trailing, &is_inside, &ht);

	int32_t new_position = 0;
	if(is_inside == FALSE) {
		if(is_trailing == TRUE) {
			new_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
		} else {
			new_position = int32_t(ht.textPosition);
		}
	} else if(ht.length == 1) {
		if(is_trailing == TRUE) {
			new_position = int32_t(std::min(ht.textPosition + 1, uint32_t(internal_text.length())));
		} else {
			new_position = int32_t(ht.textPosition);
		}
	} else {
		int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(ht.textPosition, ht.length), 1);

		if(num_positions_in_metrics == 1) {
			if(is_trailing == TRUE) {
				new_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
			} else {
				new_position = int32_t(ht.textPosition);
			}
		} else {
			bool left_to_right_section = (ht.bidiLevel % 2) == 0;
			float percentage_in_region = ((coff_pos - ht.left) / ht.width);
			if(!left_to_right_section)
				percentage_in_region = 1.0f - percentage_in_region;
			float section_size = 1.0f / float(num_positions_in_metrics);
			float running_total = section_size / 2.0f;

			new_position = int32_t(ht.textPosition);
			[&]() {
				if(percentage_in_region <= running_total) {
					return;
				}
				new_position = analysis.get_next_cursor_position(new_position);
				for(int32_t i = 0; i < num_positions_in_metrics; ++i) {
					running_total += section_size;
					if(percentage_in_region <= running_total) {
						return;
					}
					new_position = analysis.get_next_cursor_position(new_position);
				}
				new_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
			}();
		}
	}
	return uint32_t(new_position);
}
uint32_t dw_editable_text_provider::position_visually_below(win_d2d_dw_ds& s, uint32_t v) {
	auto width = s.to_screen_space(attached.position.width);
	prepare_text(s, width);

	auto current_line = analysis.line_of_position(int32_t(v));
	if(current_line >= analysis.number_of_lines() - 1)
		return uint32_t(internal_text.length());

	bool is_at_text_end = v == internal_text.length();

	DWRITE_HIT_TEST_METRICS curmetrics;
	memset(&curmetrics, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	float xout = 0;
	float yout = 0;
	layout->HitTestTextPosition(is_at_text_end ? v - 1 : v, FALSE, &xout, &yout, &curmetrics);

	float coff_pos = 0.0f;
	float voff_pos = (curmetrics.top + curmetrics.height / 2.0f);

	{
		bool left_to_right_section = (curmetrics.bidiLevel % 2) == 0;

		if(is_at_text_end) {
			coff_pos = curmetrics.left + (left_to_right_section ? curmetrics.width : 0.0f);
		} else {
			int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(curmetrics.textPosition, curmetrics.length), 1);
			if(num_positions_in_metrics == 1 || curmetrics.length == 0) {
				coff_pos = curmetrics.left + (left_to_right_section ? 0.0f : curmetrics.width);
			} else {
				int32_t num_positions_after_cursor = analysis.number_of_cursor_positions_in_range(v, (curmetrics.textPosition + curmetrics.length) - v);
				float percentage_through_region = float(num_positions_in_metrics - num_positions_after_cursor) / float(num_positions_in_metrics);

				coff_pos = curmetrics.left + (left_to_right_section ? percentage_through_region * curmetrics.width : (1.0f - percentage_through_region) * curmetrics.width);
			}
		}
	}
	
	voff_pos += s.font_collection[font.id].info.line_spacing;

	BOOL is_trailing = FALSE;
	BOOL is_inside = FALSE;
	DWRITE_HIT_TEST_METRICS ht;
	memset(&ht, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	layout->HitTestPoint(coff_pos, voff_pos, &is_trailing, &is_inside, &ht);

	int32_t new_position = 0;
	if(is_inside == TRUE) {
		if(is_trailing == TRUE) {
			new_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
		} else {
			new_position = int32_t(ht.textPosition);
		}
	} else if(ht.length == 1) {
		if(is_trailing == TRUE) {
			new_position = int32_t(std::min(ht.textPosition + 1, uint32_t(internal_text.length())));
		} else {
			new_position = int32_t(ht.textPosition);
		}
	} else {
		int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(ht.textPosition, ht.length), 1);

		if(num_positions_in_metrics == 1) {
			if(is_trailing == TRUE) {
				new_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
			} else {
				new_position = int32_t(ht.textPosition);
			}
		} else {
			bool left_to_right_section = (ht.bidiLevel % 2) == 0;
			float percentage_in_region = ((coff_pos - ht.left) / ht.width);
			if(!left_to_right_section)
				percentage_in_region = 1.0f - percentage_in_region;
			float section_size = 1.0f / float(num_positions_in_metrics);
			float running_total = section_size / 2.0f;

			new_position = int32_t(ht.textPosition);
			[&]() {
				if(percentage_in_region <= running_total) {
					return;
				}
				new_position = analysis.get_next_cursor_position(new_position);
				for(int32_t i = 0; i < num_positions_in_metrics; ++i) {
					running_total += section_size;
					if(percentage_in_region <= running_total) {
						return;
					}
					new_position = analysis.get_next_cursor_position(new_position);
				}
				new_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
			}();
		}
	}
	return uint32_t(new_position);
}
uint32_t dw_editable_text_provider::get_cursor_position_under_point(system_interface& s, screen_space_point pt) {
	if(layout) {
		auto adjusted_x = pt.x;
		auto adjusted_y = pt.y - static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing * starting_line;

		BOOL is_trailing = FALSE;
		BOOL is_inside = FALSE;
		DWRITE_HIT_TEST_METRICS ht;
		memset(&ht, 0, sizeof(DWRITE_HIT_TEST_METRICS));
		layout->HitTestPoint(adjusted_x, adjusted_y, &is_trailing, &is_inside, &ht);

		if(is_inside == FALSE) {
			if(is_trailing == TRUE) {
				return std::min(ht.textPosition + ht.length, uint32_t(internal_text.length()));
			} else {
				return ht.textPosition;
			}
		} else if(ht.length == 1) {
			if(is_trailing == TRUE) {
				return std::min(ht.textPosition + 1, uint32_t(internal_text.length()));
			} else {
				return ht.textPosition;
			}
		} else {
			int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(ht.textPosition, ht.length), 1);

			if(num_positions_in_metrics == 1) {
				if(is_trailing == TRUE) {
					return std::min(ht.textPosition + ht.length, uint32_t(internal_text.length()));
				} else {
					return ht.textPosition;
				}
			} else {
				bool left_to_right_section = (ht.bidiLevel % 2) == 0;
				float percentage_in_region = ((adjusted_x - ht.left) / ht.width);
				if(!left_to_right_section)
					percentage_in_region = 1.0f - percentage_in_region;
				float section_size = 1.0f / float(num_positions_in_metrics);
				float running_total = section_size / 2.0f;

				auto temp_text_pos = ht.textPosition;
				[&]() {
					if(percentage_in_region <= running_total) {
						return;
					}
					temp_text_pos = analysis.get_next_cursor_position(temp_text_pos);
					for(int32_t i = 0; i < num_positions_in_metrics; ++i) {
						running_total += section_size;
						if(percentage_in_region <= running_total) {
							return;
						}
						temp_text_pos = analysis.get_next_cursor_position(temp_text_pos);
					}
					temp_text_pos = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
				}();
				return temp_text_pos;
			}
		}
	}
	return 0;
}
void dw_editable_text_provider::set_cursor_position(system_interface& s, uint32_t position, bool extend_selection) {
	if(read_only)
		return;

	if(cursor_position != uint16_t(position) || (anchor_position != uint16_t(position) && extend_selection == false)) {
		cursor_position = uint16_t(position);
		if(!extend_selection) {
			anchor_position = uint16_t(position);
		}
		on_selection_change(static_cast<win_d2d_dw_ds&>(s));
	}
}
void dw_editable_text_provider::set_selection(win_d2d_dw_ds& win, uint32_t start, uint32_t end) {
	if(read_only)
		return;

	if(cursor_position != int32_t(end) || anchor_position != int32_t(start)) {
		anchor_position = int32_t(start);
		cursor_position = int32_t(end);
		on_selection_change(win);
	}
}
void dw_editable_text_provider::internal_move_cursor_to_point(win_d2d_dw_ds& win, int32_t x, int32_t y, bool extend_selection) {
	if(!read_only) {
		if(layout) {
			if(x == 0 && y == 0) { // because this is what kb activation gives us
				anchor_position = 0;
				cursor_position = int32_t(internal_text.length());
			} else {

				BOOL is_trailing = FALSE;
				BOOL is_inside = FALSE;
				DWRITE_HIT_TEST_METRICS ht;
				memset(&ht, 0, sizeof(DWRITE_HIT_TEST_METRICS));
				layout->HitTestPoint(x, y, &is_trailing, &is_inside, &ht);


				if(is_inside == FALSE) {
					if(is_trailing == TRUE) {
						cursor_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
					} else {
						cursor_position = int32_t(ht.textPosition);
					}
				} else if(ht.length == 1) {
					if(is_trailing == TRUE) {
						cursor_position = int32_t(std::min(ht.textPosition + 1, uint32_t(internal_text.length())));
					} else {
						cursor_position = int32_t(ht.textPosition);
					}
				} else {
					int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(ht.textPosition, ht.length), 1);

					if(num_positions_in_metrics == 1) {
						if(is_trailing == TRUE) {
							cursor_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
						} else {
							cursor_position = int32_t(ht.textPosition);
						}
					} else {
						bool left_to_right_section = (ht.bidiLevel % 2) == 0;
						float percentage_in_region =  ((x - ht.left) / ht.width);
						if(!left_to_right_section)
							percentage_in_region = 1.0f - percentage_in_region;
						float section_size = 1.0f / float(num_positions_in_metrics);
						float running_total = section_size / 2.0f;

						cursor_position = int32_t(ht.textPosition);
						[&]() {
							if(percentage_in_region <= running_total) {
								return;
							}
							cursor_position = analysis.get_next_cursor_position(cursor_position);
							for(int32_t i = 0; i < num_positions_in_metrics; ++i) {
								running_total += section_size;
								if(percentage_in_region <= running_total) {
									return;
								}
								cursor_position = analysis.get_next_cursor_position(cursor_position);
							}
							cursor_position = int32_t(std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())));
						}();
					}
				}
				if(!extend_selection) {
					anchor_position = cursor_position;
				}
			}
		}
		on_selection_change(win);
	}
}
void dw_editable_text_provider::move_cursor_by_screen_pt(system_interface& win, screen_space_point pt, bool extend_selection) {
	if(read_only)
		return;

	auto adjusted_x = pt.x;
	auto adjusted_y = pt.y - static_cast<win_d2d_dw_ds&>(win).font_collection[font.id].info.line_spacing * starting_line;

	internal_move_cursor_to_point(static_cast<win_d2d_dw_ds&>(win), adjusted_x, adjusted_y, extend_selection);
	if(extend_selection == false)
		mouse_entry_position = cursor_position;

	if(static_cast<win_d2d_dw_ds&>(win).selecting_edit_text == edit_selection_mode::word) {
		if(!analysis.is_word_position(cursor_position)) {
			if(cursor_position >= mouse_entry_position) {
				cursor_position = analysis.get_next_word_position(cursor_position);
				anchor_position = analysis.get_previous_word_position(mouse_entry_position);
			} else {
				cursor_position = analysis.get_previous_word_position(cursor_position);
				anchor_position = analysis.get_next_word_position(mouse_entry_position);
			}
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
		}
	} else if(static_cast<win_d2d_dw_ds&>(win).selecting_edit_text == edit_selection_mode::line) {
		if(cursor_position >= mouse_entry_position) {
			cursor_position = analysis.end_of_line(analysis.line_of_position(cursor_position));
			anchor_position = analysis.start_of_line(analysis.line_of_position(mouse_entry_position));
		} else {
			cursor_position = analysis.start_of_line(analysis.line_of_position(cursor_position));
			anchor_position = analysis.end_of_line(analysis.line_of_position(mouse_entry_position));
		}
		on_selection_change(static_cast<win_d2d_dw_ds&>(win));
	}
	return;
}
void dw_editable_text_provider::insert_codepoint(system_interface& s, uint32_t codepoint) {
	if(read_only)
		return;

	auto old_end = std::max(anchor_position, cursor_position);
	auto old_start = std::min(anchor_position, cursor_position);
	if(anchor_position != cursor_position) {
		send_command(s, edit_command::delete_selection, false);
	}
	if(!changes_made)
		static_cast<win_d2d_dw_ds&>(s).edit_undo_buffer.push_state(undo_item{ this, internal_text, anchor_position, cursor_position });
	auto insert_position = std::min(cursor_position, uint16_t(internal_text.length()));
	if(codepoint < 0x10000) {
		internal_text.insert(insert_position, 1, uint16_t(codepoint));
		++cursor_position;
	} else {
		auto p = text::make_surrogate_pair(codepoint);
		internal_text.insert(insert_position, 1, uint16_t(p.high));
		internal_text.insert(insert_position + 1, 1, uint16_t(p.low));
		cursor_position += 2;
	}
	anchor_position = cursor_position;
	on_text_change(static_cast<win_d2d_dw_ds&>(s));
	on_text_change_ts(static_cast<win_d2d_dw_ds&>(s), uint32_t(old_start), uint32_t(old_end), uint32_t(cursor_position));
}
void dw_editable_text_provider::send_command(system_interface& win, edit_command cmd, bool extend_selection) {
	switch(cmd) {
		case edit_command::new_line:
			if(multiline == false) {
				if(static_cast<win_d2d_dw_ds&>(win).minui_root->contains_focus(&attached)) {
					static_cast<win_d2d_dw_ds&>(win).minui_root->back_out_focus(attached);
				}
			} else {
				insert_codepoint(win, L'\n');
			}
			return;
		case edit_command::backspace:
			if(anchor_position != cursor_position) {
				send_command(win, edit_command::delete_selection, false);
			} else {
				if(!changes_made)
					static_cast<win_d2d_dw_ds&>(win).edit_undo_buffer.push_state(undo_item{ this, internal_text, anchor_position, cursor_position });
				auto previous_position = analysis.get_previous_cursor_position(cursor_position);
				if(previous_position != cursor_position) {
					internal_text.erase(size_t(previous_position), size_t(cursor_position - previous_position));
					auto old_cursor = cursor_position;
					cursor_position = previous_position;
					anchor_position = previous_position;
					on_text_change_ts(static_cast<win_d2d_dw_ds&>(win), uint32_t(previous_position), uint32_t(old_cursor), uint32_t(previous_position));
				}
			}
			on_text_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::delete_char:
			if(anchor_position != cursor_position) {
				send_command(win, edit_command::delete_selection, false);
			} else {
				if(!changes_made)
					static_cast<win_d2d_dw_ds&>(win).edit_undo_buffer.push_state(undo_item{ this, internal_text, anchor_position, cursor_position });
				auto next_position = analysis.get_next_cursor_position(cursor_position);
				if(next_position != cursor_position) {
					internal_text.erase(size_t(cursor_position), size_t(next_position - cursor_position));
					on_text_change_ts(static_cast<win_d2d_dw_ds&>(win), uint32_t(cursor_position), uint32_t(next_position), uint32_t(cursor_position));
				}
			}

			on_text_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::backspace_word:
			if(anchor_position != cursor_position) {
				send_command(win, edit_command::delete_selection, false);
			} else {
				static_cast<win_d2d_dw_ds&>(win).edit_undo_buffer.push_state(undo_item{ this, internal_text, anchor_position, cursor_position });
				auto previous_position = analysis.get_previous_word_position(cursor_position);
				if(previous_position != cursor_position) {
					internal_text.erase(size_t(previous_position), size_t(cursor_position - previous_position));
					auto old_cursor = cursor_position;
					cursor_position = previous_position;
					anchor_position = previous_position;
					on_text_change_ts(static_cast<win_d2d_dw_ds&>(win), uint32_t(previous_position), uint32_t(old_cursor), uint32_t(previous_position));
				}
			}

			on_text_change(static_cast<win_d2d_dw_ds&>(win));
			return;
			return;
		case edit_command::delete_word:
			if(anchor_position != cursor_position) {
				send_command(win, edit_command::delete_selection, false);
			} else {
				static_cast<win_d2d_dw_ds&>(win).edit_undo_buffer.push_state(undo_item{ this, internal_text, anchor_position, cursor_position });
				auto next_position = analysis.get_next_word_position(cursor_position);
				if(next_position != cursor_position) {
					internal_text.erase(size_t(cursor_position), size_t(next_position - cursor_position));
					on_text_change_ts(static_cast<win_d2d_dw_ds&>(win), uint32_t(cursor_position), uint32_t(next_position), uint32_t(cursor_position));
				}
			}
			on_text_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::tab:
			//insert_codepoint(win, 0x2003);
			return;
		case edit_command::cursor_down:
			cursor_position = position_visually_below(static_cast<win_d2d_dw_ds&>(win), cursor_position);
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::cursor_up:
			cursor_position = position_visually_above(static_cast<win_d2d_dw_ds&>(win), cursor_position);
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::cursor_left:
			cursor_position = analysis.left_visual_cursor_position(cursor_position, internal_text, static_cast<win_d2d_dw_ds&>(win).left_to_right, static_cast<win_d2d_dw_ds&>(win));
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::cursor_right:
			cursor_position = analysis.right_visual_cursor_position(cursor_position, internal_text, static_cast<win_d2d_dw_ds&>(win).left_to_right, static_cast<win_d2d_dw_ds&>(win));
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::cursor_left_word:
			cursor_position = analysis.left_visual_word_position(cursor_position);
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::cursor_right_word:
			cursor_position = analysis.right_visual_word_position(cursor_position);
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::to_line_start:
			cursor_position = analysis.start_of_line(analysis.line_of_position(cursor_position));
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::to_line_end:
		{
			auto cline = analysis.line_of_position(cursor_position);
			cursor_position = analysis.end_of_line(cline);
			while(analysis.line_of_position(cursor_position - 1) >= cline && cursor_position >= 1 && text::codepoint_is_space(internal_text[cursor_position - 1])) {
				--cursor_position;
			}
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		}
		case edit_command::to_text_start:
			cursor_position = 0;
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::to_text_end:
			cursor_position = int32_t(internal_text.length());
			if(!extend_selection)
				anchor_position = cursor_position;
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::cut:
			if(anchor_position != cursor_position) {
				send_command(win, edit_command::copy, false);
				send_command(win, edit_command::delete_selection, false);
			}
			return;
		case edit_command::copy:
			if(anchor_position != cursor_position) {
				std::wstring_view v(internal_text);
				auto start = std::min(anchor_position, cursor_position);
				auto length = std::max(anchor_position, cursor_position) - start;
				win.text_to_clipboard(v.substr(start, length));
			}
			return;
		case edit_command::paste:
		{
			auto old_start_position = std::min(anchor_position, cursor_position);
			auto old_end_position = std::max(anchor_position, cursor_position);
			static_cast<win_d2d_dw_ds&>(win).edit_undo_buffer.push_state(undo_item{ this, internal_text, anchor_position, cursor_position });
			if(anchor_position != cursor_position) {
				std::wstring_view v(internal_text);
				auto start = std::min(anchor_position, cursor_position);
				auto length = std::max(anchor_position, cursor_position) - start;
				internal_text.erase(size_t(start), size_t(length));
				cursor_position = start;
				anchor_position = start;
			}
			auto cb = win.text_from_clipboard();
			internal_text.insert(size_t(cursor_position), cb);
			cursor_position += int32_t(cb.length());
			anchor_position = cursor_position;

			on_text_change(static_cast<win_d2d_dw_ds&>(win));

			on_text_change_ts(static_cast<win_d2d_dw_ds&>(win), uint32_t(old_start_position), uint32_t(old_end_position), uint32_t(cursor_position));
			changes_made = false;
		}
		return;
		case edit_command::select_all:
			anchor_position = 0;
			cursor_position = int32_t(internal_text.length());
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::undo:
		{
			auto undostate = static_cast<win_d2d_dw_ds&>(win).edit_undo_buffer.undo(undo_item{ this, internal_text, anchor_position, cursor_position });
			if(undostate.has_value()) {
				auto old_length = internal_text.length();
				internal_text = (*undostate).contents;
				cursor_position = (*undostate).cursor;
				anchor_position = (*undostate).anchor;

				on_text_change(static_cast<win_d2d_dw_ds&>(win));
				on_text_change_ts(static_cast<win_d2d_dw_ds&>(win), uint32_t(0), uint32_t(old_length), uint32_t(internal_text.length()));
			}
		}
		return;
		case edit_command::redo:
		{
			auto redostate = static_cast<win_d2d_dw_ds&>(win).edit_undo_buffer.redo(undo_item{ this, internal_text, anchor_position, cursor_position });
			if(redostate.has_value()) {
				auto old_length = internal_text.length();
				internal_text = (*redostate).contents;
				cursor_position = (*redostate).cursor;
				anchor_position = (*redostate).anchor;

				on_text_change(static_cast<win_d2d_dw_ds&>(win));
				on_text_change_ts(static_cast<win_d2d_dw_ds&>(win), uint32_t(0), uint32_t(old_length), uint32_t(internal_text.length()));
			}
		}
		return;
		case edit_command::select_current_word:
			anchor_position = analysis.get_previous_word_position(cursor_position);
			cursor_position = analysis.get_next_word_position(cursor_position);
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::select_current_section:
			anchor_position = analysis.start_of_line(analysis.line_of_position(cursor_position));
			cursor_position = analysis.end_of_line(analysis.line_of_position(cursor_position));
			on_selection_change(static_cast<win_d2d_dw_ds&>(win));
			return;
		case edit_command::delete_selection:
			if(anchor_position != cursor_position) {
				static_cast<win_d2d_dw_ds&>(win).edit_undo_buffer.push_state(undo_item{ this, internal_text, anchor_position, cursor_position });

				auto start = std::min(anchor_position, cursor_position);
				auto length = std::max(anchor_position, cursor_position) - start;
				internal_text.erase(size_t(start), size_t(length));
				cursor_position = start;
				anchor_position = start;

				on_text_change(static_cast<win_d2d_dw_ds&>(win));
				on_text_change_ts(static_cast<win_d2d_dw_ds&>(win), uint32_t(start), uint32_t(start + length), uint32_t(start));
			}
			return;
	}
}
void dw_editable_text_provider::insert_text(system_interface& s, uint32_t position_start, uint32_t position_end, native_string_view content) {
	if(static_cast<win_d2d_dw_ds&>(s).minui_root->contains_focus(&attached)) {
		if(!changes_made)
			static_cast<win_d2d_dw_ds&>(s).edit_undo_buffer.push_state(undo_item{ this, internal_text , anchor_position, cursor_position });
	}

	if(position_start != position_end)
		internal_text.erase(size_t(position_start), size_t(position_end - position_start));
	internal_text.insert(size_t(position_start), content);

	if(int32_t(position_end) < anchor_position) {
		anchor_position += int32_t(content.length()) - int32_t(position_end - position_start);
		anchor_position = std::max(uint16_t(0), anchor_position);
	} else if(int32_t(position_start) <= anchor_position && anchor_position <= int32_t(position_end)) {
		anchor_position = int32_t(position_start + content.length());
	}
	if(int32_t(position_end) < cursor_position) {
		cursor_position += int32_t(content.length()) - int32_t(position_end - position_start);
		cursor_position = std::max(uint16_t(0), cursor_position);
	} else if(int32_t(position_start) <= cursor_position && cursor_position <= int32_t(position_end)) {
		cursor_position = int32_t(position_start + content.length());
	}

	on_text_change(static_cast<win_d2d_dw_ds&>(s));
	on_text_change_ts(static_cast<win_d2d_dw_ds&>(s), position_start, position_end, uint32_t(content.length()));
}
void dw_editable_text_provider::set_read_only(system_interface& s, bool is_read_only) {
	if(is_read_only == read_only)
		return;
	read_only = is_read_only;
	if(read_only) {
		DestroyCaret();
	} else {
		CreateCaret(static_cast<win_d2d_dw_ds&>(s).m_hwnd, nullptr,
			int32_t(std::ceil(1.0f * static_cast<win_d2d_dw_ds&>(s).global_size_multiplier * static_cast<win_d2d_dw_ds&>(s).dpi / 96.0f)),
			int32_t(static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing));
		on_selection_change(static_cast<win_d2d_dw_ds&>(s));
	}
}
ieditable_text::detailed_mouse_test_result dw_editable_text_provider::get_detailed_position(system_interface& s, screen_space_point pt) {
	if(!layout)
		return ieditable_text::detailed_mouse_test_result{ 0, 0 };

	auto adj_x = pt.x;
	auto adj_y = pt.y - static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing * starting_line;

	BOOL is_trailing = FALSE;
	BOOL is_inside = FALSE;
	DWRITE_HIT_TEST_METRICS ht;
	memset(&ht, 0, sizeof(DWRITE_HIT_TEST_METRICS));
	layout->HitTestPoint(float(adj_x), float(adj_y), &is_trailing, &is_inside, &ht);

	if(is_inside == FALSE) {
		if(is_trailing == TRUE) {
			return ieditable_text::detailed_mouse_test_result{ std::min(ht.textPosition + ht.length, uint32_t(internal_text.length())), 3 };
		} else {
			return ieditable_text::detailed_mouse_test_result{ ht.textPosition, 0 };
		}
	} else if(ht.length == 1) {
		auto percentage = (adj_x - ht.left) / ht.width;
		bool right_to_left_section = (ht.bidiLevel % 2) != 0;
		if(right_to_left_section)
			percentage = 1.0f - percentage;
		uint32_t quadrent = 0;
		if(percentage < 0.25f) {
			quadrent = 2;
		} else if(percentage < 0.50f) {
			quadrent = 3;
		} else if(percentage < 0.75f) {
			quadrent = 0;
		} else {
			quadrent = 1;
		}

		return ieditable_text::detailed_mouse_test_result{ ht.textPosition + ((is_trailing == TRUE) != (right_to_left_section == true) ? 1 : 0), quadrent };
	} else {
		bool right_to_left_section = (ht.bidiLevel % 2) != 0;

		auto percentage =  (adj_x - ht.left) / ht.width;

		int32_t num_positions_in_metrics = std::max(analysis.number_of_cursor_positions_in_range(ht.textPosition, ht.length), 1);

		auto temp_text_pos = ht.textPosition;
		int32_t count = 0;
		while(temp_text_pos < ht.textPosition + ht.length) {
			auto next_pos = analysis.get_next_cursor_position(temp_text_pos);

			if(percentage < float(count + 1) / float(num_positions_in_metrics)) {
				auto sub_percentage = (percentage * float(num_positions_in_metrics) - float(count));
				uint32_t quadrent = 0;
				if(sub_percentage < 0.25f) {
					quadrent = 2;
				} else if(sub_percentage < 0.50f) {
					quadrent = 3;
				} else if(sub_percentage < 0.75f) {
					quadrent = 0;
				} else {
					quadrent = 1;
				}
				if(right_to_left_section)
					quadrent = 3 - quadrent;
				return ieditable_text::detailed_mouse_test_result{ quadrent < 2 ? temp_text_pos : next_pos, quadrent };
			}

			temp_text_pos = next_pos;
			++count;
		}
		{
			auto next_pos = analysis.get_next_cursor_position(temp_text_pos);
			return ieditable_text::detailed_mouse_test_result{ uint32_t(next_pos), uint32_t(right_to_left_section ? 1 : 2) };
		}
	}
}
void dw_editable_text_provider::on_focus(system_interface& s) {
	if(!read_only) {
		CreateCaret(static_cast<win_d2d_dw_ds&>(s).m_hwnd, nullptr,
			int32_t(std::ceil(1.0f * static_cast<win_d2d_dw_ds&>(s).global_size_multiplier * static_cast<win_d2d_dw_ds&>(s).dpi / 96.0f)),
			int32_t(static_cast<win_d2d_dw_ds&>(s).font_collection[font.id].info.line_spacing));
		on_selection_change(static_cast<win_d2d_dw_ds&>(s));
	}

	ITfDocumentMgr* old_doc = nullptr;
	static_cast<win_d2d_dw_ds&>(s).ts_manager_ptr->AssociateFocus(static_cast<win_d2d_dw_ds&>(s).m_hwnd, tso->document, &old_doc);
	safe_release(old_doc);
}
void dw_editable_text_provider::on_lose_focus(system_interface& s) {
	if(!read_only) {
		DestroyCaret();
	}

	ITfDocumentMgr* old_doc = nullptr;
	static_cast<win_d2d_dw_ds&>(s).ts_manager_ptr->AssociateFocus(static_cast<win_d2d_dw_ds&>(s).m_hwnd, nullptr, &old_doc);
	safe_release(old_doc);
}

void dw_editable_text_provider::register_composition_result(win_d2d_dw_ds& win) {
	// accessibility notification
	//if(acc_obj) {
	//	win.accessibility_interface.on_composition_result(acc_obj, std::wstring_view(text.data() + temp_text_position, size_t(temp_text_length)));
	//}
	temp_text_position = 0;
	temp_text_length = 0;
	selection_out_of_date = true;
}
void dw_editable_text_provider::register_conversion_target_change(win_d2d_dw_ds& win) {
	// accessibility notification
	//if(acc_obj && win.is_visible(l_id)) {
	//	win.accessibility_interface.on_conversion_target_changed(acc_obj);
	//}
}
void dw_editable_text_provider::set_temporary_selection(win_d2d_dw_ds& win, uint32_t start, uint32_t end) {
	temp_text_position = int32_t(start);
	temp_text_length = int32_t(end - start);
	selection_out_of_date = true;

	// accessibility notification
	//if(acc_obj) {
	//	win.accessibility_interface.on_composition_change(acc_obj, std::wstring_view(text.data(), size_t(end - start)));
	//}
		
}
uint32_t dw_editable_text_provider::get_position_from_point(win_d2d_dw_ds& win, screen_space_point pt) {
	auto full_result = this->get_detailed_position(win, pt);
	return full_result.position;
}
void dw_editable_text_provider::get_range_bounds(win_d2d_dw_ds& win, uint32_t position_start, uint32_t position_end, std::vector<screen_space_rect>& rects) {
	if(position_start == position_end)
		return;

	auto selection_start = std::min(position_start, position_end);
	auto selection_end = std::max(position_start, position_end);

	prepare_text(win, win.to_screen_space(attached.position.width));
	
	uint32_t metrics_size = 0;
	layout->HitTestTextRange(selection_start, selection_end - selection_start, 0, 0, nullptr, 0, &metrics_size);

	std::vector<DWRITE_HIT_TEST_METRICS> mstorage(metrics_size);
	layout->HitTestTextRange(selection_start, selection_end - selection_start, 0, 0, mstorage.data(), metrics_size, &metrics_size);

	rects.reserve(metrics_size);

	auto screen_pos_base = win.to_screen_space(win.minui_root->workspace_placement(attached));
	auto base_size = win.to_screen_space(attached.position);
	screen_pos_base.x += win.window_border_size;
	screen_pos_base.y += win.window_border_size;
	if(!win.left_to_right)
		screen_pos_base.x = win.client_x - (screen_pos_base.x + base_size.width);

	for(auto& r : mstorage) {
		rects.push_back(screen_space_rect{
			int32_t(std::round(r.left)) + screen_pos_base.x,
			int32_t(std::round(r.top) + screen_pos_base.y - win.font_collection[font.id].info.line_spacing * starting_line),
			int32_t(std::round(r.width)),
			int32_t(std::round(r.height)) });
		
	}
}

enum class lock_state : uint8_t {
	unlocked, locked_read, locked_readwrite
};

struct mouse_sink {
	ITfMouseSink* sink;
	LONG range_start;
	LONG range_length;
};

struct text_services_object : public ITextStoreACP2, public ITfInputScope, public ITfContextOwnerCompositionSink, public ITfMouseTrackerACP {

	std::vector<TS_ATTRVAL> gathered_attributes;
	std::vector<mouse_sink> installed_mouse_sinks;
	dw_editable_text_provider* ei = nullptr;
	win_d2d_dw_ds* win = nullptr;
	ITfDocumentMgr* document = nullptr;
	ITfContext* primary_context = nullptr;
	TfEditCookie content_identifier = 0;
	ITextStoreACPSink* advise_sink = nullptr;
	lock_state document_lock_state = lock_state::unlocked;
	bool notify_on_text_change = false;
	bool notify_on_selection_change = false;
	bool relock_pending = false;
	bool in_composition = false;

	ULONG m_refCount;

	void free_gathered_attributes() {
		for(auto& i : gathered_attributes) {
			VariantClear(&(i.varValue));
		}
		gathered_attributes.clear();
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) {
		if(riid == __uuidof(IUnknown))
			*ppvObject = static_cast<ITextStoreACP2*>(this);
		else if(riid == __uuidof(ITextStoreACP2))
			*ppvObject = static_cast<ITextStoreACP2*>(this);
		else if(riid == __uuidof(ITfInputScope))
			*ppvObject = static_cast<ITfInputScope*>(this);
		else if(riid == __uuidof(ITfContextOwnerCompositionSink))
			*ppvObject = static_cast<ITfContextOwnerCompositionSink*>(this);
		else if(riid == __uuidof(ITfMouseTrackerACP))
			*ppvObject = static_cast<ITfMouseTrackerACP*>(this);
		else {
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
		(static_cast<IUnknown*>(*ppvObject))->AddRef();
		return S_OK;
	}
	virtual ULONG STDMETHODCALLTYPE AddRef() {
		return InterlockedIncrement(&m_refCount);
	}
	virtual ULONG STDMETHODCALLTYPE Release() {
		long val = InterlockedDecrement(&m_refCount);
		if(val == 0) {
			delete this;
		}
		return val;
	}

	//ITfMouseTrackerACP
	virtual HRESULT STDMETHODCALLTYPE AdviseMouseSink(__RPC__in_opt ITfRangeACP* range, __RPC__in_opt ITfMouseSink* pSink, __RPC__out DWORD* pdwCookie) {
		if(!range || !pSink || !pdwCookie)
			return E_INVALIDARG;
		for(uint32_t i = 0; i < installed_mouse_sinks.size(); ++i) {
			if(installed_mouse_sinks[i].sink == nullptr) {
				installed_mouse_sinks[i].sink = pSink;
				pSink->AddRef();
				installed_mouse_sinks[i].range_start = 0;
				installed_mouse_sinks[i].range_length = 0;
				range->GetExtent(&(installed_mouse_sinks[i].range_start), &(installed_mouse_sinks[i].range_length));
				*pdwCookie = DWORD(i);
				return S_OK;
			}
		}
		installed_mouse_sinks.emplace_back();
		installed_mouse_sinks.back().sink = pSink;
		pSink->AddRef();
		installed_mouse_sinks.back().range_start = 0;
		installed_mouse_sinks.back().range_length = 0;
		range->GetExtent(&(installed_mouse_sinks.back().range_start), &(installed_mouse_sinks.back().range_length));
		*pdwCookie = DWORD(installed_mouse_sinks.size() - 1);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE UnadviseMouseSink(DWORD dwCookie) {
		if(dwCookie < installed_mouse_sinks.size()) {
			safe_release(installed_mouse_sinks[dwCookie].sink);
		}
		return S_OK;
	}

	bool send_mouse_event(int32_t x, int32_t y, uint32_t buttons) {
		if(installed_mouse_sinks.empty())
			return false;
		if(!ei)
			return false;

		auto detailed_pos = ei->get_detailed_position(*win, screen_space_point{ x, y });
		for(auto& ms : installed_mouse_sinks) {
			if(ms.sink && int32_t(detailed_pos.position) >= ms.range_start && int32_t(detailed_pos.position) <= ms.range_start + ms.range_length) {
				BOOL eaten = false;
				ms.sink->OnMouseEvent(detailed_pos.position, detailed_pos.quadrent, buttons, &eaten);
				if(eaten)
					return true;
			}
		}
		return false;
	}

	//ITfContextOwnerCompositionSink
	virtual HRESULT STDMETHODCALLTYPE OnStartComposition(__RPC__in_opt ITfCompositionView* /*pComposition*/, __RPC__out BOOL* pfOk) {
		*pfOk = TRUE;
		in_composition = true;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnUpdateComposition(__RPC__in_opt ITfCompositionView* pComposition, __RPC__in_opt ITfRange* /*pRangeNew*/) {
		ITfRange* view_range = nullptr;
		pComposition->GetRange(&view_range);
		if(view_range) {
			ITfRangeACP* acp_range = nullptr;
			view_range->QueryInterface(IID_PPV_ARGS(&acp_range));
			if(ei && acp_range) {
				LONG start = 0;
				LONG count = 0;
				acp_range->GetExtent(&start, &count);
				if(count > 0) {
					ei->set_temporary_selection(*win, uint32_t(start), uint32_t(start + count));
				}
			}
			safe_release(acp_range);
		}
		safe_release(view_range);

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnEndComposition(__RPC__in_opt ITfCompositionView* /*pComposition*/) {
		if(ei) {
			ei->register_composition_result(*win);
		}
		in_composition = false;
		return S_OK;
	}

	// ITextStoreACP2
	virtual HRESULT STDMETHODCALLTYPE AdviseSink(__RPC__in REFIID riid, __RPC__in_opt IUnknown* punk, DWORD dwMask) {
		if(!IsEqualGUID(riid, IID_ITextStoreACPSink)) {
			return E_INVALIDARG;
		}
		ITextStoreACPSink* temp = nullptr;
		if(FAILED(punk->QueryInterface(IID_ITextStoreACPSink, (void**)&temp))) {
			return E_NOINTERFACE;
		}
		if(advise_sink) {
			if(advise_sink == temp) {
				safe_release(temp);
			} else {
				return CONNECT_E_ADVISELIMIT;
			}
		} else {
			advise_sink = temp;
		}
		notify_on_text_change = (TS_AS_TEXT_CHANGE & dwMask) != 0;
		notify_on_selection_change = (TS_AS_SEL_CHANGE & dwMask) != 0;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE UnadviseSink(__RPC__in_opt IUnknown* /*punk*/) {
		safe_release(advise_sink);
		notify_on_text_change = false;
		notify_on_selection_change = false;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE RequestLock(DWORD dwLockFlags, __RPC__out HRESULT* phrSession) {
		if(!advise_sink) {
			*phrSession = E_FAIL;
			return E_UNEXPECTED;
		}

		relock_pending = false;

		if(document_lock_state != lock_state::unlocked) {
			if(dwLockFlags & TS_LF_SYNC) {
				*phrSession = TS_E_SYNCHRONOUS;
				return S_OK;
			} else {
				if(document_lock_state == lock_state::locked_read && (dwLockFlags & TS_LF_READWRITE) == TS_LF_READWRITE) {
					relock_pending = true;
					*phrSession = TS_S_ASYNC;
					return S_OK;
				}
			}
			return E_FAIL;
		}

		if((TS_LF_READ & dwLockFlags) != 0) {
			document_lock_state = lock_state::locked_read;
		}
		if((TS_LF_READWRITE & dwLockFlags) != 0) {
			document_lock_state = lock_state::locked_readwrite;
		}

		*phrSession = advise_sink->OnLockGranted(dwLockFlags);
		document_lock_state = lock_state::unlocked;

		if(relock_pending) {
			relock_pending = false;
			HRESULT hr = S_OK;
			RequestLock(TS_LF_READWRITE, &hr);
		}

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetStatus(__RPC__out TS_STATUS* pdcs) {
		if(!pdcs)
			return E_INVALIDARG;
		pdcs->dwStaticFlags = TS_SS_NOHIDDENTEXT;
		pdcs->dwDynamicFlags = (ei && ei->is_read_only() ? TS_SD_READONLY : 0);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInsert(LONG acpTestStart, LONG acpTestEnd, ULONG /*cch*/, __RPC__out LONG* pacpResultStart, __RPC__out LONG* pacpResultEnd) {
		if(!pacpResultStart || !pacpResultEnd || acpTestStart > acpTestEnd)
			return E_INVALIDARG;
		if(!ei)
			return TF_E_DISCONNECTED;
		*pacpResultStart = std::min(acpTestStart, LONG(ei->internal_text.length()));
		*pacpResultEnd = std::min(acpTestEnd, LONG(ei->internal_text.length()));
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetSelection(ULONG ulIndex, ULONG ulCount, __RPC__out_ecount_part(ulCount, *pcFetched) TS_SELECTION_ACP* pSelection, __RPC__out ULONG* pcFetched) {
		if(!pcFetched)
			return E_INVALIDARG;
		if(document_lock_state == lock_state::unlocked)
			return TS_E_NOLOCK;

		if(ei && ulCount > 0 && (ulIndex == 0 || ulIndex == TF_DEFAULT_SELECTION)) {
			if(!pSelection)
				return E_INVALIDARG;
			auto start = ei->cursor_position;
			auto end = ei->anchor_position;
			pSelection[0].acpStart = int32_t(std::min(start, end));
			pSelection[0].acpEnd = int32_t(std::max(start, end));
			pSelection[0].style.ase = start < end ? TS_AE_START : TS_AE_END;
			pSelection[0].style.fInterimChar = FALSE;
			*pcFetched = 1;
		} else {
			*pcFetched = 0;
		}
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE SetSelection(ULONG ulCount, __RPC__in_ecount_full(ulCount) const TS_SELECTION_ACP* pSelection) {
		if(document_lock_state != lock_state::locked_readwrite)
			return TF_E_NOLOCK;
		if(!ei)
			return TF_E_DISCONNECTED;
		if(ulCount > 0) {
			if(!pSelection)
				return E_INVALIDARG;

			auto start = pSelection->style.ase == TS_AE_START ? pSelection->acpEnd : pSelection->acpStart;
			auto end = pSelection->style.ase == TS_AE_START ? pSelection->acpStart : pSelection->acpEnd;
			ei->set_selection(*win, start, end);
		}
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetText(LONG acpStart, LONG acpEnd, __RPC__out_ecount_part(cchPlainReq, *pcchPlainRet) WCHAR* pchPlain, ULONG cchPlainReq, __RPC__out ULONG* pcchPlainRet, __RPC__out_ecount_part(cRunInfoReq, *pcRunInfoRet) TS_RUNINFO* prgRunInfo, ULONG cRunInfoReq, __RPC__out ULONG* pcRunInfoRet, __RPC__out LONG* pacpNext) {

		if(!pcchPlainRet || !pcRunInfoRet)
			return E_INVALIDARG;
		if(!pchPlain && cchPlainReq != 0)
			return E_INVALIDARG;
		if(!prgRunInfo && cRunInfoReq != 0)
			return E_INVALIDARG;
		if(!pacpNext)
			return E_INVALIDARG;
		if(document_lock_state == lock_state::unlocked)
			return TF_E_NOLOCK;

		*pcchPlainRet = 0;

		if(!ei)
			return TF_E_DISCONNECTED;

		if((cchPlainReq == 0) && (cRunInfoReq == 0)) {
			return S_OK;
		}
		auto len = LONG(ei->internal_text.length());
		acpEnd = std::min(acpEnd, len);
		if(acpEnd == -1)
			acpEnd = len;

		acpEnd = std::min(acpEnd, acpStart + LONG(cchPlainReq));
		if(acpStart != acpEnd) {
			std::copy(ei->internal_text.c_str(), ei->internal_text.c_str() + (acpEnd - acpStart), pchPlain);
			*pcchPlainRet = (acpEnd - acpStart);
		}
		if(*pcchPlainRet < cchPlainReq) {
			*(pchPlain + *pcchPlainRet) = 0;
		}
		if(cRunInfoReq != 0) {
			prgRunInfo[0].uCount = acpEnd - acpStart;
			prgRunInfo[0].type = TS_RT_PLAIN;
			*pcRunInfoRet = 1;
		} else {
			*pcRunInfoRet = 0;
		}

		*pacpNext = acpEnd;

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE SetText(DWORD /*dwFlags*/, LONG acpStart, LONG acpEnd, __RPC__in_ecount_full(cch) const WCHAR* pchText, ULONG cch, __RPC__out TS_TEXTCHANGE* pChange) {
		if(document_lock_state != lock_state::locked_readwrite)
			return TF_E_NOLOCK;
		if(!ei)
			return TF_E_DISCONNECTED;
		if(!pChange)
			return E_INVALIDARG;
		if(!pchText && cch > 0)
			return E_INVALIDARG;

		auto len = LONG(ei->internal_text.length());

		if(acpStart > len)
			return E_INVALIDARG;

		LONG acpRemovingEnd = acpEnd >= acpStart ? std::min(acpEnd, len) : acpStart;


		win->ts_send_notifications = false;
		ei->insert_text(*win, uint32_t(acpStart), uint32_t(acpRemovingEnd), std::wstring_view(pchText, cch));
		win->ts_send_notifications = true;

		pChange->acpStart = acpStart;
		pChange->acpOldEnd = acpRemovingEnd;
		pChange->acpNewEnd = acpStart + cch;

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetFormattedText(LONG /*acpStart*/, LONG /*acpEnd*/, __RPC__deref_out_opt IDataObject** /*ppDataObject*/) {
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetEmbedded(LONG /*acpPos*/, __RPC__in REFGUID /*rguidService*/, __RPC__in REFIID /*riid*/, __RPC__deref_out_opt IUnknown** ppunk) {
		if(!ppunk)
			return E_INVALIDARG;
		*ppunk = nullptr;
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInsertEmbedded(__RPC__in const GUID* /*pguidService*/, __RPC__in const FORMATETC* pFormatEtc, __RPC__out BOOL* pfInsertable) {
		if(!pFormatEtc || !pfInsertable)
			return E_INVALIDARG;
		if(pfInsertable)
			*pfInsertable = FALSE;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE InsertEmbedded(DWORD /*dwFlags*/, LONG /*acpStart*/, LONG /*acpEnd*/, __RPC__in_opt IDataObject* /*pDataObject*/, __RPC__out TS_TEXTCHANGE* /*pChange*/) {
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE InsertTextAtSelection(DWORD dwFlags, __RPC__in_ecount_full(cch) const WCHAR* pchText, ULONG cch, __RPC__out LONG* pacpStart, __RPC__out LONG* pacpEnd, __RPC__out TS_TEXTCHANGE* pChange) {


		if(!ei)
			return TF_E_DISCONNECTED;

		LONG acpStart = std::min(ei->cursor_position, ei->anchor_position);
		LONG acpEnd = std::max(ei->cursor_position, ei->anchor_position);

		if((dwFlags & TS_IAS_QUERYONLY) != 0) {
			if(document_lock_state == lock_state::unlocked)
				return TS_E_NOLOCK;
			if(pacpStart)
				*pacpStart = acpStart;
			if(pacpEnd)
				*pacpEnd = acpStart + cch;
			return S_OK;
		}

		if(document_lock_state != lock_state::locked_readwrite)
			return TF_E_NOLOCK;
		if(!pchText)
			return E_INVALIDARG;


		win->ts_send_notifications = false;
		ei->insert_text(*win, uint32_t(acpStart), uint32_t(acpEnd), std::wstring_view(pchText, cch));
		win->ts_send_notifications = true;

		if(pacpStart)
			*pacpStart = acpStart;
		if(pacpEnd)
			*pacpEnd = acpStart + cch;

		if(pChange) {
			pChange->acpStart = acpStart;
			pChange->acpOldEnd = acpEnd;
			pChange->acpNewEnd = acpStart + cch;
		}

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE InsertEmbeddedAtSelection(DWORD /*dwFlags*/, __RPC__in_opt IDataObject* /*pDataObject*/, __RPC__out LONG* /*pacpStart*/, __RPC__out LONG* /*pacpEnd*/, __RPC__out TS_TEXTCHANGE* /*pChange*/) {

		return E_NOTIMPL;
	}

	void fill_gathered_attributes(ULONG cFilterAttrs, __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID* paFilterAttrs, bool fill_variants, int32_t position) {
		free_gathered_attributes();

		for(uint32_t i = 0; i < cFilterAttrs; ++i) {
			if(IsEqualGUID(paFilterAttrs[i], GUID_PROP_INPUTSCOPE)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_UNKNOWN;
					ITfInputScope* is = nullptr;
					this->QueryInterface(IID_PPV_ARGS(&is));
					gathered_attributes.back().varValue.punkVal = is;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Font_FaceName) && ei) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BSTR;
					gathered_attributes.back().varValue.bstrVal = SysAllocString(win->font_collection[ei->font].info.name.c_str());
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Font_SizePts) && ei) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_I4;
					gathered_attributes.back().varValue.lVal = int32_t(win->font_collection[ei->font].info.font_size * 96.0f / win->dpi);
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Font_Style_Bold) && ei) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = win->font_collection[ei->font].info.weight > 400 ? VARIANT_TRUE : VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Font_Style_Height) && ei) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_I4;
					gathered_attributes.back().varValue.lVal = int32_t(win->font_collection[ei->font].info.font_size);
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Font_Style_Hidden)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Font_Style_Italic) && ei) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = win->font_collection[ei->font].info.is_oblique ? VARIANT_TRUE : VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Font_Style_Weight) && ei) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_I4;
					gathered_attributes.back().varValue.lVal = win->font_collection[ei->font].info.weight;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_Alignment_Center)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = ei && ei->get_alignment() == text::content_alignment::centered ? VARIANT_TRUE : VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_Alignment_Justify)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = ei && ei->get_alignment() == text::content_alignment::justified ? VARIANT_TRUE : VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_Alignment_Left)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = ei && ei->get_alignment() == text::content_alignment::leading ? VARIANT_TRUE : VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_Alignment_Right)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = ei && ei->get_alignment() == text::content_alignment::trailing ? VARIANT_TRUE : VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_Orientation)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_I4;
					gathered_attributes.back().varValue.lVal = 0;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_ReadOnly)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = ei && ei->is_read_only() ? VARIANT_TRUE : VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_RightToLeft)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					if(position >= 0) {
						gathered_attributes.back().varValue.boolVal = ei && ei->analysis.position_is_ltr(position) ? VARIANT_FALSE : VARIANT_TRUE;
					} else {
						gathered_attributes.back().varValue.boolVal = VARIANT_TRUE;
					}
				}
			} else if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_VerticalWriting)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					gathered_attributes.back().varValue.boolVal = VARIANT_FALSE;
				}
			} else if(IsEqualGUID(paFilterAttrs[i], GUID_PROP_COMPOSING)) {
				gathered_attributes.emplace_back();
				gathered_attributes.back().idAttr = paFilterAttrs[i];
				gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
				if(fill_variants) {
					gathered_attributes.back().varValue.vt = VT_BOOL;
					if(position >= 0) {
						gathered_attributes.back().varValue.boolVal = ei && (uint32_t(position) >= ei->temp_text_position) && (uint32_t(position) < ei->temp_text_length + ei->temp_text_position) ? VARIANT_TRUE : VARIANT_FALSE;
					} else {
						gathered_attributes.back().varValue.boolVal = VARIANT_FALSE;
					}
				}
			}
		}
	}

	virtual HRESULT STDMETHODCALLTYPE RequestSupportedAttrs(DWORD dwFlags, ULONG cFilterAttrs, __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID* paFilterAttrs) {
		if(!paFilterAttrs && cFilterAttrs > 0)
			return E_INVALIDARG;
		if(!ei)
			return TF_E_DISCONNECTED;

		bool fill_variants = (TS_ATTR_FIND_WANT_VALUE & dwFlags) != 0;
		fill_gathered_attributes(cFilterAttrs, paFilterAttrs, fill_variants, -1);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE RequestAttrsAtPosition(LONG acpPos, ULONG cFilterAttrs, __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID* paFilterAttrs, DWORD dwFlags) {
		if(!paFilterAttrs && cFilterAttrs > 0)
			return E_INVALIDARG;
		if(!ei)
			return TF_E_DISCONNECTED;

		bool fill_variants = (TS_ATTR_FIND_WANT_VALUE & dwFlags) != 0;
		fill_gathered_attributes(cFilterAttrs, paFilterAttrs, fill_variants, acpPos);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE RequestAttrsTransitioningAtPosition(LONG acpPos, ULONG cFilterAttrs, __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID* paFilterAttrs, DWORD dwFlags) {
		if(!paFilterAttrs && cFilterAttrs > 0)
			return E_INVALIDARG;
		if(!ei)
			return TF_E_DISCONNECTED;

		free_gathered_attributes();
		bool fill_variants = (TS_ATTR_FIND_WANT_VALUE & dwFlags) != 0;
		bool attributes_that_end = (TS_ATTR_FIND_WANT_END & dwFlags) != 0;
		for(uint32_t i = 0; i < cFilterAttrs; ++i) {
			if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_RightToLeft)) {
				if(acpPos > 0 && ei && ei->analysis.position_is_ltr(uint32_t(acpPos - 1)) != ei->analysis.position_is_ltr(uint32_t(acpPos))) {

					gathered_attributes.emplace_back();
					gathered_attributes.back().idAttr = paFilterAttrs[i];
					gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
					if(fill_variants) {
						gathered_attributes.back().varValue.vt = VT_BOOL;

						if(attributes_that_end)
							gathered_attributes.back().varValue.boolVal = ei && ei->analysis.position_is_ltr(uint32_t(acpPos - 1)) ? VARIANT_FALSE : VARIANT_TRUE;
						else
							gathered_attributes.back().varValue.boolVal = ei && ei->analysis.position_is_ltr(uint32_t(acpPos)) ? VARIANT_FALSE : VARIANT_TRUE;
					}
				}
			} else if(IsEqualGUID(paFilterAttrs[i], GUID_PROP_COMPOSING)) {
				if(acpPos > 0 && ei &&
					(((uint32_t(acpPos - 1) >= ei->temp_text_position) && (uint32_t(acpPos - 1) < ei->temp_text_length + ei->temp_text_position))
						!=
						((uint32_t(acpPos) >= ei->temp_text_position) && (uint32_t(acpPos) < ei->temp_text_length + ei->temp_text_position)))) {

					gathered_attributes.emplace_back();
					gathered_attributes.back().idAttr = paFilterAttrs[i];
					gathered_attributes.back().dwOverlapId = int32_t(gathered_attributes.size());
					if(fill_variants) {
						gathered_attributes.back().varValue.vt = VT_BOOL;
						if(attributes_that_end)
							gathered_attributes.back().varValue.boolVal = ei && (uint32_t(acpPos - 1) >= ei->temp_text_position) && (uint32_t(acpPos - 1) < ei->temp_text_length + ei->temp_text_position) ? VARIANT_TRUE : VARIANT_FALSE;
						else
							gathered_attributes.back().varValue.boolVal = ei && (uint32_t(acpPos) >= ei->temp_text_position) && (uint32_t(acpPos) < ei->temp_text_length + ei->temp_text_position) ? VARIANT_TRUE : VARIANT_FALSE;

					}
				}
			}
		}
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FindNextAttrTransition(LONG acpStart, LONG acpHalt, ULONG cFilterAttrs, __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID* paFilterAttrs, DWORD dwFlags, __RPC__out LONG* pacpNext, __RPC__out BOOL* pfFound, __RPC__out LONG* plFoundOffset) {
		if(!pacpNext || !pfFound || !plFoundOffset)
			return E_INVALIDARG;

		*pfFound = FALSE;
		if(plFoundOffset)
			*plFoundOffset = 0;
		*pacpNext = acpStart;

		if(!ei)
			return TF_E_DISCONNECTED;

		LONG initial_position = std::max(acpStart, LONG(1));
		LONG end_position = std::min(acpHalt, LONG(ei->internal_text.length() - 1));
		end_position = std::max(initial_position, end_position);

		if((TS_ATTR_FIND_BACKWARDS & dwFlags) != 0) {
			std::swap(initial_position, end_position);
		}
		while(initial_position != end_position) {
			for(uint32_t i = 0; i < cFilterAttrs; ++i) {
				if(IsEqualGUID(paFilterAttrs[i], TSATTRID_Text_RightToLeft)) {
					if(ei->analysis.position_is_ltr(uint32_t(initial_position - 1)) != ei->analysis.position_is_ltr(uint32_t(initial_position))) {
						*pfFound = TRUE;
						if(plFoundOffset)
							*plFoundOffset = initial_position;
						*pacpNext = initial_position;
						return S_OK;
					}
				} else if(IsEqualGUID(paFilterAttrs[i], GUID_PROP_COMPOSING)) {
					if(((uint32_t(initial_position - 1) >= ei->temp_text_position) && (uint32_t(initial_position - 1) < ei->temp_text_length + ei->temp_text_position))
						!=
						((uint32_t(initial_position) >= ei->temp_text_position) && (uint32_t(initial_position) < ei->temp_text_length + ei->temp_text_position))) {
						*pfFound = TRUE;
						if(plFoundOffset)
							*plFoundOffset = initial_position;
						*pacpNext = initial_position;
						return S_OK;
					}
				}
			}
			if((TS_ATTR_FIND_BACKWARDS & dwFlags) != 0) {
				--initial_position;
			} else {
				++initial_position;
			}
		}
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE RetrieveRequestedAttrs(ULONG ulCount, __RPC__out_ecount_part(ulCount, *pcFetched) TS_ATTRVAL* paAttrVals, __RPC__out ULONG* pcFetched) {

		*pcFetched = 0;
		uint32_t i = 0;
		for(; i < ulCount && i < gathered_attributes.size(); ++i) {
			paAttrVals[i] = gathered_attributes[i];
			*pcFetched++;
		}
		for(; i < gathered_attributes.size(); ++i) {
			VariantClear(&(gathered_attributes[i].varValue));
		}
		gathered_attributes.clear();
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetEndACP(__RPC__out LONG* pacp) {
		if(!pacp)
			return E_INVALIDARG;
		if(document_lock_state == lock_state::unlocked)
			return TS_E_NOLOCK;
		*pacp = 0;
		if(!ei)
			return TF_E_DISCONNECTED;
		*pacp = LONG(ei->internal_text.length());
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetActiveView(__RPC__out TsViewCookie* pvcView) {
		if(!pvcView)
			return E_INVALIDARG;
		*pvcView = 0;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetACPFromPoint(TsViewCookie /*vcView*/, __RPC__in const POINT* ptScreen, DWORD dwFlags, __RPC__out LONG* pacp) {

		*pacp = 0;
		if(!ei)
			return TF_E_DISCONNECTED;

		RECT window_rect;
		GetWindowRect(win->m_hwnd, &window_rect);

		auto screen_pos_base = win->to_screen_space(win->minui_root->workspace_placement(ei->attached));
		auto base_size = win->to_screen_space(ei->attached.position);
		screen_pos_base.x += win->window_border_size;
		screen_pos_base.y += win->window_border_size;
		if(!win->left_to_right)
			screen_pos_base.x = win->client_x - (screen_pos_base.x + base_size.width);

		if((GXFPF_NEAREST & dwFlags) == 0) {
			if(ptScreen->x < window_rect.left + screen_pos_base.x || ptScreen->y < window_rect.top + screen_pos_base.y
				|| ptScreen->x > window_rect.top + screen_pos_base.x + base_size.width || ptScreen->y > window_rect.top + screen_pos_base.y + base_size.height) {
				return TS_E_INVALIDPOINT;
			}
		}

		auto from_point = ei->get_position_from_point(*win, screen_space_point{ int32_t(ptScreen->x - (window_rect.left + screen_pos_base.x)), int32_t(ptScreen->y - (window_rect.top + screen_pos_base.y) });
		*pacp = from_point;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTextExt(TsViewCookie /*vcView*/, LONG acpStart, LONG acpEnd, __RPC__out RECT* prc, __RPC__out BOOL* pfClipped) {
		if(!pfClipped || !prc)
			return E_INVALIDARG;
		if(document_lock_state == lock_state::unlocked)
			return TS_E_NOLOCK;

		*pfClipped = FALSE;
		*prc = RECT{ 0, 0, 0, 0 };

		if(win->is_minimized()) {
			*pfClipped = TRUE;
			return S_OK;
		}
		if(!ei)
			return TF_E_DISCONNECTED;

		std::vector<screen_space_rect> selection_rects;
		ei->get_range_bounds(*win, uint32_t(acpStart), uint32_t(acpEnd), selection_rects);

		RECT window_rect;
		GetWindowRect(win->m_hwnd, &window_rect);

		if(selection_rects.size() > 0) {
			prc->left = selection_rects[0].x + window_rect.left;
			prc->top = selection_rects[0].y + window_rect.top;
			prc->right = selection_rects[0].x + selection_rects[0].width + window_rect.left;
			prc->bottom = selection_rects[0].y + selection_rects[0].height + window_rect.top;
		}
		for(uint32_t i = 1; i < selection_rects.size(); ++i) {
			prc->left = std::min(LONG(selection_rects[i].x + window_rect.left), prc->left);
			prc->top = std::min(LONG(selection_rects[i].y + window_rect.top), prc->top);
			prc->right = std::max(LONG(selection_rects[i].x + selection_rects[i].width + window_rect.left), prc->right);
			prc->bottom = std::max(LONG(selection_rects[i].y + selection_rects[i].height + window_rect.top), prc->bottom);
		}

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetScreenExt(TsViewCookie /*vcView*/, __RPC__out RECT* prc) {
		if(!prc)
			return E_INVALIDARG;

		*prc = RECT{ 0, 0, 0, 0 };

		if(!ei)
			return TF_E_DISCONNECTED;
		
		if(win->is_minimized())
			return S_OK;

		auto screen_pos_base = win->to_screen_space(win->minui_root->workspace_placement(ei->attached));
		auto base_size = win->to_screen_space(ei->attached.position);
		screen_pos_base.x += win->window_border_size;
		screen_pos_base.y += win->window_border_size;
		if(!win->left_to_right)
			screen_pos_base.x = win->client_x - (screen_pos_base.x + base_size.width);

		RECT window_rect;
		GetWindowRect(win->m_hwnd, &window_rect);

		prc->left = window_rect.left + screen_pos_base.x;
		prc->top = window_rect.top + screen_pos_base.y;
		prc->right = window_rect.left + screen_pos_base.x + base_size.width;
		prc->bottom = window_rect.top + screen_pos_base.y + base_size.height;

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetInputScopes(__RPC__deref_out_ecount_full_opt(*pcCount) InputScope** pprgInputScopes, __RPC__out UINT* pcCount) {
		*pprgInputScopes = (InputScope*)CoTaskMemAlloc(sizeof(InputScope));
		if(ei) {
			switch(ei->content_type()) {
				case edit_contents::generic_text:
					*(*pprgInputScopes) = IS_TEXT;
					break;
				case edit_contents::number:
					*(*pprgInputScopes) = IS_NUMBER;
					break;
				case edit_contents::single_char:
					*(*pprgInputScopes) = IS_ONECHAR;
					break;
				case edit_contents::email:
					*(*pprgInputScopes) = IS_EMAIL_SMTPEMAILADDRESS;
					break;
				case edit_contents::date:
					*(*pprgInputScopes) = IS_DATE_FULLDATE;
					break;
				case edit_contents::time:
					*(*pprgInputScopes) = IS_TIME_FULLTIME;
					break;
				case edit_contents::url:
					*(*pprgInputScopes) = IS_URL;
					break;
			}
		} else {
			*(*pprgInputScopes) = IS_DEFAULT;
		}
		*pcCount = 1;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetPhrase(__RPC__deref_out_ecount_full_opt(*pcCount) BSTR** ppbstrPhrases, __RPC__out UINT* pcCount) {
		*ppbstrPhrases = nullptr;
		*pcCount = 0;
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetRegularExpression(__RPC__deref_out_opt BSTR* pbstrRegExp) {
		*pbstrRegExp = nullptr;
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetSRGS(__RPC__deref_out_opt BSTR* pbstrSRGS) {
		*pbstrSRGS = nullptr;
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetXML(__RPC__deref_out_opt BSTR* pbstrXML) {
		*pbstrXML = nullptr;
		return E_NOTIMPL;
	}

	text_services_object(ITfThreadMgr* manager_ptr, TfClientId owner, win_d2d_dw_ds* win, dw_editable_text_provider* ei) : win(win), ei(ei), m_refCount(1) {
		manager_ptr->CreateDocumentMgr(&document);
		auto hr = document->CreateContext(owner, 0, static_cast<ITextStoreACP2*>(this), &primary_context, &content_identifier);
		hr = document->Push(primary_context);
	}
	void disconnect() {
		ei = nullptr;
	}
	virtual ~text_services_object() {
		free_gathered_attributes();
	}
};
void dw_editable_text_provider::consume_mouse_event(system_interface& win, int32_t x, int32_t y, uint32_t buttons) {
	tso->send_mouse_event(x, y, buttons);
}
void dw_editable_text_provider::on_selection_change_ts(win_d2d_dw_ds& s) {
	if(s.ts_send_notifications && tso->advise_sink && tso->notify_on_selection_change) {
		tso->advise_sink->OnSelectionChange();
	}
}
void dw_editable_text_provider::on_text_change_ts(win_d2d_dw_ds& s, uint32_t old_start, uint32_t old_end, uint32_t new_end) {
	if(s.ts_send_notifications && tso->advise_sink && tso->notify_on_text_change) {
		TS_TEXTCHANGE chng{ LONG(old_start), LONG(old_end), LONG(new_end) };
		tso->advise_sink->OnTextChange(0, &chng);
	}
}
void dw_editable_text_provider::clear_temporary_contents(win_d2d_dw_ds& win) {
	internal_text.erase(size_t(temp_text_position), size_t(temp_text_length));
	cursor_position = temp_text_position;
	anchor_position = temp_text_position;
	auto old_length = temp_text_length;
	temp_text_length = 0;

	on_text_change(win);

	if(win.ts_send_notifications && tso->advise_sink && tso->notify_on_text_change) {
		TS_TEXTCHANGE chng{ LONG(temp_text_position), LONG(temp_text_position + old_length), LONG(temp_text_position) };
		tso->advise_sink->OnTextChange(0, &chng);
	}
}
dw_editable_text_provider::dw_editable_text_provider(ui_node& attached, win_d2d_dw_ds& win) : attached(attached) {
	tso = new text_services_object(win.ts_manager_ptr, win.ts_client_id, &win, this);
}
dw_editable_text_provider::~dw_editable_text_provider() {
	safe_release(layout);
	safe_release(cached_text);
	tso->disconnect();
	safe_release(tso);
}

std::optional<undo_item> undo_buffer::undo(undo_item current_state) {
	push_state(current_state);

	auto temp_back = buffer_position - 1;
	if(temp_back < 0) {
		temp_back += total_size;
	}

	if(interal_buffer[temp_back].from == current_state.from) {
		buffer_position = temp_back;
		return interal_buffer[temp_back];
	}

	return std::optional<undo_item>{};
}
std::optional<undo_item> undo_buffer::redo(undo_item current_state) {
	if(interal_buffer[buffer_position] == current_state) {
		auto temp_back = buffer_position + 1;
		if(temp_back >= total_size) {
			temp_back -= total_size;
		}

		if(interal_buffer[temp_back].from == current_state.from) {
			buffer_position = temp_back;
			return interal_buffer[buffer_position];
		}
	}
	return std::optional<undo_item>{};
}
void undo_buffer::push_state(undo_item state) {
	if(interal_buffer[buffer_position] != state) {
		++buffer_position;
		if(buffer_position >= total_size) {
			buffer_position -= total_size;
		}

		interal_buffer[buffer_position] = state;
		auto temp_next = buffer_position + 1;
		if(temp_next >= total_size) {
			temp_next -= total_size;
		}
		interal_buffer[temp_next].from = nullptr;
	}
}

void win_d2d_dw_ds::create_window() {
	// todo: coinitialize

	CoCreateInstance(CLSID_TF_ThreadMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr, (void**)&ts_manager_ptr);
	ts_manager_ptr->Activate(&ts_client_id);

	// create window
	// run message loop

	ts_manager_ptr->Deactivate();
}

}
