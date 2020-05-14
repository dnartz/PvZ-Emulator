#pragma once
#include <random>

#include "object/scene.h"

namespace pvz_emulator::system {

class rng {
	object::scene& scene;

public:
	rng(object::scene& s) : scene(s) {}

	unsigned int randint(unsigned int n) {
		return scene.rng() % n;
	}

	template<typename A>
	size_t random_weighted_sample(const A& v) {
		std::discrete_distribution<> d(v.begin(), v.end());
		return d(scene.rng);
	}

	float randfloat(double a, double b) {
		std::uniform_real_distribution<double> dis(a, b);
		return static_cast<float>(dis(scene.rng));
	}
};

};
