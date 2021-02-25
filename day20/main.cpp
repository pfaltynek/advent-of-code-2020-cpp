#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <algorithm>
#include <map>
#include <regex>
#include <set>
#include <string>

#define DEBUG_PRINTS 0

const std::regex HEADER("^Tile (\\d+):$");
const std::regex MAP_LINE("^([#.]{10})$");
const uint8_t BORDERS_COUNT = 8;
const uint8_t UP = 0x01;
const uint8_t RI = 0x02;
const uint8_t DW = 0x04;
const uint8_t LE = 0x08;
const std::vector<std::string> SEA_MONSTER = {"                  # ", "#    ##    ##    ###", " #  #  #  #  #  #   "};

typedef struct SeaMonsterInfo {
	uint16_t w, h;
	std::set<coord_str> points;

	void print() {
#if DEBUG_PRINTS
		std::cout << std::endl;

		for (size_t i = 0; i < h; i++) {
			for (size_t j = 0; j < w; j++) {
				if (points.count({static_cast<int32_t>(j), static_cast<int32_t>(i)})) {
					std::cout << "X";
				} else {
					std::cout << ".";
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
#endif
	}
} sea_monster_info_str;
typedef struct MapTileInfo {
	uint16_t id;
	uint8_t content[8];
	uint16_t borders[8];
	uint16_t connection[4];

	uint16_t mirror_value(const uint16_t value, const uint16_t bit_count = 10) {
		uint16_t result = 0, source = value;

		for (size_t i = 0; i < bit_count; i++) {
			result = result << 1;
			result |= (source & 0x0001);
			source = source >> 1;
		}

		return result;
	}

	uint8_t decode_content(const std::string border) {
		uint16_t result = decode_border(border);

		return result & 0xFF;
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

	uint16_t get_connections_count() {
		uint16_t result = 0;

		for (size_t i = 0; i < 4; i++) {
			if (connection[i]) {
				result++;
			}
		}

		return result;
	}

	bool is_good_oriented(const uint8_t conn_packed, const uint16_t conn_id[4]) {
		if (conn_packed & UP) {
			if (!connection[0]) {
				return false;
			} else if (conn_id[0]) {
				if (conn_id[0] != connection[0]) {
					return false;
				}
			}
		} else {
			if (connection[0]) {
				return false;
			}
		}
		if (conn_packed & RI) {
			if (!connection[1]) {
				return false;
			} else if (conn_id[1]) {
				if (conn_id[1] != connection[1]) {
					return false;
				}
			}
		} else {
			if (connection[1]) {
				return false;
			}
		}
		if (conn_packed & DW) {
			if (!connection[2]) {
				return false;
			} else if (conn_id[2]) {
				if (conn_id[2] != connection[2]) {
					return false;
				}
			}
		} else {
			if (connection[2]) {
				return false;
			}
		}
		if (conn_packed & LE) {
			if (!connection[3]) {
				return false;
			} else if (conn_id[3]) {
				if (conn_id[3] != connection[3]) {
					return false;
				}
			}
		} else {
			if (connection[3]) {
				return false;
			}
		}

		return true;
	}

	void flip_horz() {
		for (size_t i = 0; i < 4; i++) {
			std::swap(content[i], content[7 - i]);
		}

		std::swap(borders[0], borders[2]);
		std::swap(borders[4], borders[6]);
		std::swap(borders[1], borders[5]);
		std::swap(borders[3], borders[7]);

		std::swap(connection[0], connection[2]);
	}

	void flip_vert() {
		for (size_t i = 0; i < 8; i++) {
			content[i] = mirror_value(content[i], 8);
		}

		std::swap(borders[0], borders[4]);
		std::swap(borders[2], borders[6]);
		std::swap(borders[1], borders[5]);
		std::swap(borders[3], borders[7]);

		std::swap(connection[1], connection[3]);
	}

	void rotate_clockwise() {
		uint16_t tmp;
		uint8_t tmp_content[8];

		tmp = borders[3];
		borders[3] = borders[2];
		borders[2] = borders[1];
		borders[1] = borders[0];
		borders[0] = tmp;
		tmp = borders[7];
		borders[7] = borders[6];
		borders[6] = borders[5];
		borders[5] = borders[4];
		borders[4] = tmp;

		tmp = connection[3];
		connection[3] = connection[2];
		connection[2] = connection[1];
		connection[1] = connection[0];
		connection[0] = tmp;

		for (size_t i = 0; i < 8; i++) {
			tmp_content[i] = content[i];
			content[i] = 0;
		}

		for (size_t i = 0; i < 8; i++) {
			for (size_t j = 0; j < 8; j++) {
				content[i] = content[i] >> 1;
				content[i] |= tmp_content[j] & 0x80;
				tmp_content[j] = tmp_content[j] << 1;
			}
		}
	}

	void rotate_counterclockwise() {
		uint16_t tmp;
		uint8_t tmp_content[8];

		tmp = borders[0];
		borders[0] = borders[1];
		borders[1] = borders[2];
		borders[2] = borders[3];
		borders[3] = tmp;
		tmp = borders[4];
		borders[4] = borders[5];
		borders[5] = borders[6];
		borders[6] = borders[7];
		borders[7] = tmp;

		tmp = connection[0];
		connection[0] = connection[1];
		connection[1] = connection[2];
		connection[2] = connection[3];
		connection[3] = tmp;

		for (size_t i = 0; i < 8; i++) {
			tmp_content[i] = content[i];
			content[i] = 0;
		}

		for (size_t i = 0; i < 8; i++) {
			for (size_t j = 0; j < 8; j++) {
				content[i] = content[i] << 1;
				content[i] |= tmp_content[j] & 0x01;
				tmp_content[j] = tmp_content[j] >> 1;
			}
		}
	}

	MapTileInfo(uint16_t id, std::vector<std::string> content) {
		std::string left = "", right = "";
		this->id = id;

		for (size_t i = 0; i < 4; i++) {
			connection[i] = 0;
		}

		for (size_t i = 0; i < content.size(); i++) {
			right += content[i][9];
			left = content[i][0] + left;
			if (i && (i < 9)) {
				this->content[i - 1] = decode_content(content[i].substr(1, 8));
			}
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

	std::vector<std::string> get_content_as_text() {
		std::vector<std::string> result = {};
		std::string line;
		uint8_t mask;

		for (size_t i = 0; i < 8; i++) {
			mask = 0x80;
			line.clear();

			for (size_t j = 0; j < 8; j++) {
				if (content[i] & mask) {
					line += '#';
				} else {
					line += '.';
				}
				mask = mask >> 1;
			}
			result.push_back(line);
		}

		return result;
	}

	void print(const bool print_id_header) {
#if DEBUG_PRINTS
		std::vector<std::string> txt = get_content_as_text();

		if (print_id_header) {
			std::cout << "ID: " << id << std::endl;
		}

		for (size_t i = 0; i < txt.size(); i++) {
			std::cout << txt[i] << std::endl;
		}

		std::cout << std::endl;
#endif
	}
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
	std::vector<map_tile_info_str> tiles_;
	std::map<uint16_t, size_t> tiles_map_;
	std::map<coord_str, uint16_t> map_;
	uint64_t get_corner_ids_multiplication();
	uint64_t get_water_count_around_sea_monsters();
	uint16_t map_max_x_, map_max_y_;
	std::vector<sea_monster_info_str> sea_monsters_;
	std::map<coord_str, char> full_map_;

	void find_all_connections();
	void fill_map();
	void fill_full_map();
	void prepare_sea_monsters();
	void tile_position(const uint16_t id, const uint8_t conn_packed, const uint16_t conn_id[4]);
};

bool AoC2020_day20::init(const std::vector<std::string> lines) {
	std::smatch sm;
	bool id_done = false;
	uint16_t id;
	std::vector<std::string> tile_content, input(lines);

	tiles_.clear();
	tiles_map_.clear();

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
					tiles_map_[id] = tiles_.size();
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
	for (size_t i = 0; i < tiles_.size(); i++) {
		for (size_t j = i + 1; j < tiles_.size(); j++) {
			for (size_t a = 0; a < BORDERS_COUNT; a++) {
				for (size_t b = 0; b < BORDERS_COUNT; b++) {
					if (tiles_[i].borders[a] == tiles_[j].borders[b]) {
						if (!tiles_[i].connection[a % 4]) {
							tiles_[i].connection[a % 4] = tiles_[j].id;
						}
						if (!tiles_[j].connection[b % 4]) {
							tiles_[j].connection[b % 4] = tiles_[i].id;
						}
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

	for (size_t i = 0; i < tiles_.size(); i++) {
		if (tiles_[i].get_connections_count() == 2) {
			cnt++;
			result *= static_cast<uint64_t>(tiles_[i].id);
		}
	}

	if (cnt != 4) {
		return 0;
	} else {
		return result;
	}
}

void AoC2020_day20::tile_position(const uint16_t id, const uint8_t conn_packed, const uint16_t conn_id[4]) {
	size_t idx = tiles_map_[id];

	for (size_t i = 0; i < 4; i++) {

		if (tiles_[idx].is_good_oriented(conn_packed, conn_id)) {
			return;
		}

		tiles_[idx].flip_vert();

		if (tiles_[idx].is_good_oriented(conn_packed, conn_id)) {
			return;
		}

		tiles_[idx].flip_horz();

		if (tiles_[idx].is_good_oriented(conn_packed, conn_id)) {
			return;
		}

		tiles_[idx].flip_vert();

		if (tiles_[idx].is_good_oriented(conn_packed, conn_id)) {
			return;
		}

		tiles_[idx].flip_horz();

		if (tiles_[idx].is_good_oriented(conn_packed, conn_id)) {
			return;
		}

		tiles_[idx].rotate_clockwise();
	}

	int z = 0;
}

void AoC2020_day20::fill_map() {
	uint16_t id, idx, start_idx;
	coord_str pos = {0, 0};
	uint8_t conn_packed = 0;
	uint16_t conn_id[4];

	map_.clear();

	// find first possible corner
	for (size_t i = 0; i < tiles_.size(); i++) {
		if (tiles_[i].get_connections_count() == 2) {
			idx = i;
			id = tiles_[i].id;
			break;
		}
	}

	for (size_t i = 0; i < 4; i++) {
		conn_id[i] = 0;
	}

	// place it right
	tile_position(id, RI | DW, conn_id);
	tiles_[idx].print(true);
	map_[pos] = id;
	start_idx = idx;

	// place right all tiles of first line
	while (tiles_[idx].connection[1]) {
		id = tiles_[idx].connection[1];
		idx = tiles_map_[id];
		pos = pos + coord_step_east;

		for (size_t i = 0; i < 4; i++) {
			conn_id[i] = 0;
		}

		conn_packed = 0;
		switch (tiles_[idx].get_connections_count()) {
			case 2:
				conn_packed = LE | DW;
				conn_id[3] = map_[pos + coord_step_west];
				break;
			case 3:
				conn_packed = LE | DW | RI;
				conn_id[3] = map_[pos + coord_step_west];
				break;
			default:
				return;
		}

		tile_position(id, conn_packed, conn_id);
		tiles_[idx].print(true);
		map_[pos] = id;
	}

	map_max_x_ = pos.x;

	// place right all other lines below if exist
	while (tiles_[start_idx].connection[2]) {
		id = tiles_[start_idx].connection[2];
		idx = start_idx = tiles_map_[id];
		pos = {0, pos.y + 1};

		while (true) {
			for (size_t i = 0; i < 4; i++) {
				conn_id[i] = 0;
			}
			conn_packed = 0;

			switch (tiles_[idx].get_connections_count()) {
				case 2:
					if (pos.x) {
						conn_packed = LE | UP;
						conn_id[0] = map_[pos + coord_step_north];
						conn_id[3] = map_[pos + coord_step_west];
					} else {
						conn_packed = RI | UP;
						conn_id[0] = map_[pos + coord_step_north];
					}
					break;
				case 3:
					if (pos.x == 0) {
						conn_packed = UP | RI | DW;
						conn_id[0] = map_[pos + coord_step_north];
					} else if (pos.x == map_max_x_) {
						conn_packed = UP | DW | LE;
						conn_id[0] = map_[pos + coord_step_north];
						conn_id[3] = map_[pos + coord_step_west];
					} else {
						conn_packed = UP | RI | LE;
						conn_id[0] = map_[pos + coord_step_north];
						conn_id[3] = map_[pos + coord_step_west];
					}
					break;
				case 4:
					conn_packed = LE | UP | DW | RI;
					conn_id[0] = map_[pos + coord_step_north];
					conn_id[3] = map_[pos + coord_step_west];
					break;
				default:
					return;
			}

			tile_position(id, conn_packed, conn_id);
			tiles_[idx].print(true);
			map_[pos] = id;

			if (!tiles_[idx].connection[1]) {
				break;
			}

			id = tiles_[idx].connection[1];
			idx = tiles_map_[id];
			pos = pos + coord_step_east;
		}
	}

	map_max_y_ = pos.y;
}

void AoC2020_day20::fill_full_map() {
	uint16_t id, idx;

	full_map_.clear();

	for (size_t x = 0; x <= map_max_x_; x++) {
		for (size_t y = 0; y <= map_max_y_; y++) {
			id = map_[{static_cast<int32_t>(x), static_cast<int32_t>(y)}];
			idx = tiles_map_[id];

			for (size_t i = 0; i < 8; i++) {
				for (size_t j = 0; j < 8; j++) {
					if (tiles_[idx].content[i] & (0x80 >> j)) {
						full_map_[{static_cast<int32_t>((x * 8) + j), static_cast<int32_t>((y * 8) + i)}] = '#';
#if DEBUG_PRINTS
						std::cout << x << ", " << y << "   " << ((x * 8) + j) << ", " << ((y * 8) + i) << std::endl;
#endif
					}
				}
			}
		}
	}

#if DEBUG_PRINTS
	std::cout << std::endl;
	size_t maxx, maxy;

	maxx = map_max_x_ + 1;
	maxx *= 8;
	maxy = map_max_y_ + 1;
	maxy *= 8;

	for (size_t y = 0; y < maxy; y++) {
		for (size_t x = 0; x < maxx; x++) {
			if (full_map_.count({static_cast<int32_t>(x), static_cast<int32_t>(y)})) {
				std::cout << "#";
			} else {
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
#endif
}

void AoC2020_day20::prepare_sea_monsters() {
	std::set<coord_str> sea_mon_points, next;
	sea_monster_info_str smi;
	uint16_t x, y;
	std::vector<std::string> buff;

	sea_monsters_.clear();
	sea_mon_points.clear();
	y = SEA_MONSTER.size();

	for (size_t i = 0; i < SEA_MONSTER.size(); i++) {
		if (SEA_MONSTER[i].size() > x) {
			x = SEA_MONSTER[i].size();
		}

		for (size_t j = 0; j < SEA_MONSTER[i].size(); j++) {
			if (SEA_MONSTER[i][j] == '#') {
				sea_mon_points.insert({static_cast<int32_t>(j), static_cast<int32_t>(i)});
			}
		}
	}

	smi.w = x;
	smi.h = y;
	smi.points = sea_mon_points;

#if DEBUG_PRINTS
	smi.print();
#endif

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 2; j++) {
			sea_monsters_.push_back(smi);
			// flip horizontally
			next.clear();
			for (auto it = sea_mon_points.begin(); it != sea_mon_points.end(); it++) {
				next.insert({it->x, smi.h - it->y - 1});
			}
			smi.points = next;
#if DEBUG_PRINTS
			smi.print();
#endif
			sea_monsters_.push_back(smi);
			sea_mon_points.swap(next);

			// flip vertically
			next.clear();
			for (auto it = sea_mon_points.begin(); it != sea_mon_points.end(); it++) {
				next.insert({smi.w - it->x - 1, it->y});
			}
			smi.points = next;
#if DEBUG_PRINTS
			smi.print();
#endif
			sea_monsters_.push_back(smi);
			sea_mon_points.swap(next);
		}

		next.clear();
		for (auto it = sea_mon_points.begin(); it != sea_mon_points.end(); it++) {
			next.insert({smi.h - it->y - 1, it->x});
		}
		smi.points = sea_mon_points = next;
		std::swap(smi.w, smi.h);
#if DEBUG_PRINTS
		smi.print();
#endif
	}
}

uint64_t AoC2020_day20::get_water_count_around_sea_monsters() {
	uint64_t result = 0;
	int32_t xlim, ylim, full_map_w, full_map_h;
	coord_str start, pos;
	bool monster_found_here, monster_page = false;

	fill_map();
	fill_full_map();
	prepare_sea_monsters();

	full_map_w = map_max_x_ + 1;
	full_map_w *= 8;
	full_map_h = map_max_y_ + 1;
	full_map_h *= 8;

	for (size_t s = 0; s < sea_monsters_.size(); s++) {
		xlim = full_map_w - sea_monsters_[s].w;
		ylim = full_map_h - sea_monsters_[s].h;

		for (int32_t x = 0; x < xlim; x++) {
			for (int32_t y = 0; y < ylim; y++) {
				start = {x, y};
				monster_found_here = true;

				for (auto it = sea_monsters_[s].points.begin(); it != sea_monsters_[s].points.end(); it++) {
					if (!full_map_.count(start + *it)) {
						monster_found_here = false;
						break;
					}
				}

				if (monster_found_here) {
					monster_page = true;

					for (auto it = sea_monsters_[s].points.begin(); it != sea_monsters_[s].points.end(); it++) {
						full_map_[start + *it] = 'O';
					}
				}
			}
		}

		if (monster_page) {
			for (auto it = full_map_.begin(); it != full_map_.end(); it++) {
				if (it->second == '#') {
					result++;
				}
			}

			break;
		}
	}

	return result;
}

void AoC2020_day20::tests() {
	uint64_t result;

#if DEBUG_PRINTS
	map_tile_info_str mti(
		9999, {"..#.......", "..##......", "..###.....", "..####....", "..#####...", "..######..", "..#####...", "..####....", "..###.....", "..##......"});

	mti.print();
	mti.rotate_clockwise();
	mti.print();
	mti.rotate_clockwise();
	mti.print();
	mti.rotate_clockwise();
	mti.print();
	mti.rotate_clockwise();
	mti.print();

	mti.rotate_counterclockwise();
	mti.print();
	mti.rotate_counterclockwise();
	mti.print();
	mti.rotate_counterclockwise();
	mti.print();
	mti.rotate_counterclockwise();
	mti.print();

	mti.flip_horz();
	mti.print();
	mti.flip_horz();
	mti.print();

	mti.flip_vert();
	mti.print();
	mti.flip_vert();
	mti.print();
#endif

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

	result = get_water_count_around_sea_monsters(); // 273
}

bool AoC2020_day20::part1() {

	result1_ = std::to_string(get_corner_ids_multiplication());

	// fill_map();

	return true;
}

bool AoC2020_day20::part2() {

	result2_ = std::to_string(get_water_count_around_sea_monsters());

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
