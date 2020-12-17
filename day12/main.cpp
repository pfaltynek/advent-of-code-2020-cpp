#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>
#include <string>

const std::regex INSTRUCTION("^(N|S|E|W|F|R|L)(\\d+)$");

enum instruction_t { forward, left, right, north, south, east, west };

enum direction_t { direction_north = 0, direction_south = 2, direction_east = 1, direction_west = 3, direction_max = 4 };

class AoC2020_day12 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::pair<instruction_t, uint32_t>> instructions_;
	uint32_t get_route_end_manhattan_distance();
	uint32_t get_waypoint_route_end_manhattan_distance();
};

bool AoC2020_day12::init(const std::vector<std::string> lines) {
	std::smatch sm;
	instruction_t inst;
	uint32_t val;

	instructions_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, INSTRUCTION)) {
			switch (sm.str(1)[0]) {
				case 'F':
					inst = instruction_t::forward;
					break;
				case 'L':
					inst = instruction_t::left;
					break;
				case 'R':
					inst = instruction_t::right;
					break;
				case 'N':
					inst = instruction_t::north;
					break;
				case 'S':
					inst = instruction_t::south;
					break;
				case 'E':
					inst = instruction_t::east;
					break;
				case 'W':
					inst = instruction_t::west;
					break;
			}

			val = std::stoi(sm.str(2));

			instructions_.push_back({inst, val});

		} else {
			std::cout << "Invalid instruction at line " << i + 1 << std::endl;
			break;
		}
	}

	return true;
}

uint32_t AoC2020_day12::get_route_end_manhattan_distance() {
	coord_str pos;
	direction_t direction = direction_t::direction_east;

	for (size_t i = 0; i < instructions_.size(); i++) {
		switch (instructions_[i].first) {
			case instruction_t::forward:
				switch (direction) {
					case direction_t::direction_east:
						pos.x += instructions_[i].second;
						break;
					case direction_t::direction_west:
						pos.x -= instructions_[i].second;
						break;
					case direction_t::direction_north:
						pos.y -= instructions_[i].second;
						break;
					case direction_t::direction_south:
						pos.y += instructions_[i].second;
						break;
				}
				break;
			case instruction_t::right:
				direction = static_cast<direction_t>((direction + (instructions_[i].second / 90)) % direction_t::direction_max);
				break;
			case instruction_t::left:
				direction = static_cast<direction_t>((direction - (instructions_[i].second / 90) + direction_t::direction_max) % direction_t::direction_max);
				break;
			case instruction_t::north:
				pos.y -= instructions_[i].second;
				break;
			case instruction_t::south:
				pos.y += instructions_[i].second;
				break;
			case instruction_t::east:
				pos.x += instructions_[i].second;
				break;
			case instruction_t::west:
				pos.x -= instructions_[i].second;
				break;
		}
	}

	return abs(pos.x) + abs(pos.y);
}

uint32_t AoC2020_day12::get_waypoint_route_end_manhattan_distance() {
	coord_str wp = {10, -1}, pos, tmp;

	for (size_t i = 0; i < instructions_.size(); i++) {
		switch (instructions_[i].first) {
			case instruction_t::forward:
				pos.x += (wp.x * instructions_[i].second);
				pos.y += (wp.y * instructions_[i].second);
				break;
			case instruction_t::right:
				switch ((instructions_[i].second % 360) / 90) {
					case 0:
						break;
					case 1:
						std::swap(wp.x, wp.y);
						wp.x = -wp.x;
						break;
					case 2:
						wp.x = -wp.x;
						wp.y = -wp.y;
						break;
					case 3:
						std::swap(wp.x, wp.y);
						wp.y = -wp.y;
						break;
				}
				break;
			case instruction_t::left:
				switch ((instructions_[i].second % 360) / 90) {
					case 0:
						break;
					case 3:
						std::swap(wp.x, wp.y);
						wp.x = -wp.x;
						break;
					case 2:
						wp.x = -wp.x;
						wp.y = -wp.y;
						break;
					case 1:
						std::swap(wp.x, wp.y);
						wp.y = -wp.y;
						break;
				}
				break;
			case instruction_t::north:
				wp.y -= instructions_[i].second;
				break;
			case instruction_t::south:
				wp.y += instructions_[i].second;
				break;
			case instruction_t::east:
				wp.x += instructions_[i].second;
				break;
			case instruction_t::west:
				wp.x -= instructions_[i].second;
				break;
		}
	}

	return abs(pos.x) + abs(pos.y);
}

void AoC2020_day12::tests() {
	uint32_t result;

	if (init({"F10", "N3", "F7", "R90", "F11"})) {
		result = get_route_end_manhattan_distance(); // 25
		result = get_waypoint_route_end_manhattan_distance(); // 286
	}
}

bool AoC2020_day12::part1() {

	result1_ = std::to_string(get_route_end_manhattan_distance());

	return true;
}

bool AoC2020_day12::part2() {

	result2_ = std::to_string(get_waypoint_route_end_manhattan_distance());

	return true;
}

int32_t AoC2020_day12::get_aoc_day() {
	return 12;
}

int32_t AoC2020_day12::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day12 day12;

	return day12.main_execution();
}
