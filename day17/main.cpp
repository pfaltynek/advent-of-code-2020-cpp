#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <string>

class AoC2020_day17 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint8_t get_neighbor_active_cubes_cnt_3d(const std::map<coord_4d_str, bool>& source, const coord_4d_str center);
	uint8_t get_neighbor_active_cubes_cnt_4d(const std::map<coord_4d_str, bool>& source, const coord_4d_str center);
	uint32_t get_active_cubes_cnt_after_rounds(const uint32_t rounds, const bool only3d);
	bool is_in_next_round_active(const uint8_t actual_active_cnt, const bool active);
	void print(const std::map<coord_3d_str, bool>& src);
	void update_limits(const coord_4d_str coord);
	std::map<coord_4d_str, bool> pocket_dimension_;
	coord_4d_str min_, max_;
	std::vector<coord_4d_str> neighbors_3d_;
	std::vector<coord_4d_str> neighbors_4d_;
};

bool AoC2020_day17::init(const std::vector<std::string> lines) {
	coord_4d_str coord;

	pocket_dimension_.clear();
	min_ = max_ = {};

	for (int32_t i = 0; i < static_cast<int32_t>(lines.size()); i++) {
		for (int32_t x = 0; x < static_cast<int32_t>(lines[i].size()); x++) {
			switch (lines[i][x]) {
				case '#':
					coord = {x, i, 0, 0};
					pocket_dimension_[coord] = true;
					update_limits(coord);
					break;
				case '.':
					// nothing to do
					break;
				default:
					std::cout << "Invalid input at position " << x + 1 << " of line " << i + 1 << std::endl;
					return false;
					break;
			}
		}
	}
	neighbors_3d_ = {{coord_step_north_front.x, coord_step_north_front.y, coord_step_north_front.z, 0},
					 {coord_step_south_front.x, coord_step_south_front.y, coord_step_south_front.z, 0},
					 {coord_step_west_front.x, coord_step_west_front.y, coord_step_west_front.z, 0},
					 {coord_step_east_front.x, coord_step_east_front.y, coord_step_east_front.z, 0},
					 {coord_step_front.x, coord_step_front.y, coord_step_front.z, 0},
					 {coord_step_north_east_front.x, coord_step_north_east_front.y, coord_step_north_east_front.z, 0},
					 {coord_step_south_east_front.x, coord_step_south_east_front.y, coord_step_south_east_front.z, 0},
					 {coord_step_north_west_front.x, coord_step_north_west_front.y, coord_step_north_west_front.z, 0},
					 {coord_step_south_west_front.x, coord_step_south_west_front.y, coord_step_south_west_front.z, 0},
					 {coord_step_north_rear.x, coord_step_north_rear.y, coord_step_north_rear.z, 0},
					 {coord_step_south_rear.x, coord_step_south_rear.y, coord_step_south_rear.z, 0},
					 {coord_step_west_rear.x, coord_step_west_rear.y, coord_step_west_rear.z, 0},
					 {coord_step_east_rear.x, coord_step_east_rear.y, coord_step_east_rear.z, 0},
					 {coord_step_rear.x, coord_step_rear.y, coord_step_rear.z, 0},
					 {coord_step_north_east_rear.x, coord_step_north_east_rear.y, coord_step_north_east_rear.z, 0},
					 {coord_step_south_east_rear.x, coord_step_south_east_rear.y, coord_step_south_east_rear.z, 0},
					 {coord_step_north_west_rear.x, coord_step_north_west_rear.y, coord_step_north_west_rear.z, 0},
					 {coord_step_south_west_rear.x, coord_step_south_west_rear.y, coord_step_south_west_rear.z, 0},
					 {coord_step_north_center.x, coord_step_north_center.y, coord_step_north_center.z, 0},
					 {coord_step_south_center.x, coord_step_south_center.y, coord_step_south_center.z, 0},
					 {coord_step_west_center.x, coord_step_west_center.y, coord_step_west_center.z, 0},
					 {coord_step_east_center.x, coord_step_east_center.y, coord_step_east_center.z, 0},
					 {coord_step_north_east_center.x, coord_step_north_east_center.y, coord_step_north_east_center.z, 0},
					 {coord_step_south_east_center.x, coord_step_south_east_center.y, coord_step_south_east_center.z, 0},
					 {coord_step_north_west_center.x, coord_step_north_west_center.y, coord_step_north_west_center.z, 0},
					 {coord_step_south_west_center.x, coord_step_south_west_center.y, coord_step_south_west_center.z, 0}};

	neighbors_4d_ = coord_4d_str::get_neighbors();

	return true;
}

