#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <string>

const std::vector<coord_str> ADJACENTS = {coord_step_north, coord_step_north_east, coord_step_east, coord_step_south_east,
										  coord_step_south, coord_step_south_west, coord_step_west, coord_step_north_west};

class AoC2020_day11 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, bool> seats_;
	uint32_t get_stable_occupied_count();
	uint8_t get_adjacent_occupied_count(coord_str position);
};

bool AoC2020_day11::init(const std::vector<std::string> lines) {
	uint32_t w = 1;
	seats_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!i) {
			w = lines[i].size();
		} else {
			if (lines[i].size() != w) {
				std::cout << "Invalid line size at " << i + 1 << std::endl;
				return false;
			}
		}
		for (uint32_t j = 0; j < lines[i].size(); j++) {
			switch (lines[i][j]) {
				case '.':
					// do not store
					break;
				case '#':
					seats_[{static_cast<int32_t>(j), static_cast<int32_t>(i)}] = true;
					break;
				case 'L':
					seats_[{static_cast<int32_t>(j), static_cast<int32_t>(i)}] = false;
					break;
				default:
					std::cout << "Unknow symbol at position " << j + 1 << " of line " << i + 1 << std::endl;
					return false;
			}
		}
	}

	return true;
}

uint8_t AoC2020_day11::get_adjacent_occupied_count(coord_str position) {
	uint8_t result = 0;
	coord_str tmp;

	for (size_t i = 0; i < ADJACENTS.size(); i++) {
		tmp = position + ADJACENTS[i];
		if (seats_.count(tmp)) {
			if (seats_[tmp]) {
				result++;
			}
		}
	}

	return result;
}

uint32_t AoC2020_day11::get_stable_occupied_count() {
	uint32_t result = 0;
	bool modified = true, new_state;
	std::map<coord_str, bool> next;
	uint8_t adjacents;

	while (modified) {
		modified = false;
		next.clear();

		for (auto it = seats_.begin(); it != seats_.end(); it++) {
			adjacents = get_adjacent_occupied_count(it->first);

			new_state = it->second;
			if (it->second) {
				if (adjacents >= 4) {
					new_state = false;
					modified = true;
				}
			} else {
				if (adjacents == 0) {
					new_state = true;
					modified = true;
				}
			}
			next[it->first] = new_state;
		}

		seats_.swap(next);
	}

	for (auto it = seats_.begin(); it != seats_.end(); it++) {
		if (it->second) {
			result++;
		}
	}

	return result;
}

void AoC2020_day11::tests() {
	uint32_t result;

	if (init({"L.LL.LL.LL", "LLLLLLL.LL", "L.L.L..L..", "LLLL.LL.LL", "L.LL.LL.LL", "L.LLLLL.LL", "..L.L.....", "LLLLLLLLLL", "L.LLLLLL.L", "L.LLLLL.LL"})) {
		result = get_stable_occupied_count();
	}
}

bool AoC2020_day11::part1() {

	result1_ = std::to_string(get_stable_occupied_count());

	return true;
}

bool AoC2020_day11::part2() {

	// result2_ = std::to_string();

	return true;
}

int32_t AoC2020_day11::get_aoc_day() {
	return 11;
}

int32_t AoC2020_day11::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day11 day11;

	return day11.main_execution();
}
