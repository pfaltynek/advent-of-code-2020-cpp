#include "./../common/aoc.hpp"
#include <regex>
#include <string>

const std::regex BOARDING_PASS("^([F|B]{7})([R|L]{3})$");

class AoC2020_day05 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::pair<uint8_t, uint8_t>> boarding_passes_;
	uint8_t decode_binary(const std::string binary_string);
	uint32_t get_highest_seat_id();
};

bool AoC2020_day05::init(const std::vector<std::string> lines) {
	std::smatch sm;

	boarding_passes_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {

		if (std::regex_match(lines[i], sm, BOARDING_PASS)) {
			boarding_passes_.push_back({decode_binary(sm.str(1)), decode_binary(sm.str(2))});
		} else {
			std::cout << "Invalid boarding pas at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

uint8_t AoC2020_day05::decode_binary(const std::string binary_string) {
	uint8_t result = 0, shift_value = 1;

	for (auto it = binary_string.rbegin(); it != binary_string.rend(); it++) {
		if (*it == 'B' || *it == 'R') {
			result |= shift_value;
		}
		shift_value = shift_value << 1;
	}

	return result;
}

uint32_t AoC2020_day05::get_highest_seat_id() {
	uint32_t result = 0, tmp;

	for (size_t i = 0; i < boarding_passes_.size(); i++) {
		tmp = boarding_passes_[i].first * 8 + boarding_passes_[i].second;

		if (tmp > result) {
			result = tmp;
		}
	}

	return result;
}

void AoC2020_day05::tests() {
	uint32_t result;

	if (init({"BFFFBBFRRR"})) {
		result = get_highest_seat_id(); // 70, 7 -> 567
	}

	if (init({"FFFBBBFRRR"})) {
		result = get_highest_seat_id(); // 14, 7 -> 119
	}

	if (init({"BBFFBBFRLL"})) {
		result = get_highest_seat_id(); // 102, 4 -> 820
	}
}

bool AoC2020_day05::part1() {

	result1_ = std::to_string(get_highest_seat_id());

	return true;
}

bool AoC2020_day05::part2() {

	//result2_ = std::to_string(get_valid_passports_count(true));

	return true;
}

int32_t AoC2020_day05::get_aoc_day() {
	return 5;
}

int32_t AoC2020_day05::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day05 day05;

	return day05.main_execution();
}
