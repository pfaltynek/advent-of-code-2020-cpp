#include "./../common/aoc.hpp"
#include <string>

class AoC2020_day09 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint64_t> xmas_;
	uint64_t rule_breakning_number_;
	uint64_t get_rule_breaking_number(const uint32_t preamble_size);
	uint64_t get_encryption_weakness();
};

bool AoC2020_day09::init(const std::vector<std::string> lines) {

	xmas_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		xmas_.push_back(std::stoull(lines[i]));
	}

	return true;
}

uint64_t AoC2020_day09::get_rule_breaking_number(const uint32_t preamble_size) {
	bool rule_fulfilled;

	for (size_t i = preamble_size; i < xmas_.size(); i++) {
		rule_fulfilled = false;

		for (size_t j = i - preamble_size; j < i - 1; j++) {
			for (size_t k = j + 1; k < i; k++) {
				if (xmas_[i] == xmas_[j] + xmas_[k]) {
					rule_fulfilled = true;
					break;
				}
			}

			if (rule_fulfilled) {
				break;
			}
		}

		if (!rule_fulfilled) {
			rule_breakning_number_ = xmas_[i];
			return xmas_[i];
		}
	}

	return 0;
}

uint64_t AoC2020_day09::get_encryption_weakness() {
	uint64_t sum = 0, min = UINT64_MAX, max = 0;
	uint32_t idx = 0, count = 0;

	while (true) {
		sum += xmas_[idx + count];

		if (min > xmas_[idx + count]) {
			min = xmas_[idx + count];
		}

		if (max < xmas_[idx + count]) {
			max = xmas_[idx + count];
		}

		if (sum == rule_breakning_number_) {
			return min + max;
		} else if (sum > rule_breakning_number_) {
			idx++;
			min = UINT64_MAX;
			max = sum = count = 0;
		} else {
			count++;
		}
	}
}

void AoC2020_day09::tests() {
	int64_t result;

	if (init({"35", "20", "15", "25", "47", "40", "62", "55", "65", "95", "102", "117", "150", "182", "127", "219", "299", "277", "309", "576"})) {
		result = get_rule_breaking_number(5); // 127
		result = get_encryption_weakness();	  // 62
	}
}

bool AoC2020_day09::part1() {

	result1_ = std::to_string(get_rule_breaking_number(25));

	return true;
}

bool AoC2020_day09::part2() {

	result2_ = std::to_string(get_encryption_weakness());

	return true;
}

int32_t AoC2020_day09::get_aoc_day() {
	return 9;
}

int32_t AoC2020_day09::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day09 day09;

	return day09.main_execution();
}
