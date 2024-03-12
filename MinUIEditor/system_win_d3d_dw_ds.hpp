#pragma once

#include "../common_files/minui_interfaces.hpp"
#include "../common_files/minui_text_impl.hpp"
#include "simple_fs_types_win.hpp"
#include "simple_fs.hpp"

#ifndef UNICODE
#define UNICODE
#endif
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <dwrite_3.h>
#include <chrono>
#include <usp10.h>
#include <msctf.h>
#include "Dshow.h"

namespace minui {

using cardinal_plural_fn = text::attribute_type(*)(int64_t, int64_t, int32_t);
using ordinal_plural_fn = text::attribute_type(*)(int64_t);

void parse_into_cardinality_fns(ankerl::unordered_dense::map<std::wstring_view, cardinal_plural_fn>& map, std::wstring_view list, cardinal_plural_fn fn);
void parse_into_ordinal_fns(ankerl::unordered_dense::map<std::wstring_view, ordinal_plural_fn>& map, std::wstring_view list, ordinal_plural_fn fn);

struct locale_data_s {
	std::wstring app_locale;
	std::wstring display_name;
	bool os_locale_is_default = true;

	// for filling in number format
	uint32_t  leading_zero;
	uint32_t  grouping;
	wchar_t decimal_sep[5];
	wchar_t thousand_sep[5];
	uint32_t  negative_order;
	uint32_t lcid;


	ankerl::unordered_dense::map<std::wstring_view, cardinal_plural_fn> cardinal_functions;
	ankerl::unordered_dense::map<std::wstring_view, ordinal_plural_fn> ordinal_functions;

	cardinal_plural_fn cardinal_classification = nullptr;
	ordinal_plural_fn ordinal_classification = nullptr;

	locale_data_s();
};

template<typename T>
void safe_release(T*& v) {
	if(v) {
		v->Release();
		v = nullptr;
	}
}

struct font_data {
	std::vector<text::font_fallback> fallbacks;
	text::font info;
	IDWriteFontFallback* fallback = nullptr;
	IDWriteTextFormat3* format = nullptr;

	font_data() { }
	font_data(text::font&& f) : info(std::move(f)) { }
	font_data(font_data&& o) noexcept : fallbacks(std::move(o.fallbacks)), info(std::move(o.info)) {
		std::swap(fallback, o.fallback);
		std::swap(format, o.format);
	}
	font_data& operator=(font_data&& o) noexcept {
		fallbacks = std::move(o.fallbacks);
		info = std::move(o.info);
		std::swap(fallback, o.fallback);
		std::swap(format, o.format);
		return *this;
	}

	~font_data() {
		safe_release(fallback);
		safe_release(format);
	}
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

enum class keyboard_type : uint8_t {
	left_hand, right_hand, right_hand_tilted
};

class win_d2d_dw_ds;

struct analysis_object {
	std::vector<SCRIPT_LOGATTR> char_attributes;
	std::vector<uint16_t> line_breaks;

	void update_analyzed_text(IDWriteTextLayout* txt, std::wstring_view str, bool ltr, win_d2d_dw_ds& system);
	int32_t number_of_lines();
	int32_t line_of_position(int32_t position);
	int32_t start_of_line(int32_t line);
	int32_t end_of_line(int32_t line);
	int32_t left_visual_cursor_position(int32_t position, std::wstring_view str, bool ltr, win_d2d_dw_ds& system);
	int32_t right_visual_cursor_position(int32_t position, std::wstring_view str, bool ltr, win_d2d_dw_ds& tm);
	int32_t number_of_cursor_positions_in_range(int32_t start, int32_t count);
	int32_t get_previous_cursor_position(int32_t position);
	int32_t get_next_cursor_position(int32_t position);
	int32_t left_visual_word_position(int32_t position);
	int32_t right_visual_word_position(int32_t position);
	int32_t get_previous_word_position(int32_t position);
	int32_t get_next_word_position(int32_t position);
	bool position_is_ltr(int32_t position);
	bool is_cursor_position(int32_t position);
	bool is_word_position(int32_t position);
};

class dw_static_text_provider : public static_text_provider {
public:
	text::formatted_text internal_text;
	ui_node& attached;
	IDWriteTextLayout* layout = nullptr;
	ID2D1Bitmap1* cached_text = nullptr;
	int32_t displayed_x_size = 0;
	int32_t displayed_y_size = 0;
	uint16_t language_generation = 0;
	uint16_t font_generation = 0;
	int16_t starting_line = 0;
	int16_t lines_used = 0;
	int16_t single_line_width = 0;
	text::content_alignment alignment;
	text::font_handle font;
	bool multiline = false;
	bool requires_update = false;
	bool requires_rerender = false;

