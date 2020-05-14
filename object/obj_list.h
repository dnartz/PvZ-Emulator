#pragma once

#include<cstddef>
#include<cassert>
#include<array>

namespace {

template<typename T, size_t S> class obj_list {
	class obj_wrap {
	protected:
		T t;
		size_t next_available;
		
		obj_wrap():t(), next_available(0) {}
		friend class obj_list;

	private:
		obj_wrap(obj_wrap const&) = delete;
		obj_wrap(obj_wrap&&) = delete;
		obj_wrap& operator=(obj_wrap const&) = delete;
		obj_wrap& operator=(obj_wrap&&) = delete;
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
				return i >= list.active_end;
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

	obj_list(obj_list const&) = delete;
	obj_list(obj_list&&) = delete;
	obj_list& operator=(obj_list const&) = delete;
	obj_list& operator=(obj_list&&) = delete;

	obj_wrap a[S];
	size_t next_available;
	size_t active_end;
	size_t n_actives;
public:
	explicit obj_list() :
		active_end(0),
		next_available(0),
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

	size_t get_index(const T& p) const {
		return reinterpret_cast<const obj_wrap&>(p).next_available;
	}

	bool is_active(const T* p) const {
		auto ow = reinterpret_cast<const obj_wrap*>(p);
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

	size_t size() const {
		return n_actives;
	}

	iterator begin() {
		return iterator(0, *this);
	}

	iterator end() {
		return iterator(-1, *this);
	}
};

};

