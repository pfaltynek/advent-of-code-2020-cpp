#include "./../common/aoc.hpp"
#include <set>
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
	std::vector<uint16_t> players_[2];
	uint16_t game_;
	void play_game(const bool part2, std::vector<uint16_t>& cards1, std::vector<uint16_t>& cards2);
	uint64_t get_winners_score(const bool part2);
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
				players_[player_idx].push_back(static_cast<uint16_t>(std::stoul(lines[i])));
			}
		}
	}

	return true;
}

void AoC2020_day22::play_game(const bool part2, std::vector<uint16_t>& cards1, std::vector<uint16_t>& cards2) {
	std::set<std::string> history;
	uint16_t c1, c2, round = 0;
	std::string state;
	bool winner_player1;

	game_++;

	while (!cards1.empty() && !cards2.empty()) {

		round++;

		state.clear();

		if (part2) {
			for (size_t i = 0; i < cards1.size(); i++) {
				state += std::to_string(cards1[i]) + "|";
			}

			if (history.count(state)) {
				cards1.insert(cards1.end(), cards2.begin(), cards2.end());
				cards2.clear();

				return;
			} else {
				history.insert(state);
			}
		}

		c1 = cards1.front();
		c2 = cards2.front();
		cards1.erase(cards1.begin());
		cards2.erase(cards2.begin());

		if (part2) {
			if ((c1 <= cards1.size()) && (c2 <= cards2.size())) {
				std::vector<uint16_t> sub1, sub2;

				sub1.clear();
				sub2.clear();
				sub1.insert(sub1.begin(), cards1.begin(), cards1.begin() + c1);
				sub2.insert(sub2.begin(), cards2.begin(), cards2.begin() + c2);

				play_game(part2, sub1, sub2);

				if (sub2.empty()) {
					winner_player1 = true;
				} else {
					winner_player1 = false;
				}
			} else {
				if (c1 > c2) {
					winner_player1 = true;
				} else {
					winner_player1 = false;
				}
			}
		} else {
			if (c1 > c2) {
				winner_player1 = true;
			} else {
				winner_player1 = false;
			}
		}

		if (winner_player1) {
			cards1.push_back(c1);
			cards1.push_back(c2);
		} else {
			cards2.push_back(c2);
			cards2.push_back(c1);
		}
	}
}

uint64_t AoC2020_day22::get_winners_score(const bool part2) {
	std::vector<uint16_t> p[2];
	uint16_t multiplier, winner;
	uint64_t result = 0;

	game_ = 0;

	p[0] = players_[0];
	p[1] = players_[1];

	play_game(part2, p[0], p[1]);

	if (p[0].empty()) {
		winner = 1;
	} else {
		winner = 0;
	}

	multiplier = p[winner].size();

	for (size_t i = 0; i < p[winner].size(); i++) {
		result += p[winner][i] * multiplier;
		multiplier--;
	}

	return result;
}

void AoC2020_day22::tests() {
	uint64_t result;

	if (init({"Player 1:", "9", "2", "6", "3", "1", "", "Player 2:", "5", "8", "4", "7", "10"})) {
		result = get_winners_score(false); // 306
		result = get_winners_score(true);  // 291
	}
}

bool AoC2020_day22::part1() {

	result1_ = std::to_string(get_winners_score(false));

	return true;
}

bool AoC2020_day22::part2() {

	result2_ = std::to_string(get_winners_score(true));

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
