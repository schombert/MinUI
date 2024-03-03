#pragma once

#include <stdint.h>
#include <array>
#include <vector>
#include <string_view>

#include "unordered_dense.h"

namespace minui {
namespace text {

struct keyed_entry {
	char const* raw_text = nullptr;

	std::array<text::attribute_type, text::max_attributes> lookup_attributes = { text::attribute_type::undefined };
	std::array<text::attribute_type, text::max_attributes> provided_attributes = { text::attribute_type::undefined };
	uint32_t string_data = 0;
	uint32_t formatting_data = 0;
	uint32_t match_set_entries = 0;
	uint32_t next_entry = 0;

	uint16_t raw_text_length = 0;
};

struct option_entry {
	union {
		struct {
			std::array<text::attribute_type, text::max_attributes> lookup_attributes;
			std::array<text::attribute_type, text::max_attributes> match_attributes;
		} attributes;
		struct {
			uint32_t string_data;
			uint32_t formatting_data;
		} content;
	} payload;
	text::variable variable;
};

struct sv_hash {
	using is_transparent = void;
	using is_avalanching = void;

	size_t operator()(std::string const& v) const {
		return ankerl::unordered_dense::hash<std::string_view>{}(std::string_view(v));
	}
	size_t operator()(std::string_view v) const {
		return ankerl::unordered_dense::hash<std::string_view>{}(v);
	}
};

struct sv_eq {
	using is_transparent = void;
	using is_avalanching = void;

	bool operator()(std::string const& u, std::string_view v) const {
		return u == v;
	}
	bool operator()(std::string_view u, std::string_view v) const {
		return u == v;
	}
	bool operator()(std::string_view u, std::string const& v) const {
		return u == v;
	}
	bool operator()(std::string const& u, std::string const& v) const {
		return u == v;
	}
};

struct backing_arrays {
	std::vector<native_char> text_data;
	std::vector<text::format_marker> fomatting_storage;
	std::vector<keyed_entry> entries;
	std::vector<option_entry> match_options;

	uint16_t first_free_variable = 0;
	int8_t first_free_attribute = int8_t(text::attribute_type::last_predefined) + 1;

	ankerl::unordered_dense::map<std::string, text::handle, sv_hash, sv_eq> entry_lookup;
	ankerl::unordered_dense::map<std::string, text::variable, sv_hash, sv_eq> variable_lookup;
	ankerl::unordered_dense::map<std::string, text::attribute_type, sv_hash, sv_eq> attribute_lookup;
};

void populate_with_file_content(backing_arrays& container, char const* start, char const* end);
void reset(backing_arrays& container);
formatted_text perform_substitutions(backing_arrays& container, std::array<text::attribute_type, text::max_attributes> const& lookup, text::text_source body_text, text::text_source const* parameters, size_t parameter_count);
handle lookup_entry(backing_arrays& container, std::string_view name);

}
}
