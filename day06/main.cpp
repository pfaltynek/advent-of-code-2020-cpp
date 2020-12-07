#include "./../common/aoc.hpp"
#include <map>
#include <string>

class AoC2020_day06 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::vector<std::string>> forms_;
	uint32_t get_form_sum(const bool part2);
};

bool AoC2020_day06::init(const std::vector<std::string> lines) {
	std::vector<std::string> group = {};

	forms_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {

		if (lines[i].empty()) {
			if (group.size()) {
				forms_.push_back(group);
			}
			group.clear();
		} else {
			group.push_back(lines[i]);
		}
	}

	if (group.size()) {
		forms_.push_back(group);
	}

	return true;
}

uint32_t AoC2020_day06::get_form_sum(const bool part2) {
	uint32_t result = 0;
	std::map<char, uint32_t> map;

	for (size_t i = 0; i < forms_.size(); i++) {
		map = {};

		for (size_t j = 0; j < forms_[i].size(); j++) {
			for (size_t k = 0; k < forms_[i][j].size(); k++) {
				map[forms_[i][j][k]]++;
			}
		}
		if (part2) {
			for (auto it = map.begin(); it != map.end(); it++)
			{
				if (it->second == forms_[i].size()){
					result++;
				}
			}
		} else {
			result += map.size();
		}
	}

	return result;
}

void AoC2020_day06::tests() {
	uint32_t result;

	if (init({"abcx", "abcy", "abcz"})) {
		result = get_form_sum(false); // 6
	}

	if (init({"abc", "", "a", "b", "c", "", "ab", "ac", "", "a", "a", "a", "a", "", "b"})) {
		result = get_form_sum(false); // 11
		result = get_form_sum(true);  // 6
	}
}

bool AoC2020_day06::part1() {

	result1_ = std::to_string(get_form_sum(false));

	return true;
}

bool AoC2020_day06::part2() {

	result2_ = std::to_string(get_form_sum(true));

	return true;
}

int32_t AoC2020_day06::get_aoc_day() {
	return 6;
}

int32_t AoC2020_day06::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day06 day06;

	return day06.main_execution();
}
