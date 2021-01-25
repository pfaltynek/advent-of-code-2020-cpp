#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>
#include <string>

const std::regex RULE("^(\\d+): (.*)$");
const std::string LETTER_A = "\"a\"";
const std::string LETTER_B = "\"b\"";

class AoC2020_day19 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	void generate_all_valid_messaages();
	std::vector<std::string> process_rule(const uint32_t rule_no);
	uint32_t get_count_of_matching_messages();
	std::vector<std::string> messages_, all_valid_messages_;
	std::map<uint32_t, std::vector<std::vector<uint32_t>>> rules_;
	std::map<uint32_t, char> letters_;
};

bool AoC2020_day19::init(const std::vector<std::string> lines) {
	bool rules = true;
	std::smatch sm;
	std::vector<std::string> splitted, subrule;
	std::vector<std::vector<uint32_t>> subrules;
	uint32_t rule_no;
	std::vector<uint32_t> tmp;

	messages_.clear();
	rules_.clear();
	letters_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (rules) {
			if (lines[i].empty()) {
				rules = false;
			} else {
				if (std::regex_match(lines[i], sm, RULE)) {
					rule_no = std::stoul(sm.str(1));
					if (sm.str(2) == LETTER_A) {
						letters_[rule_no] = 'a';
					} else if (sm.str(2) == LETTER_B) {
						letters_[rule_no] = 'b';
					} else {
						splitted = split(sm.str(2), " | ");
						subrules.clear();

						for (size_t j = 0; j < splitted.size(); j++) {
							subrule = split(splitted[j], " ");
							tmp.clear();
							for (size_t k = 0; k < subrule.size(); k++) {
								tmp.push_back(std::stoul(subrule[k]));
							}
							subrules.push_back(tmp);
						}
						rules_[rule_no] = subrules;
					}
				} else {
					std::cout << "Invalid rule format at line " << i + 1 << std::endl;
					return false;
				}
			}
		} else {
			messages_.push_back(lines[i]);
		}
	}

	return true;
}

std::vector<std::string> AoC2020_day19::process_rule(const uint32_t rule_no) {
	std::string letter = "";
	std::vector<std::string> subresult, result = {};

	if (letters_.count(rule_no)) {
		letter += letters_[rule_no];
		return {letter};
	} else {
		std::vector<std::string> one_rule, tmp;

		for (size_t i = 0; i < rules_[rule_no].size(); i++) {
			one_rule = {};
			for (size_t j = 0; j < rules_[rule_no][i].size(); j++) {
				subresult = process_rule(rules_[rule_no][i][j]);

				if (one_rule.size()) {
					tmp.clear();

					for (size_t x = 0; x < one_rule.size(); x++) {
						for (size_t y = 0; y < subresult.size(); y++) {
							tmp.push_back(one_rule[x] + subresult[y]);
						}
					}

					one_rule = tmp;
				} else {
					one_rule = subresult;
				}
			}

			result.insert(result.end(), one_rule.begin(), one_rule.end());
		}

		return result;
	}
}

uint32_t AoC2020_day19::get_count_of_matching_messages() {
	uint32_t result = 0;

	for (size_t i = 0; i < messages_.size(); i++) {
		if (std::find(all_valid_messages_.begin(), all_valid_messages_.end(), messages_[i]) != all_valid_messages_.end()) {
			result++;
		}
	}

	return result;
}

void AoC2020_day19::generate_all_valid_messaages() {

	all_valid_messages_.clear();

	all_valid_messages_ = process_rule(0);
}

void AoC2020_day19::tests() {
	int64_t result;

	if (init({"0: 4 1 5", "1: 2 3 | 3 2", "2: 4 4 | 5 5", "3: 4 5 | 5 4", "4: \"a\"", "5: \"b\"", "", "aaaabb", "aaabab", "abbabb", "abbbab", "aabaab",
			  "aabbbb", "abaaab", "ababbb"})) {
		generate_all_valid_messaages();
		result = get_count_of_matching_messages(); // 8
	}

	if (init({"0: 4 1 5", "1: 2 3 | 3 2", "2: 4 4 | 5 5", "3: 4 5 | 5 4", "4: \"a\"", "5: \"b\"", "", "ababbb", "bababa", "abbbab", "aaabbb", "aaaabbb"})) {
		generate_all_valid_messaages();
		result = get_count_of_matching_messages(); // 2
	}
}

bool AoC2020_day19::part1() {

	generate_all_valid_messaages();
	result1_ = std::to_string(get_count_of_matching_messages());

	return true;
}

bool AoC2020_day19::part2() {

	result2_ = std::to_string(0);

	return true;
}

int32_t AoC2020_day19::get_aoc_day() {
	return 19;
}

int32_t AoC2020_day19::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day19 day19;

	return day19.main_execution();
}
