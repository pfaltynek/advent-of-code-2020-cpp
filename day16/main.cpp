#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <regex>
#include <string>

const std::regex LIMITS("^(.*): (\\d+)-(\\d+) or (\\d+)-(\\d+)$");

typedef struct {
	std::string name;
	uint32_t min1;
	uint32_t max1;
	uint32_t min2;
	uint32_t max2;

	bool fits(uint16_t value) const {
		return (value >= min1 && value <= max1) || (value >= min2 && value <= max2);
	}
} limits_str;

class AoC2020_day16 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint32_t scan_tickets();
	std::vector<limits_str> limits_;
	std::vector<uint32_t> my_ticket_;
	std::vector<std::vector<uint32_t>> nearby_tickets_;
};

bool AoC2020_day16::init(const std::vector<std::string> lines) {
	std::smatch sm;
	bool limits_done = false, my_ticket_done = false, my_ticket_title_done = false, nearby_tickets_title_done = false;
	std::vector<std::string> splitted;
	uint32_t ticket_size;
	std::vector<uint32_t> line;

	limits_.clear();
	my_ticket_.clear();
	nearby_tickets_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!limits_done) {
			if (lines[i].empty()) {
				limits_done = true;
			} else {
				if (std::regex_match(lines[i], sm, LIMITS)) {
					limits_str limits = {sm.str(1), std::stoul(sm.str(2)), std::stoul(sm.str(3)), std::stoul(sm.str(4)), std::stoul(sm.str(5))};
					limits_.push_back(limits);
				} else {
					std::cout << "Invalid limits input at line " << i + 1 << std::endl;
					return false;
				}
			}
		} else if (!my_ticket_done) {
			if (!my_ticket_title_done) {
				if (lines[i] == "your ticket:") {
					my_ticket_title_done = true;
				} else {
					std::cout << "'your ticket:' title not found at " << i + 1;
					return false;
				}
			} else {
				if (!my_ticket_.size()) {
					splitted = split(lines[i], ",");
					ticket_size = splitted.size();
					for (size_t j = 0; j < ticket_size; j++) {
						my_ticket_.push_back(std::stoi(splitted[j]));
					}
				} else if (lines[i].empty()) {
					my_ticket_done = true;
				} else {
					std::cout << "Invalid 'Your ticket' input at line " << i + 1 << std::endl;
					return false;
				}
			}
		} else {
			if (!nearby_tickets_title_done) {
				if (lines[i] == "nearby tickets:") {
					nearby_tickets_title_done = true;
				} else {
					std::cout << "'nearby tickets:' title not found at " << i + 1;
					return false;
				}
			} else {
				line.clear();
				splitted = split(lines[i], ",");
				if (ticket_size != splitted.size()) {
					std::cout << "Invalid ticket size at line " << i + 1 << std::endl;
					return false;
				}
				for (size_t j = 0; j < ticket_size; j++) {
					line.push_back(std::stoi(splitted[j]));
				}
				nearby_tickets_.push_back(line);
			}
		}
	}
	return true;
}

uint32_t AoC2020_day16::scan_tickets() {
	uint32_t result = 0;
	bool found;

	for (size_t i = 0; i < nearby_tickets_.size(); i++) {
		for (size_t j = 0; j < nearby_tickets_[i].size(); j++) {
			found = false;
			for (size_t k = 0; k < limits_.size(); k++) {
				if (limits_[k].fits(nearby_tickets_[i][j])) {
					found = true;
					break;
				}
			}

			if (!found) {
				result += nearby_tickets_[i][j];
			}
		}
	}

	return result;
}

void AoC2020_day16::tests() {
	uint32_t result;

	if (init({"class: 1-3 or 5-7", "row: 6-11 or 33-44", "seat: 13-40 or 45-50", "", "your ticket:", "7,1,14", "", "nearby tickets:", "7,3,47", "40,4,50",
			  "55,2,20", "38,6,12"})) {
		result = scan_tickets(); // 75
	}
}

bool AoC2020_day16::part1() {

	result1_ = std::to_string(scan_tickets());

	return true;
}

bool AoC2020_day16::part2() {

	// result2_ = std::to_string();

	return true;
}

int32_t AoC2020_day16::get_aoc_day() {
	return 16;
}

int32_t AoC2020_day16::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day16 day16;

	return day16.main_execution();
}
