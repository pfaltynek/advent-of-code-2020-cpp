#include "./../common/aoc.hpp"
#include <algorithm>
#include <set>
#include <string>

class AoC2020_day23 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint8_t> cups_;
	uint64_t perform_crab_moves(const uint16_t moves_count);
};

bool AoC2020_day23::init(const std::vector<std::string> lines) {

	cups_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines[i].size(); j++) {
			if (std::isdigit(lines[i][j])) {
				cups_.push_back(lines[i][j] - '0');
			} else {
				std::cout << "Invalid cup number at line " << i + 1 << ", position " << j + 1 << std::endl;
				return false;
			}
		}
	}

	return true;
}

uint64_t AoC2020_day23::perform_crab_moves(const uint16_t moves_count) {
	std::vector<uint8_t> tmp, cups;
	std::vector<uint8_t>::iterator it;
	uint16_t next_val;
	uint8_t min, max;
	uint64_t result;

	if (!cups_.size()) {
		return 0;
	}

	cups = cups_;

	min = *std::min_element(cups.begin(), cups.end());
	max = *std::max_element(cups.begin(), cups.end());

	for (uint8_t i = 0; i < moves_count; i++) {
		tmp.clear();
		tmp.insert(tmp.begin(), cups.begin() + 1, cups.begin() + 4);
		cups.erase(cups.begin() + 1, cups.begin() + 4);

		next_val = cups[0] - 1;

		while ((it = std::find(cups.begin(), cups.end(), next_val)) == cups.end()) {
			if (next_val < min) {
				next_val = max;
			} else {
				next_val--;
			}
		}

		cups.insert(it + 1, tmp.begin(), tmp.end());
		cups.push_back(cups.front());
		cups.erase(cups.begin());
	}

	while (cups.front() != 1) {
		cups.push_back(cups.front());
		cups.erase(cups.begin());
	}

	result = 0;

	for (uint8_t i = 1; i < cups.size(); i++) {
		result = (result * 10) + cups[i];
	}

	return result;
}

void AoC2020_day23::tests() {
	uint64_t result;

	if (init({"389125467"})) {
		result = perform_crab_moves(10); // 92658374
		result = perform_crab_moves(100); // 67384529
	}
}

bool AoC2020_day23::part1() {

	result1_ = std::to_string(perform_crab_moves(100));

	return true;
}

bool AoC2020_day23::part2() {

	result2_ = std::to_string(0);

	return true;
}

int32_t AoC2020_day23::get_aoc_day() {
	return 23;
}

int32_t AoC2020_day23::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day23 day23;

	return day23.main_execution();
}
