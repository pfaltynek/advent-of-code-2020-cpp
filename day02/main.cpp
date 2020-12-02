#include "./../common/aoc.hpp"
#include <regex>
#include <string>

const std::regex PASSWORD_RULE("^(\\d+)-(\\d+) ([a-z]): ([a-z]+)$");

struct password_rule {
	uint32_t min;
	uint32_t max;
	char chr;
	std::string password;
};

class AoC2020_day02 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool is_rule_valid(const password_rule& rule);
	uint32_t get_valid_password_rules_count();
	std::vector<password_rule> rules_;
};

bool AoC2020_day02::init(const std::vector<std::string> lines) {
	std::smatch sm;

	rules_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		password_rule rule{};

		if (std::regex_match(lines[i], sm, PASSWORD_RULE)) {
			rule.min = stoi(sm.str(1));
			rule.max = stoi(sm.str(2));
			rule.chr = sm.str(3)[0];
			rule.password = sm.str(4);
			rules_.push_back(rule);
		} else {
			std::cout << "Invalid password rule at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

bool AoC2020_day02::is_rule_valid(const password_rule& rule) {
	uint32_t count = 0;
	size_t last_pos = 0;

	do {
		last_pos = rule.password.find(rule.chr, last_pos);
		if (last_pos != std::string::npos) {
			last_pos++;
			count++;
		} else {
			break;
		}
	} while (true);

	if ((count <= rule.max) && (count >= rule.min)) {
		return true;
	} else {
		return false;
	}
}

uint32_t AoC2020_day02::get_valid_password_rules_count() {
	uint32_t result = 0;
	for (size_t i = 0; i < rules_.size(); i++)
	{
		if (is_rule_valid(rules_[i])) {
			result++;
		}
	}

	return result;
}

void AoC2020_day02::tests() {
	uint32_t result;

	if (init({"1-3 a: abcde", "1-3 b: cdefg", "2-9 c: ccccccccc"})) {
		result = get_valid_password_rules_count(); // 2
	}
}

bool AoC2020_day02::part1() {

	result1_ = std::to_string(get_valid_password_rules_count());

	return true;
}

bool AoC2020_day02::part2() {
	uint32_t result = 0;

	return false;
}

int32_t AoC2020_day02::get_aoc_day() {
	return 2;
}

int32_t AoC2020_day02::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day02 day02;

	return day02.main_execution();
}
