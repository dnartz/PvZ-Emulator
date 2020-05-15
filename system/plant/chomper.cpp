#include "plant.h"
#include "system/damage.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_chomper::update(plant& p) {
	switch (p.status) {
	case plant_status::wait:
		if (find_target(p, p.row, false)) {
			p.reanim.fps = 24;
			p.reanim.type = reanim_type::once;
			p.reanim.n_repeated = 0;
			p.set_reanim_frame(plant_reanim_name::anim_bite);
			p.status = plant_status::chomper_bite_begin;
			p.countdown.status = 70;
		}
		return;

	case plant_status::chomper_bite_begin: {
		if (p.countdown.status > 0) {
			return;
		}

		auto z = find_target(p, p.row, false);

		bool cannot_swallow = z &&
			(z->type == zombie_type::gargantuar ||
			z->type == zombie_type::giga_gargantuar);

		bool cannot_eat = z == nullptr ||
			z->countdown.freeze <= 0 &&
			z->countdown.butter <= 0 && (
				z->has_pogo_status() ||
				z->status == zombie_status::pole_valuting_jumpping ||
				z->status == zombie_status::pole_valuting_running);

		if (cannot_swallow) {
			damage(scene).take(*z, 40, 0);
			p.status = plant_status::chomper_bite_fail;
		} else if (cannot_eat) {
			p.status = plant_status::chomper_bite_fail;
		} else {
			zombie_factory(scene).destroy(*z);
			p.status = plant_status::chomper_bite_success;
		}

		return;
	}
	case plant_status::chomper_bite_success:
		if (p.reanim.n_repeated > 0) {
			p.reanim.fps = 15;
			p.reanim.type = reanim_type::repeat;
			p.reanim.n_repeated = 0;
			p.set_reanim_frame(plant_reanim_name::anim_chew);

			p.status = plant_status::chomper_chew;
			p.countdown.status = 4000;
		}
        return;

	case plant_status::chomper_chew:
		if (p.countdown.status == 0) {
			p.reanim.fps = 12;
			p.reanim.type = reanim_type::once;
			p.reanim.n_repeated = 0;
			p.set_reanim_frame(plant_reanim_name::anim_swallow);

			p.status = plant_status::chomper_swallow;
		}
		return;

	case plant_status::chomper_swallow:
	case plant_status::chomper_bite_fail:
		if (p.reanim.n_repeated > 0) {
			p.set_reanim(plant_reanim_name::anim_idle, reanim_type::repeat, 12);
			p.status = plant_status::wait;
		}
		return;

	default:
		return;
	}
}

void plant_chomper::init(plant & p, unsigned int row, unsigned int col) {
    plant_base::init(p, plant_type::chomper, row, col);
    p.status = plant_status::wait;
}

}