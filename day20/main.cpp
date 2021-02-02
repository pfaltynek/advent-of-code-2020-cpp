#include "./../common/aoc.hpp"
#include <map>
#include <regex>
#include <string>

const std::regex HEADER("^Tile (\\d+):$");
const std::regex MAP_LINE("^([#.]{10})$");
const uint8_t BORDERS_COUNT = 8;

typedef struct CommonBorderInfo {
	uint16_t from_tile_id, to_tile_id;
	uint8_t from_border_idx, to_border_idx;
} common_border_info_str;

typedef struct MapTileInfo {
	uint16_t id;
	// uint16_t content[10];
	uint16_t borders[8];

	uint16_t mirror_value(const uint16_t value, const uint16_t bit_count = 10) {
		uint16_t result = 0, source = value;

		for (size_t i = 0; i < bit_count; i++) {
			result = result << 1;
			result |= (source & 0x0001);
			source = source >> 1;
		}

		return result;
	}

	uint16_t decode_border(const std::string border) {
		uint16_t result = 0;

		for (size_t i = 0; i < border.size(); i++) {
			result = result << 1;
			if (border[i] == '#') {
				result |= 1;
			}
		}

		return result;
	}

	MapTileInfo(uint16_t id, std::vector<std::string> content) {
		std::string left = "", right = "";
		this->id = id;

		for (size_t i = 0; i < content.size(); i++) {
			right += content[i][9];
			left = content[i][0] + left;
		}

		borders[0] = decode_border(content[0]);
		borders[1] = decode_border(right);
		borders[2] = mirror_value(decode_border(content[9]));
		borders[3] = decode_border(left);
		borders[4] = mirror_value(decode_border(content[0]));
		borders[5] = mirror_value(decode_border(right));
		borders[6] = decode_border(content[9]);
		borders[7] = mirror_value(decode_border(left));
	};
} map_tile_info_str;

class AoC2020_day20 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<uint16_t, std::vector<common_border_info_str>> connections_;
	std::vector<map_tile_info_str> tiles_;
	uint64_t get_corner_ids_multiplication();

	void find_all_connections();
};

bool AoC2020_day20::init(const std::vector<std::string> lines) {
	std::smatch sm;
	bool id_done = false;
	uint16_t id;
	std::vector<std::string> tile_content, input(lines);

	tiles_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].empty()) {
			continue;
		}

		if (!id_done) {
			if (std::regex_match(lines[i], sm, HEADER)) {
				id = static_cast<uint16_t>(std::stoi(sm.str(1)));
				id_done = true;
				tile_content.clear();
			} else {
				std::cout << "Invalid map tile format at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			if (std::regex_match(lines[i], sm, MAP_LINE)) {
				tile_content.push_back(sm.str(1));

				if (tile_content.size() == 10) {
					tiles_.push_back(map_tile_info_str(id, tile_content));
					id_done = false;
					tile_content.clear();
				}
			} else {
				std::cout << "Invalid map tile format at line " << i + 1 << std::endl;
				return false;
			}
		}
	}

	if (tile_content.empty() && !id_done) {
		return true;
	} else {
		std::cout << "Incomplete last tile definition" << std::endl;
		return false;
	}
}

void AoC2020_day20::find_all_connections() {
	common_border_info_str cbi1, cbi2;

	connections_.clear();

	for (size_t i = 0; i < tiles_.size(); i++) {
		for (size_t j = i + 1; j < tiles_.size(); j++) {
			for (size_t a = 0; a < BORDERS_COUNT; a++) {
				for (size_t b = 0; b < BORDERS_COUNT; b++) {
					if (tiles_[i].borders[a] == tiles_[j].borders[b]) {
						cbi1.from_tile_id = tiles_[i].id;
						cbi1.to_tile_id = tiles_[j].id;
						cbi1.from_border_idx = a;
						cbi1.to_border_idx = b;

						cbi2.from_tile_id = tiles_[j].id;
						cbi2.to_tile_id = tiles_[i].id;
						cbi2.from_border_idx = b;
						cbi2.to_border_idx = a;

						connections_[tiles_[i].id].push_back(cbi1);
						connections_[tiles_[j].id].push_back(cbi2);
					}
				}
			}
		}
	}
}

uint64_t AoC2020_day20::get_corner_ids_multiplication() {
	uint64_t result = 1;
	uint8_t cnt = 0;

	find_all_connections();

	for (auto it = connections_.begin(); it != connections_.end(); it++) {
		if (it->second.size() == 4) {
			cnt++;
			result *= static_cast<uint64_t>(it->first);
		}
	}

	if (cnt != 4) {
		return 0;
	} else {
		return result;
	}
}

void AoC2020_day20::tests() {
	uint64_t result;

	if (init({"Tile 2311:", "..##.#..#.", "##..#.....", "#...##..#.", "####.#...#", "##.##.###.", "##...#.###", ".#.#.#..##", "..#....#..",
			  "###...#.#.", "..###..###", "",			"Tile 1951:", "#.##...##.", "#.####...#", ".....#..##", "#...######", ".##.#....#",
			  ".###.#####", "###.##.##.", ".###....#.", "..#.#..#.#", "#...##.#..", "",			  "Tile 1171:", "####...##.", "#..##.#..#",
			  "##.#..#.#.", ".###.####.", "..###.####", ".##....##.", ".#...####.", "#.##.####.", "####..#...", ".....##...", "",
			  "Tile 1427:", "###.##.#..", ".#..#.##..", ".#.##.#..#", "#.#.#.##.#", "....#...##", "...##..##.", "...#.#####", ".#.####.#.",
			  "..#..###.#", "..##.#..#.", "",			"Tile 1489:", "##.#.#....", "..##...#..", ".##..##...", "..#...#...", "#####...#.",
			  "#..#.#.#.#", "...#.#.#..", "##.#...##.", "..##.##.##", "###.##.#..", "",			  "Tile 2473:", "#....####.", "#..#.##...",
			  "#.##..#...", "######.#.#", ".#...#.#.#", ".#########", ".###.#..#.", "########.#", "##...##.#.", "..###.#.#.", "",
			  "Tile 2971:", "..#.#....#", "#...###...", "#.#.###...", "##.##..#..", ".#####..##", ".#..####.#", "#..#.#..#.", "..####.###",
			  "..#.#.###.", "...#.#.#.#", "",			"Tile 2729:", "...#.#.#.#", "####.#....", "..#.#.....", "....#..#.#", ".##..##.#.",
			  ".#.####...", "####.#.#..", "##.####...", "##..#.##..", "#.##...##.", "",			  "Tile 3079:", "#.#.#####.", ".#..######",
			  "..#.......", "######....", "####.#..#.", ".#...#.##.", "#.#####.##", "..#.###...", "..#.......", "..#.###...", ""})) {
	}

	result = get_corner_ids_multiplication(); // 20899048083289
}

bool AoC2020_day20::part1() {

	result1_ = std::to_string(get_corner_ids_multiplication());

	return true;
}

bool AoC2020_day20::part2() {

	result2_ = std::to_string(0);

	return true;
}

int32_t AoC2020_day20::get_aoc_day() {
	return 20;
}

int32_t AoC2020_day20::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day20 day20;

	return day20.main_execution();
}
