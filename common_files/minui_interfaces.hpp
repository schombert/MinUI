#pragma once
#include <stdint.h>
#include <ctype.h>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <variant>
#include <optional>
#include <chrono>
#include <array>

namespace minui {

#ifdef _WIN64

#ifndef UNICODE
#define UNICODE
#endif

using native_string = std::wstring;
using native_string_view = std::wstring_view;
using native_char = wchar_t;

template<typename T>
native_string to_native_string(T&& v) noexcept {
	return std::to_wstring(v);
}

#define NATIVE(X) L##X
#define NATIVE_M(X) NATIVE(X)

#else

using native_string = std::string;
using native_string_view = std::string_view;
using native_char = char;

template<typename T>
native_string to_native_string(T&& v) noexcept {
	return std::to_string(v);
}

#define NATIVE(X) X
#define NATIVE_M(X) NATIVE(X)

#endif



namespace text {

struct surrogate_pair {
	uint16_t high = 0; // aka leading
	uint16_t low = 0; // aka trailing
};

enum class attribute_type : int8_t {
	undefined = -1,
	zero = 0,
	one = 1,
	two = 2,
	few = 3,
	many = 4,
	other = 5,
	ord_zero = 6,
	ord_one = 7,
	ord_two = 8,
	ord_few = 9,
	ord_many = 10,
	ord_other = 11,
	z = 12, // == true zero
	last_predefined = 12,
};

inline constexpr uint32_t max_attributes = 8;

enum class extra_formatting : uint8_t {
	none, small_caps, italic, old_numbers, tabular_numbers, bold, superscript, subscript, close_formatting
};
struct variable {
	uint16_t id = 0;
	auto operator<=>(variable const&) const = default;
};
struct variable_attributes {
	attribute_type low = attribute_type::undefined;
	attribute_type high = attribute_type::undefined;
};
struct inline_icon_id {
	uint16_t id = 0;
	auto operator<=>(inline_icon_id const&) const = default;
};
struct substitution_mark {
	uint16_t id = 0;
	auto operator<=>(substitution_mark const&) const = default;
};
struct match_set_reference {
	uint16_t id = 0;
	auto operator<=>(match_set_reference const&) const = default;
};
struct match_set_terminal_reference {
	uint16_t id = 0;
	auto operator<=>(match_set_terminal_reference const&) const = default;
};
struct color {
	uint16_t brush;
	auto operator<=>(color const&) const = default;
};
struct color_end {
};

struct handle {
	uint32_t id = 0xFFFFFFFF;
	auto operator<=>(handle const&) const = default;
	operator bool() const {
		return id != 0xFFFFFFFF;
	}
};
struct font_handle {
	uint16_t id = 0xFFFF;
	auto operator<=>(font_handle const&) const = default;
	operator bool() const {
		return id != 0xFFFF;
	}
};

using formatting_content = std::variant<std::monostate, variable, variable_attributes, inline_icon_id, substitution_mark, extra_formatting, match_set_reference, match_set_terminal_reference, color, color_end>;

struct format_marker {
	uint16_t position;
	formatting_content format;

	bool operator<(const format_marker& o) const noexcept {
		return position < o.position;
	}
};

struct formatted_text_reference {
	native_char const* text_content = nullptr;
	format_marker const* formatting_content = nullptr;
	std::array<attribute_type, max_attributes> provided_attribues = { attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined };
	uint32_t text_length = 0;
	uint32_t formatting_length = 0;
};

struct formatted_text {
	native_string text_content;
	std::vector<format_marker> formatting_content;
	std::array<attribute_type, max_attributes> provided_attribues = { attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined };

	operator formatted_text_reference() const& {
		return formatted_text_reference{ text_content.data(), formatting_content.data(), provided_attribues, uint32_t(text_content.size()), uint32_t(formatting_content.size()) };
	}
};

struct text_source {
	std::variant<handle, formatted_text_reference> input_text;
	variable var{ 0 };

