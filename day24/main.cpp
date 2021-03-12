#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <queue>
#include <set>
#include <string>

const coord_str coord_hex_step_west = {-1, 0};
const coord_str coord_hex_step_east = {1, 0};
const coord_str coord_hex_step_north_east = {1, -1};
const coord_str coord_hex_step_south_east = {0, 1};
const coord_str coord_hex_step_north_west = {0, -1};
const coord_str coord_hex_step_south_west = {-1, 1};

class AoC2020_day24 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::vector<coord_str>> tiles_paths_;
	std::set<coord_str> black_tiles_;
	uint64_t get_black_tiles_count();
	uint64_t get_black_tiles_count_after_days(const uint8_t days);
	std::vector<coord_str> get_hexa_neighbors();
	coord_str get_final_tile(const std::vector<coord_str> path);
	std::vector<coord_str> neighbors_;
	uint8_t get_neighbors_black_count(coord_str base);
};

bool AoC2020_day24::init(const std::vector<std::string> lines) {
	bool half = false, north = false;
	std::vector<coord_str> path;

	tiles_paths_.clear();

	for (size_t i = 0; i < lines.size(); i++) {

		path.clear();

		for (size_t j = 0; j < lines[i].size(); j++) {
			if (half) {
				switch (lines[i][j]) {
					case 'e':
						if (north) {
							path.push_back(coord_hex_step_north_east);
						} else {
							path.push_back(coord_hex_step_south_east);
						}
						half = false;
						break;
					case 'w':
						if (north) {
							path.push_back(coord_hex_step_north_west);
						} else {
							path.push_back(coord_hex_step_south_west);
						}
						half = false;
						break;
					default:
						std::cout << "Invalid direction at line " << i + 1 << ", position " << j + 1 << std::endl;
						return false;
				}
			} else {
				switch (lines[i][j]) {
					case 'e':
						path.push_back(coord_hex_step_east);
						break;
					case 'w':
						path.push_back(coord_hex_step_west);
						break;
					case 's':
						half = true;
						north = false;
						break;
					case 'n':
						half = true;
						north = true;
						break;
					default:
						std::cout << "Invalid direction at line " << i + 1 << ", position " << j + 1 << std::endl;
						return false;
				}
			}
		}

		if (half) {
			std::cout << "Incomplete direction at end of line " << i + 1 << std::endl;
			return false;
		}

		tiles_paths_.push_back(path);
	}
	neighbors_ = get_hexa_neighbors();

	return true;
}

coord_str AoC2020_day24::get_final_tile(const std::vector<coord_str> path) {
	coord_str result = {0, 0};

	for (size_t i = 0; i < path.size(); i++) {
		result = result + path[i];
	}

	return result;
}

uint64_t AoC2020_day24::get_black_tiles_count() {
	coord_str tile;

	black_tiles_.clear();

	for (size_t i = 0; i < tiles_paths_.size(); i++) {
		tile = get_final_tile(tiles_paths_[i]);

		if (black_tiles_.count(tile)) {
			black_tiles_.erase(tile);
		} else {
			black_tiles_.emplace(tile);
		}
	}

	return black_tiles_.size();
}

std::vector<coord_str> AoC2020_day24::get_hexa_neighbors() {
	std::vector<coord_str> result = {};

	result.push_back(coord_hex_step_east);
	result.push_back(coord_hex_step_west);
	result.push_back(coord_hex_step_north_east);
	result.push_back(coord_hex_step_north_west);
	result.push_back(coord_hex_step_south_east);
	result.push_back(coord_hex_step_south_west);

	return result;
}

uint8_t AoC2020_day24::get_neighbors_black_count(coord_str base) {
	uint8_t result = 0;

	for (size_t i = 0; i < neighbors_.size(); i++) {
		if (black_tiles_.count(base + neighbors_[i])) {
			result++;
		}
	}

	return result;
}

uint64_t AoC2020_day24::get_black_tiles_count_after_days(const uint8_t days) {
	std::queue<coord_str> queue = {};
	std::set<coord_str> next = {};
	coord_str coord;
	uint8_t around_count;
	uint64_t result;

	for (size_t i = 0; i < days; i++) {
		next.clear();
		
		for (auto it = black_tiles_.begin(); it != black_tiles_.end(); it++) {
			coord = *it;
			queue.push(coord);
			for (size_t j = 0; j < neighbors_.size(); j++) {
				queue.push(coord + neighbors_[j]);
			}
		}

		while (queue.size()) {
			coord = queue.front();
			queue.pop();
			around_count = get_neighbors_black_count(coord);

			if (black_tiles_.count(coord)) {
				if ((around_count == 1) || (around_count == 2)) {
					next.emplace(coord);
				}
			} else {
				if (around_count == 2) {
					next.emplace(coord);
				}
			}
		}

		black_tiles_.swap(next);
		result = black_tiles_.size();
	}

	return result;
}

void AoC2020_day24::tests() {
	uint64_t result;
	coord_str tile;

	if (init({"esenee"})) {
		tile = get_final_tile(tiles_paths_[0]); // {3, 0}
	}

	if (init({"esew"})) {
		tile = get_final_tile(tiles_paths_[0]); // {0, 1}
	}

	if (init({"nwwswee"})) {
		tile = get_final_tile(tiles_paths_[0]); // {0, 0}
	}

	if (init({"sesenwnenenewseeswwswswwnenewsewsw",
			  "neeenesenwnwwswnenewnwwsewnenwseswesw",
			  "seswneswswsenwwnwse",
			  "nwnwneseeswswnenewneswwnewseswneseene",
			  "swweswneswnenwsewnwneneseenw",
			  "eesenwseswswnenwswnwnwsewwnwsene",
			  "sewnenenenesenwsewnenwwwse",
			  "wenwwweseeeweswwwnwwe",
			  "wsweesenenewnwwnwsenewsenwwsesesenwne",
			  "neeswseenwwswnwswswnw",
			  "nenwswwsewswnenenewsenwsenwnesesenew",
			  "enewnwewneswsewnwswenweswnenwsenwsw",
			  "sweneswneswneneenwnewenewwneswswnese",
			  "swwesenesewenwneswnwwneseswwne",
			  "enesenwswwswneneswsenwnewswseenwsese",
			  "wnwnesenesenenwwnenwsewesewsesesew",
			  "nenewswnwewswnenesenwnesewesw",
			  "eneswnwswnwsenenwnwnwwseeswneewsenese",
			  "neswnwewnwnwseenwseesewsenwsweewe",
			  "wseweeenwnesenwwwswnew"})) {
		result = get_black_tiles_count(); // 10
		result = get_black_tiles_count_after_days(100);
	}
}

bool AoC2020_day24::part1() {

	result1_ = std::to_string(get_black_tiles_count());

	return true;
}

bool AoC2020_day24::part2() {

	result2_ = std::to_string(get_black_tiles_count_after_days(100));

	return true;
}

int32_t AoC2020_day24::get_aoc_day() {
	return 24;
}

int32_t AoC2020_day24::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day24 day24;

	return day24.main_execution();
}
