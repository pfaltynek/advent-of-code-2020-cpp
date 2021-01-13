#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>
#include <set>
#include <string>

const std::regex LIMITS("^(.*): (\\d+)-(\\d+) or (\\d+)-(\\d+)$");
const std::string DEPARTURE = "departure";

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
	uint64_t get_departures_value();
	std::vector<limits_str> limits_;
	std::vector<uint32_t> my_ticket_;
	std::vector<std::vector<uint32_t>> nearby_tickets_, valid_tickets_;
	uint32_t ticket_size_;
};

bool AoC2020_day16::init(const std::vector<std::string> lines) {
	std::smatch sm;
	bool limits_done = false, my_ticket_done = false, my_ticket_title_done = false, nearby_tickets_title_done = false;
	std::vector<std::string> splitted;
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
					ticket_size_ = splitted.size();
					for (size_t j = 0; j < ticket_size_; j++) {
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
				if (ticket_size_ != splitted.size()) {
					std::cout << "Invalid ticket size at line " << i + 1 << std::endl;
					return false;
				}
				for (size_t j = 0; j < ticket_size_; j++) {
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
	bool found, ticket_valid;

	valid_tickets_.clear();

	for (size_t i = 0; i < nearby_tickets_.size(); i++) {
		ticket_valid = true;

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
				ticket_valid = false;
			}
		}

		if (ticket_valid) {
			valid_tickets_.push_back(nearby_tickets_[i]);
		}
	}

	return result;
}

uint64_t AoC2020_day16::get_departures_value() {
	uint64_t result = 0;
	std::set<uint32_t> valid_limits = {};
	std::map<uint32_t, std::set<uint32_t>> all_valid_limits = {};
	std::vector<std::pair<uint32_t, uint32_t>> final = {}, now_done, departures = {};

	for (size_t i = 0; i < ticket_size_; i++) {
		for (size_t j = 0; j < valid_tickets_.size(); j++) {

			for (size_t k = 0; k < limits_.size(); k++) {
				if (!j) {
					if (limits_[k].fits(valid_tickets_[j][i])) {
						valid_limits.emplace(k);
					}
				} else {
					if (!limits_[k].fits(valid_tickets_[j][i])) {
						valid_limits.erase(k);
					}
				}
			}
		}

		all_valid_limits[i] = valid_limits;
		valid_limits.clear();
	}

	while (all_valid_limits.size()) {
		now_done.clear();

		for (auto it = all_valid_limits.begin(); it != all_valid_limits.end(); it++) {
			if (it->second.size() == 1) {
				now_done.push_back({it->first, *(it->second.begin())});
			}
		}

		for (size_t i = 0; i < now_done.size(); i++) {
			all_valid_limits.erase(now_done[i].first);
		}

		for (auto it = all_valid_limits.begin(); it != all_valid_limits.end(); it++) {
			for (size_t i = 0; i < now_done.size(); i++) {
				it->second.erase(now_done[i].second);
			}
		}

		final.insert(final.end(), now_done.begin(), now_done.end());
	}

	for (size_t i = 0; i < final.size(); i++) {
		if (limits_[final[i].second].name.find(DEPARTURE) == 0) {
			departures.push_back(final[i]);
		}
	}

	for (size_t i = 0; i < departures.size(); i++) {
		if (result) {
			result *= my_ticket_[departures[i].first];
		} else {
			result = my_ticket_[departures[i].first];
		}
	}

	return result;
}

void AoC2020_day16::tests() {
	uint32_t result;

	if (init({"class: 1-3 or 5-7", "row: 6-11 or 33-44", "seat: 13-40 or 45-50", "", "your ticket:", "7,1,14", "", "nearby tickets:", "7,3,47", "40,4,50",
			  "55,2,20", "38,6,12"})) {
		result = scan_tickets();		 // 75
		result = get_departures_value(); // 0
	}

	if (init({"class: 0-1 or 4-19", "row: 0-5 or 8-19", "seat: 0-13 or 16-19", "", "your ticket:", "11,12,13", "", "nearby tickets:", "3,9,18", "15,1,5",
			  "5,14,9"})) {
		valid_tickets_ = nearby_tickets_;
		result = get_departures_value(); // 0
	}
}

bool AoC2020_day16::part1() {

	result1_ = std::to_string(scan_tickets());

	return true;
}

bool AoC2020_day16::part2() {

	result2_ = std::to_string(get_departures_value()); // 2766848827 too low

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
