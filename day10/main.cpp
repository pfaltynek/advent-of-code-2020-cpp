#include "./../common/aoc.hpp"
#include <algorithm>
#include <string>

class AoC2020_day10 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint8_t> joltages_;
	uint32_t get_joltage_differences();
	uint64_t get_adapters_arrangement_count();
	uint64_t get_arangement_count_multiplicator(uint8_t chunk_length);
};

bool AoC2020_day10::init(const std::vector<std::string> lines) {

	joltages_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		joltages_.push_back(static_cast<uint8_t>(std::stoi(lines[i])));
	}

	return true;
}

uint32_t AoC2020_day10::get_joltage_differences() {
	std::sort(joltages_.begin(), joltages_.end());
	uint32_t diff1, diff2, diff3, diff;
	uint8_t prev = 0;

	diff1 = diff2 = 0;
	diff3 = 1;

	for (size_t i = 0; i < joltages_.size(); i++) {
		diff = joltages_[i] - prev;
		prev = joltages_[i];

		switch (diff) {
			case 1:
				diff1++;
				break;
			case 2:
				diff2++;
				break;
			case 3:
				diff3++;
				break;
			default:
				return 0;
		}
	}

	return diff1 * diff3;
}

uint64_t AoC2020_day10::get_arangement_count_multiplicator(uint8_t chunk_length) {

	// each chunk can make more arrangements => multiplies actual arrangement count
	// multiplication constant manually calculated (paper & pencil)
	switch (chunk_length) {
		case 1:
		case 2:
			return 1;
		case 3:
			return 2;
		case 4:
			return 4;
		case 5:
			return 7;
			break;
		default:
			return 0;
	}
}

uint64_t AoC2020_day10::get_adapters_arrangement_count() {
	uint64_t result = 1;
	uint8_t chunk_length = 1, prev = 0, diff;

	// identify chunks as sequences of diffs at 1, starting and finished by diff 3
	// diff 2 should be a problem :(
	for (size_t i = 0; i < joltages_.size(); i++) {
		diff = joltages_[i] - prev;

		switch (diff) {
			case 1:
				chunk_length++;
				break;
			case 3:
				result *= get_arangement_count_multiplicator(chunk_length);
				chunk_length = 1;
				break;
			case 2:
			default:
				// problem
				return 0;
		}
		prev = joltages_[i];
	}

	result *= get_arangement_count_multiplicator(chunk_length);

	return result;
}

void AoC2020_day10::tests() {
	uint64_t result;

	if (init({"16", "10", "15", "5", "1", "11", "7", "19", "6", "12", "4"})) {
		result = get_joltage_differences();		   // 7 * 5
		result = get_adapters_arrangement_count(); // 8
	}

	if (init({"28", "33", "18", "42", "31", "14", "46", "20", "48", "47", "24", "23", "49", "45", "19", "38",
			  "39", "11", "1",	"32", "25", "35", "8",	"17", "7",	"9",  "4",	"2",  "34", "10", "3"})) {
		result = get_joltage_differences();		   // 22 * 10
		result = get_adapters_arrangement_count(); // 19208
	}
}

bool AoC2020_day10::part1() {

	result1_ = std::to_string(get_joltage_differences());

	return true;
}

bool AoC2020_day10::part2() {

	result2_ = std::to_string(get_adapters_arrangement_count());

	return true;
}

int32_t AoC2020_day10::get_aoc_day() {
	return 10;
}

int32_t AoC2020_day10::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day10 day10;

	return day10.main_execution();
}
