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
	int64_t evaluate_line_result(const std::string line, const bool add_has_higher_priority);
	std::string solve_brackets(const std::string line, const bool add_has_higher_priority);
	std::string solve_adds(const std::string line);
	int64_t get_all_lines_results_sum(const bool add_has_higher_priority);
};

bool AoC2020_day18::init(const std::vector<std::string> lines) {
	lines_ = lines;

	return true;
}

std::string AoC2020_day18::solve_brackets(const std::string line, const bool add_has_higher_priority) {
	std::string parent = "", output = line, number1 = "", number2 = "", input;
	bool in_parent, was_parent;

	do {
		in_parent = was_parent = false;
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
						output += std::to_string(evaluate_line_result(parent, add_has_higher_priority));
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

	return output;
}

std::string AoC2020_day18::solve_adds(const std::string line) {
	bool first = true;
	std::string input = line + ' ', output = "", number1 = "", number2 = "";

	for (size_t i = 0; i < input.size(); i++) {
		if (isdigit(input[i])) {
			number1 += input[i];
		} else {
			if (first) {
				switch (input[i]) {
					case '+':
						number2 = number1;
						number1 = "";
						first = false;
						break;
					case '*':
						output += number1 + '*';
						number1 = "";
						break;
					case ' ':
						output += number1;
						break;
				}
			} else {
				number2 = std::to_string(stoll(number1) + stoll(number2));

				switch (input[i]) {
					case '+':
						number1 = "";
						first = false;
						break;
					case '*':
						first = true;
						output += number2 + '*';
						number1 = number2 = "";
						break;
					case ' ':
						output += number2;
						break;
				}
			}
		}
	}

	return output;
}

int64_t AoC2020_day18::evaluate_line_result(const std::string line, const bool add_has_higher_priority) {
	std::string output, number1 = "";
	int64_t result = 0;

	bool add = true;

	output = solve_brackets(line, add_has_higher_priority);

	if (add_has_higher_priority) {
		output = solve_adds(output);
	}

	add = true;
	output += '*';
	for (size_t i = 0; i < output.size(); i++) {
		if (isdigit(output[i])) {
			number1 += output[i];
		} else {
			if (add) {
				result += stoll(number1);
			} else {
				result *= stoll(number1);
			}
			switch (output[i]) {
				case '+':
					add = true;
					break;
				case '*':
					add = false;
					break;
			}
			number1 = "";
		}
	}

	return result;
}

int64_t AoC2020_day18::get_all_lines_results_sum(const bool add_has_higher_priority) {
	int64_t result = 0;

	for (size_t i = 0; i < lines_.size(); i++) {
		result += evaluate_line_result(lines_[i], add_has_higher_priority);
	}

	return result;
}

void AoC2020_day18::tests() {
	int64_t result;

	if (init({"1 + 2 * 3 + 4 * 5 + 6"})) {
		result = get_all_lines_results_sum(false); // 71
		result = get_all_lines_results_sum(true);  // 231
	}

	if (init({"1 + (2 * 3) + (4 * (5 + 6))"})) {
		result = get_all_lines_results_sum(false); // 51
		result = get_all_lines_results_sum(true);  // 51
	}

	if (init({"2 * 3 + (4 * 5)"})) {
		result = get_all_lines_results_sum(false); // 26
		result = get_all_lines_results_sum(true);  // 46
	}

	if (init({"5 + (8 * 3 + 9 + 3 * 4 * 3)"})) {
		result = get_all_lines_results_sum(false); // 437
		result = get_all_lines_results_sum(true);  // 1445
	}

	if (init({"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"})) {
		result = get_all_lines_results_sum(false); // 12240
		result = get_all_lines_results_sum(true);  // 669060
	}

	if (init({"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"})) {
		result = get_all_lines_results_sum(false); // 13632
		result = get_all_lines_results_sum(true);  // 23340
	}
}

bool AoC2020_day18::part1() {

	result1_ = std::to_string(get_all_lines_results_sum(false));

	return true;
}

bool AoC2020_day18::part2() {

	result2_ = std::to_string(get_all_lines_results_sum(true));

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