	dw_static_text_provider(ui_node& attached) : attached(attached) { }
	~dw_static_text_provider() {
		safe_release(layout);
		safe_release(cached_text);
	}
	text::formatted_text_reference view_text(system_interface&) const final {
		return internal_text;
	}
	void set_text(system_interface&, text::formatted_text&&) final;

	text::content_alignment get_alignment() const final {
		return alignment;
	}
	void set_alignment(system_interface&, text::content_alignment) final;

	text::font_handle get_font() const final {
		return font;
	}
	void set_font(system_interface&, text::font_handle) final;

	bool get_is_multiline() const final {
		return multiline;
	}
	void set_is_multiline(system_interface&, bool) final;

	int32_t get_number_of_displayed_lines(system_interface&) final;
	int32_t get_number_of_text_lines(system_interface&) final;
	int32_t get_starting_display_line() final;
	void set_starting_display_line(int32_t v) final;
	em get_single_line_width(system_interface&) final;
	em get_line_height(system_interface&) final;
	void resize_to_width(system_interface& s, int32_t w) final;

	void render(system_interface&, layout_rect r, uint16_t brush, rendering_modifiers display_flags = rendering_modifiers::none, bool in_focus = false) final;

	screen_space_rect get_character_bounds(system_interface&, uint32_t position) const final;
	istatic_text::mouse_test_result get_position(system_interface&, screen_space_point pt) final;
};


struct text_services_object;

class dw_editable_text_provider : public editable_text_provider {
	void prepare_selection_regions();
	void update_cached_cursor_position();
	void prepare_text(win_d2d_dw_ds& s);
	void on_selection_change_ts(win_d2d_dw_ds& s);
	void on_text_change_ts(win_d2d_dw_ds& s, uint32_t old_start, uint32_t old_end, uint32_t new_end);
	void on_selection_change(win_d2d_dw_ds& s);
	void on_text_change(win_d2d_dw_ds& win);
	void make_line_visible(win_d2d_dw_ds& s, int32_t line);
	uint32_t position_visually_above(win_d2d_dw_ds& s, uint32_t v);
	uint32_t position_visually_below(win_d2d_dw_ds& s, uint32_t v);
	void internal_move_cursor_to_point(win_d2d_dw_ds& win, int32_t x, int32_t y, bool extend_selection);
public:
	void register_composition_result(win_d2d_dw_ds& win);
	void register_conversion_target_change(win_d2d_dw_ds& win);
	void clear_temporary_contents(win_d2d_dw_ds& win);
	void set_temporary_selection(win_d2d_dw_ds& win, uint32_t start, uint32_t end);
	void set_selection(win_d2d_dw_ds& s, uint32_t start, uint32_t end);
	uint32_t get_position_from_point(win_d2d_dw_ds& win, screen_space_point pt);
	void get_range_bounds(win_d2d_dw_ds& win, uint32_t position_start, uint32_t position_end, std::vector<screen_space_rect>& rects);

	struct selection_run {
		int32_t start = 0;
		int32_t end = 0;
		int32_t line = 0;
	};

