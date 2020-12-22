#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <algorithm>
#include <map>
#include <string>

class AoC2020_day13 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint32_t> bus_ids_;
	uint32_t time_;
	uint32_t get_first_bus_id_and_time();
	uint64_t get_earliest_timestamp();
	uint64_t get_earliest_timestamp_bf();
};

bool AoC2020_day13::init(const std::vector<std::string> lines) {
	std::vector<std::string> buses;

	bus_ids_.clear();

	if (lines.size() != 2) {
		std::cout << "Invalid input size" << std::endl;
		return false;
	}

	time_ = std::stoi(lines[0]);

	buses = split(lines[1], ",");

	for (size_t i = 0; i < buses.size(); i++) {
		if (buses[i] == "x") {
			bus_ids_.push_back(0);
		} else {
			bus_ids_.push_back(std::stoi(buses[i]));
		}
	}

	return true;
}

uint32_t AoC2020_day13::get_first_bus_id_and_time() {
	uint32_t time = UINT32_MAX, tmp, bus_id = 0;

	for (size_t i = 0; i < bus_ids_.size(); i++) {
		if (bus_ids_[i]) {
			tmp = bus_ids_[i] - (time_ % bus_ids_[i]);

			if (tmp < time) {
				time = tmp;
				bus_id = bus_ids_[i];
			}
		}
	}

	return bus_id * time;
}

uint64_t AoC2020_day13::get_earliest_timestamp_bf() {
	uint64_t result = 0;
	std::map<uint32_t, uint32_t> bus_id_map = {};
	std::vector<uint32_t> bus_ids_ordered = {};
	bool found = false;
	uint64_t mult;

	for (size_t i = 0; i < bus_ids_.size(); i++) {
		if (bus_ids_[i]) {
			bus_id_map[bus_ids_[i]] = i;
			bus_ids_ordered.push_back(bus_ids_[i]);
		}
	}

	if (bus_ids_ordered.size() == 0) {
		return 0;
	} else if (bus_ids_ordered.size() == 1) {
		result = bus_ids_ordered[0];

		return result - bus_id_map[result];
	}

	std::sort(bus_ids_ordered.rbegin(), bus_ids_ordered.rend());

	mult = 1;
	while (!found) {
		result = (bus_ids_ordered[0] * mult) - bus_id_map[bus_ids_ordered[0]];
		found = true;
		for (size_t i = 1; i < bus_ids_ordered.size(); i++) {
			if ((result + bus_id_map[bus_ids_ordered[i]]) % bus_ids_ordered[i]) {
				found = false;
				mult++;
			}
		}
	}

	return result;
}

uint64_t AoC2020_day13::get_earliest_timestamp() {
	uint64_t result = 0;
	std::vector<std::pair<uint32_t, uint32_t>> bus_ids = {};
	uint64_t mult = 0, increment = 1;

	for (size_t i = 0; i < bus_ids_.size(); i++) {
		if (bus_ids_[i]) {
			bus_ids.push_back({bus_ids_[i], i});
		}
	}

	if (bus_ids.size() == 0) {
		return 0;
	}

	for (size_t i = 1; i < bus_ids.size(); i++) {
		while (((bus_ids[0].first * mult - bus_ids[0].second + bus_ids[i].second) % bus_ids[i].first) != 0) {
			mult += increment;
		}
		increment *= bus_ids[i].first;
	}

	result = (bus_ids[0].first * mult) - bus_ids[0].second;

	return result;
}

void AoC2020_day13::tests() {
	uint32_t result;

	if (init({"55", "17,x,13,19"})) {
		result = get_earliest_timestamp_bf(); // 3417
		result = get_earliest_timestamp();	  // 3417
	}

	if (init({"939", "7,13,x,x,59,x,31,19"})) {
		result = get_first_bus_id_and_time(); // 295
		result = get_earliest_timestamp_bf(); // 1068781
		result = get_earliest_timestamp();	  // 1068781
	}

	if (init({"939", "67,7,59,61"})) {
		result = get_earliest_timestamp_bf(); // 754018
		result = get_earliest_timestamp();	  // 754018
	}
	if (init({"939", "67,x,7,59,61"})) {
		result = get_earliest_timestamp_bf(); // 779210
		result = get_earliest_timestamp();	  // 779210
	}
	if (init({"939", "67,7,x,59,61"})) {
		result = get_earliest_timestamp_bf(); // 1261476
		result = get_earliest_timestamp();	  // 1261476
	}
	if (init({"939", "1789,37,47,1889"})) {
		result = get_earliest_timestamp_bf(); // 1202161486
		result = get_earliest_timestamp();	  // 1202161486
	}
}

bool AoC2020_day13::part1() {

	result1_ = std::to_string(get_first_bus_id_and_time());

	return true;
}

bool AoC2020_day13::part2() {

	// too slow - calculated more than 2 days
	// tried solution by Chinese Remainder theorem but did not produce valid result
	// now optimized solution inspired by Reedit discussion

	// result2_ = std::to_string(get_earliest_timestamp_bf());

	result2_ = std::to_string(get_earliest_timestamp());

	return true;
}

int32_t AoC2020_day13::get_aoc_day() {
	return 13;
}

int32_t AoC2020_day13::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day13 day13;

	return day13.main_execution();
}
