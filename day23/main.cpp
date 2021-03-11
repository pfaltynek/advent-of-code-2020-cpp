#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <string>

const uint32_t C_PART2_CUPS_COUNT = 1000000;

typedef struct CUP {
	uint32_t value;
	CUP* pnext;
} cup_str;

class AoC2020_day23 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint8_t> cups_;
	uint64_t perform_crab_moves(const bool part2, const uint32_t moves_count);
	void build_cups_map(const uint32_t max = 0);
	cup_str* current_;
	std::map<uint32_t, cup_str> cups_map_;
	uint32_t min_, max_;
};

bool AoC2020_day23::init(const std::vector<std::string> lines) {

	cups_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines[i].size(); j++) {
			if (std::isdigit(lines[i][j])) {
				cups_.push_back(lines[i][j] - '0');
			} else {
				std::cout << "Invalid cup number at line " << i + 1 << ", position " << j + 1 << std::endl;
				return false;
			}
		}
	}

	min_ = *std::min_element(cups_.begin(), cups_.end());
	max_ = *std::max_element(cups_.begin(), cups_.end());

	return true;
}

void AoC2020_day23::build_cups_map(const uint32_t max) {
	cup_str cup;
	cup_str *pcup, *pstart;

	cups_map_.clear();

	cup.value = cups_[0];
	cups_map_[cups_[0]] = cup;
	pcup = pstart = &cups_map_[cups_[0]];

	for (size_t i = 1; i < cups_.size(); i++) {
		cup.value = cups_[i];
		cups_map_[cups_[i]] = cup;
		pcup->pnext = &cups_map_[cups_[i]];
		pcup = &cups_map_[cups_[i]];
	}

	for (size_t i = max_ + 1; i <= max; i++) {
		cup.value = i;
		cups_map_[i] = cup;
		pcup->pnext = &cups_map_[i];
		pcup = &cups_map_[i];
		max_ = i;
	}

	pcup->pnext = pstart;
	current_ = pstart;
}

uint64_t AoC2020_day23::perform_crab_moves(const bool part2, const uint32_t moves_count) {
	cup_str *first, *last;
	uint32_t next_val;
	uint64_t result;
	std::set<uint32_t> moving;

	if (part2) {
		build_cups_map(C_PART2_CUPS_COUNT);
	} else {
		build_cups_map();
	}

	for (size_t i = 0; i < moves_count; i++) {
		first = current_->pnext;
		last = first->pnext->pnext;

		moving.clear();
		moving.emplace(first->value);
		moving.emplace(first->pnext->value);
		moving.emplace(first->pnext->pnext->value);

		next_val = current_->value;

		do {
			next_val--;

			if (next_val < min_) {
				next_val = max_;
			}
		} while (moving.count(next_val));

		current_->pnext = last->pnext;
		last->pnext = cups_map_[next_val].pnext;
		cups_map_[next_val].pnext = first;

		current_ = current_->pnext;
	}

	first = cups_map_[1].pnext;

	if (part2) {
		result = first->value;
		result *= first->pnext->value;
	} else {
		result = 0;

		while (first->value != 1) {
			result = (result * 10) + first->value;
			first = first->pnext;
		}
	}

	return result;
}

void AoC2020_day23::tests() {
	uint64_t result;

	if (init({"389125467"})) {
		result = perform_crab_moves(false, 10);	 // 92658374
		result = perform_crab_moves(false, 100); // 67384529

		result = perform_crab_moves(true, 10000000); // 149245887792
	}
}

bool AoC2020_day23::part1() {

	result1_ = std::to_string(perform_crab_moves(false, 100));

	return true;
}

bool AoC2020_day23::part2() {

	result2_ = std::to_string(perform_crab_moves(true, 10000000));

	return true;
}

int32_t AoC2020_day23::get_aoc_day() {
	return 23;
}

int32_t AoC2020_day23::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day23 day23;

	return day23.main_execution();
}
