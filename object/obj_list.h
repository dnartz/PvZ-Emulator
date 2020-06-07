#pragma once

#include<cmath>
#include<cstring>
#include<cstddef>
#include<cassert>
#include<array>

namespace pvz_emulator::object {

template<typename T> class obj_list {
	class obj_wrap {
	protected:
		T t;
		size_t next_available;

    public:
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

	std::vector<obj_wrap> a;
    typename decltype(a)::size_type next_available;
	typename decltype(a)::size_type active_end;
	typename decltype(a)::size_type n_actives;

	void resize(typename decltype(a)::size_type len) {
        a.resize(1ul << static_cast<unsigned int>(ceil(log2(len))));
	}
public:
	explicit obj_list() :
		next_available(0),
		active_end(0),
		n_actives(0)
	{}

	T& alloc() {
        typename decltype(a)::size_type i;

		if (next_available >= active_end) {
            i = active_end;
            next_available = ++active_end;

            if (i >= a.size()) {
                resize(i + 1);
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
		if (i >= a.size() || i < 0) {
			return nullptr;
		}

		if (a[i].next_available == i) {
			return &a[i].t;
		}

		return nullptr;
	}

	int get_index(const T& p) const {
		auto& t = reinterpret_cast<const obj_wrap&>(p);
		if (&t < &a.front() || &t > &a.back()) {
			return -1;
		}

		return static_cast<int>(reinterpret_cast<const obj_wrap&>(p).next_available);
	}

	bool is_active(const T* p) const {
		auto ow = reinterpret_cast<const obj_wrap*>(p);
		if (ow < &a.front() || ow > &a.back()) {
			return false;
		}

		auto i = ow->next_available;

		return ow - a.data() == i;
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
        resize(active_end);
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
		a.clear();
	}
};

};