void AoC2020_day17::update_limits(const coord_4d_str coord) {
	if (min_.x > coord.x) {
		min_.x = coord.x;
	}
	if (max_.x < coord.x) {
		max_.x = coord.x;
	}
	if (min_.y > coord.y) {
		min_.y = coord.y;
	}
	if (max_.y < coord.y) {
		max_.y = coord.y;
	}
	if (min_.z > coord.z) {
		min_.z = coord.z;
	}
	if (max_.z < coord.z) {
		max_.z = coord.z;
	}
	if (min_.w > coord.w) {
		min_.w = coord.w;
	}
	if (max_.w < coord.w) {
		max_.w = coord.w;
	}
}

uint8_t AoC2020_day17::get_neighbor_active_cubes_cnt_3d(const std::map<coord_4d_str, bool>& source, const coord_4d_str center) {
	uint8_t result = 0;
	coord_4d_str coord;

	for (size_t i = 0; i < neighbors_3d_.size(); i++) {
		coord = center;
		coord = coord + neighbors_3d_[i];
		if (source.count(coord)) {
			result++;
		}
	}

	return result;
}

uint8_t AoC2020_day17::get_neighbor_active_cubes_cnt_4d(const std::map<coord_4d_str, bool>& source, const coord_4d_str center) {
	uint8_t result = 0;
	coord_4d_str coord;
	std::vector<coord_4d_str> neighbors;

	for (size_t i = 0; i < neighbors_4d_.size(); i++) {
		coord = center;
		coord = coord + neighbors_4d_[i];
		if (source.count(coord)) {
			result++;
		}
	}

	return result;
}

bool AoC2020_day17::is_in_next_round_active(const uint8_t actual_active_cnt, const bool active) {
	switch (actual_active_cnt) {
		case 2:
			if (active) {
				return true;
			}
			break;
		case 3:
			return true;
		default:
			// nothing to do
			break;
	}

	return false;
}

uint32_t AoC2020_day17::get_active_cubes_cnt_after_rounds(const uint32_t rounds, const bool only3d) {
	std::map<coord_4d_str, bool> input, output = {};
	uint8_t active_neighbors;
	input = pocket_dimension_;
	coord_4d_str coord, min, max;

	min = min_;
	max = max_;

	for (size_t r = 0; r < rounds; r++) {
		for (int32_t x = min_.x - 1; x <= max_.x + 1; x++) {
			for (int32_t y = min_.y - 1; y <= max_.y + 1; y++) {
				for (int32_t z = min_.z - 1; z <= max_.z + 1; z++) {
					if (only3d) {
						coord = {x, y, z, 0};
						active_neighbors = get_neighbor_active_cubes_cnt_3d(input, coord);

						if (is_in_next_round_active(active_neighbors, input.count(coord) != 0)) {
							output[coord] = true;
							update_limits(coord);
						}
					} else {
						for (int32_t w = min_.w - 1; w <= max_.w + 1; w++) {
							coord = {x, y, z, w};
							active_neighbors = get_neighbor_active_cubes_cnt_4d(input, coord);
							if (is_in_next_round_active(active_neighbors, input.count(coord) != 0)) {
								output[coord] = true;
								update_limits(coord);
							}
						}
					}
				}
			}
		}
		input.clear();
		input.swap(output);
		// print(input);
	}

	min_ = min;
	max_ = max;

	return input.size();
}

void AoC2020_day17::print(const std::map<coord_3d_str, bool>& src) {
	for (int32_t z = min_.z; z <= max_.z; z++) {
		std::cout << "z=" << z << std::endl;
		for (int32_t y = min_.y; y <= max_.y; y++) {
			for (int32_t x = min_.x; x <= max_.x; x++) {
				if (src.count({x, y, z})) {
					std::cout << '#';
				} else {
					std::cout << '.';
				}
			}
			std::cout << std::endl;
		}
	}
}

void AoC2020_day17::tests() {
	uint32_t result;

	if (init({".#.", "..#", "###"})) {
		result = get_active_cubes_cnt_after_rounds(6, true); // 112
		result = get_active_cubes_cnt_after_rounds(6, false); // 848
	}
}

bool AoC2020_day17::part1() {

	result1_ = std::to_string(get_active_cubes_cnt_after_rounds(6, true));

	return true;
}

bool AoC2020_day17::part2() {

	result2_ = std::to_string(get_active_cubes_cnt_after_rounds(6, false));

	return true;
}

int32_t AoC2020_day17::get_aoc_day() {
	return 17;
}

int32_t AoC2020_day17::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day17 day17;

	return day17.main_execution();
}
