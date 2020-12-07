#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>
#include <string>

const std::regex BAGS_RULE("^(.*) bags contain (.*).$");
const std::regex BAGS_CONTENT_RULE("^(\\d) (.*) bags?$");
const std::regex BAGS_NO_CONTENT_RULE("^no other bags$");

const std::string MY_BAG_COLOR = "shiny gold";

class AoC2020_day07 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::pair<std::string, std::map<std::string, uint8_t>>> bags_;
	std::map<std::string, std::vector<std::string>> bags_reverse_;
	uint32_t get_colors_count();
};

bool AoC2020_day07::init(const std::vector<std::string> lines) {
	std::vector<std::string> group = {};
	std::smatch sm;
	std::string color, content;
	std::vector<std::string> bags;
	std::map<std::string, uint8_t> content_map;

	bags_.clear();
	bags_reverse_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, BAGS_RULE)) {
			color = sm.str(1);
			content = sm.str(2);

			if (std::regex_match(content, sm, BAGS_NO_CONTENT_RULE)) {
				bags_.push_back({color, {}});
			} else {
				bags = split(content, ", ");
				content_map = {};

				for (size_t j = 0; j < bags.size(); j++) {
					if (std::regex_match(bags[j], sm, BAGS_CONTENT_RULE)) {
						content_map[sm.str(2)] = std::stoi(sm.str(1));

						if (bags_reverse_.count(sm.str(2))) {
							bags_reverse_[sm.str(2)].push_back(color);
						} else {
							bags_reverse_[sm.str(2)] = {color};
						}
					} else {
						std::cout << "Invalid bag content at line " << i + 1 << ", position " << j + 1 << std::endl;
						return false;
					}
				}

				bags_.push_back({color, content_map});
			}
		} else {
			std::cout << "Invalid rule at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

uint32_t AoC2020_day07::get_colors_count() {
	std::vector<std::string> curr, next = {};
	bool all_root = false;
	std::map<std::string, uint32_t> map_bags = {};

	curr = {MY_BAG_COLOR};

	while (!all_root) {
		all_root = true;
		for (size_t i = 0; i < curr.size(); i++) {
			if (bags_reverse_.count(curr[i])) {
				next.insert(next.end(), bags_reverse_[curr[i]].begin(), bags_reverse_[curr[i]].end());
				all_root = false;
			} else {
				next.push_back(curr[i]);
			}
		}

		for (size_t i = 0; i < next.size(); i++) {
			map_bags[next[i]]++;
		}

		curr.swap(next);
		next = {};
	}

	return map_bags.size();
}

void AoC2020_day07::tests() {
	uint32_t result;

	if (init({"light red bags contain 1 bright white bag, 2 muted yellow bags.", "dark orange bags contain 3 bright white bags, 4 muted yellow bags.",
			  "bright white bags contain 1 shiny gold bag.", "muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.",
			  "shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.", "dark olive bags contain 3 faded blue bags, 4 dotted black bags.",
			  "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.", "faded blue bags contain no other bags.",
			  "dotted black bags contain no other bags."})) {
		result = get_colors_count(); // 4
	}
}

bool AoC2020_day07::part1() {

	result1_ = std::to_string(get_colors_count());

	return true;
}

bool AoC2020_day07::part2() {

	result2_ = std::to_string(get_colors_count());

	return true;
}

int32_t AoC2020_day07::get_aoc_day() {
	return 7;
}

int32_t AoC2020_day07::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day07 day07;

	return day07.main_execution();
}
