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
	uint32_t get_tree_count_by_slopes(const bool part2);
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

uint32_t AoC2020_day03::get_tree_count_by_slopes(const bool part2) {
	uint32_t result, maxx = map_[0].size(), maxy = map_.size(), x, y;
	std::vector<std::pair<uint32_t, uint32_t>> slopes;
	std::vector<uint32_t> sums{};

	if (part2) {
		slopes = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
	} else {
		slopes = {{3, 1}};
	}

	for (size_t i = 0; i < slopes.size(); i++) {

		x = y = result = 0;

		while (y < maxy) {
			if (map_[y][x % maxx] == '#') {
				result++;
			}
			x += slopes[i].first;
			y += slopes[i].second;
		}

		sums.push_back(result);
	}

	result = sums[0];

	for (uint32_t i = 1; i < sums.size(); i++) {
		result *= sums[i];
	}

	return result;
}

void AoC2020_day03::tests() {
	uint32_t result;

	if (init({"..##.......", "#...#...#..", ".#....#..#.", "..#.#...#.#", ".#...##..#.", "..#.##.....", ".#.#.#....#", ".#........#", "#.##...#...",
			  "#...##....#", ".#..#...#.#"})) {
		result = get_tree_count_by_slopes(false); // 7
		result = get_tree_count_by_slopes(true);  // 336
	}
}

bool AoC2020_day03::part1() {

	result1_ = std::to_string(get_tree_count_by_slopes(false));

	return true;
}

bool AoC2020_day03::part2() {

	result2_ = std::to_string(get_tree_count_by_slopes(true));

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
