#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <regex>
#include <set>
#include <string>

const std::regex INSTRUCTION_FORMAT("^(acc|jmp|nop) ([+|-]\\d+)$");

enum instruction_t { nop = 0, acc = 1, jmp = 2 };

class AoC2020_day08 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::pair<instruction_t, int32_t>> boot_code_;
	int32_t get_acc_value_before_loop();
};

bool AoC2020_day08::init(const std::vector<std::string> lines) {
	std::smatch sm;
	instruction_t inst;

	boot_code_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, INSTRUCTION_FORMAT)) {
			inst = instruction_t::nop;
			if (sm.str(1) == "jmp") {
				inst = instruction_t::jmp;
			} else if (sm.str(1) == "acc") {
				inst = instruction_t::acc;
			}

			boot_code_.push_back({inst, stoi(sm.str(2))});

		} else {
			std::cout << "Invalid instruction at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

int32_t AoC2020_day08::get_acc_value_before_loop() {
	int32_t result = 0;
	std::set<uint32_t> log = {};
	uint32_t addr = 0;

	while(true) {
		switch (boot_code_[addr].first) {
			case instruction_t::nop:
				addr++;
				break;
			case instruction_t::jmp:
				addr += boot_code_[addr].second;
				break;
			case instruction_t::acc:
				result += boot_code_[addr].second;
				addr++;
				break;
		}
		if (log.count(addr)) {
			return result;
		} else {
			log.emplace(addr);
		}
	}

	return result;
}

void AoC2020_day08::tests() {
	int32_t result;

	if (init({"nop +0", "acc +1", "jmp +4", "acc +3", "jmp -3", "acc -99", "acc +1", "jmp -4", "acc +6"})) {
		result = get_acc_value_before_loop(); // 4
	}
}

bool AoC2020_day08::part1() {

	result1_ = std::to_string(get_acc_value_before_loop());

	return true;
}

bool AoC2020_day08::part2() {

	//result2_ = std::to_string(get_bags_count(MY_BAG_COLOR));

	return true;
}

int32_t AoC2020_day08::get_aoc_day() {
	return 8;
}

int32_t AoC2020_day08::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day08 day08;

	return day08.main_execution();
}
