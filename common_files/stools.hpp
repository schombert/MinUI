#pragma once
#include <vector>
#include <functional>
#include <cstring>
#include <stdint.h>
#include <string>
#include <string_view>
#include <array>
#include <span>
#include <bit>
#include "unordered_dense.h"

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
	void write_relocation(size_t reloc_address, std::function<void(out_buffer&)>&& f) {
		pending_writes.emplace_back(reloc_address, std::move(f));
	}
	size_t get_data_position() const { 
		return data_.size();
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

namespace ankerl {
namespace unordered_dense {

namespace detail {

// table class altered to function as a static view
template <class Key,
	class T, // when void, treat it as a set.
	class Hash,
	class KeyEqual,
	class Bucket>
class table_view : public base_table_type_map<T> {
	using underlying_value_type = std::pair<Key, T>;
	using underlying_container_type = std::span<std::pair<Key, T>>;

public:
	using value_container_type = std::span<std::pair<Key, T>>;

private:
	using bucket_alloc =
		typename std::allocator_traits<typename value_container_type::allocator_type>::template rebind_alloc<Bucket>;
	using bucket_alloc_traits = std::allocator_traits<bucket_alloc>;

	static constexpr uint8_t initial_shifts = 64 - 2; // 2^(64-m_shift) number of buckets
	static constexpr float default_max_load_factor = 0.8F;

public:
	using key_type = Key;
	using value_type = typename value_container_type::value_type;
	using size_type = typename value_container_type::size_type;
	using difference_type = typename value_container_type::difference_type;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using reference = typename value_container_type::reference;
	using const_reference = typename value_container_type::const_reference;
	using pointer = typename value_container_type::pointer;
	using const_pointer = typename value_container_type::const_pointer;
	using const_iterator = typename value_container_type::const_iterator;
	using iterator = typename value_container_type::iterator;
	using bucket_type = Bucket;

private:
	using value_idx_type = decltype(Bucket::m_value_idx);
	using dist_and_fingerprint_type = decltype(Bucket::m_dist_and_fingerprint);

	static_assert(std::is_trivially_destructible_v<Bucket>, "assert there's no need to call destructor / std::destroy");
	static_assert(std::is_trivially_copyable_v<Bucket>, "assert we can just memset / memcpy");

	value_container_type m_values; // Contains all the key-value pairs in one densely stored container. No holes.
	std::span<bucket_type> m_buckets;
	uint8_t m_shifts = initial_shifts;

	Hash m_hash{ };
	KeyEqual m_equal{ };


	[[nodiscard]] auto next(value_idx_type bucket_idx) const -> value_idx_type {
		return ANKERL_UNORDERED_DENSE_UNLIKELY(bucket_idx + 1U == m_buckets.size())
			? 0
			: static_cast<value_idx_type>(bucket_idx + 1U);
	}

	// Helper to access bucket through pointer types
	[[nodiscard]] static constexpr auto at(bucket_pointer bucket_ptr, size_t offset) -> Bucket& {
		return *(bucket_ptr + static_cast<typename std::allocator_traits<bucket_alloc>::difference_type>(offset));
	}

	// use the dist_inc and dist_dec functions so that uint16_t types work without warning
	[[nodiscard]] static constexpr auto dist_inc(dist_and_fingerprint_type x) -> dist_and_fingerprint_type {
		return static_cast<dist_and_fingerprint_type>(x + Bucket::dist_inc);
	}

	[[nodiscard]] static constexpr auto dist_dec(dist_and_fingerprint_type x) -> dist_and_fingerprint_type {
		return static_cast<dist_and_fingerprint_type>(x - Bucket::dist_inc);
	}

	// The goal of mixed_hash is to always produce a high quality 64bit hash.
	template <typename K>
	[[nodiscard]] constexpr auto mixed_hash(K const& key) const -> uint64_t {
		if constexpr(is_detected_v<detect_avalanching, Hash>) {
			// we know that the hash is good because is_avalanching.
			if constexpr(sizeof(decltype(m_hash(key))) < sizeof(uint64_t)) {
				// 32bit hash and is_avalanching => multiply with a constant to avalanche bits upwards
				return m_hash(key) * UINT64_C(0x9ddfea08eb382d69);
			} else {
				// 64bit and is_avalanching => only use the hash itself.
				return m_hash(key);
			}
		} else {
			// not is_avalanching => apply wyhash
			return wyhash::hash(m_hash(key));
		}
	}

	[[nodiscard]] constexpr auto dist_and_fingerprint_from_hash(uint64_t hash) const -> dist_and_fingerprint_type {
		return Bucket::dist_inc | (static_cast<dist_and_fingerprint_type>(hash) & Bucket::fingerprint_mask);
	}

	[[nodiscard]] constexpr auto bucket_idx_from_hash(uint64_t hash) const -> value_idx_type {
		return static_cast<value_idx_type>(hash >> m_shifts);
	}

	[[nodiscard]] static constexpr auto get_key(value_type const& vt) -> key_type const& {
		if constexpr(is_map_v<T>) {
			return vt.first;
		} else {
			return vt;
		}
	}

	template <typename K>
	[[nodiscard]] auto next_while_less(K const& key) const -> Bucket {
		auto hash = mixed_hash(key);
		auto dist_and_fingerprint = dist_and_fingerprint_from_hash(hash);
		auto bucket_idx = bucket_idx_from_hash(hash);

		while(dist_and_fingerprint < m_buckets[bucket_idx].m_dist_and_fingerprint) {
			dist_and_fingerprint = dist_inc(dist_and_fingerprint);
			bucket_idx = next(bucket_idx);
		}
		return { dist_and_fingerprint, bucket_idx };
	}

	[[nodiscard]] static constexpr auto calc_num_buckets(uint8_t shifts) -> size_t {
		return (std::min)(max_bucket_count(), size_t{ 1 } << (64U - shifts));
	}

	auto calc_m_shifts() -> void {
		m_shifts = uint8_t(65 - std::bit_width(m_buckets.size()));
	}

	[[nodiscard]] constexpr auto calc_shifts_for_size(size_t s) const -> uint8_t {
		auto shifts = initial_shifts;
		while(shifts > 0 && static_cast<size_t>(static_cast<float>(calc_num_buckets(shifts)) * default_max_load_factor) < s) {
			--shifts;
		}
		return shifts;
	}

	template <typename K>
	auto do_find(K const& key) -> iterator {
		if(ANKERL_UNORDERED_DENSE_UNLIKELY(empty())) {
			return end();
		}

		auto mh = mixed_hash(key);
		auto dist_and_fingerprint = dist_and_fingerprint_from_hash(mh);
		auto bucket_idx = bucket_idx_from_hash(mh);
		auto* bucket = &m_buckets[bucket_idx];

		// unrolled loop. *Always* check a few directly, then enter the loop. This is faster.
		if(dist_and_fingerprint == bucket->m_dist_and_fingerprint && m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
			return begin() + static_cast<difference_type>(bucket->m_value_idx);
		}
		dist_and_fingerprint = dist_inc(dist_and_fingerprint);
		bucket_idx = next(bucket_idx);
		bucket = &m_buckets[bucket_idx];

		if(dist_and_fingerprint == bucket->m_dist_and_fingerprint && m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
			return begin() + static_cast<difference_type>(bucket->m_value_idx);
		}
		dist_and_fingerprint = dist_inc(dist_and_fingerprint);
		bucket_idx = next(bucket_idx);
		bucket = &m_buckets[bucket_idx];

		while(true) {
			if(dist_and_fingerprint == bucket->m_dist_and_fingerprint) {
				if(m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
					return begin() + static_cast<difference_type>(bucket->m_value_idx);
				}
			} else if(dist_and_fingerprint > bucket->m_dist_and_fingerprint) {
				return end();
			}
			dist_and_fingerprint = dist_inc(dist_and_fingerprint);
			bucket_idx = next(bucket_idx);
			bucket = &m_buckets[bucket_idx];
		}
	}

	template <typename K>
	auto do_atomic_find(K const& key) const -> std::optional<T> {
		if(ANKERL_UNORDERED_DENSE_UNLIKELY(empty())) {
			return std::optional<T>{};
		}

		auto mh = mixed_hash(key);
		auto dist_and_fingerprint = dist_and_fingerprint_from_hash(mh);
		auto bucket_idx = bucket_idx_from_hash(mh);
		auto* bucket = &m_buckets[bucket_idx];

		// unrolled loop. *Always* check a few directly, then enter the loop. This is faster.
		if(dist_and_fingerprint == bucket->m_dist_and_fingerprint && m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
			return std::optional<T>{ (begin() + static_cast<difference_type>(bucket->m_value_idx))->second };
		}
		dist_and_fingerprint = dist_inc(dist_and_fingerprint);
		bucket_idx = next(bucket_idx);
		bucket = &m_buckets[bucket_idx];

		if(dist_and_fingerprint == bucket->m_dist_and_fingerprint && m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
			return std::optional<T>{ (begin() + static_cast<difference_type>(bucket->m_value_idx))->second };
		}
		dist_and_fingerprint = dist_inc(dist_and_fingerprint);
		bucket_idx = next(bucket_idx);
		bucket = &m_buckets[bucket_idx];

		while(true) {
			if(dist_and_fingerprint == bucket->m_dist_and_fingerprint) {
				if(m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
					return std::optional<T>{ (begin() + static_cast<difference_type>(bucket->m_value_idx))->second };
				}
			} else if(dist_and_fingerprint > bucket->m_dist_and_fingerprint) {
				return std::optional<T>{};
			}
			dist_and_fingerprint = dist_inc(dist_and_fingerprint);
			bucket_idx = next(bucket_idx);
			bucket = &m_buckets[bucket_idx];
		}
	}

	template <typename K>
	auto do_find(K const& key) const -> const_iterator {
		return const_cast<table_view*>(this)->do_find(key); // NOLINT(cppcoreguidelines-pro-type-const-cast)
	}

	template <typename K, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
	auto do_at(K const& key) -> Q& {
		if(auto it = find(key); ANKERL_UNORDERED_DENSE_LIKELY(end() != it)) {
			return it->second;
		}
		on_error_key_not_found();
	}

	template <typename K, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
	auto do_at(K const& key) const -> Q const& {
		return const_cast<table_view*>(this)->at(key); // NOLINT(cppcoreguidelines-pro-type-const-cast)
	}

public:
	explicit table_view(value_container_type m_values,
		std::span<bucket_type> m_buckets,
		Hash const& hash = Hash(),
		KeyEqual const& equal = KeyEqual())
		: m_values(m_values),
		m_buckets(m_buckets)
		, m_hash(hash)
		, m_equal(equal) {

		calc_m_shifts();
	}
	table_view(table_view const& other) noexcept {
		m_values = other.m_values;
		m_hash = other.m_hash;
		m_equal = other.m_equal;
		m_buckets = other.m_buckets;
		m_shifts = other.m_shifts;
	}

	table_view() : table_view(value_container_type{ }, std::span<bucket_type>{}) {

	}

	auto operator=(table_view const& other) noexcept -> table_view& {
		m_values = other.m_values;
		m_hash = other.m_hash;
		m_equal = other.m_equal;
		m_buckets = other.m_buckets;
		m_shifts = other.m_shifts;
		return *this;
	}

	// iterators //////////////////////////////////////////////////////////////

	auto begin() noexcept -> iterator {
		return m_values.begin();
	}

	auto begin() const noexcept -> const_iterator {
		return m_values.begin();
	}

	auto cbegin() const noexcept -> const_iterator {
		return m_values.cbegin();
	}

	auto end() noexcept -> iterator {
		return m_values.end();
	}

	auto cend() const noexcept -> const_iterator {
		return m_values.cend();
	}

	auto end() const noexcept -> const_iterator {
		return m_values.end();
	}

	// capacity ///////////////////////////////////////////////////////////////

	[[nodiscard]] auto empty() const noexcept -> bool {
		return m_values.empty();
	}

	[[nodiscard]] auto size() const noexcept -> size_t {
		return m_values.size();
	}

	// lookup /////////////////////////////////////////////////////////////////

	template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
	auto at(key_type const& key) -> Q& {
		return do_at(key);
	}

	template <typename K,
		typename Q = T,
		typename H = Hash,
		typename KE = KeyEqual,
		std::enable_if_t<is_map_v<Q>&& is_transparent_v<H, KE>, bool> = true>
	auto at(K const& key) -> Q& {
		return do_at(key);
	}

	template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
	auto at(key_type const& key) const -> Q const& {
		return do_at(key);
	}

	template <typename K,
		typename Q = T,
		typename H = Hash,
		typename KE = KeyEqual,
		std::enable_if_t<is_map_v<Q>&& is_transparent_v<H, KE>, bool> = true>
	auto at(K const& key) const -> Q const& {
		return do_at(key);
	}

	auto count(Key const& key) const -> size_t {
		return find(key) == end() ? 0 : 1;
	}

	template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
	auto count(K const& key) const -> size_t {
		return find(key) == end() ? 0 : 1;
	}

	auto find(Key const& key) -> iterator {
		return do_find(key);
	}

	auto find(Key const& key) const -> const_iterator {
		return do_find(key);
	}

	template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
	auto atomic_find(Key const& key) const -> std::optional<Q> {
		return do_atomic_find(key);
	}

	template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
	auto find(K const& key) -> iterator {
		return do_find(key);
	}

	template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
	auto find(K const& key) const -> const_iterator {
		return do_find(key);
	}

	auto contains(Key const& key) const -> bool {
		return find(key) != end();
	}

	template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
	auto contains(K const& key) const -> bool {
		return find(key) != end();
	}

	auto equal_range(Key const& key) -> std::pair<iterator, iterator> {
		auto it = do_find(key);
		return { it, it == end() ? end() : it + 1 };
	}

	auto equal_range(const Key& key) const -> std::pair<const_iterator, const_iterator> {
		auto it = do_find(key);
		return { it, it == end() ? end() : it + 1 };
	}

	template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
	auto equal_range(K const& key) -> std::pair<iterator, iterator> {
		auto it = do_find(key);
		return { it, it == end() ? end() : it + 1 };
	}

	template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
	auto equal_range(K const& key) const -> std::pair<const_iterator, const_iterator> {
		auto it = do_find(key);
		return { it, it == end() ? end() : it + 1 };
	}

	// bucket interface ///////////////////////////////////////////////////////

	auto bucket_count() const noexcept -> size_t { // NOLINT(modernize-use-nodiscard)
		return m_buckets.size();
	}

	static constexpr auto max_bucket_count() noexcept -> size_t { // NOLINT(modernize-use-nodiscard)
		return max_size();
	}

	// hash policy ////////////////////////////////////////////////////////////

	[[nodiscard]] auto load_factor() const -> float {
		return bucket_count() ? static_cast<float>(size()) / static_cast<float>(bucket_count()) : 0.0F;
	}

	// observers //////////////////////////////////////////////////////////////

	auto hash_function() const -> hasher {
		return m_hash;
	}

	auto key_eq() const -> key_equal {
		return m_equal;
	}

	// nonstandard API: expose the underlying values container
	[[nodiscard]] auto values() const noexcept -> value_container_type const& {
		return m_values;
	}

	// non-member functions ///////////////////////////////////////////////////

	friend auto operator==(table_view const& a, table_view const& b) -> bool {
		if(&a == &b) {
			return true;
		}
		if(a.size() != b.size()) {
			return false;
		}
		for(auto const& b_entry : b) {
			auto it = a.find(get_key(b_entry));
			if constexpr(is_map_v<T>) {
				// map: check that key is here, then also check that value is the same
				if(a.end() == it || !(b_entry.second == it->second)) {
					return false;
				}
			} else {
				// set: only check that the key is here
				if(a.end() == it) {
					return false;
				}
			}
		}
		return true;
	}

	friend auto operator!=(table_view const& a, table_view const& b) -> bool {
		return !(a == b);
	}
};

}

ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
	class T,
	class Hash = hash<Key>,
	class KeyEqual = std::equal_to<Key>,
	class AllocatorOrContainer = std::allocator<std::pair<Key, T>>,
	class Bucket = bucket_type::standard>
	using map_view = detail::table_view<Key, T, Hash, KeyEqual, AllocatorOrContainer, Bucket>;

}
}
