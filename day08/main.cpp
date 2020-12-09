#include "./../common/aoc.hpp"
#include <regex>
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
	std::vector<uint32_t> log_;
	bool get_acc_final_value(int32_t& acc_final);
	int32_t get_patched_code_acc_final_value();
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

bool AoC2020_day08::get_acc_final_value(int32_t& acc_final) {
	uint32_t addr = 0;

	log_.clear();
	acc_final = 0;

	while (true) {
		switch (boot_code_[addr].first) {
			case instruction_t::nop:
				addr++;
				break;
			case instruction_t::jmp:
				addr += boot_code_[addr].second;
				break;
			case instruction_t::acc:
				acc_final += boot_code_[addr].second;
				addr++;
				break;
		}
		if (std::find(log_.begin(), log_.end(), addr) != log_.end()) {
			return false;
		} else if (addr >= boot_code_.size()) {
			return true;
		} else {
			log_.push_back(addr);
		}
	}
}

int32_t AoC2020_day08::get_patched_code_acc_final_value() {
	std::vector<uint32_t> log = log_;
	auto it = log.rbegin();
	bool found = false;
	int32_t acc_final = 0;

	while (!found && it != log.rend()) {
		switch (boot_code_[*it].first) {
			case instruction_t::jmp:
				boot_code_[*it].first = instruction_t::nop;
				found = get_acc_final_value(acc_final);
				boot_code_[*it].first = instruction_t::jmp;
				break;
			case instruction_t::nop:
				boot_code_[*it].first = instruction_t::jmp;
				found = get_acc_final_value(acc_final);
				boot_code_[*it].first = instruction_t::nop;
				break;
			case instruction_t::acc:
			default:

				break;
		}
		it++;
	}

	return acc_final;
}

void AoC2020_day08::tests() {
	int32_t result;

	if (init({"nop +0", "acc +1", "jmp +4", "acc +3", "jmp -3", "acc -99", "acc +1", "jmp -4", "acc +6"})) {
		if (!get_acc_final_value(result)) { // 5
			result = get_patched_code_acc_final_value(); // 8
		}
	}
}

bool AoC2020_day08::part1() {
	int32_t result;

	if (!get_acc_final_value(result)) {
		result1_ = std::to_string(result);
		return true;
	}

	return false;
}

bool AoC2020_day08::part2() {

	result2_ = std::to_string(get_patched_code_acc_final_value());

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
