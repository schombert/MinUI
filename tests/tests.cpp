#define CATCH_CONFIG_MAIN 1
#include "catch.hpp"

#include "../common_files/minui_text_impl.cpp"


TEST_CASE("file loading", "text parsing") {
	char file[] =
		"keyA text\n"
		"keyB more text";

	minui::text::backing_arrays container;
	minui::text::populate_with_file_content(container, file, file + strlen(file));

	REQUIRE(container.entry_lookup.size() == 2);

	auto keyB = minui::text::lookup_entry(container, "keyB");
	REQUIRE(bool(keyB) == true);

	auto entry_text = minui::text::perform_substitutions(container, { minui::text::attribute_type::undefined }, keyB, nullptr, 0);
	REQUIRE(entry_text.text_content == L"more text");
}

TEST_CASE("substitutions A", "text parsing") {
	char file[] =
		"keyA text\n"
		"apple apple\n"
		"apple.one.ind an apple\n"
		"apple.other apples\n"
		"body I have \\match{x?one}{ {v.one.ind} }{x?z}{ no {v.other} }{}{ {x} {v.other}}.\n"
		"keyB more text";

	minui::text::backing_arrays container;
	minui::text::populate_with_file_content(container, file, file + strlen(file));

	REQUIRE(container.entry_lookup.size() == 4);

	auto apple = minui::text::lookup_entry(container, "apple");
	REQUIRE(bool(apple) == true);
	auto body = minui::text::lookup_entry(container, "body");
	REQUIRE(bool(body) == true);

	auto v1 = get_variable(container, "v");
	auto x1 = get_variable(container, "x");
	
	{
		minui::text::formatted_text value_0;
		value_0.text_content = L"0";
		value_0.provided_attribues[0] = minui::text::attribute_type::other;
		value_0.provided_attribues[1] = minui::text::attribute_type::z;
		value_0.provided_attribues[2] = minui::text::attribute_type::undefined;

		minui::text::text_source parameters[] = { minui::text::text_source{ value_0, x1 }, minui::text::text_source{ apple, v1 } };
		auto entry_text = minui::text::perform_substitutions(container, { minui::text::attribute_type::undefined }, body, parameters, 2);

		REQUIRE(entry_text.text_content == L"I have no apples.");
	}

	{
		minui::text::formatted_text value_1;
		value_1.text_content = L"1";
		value_1.provided_attribues[0] = minui::text::attribute_type::one;
		value_1.provided_attribues[1] = minui::text::attribute_type::undefined;

		minui::text::text_source parameters[] = { minui::text::text_source{ value_1, x1 }, minui::text::text_source{ apple, v1 } };
		auto entry_text = minui::text::perform_substitutions(container, { minui::text::attribute_type::undefined }, body, parameters, 2);

		REQUIRE(entry_text.text_content == L"I have an apple.");
	}

	{
		minui::text::formatted_text value_3;
		value_3.text_content = L"3";
		value_3.provided_attribues[0] = minui::text::attribute_type::other;
		value_3.provided_attribues[1] = minui::text::attribute_type::undefined;

		minui::text::text_source parameters[] = { minui::text::text_source{ value_3, x1 }, minui::text::text_source{ apple, v1 } };
		auto entry_text = minui::text::perform_substitutions(container, { minui::text::attribute_type::undefined }, body, parameters, 2);

		REQUIRE(entry_text.text_content == L"I have 3 apples.");
	}
	
}