#include "minui_interfaces.hpp"
#include "minui_text_impl.hpp"
#include "stools.hpp"

#include <limits>
#include <algorithm>
#include <memory>

#ifndef UNICODE
#define UNICODE
#endif
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace minui {

#ifdef _WIN64
#define minui_aligned_alloc(X, Y) _aligned_malloc(X, Y)
#define minui_aligned_free(X) _aligned_free(X)
#else
#define minui_aligned_alloc(X, Y) std::aligned_alloc(Y, X)
#define minui_aligned_free(X) std::free(X)
#endif

class container_node : public ui_node {
public:
	std::vector<ui_node*> children;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override;
	ui_node* get_child(uint32_t index) const override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
};

struct type_range {
	uint32_t const* start;
	uint32_t const* end;
};

enum class relative_to : uint8_t {
	zero,
	all,
	half,
	one_third,
	two_thirds,
	one_fourth,
	three_fourths
};
inline float to_float(relative_to v) {
	switch(v) {
		case relative_to::zero: return 0.0f;
		case relative_to::all: return 1.0f;
		case relative_to::half: return 0.5f;
		case relative_to::one_third: return 1.0f / 3.0f;
		case relative_to::two_thirds: return 2.0f / 3.0f;
		case relative_to::one_fourth: return 0.25f;
		case relative_to::three_fourths: return 0.75f;
	}
}

class proportional_window : public ui_node {
public:
	std::vector<ui_node*> children;
	std::vector<relative_child_def> child_positions;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override;
	ui_node* get_child(uint32_t index) const override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
};


class space_filler : public ui_node {
public:
	std::vector<ui_node*> children;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override;
	ui_node* get_child(uint32_t index) const override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_update(root& r) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_create(root& r) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
};

class page_controls : public ui_node {
public:
	ui_node* left2_button = nullptr;
	ui_node* left_button = nullptr;
	ui_node* right_button = nullptr;
	ui_node* right2_button = nullptr;
	ui_node* text = nullptr;
	icon_handle page_icon = icon_handle{ -1 };
	bool vertical_arrangement = false;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override {
		return 4;
	}
	ui_node* get_child(uint32_t index) const override {
		ui_node* ar[4] = { left2_button, left_button , right_button, right2_button };
		return ar[index];
	}
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	interactable_definition interactable_layout(root& r) override;
};

class dynamic_column : public ui_node, imultitype_container {
public:
	std::vector<ui_node*> children;
	std::vector<uint16_t> page_starts;
	ui_node* page_controls = nullptr;

	uint16_t num_pages = 0;
	uint16_t current_page = 0;
	bool pending_relayout = false;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override;
	ui_node* get_child(uint32_t index) const override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void on_scroll(root& r, layout_position pos, int32_t amount) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	page_information get_page_information() override;
	void change_page(root& r, int32_t new_page) override;
	void add_managed_element(root& r, ui_node* n) override;
	void reset_managed_elements(root& r) override;
	iface_base* get_interface(iface v) override {
		if(v == iface::multitype_container)
			return static_cast<imultitype_container*>(this);
		return nullptr;
	}
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
	void repaginate(root& r);
};

class monotype_column : public ui_node, imonotype_container {
public:
	std::vector<ui_node*> children;
	std::unique_ptr<type_erased_vector> data;
	uint16_t page_size = 0;

	ui_node* page_controls = nullptr;

	uint16_t num_pages = 0;
	uint16_t current_page = 0;
	bool pending_data_update = true;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override;
	ui_node* get_child(uint32_t index) const override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void on_scroll(root& r, layout_position pos, int32_t amount) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	page_information get_page_information() override;
	void change_page(root& r, int32_t new_page) override;
	void add_item(void const* data) override;
	void clear_contents() override;
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
	iface_base* get_interface(iface v) override {
		if(v == iface::monotype_container)
			return static_cast<imonotype_container*>(this);
		return nullptr;
	}
};

class panes_set : public ui_node {
public:
	std::vector<ui_node*> children;
	uint16_t selected = 0;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override;
	ui_node* get_child(uint32_t index) const override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	page_information get_page_information() override;
	void change_page(root& r, int32_t new_page) override;
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
};

class layers : public ui_node {
public:
	std::vector<ui_node*> children;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override;
	ui_node* get_child(uint32_t index) const override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_create(root& r) override;
	void on_update(root& r) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
};

class dynamic_grid : public ui_node, imultitype_container {
public:
	std::vector<ui_node*> children;
	std::vector<uint16_t> page_starts;
	ui_node* page_controls = nullptr;

	uint16_t num_pages = 0;
	uint16_t current_page = 0;
	bool pending_relayout = false;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	uint32_t child_count() const override;
	ui_node* get_child(uint32_t index) const override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void on_scroll(root& r, layout_position pos, int32_t amount) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	page_information get_page_information() override;
	void change_page(root& r, int32_t new_page) override;
	void add_managed_element(root& r, ui_node* n) override;
	void reset_managed_elements(root& r) override;
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
	iface_base* get_interface(iface v) override {
		if(v == iface::multitype_container)
			return static_cast<imultitype_container*>(this);
		return nullptr;
	}
	void repaginate(root& r);
};

class static_text : public ui_node {
public:
	std::unique_ptr<static_text_provider> text_data;

	layout_rect margins;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	void on_reload(root& r) override;
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
	iface_base* get_interface(iface v) override {
		if(v == iface::static_text)
			return text_data.get();
		return nullptr;
	}
};

class text_button : public ui_node, icontrol {
public:
	std::unique_ptr<static_text_provider> text_data;

	layout_rect margins;
	bool enabled = true;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void on_lbutton(root& r, layout_position pos) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	void on_reload(root& r) override;
	interactable_definition interactable_layout(root& r) override {
		return interactable_definition{ r.get_icon_position(type_id), interactable_orientation::left, interactable_placement::internal };
	}
	iface_base* get_interface(iface v) override {
		if(v == iface::static_text)
			return text_data.get();
		else if(v == iface::control)
			return static_cast<icontrol*>(this);
		return nullptr;
	}
	bool get_is_enabled() override {
		return enabled;
	}
	void set_enabled(bool e) override {
		enabled = e;
	}
};

class icon_button : public ui_node, icontrol {
public:

	layout_rect icon_position;
	bool enabled = true;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void on_lbutton(root& r, layout_position pos) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;
	void recalculate_icon_position(root& r);
	interactable_definition interactable_layout(root& r) override {
		return r.get_interactable_definition(ui_node::type_id);
	}
	iface_base* get_interface(iface v) override {
		if(v == iface::control)
			return static_cast<icontrol*>(this);
		return nullptr;
	}
	bool get_is_enabled() override {
		return enabled;
	}
	void set_enabled(bool e) override {
		enabled = e;
	}
};

class edit_control : public ui_node {
public:
	std::unique_ptr<ieditable_text> text_data;

	layout_rect margins;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_visible(root& r) override;
	void on_hide(root& r) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void on_reload(root& r) override;
	void force_resize(root& r, layout_position size) override;
	void resize(root& r, layout_position maximum_space, em desired_width, em desired_height) override;
	em minimum_width(root& r) override;
	em minimum_height(root& r) override;

	void on_gain_focus(root& r) override;
	void on_lose_focus(root& r) override;
	void on_text_update(root& r) override;
	interactable_definition interactable_layout(root& r) override {
		if(r.contains_focus(this))
			return interactable_definition{ layout_position{ em{ 0 }, em{ 0 } }, interactable_orientation::left, interactable_placement::suppressed };
		else
			return interactable_definition{ r.get_icon_position(type_id), interactable_orientation::left, interactable_placement::internal };
	}
	iface_base* get_interface(iface v) override {
		if(v == iface::static_text)
			return static_cast<istatic_text*>(text_data.get());
		else if(v == iface::editable_text)
			return text_data.get();
		return nullptr;
	}
};

class page_control_icon_button : public ui_node, icontrol {
public:
	constexpr static uint32_t left_type  = 0x10;
	constexpr static uint32_t left2_type = 0x20;
	constexpr static uint32_t right_type = 0x30;
	constexpr static uint32_t right2_type = 0x40;
	constexpr static uint32_t type_mask = 0xF0;

	constexpr static uint32_t top = 0x01;
	constexpr static uint32_t bottom = 0x02;
	constexpr static uint32_t right = 0x03;
	constexpr static uint32_t position_mask = 0x0F;

	bool enabled = true;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_update(root& r) override;
	void on_lbutton(root& r, layout_position pos) override;
	interactable_definition interactable_layout(root& r) override;

	iface_base* get_interface(iface v) override {
		if(v == iface::control)
			return static_cast<icontrol*>(this);
		return nullptr;
	}
	bool get_is_enabled() override {
		return enabled;
	}
	void set_enabled(bool) override {
	}
};
class page_control_text : public ui_node {
public:
	constexpr static uint32_t horizontal = 0x00;
	constexpr static uint32_t vertical = 0x01;

	std::unique_ptr<static_text_provider> text_data;

	size_t size() const override;
	void render(root& r, layout_position offset, std::vector<postponed_render>& postponed) override;
	probe_result mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) override;
	void on_update(root& r) override;
	void on_create(root& r) override;
	void on_reload(root& r) override;
	void force_resize(root& r, layout_position size) override;
	interactable_definition interactable_layout(root& r) override {
		return interactable_definition{ r.get_icon_position(type_id), interactable_orientation::left, interactable_placement::suppressed };
	}
	iface_base* get_interface(iface v) override {
		if(v == iface::static_text)
			return text_data.get();
		return nullptr;
	}
};

class ui_node_disposal {
public:
	void operator()(ui_node* n) const {
		n->~ui_node();
		minui_aligned_free(n);
	}
};

struct stored_focus {
	ui_node* l_interface = nullptr;
	int32_t child_offset = 0;
	int32_t child_offset_end = 0;
	em tracking_margin = em{ 0 };
};
struct grouping_range {
	int32_t start;
	int32_t end;
};


struct placed_interactable {
	ui_node* element;
	layout_rect placement;
	interactable_state state;
	interactable_orientation orientation;
};

struct focus_tracker {
	ui_node* node = nullptr;
	int32_t child_offset = 0;
	int32_t child_offset_end = 0;
};
struct move_focus_to_node {
	ui_node* node = nullptr;
};
struct interaction {
	ui_node* node = nullptr;
};
struct go_up {
};

using key_action = std::variant<std::monostate, focus_tracker, move_focus_to_node, interaction, go_up>;

struct key_actions {
	key_action escape;
	std::array<key_action, 12> button_actions;
	int32_t valid_key_action_count = 0;
};

enum class click_type {
	singlec, doublec, triplec
};
enum class resize_type {
	minimize, maximize, normal
};

void null_user_function(root&, ui_node&) {
}

class root {
private:
	void load_font_definitions(char const* file_data, size_t file_size);
	text::locale_description load_locale_description(char const* file_data, size_t file_size);
public:
	probe_result under_mouse;

	std::vector<std::unique_ptr<ui_node, ui_node_disposal>> node_repository;
	std::vector<std::vector<ui_node*>> free_nodes;

	//
	// focus info
	//

	std::vector<stored_focus> focus_stack;
	std::array<grouping_range, 12> current_focus_groupings;
	int32_t current_groupings_size = 0;
	key_actions focus_actions;
	std::vector<placed_interactable> current_interactables;
	prompt_mode pmode = prompt_mode::hidden;
	bool shift_down = false;
	bool alt_down = false;
	enum class mcommand {
		primary, alt, none
	} last_mcommand_sent = mcommand::none;
	ui_node* last_mcommand_target = nullptr;
	ieditable_text* edit_target = nullptr;

	layout_position latest_mouse_position;

	system_interface& system;
	layout_position workspace;
	

	root(system_interface& system, layout_position initial_size) : system(system), workspace(initial_size) {
		system.register_root(*this);
		// make 0 the root
	}

	void render();

	ui_node* take_key_action(key_action a); // returns a node if a non-group node is selected
	void change_focus(ui_node* old_focus, ui_node* new_focus);
	void set_window_focus(focus_tracker a);
	void repopulate_key_actions();
	void set_prompt_mode(prompt_mode p) {
		pmode = p;
	}

	void on_workspace_resized(resize_type, layout_position);
	bool on_char(native_char);
	bool on_mouse_move(layout_position);
	bool on_mouse_lbutton(click_type);
	bool on_mouse_rbutton();
	bool on_mouse_lbutton_up();
	bool on_mouse_rbutton_up();
	bool on_mouse_scroll(float amount);
	bool on_key_down(uint32_t scancode, uint32_t vk_code, bool repeat);
	bool on_key_up(uint32_t scancode, uint32_t vk_code);
	void on_update();

	bool contains_focus(ui_node const*);
	void back_out_focus(ui_node&);
	layout_position workspace_placement(ui_node&);

	ui_node* make_control_by_type(ui_node* parent, uint32_t type);
	void release_node(ui_node* n);

	void load_locale_data(native_string_view locale);
	em minimum_width();
	em minimum_height();

	// 
	// data storage
	//


	char const* file_base = nullptr;
	std::unique_ptr<file> defintions_file;
	uint32_t defined_element_types = 0;

	std::span<const layout_position> d_icon_position;
	std::span<const layout_rect> d_default_position;
	std::span<const interactable_definition> d_interactable_definition;
	std::span<const icon_handle> d_icon;
	std::span<const uint16_t> d_class;
	std::span<const uint32_t> d_standard_flags;
	std::span<const uint16_t> d_foreground_brush;
	std::span<const uint16_t> d_background_brush;
	std::span<const uint16_t> d_highlight_brush;
	std::span<const uint16_t> d_info_brush;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_fixed_children;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_window_children;
	std::span<const array_reference> d_variable_definition;
	std::span<const uint16_t> d_total_variable_size;
	std::span<const background_definition> d_background_definition;
	ankerl::unordered_dense::map_view<uint32_t, const int32_t> d_divider_index;
	ankerl::unordered_dense::map_view<uint32_t, const bool> d_horizontal_orientation;
	ankerl::unordered_dense::map_view<uint32_t, const column_properties> d_column_properties;
	ankerl::unordered_dense::map_view<uint32_t, const page_ui_definitions> d_page_ui_definitions;
	ankerl::unordered_dense::map_view<uint32_t, const text_information> d_text_information;
	ankerl::unordered_dense::map_view<uint32_t, const sound_handle> d_interaction_sound;
	ankerl::unordered_dense::map_view<uint32_t, const image_information> d_image_information;
	ankerl::unordered_dense::map_view<uint32_t, const child_data_type> d_child_data_type;

	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_on_update_raw;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_on_gain_focus_raw;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_on_lose_focus_raw;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_on_visible_raw;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_on_hide_raw;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_on_create_raw;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_user_fn_a_raw;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_user_fn_b_raw;
	ankerl::unordered_dense::map_view<uint32_t, const array_reference> d_user_mouse_fn_a_raw;

	std::vector<user_function> d_on_update;
	std::vector<user_function> d_on_gain_focus;
	std::vector<user_function> d_on_lose_focus;
	std::vector<user_function> d_on_visible;
	std::vector<user_function> d_on_hide;
	std::vector<user_function> d_on_create;
	std::vector<user_function> d_user_fn_a;
	std::vector<user_function> d_user_fn_b;
	std::vector<user_function> d_user_mouse_fn_a;

	

	//
	// element definitions
	//

	void load_definitions_from_file(std::unique_ptr<file> df) {
		defintions_file = std::move(df);
		load_definitions(df->data(), df->size());
	}
	void load_definitions(char const* data, size_t size);
	layout_position get_icon_position(uint32_t type_id) const {
		return d_icon_position[type_id];
	}
	layout_rect get_default_position(uint32_t type_id) const {
		return d_default_position[type_id];
	}
	interactable_definition get_interactable_definition(uint32_t type_id) const {
		return d_interactable_definition[type_id];
	}
	icon_handle get_icon(uint32_t type_id) const {
		return d_icon[type_id];
	}
	uint32_t get_class(uint32_t type_id) const {
		return d_class[type_id];
	}
	uint32_t get_standard_flags(uint32_t type_id) const {
		return d_standard_flags[type_id];
	}
	uint16_t get_foreground_brush(uint32_t type_id) const {
		return d_foreground_brush[type_id];
	}
	uint16_t get_background_brush(uint32_t type_id) const {
		return d_background_brush[type_id];
	}
	uint16_t get_highlight_brush(uint32_t type_id) const {
		return d_highlight_brush[type_id];
	}
	uint16_t get_info_brush(uint32_t type_id) const {
		return d_info_brush[type_id];
	}
	type_range get_fixed_children(uint32_t type_id) const {
		auto r = d_fixed_children.atomic_find(type_id);
		if(r) {
			array_reference t = *r;
			return type_range{ (uint32_t const*)(file_base + t.file_offset), (uint32_t const*)(file_base + t.file_offset) + t.count };
		} else {
			return type_range{ nullptr, nullptr };
		}
	}
	relative_child_range get_window_children(uint32_t type_id) const {
		auto r = d_fixed_children.atomic_find(type_id);
		if(r) {
			array_reference t = *r;
			return relative_child_range{ (relative_child_def const*)(file_base + t.file_offset), (relative_child_def const*)(file_base + t.file_offset) + t.count };
		} else {
			return relative_child_range{ nullptr, nullptr };
		}
	}
	variable_definition_range get_variable_definition(uint32_t type_id) const {
		auto t = d_variable_definition[type_id];
		return variable_definition_range{ (variable_definition const*)(file_base + t.file_offset), (variable_definition const*)(file_base + t.file_offset) + t.count };
	}
	uint32_t get_total_variable_size(uint32_t type_id) const {
		return d_total_variable_size[type_id];
	}
	background_definition get_background_definition(uint32_t type_id) const {
		return d_background_definition[type_id];
	}
	int32_t get_divider_index(uint32_t type_id) const {
		auto r = d_divider_index.atomic_find(type_id);
		if(r)
			return *r;
		else 
			return 0;
	}
	bool get_horizontal_orientation(uint32_t type_id) const {
		auto r = d_horizontal_orientation.atomic_find(type_id);
		if(r)
			return *r;
		else
			return false;
	}
	column_properties get_column_properties(uint32_t type_id) const {
		auto r = d_column_properties.atomic_find(type_id);
		if(r)
			return *r;
		else
			return column_properties{ };
	}
	page_ui_definitions get_page_ui_definitions(uint32_t type_id) const {
		auto r = d_page_ui_definitions.atomic_find(type_id);
		if(r)
			return *r;
		else
			return page_ui_definitions{ };
	}
	text_information get_text_information(uint32_t type_id) const {
		auto r = d_text_information.atomic_find(type_id);
		if(r)
			return *r;
		else
			return text_information{ };
	}
	sound_handle get_interaction_sound(uint32_t type_id) const {
		auto r = d_interaction_sound.atomic_find(type_id);
		if(r)
			return *r;
		else
			return sound_handle{ };
	}
	image_information get_image_information(uint32_t type_id) {
		auto r = d_image_information.atomic_find(type_id);
		if(r)
			return *r;
		else
			return image_information{ };
	}
	child_data_type get_child_data_type(uint32_t type_id) {
		auto r = d_child_data_type.atomic_find(type_id);
		if(r)
			return *r;
		else
			return child_data_type{ uint16_t(0), uint16_t(0), uint16_t(0) };
	}

