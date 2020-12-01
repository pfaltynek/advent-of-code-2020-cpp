#include "./../common/aoc.hpp"
#include <string>

class AoC2020_day01 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool find_2020_expences_in_sum(int32_t& result);
	std::vector<int32_t> expenses_;
};

bool AoC2020_day01::init(const std::vector<std::string> lines) {

	expenses_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		expenses_.push_back(std::stoi(lines[i]));
	}

	return true;
}

bool AoC2020_day01::find_2020_expences_in_sum(int32_t& result) {

	for (size_t i = 0; i < expenses_.size(); i++) {
		for (size_t j = i + 1; j < expenses_.size(); j++) {
			if (expenses_[i] + expenses_[j] == 2020) {
				result = expenses_[i] * expenses_[j];
				return true;
			}
		}
	}

	return false;
}

int32_t AoC2020_day01::get_aoc_day() {
	return 1;
}

int32_t AoC2020_day01::get_aoc_year() {
	return 2020;
}

void AoC2020_day01::tests() {
	int32_t result;

	if (init({"1721", "979", "366", "299", "675", "1456"})) {
		if (find_2020_expences_in_sum(result)) {
			// 514579
		}
	}
}

bool AoC2020_day01::part1() {
	int32_t result = 0;

	if (find_2020_expences_in_sum(result)) {
		result1_ = std::to_string(result);
		return true;
	}

	return false;
}

bool AoC2020_day01::part2() {
	int64_t result = 0;

	return false;
}

int main(void) {
	AoC2020_day01 day01;

	return day01.main_execution();
}