	text_source(std::variant<handle, formatted_text_reference> const& r, variable v) : input_text(r), var(v) { }
	text_source(handle r) : input_text(r) { }
	text_source(formatted_text_reference r) : input_text(r) {
	}
};

struct unprocessed_text_source {
	std::variant<handle, formatted_text_reference> input_text;
	std::string_view var;

	unprocessed_text_source(std::variant<handle, formatted_text_reference> const& r, std::string_view v) : input_text(r), var(v) {
	}
	unprocessed_text_source(handle r) : input_text(r) {
	}
	unprocessed_text_source(formatted_text_reference r) : input_text(r) {
	}
};

struct metrics {
	uint32_t textPosition;
	uint32_t length;
	float left;
	float top;
	float width;
	float height;
	uint32_t bidiLevel;
};
struct hit_test_metrics {
	metrics metrics;
	bool is_inside;
	bool is_trailing;
};

enum class content_alignment : uint8_t {
	leading, trailing, centered, justified
};

enum class text_size : uint8_t {
	standard, note, header, label
};


enum class font_type {
	roman, sans, script, italic
};

struct font {
	native_string name;

	float span = 100.0f;
	int32_t weight = 400;
	int32_t top_leading = 0;
	int32_t bottom_leading = 0;
	float line_spacing = 0.0f; // derived
	float baseline = 0.0f; // derived
	float scale = 1.0f;
	float font_size = 0.0f; // derived
	float vertical_baseline = 0.0f; // derived
	float descender = 0.0f; // derived

	font_type type = font_type::roman;
	bool is_oblique = false;
};

struct unicode_range {
	uint32_t start = 0;
	uint32_t end = 0;
};

struct font_fallback {
	native_string name;
	std::vector<unicode_range> ranges;
	float scale = 1.0f;
};

struct locale_description {
	native_string display_name;
	bool is_left_to_right;
};

}

enum class input_mode {
	keyboard_only, mouse_only, controller_only, controller_with_pointer, mouse_and_keyboard, system_default, follow_input
};
enum class prompt_mode {
	hidden, keyboard, controller
};
enum class animation_type : uint8_t {
	fade, slide, flip, none
};
enum class animation_direction : uint8_t {
	top, right, left, bottom
};
struct screen_space_rect {
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
};

struct screen_space_point {
	int32_t x;
	int32_t y;
};

enum class layout_orientation : uint8_t {
	horizontal_left_to_right, horizontal_right_to_left
};

struct brush_color {
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;
};


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


enum class controller_button : uint8_t {
	y, x, b, a, lb, rb, start, back, tleft, tright, dup, ddown, dleft, dright, no_button
};

struct button_combination {
	controller_button first_button;
	controller_button second_button;
	bool sticky;
};

struct em {
	int16_t value = 0; // fixed point sizes in terms of 100ths of an em
	auto operator<=>(em const& o) const noexcept = default;
};
inline em operator+(em a, em b) noexcept {
	return em{ int16_t(a.value + b.value) };
}
inline em operator-(em a, em b) noexcept {
	return em{ int16_t(a.value - b.value) };
}
inline em operator+(em a, int32_t b) noexcept {
	return em{ int16_t(a.value + b * 100) };
}
inline em operator-(em a, int32_t b) noexcept {
	return em{ int16_t(a.value - b * 100) };
}
inline em operator*(em a, int32_t b) noexcept {
	return em{ int16_t(a.value * b) };
}
inline em operator/(em a, int32_t b) noexcept {
	return em{ int16_t(a.value / b) };
}
struct layout_position {
	em x;
	em y;
};
inline layout_position operator+(layout_position a, layout_position b) noexcept {
	return layout_position{ a.x + b.x, a.y + b.y };
}
inline layout_position operator-(layout_position a, layout_position b) noexcept {
	return layout_position{ a.x - b.x, a.y - b.y };
}

struct layout_rect {
	em x;
	em y;
	em width;
	em height;
};
inline layout_rect operator+(layout_rect a, layout_position b) noexcept {
	return layout_rect{ a.x + b.x, a.y + b.y, a.width, a.height };
}
inline layout_rect operator-(layout_rect a, layout_position b) noexcept {
	return layout_rect{ a.x - b.x, a.y - b.y, a.width, a.height };
}

struct animation_description {
	screen_space_rect animated_region;
	animation_type type = animation_type::none;
	animation_direction direction = animation_direction::top;
	float duration_seconds = 1.0f;
	bool animate_in = true;
};

struct animation_defintion {
	animation_type type = animation_type::none;
	float duration_seconds = 1.0f;
	bool animate_in = true;
};

struct animation_status_struct {
	animation_description description;