	user_function get_on_update(uint32_t type_id) {
		if(auto f = d_on_update[type_id]; f) {
			return f;
		} else if(auto fr = d_on_update_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_on_update[type_id] = g;
			return g;
		} else {
			d_on_update[type_id] = null_user_function;
			return null_user_function;
		}
	}
	user_function get_on_gain_focus(uint32_t type_id) {
		if(auto f = d_on_gain_focus[type_id]; f) {
			return f;
		} else if(auto fr = d_on_gain_focus_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_on_gain_focus[type_id] = g;
			return g;
		} else {
			d_on_gain_focus[type_id] = null_user_function;
			return null_user_function;
		}
	}
	user_function get_on_lose_focus(uint32_t type_id) {
		if(auto f = d_on_lose_focus[type_id]; f) {
			return f;
		} else if(auto fr = d_on_lose_focus_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_on_lose_focus[type_id] = g;
			return g;
		} else {
			d_on_lose_focus[type_id] = null_user_function;
			return null_user_function;
		}
	}
	user_function get_on_visible(uint32_t type_id) {
		if(auto f = d_on_visible[type_id]; f) {
			return f;
		} else if(auto fr = d_on_visible_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_on_visible[type_id] = g;
			return g;
		} else {
			d_on_visible[type_id] = null_user_function;
			return null_user_function;
		}
	}
	user_function get_on_hide(uint32_t type_id) {
		if(auto f = d_on_hide[type_id]; f) {
			return f;
		} else if(auto fr = d_on_hide_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_on_hide[type_id] = g;
			return g;
		} else {
			d_on_hide[type_id] = null_user_function;
			return null_user_function;
		}
	}
	user_function get_on_create(uint32_t type_id) {
		if(auto f = d_on_create[type_id]; f) {
			return f;
		} else if(auto fr = d_on_create_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_on_create[type_id] = g;
			return g;
		} else {
			d_on_create[type_id] = null_user_function;
			return null_user_function;
		}
	}
	user_function get_user_fn_a(uint32_t type_id) {
		if(auto f = d_user_fn_a[type_id]; f) {
			return f;
		} else if(auto fr = d_user_fn_a_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_user_fn_a[type_id] = g;
			return g;
		} else {
			d_user_fn_a[type_id] = null_user_function;
			return null_user_function;
		}
	}
	user_function get_user_fn_b(uint32_t type_id) {
		if(auto f = d_user_fn_b[type_id]; f) {
			return f;
		} else if(auto fr = d_user_fn_b_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_user_fn_b[type_id] = g;
			return g;
		} else {
			d_user_fn_b[type_id] = null_user_function;
			return null_user_function;
		}
	}
	user_function get_user_mouse_fn_a(uint32_t type_id) {
		if(auto f = d_user_mouse_fn_a[type_id]; f) {
			return f;
		} else if(auto fr = d_user_mouse_fn_a_raw.atomic_find(type_id); fr) {
			auto g = lookup_function(std::string_view(file_base + (*fr).file_offset, (*fr).count));
			if(!g)
				g = null_user_function;
			d_user_mouse_fn_a[type_id] = g;
			return g;
		} else {
			d_user_mouse_fn_a[type_id] = null_user_function;
			return null_user_function;
		}
	}

	void initialize_members(ui_node*) const;
	void destroy_members(ui_node*) const;

	~root() {
		for(auto& n : node_repository) {
			destroy_members(n.get());
		}
	}
};

namespace impl {
char* get_local_data(root& r, ui_node* n, uint32_t variable) {
	int32_t offset = -1;
	auto vrang = r.get_variable_definition(n->type_id);
	for(auto i = vrang.start; i != vrang.end; ++i) {
		if(i->variable == variable) {
			offset = (i->offset & 0x7FFF);
			break;
		}
	}

	if(offset < 0)
		return nullptr;

	char* addr = reinterpret_cast<char*>(n);
	auto data = addr + n->size();
	auto v = data + 8 * offset;

	return v;
}
char const* get_local_data(root& r, ui_node const* n, uint32_t variable) {
	int32_t offset = -1;
	auto vrang = r.get_variable_definition(n->type_id);
	for(auto i = vrang.start; i != vrang.end; ++i) {
		if(i->variable == variable) {
			offset = (i->offset & 0x7FFF);
			break;
		}
	}

	if(offset < 0)
		return nullptr;

	char const* addr = reinterpret_cast<char const*>(n);
	auto data = addr + n->size();
	auto v = data + 8 * offset;

	return v;
}
}

void root::initialize_members(ui_node* n) const {
	char* address = reinterpret_cast<char*>(n);
	auto data = address + n->size();

	auto type = n->type_id;
	auto members = get_variable_definition(type);
	for(auto i = members.start; i != members.end; ++i) {
		if((i->offset & 0x8000) == 0)
			run_datatype_constructor(data + (i->offset & 0x7FFF) * 8, i->data_type);
		else
			memcpy(data + (i->offset & 0x7FFF) * 8, i->raw_data, 8);
	}
}
void root::destroy_members(ui_node* n) const {
	char* address = reinterpret_cast<char*>(n);
	auto data = address + n->size();

	auto type = n->type_id;
	auto type = n->type_id;
	auto members = get_variable_definition(type);
	for(auto i = members.start; i != members.end; ++i) {
		run_datatype_destructor(data + (i->offset & 0x7FFF) * 8, i->data_type);
	}
}

void root::release_node(ui_node* n) {
	back_out_focus(*n);
	n->parent = nullptr;
	auto& free_stock = free_nodes[n->type_id];
	free_stock.push_back(n);
}

ui_node* root::make_control_by_type( ui_node* parent, uint32_t type) {
	ui_node* result = nullptr;

	auto& free_stock = free_nodes[type];
	if(!free_stock.empty()) {
		result = free_stock.back();
		free_stock.pop_back();
		result->parent = parent;
		result->behavior_flags = get_standard_flags(type);
		result->position = get_default_position(type);
		destroy_members(result);
		initialize_members(result);
		result->force_resize(*this, layout_position{ result->position.width, result->position.height });
		result->on_update(*this);
		return result;
	}

	auto class_id = get_class(type);
	char* raw_data = nullptr;
	auto var_size = get_total_variable_size(type) * 8;

	switch(class_id) {
		case 0: // unstructured container
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(container_node) + var_size, alignof(container_node)));
			memset(raw_data, 0, sizeof(container_node) + var_size);
			new (raw_data)container_node();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		case 1: // proportional window
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(proportional_window) + var_size, alignof(proportional_window)));
			memset(raw_data, 0, sizeof(proportional_window) + var_size);
			new (raw_data)proportional_window();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 2: // space filler
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(space_filler) + var_size, alignof(space_filler)));
			memset(raw_data, 0, sizeof(space_filler) + var_size);
			new (raw_data)space_filler();
			space_filler* ptr = reinterpret_cast<space_filler*>(raw_data);
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 3: // dynamic_column
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(dynamic_column) + var_size, alignof(dynamic_column)));
			memset(raw_data, 0, sizeof(dynamic_column) + var_size);
			new (raw_data)dynamic_column();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 4: // page_controls
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(page_controls) + var_size, alignof(page_controls)));
			memset(raw_data, 0, sizeof(page_controls) + var_size);
			new (raw_data)page_controls();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 5: // monotype_column
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(monotype_column) + var_size, alignof(monotype_column)));
			memset(raw_data, 0, sizeof(monotype_column) + var_size);
			new (raw_data)monotype_column();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 6: // panes_set
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(panes_set) + var_size, alignof(panes_set)));
			memset(raw_data, 0, sizeof(panes_set) + var_size);
			new (raw_data)panes_set();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 7: // layers
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(layers) + var_size, alignof(layers)));
			memset(raw_data, 0, sizeof(layers) + var_size);
			new (raw_data)layers();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 8: // dynamic_grid
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(dynamic_grid) + var_size, alignof(dynamic_grid)));
			memset(raw_data, 0, sizeof(dynamic_grid) + var_size);
			new (raw_data)dynamic_grid();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 9: // static_text
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(static_text) + var_size, alignof(static_text)));
			memset(raw_data, 0, sizeof(static_text) + var_size);
			new (raw_data)static_text();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 10: // text_button
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(text_button) + var_size, alignof(text_button)));
			memset(raw_data, 0, sizeof(text_button) + var_size);
			new (raw_data)text_button();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 11: // icon_button
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(icon_button) + var_size, alignof(icon_button)));
			memset(raw_data, 0, sizeof(icon_button) + var_size);
			new (raw_data)icon_button();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 12: // edit_control
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(edit_control) + var_size, alignof(edit_control)));
			memset(raw_data, 0, sizeof(edit_control) + var_size);
			new (raw_data)edit_control();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 13: // page_control_icon_button
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(page_control_icon_button) + 8, alignof(page_control_icon_button)));
			memset(raw_data, 0, sizeof(page_control_icon_button) + 8);
			new (raw_data)page_control_icon_button();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
		case 14: //page_control_text
		{
			raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(page_control_text) + 8, alignof(page_control_text)));
			memset(raw_data, 0, sizeof(page_control_text) + 8);
			new (raw_data)page_control_text();
			result = reinterpret_cast<ui_node*>(raw_data);
			break;
		}
	}

	node_repository.push_back(std::unique_ptr<ui_node, ui_node_disposal>{result});
	result->type_id = type;
	result->parent = parent;
	result->behavior_flags = get_standard_flags(type);
	result->position = get_default_position(type);
	initialize_members(result);
	result->on_create(*this);
	
	return result;
}

ui_node* make_page_controls(root& r, ui_node* parent) {
	ui_node* result = nullptr;

	char* raw_data = nullptr;
	raw_data = reinterpret_cast<char*>(minui_aligned_alloc(sizeof(page_controls), alignof(page_controls)));
	memset(raw_data, 0, sizeof(page_controls));
	new (raw_data)page_controls();
	page_controls* ptr = reinterpret_cast<page_controls*>(raw_data);
	result = reinterpret_cast<ui_node*>(raw_data);

	r.node_repository.push_back(std::unique_ptr<ui_node, ui_node_disposal>{result});
	result->type_id = 0;
	result->parent = parent;
	result->behavior_flags = 0;
	result->position = layout_rect{ em{ 0 }, em{ 0 }, em{ 0 }, em{ 0 } };
	result->on_create(r);

	return result;
}

layout_position get_sub_position(ui_node& parent, ui_node& child) {
	layout_position result;
	if((child.behavior_flags & behavior::position_from_right) != 0) {
		result.x = parent.position.width + child.position.x;
	} else {
		result.x = child.position.x;
	}
	if((child.behavior_flags & behavior::position_from_bottom) != 0) {
		result.y = parent.position.height + child.position.y;
	} else {
		result.y = child.position.y;
	}
	return result;
}

void root::render() {
	std::vector<postponed_render> pop_ups;
	node_repository[0]->render(*this, layout_position{ em{ 0 }, em{ 0 } }, pop_ups);
	for(uint32_t i = 0; i < pop_ups.size(); ++i) {
		pop_ups[i].n->render(*this, pop_ups[i].offset, pop_ups);
	}

	// render interactables
	if(pmode != prompt_mode::hidden) {
		for(auto& i : current_interactables) {
			auto l = system.to_screen_space(i.placement);
			system.interactable(screen_space_point{ l.x, l.y }, i.state, get_foreground_brush(i.element->type_id), get_highlight_brush(i.element->type_id), get_info_brush(i.element->type_id), get_background_brush(i.element->type_id), i.orientation, rendering_modifiers::none);
		}
	}
}

ui_node* top_focus(std::vector<stored_focus> const& vec) {
	if(!vec.empty()) 
		return vec.back().l_interface;
	return nullptr;
}

ui_node const* find_common_root(ui_node const* a, ui_node const* b) {
	if(!b)
		return nullptr;

	for(ui_node const* outer = a; outer; outer = outer->parent) {
		for(ui_node const* inner = b; inner; inner = inner->parent) {
			if(inner == outer)
				return inner;
		}
	}

	return nullptr;
}

void root::change_focus(ui_node* old_focus, ui_node* new_focus) {
	if(new_focus == old_focus)
		return;

	auto common_root = find_common_root(new_focus, old_focus);

	for(ui_node* losing = old_focus; losing && losing != common_root; losing = losing->parent) {
		losing->on_lose_focus(*this);
	}

	for(ui_node* gaining = new_focus; gaining && gaining != common_root; gaining = gaining->parent) {
		gaining->on_gain_focus(*this);
	}
}

bool root::contains_focus(ui_node const* n) {
	auto tf = top_focus(focus_stack);
	while(tf != nullptr) {
		if(tf == n)
			return true;
		tf = tf->parent;
	}
	return false;
}
void root::back_out_focus(ui_node& n) {
	if(!contains_focus(&n)) {
		return;
	}

	while(!focus_stack.empty()) {
		if(contains_focus(&n)) {
			if(top_focus(focus_stack) == &n)
				return;
			take_key_action(go_up{ });
		} else {
			set_window_focus(focus_tracker{ &n, -1, -1 });
			return;
		}
	}

	set_window_focus(focus_tracker{ &n, -1, -1 });
}

struct sub_items_iterator {
	struct node_position {
		ui_node const* n;
		int32_t child_position;
		int32_t child_max;
	};

	std::vector<node_position> iteration_stack;

	void advance_iterator_status() {
		if(iteration_stack.empty())
			return;
		
		int32_t* back_pos = nullptr;
		auto current_node = iteration_stack.back().n;
		
		bool terminated_search;
		do {
			++(*back_pos);
			terminated_search = false;
			if(*back_pos >= iteration_stack.back().child_max) {
				iteration_stack.pop_back();
				back_pos = &(iteration_stack.back().child_position);
				current_node = iteration_stack.back().n;
			} else if(auto c = current_node->get_child(*back_pos); c) {
				if((c->behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) == 0)
					terminated_search = false;
				if((c->behavior_flags & behavior::transparent_to_focus) != 0) {
					terminated_search = false;
					iteration_stack.push_back(node_position{ c, -1 , int32_t(c->child_count()) });
					back_pos = &(iteration_stack.back().child_position);
					current_node = c;
				} else if((c->behavior_flags & (behavior::interaction_info | behavior::interaction_command | behavior::interaction_focus)) != 0) {
					terminated_search = true;
				} else if(c->child_count() > 0) {
					terminated_search = true;
				}
			}
		} while(!terminated_search && !iteration_stack.empty());
	}

	sub_items_iterator(ui_node const* n) {
		if(n) {
			iteration_stack.push_back(node_position{ n, -1, int32_t(n->child_count()) });
			advance_iterator_status();
		}
	}
	sub_items_iterator() {
	}
	bool operator==(sub_items_iterator const& o) const {
		return iteration_stack == o.iteration_stack;
	}
	bool operator!=(sub_items_iterator const& o) const {
		return !(*this == o);
	}
	ui_node* operator*() const {
		if(iteration_stack.empty())
			return nullptr;
		else
			return iteration_stack.back().n->get_child(uint32_t(iteration_stack.back().child_position));
	}
	sub_items_iterator& operator++() {
		advance_iterator_status();
		return *this;
	}
	sub_items_iterator& operator+=(int32_t n) {
		while(n > 0) {
			++(*this);
			--n;
		}
		return *this;
	}
};

struct sub_items_iterator_generator {
	ui_node const* n;

	sub_items_iterator_generator(ui_node const* n) : n(n) {
	}
	sub_items_iterator begin() {
		return sub_items_iterator(n);
	}
	sub_items_iterator end() {
		return sub_items_iterator();
	}
};

sub_items_iterator_generator interactables(ui_node const& n) {
	return sub_items_iterator_generator(&n);
}

int32_t calaculate_interactables_at_node(root& lm, ui_node const& n) {
	int32_t sum = 0;
	for(auto i : interactables(n)) {
		++sum;
	}
	return sum;
}

std::array<grouping_range, 12> divide_group(grouping_range i, int32_t into) {
	std::array<grouping_range, 12> result = { };

	auto i_size = i.end - i.start;
	auto each = (i_size + into - 1) / into;
	for(int32_t j = 0; j < into; ++j) {
		result[j].start = std::min(i.start + each * j, i.end);
		result[j].end = std::min(i.start + each * (j + 1), i.end);
	}

	return result;
}


int32_t make_top_groups(root& lm, ui_node const& n, grouping_range* out) {
	if(n.child_count() != 0) {
		auto total_interactables = calaculate_interactables_at_node(lm, n);

		auto groups = divide_group(grouping_range{ 0, total_interactables }, 12);

		int32_t i = 0;

		for(int32_t j = 0; j < 12; ++j) {
			if(groups[j].start != groups[j].end) {
				out[i] = grouping_range{ groups[j].start, groups[j].end };
				++i;
			}
		}

		return i;
	} else {
		return 0;
	}
}

std::optional<grouping_range> find_group_containing(root& lm, ui_node const& n, grouping_range i) {
	if(i.start < 0)
		return std::optional<grouping_range>();

	auto total_interactables = calaculate_interactables_at_node(lm, n);
	if(total_interactables <= i.end - i.start)
		return std::optional<grouping_range>();

	std::array<grouping_range, 12> top_col_groups;
	auto top_used = uint32_t(make_top_groups(lm, n, top_col_groups.data()));

	uint32_t match = 0;
	bool is_everything = true;

	if(top_used == 0) {
		return std::optional<grouping_range>();
	} else if(top_used > 1) {
		for(; match < top_used; ++match) {
			if(top_col_groups[match].start <= i.start && top_col_groups[match].end >= i.end &&
				(top_col_groups[match].end - top_col_groups[match].start) > (i.end - i.start)) {
				is_everything = false;
				break;
			}
		}
		if(match == top_used)
			return grouping_range{ -1, -1 };
	} else {
		//fallthrough: all in one group
	}

	grouping_range test_group = top_col_groups[match];
	while(test_group.end - test_group.start > 0) {
		auto divided = divide_group(test_group, 12);

		uint32_t submatch = 0;
		for(; submatch < 12ui32; ++submatch) {
			if(divided[submatch].start <= i.start && divided[submatch].end >= i.end &&
				(divided[submatch].end - divided[submatch].start) > (i.end - i.start)) {
				is_everything = false;
				break;
			}
		}

		if(submatch == 12ui32) {
			if(is_everything) {
				return grouping_range{ -1, -1 };
			} else {
				return test_group;
			}
		}

		test_group = divided[submatch];
	}
	return std::optional<grouping_range>();
}

focus_tracker get_parent_group_or_node(root& lm, focus_tracker r)  {
	if(r.node) {
		auto opt_range = find_group_containing(lm, *r.node, grouping_range{ r.child_offset, r.child_offset_end });

		if(!opt_range.has_value()) {
			auto parent = r.node->parent;;
			while(parent) {
				if((parent->behavior_flags & behavior::transparent_to_focus) == 0) {
					return focus_tracker{ parent, -1, -1 };
				}
				parent = parent->parent;
			}
			return focus_tracker{ nullptr, -1, -1 };
		} else {
			return focus_tracker{ r.node, opt_range->start, opt_range->end };
		}
	} else {
		return r;
	}
}

