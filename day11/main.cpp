#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <string>

const std::vector<coord_str> ADJACENTS = {coord_step_north, coord_step_north_east, coord_step_east, coord_step_south_east,
										  coord_step_south, coord_step_south_west, coord_step_west, coord_step_north_west};

const char FLOOR = '.';
const char SEAT_EMPTY = 'L';
const char SEAT_OCCUPIED = '#';

class AoC2020_day11 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, char> seats_;
	uint32_t get_stable_occupied_count(const bool part2 = false);
	uint8_t get_adjacent_occupied_count(std::map<coord_str, char>& seats, coord_str position);
	uint8_t get_first_adjacent_seat_occupied_count(std::map<coord_str, char>& seats, coord_str position);
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
				case FLOOR:
				case SEAT_OCCUPIED:
				case SEAT_EMPTY:
					seats_[{static_cast<int32_t>(j), static_cast<int32_t>(i)}] = lines[i][j];
					break;
				default:
					std::cout << "Unknow symbol at position " << j + 1 << " of line " << i + 1 << std::endl;
					return false;
			}
		}
	}

	return true;
}

uint8_t AoC2020_day11::get_first_adjacent_seat_occupied_count(std::map<coord_str, char>& seats, coord_str position) {
	uint8_t result = 0;
	coord_str tmp;

	for (size_t i = 0; i < ADJACENTS.size(); i++) {
		tmp = position;

		do {
			tmp = tmp + ADJACENTS[i];

			if (!seats.count(tmp)) {
				break;
			} else if (seats[tmp] == FLOOR) {
				continue;
			} else {
				if (seats[tmp] == SEAT_OCCUPIED) {
					result++;
				}
				break;
			}
		} while (true);
	}

	return result;
}

uint8_t AoC2020_day11::get_adjacent_occupied_count(std::map<coord_str, char>& seats, coord_str position) {
	uint8_t result = 0;
	coord_str tmp;

	for (size_t i = 0; i < ADJACENTS.size(); i++) {
		tmp = position + ADJACENTS[i];
		if (seats.count(tmp)) {
			if (seats[tmp] == SEAT_OCCUPIED) {
				result++;
			}
		}
	}

	return result;
}

uint32_t AoC2020_day11::get_stable_occupied_count(const bool part2) {
	uint32_t result = 0;
	bool modified = true;
	char new_state;
	std::map<coord_str, char> next, seats;
	uint8_t adjacents, adjacents_limit;

	seats = seats_;

	if (part2) {
		adjacents_limit = 5;
	} else {
		adjacents_limit = 4;
	}

	while (modified) {
		modified = false;
		next.clear();

		for (auto it = seats.begin(); it != seats.end(); it++) {
			if (part2) {
				adjacents = get_first_adjacent_seat_occupied_count(seats, it->first);
			} else {
				adjacents = get_adjacent_occupied_count(seats, it->first);
			}

			new_state = it->second;
			switch (it->second) {
				case SEAT_OCCUPIED:
					if (adjacents >= adjacents_limit) {
						new_state = SEAT_EMPTY;
						modified = true;
					}
					break;
				case SEAT_EMPTY:
					if (adjacents == 0) {
						new_state = SEAT_OCCUPIED;
						modified = true;
					}
					break;
			}

			next[it->first] = new_state;
		}

		seats.swap(next);
	}

	for (auto it = seats.begin(); it != seats.end(); it++) {
		if (it->second == SEAT_OCCUPIED) {
			result++;
		}
	}

	return result;
}

void AoC2020_day11::tests() {
	uint32_t result;

	if (init({"L.LL.LL.LL", "LLLLLLL.LL", "L.L.L..L..", "LLLL.LL.LL", "L.LL.LL.LL", "L.LLLLL.LL", "..L.L.....", "LLLLLLLLLL", "L.LLLLLL.L", "L.LLLLL.LL"})) {
		result = get_stable_occupied_count();	  // 37
		result = get_stable_occupied_count(true); // 26
	}
}

bool AoC2020_day11::part1() {

	result1_ = std::to_string(get_stable_occupied_count());

	return true;
}

bool AoC2020_day11::part2() {

	result2_ = std::to_string(get_stable_occupied_count(true));

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
