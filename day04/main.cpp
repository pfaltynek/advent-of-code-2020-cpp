#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>
#include <string>

const std::regex PASSPORT_DATA("^(byr|iyr|eyr|hgt|hcl|ecl|pid|cid):(.+)$");

class AoC2020_day04 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool decode_passport(const std::vector<std::string> source, std::map<std::string, std::string>& output);
	uint32_t get_valid_passports_count();
	std::vector<std::map<std::string, std::string>> passports_;
};

bool AoC2020_day04::init(const std::vector<std::string> lines) {
	std::vector<std::string> passport = {}, tmp;
	std::map<std::string, std::string> passport_map;

	passports_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].empty()) {
			if (decode_passport(passport, passport_map)) {
				passports_.push_back(passport_map);
				passport.clear();
			} else {
				std::cout << "Invalid passport data at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			tmp.clear();
			tmp = split(lines[i], " ");
			passport.insert(passport.end(), tmp.begin(), tmp.end());
		}
	}

	if (passport.size()) {
		if (decode_passport(passport, passport_map)) {
			passports_.push_back(passport_map);
		} else {
			std::cout << "Invalid passport data at line " << lines.size() + 1 << std::endl;
			return false;
		}
	}

	return true;
}

bool AoC2020_day04::decode_passport(const std::vector<std::string> source, std::map<std::string, std::string>& output) {
	std::smatch sm;

	output.clear();

	for (size_t i = 0; i < source.size(); i++) {
		if (std::regex_match(source[i], sm, PASSPORT_DATA)) {
			output[sm.str(1)] = sm.str(2);
		} else {
			return false;
		}
	}

	return true;
}

uint32_t AoC2020_day04::get_valid_passports_count() {
	uint32_t result = 0;

	for (size_t i = 0; i < passports_.size(); i++) {
		if (passports_[i].size() == 7) {
			if (passports_[i].count("cid") == 0) {
				result++;
			}
		} else if (passports_[i].size() == 8) {
			result++;
		}
	}

	return result;
}

void AoC2020_day04::tests() {
	uint32_t result;

	if (init({"ecl:gry pid:860033327 eyr:2020 hcl:#fffffd", "byr:1937 iyr:2017 cid:147 hgt:183cm", "", "iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884",
			  "hcl:#cfa07d byr:1929", "", "hcl:#ae17e1 iyr:2013", "eyr:2024", "ecl:brn pid:760753108 byr:1931", "hgt:179cm", "",
			  "hcl:#cfa07d eyr:2025 pid:166559648", "iyr:2011 ecl:brn hgt:59in"})) {
		result = get_valid_passports_count(); // 2
	}
}

bool AoC2020_day04::part1() {

	result1_ = std::to_string(get_valid_passports_count());

	return true;
}

bool AoC2020_day04::part2() {

	result2_ = std::to_string(get_valid_passports_count());

	return true;
}

int32_t AoC2020_day04::get_aoc_day() {
	return 2;
}

int32_t AoC2020_day04::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day04 day04;

	return day04.main_execution();
}