void root::repopulate_key_actions() {
	auto ws_size = system.get_workspace();
	auto add_interactable = [&](ui_node* n, int32_t group, bool display_as_group) {
		auto i_layout = n->interactable_layout(*this);
		n->behavior_flags |= behavior::interaction_flagged;
		if(i_layout.placement == interactable_placement::internal) {
			if(display_as_group)
				current_interactables.push_back(placed_interactable{
					n,
					layout_rect{ i_layout.relative_position.x, i_layout.relative_position.y, em{ 100 }, em{ 100 } } + workspace_placement(*n),
					interactable_state(interactable_state::group, uint8_t(group + 1)),
					i_layout.orientation
				});
			else
				current_interactables.push_back(placed_interactable{
					n,
					layout_rect{ i_layout.relative_position.x, i_layout.relative_position.y, em{ 100 }, em{ 100 } } + workspace_placement(*n),
					interactable_state(interactable_state::key, uint8_t(group + 1)),
					i_layout.orientation
				});
		} else if(i_layout.placement == interactable_placement::external) {
			layout_rect offset{ em{ 0 }, em{ 0 }, em{ 100 }, em{ 100 } };
			auto base = workspace_placement(*n);
			if(i_layout.orientation == interactable_orientation::left) {
				offset.y = n->position.height / 2 - em{ 50 };
				if(base.x >= em{ 100 })
					offset.x = em{ -100 };
				else
					offset.x = n->position.width;
			} else if(i_layout.orientation == interactable_orientation::right) {
				offset.y = n->position.height / 2 - em{ 50 };
				if(base.x + n->position.width + em{ 100 } > ws_size.x)
					offset.x = n->position.width;
				else
					offset.x = em{ -100 };
			} else if(i_layout.orientation == interactable_orientation::above) {
				offset.x = n->position.width / 2 - em{ 50 };
				if(base.y >= em{ 100 })
					offset.y = em{ -100 };
				else
					offset.y = n->position.height;
			} else {
				offset.x = n->position.width / 2 - em{ 50 };
				if(base.y + n->position.height + em{ 100 } > ws_size.y)
					offset.y = n->position.height;
				else
					offset.y = em{ -100 };
			}
			if(display_as_group)
				current_interactables.push_back(placed_interactable{
					n,
					offset + base,
					interactable_state(interactable_state::group, uint8_t(group + 1)),
					i_layout.orientation
				});
			else
				current_interactables.push_back(placed_interactable{
					n,
					offset + base,
					interactable_state(interactable_state::key, uint8_t(group + 1)),
					i_layout.orientation
				});
		}
	};

	for(auto& n : current_interactables)
		n.element->behavior_flags &= ~behavior::interaction_flagged;
	current_interactables.clear();

	if(node_repository.empty())
		return;

	int32_t start_offset = 0;
	ui_node* n = node_repository[0].get();
	if(focus_stack.size() == 0) {
		focus_actions.escape = go_up{ };
	} else if(focus_stack.size() == 1){
		auto explicit_parent = get_parent_group_or_node(*this,
			focus_tracker{ focus_stack.back().l_interface, focus_stack.back().child_offset, focus_stack.back().child_offset_end });

		add_interactable(explicit_parent.node, -1, false);
		if(explicit_parent.child_offset == -1) {
			focus_actions.escape = move_focus_to_node{ explicit_parent.node };
		} else {
			focus_actions.escape = explicit_parent;
		}
		start_offset = focus_stack.back().child_offset;
		n = focus_stack.back().l_interface;
	} else {
		start_offset = focus_stack.back().child_offset;
		n = focus_stack.back().l_interface;
		focus_actions.escape = go_up{ };
		add_interactable(n, -1, false);
	}

	focus_actions.valid_key_action_count = 0;
	

	auto interactables_range = interactables(*n);
	auto i = interactables_range.begin();
	

	while(start_offset > 0) {
		++i;
		--start_offset;
	}

	for(int32_t group = 0; group < current_groupings_size; ++group) {
		auto count_in_group = current_focus_groupings[group].end - current_focus_groupings[group].start;
		bool display_as_group = count_in_group > 1;

		if(count_in_group == 1) {
			auto primary_node = (*i);
			if(primary_node && (primary_node->behavior_flags & (behavior::interaction_info | behavior::interaction_command | behavior::interaction_focus)) != 0) {
				focus_actions.button_actions[group] = interaction{ primary_node };
				++focus_actions.valid_key_action_count;
			} else /*if(primary_node->child_count() > 0)*/ {
				focus_actions.button_actions[group] = move_focus_to_node{ primary_node };
				++focus_actions.valid_key_action_count;
			}
		} else {
			focus_actions.button_actions[group] = focus_tracker{ n, current_focus_groupings[group].start, current_focus_groupings[group].end };
			++focus_actions.valid_key_action_count;
		}

		while(count_in_group > 0) {
			// position interactable for nth group member
			if(auto n = *i; n)
				add_interactable(n, group, display_as_group);

			++i;
			--count_in_group;
		}
	}
}

void root::set_window_focus(focus_tracker r) {
	auto focus_id = top_focus(focus_stack);

	if(r.node == nullptr) {
		change_focus(focus_id, nullptr);
		focus_stack.clear();
		if(!node_repository.empty()) {
			current_groupings_size = make_top_groups(*this, *(node_repository[0].get()), current_focus_groupings.data());
			repopulate_key_actions();
		}
		return;
	}

	if(r.node != focus_id || focus_stack.empty() || r.child_offset != focus_stack.back().child_offset || r.child_offset_end != focus_stack.back().child_offset_end) {

		change_focus(focus_id, r.node);

		if(system.is_mouse_cursor_visible()) {
			focus_stack.clear();

			auto rel_position = this->latest_mouse_position - workspace_placement(*r.node);

			em xdistance_to_rect = em{ 0 } - rel_position.x;
			xdistance_to_rect = std::max(xdistance_to_rect, rel_position.x - r.node->position.width);

			em ydistance_to_rect = em{ 0 } - rel_position.y;
			ydistance_to_rect = std::max(ydistance_to_rect, rel_position.y - r.node->position.height);

			auto gdistance_to_rect = std::max(ydistance_to_rect, xdistance_to_rect);
			gdistance_to_rect = std::max(gdistance_to_rect, em{ 0 });
			if(gdistance_to_rect > em{ 0 })
				gdistance_to_rect = gdistance_to_rect + em{ 35 };

			focus_stack.push_back(stored_focus{ r.node, r.child_offset, r.child_offset_end, gdistance_to_rect });
		} else {
			focus_stack.push_back(stored_focus{ r.node, r.child_offset, r.child_offset_end, em{ 0 } });
		}

		if(r.child_offset != -1) {
			current_focus_groupings = divide_group(grouping_range{ r.child_offset, r.child_offset_end }, 12);
			current_groupings_size = 12;
		} else {
			current_groupings_size = make_top_groups(*this, *r.node, current_focus_groupings.data());
		}

		repopulate_key_actions();
	}
}

ui_node* root::take_key_action(key_action a) {
	if(std::holds_alternative<focus_tracker>(a)) {
		auto i = std::get<focus_tracker>(a);
		set_window_focus(i);
	} else if(std::holds_alternative<interaction>(a)) {
		auto target = std::get<interaction>(a).node;
		return target;
	} else if(std::holds_alternative<move_focus_to_node>(a)) {
		auto to_node = std::get<move_focus_to_node>(a).node;
		set_window_focus(focus_tracker{ to_node, -1, -1 });
	} else if(std::holds_alternative<go_up>(a)) {
		if(focus_stack.size() > 1) {

			auto old_focus_id = focus_stack.empty() ? nullptr : focus_stack.back().l_interface;
			auto new_focus_id = focus_stack.size() < 2 ? nullptr : (focus_stack[focus_stack.size() - 2].l_interface);

			change_focus(old_focus_id, new_focus_id);

			focus_stack.pop_back();

			if(new_focus_id == nullptr) {
				set_window_focus(focus_tracker{ nullptr, 0, 0 });
			} else {
				if(focus_stack.back().child_offset != -1) {
					current_focus_groupings = divide_group(grouping_range{ focus_stack.back().child_offset, focus_stack.back().child_offset_end }, 12);
					current_groupings_size = 12;
				} else {
					current_groupings_size = make_top_groups(*this, *new_focus_id, current_focus_groupings.data());
				}

				repopulate_key_actions();
			}
		} else {
			set_window_focus(focus_tracker{ nullptr, 0, 0 });
		}
	}
	return nullptr;
}

//
// container_node
//

size_t container_node::size() const {
	return sizeof(container_node);
}
void container_node::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	render_background(r, r.get_background_definition(type_id), offset, *this);

	if((ui_node::behavior_flags & behavior::standard_background) != 0) {
		r.system.rectangle(
			screen_space_rect{ r.system.to_screen_space(offset.x), r.system.to_screen_space(offset.y), r.system.to_screen_space(position.width), r.system.to_screen_space(position.height) },
			((ui_node::behavior_flags & behavior::visually_interactable) != 0 && r.under_mouse.type_array[size_t(mouse_interactivity::position)].node == this) ? rendering_modifiers::highlighted : rendering_modifiers::none,
			r.get_background_brush(ui_node::type_id));
	}

	auto ico_pos = r.get_icon_position(ui_node::type_id);

	if(r.pmode == prompt_mode::hidden || (ui_node::behavior_flags & behavior::interaction_flagged) == 0) {
		r.system.icon(r.get_icon(ui_node::type_id),
		screen_space_rect{
			r.system.to_screen_space(offset.x + ico_pos.x), r.system.to_screen_space(offset.y + ico_pos.y),
			r.system.to_screen_space(em{ 100 }), r.system.to_screen_space(em{ 100 })
		},
		r.get_foreground_brush(ui_node::type_id));
	}

	for(auto c : children) {
		auto child_position = get_sub_position(*this, *c) + offset;
		if((c->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ c, child_position });
		} else {
			c->render(r, child_position, postponed);
		}
	}
}
uint32_t container_node::child_count() const {
	return uint32_t(children.size());
}
ui_node* container_node::get_child(uint32_t index) const {
	return children[index];
}
probe_result container_node::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;
	
	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
	}
	for(auto c : children) {
		auto child_position = get_sub_position(*this, *c) + offset;
		if((c->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ c, child_position });
		} else {
			auto c_result = c->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}

	return result;
}
void container_node::on_gain_focus(root& r) {
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void container_node::on_lose_focus(root& r) {
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void container_node::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);
	for(auto c : children)
		c->on_visible(r);
}
void container_node::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);
	for(auto c : children)
		c->on_hide(r);
}
void container_node::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	for(auto c : children)
		c->on_update(r);
}
void container_node::on_create(root& r) {
	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	auto c = r.get_fixed_children(type_id);
	while(c.start != c.end) {
		auto n = r.make_control_by_type(this, *(c.start));
		children.push_back(n);
		++c.start;
	}
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}
}

void render_background(root& r, background_definition const& background, layout_position offset, ui_node const& node, rendering_modifiers rm = rendering_modifiers::none) {
	if(rm == rendering_modifiers::none) {
		if((node.behavior_flags & behavior::visually_interactable) != 0 && r.under_mouse.type_array[size_t(mouse_interactivity::position)].node == &node)
			rm = rendering_modifiers::highlighted;
	}
	if(background.image.value != -1) {
		r.system.background(background.image, r.get_background_brush(node.type_id),
			screen_space_rect{ r.system.to_screen_space(offset.x + background.exterior_edge_offsets.x), r.system.to_screen_space(offset.y + background.exterior_edge_offsets.y), r.system.to_screen_space(node.position.width + background.exterior_edge_offsets.width), r.system.to_screen_space(node.position.height + background.exterior_edge_offsets.height) },
			background.texture_interior_region, rm);
	} else if(background.brush != std::numeric_limits<uint16_t>::max()) {
		r.system.rectangle(
			screen_space_rect{ r.system.to_screen_space(offset.x + background.exterior_edge_offsets.x), r.system.to_screen_space(offset.y + background.exterior_edge_offsets.y), r.system.to_screen_space(node.position.width + background.exterior_edge_offsets.width), r.system.to_screen_space(node.position.height + background.exterior_edge_offsets.height) },
			 rm, background.brush);
	} else {
		r.system.empty_rectangle(
			screen_space_rect{ r.system.to_screen_space(offset.x), r.system.to_screen_space(offset.y), r.system.to_screen_space(node.position.width), r.system.to_screen_space(node.position.height) },
			rm, r.get_background_brush(node.type_id));
	}

	if(background.left_border != 0) {
		r.system.rectangle(
			screen_space_rect{ 
				r.system.to_screen_space(offset.x),
				r.system.to_screen_space(offset.y), 
				r.system.to_screen_space(em{ background.left_border }),
				r.system.to_screen_space(node.position.height) },
			rm, r.get_foreground_brush(node.type_id));
	}
	if(background.right_border != 0) {
		r.system.rectangle(
			screen_space_rect{
				r.system.to_screen_space(offset.x + node.position.width - em{ background.right_border }),
				r.system.to_screen_space(offset.y),
				r.system.to_screen_space(em{ background.right_border }),
				r.system.to_screen_space(node.position.height) },
				rm, r.get_foreground_brush(node.type_id));
	}
	if(background.top_border != 0) {
		r.system.rectangle(
			screen_space_rect{
				r.system.to_screen_space(offset.x),
				r.system.to_screen_space(offset.y),
				r.system.to_screen_space(node.position.width),
				r.system.to_screen_space(em{ background.top_border }) },
				rm, r.get_foreground_brush(node.type_id));
	}
	if(background.bottom_border != 0) {
		r.system.rectangle(
			screen_space_rect{
				r.system.to_screen_space(offset.x),
				r.system.to_screen_space(offset.y + node.position.height - em{ background.bottom_border }),
				r.system.to_screen_space(node.position.width),
				r.system.to_screen_space(em{ background.bottom_border }) },
				rm, r.get_foreground_brush(node.type_id));
	}
}

//
// proportional_window
//

size_t proportional_window::size() const {
	return sizeof(proportional_window);
}
void proportional_window::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	render_background(r, r.get_background_definition(type_id), offset, *this);

	auto ico_pos = r.get_icon_position(ui_node::type_id);

	if(r.pmode == prompt_mode::hidden || (ui_node::behavior_flags & behavior::interaction_flagged) == 0) {
		r.system.icon(r.get_icon(ui_node::type_id),
			screen_space_rect{
				r.system.to_screen_space(offset.x + ico_pos.x), r.system.to_screen_space(offset.y + ico_pos.y),
				r.system.to_screen_space(em{ 100 }), r.system.to_screen_space(em{ 100 })
			},
			r.get_foreground_brush(ui_node::type_id));
	}

	for(auto c : children) {
		auto child_position = get_sub_position(*this, *c) + offset;
		if((c->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ c, child_position });
		} else {
			c->render(r, child_position, postponed);
		}
	}
}
uint32_t proportional_window::child_count() const {
	return uint32_t(children.size());
}
ui_node* proportional_window::get_child(uint32_t index) const {
	return children[index];
}
probe_result proportional_window::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;
	
	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
	}
	for(auto c : children) {
		auto child_position = get_sub_position(*this, *c) + offset;
		if((c->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ c, child_position });
		} else {
			auto c_result = c->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}

	return result;
}
void proportional_window::on_gain_focus(root& r) {
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void proportional_window::on_lose_focus(root& r) {
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void proportional_window::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);

	for(auto c : children)
		c->on_visible(r);
}
void proportional_window::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);

	for(auto c : children)
		c->on_hide(r);
}
void proportional_window::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	for(auto c : children)
		c->on_update(r);
}
void proportional_window::on_create(root& r) {
	auto cformatting = r.get_window_children(type_id);
	child_positions.insert(child_positions.begin(), cformatting.start, cformatting.end);

	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	auto c = r.get_fixed_children(type_id);
	while(c.start != c.end) {
		auto n = r.make_control_by_type(this, *(c.start));
		children.push_back(n);
		++c.start;
	}

	force_resize(r, layout_position{ position.width, position.height });
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}
}
void proportional_window::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;
	for(uint32_t i = 0; i < children.size(); ++i) {
		auto& fmt = child_positions[i];
		layout_rect sz;
		switch(fmt.x_start_base) {
			case relative_to::zero:
				sz.x = em{ 0 } + fmt.x_start_offset;
				break;
			case relative_to::all:
				sz.x = position.width + fmt.x_start_offset;
				break;
			case relative_to::half:
				sz.x = position.width / 2 + fmt.x_start_offset;
				break;
			case relative_to::one_third:
				sz.x = position.width / 3 + fmt.x_start_offset;
				break;
			case relative_to::two_thirds:
				sz.x = (position.width * 2) / 3 + fmt.x_start_offset;
				break;
			case relative_to::one_fourth:
				sz.x = position.width / 4 + fmt.x_start_offset;
				break;
			case relative_to::three_fourths:
				sz.x = (position.width * 3) / 4 + fmt.x_start_offset;
				break;
		}
		switch(fmt.x_end_base) {
			case relative_to::zero:
				sz.width = (em{ 0 } + fmt.x_end_offset) - sz.x;
				break;
			case relative_to::all:
				sz.width = (position.width + fmt.x_end_offset) - sz.x;
				break;
			case relative_to::half:
				sz.width = (position.width / 2 + fmt.x_end_offset) - sz.x;
				break;
			case relative_to::one_third:
				sz.width = (position.width / 3 + fmt.x_end_offset) - sz.x;
				break;
			case relative_to::two_thirds:
				sz.width = ((position.width * 2) / 3 + fmt.x_end_offset) - sz.x;
				break;
			case relative_to::one_fourth:
				sz.width = (position.width / 4 + fmt.x_end_offset) - sz.x;
				break;
			case relative_to::three_fourths:
				sz.width = ((position.width * 3) / 4 + fmt.x_end_offset) - sz.x;
				break;
		}
		sz.width = std::max(sz.width, em{ 0 });

		switch(fmt.y_start_base) {
			case relative_to::zero:
				sz.y = em{ 0 } + fmt.y_start_offset;
				break;
			case relative_to::all:
				sz.y = position.height + fmt.y_start_offset;
				break;
			case relative_to::half:
				sz.y = position.height / 2 + fmt.y_start_offset;
				break;
			case relative_to::one_third:
				sz.y = position.height / 3 + fmt.y_start_offset;
				break;
			case relative_to::two_thirds:
				sz.y = (position.height * 2) / 3 + fmt.y_start_offset;
				break;
			case relative_to::one_fourth:
				sz.y = position.height / 4 + fmt.y_start_offset;
				break;
			case relative_to::three_fourths:
				sz.y = (position.height * 3) / 4 + fmt.y_start_offset;
				break;
		}
		switch(fmt.y_end_base) {
			case relative_to::zero:
				sz.height = (em{ 0 } + fmt.y_end_offset) - sz.y;
				break;
			case relative_to::all:
				sz.height = (position.height + fmt.y_end_offset) - sz.y;
				break;
			case relative_to::half:
				sz.height = (position.height / 2 + fmt.y_end_offset) - sz.y;
				break;
			case relative_to::one_third:
				sz.height = (position.height / 3 + fmt.y_end_offset) - sz.y;
				break;
			case relative_to::two_thirds:
				sz.height = ((position.height * 2) / 3 + fmt.y_end_offset) - sz.y;
				break;
			case relative_to::one_fourth:
				sz.height = (position.height / 4 + fmt.y_end_offset) - sz.y;
				break;
			case relative_to::three_fourths:
				sz.height = ((position.height * 3) / 4 + fmt.y_end_offset) - sz.y;
				break;
		}
		sz.height = std::max(sz.height, em{ 0 });

		children[i]->position.x = sz.x;
		children[i]->position.y = sz.y;
		children[i]->force_resize(r, layout_position{ sz.width, sz.height});
	}
}
void proportional_window::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	auto target_width = std::min(std::max(desired_width, minimum_width(r)), maximum_space.x);
	auto target_height = std::min(std::max(desired_height, minimum_height(r)), maximum_space.y);
	force_resize(r, layout_position{ target_width, target_height });
}
em proportional_window::minimum_width(root& r) {
	em result{ 0 };
	for(uint32_t i = 0; i < children.size(); ++i) {
		auto& fmt = child_positions[i];
		auto min_c = children[i]->minimum_width(r);
		auto offset_amt = fmt.x_end_offset - fmt.x_start_offset;
		
		auto base_target = min_c - offset_amt;
		auto proportional_amount = to_float(fmt.x_end_base) - to_float(fmt.x_start_base);
		if(proportional_amount > 0.0f) {
			result = std::max(result, em{ int16_t(std::ceil(float(base_target.value) / proportional_amount)) });
		}
	}
	return result;
}
em proportional_window::minimum_height(root& r) {
	em result{ 0 };
	for(uint32_t i = 0; i < children.size(); ++i) {
		auto& fmt = child_positions[i];
		auto min_c = children[i]->minimum_height(r);
		auto offset_amt = fmt.y_end_offset - fmt.y_start_offset;

		auto base_target = min_c - offset_amt;
		auto proportional_amount = to_float(fmt.y_end_base) - to_float(fmt.y_start_base);
		if(proportional_amount > 0.0f) {
			result = std::max(result, em{ int16_t(std::ceil(float(base_target.value) / proportional_amount)) });
		}
	}
	return result;
}