	std::chrono::time_point<std::chrono::steady_clock> start_time;
	bool is_running = false;

};

struct interactable_state {
private:
	uint8_t data = 0ui8;
	struct impl_key_type {
	};
	struct impl_group_type {
	};
	struct impl_group_start_type {
	};
public:
	constexpr static impl_group_type group{ };
	constexpr static impl_key_type key{ };

	interactable_state() : data(0ui8) {
	}
	interactable_state(impl_group_type, uint8_t v) {
		data = uint8_t((0x1F & v) | 0x40);
	}
	interactable_state(impl_key_type, uint8_t v) {
		data = uint8_t((0x1F & v) | 0x20);
	}
	int32_t get_key() const {
		return (0x1F & data);
	}
	bool holds_key() const {
		return (0xE0 & data) == 0x20;
	}
	bool holds_group() const {
		return (0xE0 & data) == 0x40;
	}
	bool holds_data() const {
		return (0xE0 & data) != 0x00;
	}
};

enum class interactable_orientation : uint8_t {
	left = 0, above = 1, right = 2, below = 3
};
enum class interactable_placement : uint8_t {
	internal, external, suppressed
};
struct interactable_definition {
	layout_position relative_position;
	interactable_orientation orientation = interactable_orientation::left;
	interactable_placement placement = interactable_placement::internal;
};

struct icon_handle {
	int32_t value = -1;
};
struct image_handle {
	int32_t value = -1;
};
struct sound_handle {
	int32_t value = -1;
};

class ui_node;
class system_interface;

enum class rendering_modifiers : uint8_t {
	none,
	highlighted,
	disabled
};


enum class iface : uint32_t {
	multitype_container,
	monotype_container,
	static_text,
	editable_text,
	control
};

class iface_base {
public:
	virtual iface get_type() const = 0;
};
class icontrol : public iface_base {
	iface get_type() const final {
		return iface::control;
	}
	virtual bool get_is_enabled() = 0;
	virtual void set_enabled(bool enabled) = 0;
};
class imultitype_container : public iface_base {
public:
	iface get_type() const final {
		return iface::multitype_container;
	}
	virtual void add_managed_element(root& r, ui_node* n) = 0;
	virtual void reset_managed_elements(root& r) = 0;
};
class imonotype_container : public iface_base {
public:
	iface get_type() const final {
		return iface::monotype_container;
	}
	virtual void add_item(void const* data) = 0;
	virtual void clear_contents() = 0;
};
class istatic_text : public iface_base {
public:
	struct mouse_test_result {
		uint32_t position;
		uint32_t length;
		bool inside;
		bool trailing;
	};

	iface get_type() const {
		return iface::static_text;
	}

	virtual text::formatted_text_reference view_text(system_interface&) const = 0;
	virtual void set_text(system_interface&, text::formatted_text&&) = 0;

	virtual text::content_alignment get_alignment() const = 0;
	virtual void set_alignment(system_interface&, text::content_alignment) = 0;

	virtual text::font_handle get_font() const = 0;
	virtual void set_font(system_interface&, text::font_handle) = 0;

