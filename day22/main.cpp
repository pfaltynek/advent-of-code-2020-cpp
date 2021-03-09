#include "./../common/aoc.hpp"
#include <queue>
#include <string>

class AoC2020_day22 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::queue<uint16_t> players_[2];
	uint64_t get_winners_score();
};

bool AoC2020_day22::init(const std::vector<std::string> lines) {
	uint8_t player_idx = 0;
	bool title_done = false;
	std::string player;

	players_[0] = {};
	players_[1] = {};

	for (size_t i = 0; i < lines.size(); i++) {
		if (!title_done) {
			player = "Player " + std::to_string(player_idx + 1) + ":";
			if (player == lines[i]) {
				title_done = true;
			} else {
				std::cout << "Invalid player header at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			if (lines[i].empty()) {
				player_idx++;
				title_done = false;
			} else {
				players_[player_idx].emplace(static_cast<uint16_t>(std::stoul(lines[i])));
			}
		}
	}

	return true;
}

uint64_t AoC2020_day22::get_winners_score() {
	std::queue<uint16_t> p[2];
	uint16_t c1, c2, round = 0, multiplier, winner;
	uint64_t result = 0;

	p[0] = players_[0];
	p[1] = players_[1];

	while (!p[0].empty() && !p[1].empty()) {
		c1 = p[0].front();
		c2 = p[1].front();
		p[0].pop();
		p[1].pop();

		if (c1 > c2) {
			p[0].emplace(c1);
			p[0].emplace(c2);
		} else {
			p[1].emplace(c2);
			p[1].emplace(c1);
		}
		round++;
	}

	if (p[0].empty()) {
		winner = 1;
	} else {
		winner = 0;
	}

	multiplier = p[winner].size();

	while (!p[winner].empty()) {
		result += p[winner].front() * multiplier;
		p[winner].pop();
		multiplier--;
	}

	return result;
}

void AoC2020_day22::tests() {
	uint64_t result;

	if (init({"Player 1:", "9", "2", "6", "3", "1", "", "Player 2:", "5", "8", "4", "7", "10"})) {
		result = get_winners_score(); // 306
	}
}

bool AoC2020_day22::part1() {

	result1_ = std::to_string(get_winners_score());

	return true;
}

bool AoC2020_day22::part2() {

	result2_ = std::to_string(0);

	return true;
}

int32_t AoC2020_day22::get_aoc_day() {
	return 22;
}

int32_t AoC2020_day22::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day22 day22;

	return day22.main_execution();
}
