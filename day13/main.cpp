#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
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
	uint32_t get_waypoint_route_end_manhattan_distance();
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
		if (buses[i] != "x") {
			bus_ids_.push_back(std::stoi(buses[i]));
		}
	}

	return true;
}

uint32_t AoC2020_day13::get_first_bus_id_and_time() {
	uint32_t time = UINT32_MAX, div, tmp, bus_id = 0;

	for (size_t i = 0; i < bus_ids_.size(); i++) {
		tmp = bus_ids_[i] - (time_ % bus_ids_[i]);

		if (tmp < time) {
			time = tmp;
			bus_id = bus_ids_[i];
		}
	}

	return bus_id * time;
}

void AoC2020_day13::tests() {
	uint32_t result;

	if (init({"939", "7,13,x,x,59,x,31,19"})) {
		result = get_first_bus_id_and_time(); // 295
	}
}

bool AoC2020_day13::part1() {

	result1_ = std::to_string(get_first_bus_id_and_time());

	return true;
}

bool AoC2020_day13::part2() {

	result2_ = std::to_string(get_first_bus_id_and_time());

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
