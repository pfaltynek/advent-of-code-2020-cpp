#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <string>

const std::vector<coord_3d_str> NEIGHBORS = {coord_step_north_front,
											 coord_step_south_front,
											 coord_step_west_front,
											 coord_step_east_front,
											 coord_step_front,
											 coord_step_north_east_front,
											 coord_step_south_east_front,
											 coord_step_north_west_front,
											 coord_step_south_west_front,
											 coord_step_north_rear,
											 coord_step_south_rear,
											 coord_step_west_rear,
											 coord_step_east_rear,
											 coord_step_rear,
											 coord_step_north_east_rear,
											 coord_step_south_east_rear,
											 coord_step_north_west_rear,
											 coord_step_south_west_rear,
											 coord_step_north_center,
											 coord_step_south_center,
											 coord_step_west_center,
											 coord_step_east_center,
											 coord_step_north_east_center,
											 coord_step_south_east_center,
											 coord_step_north_west_center,
											 coord_step_south_west_center};

class AoC2020_day17 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint8_t get_neighbor_active_cubes_cnt(const std::map<coord_3d_str, bool>& source, const coord_3d_str center);
	uint32_t get_active_cubes_cnt_after_rounds(const uint32_t rounds);
	void print(const std::map<coord_3d_str, bool>& src);
	void update_limits(const coord_3d_str coord);
	std::map<coord_3d_str, bool> pocket_dimension_;
	coord_3d_str min_, max_;
};

bool AoC2020_day17::init(const std::vector<std::string> lines) {
	coord_3d_str coord;

	pocket_dimension_.clear();
	min_ = max_ = {};

	for (int32_t i = 0; i < static_cast<int32_t>(lines.size()); i++) {
		for (int32_t x = 0; x < static_cast<int32_t>(lines[i].size()); x++) {
			switch (lines[i][x]) {
				case '#':
					coord = {x, i, 0};
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

	return true;
}

void AoC2020_day17::update_limits(const coord_3d_str coord) {
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
}

uint8_t AoC2020_day17::get_neighbor_active_cubes_cnt(const std::map<coord_3d_str, bool>& source, const coord_3d_str center) {
	uint8_t result = 0;
	coord_3d_str coord;

	for (size_t i = 0; i < NEIGHBORS.size(); i++) {
		coord = center;
		coord = coord + NEIGHBORS[i];
		if (source.count(coord)) {
			result++;
		}
	}

	return result;
}

uint32_t AoC2020_day17::get_active_cubes_cnt_after_rounds(const uint32_t rounds) {
	std::map<coord_3d_str, bool> input, output = {};
	uint8_t active_neighbors;
	input = pocket_dimension_;
	coord_3d_str coord;

	for (size_t r = 0; r < rounds; r++) {
		for (int32_t x = min_.x - 1; x <= max_.x + 1; x++) {
			for (int32_t y = min_.y - 1; y <= max_.y + 1; y++) {
				for (int32_t z = min_.z - 1; z <= max_.z + 1; z++) {
					coord = {x, y, z};
					active_neighbors = get_neighbor_active_cubes_cnt(input, coord);

					switch (active_neighbors) {
						case 2:
							if (input.count(coord)) {
								output[coord] = true;
								update_limits(coord);
							}
							break;
						case 3:
							output[coord] = true;
							update_limits(coord);
							break;
						default:
							// nothing to do
							break;
					}
				}
			}
		}
		input.clear();
		input.swap(output);
		// print(input);
	}

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
		result = get_active_cubes_cnt_after_rounds(6); // 112
	}
}

bool AoC2020_day17::part1() {

	result1_ = std::to_string(get_active_cubes_cnt_after_rounds(6));

	return true;
}

bool AoC2020_day17::part2() {

	// result2_ = std::to_string();

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