	virtual bool get_is_multiline() const = 0;
	virtual void set_is_multiline(system_interface&, bool) = 0;

	virtual int32_t get_number_of_displayed_lines(system_interface&) = 0;
	virtual int32_t get_number_of_text_lines(system_interface&) = 0;
	virtual int32_t get_starting_display_line() = 0;
	virtual em get_single_line_width(system_interface&) = 0;
	virtual void set_starting_display_line(int32_t v) = 0;
	virtual em get_line_height(system_interface&) = 0;
	virtual void resize_to_width(system_interface& s, int32_t w) = 0;

	virtual void render(system_interface&, layout_rect, uint16_t brush, rendering_modifiers display_flags = rendering_modifiers::none, bool in_focus = false) = 0;

	virtual screen_space_rect get_character_bounds(system_interface&, uint32_t position) const = 0;
	virtual mouse_test_result get_position(system_interface&, screen_space_point pt) = 0;
};

enum class edit_contents : uint8_t {
	generic_text, number, single_char, email, date, time, url
};

class ieditable_text : public istatic_text {
public:
	struct detailed_mouse_test_result {
		uint32_t position;
		uint32_t quadrent;
	};

	iface get_type() const final {
		return iface::editable_text;
	}

	enum class edit_command : uint8_t {
		new_line, backspace, delete_char, backspace_word, delete_word, tab, cursor_down, cursor_up, cursor_left, cursor_right, cursor_left_word, cursor_right_word, to_line_start, to_line_end, to_text_start, to_text_end, cut, copy, paste, select_all, undo, redo, select_current_word, select_current_section, delete_selection
	};
	
	virtual edit_contents content_type() const = 0;
	virtual uint32_t get_cursor_position() const = 0;
	virtual uint32_t get_cursor_position_under_point(system_interface&, screen_space_point) = 0;
	virtual void set_cursor_position(system_interface&, uint32_t, bool extend_selection) = 0;
	virtual void insert_codepoint(system_interface&, uint32_t codepoint) = 0;
	virtual void send_command(system_interface&, edit_command, bool extend_selection) = 0;
	virtual void insert_text(system_interface&, uint32_t position_start, uint32_t position_end, native_string_view content) = 0;
	virtual uint32_t get_selection_anchor() const = 0;
	virtual void set_read_only(system_interface& s, bool is_read_only) = 0;
	virtual bool is_read_only() const = 0;
	virtual detailed_mouse_test_result get_detailed_position(system_interface&, screen_space_point pt) = 0;
	virtual void on_focus(system_interface&) = 0;
	virtual void on_lose_focus(system_interface&) = 0;
	virtual void move_cursor_by_screen_pt(system_interface& win, screen_space_point pt, bool extend_selection) = 0;
	virtual void consume_mouse_event(system_interface& win, int32_t x, int32_t y, uint32_t buttons);
};

class static_text_provider : public istatic_text {
public:
	virtual ~static_text_provider() = 0;
};
class editable_text_provider : public ieditable_text {
public:
	virtual ~editable_text_provider() = 0;
};

class file {
public:
	virtual char const* data() = 0;
	virtual size_t size() = 0;
	virtual native_string name() = 0;
	virtual ~file() { }
};

class directory {
public:
	virtual std::unique_ptr<file> open_file(native_string_view name) = 0;
	virtual std::unique_ptr<directory> open_directory(native_string_view name) = 0;
	virtual std::vector<std::unique_ptr<file>> list_files() = 0;
	virtual std::vector<std::unique_ptr<directory>> list_directories() = 0;
	virtual native_string name() = 0;
	virtual ~directory() { }
};

class system_interface {
public:
	virtual void register_root(root& r) = 0;