//
// space filler
//

size_t space_filler::size() const {
	return sizeof(space_filler);
}
void space_filler::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	render_background(r, r.get_background_definition(type_id), offset, *this);

	for(auto c : children) {
		auto child_position = get_sub_position(*this, *c) + offset;
		if((c->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ c, child_position });
		} else {
			c->render(r, child_position, postponed);
		}
	}
}
uint32_t space_filler::child_count() const {
	return uint32_t(children.size());
}
ui_node* space_filler::get_child(uint32_t index) const {
	return children[index];
}
probe_result space_filler::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;
	
	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
	}
	for(auto c : children) {
		auto child_position = get_sub_position(*this, *c) + offset;
		if((c->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ c, child_position });
		} else {
			auto c_result = c->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}

	return result;
}
void space_filler::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);

	for(auto c : children)
		c->on_visible(r);
}
void space_filler::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);

	for(auto c : children)
		c->on_hide(r);
}
void space_filler::on_gain_focus(root& r) {
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void space_filler::on_lose_focus(root& r) {
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void space_filler::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	for(auto c : children)
		c->on_update(r);
}
void space_filler::on_create(root& r) {
	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}

	auto c = r.get_fixed_children(type_id);
	while(c.start != c.end) {
		auto n = r.make_control_by_type(this, *(c.start));
		children.push_back(n);
		++c.start;
	}

	force_resize(r, layout_position{ position.width, position.height });

	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}
}
void space_filler::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;
	bool horizontal = r.get_horizontal_orientation(ui_node::type_id);
	auto separator = r.get_divider_index(ui_node::type_id);
	auto leading = em{ 0 };
	auto trailing = horizontal ? size.x : size.y;

	for(uint32_t i = 0; i < children.size(); ++i) {
		if((children[i]->behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
			continue;

		if(horizontal) {
			children[i]->position.y = em{ 0 };
		} else {
			children[i]->position.x = em{ 0 };
		}

		if(i < uint32_t(separator)) {
			auto space_used = horizontal ? children[i]->minimum_width(r) : children[i]->minimum_height(r);
			if(horizontal) {
				children[i]->position.x = leading;
				children[i]->force_resize(r, layout_position{ space_used, size.y });
			} else {
				children[i]->position.y = leading;
				children[i]->force_resize(r, layout_position{ size.x, space_used });
			}
			leading = leading + space_used;
		} else if(i > uint32_t(separator)) {
			auto space_used = horizontal ? children[i]->minimum_width(r) : children[i]->minimum_height(r);
			trailing = trailing - space_used;
			if(horizontal) {
				children[i]->position.x = trailing;
				children[i]->force_resize(r, layout_position{ space_used, size.y });
			} else {
				children[i]->position.y = trailing;
				children[i]->force_resize(r, layout_position{ size.x, space_used });
			}
		}
	}

	// position separator
	if(horizontal) {
		children[separator]->position.x = leading;
		children[separator]->force_resize(r, layout_position{ std::max(em{ 0 }, trailing - leading), size.y });
	} else {
		children[separator]->position.y = leading;
		children[separator]->force_resize(r, layout_position{ size.x, std::max(em{ 0 }, trailing - leading) });
	}
}
void space_filler::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	auto target_width = std::min(std::max(desired_width, minimum_width(r)), maximum_space.x);
	auto target_height = std::min(std::max(desired_height, minimum_height(r)), maximum_space.y);
	force_resize(r, layout_position{ target_width, target_height });
}
em space_filler::minimum_width(root& r) {
	em result{ 0 };
	bool horizontal = r.get_horizontal_orientation(ui_node::type_id);
	if(horizontal) {
		for(uint32_t i = 0; i < children.size(); ++i) {
			result = result + children[i]->minimum_width(r);
		}
	} else {
		for(uint32_t i = 0; i < children.size(); ++i) {
			result = std::max(result, children[i]->minimum_width(r));
		}
	}
	return result;
}
em space_filler::minimum_height(root& r) {
	em result{ 0 };
	bool horizontal = r.get_horizontal_orientation(ui_node::type_id);
	if(!horizontal) {
		for(uint32_t i = 0; i < children.size(); ++i) {
			result = result + children[i]->minimum_height(r);
		}
	} else {
		for(uint32_t i = 0; i < children.size(); ++i) {
			result = std::max(result, children[i]->minimum_height(r));
		}
	}
	return result;
}

//
// page_control_icon_button
//

size_t page_control_icon_button::size() const {
	return sizeof(page_control_icon_button);
}
void page_control_icon_button::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	r.system.icon(
		r.get_icon(ui_node::type_id),
		r.system.to_screen_space(layout_rect{ em{ 0 }, em{ 0 }, em{ 100 }, em{ 100 } } + offset),
		r.get_foreground_brush(ui_node::type_id),
		enabled ? rendering_modifiers::none : rendering_modifiers::disabled);
}
probe_result page_control_icon_button::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
		result.type_array[size_t(mouse_interactivity::button)].node = this;
		result.type_array[size_t(mouse_interactivity::button)].relative_location = probe_pos - offset;
	}

	return result;
}
void page_control_icon_button::on_update(root& r) {
	auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(this) + sizeof(page_control_icon_button));
	auto type = (*data & page_control_icon_button::type_mask);
	auto parent_pages = parent->parent->get_page_information();
	if(type == left2_type || type == left_type) {
		enabled = (parent_pages.current_page > 0);
	} else {
		enabled = (parent_pages.current_page + 1 < parent_pages.total_pages);
	}
}
void page_control_icon_button::on_lbutton(root& r, layout_position pos) {
	if(enabled) {
		auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(this) + sizeof(page_control_icon_button));
		auto type = (*data & page_control_icon_button::type_mask);
		auto range = parent->parent->get_page_information();

		r.system.play_sound(r.get_interaction_sound(type_id));
		
		switch(type) {
			case left2_type:
				auto new_page = range.current_page - int32_t(std::ceil(std::sqrt(float(range.total_pages))));
				parent->parent->change_page(r, new_page);
				break;
			case left_type:
				auto new_page = range.current_page - int32_t(1);
				parent->parent->change_page(r, new_page);
				break;
			case right2_type:
				auto new_page = range.current_page + int32_t(std::ceil(std::sqrt(float(range.total_pages))));
				parent->parent->change_page(r, new_page);
				break;
			case right_type:
				auto new_page = range.current_page + int32_t(1);
				parent->parent->change_page(r, new_page);
				break;
			default:
				break;
		}
	}

}

interactable_definition page_control_icon_button::interactable_layout(root& r) {
	auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(this) + sizeof(page_control_icon_button));
	auto position = (*data & page_control_icon_button::position_mask);
	switch(position) {
		case page_control_icon_button::right:
			return interactable_definition{ layout_position{ em{ 0 }, em{ 0 } }, interactable_orientation::right, interactable_placement::external };
		case page_control_icon_button::bottom:
			return interactable_definition{ layout_position{ em{ 0 }, em{ 0 } }, interactable_orientation::below, interactable_placement::external };
		default:
			return interactable_definition{ layout_position{ em{ 0 }, em{ 0 } }, interactable_orientation::above, interactable_placement::external };
	}
}

//
// page_control_text
//

size_t page_control_text::size() const {
	return sizeof(page_control_text);
}
void page_control_text::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	text_data->render(r.system, layout_rect{ offset.x, offset.y, position.width, position.height }, r.get_foreground_brush(ui_node::type_id));

	auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(this) + sizeof(page_control_icon_button));

	if(*data == page_control_text::vertical) {
		auto full_width = r.system.to_screen_space(position.width);
		auto full_height = r.system.to_screen_space(position.height);
		auto screen_offset = r.system.to_screen_space(offset);

		r.system.rectangle(screen_space_rect{ int32_t(full_width * 0.2f) + screen_offset.x, full_height / 2 - 1 + screen_offset.y, int32_t(full_width * 0.8f), 2 }, rendering_modifiers::none, r.get_foreground_brush(ui_node::type_id));
	}
}
probe_result page_control_text::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	return probe_result{ };
}
void page_control_text::on_create(root& r) {
	text_data = r.system.make_text(*this);
	text_data->set_font(r.system, r.get_text_information(ui_node::type_id).font);
	text_data->set_alignment(r.system, text::content_alignment::centered);
}
void page_control_text::on_reload(root& r) {
	on_update(r);
}
void page_control_text::on_update(root& r) {
	auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(this) + sizeof(page_control_icon_button));
	auto range = parent->parent->get_page_information();

	auto current_page = r.system.int_to_text(range.current_page + 1, false);
	auto max_page = r.system.int_to_text(range.total_pages, false);

	if(*data == page_control_text::horizontal) {
		auto result = r.system.perform_substitutions(r.system.get_hande("page_numbers_h"), text::unprocessed_text_source{ max_page, "max" }, text::unprocessed_text_source{ current_page, "current" });
		text_data->set_text(r.system, std::move(result));
	} else {
		auto result = r.system.perform_substitutions(r.system.get_hande("page_numbers_v"), text::unprocessed_text_source{ max_page, "max" }, text::unprocessed_text_source{ current_page, "current" });
		text_data->set_text(r.system, std::move(result));
	}
}
void page_control_text::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;
}

//
// page_controls
//

size_t page_controls::size() const {
	return sizeof(page_controls);
}
void page_controls::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	std::array<ui_node*, 5> children = { left2_button, left_button, text, right_button, right2_button };

	if(r.contains_focus(this)) {
		for(auto c : children) {
			auto child_position = get_sub_position(*this, *c) + offset;
			c->render(r, child_position, postponed);
		}
	} else {
		text->render(r, get_sub_position(*this, *text) + offset, postponed);

		if(r.pmode == prompt_mode::hidden || (ui_node::behavior_flags & behavior::interaction_flagged) == 0) {
			auto pos = (vertical_arrangement && position.height > em{ 190 }) ?
				layout_rect{ position.width / 2 - em{ 50 }, em{ 0 }, em{ 100 }, em{ 100 } } :
				layout_rect{ em{ 0 }, position.height / 2 - em{ 50 }, em{ 100 }, em{ 100 } };

			r.system.icon(
				page_icon,
				r.system.to_screen_space(pos + offset),
				r.get_foreground_brush(parent->type_id));
		}
	}
}
probe_result page_controls::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
		result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		
		result.type_array[size_t(mouse_interactivity::position)].node = this;
		result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
	}

	if(r.contains_focus(this)) {
		std::array<ui_node*, 4> children = { left2_button, left_button, right_button, right2_button };
		for(auto c : children) {
			auto child_position = get_sub_position(*this, *c) + offset;

			auto c_result = c->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}

	return result;
}
void page_controls::on_update(root& r) {
	auto container_pages = parent->get_page_information();
	if(container_pages.total_pages <= 1) {
		ui_node::behavior_flags |= behavior::visually_hidden;
	} else {
		ui_node::behavior_flags &= ~behavior::visually_hidden;

		std::array<ui_node*, 5> children = { left2_button, left_button, text, right_button, right2_button };
		for(auto c : children)
			c->on_update(r);
	}
}
interactable_definition page_controls::interactable_layout(root& r) {
	if(r.contains_focus(this)) {
		return interactable_definition{ layout_position{ em{ 0 }, em{ 0 } }, interactable_orientation::left, interactable_placement::suppressed };
	} else {
		auto pos = (vertical_arrangement && position.height > em{ 190 }) ?
			layout_position{ position.width / 2 - em{ 50 }, em{ 0 } } :
			layout_position{ em{ 0 }, position.height / 2 - em{ 50 } };
		return interactable_definition{ pos, interactable_orientation::left, interactable_placement::internal};
	}
}
void page_controls::on_create(root& r) {
	auto controls_defs = r.get_page_ui_definitions(parent->type_id);
	left2_button = r.make_control_by_type(this, controls_defs.left2_button);
	left_button = r.make_control_by_type(this, controls_defs.left_button);
	right2_button = r.make_control_by_type(this, controls_defs.right2_button);
	right_button = r.make_control_by_type(this, controls_defs.right_button);
	text = r.make_control_by_type(this, controls_defs.text);

	{
		auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
		*data = page_control_icon_button::left2_type;
	}
	{
		auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
		*data = page_control_icon_button::left_type;
	}
	{
		auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
		*data = page_control_icon_button::right2_type;
	}
	{
		auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
		*data = page_control_icon_button::right_type;
	}

	vertical_arrangement = controls_defs.vertical_arrangement;
	page_icon = controls_defs.page_icon;
	behavior_flags |= (vertical_arrangement ? behavior::position_from_right : behavior::position_from_bottom);
}
void page_controls::force_resize(root& r, layout_position size) {
	if(size.x == position.width && size.y == position.height)
		return;

	if(vertical_arrangement) {
		if(size.y >= em{ 100 } * 6) {
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::vertical;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::right;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::right;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::right;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::right;
			}
			text->force_resize(r, layout_position{ size.x, em{ 200 } });
			text->position.x = em{ 0 };
			text->position.y = size.y / 2 - em{ 100 };
			left2_button->position.x = size.x / 2 - em{ 50 };
			left2_button->position.y = size.y / 2 - em{ 300 };
			left_button->position.x = size.x / 2 - em{ 50 };
			left_button->position.y = size.y / 2 - em{ 200 };
			right_button->position.x = size.x / 2 - em{ 50 };
			right_button->position.y = size.y / 2 + em{ 100 };
			right2_button->position.x = size.x / 2 - em{ 50 };
			right2_button->position.y = size.y / 2 + em{ 200 };
		} else if(size.y >= em{ 100 } * 4) {
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::vertical;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::bottom;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::bottom;
			}
			text->force_resize(r, layout_position{ size.x, em{ 200 } });
			text->position.x = em{ 0 };
			text->position.y = size.y / 2 - em{ 100 };
			left2_button->position.x = size.x / 2 - em{ 0 };
			left2_button->position.y = size.y / 2 - em{ 200 };
			left_button->position.x = size.x / 2 - em{ 100 };
			left_button->position.y = size.y / 2 - em{ 200 };
			right_button->position.x = size.x / 2 - em{ 100 };
			right_button->position.y = size.y / 2 + em{ 100 };
			right2_button->position.x = size.x / 2 - em{ 0 };
			right2_button->position.y = size.y / 2 + em{ 100 };
		} else if(size.y >= em{ 100 } * 3) {
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::horizontal;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::bottom;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::bottom;
			}
			text->force_resize(r, layout_position{ size.x, em{ 100 } });
			text->position.x = em{ 0 };
			text->position.y = size.y / 2 - em{ 50 };
			left2_button->position.x = size.x / 2 - em{ 0 };
			left2_button->position.y = size.y / 2 - em{ 150 };
			left_button->position.x = size.x / 2 - em{ 100 };
			left_button->position.y = size.y / 2 - em{ 150 };
			right_button->position.x = size.x / 2 - em{ 100 };
			right_button->position.y = size.y / 2 + em{ 50 };
			right2_button->position.x = size.x / 2 - em{ 0 };
			right2_button->position.y = size.y / 2 + em{ 50 };
		} else if(size.y >= em{ 100 } * 2) {
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::horizontal;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::top;
			}
			text->force_resize(r, layout_position{ size.x, em{ 100 } });
			text->position.x = em{ 0 };
			text->position.y = size.y / 2;
			left2_button->position.x = size.x / 2 - em{ 200 };
			left2_button->position.y = size.y / 2 - em{ 100 };
			left_button->position.x = size.x / 2 - em{ 100 };
			left_button->position.y = size.y / 2 - em{ 100 };
			right_button->position.x = size.x / 2 + em{ 0 };
			right_button->position.y = size.y / 2 - em{ 100 };
			right2_button->position.x = size.x / 2 + em{ 100 };
			right2_button->position.y = size.y / 2 - em{ 100 };
		} else { // fallback horizontal
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::horizontal;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::top;
			}
			text->force_resize(r, layout_position{ em{ 300 }, em{ 100 } });
			text->position.x = size.x / 2 - em{ 150 };
			text->position.y = size.y / 2 - em{ 50 };
			left2_button->position.x = size.x / 2 - em{ 350 };
			left2_button->position.y = size.y / 2 - em{ 50 };
			left_button->position.x = size.x / 2 - em{ 250 };
			left_button->position.y = size.y / 2 - em{ 50 };
			right_button->position.x = size.x / 2 + em{ 150 };
			right_button->position.y = size.y / 2 - em{ 50 };
			right2_button->position.x = size.x / 2 + em{ 250 };
			right2_button->position.y = size.y / 2 - em{ 50 };
		}
		position.x = em{ 0 } - size.x;
	} else { // horizontal arrangement
		if(size.x >= em{ 100 } * 7) {
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::horizontal;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::top;
			}
			text->force_resize(r, layout_position{ em{ 300 }, em{ 100 } });
			text->position.x = size.x / 2 - em{ 150 };
			text->position.y = size.y / 2 - em{ 50 };
			left2_button->position.x = size.x / 2 - em{ 350 };
			left2_button->position.y = size.y / 2 - em{ 50 };
			left_button->position.x = size.x / 2 - em{ 250 };
			left_button->position.y = size.y / 2 - em{ 50 };
			right_button->position.x = size.x / 2 + em{ 150 };
			right_button->position.y = size.y / 2 - em{ 50 };
			right2_button->position.x = size.x / 2 + em{ 250 };
			right2_button->position.y = size.y / 2 - em{ 50 };
		} else if(size.x >= em{ 100 } * 4) {
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::horizontal;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::top;
			}
			text->force_resize(r, layout_position{ size.x, em{ 100 } });
			text->position.x = em{ 0 };
			text->position.y = size.y / 2;
			left2_button->position.x = size.x / 2 - em{ 200 };
			left2_button->position.y = size.y / 2 - em{ 100 };
			left_button->position.x = size.x / 2 - em{ 100 };
			left_button->position.y = size.y / 2 - em{ 100 };
			right_button->position.x = size.x / 2 + em{ 0 };
			right_button->position.y = size.y / 2 - em{ 100 };
			right2_button->position.x = size.x / 2 + em{ 100 };
			right2_button->position.y = size.y / 2 - em{ 100 };
		} else if(size.x >= em{ 100 } * 3) {
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::horizontal;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::bottom;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::bottom;
			}
			text->force_resize(r, layout_position{ size.x, em{ 100 } });
			text->position.x = em{ 0 };
			text->position.y = size.y / 2 - em{ 50 };
			left2_button->position.x = size.x / 2 - em{ 0 };
			left2_button->position.y = size.y / 2 - em{ 150 };
			left_button->position.x = size.x / 2 - em{ 100 };
			left_button->position.y = size.y / 2 - em{ 150 };
			right_button->position.x = size.x / 2 - em{ 100 };
			right_button->position.y = size.y / 2 + em{ 50 };
			right2_button->position.x = size.x / 2 - em{ 0 };
			right2_button->position.y = size.y / 2 + em{ 50 };
		} else { // fallback vertical arrangement
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(text) + text->size());
				*data = page_control_text::vertical;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left2_button) + left2_button->size());
				*data = page_control_icon_button::left2_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(left_button) + left_button->size());
				*data = page_control_icon_button::left_type | page_control_icon_button::top;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right_button) + right_button->size());
				*data = page_control_icon_button::right_type | page_control_icon_button::bottom;
			}
			{
				auto data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(right2_button) + right2_button->size());
				*data = page_control_icon_button::right2_type | page_control_icon_button::bottom;
			}
			text->force_resize(r, layout_position{ size.x, em{ 200 } });
			text->position.x = em{ 0 };
			text->position.y = size.y / 2 - em{ 100 };
			left2_button->position.x = size.x / 2 - em{ 0 };
			left2_button->position.y = size.y / 2 - em{ 200 };
			left_button->position.x = size.x / 2 - em{ 100 };
			left_button->position.y = size.y / 2 - em{ 200 };
			right_button->position.x = size.x / 2 - em{ 100 };
			right_button->position.y = size.y / 2 + em{ 100 };
			right2_button->position.x = size.x / 2 - em{ 0 };
			right2_button->position.y = size.y / 2 + em{ 100 };
		}
		position.y = em{ 0 } - size.y;
	}

	text->on_update(r);
}

