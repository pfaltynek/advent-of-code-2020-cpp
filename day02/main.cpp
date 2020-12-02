#include "./../common/aoc.hpp"
#include <regex>
#include <string>

const std::regex PASSWORD_RULE("^(\\d+)-(\\d+) ([a-z]): ([a-z]+)$");

struct password_rule {
	int32_t min;
	int32_t max;
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

void AoC2020_day02::tests() {
	int64_t result;

	if (init({"1-3 a: abcde", "1-3 b: cdefg", "2-9 c: ccccccccc"})) {
	}
}

bool AoC2020_day02::part1() {
	int64_t result = 0;

	return false;
}

bool AoC2020_day02::part2() {
	int64_t result = 0;

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
