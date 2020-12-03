#include "./../common/aoc.hpp"
#include <string>

class AoC2020_day03 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint32_t get_tree_count_by_3_1_slope();
	std::vector<std::string> map_;
};

bool AoC2020_day03::init(const std::vector<std::string> lines) {
	uint32_t size = 0;
	map_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (i) {
			if (lines[i].size() == size) {
				map_.push_back(lines[i]);
			} else {
				std::cout << "Invalid map size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			map_.push_back(lines[i]);
			size = lines[i].size();
		}
	}

	return true;
}

uint32_t AoC2020_day03::get_tree_count_by_3_1_slope() {
	uint32_t result = 0, maxx = map_[0].size(), maxy = map_.size(), x = 0;

	for (uint32_t y = 0; y < maxy; y++) {
		if (map_[y][x % maxx] == '#') {
			result++;
		}
		x += 3;
	}

	return result;
}

void AoC2020_day03::tests() {
	uint32_t result;

	if (init({"..##.......", "#...#...#..", ".#....#..#.", "..#.#...#.#", ".#...##..#.", "..#.##.....", ".#.#.#....#", ".#........#", "#.##...#...",
			  "#...##....#", ".#..#...#.#"})) {
		result = get_tree_count_by_3_1_slope(); // 7
	}
}

bool AoC2020_day03::part1() {

	result1_ = std::to_string(get_tree_count_by_3_1_slope());

	return true;
}

bool AoC2020_day03::part2() {

	return true;
}

int32_t AoC2020_day03::get_aoc_day() {
	return 2;
}

int32_t AoC2020_day03::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day03 day03;

	return day03.main_execution();
}