void page_controls::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	if(vertical_arrangement) {
		auto height = maximum_space.y;
		em width = em{ 100 };
		if(height >= em{ 100 } * 4) {
			width = std::max(em{ 100 } * 2 , desired_width);
		} else {
			width = std::max(em{ 100 } * 3, desired_width);
		}
		force_resize(r, layout_position{ width, height });
	} else {
		auto height = em{ 100 };
		em width = maximum_space.x;
		if(width >= em{ 100 } *7) {
			height = std::max(em{ 100 }, desired_height);
		} else if(width >= em{ 100 } *4) {
			height = std::max(em{ 200 }, desired_height);
		} else if(width >= em{ 100 } *3) {
			height = std::max(em{ 300 }, desired_height);
		} else { // fallback vertical arrangement
			height = std::max(em{ 400 }, desired_height);
		}
		force_resize(r, layout_position{ width, height });
	}
}
em page_controls::minimum_width(root& r) {
	return text->minimum_width(r);
}
em page_controls::minimum_height(root& r) {
	return text->minimum_height(r) * 2;
}

//
// dynamic_column
//
size_t dynamic_column::size() const {
	return sizeof(dynamic_column);
}
void dynamic_column::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	if(pending_relayout)
		repaginate(r);

	auto col_settings = r.get_column_properties(ui_node::type_id);

	render_background(r, r.get_background_definition(type_id), offset, *this);

	em last_height{ -1 };
	bool mode = false;
	bool in_group = false;

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		if(children[i]->position.y <= last_height) {
			mode = false;
		}
		last_height = children[i]->position.y;

		if((children[i]->behavior_flags & (behavior::layout_as_section_header | behavior::layout_as_column_header | behavior::layout_as_separator)) != 0) {
			mode = false;
		}
		if((children[i]->behavior_flags & behavior::layout_start_group) != 0) {
			in_group = true;
		}
		if((children[i]->behavior_flags & behavior::layout_end_group) != 0) {
			in_group = false;
		}

		if(col_settings.enhance_line_visibility)
			r.system.set_line_highlight_mode(mode);

		auto child_position = get_sub_position(*this, *children[i]) + offset;
		if((children[i]->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ children[i], child_position });
		} else {
			children[i]->render(r, child_position, postponed);
		}
		if(!in_group && (children[i]->behavior_flags & behavior::layout_as_separator) == 0) {
			mode = !mode;
		}
	}

	if(col_settings.enhance_line_visibility)
		r.system.set_line_highlight_mode(false);

	{
		auto child_position = get_sub_position(*this, *page_controls) + offset;
		if((page_controls->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ page_controls, child_position });
		} else {
			page_controls->render(r, child_position, postponed);
		}
	}
}
uint32_t dynamic_column::child_count() const {
	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page == num_pages - 1 ? uint32_t(children.size()) : page_starts[current_page]);
	return page_end - page_start +((page_controls->behavior_flags & behavior::visually_hidden) != 0 ? 0 : 1);
}
ui_node* dynamic_column::get_child(uint32_t index) const {
	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);
	if(index < page_end - page_start)
		return children[page_start + index];
	else
		return page_controls;
}
probe_result dynamic_column::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;
	
	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
		result.type_array[size_t(mouse_interactivity::scroll)].node = this;
		result.type_array[size_t(mouse_interactivity::scroll)].relative_location = probe_pos - offset;
	}

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		auto child_position = get_sub_position(*this, *children[i]) + offset;
		if((children[i]->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ children[i], child_position });
		} else {
			auto c_result = children[i]->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}
	{
		auto child_position = get_sub_position(*this, *page_controls) + offset;
		if((page_controls->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ page_controls, child_position });
		} else {
			auto c_result = page_controls->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}

	return result;
}
void dynamic_column::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		children[i]->on_visible(r);
	}
}
void dynamic_column::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		children[i]->on_hide(r);
	}
}
void dynamic_column::on_gain_focus(root& r) {
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void dynamic_column::on_lose_focus(root& r) {
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void dynamic_column::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);
	
	if(pending_relayout)
		repaginate(r);

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		children[i]->on_update(r);
	}

	page_controls->on_update(r);
}
void dynamic_column::on_create(root& r) {
	page_controls = make_page_controls(r, this);

	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}

	force_resize(r, layout_position{ position.width, position.height });

	page_controls->on_update(r);
}
void dynamic_column::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;

	auto col_settings = r.get_column_properties(ui_node::type_id);
	auto width_per_column = col_settings.number_of_columns != 0 ? size.x / col_settings.number_of_columns : em{ 0 };

	page_starts.clear();

	int32_t cur_col = 0;
	int32_t col_start = 0;
	em cur_height{ 0 };
	em cur_x_off{ 0 };
	em max_width = col_settings.minimum_width;
	int32_t cur_page = 0;
	int32_t group_start = -1;

	page_controls->resize(r, size, em{ 0 }, em{ 0 });
	auto available_size = size.y - page_controls->position.height;

	int32_t i = 0;
	for(; i < int32_t(children.size()); ++i) {
		if((children[i]->behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
			continue;
		if((children[i]->behavior_flags & behavior::layout_start_group) != 0) {
			group_start = i;
		}
		if((children[i]->behavior_flags & behavior::layout_as_separator) != 0 && col_start == i) {
			++col_start;
			continue;
		}

		bool child_fits_in_column = true;
		if(width_per_column > em{ 0 }) {
			children[i]->resize(r, minui::layout_position{ width_per_column, available_size - cur_height }, width_per_column, em{ 0 });
			if(children[i]->position.height > available_size - cur_height)
				child_fits_in_column = false;
		} else {
			children[i]->resize(r, minui::layout_position{ size.x - cur_x_off , available_size - cur_height}, em{ 0 }, em{ 0 });
			if(children[i]->position.height > available_size - cur_height || children[i]->position.width > size.x - cur_x_off)
				child_fits_in_column = false;
		}

		if((!child_fits_in_column || (children[i]->behavior_flags & behavior::layout_as_column_header) != 0) && i != col_start) {

			++cur_col;
			if(col_settings.number_of_columns == 0) {
				// finish last col width
				for(uint32_t j = col_start; j < i; ++j) {
					children[j]->force_resize(r, layout_position{ max_width, children[j]->position.height });
				}
				cur_x_off = cur_x_off + max_width;
			} else if(cur_col >= col_settings.number_of_columns) { // new page
				page_starts.push_back(uint16_t(i));
				cur_x_off = em{ 0 };
			} else {
				cur_x_off = cur_x_off + width_per_column;
			}

			// position column
			if(col_settings.layout == column_layout::centered) {
				for(uint32_t j = col_start; j < i; ++j) {
					children[j]->position.x = children[j]->position.x + (available_size - cur_height) / 2;
				}
			} else if(col_settings.layout == column_layout::bottom) {
				for(uint32_t j = col_start; j < i; ++j) {
					children[j]->position.x = children[j]->position.x + (available_size - cur_height);
				}
			}

			cur_height = em{ 0 };
			col_start = i;
			max_width = col_settings.minimum_width;

			if(group_start == col_start) { // tried to fit the whole group in a column but it was impossible: leave group in place and start a new group
				group_start = i;
				--i; // restart
			} else if(group_start == -1) {
				--i; // restart
			} else {
				i = group_start - 1;
			}
		} else if(!child_fits_in_column && (i == col_start || group_start  == col_start) && cur_col != 0 && col_settings.number_of_columns == 0) { // try new page
			
			page_starts.push_back(uint16_t(i));
			cur_height = em{ 0 };
			cur_x_off = em{ 0 };
			col_start = i;
			max_width = col_settings.minimum_width;

			if(group_start == -1) {
				--i; // restart
			} else {
				i = group_start - 1;
			}
		} else {
			if((children[i]->behavior_flags & behavior::layout_end_group) != 0) {
				group_start = -1;
			}

			children[i]->position.x = cur_x_off;
			children[i]->position.y = cur_height;
			cur_height = cur_height + children[i]->position.height;
			max_width = std::max(max_width, children[i]->position.width);
		}
	}

	if(col_settings.number_of_columns == 0) {
		// finish last col width
		for(uint32_t j = col_start; j < i; ++j) {
			children[j]->force_resize(r, layout_position{ max_width, children[j]->position.height });
		}
	}
	//  position last column
	if(col_settings.layout == column_layout::centered) {
		for(uint32_t j = col_start; j < i; ++j) {
			children[j]->position.x = children[j]->position.x + (available_size - cur_height) / 2;
		}
	} else if(col_settings.layout == column_layout::bottom) {
		for(uint32_t j = col_start; j < i; ++j) {
			children[j]->position.x = children[j]->position.x + (available_size - cur_height);
		}
	}

	num_pages = uint16_t(page_starts.size() + 1);
	current_page = std::min(current_page, uint16_t(num_pages - 1));

	if(num_pages > 1) {
		page_controls->behavior_flags |= ~(behavior::visually_hidden | behavior::functionally_hidden);
	} else {
		page_controls->behavior_flags |= behavior::functionally_hidden;
	}
	page_controls->on_update(r);

	pending_relayout = false;
}
void dynamic_column::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	auto target_width = std::min(std::max(desired_width, minimum_width(r)), maximum_space.x);
	auto target_height = std::min(std::max(desired_height, minimum_height(r)), maximum_space.y);
	force_resize(r, layout_position{ target_width, target_height });
}
em dynamic_column::minimum_width(root& r) {
	em result{ 0 };
	bool horizontal = r.get_horizontal_orientation(ui_node::type_id);
	if(horizontal) {
		for(uint32_t i = 0; i < children.size(); ++i) {
			result = result + children[i]->minimum_width(r);
		}
	} else {
		for(uint32_t i = 0; i < children.size(); ++i) {
			result = std::max(result, children[i]->minimum_width(r));
		}
	}
	return std::max(result, page_controls->minimum_width(r));
}
em dynamic_column::minimum_height(root& r) {
	em result{ 0 };
	bool horizontal = r.get_horizontal_orientation(ui_node::type_id);
	if(!horizontal) {
		for(uint32_t i = 0; i < children.size(); ++i) {
			result = result + children[i]->minimum_height(r);
		}
	} else {
		for(uint32_t i = 0; i < children.size(); ++i) {
			result = std::max(result, children[i]->minimum_height(r));
		}
	}
	return std::max(result, page_controls->minimum_height(r));
}

page_information dynamic_column::get_page_information() {
	return page_information{ current_page, num_pages };
}
void dynamic_column::change_page(root& r, int32_t new_page) {
	if(current_page == uint16_t(new_page))
		return;

	if(!r.contains_focus(page_controls))
		r.back_out_focus(*this);

	{
		uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
		uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

		for(uint32_t i = page_start; i < page_end; ++i) {
			children[i]->on_hide(r);
		}
	}

	current_page = uint16_t(new_page);

	{
		uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
		uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

		for(uint32_t i = page_start; i < page_end; ++i) {
			children[i]->on_update(r);
			children[i]->on_visible(r);
		}
	}

	page_controls->on_update(r);
}
void dynamic_column::on_scroll(root& r, layout_position pos, int32_t amount) {
	change_page(r, std::clamp(current_page + amount, 0, int32_t(num_pages) - 1));
}
void dynamic_column::add_managed_element(root& r, ui_node* n) {
	children.push_back(n);
	pending_relayout = true;
}
void dynamic_column::reset_managed_elements(root& r) {
	r.back_out_focus(*this);
	for(auto c : children)
		r.release_node(c);
	children.clear();
	pending_relayout = true;
}
void dynamic_column::repaginate(root& r) {
	force_resize(r, layout_position{ position.width, position.height });
}

//
// monotype column
//

size_t monotype_column::size() const {
	return sizeof(monotype_column);
}
void monotype_column::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	render_background(r, r.get_background_definition(type_id), offset, *this);

	em last_height{ -1 };
	bool mode = false;
	bool in_group = false;
	auto col_settings = r.get_column_properties(ui_node::type_id);

	uint32_t page_start = page_size * current_page;
	uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));

	for(uint32_t i = 0; i < in_page; ++i) {
		if(children[i]->position.y <= last_height) {
			mode = false;
		}
		last_height = children[i]->position.y;

		if((children[i]->behavior_flags & (behavior::layout_as_section_header | behavior::layout_as_column_header | behavior::layout_as_separator)) != 0) {
			mode = false;
		}
		if((children[i]->behavior_flags & behavior::layout_start_group) != 0) {
			in_group = true;
		}
		if((children[i]->behavior_flags & behavior::layout_end_group) != 0) {
			in_group = false;
		}

		r.system.set_line_highlight_mode(mode);

		auto child_position = get_sub_position(*this, *children[i]) + offset;
		if((children[i]->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ children[i], child_position });
		} else {
			children[i]->render(r, child_position, postponed);
		}
		if(!in_group && (children[i]->behavior_flags & behavior::layout_as_separator) == 0) {
			mode = !mode;
		}
	}
	r.system.set_line_highlight_mode(false);
	{
		auto child_position = get_sub_position(*this, *page_controls) + offset;
		if((page_controls->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ page_controls, child_position });
		} else {
			page_controls->render(r, child_position, postponed);
		}
	}
}
uint32_t monotype_column::child_count() const {
	uint32_t page_start = page_size * current_page;
	uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));
	return in_page + ((page_controls->behavior_flags & behavior::visually_hidden) != 0 ? 0 : 1);
}
ui_node* monotype_column::get_child(uint32_t index) const {
	uint32_t page_start = page_size * current_page;
	uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));
	if(index < in_page)
		return children[index];
	else
		return page_controls;
}
probe_result monotype_column::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
		result.type_array[size_t(mouse_interactivity::scroll)].node = this;
		result.type_array[size_t(mouse_interactivity::scroll)].relative_location = probe_pos - offset;
	}

	uint32_t page_start = page_size * current_page;
	uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));

	for(uint32_t i = 0; i < in_page; ++i) {
		auto child_position = get_sub_position(*this, *children[i]) + offset;
		if((children[i]->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ children[i], child_position });
		} else {
			auto c_result = children[i]->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}
	{
		auto child_position = get_sub_position(*this, *page_controls) + offset;
		if((page_controls->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ page_controls, child_position });
		} else {
			auto c_result = page_controls->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}

	return result;
}
void monotype_column::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);

	uint32_t page_start = page_size * current_page;
	uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));

	for(uint32_t i = 0; i < in_page && i < children.size(); ++i) {
		children[i]->on_visible(r);
	}
}
void monotype_column::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);

	uint32_t page_start = page_size * current_page;
	uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));

	for(uint32_t i = 0; i < in_page && i < children.size(); ++i) {
		children[i]->on_hide(r);
	}
}
void monotype_column::on_gain_focus(root& r) {
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void monotype_column::on_lose_focus(root& r) {
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void monotype_column::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	if(pending_data_update) {
		auto size = data->size();
		num_pages = (size + page_size - 1) / page_size;
		current_page = std::min(current_page, uint16_t(std::max(uint16_t(1), num_pages) - 1));

		uint32_t page_start = page_size * current_page;
		uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));

		auto item_type = r.get_child_data_type(ui_node::type_id);

		uint32_t i = 0;
		for(; i < in_page && i < children.size(); ++i) {
			auto dat = impl::get_local_data(r, children[i], item_type.variable);
			if(dat) {
				memcpy(dat, (*data)[i + page_start], datatype_size(item_type.data_type));
			}
			children[i]->behavior_flags &= ~behavior::functionally_hidden;
			children[i]->on_update(r);
		}
		for(; i < children.size(); ++i) {
			children[i]->behavior_flags |= behavior::functionally_hidden;
		}

		page_controls->on_update(r);
	}
}
void monotype_column::on_create(root& r) {
	data = make_vector_of(r.get_child_data_type(type_id).data_type);
	page_controls = make_page_controls(r, this);

	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}

	force_resize(r, layout_position{ position.width, position.height });

	page_controls->on_update(r);
}
void monotype_column::force_resize(root& r, layout_position size) {
	auto item_type = r.get_child_data_type(ui_node::type_id);

	position.width = size.x;
	position.height = size.y;

	auto col_settings = r.get_column_properties(ui_node::type_id);

	auto width_per_column = col_settings.number_of_columns != 0 ? size.x / col_settings.number_of_columns : em{ 0 };

	int32_t cur_col = 0;
	int32_t col_start = 0;
	em cur_height{ 0 };
	em cur_x_off{ 0 };

	page_controls->resize(r, size, em{ 0 }, em{ 0 });
	auto available_size = size.y - page_controls->position.height;

	int32_t i = 0;
	for(; true; ++i) {
		bool child_fits_in_column = true;
		if(i >= children.size()) {
			children.push_back(r.make_control_by_type(this, item_type.child_control_type));
		}
		if(width_per_column == em{ 0 }) {
			auto item_min = children[0]->minimum_width(r);
			auto cols = (size.x.value * 100) / item_min.value;
			col_settings.number_of_columns = int8_t(cols);
			width_per_column = size.x / cols;
		}
			
		children[i]->resize(r, minui::layout_position{ width_per_column, available_size - cur_height }, width_per_column, em{ 0 });
		if(children[i]->position.height > available_size - cur_height)
			child_fits_in_column = false;

		if(!child_fits_in_column && i != col_start) {
			++cur_col;
			if(cur_col >= col_settings.number_of_columns) { // new page
				break;
			} else {
				cur_x_off = cur_x_off + width_per_column;
			}

			// position column
			if(col_settings.layout == column_layout::centered) {
				for(uint32_t j = col_start; j < i; ++j) {
					children[j]->position.x = children[j]->position.x + (available_size - cur_height) / 2;
				}
			} else if(col_settings.layout == column_layout::bottom) {
				for(uint32_t j = col_start; j < i; ++j) {
					children[j]->position.x = children[j]->position.x + (available_size - cur_height);
				}
			}

			cur_height = em{ 0 };
			col_start = i;

			
			--i; // restart
		} else {
			children[i]->position.x = cur_x_off;
			children[i]->position.y = cur_height;
			cur_height = cur_height + children[i]->position.height;
		}
	}

	//  position last column
	if(col_settings.layout == column_layout::centered) {
		for(uint32_t j = col_start; j < i; ++j) {
			children[j]->position.x = children[j]->position.x + (available_size - cur_height) / 2;
		}
	} else if(col_settings.layout == column_layout::bottom) {
		for(uint32_t j = col_start; j < i; ++j) {
			children[j]->position.x = children[j]->position.x + (available_size - cur_height);
		}
	}

	page_size = uint16_t(i);
	num_pages = uint16_t((data->size() + page_size - 1) / page_size);
	current_page = std::min(current_page, uint16_t(num_pages - 1));

	if(num_pages > 1) {
		page_controls->behavior_flags |= ~(behavior::visually_hidden | behavior::functionally_hidden);
	} else {
		page_controls->behavior_flags |= behavior::functionally_hidden;
	}

	uint32_t page_start = page_size * current_page;
	uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));

	uint32_t i = 0;
	for(; i < in_page; ++i) {
		auto dat = impl::get_local_data(r, children[i], item_type.variable);
		if(dat) {
			memcpy(dat, (*data)[i + page_start], datatype_size(item_type.data_type));
		}
		children[i]->behavior_flags &= ~behavior::functionally_hidden;
		children[i]->on_update(r);
	}
	for(; i < children.size(); ++i) {
		children[i]->behavior_flags |= behavior::functionally_hidden;
	}
	pending_data_update = false;
	page_controls->on_update(r);
}
void monotype_column::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	auto target_width = std::min(std::max(desired_width, minimum_width(r)), maximum_space.x);
	auto target_height = std::min(std::max(desired_height, minimum_height(r)), maximum_space.y);
	force_resize(r, layout_position{ target_width, target_height });
}
em monotype_column::minimum_width(root& r) {
	if(children.empty()) {
		auto item_type = r.get_child_data_type(ui_node::type_id);
		children.push_back(r.make_control_by_type(this, item_type.child_control_type));
	}

	return std::max(page_controls->minimum_width(r), children[0]->minimum_width(r));
}
em monotype_column::minimum_height(root& r) {
	if(children.empty()) {
		auto item_type = r.get_child_data_type(ui_node::type_id);
		children.push_back(r.make_control_by_type(this, item_type.child_control_type));
	}

	return page_controls->minimum_height(r) + children[0]->minimum_height(r);
}
page_information monotype_column::get_page_information() {
	return page_information{ current_page, num_pages };
}
void monotype_column::change_page(root& r, int32_t new_page) {
	if(current_page == uint16_t(new_page))
		return;

	if(!r.contains_focus(page_controls))
		r.back_out_focus(*this);

	current_page = uint16_t(new_page);

	uint32_t page_start = page_size * current_page;
	uint32_t in_page = std::min(uint32_t(data->size() - page_start), uint32_t(page_size));
	auto item_type = r.get_child_data_type(ui_node::type_id);

	uint32_t i = 0;
	for(; i < in_page && i < children.size(); ++i) {
		auto dat = impl::get_local_data(r, children[i], item_type.variable);
		if(dat) {
			memcpy(dat, (*data)[i + page_start], datatype_size(item_type.data_type));
		}
		children[i]->behavior_flags &= ~behavior::functionally_hidden;
		children[i]->on_update(r);
	}
	for(; i < children.size(); ++i) {
		children[i]->behavior_flags |= behavior::functionally_hidden;
	}
	pending_data_update = false;
	page_controls->on_update(r);
}
void monotype_column::on_scroll(root& r, layout_position pos, int32_t amount) {
	change_page(r, std::clamp(current_page + amount, 0, int32_t(num_pages) - 1));
}
void monotype_column::add_item(void const* d) {
	data->push_back(d);
	pending_data_update = true;
}
void monotype_column::clear_contents() {
	data->clear();
	pending_data_update = true;
}