	std::wstring internal_text;
	analysis_object analysis;
	std::vector<selection_run> cached_selection_region;
	ui_node& attached;
	IDWriteTextLayout* layout = nullptr;
	ID2D1Bitmap1* cached_text = nullptr;
	text_services_object* tso = nullptr;
	int32_t displayed_x_size = 0;
	int32_t displayed_y_size = 0;
	uint16_t anchor_position = 0;
	uint16_t cursor_position = 0;
	uint16_t mouse_entry_position = 0;
	uint16_t temp_text_position = 0;
	uint16_t temp_text_length = 0;
	uint16_t cached_cursor_postion = 0;
	uint16_t language_generation = 0;
	uint16_t font_generation = 0;
	int16_t starting_line = 0;
	int16_t lines_used = 0;
	int16_t single_line_width = 0;
	text::content_alignment alignment;
	text::font_handle font;
	edit_contents internal_content_type = edit_contents::generic_text;
	bool multiline = false;
	bool requires_update = true;
	bool read_only = false;
	bool selection_out_of_date = false;
	bool render_out_of_date = false;
	bool changes_made = false;

	dw_editable_text_provider(ui_node& attached, win_d2d_dw_ds& win);
	~dw_editable_text_provider();

	edit_contents content_type() const final {
		return internal_content_type;
	}
	text::formatted_text_reference view_text(system_interface&) const final {
		text::formatted_text_reference temp;
		temp.text_content = internal_text.data();
		temp.text_length = uint32_t(internal_text.length());
		return temp;
	}
	void set_text(system_interface&, text::formatted_text&&) final;

	text::content_alignment get_alignment() const final {
		return alignment;
	}
	void set_alignment(system_interface&, text::content_alignment) final;

	text::font_handle get_font() const final {
		return font;
	}
	void set_font(system_interface&, text::font_handle) final;

	bool get_is_multiline() const final {
		return multiline;
	}
	void set_is_multiline(system_interface&, bool) final;

	int32_t get_number_of_displayed_lines(system_interface&) final;
	int32_t get_number_of_text_lines(system_interface&) final;
	int32_t get_starting_display_line() final;
	em get_single_line_width(system_interface&) final;
	void set_starting_display_line(int32_t v) final;
	em get_line_height(system_interface&) final;
	void resize_to_width(system_interface& s, int32_t w) final;

	void render(system_interface&, layout_rect r, uint16_t brush, rendering_modifiers display_flags = rendering_modifiers::none, bool in_focus = false) final;

	screen_space_rect get_character_bounds(system_interface&, uint32_t position) const final;
	istatic_text::mouse_test_result get_position(system_interface&, screen_space_point pt) final;
	uint32_t get_cursor_position() const final {
		return cursor_position;
	}
	uint32_t get_cursor_position_under_point(system_interface&, screen_space_point) final;
	void set_cursor_position(system_interface&, uint32_t, bool extend_selection) final;
	void insert_codepoint(system_interface&, uint32_t codepoint) final;
	void send_command(system_interface&, edit_command, bool extend_selection) final;
	void insert_text(system_interface&, uint32_t position_start, uint32_t position_end, native_string_view content) final;
	uint32_t get_selection_anchor() const final {
		return anchor_position;
	}
	void set_read_only(system_interface& s, bool is_read_only) final;
	bool is_read_only() const final {
		return read_only;
	}
	ieditable_text::detailed_mouse_test_result get_detailed_position(system_interface&, screen_space_point pt) final;
	void move_cursor_by_screen_pt(system_interface& win, screen_space_point pt, bool extend_selection) final;
	void on_focus(system_interface&) final;
	void on_lose_focus(system_interface&) final;
	void consume_mouse_event(system_interface& win, int32_t x, int32_t y, uint32_t buttons) final;
};

struct arrangement_result {
	IDWriteTextLayout* ptr = nullptr;
	int32_t lines_used = 0;
	int32_t single_line_width = 0;
};

enum class edit_selection_mode : uint8_t {
	none, standard, word, line
};

struct undo_item {
	dw_editable_text_provider* from = nullptr;
	std::wstring contents;
	int32_t anchor = 0;
	int32_t cursor = 0;

	bool operator==(undo_item const& o) const noexcept {
		return from == o.from && contents == o.contents;
	}
	bool operator!=(undo_item const& o) const noexcept {
		return !(*this == o);
	}
};

struct undo_buffer {
	constexpr static int32_t total_size = 32;

