#include "./../common/aoc.hpp"
#include <map>
#include <string>

class AoC2020_day18 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> lines_;
	int64_t evaluate_line_result(const std::string line);
	int64_t get_all_lines_results_sum();
};

bool AoC2020_day18::init(const std::vector<std::string> lines) {
	lines_ = lines;

	return true;
}

int64_t AoC2020_day18::evaluate_line_result(const std::string line) {
	std::string parent = "", output = line, number = "", input;
	int64_t result = 0;
	bool in_parent = false;
	bool was_parent = true;
	bool add = true;

	do {
		in_parent = was_parent = false;
		add = true;
		input = output;
		output = "";

		for (size_t i = 0; i < input.size(); i++) {
			switch (input[i]) {
				case ' ':
					// skip
					break;
				case '(':
					if (in_parent) {
						output += '(';
						output += parent;
					}
					parent = "";
					in_parent = was_parent = true;
					break;
				case ')':
					if (in_parent) {
						output += std::to_string(evaluate_line_result(parent));
						parent = "";
						in_parent = false;
					} else {
						output += ')';
					}
					break;
				default:
					if (in_parent) {
						parent += input[i];
					} else {
						output += input[i];
					}
					break;
			}
		}
	} while (was_parent);

	output += '+';

	for (size_t i = 0; i < output.size(); i++) {
		if (isdigit(output[i])) {
			number += output[i];
		} else {
			if (add) {
				result += stoi(number);
			} else {
				result *= stoi(number);
			}
			switch (output[i]) {
				case '+':
					add = true;
					break;
				case '*':
					add = false;
					break;
				default:
					int z = 11;
					break;
			}
			number = "";
		}
	}

	return result;
}

int64_t AoC2020_day18::get_all_lines_results_sum() {
	int64_t result = 0;

	for (size_t i = 0; i < lines_.size(); i++) {
		result += evaluate_line_result(lines_[i]);
	}

	return result;
}

void AoC2020_day18::tests() {
	int64_t result;

	if (init({"1 + 2 * 3 + 4 * 5 + 6"})) {
		result = get_all_lines_results_sum(); // 71
	}

	if (init({"2 * 3 + (4 * 5)"})) {
		result = get_all_lines_results_sum(); // 26
	}

	if (init({"5 + (8 * 3 + 9 + 3 * 4 * 3)"})) {
		result = get_all_lines_results_sum(); // 437
	}

	if (init({"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"})) {
		result = get_all_lines_results_sum(); // 12240
	}

	if (init({"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"})) {
		result = get_all_lines_results_sum(); // 13632
	}
}

bool AoC2020_day18::part1() {

	result1_ = std::to_string(get_all_lines_results_sum());

	return true;
}

bool AoC2020_day18::part2() {

	// result2_ = std::to_string();

	return true;
}

int32_t AoC2020_day18::get_aoc_day() {
	return 18;
}

int32_t AoC2020_day18::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day18 day18;

	return day18.main_execution();
}