//
// panes_set
//

size_t panes_set::size() const {
	return sizeof(panes_set);
}
void panes_set::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	auto child_position = get_sub_position(*this, *children[selected]) + offset;
	if((children[selected]->behavior_flags & behavior::pop_up) != 0) {
		postponed.push_back(postponed_render{ children[selected], child_position });
	} else {
		children[selected]->render(r, child_position, postponed);
	}
}
uint32_t panes_set::child_count() const {
	return uint32_t(1);
}
ui_node* panes_set::get_child(uint32_t index) const {
	return children[selected];
}
probe_result panes_set::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
	}

	auto child_position = get_sub_position(*this, *children[selected]) + offset;
	if((children[selected]->behavior_flags & behavior::pop_up) != 0) {
		postponed.push_back(postponed_render{ children[selected], child_position });
	} else {
		auto c_result = children[selected]->mouse_probe(r, probe_pos, child_position, postponed);
		for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
			if(c_result.type_array[i].node) {
				result.type_array[i] = c_result.type_array[i];
			}
		}
	}
	
	return result;
}
void panes_set::on_gain_focus(root& r) {
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void panes_set::on_lose_focus(root& r) {
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void panes_set::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);
	children[selected]->on_visible(r);
}
void panes_set::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);
	children[selected]->on_hide(r);
}
void panes_set::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	children[selected]->on_update(r);
}
void panes_set::on_create(root& r) {
	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}

	auto c = r.get_fixed_children(type_id);
	while(c.start != c.end) {
		auto n = r.make_control_by_type(this, *(c.start));
		children.push_back(n);
		++c.start;
	}

	force_resize(r, layout_position{ position.width, position.height });

	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}
}
page_information panes_set::get_page_information() {
	return page_information{ selected, uint16_t(children.size()) };
}
void panes_set::change_page(root& r, int32_t new_page) {
	if(new_page == selected)
		return;

	r.back_out_focus(*this);

	children[selected]->on_hide(r);
	selected = uint16_t(new_page);
	children[selected]->on_update(r);
	children[selected]->on_visible(r);
}

void panes_set::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;
	for(auto c : children) {
		c->force_resize(r, size);
	}
}
void panes_set::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	for(auto c : children) {
		c->resize(r, maximum_space, desired_width, desired_height);
	}
	em maxwidth{ 0 };
	em maxheight{ 0 };
	for(auto c : children) {
		maxwidth = std::max(maxwidth, c->position.width);
		maxheight = std::max(maxheight, c->position.height);
	}
	for(auto c : children) {
		c->force_resize(r, layout_position{ maxwidth, maxheight });
	}
	position.width = maxwidth;
	position.height = maxheight;
}
em panes_set::minimum_width(root& r) {
	em result{ 0 };
	for(auto c : children) {
		result = std::max(result, c->minimum_width(r));
	}
	return result;
}
em panes_set::minimum_height(root& r) {
	em result{ 0 };
	for(auto c : children) {
		result = std::max(result, c->minimum_height(r));
	}
	return result;
}

//
// layers
//

size_t layers::size() const {
	return sizeof(layers);
}
void layers::render(root& r, layout_position offset, std::vector<postponed_render>& ) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	std::vector<postponed_render> postponed;
	for(uint32_t i = 0; i < children.size(); ++i) {
		auto child_position = get_sub_position(*this, *children[i]) + offset;
		children[i]->render(r, child_position, postponed);

		for(uint32_t j = 0; j < postponed.size(); ++j) {
			postponed[j].n->render(r, postponed[j].offset, postponed);
		}
		postponed.clear();
	}
}
uint32_t layers::child_count() const {
	return uint32_t(children.size());
}
ui_node* layers::get_child(uint32_t index) const {
	return children[index];
}
probe_result layers::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>&) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
	}

	std::vector<postponed_render> postponed;
	for(uint32_t i = 0; i < children.size(); ++i) {
		{
			auto child_position = get_sub_position(*this, *children[i]) + offset;
			auto c_result = children[i]->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}

		for(uint32_t j = 0; j < postponed.size(); ++j) {
			auto c_result = postponed[j].n->mouse_probe(r, probe_pos, postponed[j].offset, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
		postponed.clear();
	}

	return result;
}
void layers::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	for(uint32_t i = 0; i < children.size(); ++i) {
		children[i]->on_update(r);
	}
}
void layers::on_visible(root& r) {
	for(auto c : children)
		c->on_visible(r);
}
void layers::on_hide(root& r) {
	for(auto c : children)
		c->on_hide(r);
}
void layers::on_gain_focus(root& r) {
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void layers::on_lose_focus(root& r) {
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void layers::on_create(root& r) {
	auto c = r.get_fixed_children(type_id);
	while(c.start != c.end) {
		auto n = r.make_control_by_type(this, *(c.start));
		children.push_back(n);
		++c.start;
	}

	force_resize(r, layout_position{ position.width, position.height });
}

void layers::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;
	for(auto c : children) {
		c->force_resize(r, size);
	}
}
void layers::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	for(auto c : children) {
		c->resize(r, maximum_space, desired_width, desired_height);
	}
	em maxwidth{ 0 };
	em maxheight{ 0 };
	for(auto c : children) {
		maxwidth = std::max(maxwidth, c->position.width);
		maxheight = std::max(maxheight, c->position.height);
	}
	for(auto c : children) {
		c->force_resize(r, layout_position{ maxwidth, maxheight });
	}
	position.width = maxwidth;
	position.height = maxheight;
}
em layers::minimum_width(root& r) {
	em result{ 0 };
	for(auto c : children) {
		result = std::max(result, c->minimum_width(r));
	}
	return result;
}
em layers::minimum_height(root& r) {
	em result{ 0 };
	for(auto c : children) {
		result = std::max(result, c->minimum_height(r));
	}
	return result;
}


//
// dynamic_grid
//
size_t dynamic_grid::size() const {
	return sizeof(dynamic_grid);
}
void dynamic_grid::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	if(pending_relayout)
		repaginate(r);

	render_background(r, r.get_background_definition(type_id), offset, *this);

	em last_height{ -1 };
	bool mode = false;
	bool in_group = false;

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		if(children[i]->position.y <= last_height) {
			mode = false;
		}
		last_height = children[i]->position.y;

		if((children[i]->behavior_flags & (behavior::layout_as_section_header | behavior::layout_as_column_header | behavior::layout_as_separator)) != 0) {
			mode = false;
		}
		if((children[i]->behavior_flags & behavior::layout_start_group) != 0) {
			in_group = true;
		}
		if((children[i]->behavior_flags & behavior::layout_end_group) != 0) {
			in_group = false;
		}

		auto child_position = get_sub_position(*this, *children[i]) + offset;
		if((children[i]->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ children[i], child_position });
		} else {
			children[i]->render(r, child_position, postponed);
		}
		if(!in_group && (children[i]->behavior_flags & behavior::layout_as_separator) == 0) {
			mode = !mode;
		}
	}

	{
		auto child_position = get_sub_position(*this, *page_controls) + offset;
		if((page_controls->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ page_controls, child_position });
		} else {
			page_controls->render(r, child_position, postponed);
		}
	}
}
uint32_t dynamic_grid::child_count() const {
	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);
	return page_end - page_start + ((page_controls->behavior_flags & behavior::visually_hidden) != 0 ? 0 : 1);
}
ui_node* dynamic_grid::get_child(uint32_t index) const {
	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);
	if(index < page_end - page_start)
		return children[page_start + index];
	else
		return page_controls;
}
probe_result dynamic_grid::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
		result.type_array[size_t(mouse_interactivity::scroll)].node = this;
		result.type_array[size_t(mouse_interactivity::scroll)].relative_location = probe_pos - offset;
	}

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		auto child_position = get_sub_position(*this, *children[i]) + offset;
		if((children[i]->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ children[i], child_position });
		} else {
			auto c_result = children[i]->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}
	{
		auto child_position = get_sub_position(*this, *page_controls) + offset;
		if((page_controls->behavior_flags & behavior::pop_up) != 0) {
			postponed.push_back(postponed_render{ page_controls, child_position });
		} else {
			auto c_result = page_controls->mouse_probe(r, probe_pos, child_position, postponed);
			for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
				if(c_result.type_array[i].node) {
					result.type_array[i] = c_result.type_array[i];
				}
			}
		}
	}

	return result;
}
void dynamic_grid::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		children[i]->on_visible(r);
	}
}
void dynamic_grid::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		children[i]->on_hide(r);
	}
}
void dynamic_grid::on_gain_focus(root& r) {
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void dynamic_grid::on_lose_focus(root& r) {
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void dynamic_grid::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);

	if(pending_relayout)
		repaginate(r);

	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
	uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

	for(uint32_t i = page_start; i < page_end; ++i) {
		children[i]->on_update(r);
	}

	page_controls->on_update(r);
}
void dynamic_grid::on_create(root& r) {
	page_controls = make_page_controls(r, this);

	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}

	force_resize(r, layout_position{ position.width, position.height });

	page_controls->on_update(r);
}
void dynamic_grid::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;

	auto page_s = r.get_page_ui_definitions(ui_node::type_id);

	page_starts.clear();

	em cur_height{ 0 };
	em cur_x_off{ 0 };

	em max_width = size.x;
	em max_height = size.y;

	page_controls->resize(r, size, em{ 0 }, em{ 0 });

	if(page_s.vertical_arrangement) {
		max_height = max_height - page_controls->position.height;
	} else {
		max_width = max_width - page_controls->position.width;
	}

	int32_t cur_page = 0;
	int32_t row_start = 0;
	int32_t page_start = 0;

	em row_height{ 0 };

	int32_t i = 0;
	for(; i < int32_t(children.size()); ++i) {
		if((children[i]->behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
			continue;
		if((children[i]->behavior_flags & behavior::layout_as_separator) != 0 && row_start == i) {
			++row_start;
			continue;
		}

		bool child_fits_in_row = (children[i]->behavior_flags & behavior::layout_as_column_header) == 0;
		bool row_fits_in_page = true;

		children[i]->resize(r, minui::layout_position{ max_width - cur_x_off, max_height - cur_height }, em{ 0 }, em{ 0 });
		if(children[i]->position.width > max_width - cur_x_off) {
			child_fits_in_row = false;
		}
		if(children[i]->position.height > max_height - cur_height) {
			row_fits_in_page = false;
		}

		if((child_fits_in_row && row_fits_in_page) || (!child_fits_in_row && row_fits_in_page && row_start == i) || (!row_fits_in_page && page_start == i)) {
			children[i]->position.x = cur_x_off;
			children[i]->position.y = cur_height;

			row_height = std::max(row_height, children[i]->position.height);
			cur_x_off = cur_x_off + children[i]->position.width;
		} else if(!row_fits_in_page) {
			page_starts.push_back(uint16_t(i));
			page_start = i;
			row_start = i;
			cur_x_off = em{ 0 };
			cur_height = em{ 0 };
			
			// position row
			for(uint32_t j = row_start; j < i; ++j) {
				children[j]->position.x = children[j]->position.x + (row_height - children[j]->position.height) / 2;
			}
			row_height = em{ 0 };

			--i; // restart
		} else { // ! child fits in row
			row_start = i;
			cur_x_off = em{ 0 };
			cur_height = cur_height + row_height;
			for(uint32_t j = row_start; j < i; ++j) {
				children[j]->position.x = children[j]->position.x + (row_height - children[j]->position.height) / 2;
			}
			row_height = em{ 0 };
			--i; // restart
		}
	}

	for(uint32_t j = row_start; j < i; ++j) {
		children[j]->position.x = children[j]->position.x + (row_height - children[j]->position.height) / 2;
	}

	num_pages = uint16_t(page_starts.size() + 1);
	current_page = std::min(current_page, uint16_t(num_pages - 1));

	if(num_pages > 1) {
		page_controls->behavior_flags |= ~(behavior::visually_hidden | behavior::functionally_hidden);
	} else {
		page_controls->behavior_flags |= behavior::functionally_hidden;
	}
	page_controls->on_update(r);

	pending_relayout = false;
}
void dynamic_grid::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	auto target_width = std::min(std::max(desired_width, minimum_width(r)), maximum_space.x);
	auto target_height = std::min(std::max(desired_height, minimum_height(r)), maximum_space.y);
	force_resize(r, layout_position{ target_width, target_height });
}
em dynamic_grid::minimum_width(root& r) {
	em result{ 0 };
	bool vert = r.get_page_ui_definitions(ui_node::type_id).vertical_arrangement;
	
	for(uint32_t i = 0; i < children.size(); ++i) {
		result = std::max(result, children[i]->minimum_width(r));
	}
	
	return vert ? std::max(result, page_controls->minimum_width(r)) : (result + page_controls->minimum_width(r));
}
em dynamic_grid::minimum_height(root& r) {
	em result{ 0 };
	bool vert = r.get_page_ui_definitions(ui_node::type_id).vertical_arrangement;

	for(uint32_t i = 0; i < children.size(); ++i) {
		result = std::max(result, children[i]->minimum_height(r));
	}

	return !vert ? std::max(result, page_controls->minimum_height(r)) : (result + page_controls->minimum_height(r));
}

page_information dynamic_grid::get_page_information() {
	return page_information{ current_page, num_pages };
}
void dynamic_grid::change_page(root& r, int32_t new_page) {
	if(current_page == uint16_t(new_page))
		return;

	if(!r.contains_focus(page_controls))
		r.back_out_focus(*this);

	{
		uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
		uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

		for(uint32_t i = page_start; i < page_end; ++i) {
			children[i]->on_hide(r);
		}
	}

	current_page = uint16_t(new_page);

	{
		uint32_t page_start = (current_page == 0 ? 0 : page_starts[current_page - 1]);
		uint32_t page_end = (current_page >= int32_t(num_pages) - 1 ? uint32_t(children.size()) : page_starts[current_page]);

		for(uint32_t i = page_start; i < page_end; ++i) {
			children[i]->on_update(r);
			children[i]->on_visible(r);
		}
	}

	page_controls->on_update(r);
}
void dynamic_grid::on_scroll(root& r, layout_position pos, int32_t amount) {
	change_page(r, std::clamp(current_page + amount, 0, int32_t(num_pages) - 1));
}
void dynamic_grid::add_managed_element(root& r, ui_node* n) {
	children.push_back(n);
	pending_relayout = true;
}
void dynamic_grid::reset_managed_elements(root& r) {
	r.back_out_focus(*this);
	for(auto c : children)
		r.release_node(c);
	children.clear();
	pending_relayout = true;
}
void dynamic_grid::repaginate(root& r) {
	force_resize(r, layout_position{ position.width, position.height });
}

//
// static_text
//

size_t static_text::size() const {
	return sizeof(static_text);
}
void static_text::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	render_background(r, r.get_background_definition(type_id), offset, *this);

	layout_rect rct{ offset.x + margins.x, offset.y + margins.y, position.width - (margins.x + margins.width), position.height - (margins.y + margins.height) };
	text_data->render(r.system, rct, r.get_foreground_brush(ui_node::type_id));
}
probe_result static_text::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	return probe_result{ };
}
void static_text::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);
}
void static_text::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);
}
void static_text::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);
}
void static_text::on_create(root& r) {
	text_data = r.system.make_text(*this);
	auto data = r.get_text_information(ui_node::type_id);
	margins = data.margins;
	text_data->set_font(r.system, data.font);

	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}

	force_resize(r, layout_position{ position.width, position.height });
}
void static_text::on_reload(root& r) {
	auto data = r.get_text_information(ui_node::type_id);
	text_data->set_font(r.system, data.font);

	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
}
void static_text::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;
}
void static_text::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	auto data = r.get_text_information(ui_node::type_id);
	auto lh = text_data->get_line_height(r.system);

	if(data.multiline) {
		desired_width = std::min(std::max(desired_width, data.margins.x + data.margins.width + data.minimum_space), maximum_space.x);
		text_data->resize_to_width(r.system, r.system.to_screen_space(desired_width - (data.margins.x + data.margins.width)));
		desired_height = std::max(desired_height, lh * std::max(1, text_data->get_number_of_text_lines(r.system)));
	} else {
		auto text_bounds = text_data->get_single_line_width(r.system);
		desired_height = std::max(desired_height, lh);
		desired_width = std::min(std::max(desired_width, std::max(text_bounds, data.minimum_space) + data.margins.x + data.margins.width), maximum_space.x);
	}

	force_resize(r, layout_position{ desired_width, desired_height });
}
em static_text::minimum_width(root& r) {
	auto data = r.get_text_information(ui_node::type_id);
	auto text_bounds = text_data->get_single_line_width(r.system);

	if(data.multiline) {
		return (data.margins.x + data.margins.width + data.minimum_space);
	} else {
		return std::max(text_bounds, data.minimum_space) + data.margins.x + data.margins.width;
	}
}
em static_text::minimum_height(root& r) {
	auto lh = text_data->get_line_height(r.system);
	return lh * std::max(1, text_data->get_number_of_text_lines(r.system));
}

//
// text_button
//