	undo_item interal_buffer[total_size] = { };
	int32_t buffer_position = 0;

	std::optional<undo_item> undo(undo_item current_state);
	std::optional<undo_item> redo(undo_item current_state);
	void push_state(undo_item state);
};

struct icon_slot_item {
	ID2D1Bitmap* icon_bitmap = nullptr;
	ID2D1SvgDocument* doc = nullptr;

	icon_slot_item() = default;
	icon_slot_item(icon_slot_item const& o) = delete;
	icon_slot_item(icon_slot_item&& o) noexcept {
		icon_bitmap = o.icon_bitmap;
		o.icon_bitmap = nullptr;
		doc = o.doc;
		o.doc = nullptr;
	}
	icon_slot_item& operator=(icon_slot_item&& o) noexcept {
		std::swap(icon_bitmap, o.icon_bitmap);
		std::swap(doc, o.doc);
		return *this;
	}
	~icon_slot_item() {
		safe_release(icon_bitmap);
	}
	void redraw(win_d2d_dw_ds& s);
};

struct icon_slot {
	em x{ 100 };
	em y{ 100 };
	std::vector< icon_slot_item> sub_items;
};

struct brush_slot {
	ID2D1Brush* brush = nullptr;
	ID2D1Bitmap* brush_bitmap = nullptr;
	ID2D1Brush* disabled_brush = nullptr;
	ID2D1Bitmap* disabled_brush_bitmap = nullptr;

	brush_color color;
	float line_shading = 0.0f;
	float highlight_shading = 0.0f;
	float line_highlight_shading = 0.0f;

	brush_slot() = default;
	brush_slot(brush_slot const& o) = delete;
	brush_slot(brush_slot&& o) noexcept {
		brush = o.brush;
		o.brush = nullptr;
		brush_bitmap = o.brush_bitmap;
		o.brush_bitmap = nullptr;
		disabled_brush = o.disabled_brush;
		o.disabled_brush = nullptr;
		disabled_brush_bitmap = o.disabled_brush_bitmap;
		o.disabled_brush_bitmap = nullptr;
		color = o.color;
		line_shading = o.line_shading;
		highlight_shading = o.highlight_shading;
		line_highlight_shading = o.line_highlight_shading;
	}
	brush_slot& operator=(brush_slot&& o) noexcept {
		std::swap(brush, o.brush);
		std::swap(brush_bitmap, o.brush_bitmap);
		std::swap(disabled_brush, o.disabled_brush);
		std::swap(disabled_brush_bitmap, o.disabled_brush_bitmap);
		color = o.color;
		line_shading = o.line_shading;
		highlight_shading = o.highlight_shading;
		line_highlight_shading = o.line_highlight_shading;
		return *this;
	}
	~brush_slot() {
		safe_release(brush);
		safe_release(brush_bitmap);
		safe_release(disabled_brush);
		safe_release(disabled_brush_bitmap);
	}
};

struct  sound_slot {
	IGraphBuilder* graph_interface = nullptr;
	IMediaControl* control_interface = nullptr;
	IBasicAudio* audio_interface = nullptr;
	IMediaSeeking* seek_interface = nullptr;
	IMediaEventEx* event_interface = nullptr;

	std::wstring filename;
	float volume_multiplier = 1.0f;

	sound_slot() {
	}
	sound_slot(std::wstring const& file) : filename(file) {
	}
	sound_slot(sound_slot const&) = delete;
	sound_slot(sound_slot&& o) noexcept
		: graph_interface(o.graph_interface), control_interface(o.control_interface),
		audio_interface(o.audio_interface), seek_interface(o.seek_interface), event_interface(o.event_interface), filename(std::move(o.filename)),
		volume_multiplier(o.volume_multiplier) {

		o.graph_interface = nullptr;
		o.control_interface = nullptr;
		o.audio_interface = nullptr;
		o.seek_interface = nullptr;
		o.event_interface = nullptr;
	}
	sound_slot& operator=(sound_slot&& o) noexcept {
		filename = std::move(o.filename);
		volume_multiplier = o.volume_multiplier;

		graph_interface = o.graph_interface;
		control_interface = o.control_interface;
		audio_interface = o.audio_interface;
		seek_interface = o.seek_interface;
		event_interface = o.event_interface;

		o.graph_interface = nullptr;
		o.control_interface = nullptr;
		o.audio_interface = nullptr;
		o.seek_interface = nullptr;
		o.event_interface = nullptr;
		return *this;
	}
	~sound_slot() {
		if(event_interface) {
			event_interface->SetNotifyWindow(NULL, 0, NULL);
			event_interface = nullptr;
		}

		safe_release(graph_interface);
		safe_release(control_interface);
		safe_release(audio_interface);
		safe_release(seek_interface);
		safe_release(event_interface);
	}

