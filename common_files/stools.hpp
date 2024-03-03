#pragma once
#include <vector>
#include <functional>
#include <cstring>
#include <stdint.h>
#include <string>
#include <string_view>
#include <array>
#include <span>

namespace serialization {


class out_buffer {
private:
	std::vector<char> data_;
	std::vector<std::pair<size_t, std::function<void(out_buffer&)>>> pending_writes;
public:
	char const* data() const {
		return data_.data();
	}
	size_t size() const {
		return data_.size();
	}
	void finalize() {
		while(!pending_writes.empty()) {
			auto relocation_address = data_.data() + pending_writes.back().first;
			uint32_t new_address = uint32_t(data_.size());
			std::memcpy(relocation_address, &new_address, sizeof(uint32_t));

			pending_writes.back().second(*this);

			pending_writes.pop_back();
		}
	}

	template<typename T>
	void write(T const& d) {
		auto start_size = data_.size();
		auto extra = alignof(T) - (start_size % alignof(T));
		data_.resize(start_size + sizeof(T) + extra, 0);
		std::memcpy(data_.data() + extra + start_size, &d, sizeof(T));
	}
	template<typename T>
	void write_fixed(T const* d, size_t count) {
		auto start_size = data_.size();
		auto extra = alignof(T) - (start_size % alignof(T));
		data_.resize(start_size + extra + sizeof(T) * count, 0);
		std::memcpy(data_.data() + extra + start_size, d, sizeof(T) * count);
	}
	template<typename T>
	void write_variable(T const* d, size_t count) {
		uint32_t c = uint32_t(count);
		write(c);
		write_fixed(T, count);
	}
	void write_relocation(std::function<void(out_buffer&)>&& f) {
		auto reloc_address = data_.size();
		uint32_t placeholder = 0;
		write(placeholder);
		pending_writes.emplace_back(reloc_address, std::move(f));
	}
	void write(std::string_view sv) {
		write_variable(sv.data(), sv.length());
	}
	void write(std::string const& s) {
		write_variable(s.data(), s.length());
	}
	void write(std::wstring_view sv) {
		write_variable(sv.data(), sv.length());
	}
	void write(std::wstring const& s) {
		write_variable(s.data(), s.length());
	}
};

class in_buffer {
private:
	char const* base_offset;
	char const* data;
	size_t size;
	size_t base_size;
	size_t read_position = 0;
public:
	in_buffer(char const* data, size_t size) : data(data), size(size) {
		base_offset = data;
		base_size = size;
	}
	in_buffer(char const* base_offset, char const* data, size_t size, size_t base_size) : base_offset(base_offset), data(data), size(size), base_size(base_size) {
	}

	template<typename T>
	T read() {
		T temp = T{ };
		auto extra = alignof(T) - (((data + read_position) - base_offset) % alignof(T));
		read_position += extra;
		if(read_position + sizeof(T) <= size) {
			std::memcpy(&temp, data + read_position, sizeof(T));
			read_position += sizeof(T);
		}
		return temp;
	}
	template<typename T>
	std::span<T const> read_fixed(size_t count) const {
		auto extra = alignof(T) - (((data + read_position) - base_offset) % alignof(T));
		read_position += extra;

		auto len = std::min(count, (size - read_position) / sizeof(T));
		auto start = (T const*)(data + read_position + extra);
		return std::span<T const>(start, start + len);
	}
	template<typename T>
	std::span<T const> read_variable() const {
		auto count = read<uint32_t>();
		return read_fixed(count);
	}
	in_buffer read_relocation() {
		uint32_t offset = read<uint32_t>();
		return in_buffer(base_offset, base_offset + offset, (base_offset + base_size) - (base_offset + offset), base_size);
	}
	template<>
	std::string_view read<std::string_view>() {
		auto s = read_variable<char>();
		return std::string_view(s.data(), s.size());
	}
	template<>
	std::wstring_view read<std::wstring_view>() {
		auto s = read_variable<wchar_t>();
		return std::wstring_view(s.data(), s.size());
	}
};

}
