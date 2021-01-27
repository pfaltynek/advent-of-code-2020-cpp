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
	void generate_all_valid_messages();
	void build_cache(const uint32_t rule_no);
	uint32_t get_count_of_matching_messages();
	uint32_t get_count_of_matching_messages_with_loops();
	std::vector<std::string> messages_, messages_loop;
	std::map<uint32_t, std::vector<std::string>> cache_;
	std::map<uint32_t, std::vector<std::vector<uint32_t>>> rules_;
	std::map<uint32_t, char> letters_;
	uint32_t part1_result;
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

void AoC2020_day19::build_cache(const uint32_t rule_no) {
	std::vector<std::string> rule_cache, one_rule, tmp, subresult;
	std::string letter = "";

	// make sure all required rules are in cache
	if (letters_.count(rule_no)) {
		letter += letters_[rule_no];
		cache_[rule_no] = {letter};

		return;
	}

	for (size_t i = 0; i < rules_[rule_no].size(); i++) {
		for (size_t j = 0; j < rules_[rule_no][i].size(); j++) {
			uint32_t x = rules_[rule_no][i][j];
			if (!cache_.count(x)) {
				build_cache(x);
			}
		}
	}

	// add current rule into cache
	rule_cache.clear();
	for (size_t i = 0; i < rules_[rule_no].size(); i++) {
		one_rule = {};
		for (size_t j = 0; j < rules_[rule_no][i].size(); j++) {
			subresult = cache_[rules_[rule_no][i][j]];

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
		rule_cache.insert(rule_cache.end(), one_rule.begin(), one_rule.end());
	}
	cache_[rule_no] = rule_cache;
}

uint32_t AoC2020_day19::get_count_of_matching_messages() {
	uint32_t result = 0;

	messages_loop.clear();

	for (size_t i = 0; i < messages_.size(); i++) {
		if (std::find(cache_[0].begin(), cache_[0].end(), messages_[i]) != cache_[0].end()) {
			result++;
		} else {
			messages_loop.push_back(messages_[i]);
		}
	}

	part1_result = result;

	return result;
}

uint32_t AoC2020_day19::get_count_of_matching_messages_with_loops() {
	uint32_t result = 0, left = 0, right = 0;
	bool lfound, rfound;

	// defined rules make the looped message look like 42 42 31, 42 42 42 31 31
	// so message starts by rule 42 and ends by rule 31, rest of the message contains from left the 42 rule part
	// and from right the 31 rule part, count of parts by rule 42 is alway higher than count
	// of parts by rule 31

	for (size_t i = 0; i < messages_loop.size(); i++) {
		std::string msg = messages_loop[i], part;
		size_t pos, start, msg_size, part_size;

		msg_size = msg.size();
		left = right = 0;

		do {
			lfound = rfound = false;

			for (size_t l = 0; l < cache_[42].size(); l++) {
				part = cache_[42][l];
				part_size = cache_[42][l].size();

				pos = msg.find(cache_[42][l]);
				if (pos == 0) {
					msg = msg.substr(cache_[42][l].size(), msg.size() - cache_[42][l].size());
					lfound = true;
					left++;
					break;
				}
			}

			msg_size = msg.size();

			for (size_t r = 0; r < cache_[31].size(); r++) {
				part = cache_[31][r];
				part_size = cache_[31][r].size();

				start = msg_size - part_size;
				pos = msg.find(cache_[31][r], start);
				if (pos == start) {
					msg = msg.substr(0, start);
					rfound = true;
					right++;
					break;
				}
			}
		} while ((rfound | lfound) && msg.size());

		if (!msg.size() && (left > right) && (right > 0)) {
			result++;
		}
	}

	return result;
}

void AoC2020_day19::generate_all_valid_messages() {

	cache_.clear();

	build_cache(0);
}

void AoC2020_day19::tests() {
	int64_t result;

	if (init({"0: 4 1 5", "1: 2 3 | 3 2", "2: 4 4 | 5 5", "3: 4 5 | 5 4", "4: \"a\"", "5: \"b\"", "", "aaaabb", "aaabab", "abbabb", "abbbab", "aabaab",
			  "aabbbb", "abaaab", "ababbb"})) {
		generate_all_valid_messages();
		result = get_count_of_matching_messages(); // 8
	}

	if (init({"0: 4 1 5", "1: 2 3 | 3 2", "2: 4 4 | 5 5", "3: 4 5 | 5 4", "4: \"a\"", "5: \"b\"", "", "ababbb", "bababa", "abbbab", "aaabbb", "aaaabbb"})) {
		generate_all_valid_messages();
		result = get_count_of_matching_messages(); // 2
	}

	if (init({"42: 9 14 | 10 1",
			  "9: 14 27 | 1 26",
			  "10: 23 14 | 28 1",
			  "1: \"a\"",
			  "11: 42 31",
			  "5: 1 14 | 15 1",
			  "19: 14 1 | 14 14",
			  "12: 24 14 | 19 1",
			  "16: 15 1 | 14 14",
			  "31: 14 17 | 1 13",
			  "6: 14 14 | 1 14",
			  "2: 1 24 | 14 4",
			  "0: 8 11",
			  "13: 14 3 | 1 12",
			  "15: 1 | 14",
			  "17: 14 2 | 1 7",
			  "23: 25 1 | 22 14",
			  "28: 16 1",
			  "4: 1 1",
			  "20: 14 14 | 1 15",
			  "3: 5 14 | 16 1",
			  "27: 1 6 | 14 18",
			  "14: \"b\"",
			  "21: 14 1 | 1 14",
			  "25: 1 1 | 1 14",
			  "22: 14 14",
			  "8: 42",
			  "26: 14 22 | 1 20",
			  "18: 15 15",
			  "7: 14 5 | 1 21",
			  "24: 14 1",
			  "",
			  "abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa",
			  "bbabbbbaabaabba",
			  "babbbbaabbbbbabbbbbbaabaaabaaa",
			  "aaabbbbbbaaaabaababaabababbabaaabbababababaaa",
			  "bbbbbbbaaaabbbbaaabbabaaa",
			  "bbbababbbbaaaaaaaabbababaaababaabab",
			  "ababaaaaaabaaab",
			  "ababaaaaabbbaba",
			  "baabbaaaabbaaaababbaababb",
			  "abbbbabbbbaaaababbbbbbaaaababb",
			  "aaaaabbaabaaaaababaa",
			  "aaaabbaaaabbaaa",
			  "aaaabbaabbaaaaaaabbbabbbaaabbaabaaa",
			  "babaaabbbaaabaababbaabababaaab",
			  "aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba"})) {
		generate_all_valid_messages();
		result = get_count_of_matching_messages();			   // 3
		result += get_count_of_matching_messages_with_loops(); // 12 in total
	}
}

bool AoC2020_day19::part1() {

	generate_all_valid_messages();
	result1_ = std::to_string(get_count_of_matching_messages());

	return true;
}

bool AoC2020_day19::part2() {

	part1_result += get_count_of_matching_messages_with_loops();
	result2_ = std::to_string(part1_result);

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