	void set_file(std::wstring const& file) {
		filename = file;
	}
};

class sfs_file : public file {
public:
	std::variant<simple_fs::unopened_file, simple_fs::file> f;

	sfs_file(simple_fs::unopened_file &&u) : f(std::move(u)) { }
	sfs_file(simple_fs::file&& u) : f(std::move(u)) {}

	 char const* data() final;
	size_t size() final;
	native_string name() final;
	~sfs_file() {}
};

class sfs_directory : public directory {
public:
	simple_fs::directory d;

	sfs_directory(simple_fs::directory&& u) : d(std::move(u)) {}

	std::unique_ptr<file> open_file(native_string_view name) final;
	std::unique_ptr<directory> open_directory(native_string_view name) final;
	std::vector<std::unique_ptr<file>> list_files() final;
	std::vector<std::unique_ptr<directory>> list_directories() final;
	native_string name() final;
	~sfs_directory() {}
};


class win_d2d_dw_ds : public system_interface {
public:
	std::vector<font_data> font_collection;
	std::vector<icon_slot> icon_collection;
	std::vector<brush_slot> brush_collection;
	std::vector< sound_slot> sound_collection;
	simple_fs::file_system fs;

	locale_data_s locale_data;
	text::backing_arrays text_data;
	undo_buffer edit_undo_buffer;

	HWND m_hwnd = nullptr;
	int32_t base_layout_size = 0;
	int32_t base_border_size = 0;
	int32_t window_border_size = 0;
	DWORD window_style = 0;
	DWORD window_style_ex = 0;
	int32_t pixels_per_em = 0;
	float dpi = 96.0f;
	float global_size_multiplier = 1.0f;
	int32_t client_x = 0;
	int32_t client_y = 0;
	bool left_to_right = true;

	float sfx_volume_multiplier = 1.0f;

	bool cursor_is_visible = true;
	bool hide_window_elements = false;
	bool sticky_modifier_keys = false;
	bool shift_down = false;
	bool alt_down = false;
	edit_selection_mode selecting_edit_text = edit_selection_mode::none;

	std::chrono::steady_clock::time_point last_double_click;
	uint32_t double_click_ms = 500;
	uint32_t controller_number_plugged_in = 0;
	input_mode imode = input_mode::follow_input;
	bool controller_mouse_running = false;
	double controller_deadzone = 0.02;
	float controller_sensitivity = 1.0f;
	std::chrono::steady_clock::time_point controller_mouse_start;
	double controller_mouse_current_x = 0.0;
	double controller_mouse_current_y = 0.0;
	uint32_t controller_buttons_val = 0;
	keyboard_type kb_type = keyboard_type::left_hand;
	uint32_t caret_blink_ms = 1000;
	bool caret_blinks = true;

	bool is_suspended = false;
	bool rendering_as_highlighted_line = false;

	uint16_t language_generation = 0;
	uint16_t font_generation = 0;

	ID3D11Device* d3d_device = nullptr;
	IDXGIDevice1* dxgi_device = nullptr;
	ID2D1Device5* d2d_device = nullptr;

	ID3D11DeviceContext* d3d_device_context = nullptr;
	ID2D1DeviceContext5* d2d_device_context = nullptr;
	IDXGISwapChain1* swap_chain = nullptr;