	// WINDOW FUNCTIONS
	virtual layout_position get_workspace() const = 0;
	virtual void hide_mouse_cursor() = 0;
	virtual void show_mouse_cursor() = 0;
	virtual bool is_mouse_cursor_visible() const = 0;
	virtual int32_t get_key_state(uint32_t scan_code) const = 0;
	virtual bool is_shift_held_down() const = 0;
	virtual bool is_ctrl_held_down() const = 0;
	virtual uint32_t get_window_dpi() const = 0;
	virtual bool is_maximized() const = 0;
	virtual bool is_minimized() const = 0;
	virtual void maximize() = 0;
	virtual void minimize() = 0;
	virtual void restore() = 0;
	virtual void close() = 0;
	virtual bool get_ltr() const = 0;
	virtual void set_window_title(native_char const* t) = 0;
	virtual bool window_has_focus() const = 0;

	virtual int32_t to_screen_space(em v) const = 0;
	virtual screen_space_point to_screen_space(layout_position p) const = 0;
	virtual screen_space_rect to_screen_space(layout_rect r) const = 0;
	virtual em to_ui_space(float v) const = 0;
	virtual layout_position to_ui_space(screen_space_point p) const = 0;
	virtual layout_rect to_ui_space(screen_space_rect r) const = 0;

	// SYSTEM FUNCTIONS
	virtual void display_fatal_error_message(native_string_view) = 0;
	virtual void text_to_clipboard(native_string_view) = 0;
	virtual native_string text_from_clipboard() = 0;

	// FILE FUNCTIONS
	virtual std::unique_ptr<directory> get_root_directory() = 0;

	// SOUND FUNCTIONS
	virtual void  load_sound(sound_handle, native_string_view file_name) = 0;
	virtual void play_sound(sound_handle) = 0;

	// LOCALE FUNCTION
	virtual void set_locale(native_string_view id) = 0;
	virtual void set_locale_name(native_string name) = 0;
	virtual void set_ltr_mode(bool is_ltr) = 0;
	virtual native_string_view get_locale_name() = 0;
	virtual native_string_view get_locale() = 0;

	// TEXT FUNCTIONS
	virtual std::unique_ptr<static_text_provider> make_text(ui_node&) = 0;
	virtual std::unique_ptr<editable_text_provider> make_editable_text(ui_node&) = 0;

	virtual void add_font_file_to_collection(native_string_view file_name) = 0;
	virtual void finalize_font_collection() = 0;
	virtual void make_font_definition(text::font_handle slot, text::font&& font_object) = 0;
	virtual void add_font_fallback(text::font_handle slot, text::font_fallback&& font_object) = 0;
	virtual void add_localization_file(native_string_view file_name) = 0;

	virtual text::handle get_hande(std::string_view key) = 0;
	virtual text::formatted_text fp_to_text(double fp, int32_t precision, bool show_plus = false) = 0;
	virtual text::formatted_text int_to_text(int64_t value, bool show_plus = false) = 0;
	virtual int64_t text_to_int(native_string_view text) = 0;
	virtual double text_to_double(native_string_view text) = 0;

	virtual text::formatted_text perform_substitutions(text::text_source body_text, text::text_source const* parameters, size_t parameter_count) = 0;
	virtual text::variable get_text_variable(std::string_view name) = 0;

