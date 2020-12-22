#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>
#include <string>

const std::regex VALUE("^mem\\[(\\d+)\\] = (\\d+)$");
const std::regex MASK("^mask = ([X||1|0]{36})$");

class AoC2020_day14 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::pair<bool, std::pair<uint64_t, uint64_t>>> instructions_;
	uint64_t get_final_mem_sum();
};

bool AoC2020_day14::init(const std::vector<std::string> lines) {
	std::smatch sm;
	uint64_t val1, val2;

	instructions_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, MASK)) {
			val1 = val2 = 0;

			for (size_t j = 0; j < sm.str(1).size(); j++) {
				val1 = val1 << 1;
				val2 = val2 << 1;

				switch (sm.str(1)[j]) {
					case '1':
						val1 |= 0;
						val2 |= 1;
						break;
					case '0':
						val1 |= 0;
						val2 |= 0;
						break;
					case 'X':
						val1 |= 1;
						val2 |= 0;
						break;
					default:
						std::cout << "Invalid instruction at line " << i + 1 << std::endl;
						return false;
				}
			}

			instructions_.push_back({true, {val1, val2}});
		} else if (std::regex_match(lines[i], sm, VALUE)) {
			val1 = std::stoi(sm.str(1));
			val2 = std::stoi(sm.str(2));

			instructions_.push_back({false, {val1, val2}});
		} else {
			std::cout << "Invalid instruction at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

uint64_t AoC2020_day14::get_final_mem_sum() {
	uint64_t result = 0, mask = 0, preset = 0, val;
	std::map<uint64_t, uint64_t> memory = {};

	for (size_t i = 0; i < instructions_.size(); i++) {
		if (instructions_[i].first) {
			mask = instructions_[i].second.first;
			preset = instructions_[i].second.second;
		} else {
			val = instructions_[i].second.second;
			val &= mask;
			val |= preset;
			memory[instructions_[i].second.first] = val;
		}
	}

	for (auto it = memory.begin(); it != memory.end(); it++) {
		result += it->second;
	}

	return result;
}

void AoC2020_day14::tests() {
	uint64_t result;

	if (init({"mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X", "mem[8] = 11", "mem[7] = 101", "mem[8] = 0"})) {
		result = get_final_mem_sum(); // 165
	}
}

bool AoC2020_day14::part1() {

	result1_ = std::to_string(get_final_mem_sum());

	return true;
}

bool AoC2020_day14::part2() {

	// result2_ = std::to_string();

	return true;
}

int32_t AoC2020_day14::get_aoc_day() {
	return 14;
}

int32_t AoC2020_day14::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day14 day14;

	return day14.main_execution();
}