	IDWriteFactory6* dwrite_factory = nullptr;
	IDWriteFontSetBuilder2* dw_font_collection_builder = nullptr;
	IDWriteFontCollection2* dw_font_collection = nullptr;
	IDWriteRenderingParams3* rendering_params = nullptr;

	ID2D1Bitmap1* back_buffer_target = nullptr;
	ID2D1SolidColorBrush* solid_brush = nullptr;
	ID2D1SolidColorBrush* white_brush = nullptr;
	ID2D1StrokeStyle* plain_strokes = nullptr;

	ID2D1Factory6* d2d_factory = nullptr;
	IWICImagingFactory* wic_factory = nullptr;

	ITfThreadMgr* ts_manager_ptr = nullptr;
	TfClientId ts_client_id = 0;
	bool ts_send_notifications = true;

	void resize_text();
	void recreate_dpi_dependent_resource();
	void create_window_size_resources();
	void create_device_resources();
	void release_device_resources();
	void d2dsetup();
	void render();

	void on_controller_input();
	void on_device_change(WPARAM, HANDLE);

	void apply_formatting(IDWriteTextLayout* target, text::format_marker const* formatting, uint32_t format_count);
	arrangement_result create_text_arragement(text::formatted_text_reference text, text::content_alignment text_alignment, text::font_handle font, bool single_line, int32_t max_width);
	ID2D1Brush* get_brush(uint16_t b, rendering_modifiers m);

	root* minui_root = nullptr;
	
	

	win_d2d_dw_ds(int32_t base_layout_size, int32_t base_border_size, bool left_to_right) : base_layout_size(base_layout_size), base_border_size(base_border_size), left_to_right(left_to_right) {
		simple_fs::add_root(fs, L".");
	}
	~win_d2d_dw_ds() {
		safe_release(dw_font_collection_builder);
		safe_release(dw_font_collection);
		safe_release(dwrite_factory);
		safe_release(rendering_params);

		safe_release(d3d_device);
		safe_release(dxgi_device);
		safe_release(d2d_device);

		safe_release(d3d_device_context);
		safe_release(d2d_device_context);
		safe_release(swap_chain);

		safe_release(solid_brush);
		safe_release(white_brush);
		safe_release(plain_strokes);

		safe_release(ts_manager_ptr);
		safe_release(wic_factory);
		safe_release(d2d_factory);

		safe_release(back_buffer_target);
	}

	void create_window(int32_t window_x_size, int32_t window_y_size, bool borderless, bool fullscreen);

	void on_dpi_change(float new_dpi);
	void reshow_cursor();

	bool get_ltr(bool ltr) const {
		return left_to_right;
	}
	std::wstring_view get_app_locale() const {
		return locale_data.app_locale;
	}
	uint32_t get_lcid() const {
		return locale_data.lcid;
	}
	void register_root(root& r) {
		minui_root = &r;
	}

	// WINDOW FUNCTIONS
	layout_position get_workspace() const final;
	void hide_mouse_cursor() final;
	void show_mouse_cursor() final;
	bool is_mouse_cursor_visible() const final;
	uint32_t get_window_dpi() const final;
	bool is_maximized() const final;
	bool is_minimized() const final;
	void maximize() final;
	void minimize() final;
	void restore() final;
	void close() final;
	void set_window_title(native_char const* t) final;
	bool window_has_focus() const final;

	int32_t to_screen_space(em v) const final {
		return int32_t(float(v.value * pixels_per_em) / 100.0f);
	}
	screen_space_point to_screen_space(layout_position p) const final {
		return screen_space_point{ to_screen_space(p.x), to_screen_space(p.y) };
	}
	screen_space_rect to_screen_space(layout_rect r) const final {
		return screen_space_rect{ to_screen_space(r.x), to_screen_space(r.y), int32_t(float(r.width.value * pixels_per_em) / 100.0f), int32_t(float(r.height.value * pixels_per_em) / 100.0f) };
	}
	em to_ui_space(float v) const final {
		return em{ int16_t(v * 100.0f / float(pixels_per_em)) };
	}
	layout_position to_ui_space(screen_space_point p) const final {
		return layout_position{ to_ui_space(p.x), to_ui_space(p.y) };
	}
	layout_rect to_ui_space(screen_space_rect r) const final {
		return layout_rect{ to_ui_space(r.x), to_ui_space(r.y), em{ int16_t(r.width * 100.0f / float(pixels_per_em)) }, em{ int16_t(r.height * 100.0f / float(pixels_per_em)) } };
	}