	inline text::formatted_text perform_substitutions(text::text_source body_text, text::unprocessed_text_source const& p1) {
		std::array<text::text_source, 1> tr = { text::text_source(p1.input_text, get_text_variable(p1.var)) };
		return perform_substitutions(body_text, tr.data(), tr.size());
	}
	inline text::formatted_text perform_substitutions(text::text_source body_text, text::unprocessed_text_source const& p1, text::unprocessed_text_source const& p2) {
		std::array<text::text_source, 2> tr = { text::text_source(p1.input_text, get_text_variable(p1.var)), text::text_source(p2.input_text, get_text_variable(p2.var)) };
		return perform_substitutions(body_text, tr.data(), tr.size());
	}
	inline text::formatted_text perform_substitutions(text::text_source body_text, text::unprocessed_text_source const& p1, text::unprocessed_text_source const& p2, text::unprocessed_text_source const& p3) {
		std::array<text::text_source, 3> tr = { text::text_source(p1.input_text, get_text_variable(p1.var)), text::text_source(p2.input_text, get_text_variable(p2.var)), text::text_source(p3.input_text, get_text_variable(p3.var)) };
		return perform_substitutions(body_text, tr.data(), tr.size());
	}
	inline text::formatted_text perform_substitutions(text::text_source body_text, text::unprocessed_text_source const& p1, text::unprocessed_text_source const& p2, text::unprocessed_text_source const& p3, text::unprocessed_text_source const& p4) {
		std::array<text::text_source, 4> tr = { text::text_source(p1.input_text, get_text_variable(p1.var)), text::text_source(p2.input_text, get_text_variable(p2.var)), text::text_source(p3.input_text, get_text_variable(p3.var)), text::text_source(p4.input_text, get_text_variable(p4.var)) };
		return perform_substitutions(body_text, tr.data(), tr.size());
	}

	// GRAPHICS FUNCTIONS
	virtual void rectangle(screen_space_rect content_rect, rendering_modifiers display_flags, uint16_t brush) = 0;
	virtual void empty_rectangle(screen_space_rect content_rect, rendering_modifiers display_flags, uint16_t brush) = 0;
	virtual void line(screen_space_point start, screen_space_point end, float width, uint16_t brush) = 0;
	virtual void interactable(screen_space_point location, interactable_state state, uint16_t fg_brush, uint16_t hl_brush, uint16_t info_brush, uint16_t bg_brush, interactable_orientation o, rendering_modifiers display_flags = rendering_modifiers::none) = 0;
	virtual void image(image_handle img, screen_space_rect, int32_t sub_slot = 0) = 0;
	virtual void background(image_handle img, uint16_t brush, screen_space_rect, layout_rect interior, rendering_modifiers display_flags = rendering_modifiers::none, int32_t sub_slot = 0) = 0;
	virtual void icon(icon_handle ico, screen_space_rect, uint16_t br, rendering_modifiers display_flags = rendering_modifiers::none, int32_t sub_slot = 0) = 0;
	virtual void set_line_highlight_mode(bool highlight_on) = 0;

	
	virtual void stop_ui_animations() = 0;
	virtual void prepare_ui_animation() = 0;
	virtual void prepare_layered_ui_animation() = 0;
	virtual void start_ui_animation(animation_description description) = 0;
	virtual void register_in_place_animation() = 0;
	
	virtual layout_position get_icon_size(icon_handle ico) = 0;
	virtual void add_to_icon_slot(icon_handle slot, native_string_view file_name, em x_ems, em y_ems, int32_t sub_index) = 0;
	virtual void add_svg_to_icon_slot(icon_handle slot, native_string_view file_name, em x_ems, em y_ems, int32_t sub_index) = 0;
	virtual int32_t get_icon_set_size(icon_handle ico) = 0;
	virtual void add_to_image_slot(image_handle slot, native_string_view file_name, em x_ems, em y_ems, int32_t sub_index) = 0;
	virtual int32_t get_image_set_size(image_handle ico) = 0;