size_t text_button::size() const {
	return sizeof(text_button);
}
void text_button::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	render_background(r, r.get_background_definition(type_id), offset, *this, enabled ? rendering_modifiers::none : rendering_modifiers::disabled);

	auto ico_pos = r.get_icon_position(ui_node::type_id);
	
	if(r.pmode == prompt_mode::hidden || (ui_node::behavior_flags & behavior::interaction_flagged) == 0) {
		r.system.icon(r.get_icon(ui_node::type_id),
			screen_space_rect{
				r.system.to_screen_space(offset.x + ico_pos.x), r.system.to_screen_space(offset.y + ico_pos.y),
				r.system.to_screen_space(em{ 100 }), r.system.to_screen_space(em{ 100 })
			},
			r.get_foreground_brush(ui_node::type_id),
			enabled ? rendering_modifiers::none : rendering_modifiers::disabled);
	}


	layout_rect rct{ offset.x + margins.x, offset.y + margins.y, position.width - (margins.x + margins.width), position.height - (margins.y + margins.height) };
	text_data->render(r.system, rct, r.get_foreground_brush(ui_node::type_id), enabled ? rendering_modifiers::none : rendering_modifiers::disabled);
}
probe_result text_button::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
		result.type_array[size_t(mouse_interactivity::button)].node = this;
		result.type_array[size_t(mouse_interactivity::button)].relative_location = probe_pos - offset;
	}

	return result;
}
void text_button::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);
}
void text_button::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);
}
void text_button::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);
}
void text_button::on_create(root& r) {
	text_data = r.system.make_text(*this);
	auto data = r.get_text_information(ui_node::type_id);
	text_data->set_font(r.system, data.font);
	margins = data.margins;

	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}

	force_resize(r, layout_position{ position.width, position.height });
}
void text_button::on_reload(root& r) {
	auto data = r.get_text_information(ui_node::type_id);
	text_data->set_font(r.system, data.font);

	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
}
void text_button::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;
}
void text_button::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	auto data = r.get_text_information(ui_node::type_id);
	auto lh = text_data->get_line_height(r.system);

	if(data.multiline) {
		desired_width = std::min(std::max(desired_width, data.margins.x + data.margins.width + data.minimum_space), maximum_space.x);
		text_data->resize_to_width(r.system, r.system.to_screen_space(desired_width - (data.margins.x + data.margins.width)));
		desired_height = std::max(desired_height, lh * std::max(1, text_data->get_number_of_text_lines(r.system)));
	} else {
		auto text_bounds = text_data->get_single_line_width(r.system);
		desired_height =std::max(desired_height, lh);
		desired_width = std::min(std::max(desired_width, std::max(text_bounds, data.minimum_space) + data.margins.x + data.margins.width), maximum_space.x);
	}

	force_resize(r, layout_position{ desired_width, desired_height });
}
em text_button::minimum_width(root& r) {
	auto data = r.get_text_information(ui_node::type_id);
	

	if(data.multiline) {
		return (data.margins.x + data.margins.width + data.minimum_space);
	} else {
		auto text_bounds = text_data->get_single_line_width(r.system);
		return std::max(text_bounds, data.minimum_space) + data.margins.x + data.margins.width;
	}
}
em text_button::minimum_height(root& r) {
	auto lh = text_data->get_line_height(r.system);
	return lh * std::max(1, text_data->get_number_of_text_lines(r.system));
}
void text_button::on_lbutton(root& r, layout_position pos) {
	if(enabled) {
		auto fn = r.get_user_mouse_fn_a(ui_node::type_id);
		r.system.play_sound(r.get_interaction_sound(type_id));
		fn(r, *this);
	}
}


//
// icon_button
//

size_t icon_button::size() const {
	return sizeof(icon_button);
}
void icon_button::recalculate_icon_position(root& r) {
	auto data = r.get_image_information(ui_node::type_id);
	auto h_space = position.width - (data.margins.x + data.margins.width);
	auto v_space = position.height - (data.margins.y + data.margins.height);

	if(data.fixed_horizontal_size != em{ 0 }) {
		icon_position.width = data.fixed_horizontal_size;
		if(data.h_alginment == text::content_alignment::leading) {
			icon_position.x = data.margins.x;
		} else if(data.h_alginment == text::content_alignment::trailing) {
			icon_position.x = (position.width - data.margins.width) - data.fixed_horizontal_size;
		} else {
			icon_position.x = data.margins.x + (h_space - data.fixed_horizontal_size) / 2;
		}
	} else {
		icon_position.width = h_space;
		icon_position.x = data.margins.x;
	}

	if(data.fixed_vertical_size != em{ 0 }) {
		icon_position.height = data.fixed_vertical_size;
		if(data.h_alginment == text::content_alignment::leading) {
			icon_position.y = data.margins.y;
		} else if(data.h_alginment == text::content_alignment::trailing) {
			icon_position.y = (position.height - data.margins.height) - data.fixed_vertical_size;
		} else {
			icon_position.y = data.margins.y + (v_space - data.fixed_vertical_size) / 2;
		}
	} else {
		icon_position.height = v_space;
		icon_position.y = data.margins.y;
	}
}
void icon_button::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	render_background(r, r.get_background_definition(type_id), offset, *this, enabled ? rendering_modifiers::none : rendering_modifiers::disabled);

	auto ico_pos = r.get_icon_position(ui_node::type_id);

	r.system.icon(
		r.get_icon(ui_node::type_id),
		r.system.to_screen_space(icon_position + offset),
		r.get_foreground_brush(ui_node::type_id),
		enabled ? rendering_modifiers::none : rendering_modifiers::disabled);
}
probe_result icon_button::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
		result.type_array[size_t(mouse_interactivity::button)].node = this;
		result.type_array[size_t(mouse_interactivity::button)].relative_location = probe_pos - offset;
	}

	return result;
}
void icon_button::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);
}
void icon_button::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);
}
void icon_button::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);
}
void icon_button::on_create(root& r) {
	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}

	force_resize(r, layout_position{ position.width, position.height });
}
void icon_button::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;

	recalculate_icon_position(r);
}
void icon_button::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	desired_height = std::min(std::max(desired_height, minimum_height(r)), maximum_space.y);
	desired_width = std::min(std::max(desired_width, minimum_width(r)), maximum_space.x);

	force_resize(r, layout_position{ desired_width, desired_height });
}
em icon_button::minimum_width(root& r) {
	auto data = r.get_image_information(ui_node::type_id);
	if(data.fixed_horizontal_size != em{ 0 }) {
		return data.fixed_horizontal_size + data.margins.x + data.margins.width;
	} else {
		return em{ 100 } + data.margins.x + data.margins.width;
	}
}
em icon_button::minimum_height(root& r) {
	auto data = r.get_image_information(ui_node::type_id);
	if(data.fixed_vertical_size != em{ 0 }) {
		return data.fixed_vertical_size + data.margins.y + data.margins.height;
	} else {
		return em{ 100 } + data.margins.y + data.margins.height;
	}
}
void icon_button::on_lbutton(root& r, layout_position pos) {
	if(enabled) {
		r.system.play_sound(r.get_interaction_sound(type_id));
		auto fn = r.get_user_mouse_fn_a(ui_node::type_id);
		fn(r, *this);
	}
}

//
// edit_control
//

size_t edit_control::size() const {
	return sizeof(edit_control);
}
void edit_control::render(root& r, layout_position offset, std::vector<postponed_render>& postponed) {
	if((ui_node::behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return;

	render_background(r, r.get_background_definition(type_id), offset, *this);

	auto ico_pos = r.get_icon_position(ui_node::type_id);

	if(r.contains_focus(this)) {
		r.system.icon(
			r.get_icon(ui_node::type_id),
			screen_space_rect{
				r.system.to_screen_space(offset.x + ico_pos.x), r.system.to_screen_space(offset.y + ico_pos.y),
				r.system.to_screen_space(em{ 100 }), r.system.to_screen_space(em{ 100 })
			},
			r.get_highlight_brush(ui_node::type_id),
			text_data->is_read_only() ? rendering_modifiers::disabled : rendering_modifiers::none);
	} else if(r.pmode == prompt_mode::hidden || (ui_node::behavior_flags & behavior::interaction_flagged) == 0) {
		r.system.icon(
			r.get_icon(ui_node::type_id),
			screen_space_rect{
				r.system.to_screen_space(offset.x + ico_pos.x), r.system.to_screen_space(offset.y + ico_pos.y),
				r.system.to_screen_space(em{ 100 }), r.system.to_screen_space(em{ 100 })
			},
			r.get_foreground_brush(ui_node::type_id),
			text_data->is_read_only() ? rendering_modifiers::disabled : rendering_modifiers::none);
	}
	
	layout_rect rct{ offset.x + margins.x, offset.y + margins.y, position.width - (margins.x + margins.width), position.height - (margins.y + margins.height) };
	text_data->render(r.system, rct, r.get_foreground_brush(ui_node::type_id));
}
probe_result edit_control::mouse_probe(root& r, layout_position probe_pos, layout_position offset, std::vector<postponed_render>& postponed) {
	probe_result result;

	if(offset.x <= probe_pos.x && probe_pos.x < offset.x + ui_node::position.width) {
		if((ui_node::behavior_flags & behavior::transparent_to_focus) == 0) {
			result.type_array[size_t(mouse_interactivity::focus_target)].node = this;
			result.type_array[size_t(mouse_interactivity::focus_target)].relative_location = probe_pos - offset;
		}
		if((ui_node::behavior_flags & behavior::visually_interactable) != 0) {
			result.type_array[size_t(mouse_interactivity::position)].node = this;
			result.type_array[size_t(mouse_interactivity::position)].relative_location = probe_pos - offset;
		}
		result.type_array[size_t(mouse_interactivity::button)].node = this;
		result.type_array[size_t(mouse_interactivity::button)].relative_location = probe_pos - offset;
	}

	return result;
}
void edit_control::on_visible(root& r) {
	auto fn = r.get_on_visible(ui_node::type_id);
	fn(r, *this);
}
void edit_control::on_hide(root& r) {
	auto fn = r.get_on_hide(ui_node::type_id);
	fn(r, *this);
}
void edit_control::on_gain_focus(root& r) {
	r.edit_target = text_data.get();
	text_data->on_focus(r.system);
	auto fn = r.get_on_gain_focus(ui_node::type_id);
	fn(r, *this);
}
void edit_control::on_lose_focus(root& r) {
	r.edit_target = nullptr;
	text_data->on_lose_focus(r.system);
	auto fn = r.get_on_lose_focus(ui_node::type_id);
	fn(r, *this);
}
void edit_control::on_text_update(root& r) {
	auto fn = r.get_user_fn_a(ui_node::type_id);
	fn(r, *this);
}
void edit_control::on_update(root& r) {
	if((ui_node::behavior_flags & behavior::functionally_hidden) != 0)
		return;

	auto fn = r.get_on_update(ui_node::type_id);
	fn(r, *this);
}
void edit_control::on_create(root& r) {
	text_data = r.system.make_editable_text(*this);
	auto data = r.get_text_information(ui_node::type_id);
	margins = data.margins;
	text_data->set_font(r.system, data.font);

	{
		auto fn = r.get_on_update(ui_node::type_id);
		fn(r, *this);
	}
	{
		auto fn = r.get_on_create(ui_node::type_id);
		fn(r, *this);
	}

	force_resize(r, layout_position{ position.width, position.height });
}
void edit_control::on_reload(root& r) {
	auto data = r.get_text_information(ui_node::type_id);
	text_data->set_font(r.system, data.font);
}
void edit_control::force_resize(root& r, layout_position size) {
	position.width = size.x;
	position.height = size.y;
}
void edit_control::resize(root& r, layout_position maximum_space, em desired_width, em desired_height) {
	auto data = r.get_text_information(ui_node::type_id);
	auto lh = text_data->get_line_height(r.system);

	if(data.multiline) {
		desired_width = std::min(std::max(desired_width, data.margins.x + data.margins.width + data.minimum_space), maximum_space.x);
		text_data->resize_to_width(r.system, r.system.to_screen_space(desired_width - (data.margins.x + data.margins.width)));
		desired_height = std::max(desired_height, lh * std::max(1, text_data->get_number_of_text_lines(r.system)));
	} else {
		auto text_bounds = text_data->get_single_line_width(r.system);
		desired_height = std::max(desired_height, lh);
		desired_width = std::min(std::max(desired_width, std::max(text_bounds, data.minimum_space) + data.margins.x + data.margins.width), maximum_space.x);
	}

	force_resize(r, layout_position{ desired_width, desired_height });
}
em edit_control::minimum_width(root& r) {
	auto data = r.get_text_information(ui_node::type_id);

	if(data.multiline) {
		return (data.margins.x + data.margins.width + data.minimum_space);
	} else {
		auto text_bounds = text_data->get_single_line_width(r.system);
		return std::max(text_bounds, data.minimum_space) + data.margins.x + data.margins.width;
	}
}
em edit_control::minimum_height(root& r) {
	auto lh = text_data->get_line_height(r.system);
	return lh * std::max(1, text_data->get_number_of_text_lines(r.system));
}

void root::load_font_definitions(char const* file_data, size_t file_size) {
	serialization::in_buffer b{ file_data, file_size };
	auto num_fonts = b.read<uint32_t>();
	for(uint32_t i = 0; i < num_fonts; ++i) {
		text::font main_slot_font;
		main_slot_font.name = b.read<std::wstring_view>();
		main_slot_font.span = b.read<float>();
		main_slot_font.weight = b.read<int32_t>();
		main_slot_font.top_leading = b.read<int32_t>();
		main_slot_font.bottom_leading = b.read<int32_t>();
		main_slot_font.scale = b.read<float>();
		main_slot_font.type = b.read<text::font_type>();
		main_slot_font.is_oblique = b.read<bool>();

		system.make_font_definition(text::font_handle{ uint16_t(i) }, std::move(main_slot_font));

		auto num_fallbacks = b.read<uint32_t>();
		for(uint32_t j = 0; j < num_fallbacks; ++j) {
			text::font_fallback fb;
			fb.name = b.read<std::wstring_view>();
			fb.scale = b.read<float>();
			auto vspn = b.read_variable<text::unicode_range>();
			fb.ranges = std::vector<text::unicode_range>(vspn.begin(), vspn.end());
			system.add_font_fallback(text::font_handle{ uint16_t(i) }, std::move(fb));
		}
	}
}

text::locale_description root::load_locale_description(char const* file_data, size_t file_size) {
	serialization::in_buffer b{ file_data, file_size };
	text::locale_description result;
	result.is_left_to_right = b.read<bool>();
	result.display_name = b.read<std::wstring_view>();
	return result;
}

void root::load_locale_data(native_string_view locale) {
	system.set_locale(locale);
	
	auto r = system.get_root_directory();
	auto locale_container_dir = r->open_directory(NATIVE("locale"));
	auto ind_locale_dir = locale_container_dir->open_directory(locale);

	auto locale_info_dat = ind_locale_dir->open_file(NATIVE("locale.dat"));
	auto linfo = load_locale_description(locale_info_dat->data(), locale_info_dat->size());
	system.set_ltr_mode(linfo.is_left_to_right);
	system.set_locale_name(linfo.display_name);

	auto all_files = ind_locale_dir->list_files();
	for(auto& f : all_files) {
		if(f->name().ends_with(NATIVE(".txt"))) {
			system.add_localization_file(f->name());
		} else if(f->name().ends_with(NATIVE(".ttf")) || f->name().ends_with(NATIVE(".otf"))) {
			system.add_font_file_to_collection(f->name());
		} else { // ignore: .dat file

		}
	}

	auto locale_font_dat = ind_locale_dir->open_file(NATIVE("fonts.dat"));
	load_font_definitions(locale_info_dat->data(), locale_info_dat->size());

	system.finalize_font_collection();

	auto wintitle = system.perform_substitutions(system.get_hande("window_title"), nullptr, 0);
	system.set_window_title(wintitle.text_content.c_str());
}

ui_node* effective_focus_target(ui_node* in) {
	if(!in)
		return nullptr;

	if((in->behavior_flags | behavior::interaction_focus) != 0)
		return in;
	if((in->behavior_flags | behavior::transparent_to_focus) != 0 && in->child_count() > 0)
		return in;
	if(!in->parent)
		return in;

	return effective_focus_target(in->parent);
}

bool root::on_char(native_char c) {
	if(edit_target) {
		edit_target->insert_codepoint(system, uint32_t(c));
		return true;
	} 

	bool positive_result = false;
	for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
		if(under_mouse.type_array[i].node) {
			positive_result = true;
		}
	}

	return positive_result;
}

bool root::on_mouse_move(layout_position p) {
	std::vector<postponed_render> pop_ups;

	latest_mouse_position = p;
	under_mouse = node_repository[0]->mouse_probe(*this, p, layout_position{ em{ 0 }, em{ 0 } }, pop_ups);

	for(uint32_t i = 0; i < pop_ups.size(); ++i) {
		auto c_result = pop_ups[i].n->mouse_probe(*this, p, pop_ups[i].offset, pop_ups);
		for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
			if(c_result.type_array[i].node) {
				under_mouse.type_array[i] = c_result.type_array[i];
			}
		}
	}

	[&]() {
		// moving a mouse in an edit control

		auto desired_focus_container = effective_focus_target(under_mouse.type_array[size_t(mouse_interactivity::focus_target)].node);
		ui_node* old_focus = nullptr;

		if(!focus_stack.empty()) {
			auto fe = focus_stack.back().l_interface;
			old_focus = fe;
			
			if(edit_target) {
				return; // skip focus moving logic
			}

			// focus shifting logic -- when something already is in focus

			if((fe->behavior_flags & (behavior::focus_lock_mouse | behavior::focus_lock_text)) != 0) {
				return; // can't shift -- 
			}

			//if((fe->behavior_flags & behavior::focus_sticky_mouse) != 0) {
			//
			//}

			while(!focus_stack.empty() && focus_stack.back().l_interface != desired_focus_container) {
				fe = focus_stack.back().l_interface;
				if((fe->behavior_flags & (behavior::focus_lock_mouse | behavior::focus_lock_text)) != 0) {
					return; // can't shift -- 
				}
				if((fe->behavior_flags & behavior::focus_sticky_mouse) != 0) {
					auto abs_position = workspace_placement(*fe);
					auto rel_position = p - abs_position;

					em xdistance_to_rect = em{ 0 } - rel_position.x;
					xdistance_to_rect = std::max(xdistance_to_rect, rel_position.x - fe->position.width);

					em ydistance_to_rect = em{ 0 } - rel_position.y;
					ydistance_to_rect = std::max(ydistance_to_rect, rel_position.y - fe->position.height);

					auto gdistance_to_rect = std::max(ydistance_to_rect, xdistance_to_rect);
					gdistance_to_rect = std::max(gdistance_to_rect, em{0 });

					if(focus_stack.back().tracking_margin >= gdistance_to_rect) { // closer to the rect than last we checked or inside it
						focus_stack.back().tracking_margin = gdistance_to_rect;
						return;
					}
				}


			}

			// fallthrough ? -> focus stack is now empty or targets desired container
		}

		change_focus(old_focus, desired_focus_container);
		focus_stack.clear();

		if(desired_focus_container) {
			int32_t index = -1;
			if(desired_focus_container != under_mouse.type_array[size_t(mouse_interactivity::focus_target)].node) {
				int32_t spos = 0;
				for(auto i : interactables(*desired_focus_container)) {
					if(i == under_mouse.type_array[size_t(mouse_interactivity::focus_target)].node) {
						index = spos;
						break;
					}
					++spos;
				}
			}

			if(index == -1) {
				focus_stack.push_back(stored_focus{ desired_focus_container, -1, -1, em{ 0 } });
				current_groupings_size = make_top_groups(*this, *desired_focus_container, current_focus_groupings.data());
			} else {
				auto cgroup = find_group_containing(*this, *desired_focus_container, grouping_range{ index, index + 1 });
				if(!cgroup) {
					focus_stack.push_back(stored_focus{ desired_focus_container, -1, -1, em{ 0 } });
					current_groupings_size = make_top_groups(*this, *desired_focus_container, current_focus_groupings.data());
				} else {
					focus_stack.push_back(stored_focus{ desired_focus_container, cgroup->start, cgroup->end, em{ 0 } });
					current_focus_groupings = divide_group(grouping_range{ cgroup->start, cgroup->end }, 12);
					current_groupings_size = 12;
				}
			}
		}

		repopulate_key_actions();
	}();

	bool positive_result = false;
	for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
		if(under_mouse.type_array[i].node) {
			positive_result = true;
		}
	}

	return positive_result;
}


