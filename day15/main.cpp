#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <string>

class AoC2020_day15 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint32_t> starting_nums_;
	uint32_t simulate_game(uint32_t turns);
};

bool AoC2020_day15::init(const std::vector<std::string> lines) {
	std::vector<std::string> s;

	starting_nums_.clear();

	if (lines.size() != 1) {
		std::cout << "Awaited input of 1 line exactly." << std::endl;
		return false;
	}

	s = split(lines[0], ",");

	for (uint32_t i = 0; i < s.size(); i++) {
		starting_nums_.push_back(std::stoi(s[i]));
	}

	return true;
}

uint32_t AoC2020_day15::simulate_game(uint32_t turns) {
	uint32_t turn = 0, last_spoken;
	std::map<uint32_t, std::vector<uint32_t>> spoken = {};

	for (size_t i = 0; i < starting_nums_.size(); i++) {
		last_spoken = starting_nums_[i];
		spoken[last_spoken].insert(spoken[last_spoken].begin(), turn);
		turn++;
	}

	while (turn < turns) {
		if (spoken[last_spoken].size() > 1) {
			last_spoken = spoken[last_spoken][0] - spoken[last_spoken][1];

		} else {
			last_spoken = 0;
		}
		spoken[last_spoken].insert(spoken[last_spoken].begin(), turn);
		turn++;
	}

	return last_spoken;
}

void AoC2020_day15::tests() {
	uint64_t result;

	if (init({"0,3,6"})) {
		result = simulate_game(2020); // 436
	}
	if (init({"1,3,2"})) {
		result = simulate_game(2020); // 1.
	}
	if (init({"2,1,3"})) {
		result = simulate_game(2020); // 10.
	}
	if (init({"1,2,3"})) {
		result = simulate_game(2020); // 27.
	}
	if (init({"2,3,1"})) {
		result = simulate_game(2020); // 78.
	}
	if (init({"3,2,1"})) {
		result = simulate_game(2020); // 438.
	}
	if (init({"3,1,2"})) {
		result = simulate_game(2020); // 1836.
	}
}

bool AoC2020_day15::part1() {

	result1_ = std::to_string(simulate_game(2020));

	return true;
}

bool AoC2020_day15::part2() {

	// result2_ = std::to_string();

	return true;
}

int32_t AoC2020_day15::get_aoc_day() {
	return 15;
}

int32_t AoC2020_day15::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day15 day15;

	return day15.main_execution();
}
