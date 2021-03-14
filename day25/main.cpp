#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <queue>
#include <set>
#include <string>

const uint64_t CCARD_SUBJECT = 7;
const uint64_t CDOOR_SUBJECT = 7;
const uint64_t CDIVISOR = 20201227;

class AoC2020_day25 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t door_public_, card_public_, door_loop_size_, card_loop_size_;
	uint64_t get_encryption_key(const bool from_card_public_key);
	uint64_t find_loop_size(const uint64_t subject, const uint64_t public_key);
};

bool AoC2020_day25::init(const std::vector<std::string> lines) {
	bool door = false, card = false;
	std::vector<coord_str> path;

	for (size_t i = 0; i < lines.size(); i++) {
		if (!door) {
			door_public_ = std::stoul(lines[i]);
			door = true;
		} else if (!card) {
			card_public_ = std::stoul(lines[i]);
			card = true;
		}
	}

	return card && door;
}

uint64_t AoC2020_day25::find_loop_size(const uint64_t subject, const uint64_t public_key) {
	uint64_t value = 1, result = 0;

	while (value != public_key) {
		value *= subject;
		value %= CDIVISOR;
		result++;
	}

	return result;
}

uint64_t AoC2020_day25::get_encryption_key(const bool from_card_public_key) {
	uint64_t key, result, loop_size;

	card_loop_size_ = find_loop_size(CCARD_SUBJECT, card_public_);
	door_loop_size_ = find_loop_size(CDOOR_SUBJECT, door_public_);

	if (from_card_public_key) {
		key = card_public_;
		loop_size = door_loop_size_;
	} else {
		key = door_public_;
		loop_size = card_loop_size_;
	}

	result = 1;

	for (size_t i = 0; i < loop_size; i++) {
		result *= key;
		result %= CDIVISOR;
	}

	return result;
}

void AoC2020_day25::tests() {
	uint64_t result;

	if (init({"5764801", "17807724"})) {
		result = get_encryption_key(true); // 14897079
		result = get_encryption_key(false); // 14897079
	}
}

bool AoC2020_day25::part1() {

	result1_ = std::to_string(get_encryption_key(true));

	return true;
}

bool AoC2020_day25::part2() {

	result2_ = std::to_string(0);

	return true;
}

int32_t AoC2020_day25::get_aoc_day() {
	return 25;
}

int32_t AoC2020_day25::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day25 day25;

	return day25.main_execution();
}