	virtual void add_color_brush(uint16_t id, brush_color c, bool as_disabled) = 0;
	virtual void add_image_color_brush(uint16_t id, native_string_view file_name, brush_color c, bool as_disabled) = 0;
	virtual void set_brush_highlights(uint16_t id, float line_shading, float highlight_shading, float line_highlight_shading) = 0;
};

namespace behavior {

constexpr inline uint32_t focus_free = 0x00000000; // lose focus if mouse leaves
constexpr inline uint32_t focus_lock_mouse = 0x00000001; // lose focus only if mouse clicks outside / esc key is hit
constexpr inline uint32_t focus_sticky_mouse = 0x00000002; // lose focus is mouse leaves, but retains focus while mouse is tracking to it
constexpr inline uint32_t focus_lock_text = 0x00000004; // as lock mouse, but may have additional restrictions as a text box

constexpr inline uint32_t transparent_to_focus = 0x00000008; // do not treat as a focusable element -- may have children
constexpr inline uint32_t pop_up = 0x00000010;

constexpr inline uint32_t visually_hidden = 0x00000020;
constexpr inline uint32_t functionally_hidden = 0x00000040;

constexpr inline uint32_t standard_background = 0x00000080;
constexpr inline uint32_t visually_interactable = 0x00000100;

constexpr inline uint32_t position_from_bottom = 0x00000200;
constexpr inline uint32_t position_from_right = 0x00000400;

constexpr inline uint32_t layout_start_group = 0x00000800; // won't be broken from group elements unless necessary, shares highlight
constexpr inline uint32_t layout_end_group = 0x00008000; // won't be broken from next element unless necessary, shares highlight
constexpr inline uint32_t layout_as_separator = 0x00001000; // wont appear as first element in a column, resets highlight
constexpr inline uint32_t layout_as_section_header = 0x00002000; // no line highlight and reset highlight
constexpr inline uint32_t layout_as_column_header = 0x00004000; // always starts a new column

constexpr inline uint32_t interaction_command = 0x00008000; // takes a click / primary action
constexpr inline uint32_t interaction_info = 0x00010000; // takes a right click / alt click / info action
constexpr inline uint32_t interaction_focus = 0x00020000; // takes a mouse hover / shift click / expand-or-take-focus action
constexpr inline uint32_t interaction_hold = 0x00040000; // wants to know if the click/interaction is sustained (wants a start and end message)

constexpr inline uint32_t interaction_flagged = 0x80000000; // set if the element is currently labeled with an interactable tag

}

struct postponed_render {
	ui_node* n;
	layout_position offset;
};

class root;

enum class mouse_interactivity {
	button = 0,
	scroll = 1,
	position = 2,
	focus_target = 3,
	count  = 4
};

struct page_information {
	uint16_t current_page = 0;
	uint16_t total_pages = 0;
};

struct probe_result {
	struct sub_result {
		ui_node* node = nullptr;
		layout_position relative_location;
	};
	std::array<sub_result, size_t(mouse_interactivity::count)> type_array;
};

class type_erased_vector {
public:
	virtual void const* operator[](size_t index) const = 0;
	virtual size_t size() const = 0;
	virtual void pop_back() = 0;
	virtual void push_back(void const* v) = 0;
	virtual void clear() = 0;
	virtual ~type_erased_vector() { }
};

template<typename T>
class type_erased_vector_impl : public type_erased_vector {
	std::vector<T> contents;
public:
	void const* operator[](size_t index) const final {
		return reinterpret_cast<void const*>(&contents[index]);
	}
	size_t size() const final {
		return contents.size();
	}
	void pop_back() final {
		contents.pop_back();
	}
	void push_back(void const* v) final {
		contents.push_back(*reinterpret_cast<T const*>(v));
	}
	void clear() final {
		contents.clear();
	}
};

class ui_node {
public:
	ui_node* parent = nullptr;

	uint32_t type_id = 0; // the type of control it is -- for looking up values that can be loaded at startup

	uint32_t behavior_flags = 0;

	layout_rect position;
	
	virtual size_t size() const = 0;
	virtual void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) = 0;
	virtual uint32_t child_count() const {
		return 0;
	}
	virtual ui_node* get_child(uint32_t index) const {
		return nullptr;
	}
	virtual page_information get_page_information() {
		return page_information{ 0, 0 };
	}
	virtual void change_page(root& r, int32_t new_page) { }
	
