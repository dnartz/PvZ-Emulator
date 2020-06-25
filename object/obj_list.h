#pragma once

#include<cstring>
#include<cstddef>
#include<cassert>
#include<array>

namespace pvz_emulator::object {

template<typename T, size_t S> class obj_list {
	class obj_wrap {
	protected:
		T t;
		size_t next_available;
		
		obj_wrap():t(), next_available(0) {}
		friend class obj_list;
	};

	class iterator {
	private:
		int i;
		obj_list& list;

	public:
		explicit iterator(int n, obj_list& l): i(n), list(l) {
			while (i < list.active_end &&
				(list.a[i].t.is_freeable() || list.a[i].next_available != i))
			{
				i++;
			}
		}
		
		iterator& operator++() {
			do {
				i++;
			} while (i < list.active_end &&
				(list.a[i].t.is_freeable() || list.a[i].next_available != i));
			return *this;
		}

		iterator operator++(int) {
			auto original = *this;
			++(*this);
			return original;
		}

		bool operator==(const iterator& other) const {
			if (other.i == i) {
				return true;
			} else if (i == -1) {
				return other.i >= list.active_end;
			} else if (other.i == -1) {
				return static_cast<size_t>(i) >= list.active_end;
			} else {
				return false;
			}
		}

		bool operator!=(const iterator& other) const {
			return !((*this) == other);
		}

		T& operator*() const {
			return list.a[i].t;
		}
	};

	obj_wrap a[S];
	size_t next_available;
	size_t active_end;
	size_t n_actives;
public:
	explicit obj_list() :
		next_available(0),
		active_end(0),
		n_actives(0)
	{
		memset(a, 0, sizeof(a));
	}

	T& alloc() {
		size_t i;

		if (next_available > S) {
			throw std::bad_alloc();
		}

		if (next_available >= active_end) {
			if (active_end < S) {
				i = active_end;
				next_available = ++active_end;
			} else {
				throw std::bad_alloc();
			}
		} else {
			i = next_available;
			next_available = std::min(a[i].next_available, active_end);
		}

		a[i].next_available = i;
		++n_actives;

		return a[i].t;
	}

	T* get(int i) {
		if (i >= S || i < 0) {
			return nullptr;
		}

		if (a[i].next_available == i) {
			return &a[i].t;
		}

		return nullptr;
	}

	int get_index(const T& p) const {
		auto& t = reinterpret_cast<const obj_wrap&>(p);
		if (&t < &a[0] || &t > &a[S - 1]) {
			return -1;
		}

		return static_cast<int>(reinterpret_cast<const obj_wrap&>(p).next_available);
	}

	bool is_active(const T* p) const {
		auto ow = reinterpret_cast<const obj_wrap*>(p);
		if (ow < &a[0] || ow > &a[S - 1]) {
			return false;
		}

		auto i = ow->next_available;

		return ow - a == i;
	}

	void shrink_to_fit() {
		n_actives = 0;
		size_t last = 0;

		for (auto i = 0ul; i < active_end; i++) {
			if (a[i].next_available == i && !a[i].t.is_freeable()) {
				last = i + 1;
				n_actives++;
			}
		}

		next_available = active_end = last;

		for (auto i = 0ul; i < active_end; i++) {
			if (i == next_available) {
				continue;
			}

			if (a[i].next_available != i || a[i].t.is_freeable()) {
				a[i].next_available = next_available;
				next_available = i;
			}
		}

		assert(next_available <= active_end);
	}

	[[nodiscard]]
	size_t size() const {
		return n_actives;
	}

	iterator begin() {
		return iterator(0, *this);
	}

	iterator end() {
		return iterator(-1, *this);
	}

	void clear() {
		next_available = 0;
		active_end = 0;
		n_actives = 0;
		memset(a, 0, sizeof(a));
	}
};

};