bool root::on_mouse_lbutton(click_type t) {
	auto node = under_mouse.type_array[size_t(mouse_interactivity::button)].node;
	auto felement = under_mouse.type_array[size_t(mouse_interactivity::focus_target)].node;
	auto efn = effective_focus_target(node);
	if(top_focus(focus_stack) != node && top_focus(focus_stack) != efn) {
		set_window_focus(focus_tracker{ efn, -1, -1 });
	}
	if(node) {
		if(auto ei = node->get_interface(iface::editable_text); ei) {
			auto iedit = static_cast<ieditable_text*>(ei);
			auto rel_pos_base = latest_mouse_position - workspace_placement(*node);
			if(!system.get_ltr())
				rel_pos_base.x = node->position.width - rel_pos_base.x;
			auto rel_pos = system.to_screen_space(rel_pos_base);
			iedit->move_cursor_by_screen_pt(system, rel_pos, shift_down);

			if(t == click_type::singlec) {

			} else if(t == click_type::doublec) {
				iedit->send_command(system, ieditable_text::edit_command::select_current_word, shift_down);
			} else if(t == click_type::triplec) {
				iedit->send_command(system, ieditable_text::edit_command::select_current_section, shift_down);
			}
		} else {
			if(alt_down) {
				node->on_rbutton(*this, under_mouse.type_array[size_t(mouse_interactivity::button)].relative_location);
				if(last_mcommand_sent == mcommand::alt) {
					last_mcommand_target->on_rbutton_up(*this);
					last_mcommand_sent = mcommand::none;
				} else if(last_mcommand_sent == mcommand::primary) {
					last_mcommand_target->on_lbutton_up(*this);
					last_mcommand_sent = mcommand::none;
				}
				if(node->behavior_flags & behavior::interaction_hold) {
					last_mcommand_sent = mcommand::alt;
					last_mcommand_target = node;
				}
			} else if(shift_down) {
				if(node->behavior_flags & behavior::interaction_focus) {
					set_window_focus(focus_tracker{ node, -1, -1 });
				}
			} else {
				node->on_lbutton(*this, under_mouse.type_array[size_t(mouse_interactivity::button)].relative_location);
				if(last_mcommand_sent == mcommand::alt) {
					last_mcommand_target->on_rbutton_up(*this);
					last_mcommand_sent = mcommand::none;
				} else if(last_mcommand_sent == mcommand::primary) {
					last_mcommand_target->on_lbutton_up(*this);
					last_mcommand_sent = mcommand::none;
				}
				if(node->behavior_flags & behavior::interaction_hold) {
					last_mcommand_sent = mcommand::primary;
					last_mcommand_target = node;
				}
			}
		}
	}
	if(shift_down && felement && (felement->behavior_flags & behavior::interaction_focus) != 0) {
		if(!node || (node->behavior_flags & behavior::interaction_focus) == 0) {
			set_window_focus(focus_tracker{ felement, -1, -1 });
		}
	}
		

	bool positive_result = false;
	for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
		if(under_mouse.type_array[i].node) {
			positive_result = true;
		}
	}
	return positive_result;
}
bool root::on_mouse_rbutton() {
	auto node = under_mouse.type_array[size_t(mouse_interactivity::button)].node;
	if(node) {
		if(auto ei = node->get_interface(iface::editable_text); ei) {
		
		} else {
			node->on_rbutton(*this, under_mouse.type_array[size_t(mouse_interactivity::button)].relative_location);
			if(last_mcommand_sent == mcommand::alt) {
				last_mcommand_target->on_rbutton_up(*this);
				last_mcommand_sent = mcommand::none;
			} else if(last_mcommand_sent == mcommand::primary) {
				last_mcommand_target->on_lbutton_up(*this);
				last_mcommand_sent = mcommand::none;
			}
			if(node->behavior_flags & behavior::interaction_hold) {
				last_mcommand_sent = mcommand::alt;
				last_mcommand_target = node;
			}
		}
	}

	bool positive_result = false;
	for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
		if(under_mouse.type_array[i].node) {
			positive_result = true;
		}
	}
	return positive_result;
}
bool root::on_mouse_lbutton_up() {
	if(last_mcommand_sent == mcommand::alt) {
		last_mcommand_target->on_rbutton_up(*this);
		last_mcommand_sent = mcommand::none;
	} else if(last_mcommand_sent == mcommand::primary) {
		last_mcommand_target->on_lbutton_up(*this);
		last_mcommand_sent = mcommand::none;
	}
	last_mcommand_sent = mcommand::none;

	bool positive_result = false;
	for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
		if(under_mouse.type_array[i].node) {
			positive_result = true;
		}
	}
	return positive_result;
}
bool root::on_mouse_rbutton_up() {
	if(last_mcommand_sent == mcommand::alt) {
		last_mcommand_target->on_rbutton_up(*this);
		last_mcommand_sent = mcommand::none;
	} else if(last_mcommand_sent == mcommand::primary) {
		last_mcommand_target->on_lbutton_up(*this);
		last_mcommand_sent = mcommand::none;
	}
	last_mcommand_sent = mcommand::none;

	bool positive_result = false;
	for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
		if(under_mouse.type_array[i].node) {
			positive_result = true;
		}
	}
	return positive_result;
}
bool root::on_mouse_scroll(float amount) {
	auto node = under_mouse.type_array[size_t(mouse_interactivity::scroll)].node;
	if(node) {
		node->on_scroll(*this, under_mouse.type_array[size_t(mouse_interactivity::scroll)].relative_location, int32_t(std::round(amount)));
	}

	bool positive_result = false;
	for(size_t i = 0; i < size_t(mouse_interactivity::count); ++i) {
		if(under_mouse.type_array[i].node) {
			positive_result = true;
		}
	}
	return positive_result;
}

int32_t map_scancode_to_key(uint32_t in) {
	switch(in) {
		case 0:
			return 0x10;
		case 1:
			return 0x11;
		case 2:
			return 0x12;
		case 3:
			return 0x13;
		case 4:
			return 0x1E;
		case 5:
			return 0x1F;
		case 6:
			return 0x20;
		case 7:
			return 0x21;
		case 8:
			return 0x2C;
		case 9:
			return 0x2D;
		case 10:
			return 0x2E;
		case 11:
			return 0x2F;
	}
	
	return -1;
}

int32_t key_to_scancode(int32_t in) {
	switch(in) {
		case 0x10:
			return 0;
		case 0x11:
			return 1;
		case 0x12:
			return 2;
		case 0x13:
			return 3;
		case 0x1E:
			return 4;
		case 0x1F:
			return 5;
		case 0x20:
			return 6;
		case 0x21:
			return 7;
		case 0x2C:
			return 8;
		case 0x2D:
			return 9;
		case 0x2E:
			return 10;
		case 0x2F:
			return 11;
	}

	return -1;
}

bool root::on_key_down(uint32_t scancode, uint32_t vk_code, bool repeat) {
	if(repeat)
		return true;

	if(vk_code == VK_ESCAPE)
		take_key_action(focus_actions.escape);

	auto efn = top_focus(focus_stack);
	if(efn) {
		if(auto ei = efn->get_interface(iface::editable_text); ei) {
			return true;
		}
	}

	if(vk_code == VK_SHIFT) {
		shift_down = true;
		return true;
	}
	if(vk_code == VK_MENU) {
		alt_down = true;
		return true;
	}

	if(vk_code == VK_SPACE)
		take_key_action(focus_actions.escape);

	auto k = map_scancode_to_key(scancode);
	if(k == -1)
		return false;
	if(k >= focus_actions.valid_key_action_count)
		return false;

	if(std::holds_alternative< interaction>(focus_actions.button_actions[k])) {
		auto node = std::get<interaction>(focus_actions.button_actions[k]).node;
		if(alt_down) {
			node->on_rbutton(*this, under_mouse.type_array[size_t(mouse_interactivity::button)].relative_location);
			if(last_mcommand_sent == mcommand::alt) {
				last_mcommand_target->on_rbutton_up(*this);
				last_mcommand_sent = mcommand::none;
			} else if(last_mcommand_sent == mcommand::primary) {
				last_mcommand_target->on_lbutton_up(*this);
				last_mcommand_sent = mcommand::none;
			}
			if(node->behavior_flags & behavior::interaction_hold) {
				last_mcommand_sent = mcommand::alt;
				last_mcommand_target = node;
			}
		} else if(shift_down) {
			if(node->behavior_flags & behavior::interaction_focus) {
				set_window_focus(focus_tracker{ node, -1, -1 });
			}
		} else {
			node->on_lbutton(*this, under_mouse.type_array[size_t(mouse_interactivity::button)].relative_location);
			if(last_mcommand_sent == mcommand::alt) {
				last_mcommand_target->on_rbutton_up(*this);
				last_mcommand_sent = mcommand::none;
			} else if(last_mcommand_sent == mcommand::primary) {
				last_mcommand_target->on_lbutton_up(*this);
				last_mcommand_sent = mcommand::none;
			}
			if(node->behavior_flags & behavior::interaction_hold) {
				last_mcommand_sent = mcommand::primary;
				last_mcommand_target = node;
			}
		}
	} else {
		take_key_action(focus_actions.button_actions[k]);
	}

	return true;
}
bool root::on_key_up(uint32_t scancode, uint32_t vk_code) {
	auto efn = top_focus(focus_stack);
	if(efn) {
		if(auto ei = efn->get_interface(iface::editable_text); ei) {
			return true;
		}
	}

	if(last_mcommand_sent == mcommand::alt) {
		last_mcommand_target->on_rbutton_up(*this);
		last_mcommand_sent = mcommand::none;
	} else if(last_mcommand_sent == mcommand::primary) {
		last_mcommand_target->on_lbutton_up(*this);
		last_mcommand_sent = mcommand::none;
	}

	if(vk_code == VK_SHIFT) {
		shift_down = false;
		return true;
	}
	if(vk_code == VK_MENU) {
		alt_down = false;
		return true;
	}

	return true;
}

void root::load_definitions(char const* data, size_t size) {
	file_base = data;

	serialization::in_buffer buf(data, size);

	auto num_sounds = buf.read<uint16_t>();

	for(uint32_t i = 0; i < num_sounds; ++i) {
		sound_handle h{ int32_t(i) };
		system.load_sound(h, buf.read<std::wstring_view>());
	}

	auto num_brush = buf.read<uint16_t>();

	for(uint16_t i = 0; i < num_brush; ++i) {
		// regular slot
		{
			bool color_brush = buf.read<bool>();
			if(color_brush) {
				auto c = buf.read< brush_color>();
				system.add_color_brush(i, c, false);
			} else {
				auto c = buf.read< brush_color>();
				system.add_image_color_brush(i, buf.read<std::wstring_view>(), c, false);
			}
		}
		// disabled slot
		{
			bool color_brush = buf.read<bool>();
			if(color_brush) {
				auto c = buf.read< brush_color>();
				system.add_color_brush(i, c, true);
			} else {
				auto c = buf.read< brush_color>();
				system.add_image_color_brush(i, buf.read<std::wstring_view>(), c, true);
			}
		}
		// highlights
		float line_shading = buf.read<float>();
		float highlight_shading = buf.read<float>(); 
		float line_highlight_shading = buf.read<float>();
		system.set_brush_highlights(i, line_shading, highlight_shading, line_highlight_shading);
	}

	auto num_icons = buf.read<uint16_t>();
	for(uint32_t i = 0; i < num_icons; ++i) {
		icon_handle slot = icon_handle{ buf.read<int16_t>() };
		int32_t sub_index = buf.read<int16_t>();
		auto x_ems = buf.read<em>();
		auto y_ems = buf.read<em>();
		auto fn = buf.read<std::wstring_view>();
		bool is_svg = buf.read<bool>();
		if(is_svg) {
			system.add_svg_to_icon_slot(slot, fn, x_ems, y_ems, sub_index);
		} else {
			system.add_to_icon_slot(slot, fn, x_ems, y_ems, sub_index);
		}
	}

	auto num_images = buf.read<uint16_t>();
	for(uint32_t i = 0; i < num_images; ++i) {
		image_handle slot = image_handle{ buf.read<int16_t>() };
		int32_t sub_index = buf.read<int16_t>();
		auto x_ems = buf.read<em>();
		auto y_ems = buf.read<em>();
		auto fn = buf.read<std::wstring_view>();
		system.add_to_image_slot(slot, fn, x_ems, y_ems, sub_index);
	}

	defined_element_types = buf.read<uint32_t>();
	d_on_update.resize(defined_element_types, nullptr);
	d_on_gain_focus.resize(defined_element_types, nullptr);
	d_on_lose_focus.resize(defined_element_types, nullptr);
	d_on_visible.resize(defined_element_types, nullptr);
	d_on_hide.resize(defined_element_types, nullptr);
	d_on_create.resize(defined_element_types, nullptr);
	d_user_fn_a.resize(defined_element_types, nullptr);
	d_user_fn_b.resize(defined_element_types, nullptr);
	d_user_mouse_fn_a.resize(defined_element_types, nullptr);

	auto d_fixed_children_buckets = buf.read<uint32_t>();
	auto d_fixed_children_content = buf.read<uint32_t>();
	auto d_window_children_buckets = buf.read<uint32_t>();
	auto d_window_children_content = buf.read<uint32_t>();
	auto d_divider_index_buckets = buf.read<uint32_t>();
	auto d_divider_index_content = buf.read<uint32_t>();
	auto d_horizontal_orientation_buckets = buf.read<uint32_t>();
	auto d_horizontal_orientation_content = buf.read<uint32_t>();
	auto d_column_properties_buckets = buf.read<uint32_t>();
	auto d_column_properties_content = buf.read<uint32_t>();
	auto d_page_ui_definitions_buckets = buf.read<uint32_t>();
	auto d_page_ui_definitions_content = buf.read<uint32_t>();
	auto d_text_information_buckets = buf.read<uint32_t>();
	auto d_text_information_content = buf.read<uint32_t>();
	auto d_interaction_sound_buckets = buf.read<uint32_t>();
	auto d_interaction_sound_content = buf.read<uint32_t>();
	auto d_image_information_buckets = buf.read<uint32_t>();
	auto d_image_information_content = buf.read<uint32_t>();
	auto d_child_dt_buckets = buf.read<uint32_t>();
	auto d_child_dt_content = buf.read<uint32_t>();
	auto d_on_update_raw_buckets = buf.read<uint32_t>();
	auto d_on_update_raw_content = buf.read<uint32_t>();
	auto d_on_gain_focus_raw_buckets = buf.read<uint32_t>();
	auto d_on_gain_focus_raw_content = buf.read<uint32_t>();
	auto d_on_lose_focus_raw_buckets = buf.read<uint32_t>();
	auto d_on_lose_focus_raw_content = buf.read<uint32_t>();
	auto d_on_visible_raw_buckets = buf.read<uint32_t>();
	auto d_on_visible_raw_content = buf.read<uint32_t>();
	auto d_on_hide_raw_buckets = buf.read<uint32_t>();
	auto d_on_hide_raw_content = buf.read<uint32_t>();
	auto d_on_create_raw_buckets = buf.read<uint32_t>();
	auto d_on_create_raw_content = buf.read<uint32_t>();
	auto d_user_fn_a_raw_buckets = buf.read<uint32_t>();
	auto d_user_fn_a_raw_content = buf.read<uint32_t>();
	auto d_user_fn_b_raw_buckets = buf.read<uint32_t>();
	auto d_user_fn_b_raw_content = buf.read<uint32_t>();
	auto d_user_mouse_fn_a_raw_buckets = buf.read<uint32_t>();
	auto d_user_mouse_fn_a_raw_content = buf.read<uint32_t>();

	d_icon_position = buf.read_fixed<layout_position>(defined_element_types);
	d_default_position = buf.read_fixed<layout_rect>(defined_element_types);
	d_interactable_definition = buf.read_fixed<interactable_definition>(defined_element_types);
	d_icon = buf.read_fixed<icon_handle>(defined_element_types);
	d_class = buf.read_fixed<uint16_t>(defined_element_types);
	d_standard_flags = buf.read_fixed<uint32_t>(defined_element_types);
	d_foreground_brush = buf.read_fixed<uint16_t>(defined_element_types);
	d_background_brush = buf.read_fixed<uint16_t>(defined_element_types);
	d_highlight_brush = buf.read_fixed<uint16_t>(defined_element_types);
	d_info_brush = buf.read_fixed<uint16_t>(defined_element_types);

	{
		auto cspan = buf.read_fixed<decltype(d_fixed_children)::value_container_type>(d_fixed_children_content);
		auto bspan = buf.read_fixed<decltype(d_fixed_children)::bucket_type>(d_fixed_children_buckets);
		d_fixed_children = decltype(d_fixed_children)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_window_children)::value_container_type>(d_window_children_content);
		auto bspan = buf.read_fixed<decltype(d_window_children)::bucket_type>(d_window_children_buckets);
		d_window_children = decltype(d_window_children)(cspan, bspan);
	}

	d_variable_definition = buf.read_fixed<array_reference>(defined_element_types);
	d_total_variable_size = buf.read_fixed<uint16_t>(defined_element_types);
	d_background_definition = buf.read_fixed<background_definition>(defined_element_types);

	{
		auto cspan = buf.read_fixed<decltype(d_divider_index)::value_container_type>(d_divider_index_content);
		auto bspan = buf.read_fixed<decltype(d_divider_index)::bucket_type>(d_divider_index_buckets);
		d_divider_index = decltype(d_divider_index)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_horizontal_orientation)::value_container_type>(d_horizontal_orientation_content);
		auto bspan = buf.read_fixed<decltype(d_horizontal_orientation)::bucket_type>(d_horizontal_orientation_buckets);
		d_horizontal_orientation = decltype(d_horizontal_orientation)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_column_properties)::value_container_type>(d_column_properties_content);
		auto bspan = buf.read_fixed<decltype(d_column_properties)::bucket_type>(d_column_properties_buckets);
		d_column_properties = decltype(d_column_properties)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_page_ui_definitions)::value_container_type>(d_page_ui_definitions_content);
		auto bspan = buf.read_fixed<decltype(d_page_ui_definitions)::bucket_type>(d_page_ui_definitions_buckets);
		d_page_ui_definitions = decltype(d_page_ui_definitions)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_text_information)::value_container_type>(d_text_information_content);
		auto bspan = buf.read_fixed<decltype(d_text_information)::bucket_type>(d_text_information_buckets);
		d_text_information = decltype(d_text_information)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_interaction_sound)::value_container_type>(d_interaction_sound_content);
		auto bspan = buf.read_fixed<decltype(d_interaction_sound)::bucket_type>(d_interaction_sound_buckets);
		d_interaction_sound = decltype(d_interaction_sound)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_image_information)::value_container_type>(d_image_information_content);
		auto bspan = buf.read_fixed<decltype(d_image_information)::bucket_type>(d_image_information_buckets);
		d_image_information = decltype(d_image_information)(cspan, bspan);
	}
	{
		auto cspan = buf.read_fixed<decltype(d_child_data_type)::value_container_type>(d_child_dt_content);
		auto bspan = buf.read_fixed<decltype(d_child_data_type)::bucket_type>(d_child_dt_buckets);
		d_child_data_type = decltype(d_child_data_type)(cspan, bspan);
	}
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
}

bool node_is_visible(root& r, ui_node& n) {
	if((n.behavior_flags & (behavior::functionally_hidden | behavior::visually_hidden)) != 0)
		return false;
	if(&n == r.node_repository[0].get())
		return true;
	if(!n.parent)
		return false;
	return node_is_visible(r, *n.parent);
}

void root::on_update() {
	node_repository[0]->on_update(*this);

	if(system.is_mouse_cursor_visible())
		on_mouse_move(latest_mouse_position);
}

#undef minui_aligned_alloc
#undef minui_aligned_free

}
