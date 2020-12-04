#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>
#include <string>

const std::regex PASSPORT_DATA("^(byr|iyr|eyr|hgt|hcl|ecl|pid|cid):(.+)$");
const std::regex PASSPORT_YEAR("^(\\d{4})$");
const std::regex PASSPORT_HEIGHT_IN("^(\\d{2}in)$");
const std::regex PASSPORT_HEIGHT_CM("^(\\d{3}cm)$");
const std::regex PASSPORT_HAIR_COLOR("^#([a-f0-9]{6})$");
const std::regex PASSPORT_EYE_COLOR("^(amb|blu|brn|gry|grn|hzl|oth)$");
const std::regex PASSPORT_ID("^(\\d{9})$");

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
	uint32_t get_valid_passports_count(const bool part2);
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

uint32_t AoC2020_day04::get_valid_passports_count(const bool part2) {
	uint32_t result = 0, tmp;
	std::smatch sm;

	for (size_t i = 0; i < passports_.size(); i++) {
		if (part2) {
			if (std::regex_match(passports_[i]["byr"], sm, PASSPORT_YEAR)) {
				tmp = std::stoi(sm.str(1));
				if ((tmp > 2002) || (tmp < 1920)) {
					continue;
				}
			} else {
				continue;
			}

			if (std::regex_match(passports_[i]["iyr"], sm, PASSPORT_YEAR)) {
				tmp = std::stoi(sm.str(1));
				if ((tmp > 2020) || (tmp < 2010)) {
					continue;
				}
			} else {
				continue;
			}

			if (std::regex_match(passports_[i]["eyr"], sm, PASSPORT_YEAR)) {
				tmp = std::stoi(sm.str(1));
				if ((tmp > 2030) || (tmp < 2020)) {
					continue;
				}
			} else {
				continue;
			}

			if (std::regex_match(passports_[i]["hgt"], sm, PASSPORT_HEIGHT_CM)) {
				tmp = std::stoi(sm.str(1));
				if ((tmp > 193) || (tmp < 150)) {
					continue;
				}
			} else if (std::regex_match(passports_[i]["hgt"], sm, PASSPORT_HEIGHT_IN)) {
				tmp = std::stoi(sm.str(1));
				if ((tmp > 76) || (tmp < 59)) {
					continue;
				}
			} else {
				continue;
			}

			if (!std::regex_match(passports_[i]["hcl"], sm, PASSPORT_HAIR_COLOR)) {
				continue;
			}

			if (!std::regex_match(passports_[i]["ecl"], sm, PASSPORT_EYE_COLOR)) {
				continue;
			}

			if (!std::regex_match(passports_[i]["pid"], sm, PASSPORT_ID)) {
				continue;
			}

			result++;
		} else {
			if (passports_[i].size() == 7) {
				if (passports_[i].count("cid") == 0) {
					result++;
				}
			} else if (passports_[i].size() == 8) {
				result++;
			}
		}
	}

	return result;
}

void AoC2020_day04::tests() {
	uint32_t result;

	if (init({"ecl:gry pid:860033327 eyr:2020 hcl:#fffffd", "byr:1937 iyr:2017 cid:147 hgt:183cm", "", "iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884",
			  "hcl:#cfa07d byr:1929", "", "hcl:#ae17e1 iyr:2013", "eyr:2024", "ecl:brn pid:760753108 byr:1931", "hgt:179cm", "",
			  "hcl:#cfa07d eyr:2025 pid:166559648", "iyr:2011 ecl:brn hgt:59in"})) {
		result = get_valid_passports_count(false); // 2
	}

	if (init({"eyr:1972 cid:100", "hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926", "", "iyr:2019", "hcl:#602927 eyr:1967 hgt:170cm",
			  "ecl:grn pid:012533040 byr:1946", "", "hcl:dab227 iyr:2012", "ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277", "", "hgt:59cm ecl:zzz",
			  "eyr:2038 hcl:74454a iyr:2023", "pid:3556412378 byr:2007"})) {
		result = get_valid_passports_count(true); // 0
	}

	if (init({"pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980", "hcl:#623a2f", "", "eyr:2029 ecl:blu cid:129 byr:1989",
			  "iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm", "", "hcl:#888785", "hgt:164cm byr:2001 iyr:2015 cid:88", "pid:545766238 ecl:hzl", "eyr:2022", "",
			  "iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719"})) {
		result = get_valid_passports_count(true); // 4
	}
}

bool AoC2020_day04::part1() {

	result1_ = std::to_string(get_valid_passports_count(false));

	return true;
}

bool AoC2020_day04::part2() {

	result2_ = std::to_string(get_valid_passports_count(true));

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