	virtual void force_resize(root& r, layout_position size) { // forces both dimensions
		position.width = size.x;
		position.height = size.y;
	}
	virtual void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) { // -1 for don't care
		if(desired_width.value != -1) position.width = desired_width;
		if(desired_height.value != -1) position.height = desired_height;
	}
	virtual em minimum_width(root& r) {
		return em{ 0 };
	}
	virtual em minimum_height(root& r) {
		return em{ 0 };
	}
	
	virtual probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) = 0;
	virtual interactable_definition interactable_layout(root& r) = 0;
	virtual void on_lbutton(root& r, layout_position pos) { }
	virtual void on_rbutton(root& r, layout_position pos) { }
	virtual void on_lbutton_up(root& r) { }
	virtual void on_rbutton_up(root& r) { }
	virtual void on_scroll(root& r, layout_position pos, int32_t amount) { }
	virtual void on_mouse_enter(root& r) { }
	virtual void on_mouse_leave(root& r) { }
	virtual void on_gain_focus(root& r) { }
	virtual void on_lose_focus(root& r) { }
	virtual void on_visible(root& r) { }
	virtual void on_hide(root& r) { }
	virtual void on_update(root& r) = 0;
	virtual void on_create(root& r) { }
	virtual void on_reload(root& r) { } // used to force a reload of text in case of system-wide font / locale changes
	virtual void on_text_update(root& r) { } // called after a modifying function is applied to the edit interface
	virtual iface_base* get_interface(iface v) {
		return nullptr;
	}
	
	virtual ~ui_node() { }

	
};


struct variable_definition {
	uint16_t variable;
	uint16_t data_type;
	uint16_t offset;
	uint8_t raw_data[8];
};
struct variable_definition_range {
	variable_definition const* start;
	variable_definition const* end;
};
struct relative_child_def {
	relative_to x_start_base;
	em x_start_offset;
	relative_to x_end_base;
	em x_end_offset;

	relative_to y_start_base;
	em y_start_offset;
	relative_to y_end_base;
	em y_end_offset;
};
struct relative_child_range {
	relative_child_def const* start;
	relative_child_def const* end;
};
struct background_definition {
	image_handle image; // if -1, use brush instead
	layout_rect exterior_edge_offsets;
	layout_rect texture_interior_region;
	uint16_t brush; // if -1, transparent
	// TODO: BORDERS
};
enum class column_layout : uint8_t {
	top, centered, bottom
};
struct column_properties {
	em minimum_width; // per column
	int8_t number_of_columns = 1;
	column_layout layout = column_layout::top;
	bool enhance_line_visibility = false;
};
struct page_ui_definitions {
	icon_handle page_icon;
	em scale = em{ 100 };
	uint16_t left_button = 0;
	uint16_t right_button = 0;
	uint16_t left2_button = 0;
	uint16_t right2_button = 0;
	uint16_t text = 0;
	bool vertical_arrangement = false;
};
struct text_information {
	layout_rect margins;

	text::font_handle font;
	text::handle default_text;
	text::content_alignment alginment;
	em minimum_space;
	bool multiline;
};
struct image_information {
	layout_rect margins;
	em fixed_vertical_size;
	em fixed_horizontal_size;
	text::content_alignment h_alginment;
	text::content_alignment v_alginment;
};

struct array_reference {
	uint32_t file_offset;
	uint32_t count;
};

using user_function = void (*)(root& r, ui_node&);

uint32_t defined_datatypes();
uint32_t datatype_size(uint32_t data_type_id);
void run_datatype_constructor(char* address, uint32_t data_type_id);
void run_datatype_destructor(char* address, uint32_t data_type_id);
std::unique_ptr<type_erased_vector> make_vector_of(uint32_t data_type_id);
user_function lookup_function(std::string_view name);

namespace impl { // to be wrapped in a generated hpp file mapping to the appropriate types
char* get_local_data(root& r, ui_node*, uint32_t data_type);
char const* get_local_data(root& r, ui_node const*, uint32_t data_type);
}

}