	// SYSTEM FUNCTIONS
	void display_fatal_error_message(native_string_view) final;
	void text_to_clipboard(native_string_view) final;
	native_string text_from_clipboard() final;

	// FILE FUNCTIONS
	std::unique_ptr<directory> get_root_directory() final;

	// SOUND FUNCTIONS
	void  load_sound(sound_handle, native_string_view file_name) final;
	void play_sound(sound_handle) final;

	// LOCALE FUNCTION
	void set_locale(native_string_view id) final;
	void set_ltr_mode(bool is_ltr) final;
	void set_locale_name(native_string name) final;
	native_string_view get_locale_name() final;
	native_string_view get_locale() final;

	// TEXT FUNCTIONS
	std::unique_ptr<static_text_provider> make_text(ui_node&) final;
	std::unique_ptr<editable_text_provider> make_editable_text(ui_node&) final;

	void add_font_file_to_collection(native_string_view file_name) final;
	void finalize_font_collection() final;
	void make_font_definition(text::font_handle slot, text::font&& font_object) final;
	void add_font_fallback(text::font_handle slot, text::font_fallback&& font_object) final;
	void add_localization_file(native_string_view file_name) final;

	text::handle get_hande(std::string_view key) final;
	text::formatted_text fp_to_text(double fp, int32_t precision, bool show_plus = false) final;
	text::formatted_text int_to_text(int64_t value, bool show_plus = false) final;
	int64_t text_to_int(native_string_view text) final;
	double text_to_double(native_string_view text) final;

	text::formatted_text perform_substitutions(text::text_source body_text, text::text_source const* parameters, size_t parameter_count) final;
	text::variable get_text_variable(std::string_view name) final;

	// GRAPHICS FUNCTIONS
	void rectangle(screen_space_rect content_rect, rendering_modifiers display_flags, uint16_t brush) final;
	void empty_rectangle(screen_space_rect content_rect, rendering_modifiers display_flags, uint16_t brush) final;
	void line(screen_space_point start, screen_space_point end, float width, uint16_t brush) final;
	void interactable(screen_space_point location, interactable_state state, uint16_t fg_brush, interactable_orientation o, rendering_modifiers display_flags = rendering_modifiers::none) final;
	void image(image_handle img, screen_space_rect, int32_t sub_slot = 0) final;
	void background(image_handle img, screen_space_rect, layout_rect interior, rendering_modifiers display_flags = rendering_modifiers::none) final;
	void icon(icon_handle ico, screen_space_rect, uint16_t br, rendering_modifiers display_flags = rendering_modifiers::none, int32_t sub_slot = 0) final;
	void set_line_highlight_mode(bool highlight_on) final;


	void stop_ui_animations() final;
	void prepare_ui_animation() final;
	void prepare_layered_ui_animation() final;
	void start_ui_animation(animation_description description) final;
	void register_in_place_animation() final;

	layout_position get_icon_size(icon_handle ico) final;
	void add_to_icon_slot(icon_handle slot, native_string_view file_name, em x_ems, em y_ems, int32_t sub_index) final;
	void add_svg_to_icon_slot(icon_handle slot, native_string_view file_name, em x_ems, em y_ems, int32_t sub_index) final;
	int32_t get_icon_set_size(icon_handle ico) final;

	void add_color_brush(uint16_t id, brush_color c, bool as_disabled) final;
	void add_image_color_brush(uint16_t id, native_string_view file_name, brush_color c, bool as_disabled) final;
	void set_brush_highlights(uint16_t id, float line_shading, float highlight_shading, float line_highlight_shading) final;

	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	friend class dw_static_text_provider;
	friend class dw_editable_text_provider;
};

}
