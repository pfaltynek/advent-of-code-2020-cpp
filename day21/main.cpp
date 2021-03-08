#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>
#include <set>
#include <string>

const std::regex FOOD("^(.*) \\(contains (.*)\\)$");

class AoC2020_day21 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> foods_;
	std::map<std::string, std::vector<uint32_t>> allergens_in_food_map_;
	std::map<std::string, std::string> allergens_identified_;
	uint64_t get_ingredients_without_allergens_count();
	void find_common_ingredients(const std::vector<std::string>& ingredients1, const std::vector<std::string>& ingredients2, std::vector<std::string>& common);
	void remove_identified_allergens_from_ingredients(std::vector<std::string>& allergens, std::vector<std::string>& ingredients,
													  std::vector<std::string>& result);
	void identify_all_allergens();
	uint64_t count_ingredients_without_allergen();
};

bool AoC2020_day21::init(const std::vector<std::string> lines) {
	std::smatch sm;
	std::vector<std::string> left, right;

	foods_.clear();
	allergens_in_food_map_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, FOOD)) {
			left = split(sm.str(1), " ");
			right = split(sm.str(2), ", ");
			std::sort(left.begin(), left.end());
			std::sort(right.begin(), right.end());
			foods_.push_back({left, right});

			for (size_t j = 0; j < right.size(); j++) {
				allergens_in_food_map_[right[j]].push_back(foods_.size() - 1);
			}
		} else {
			std::cout << "Invalid food recipe at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

void AoC2020_day21::find_common_ingredients(const std::vector<std::string>& ingredients1, const std::vector<std::string>& ingredients2,
											std::vector<std::string>& common) {
	std::vector<std::string>::iterator comm_it;

	common.resize(std::max(ingredients1.size(), ingredients2.size()));

	comm_it = std::set_intersection(ingredients1.begin(), ingredients1.end(), ingredients2.begin(), ingredients2.end(), common.begin());

	common.resize(comm_it - common.begin());
}

void AoC2020_day21::remove_identified_allergens_from_ingredients(std::vector<std::string>& allergens, std::vector<std::string>& ingredients,
																 std::vector<std::string>& result) {
	std::vector<std::string>::iterator diff_it;

	result.resize(ingredients.size());

	diff_it = std::set_difference(ingredients.begin(), ingredients.end(), allergens.begin(), allergens.end(), result.begin());

	result.resize(diff_it - result.begin());
}

void AoC2020_day21::identify_all_allergens() {
	std::vector<std::string> common, allergens_list = {}, tmp;

	allergens_identified_.clear();

	// identify all allergens
	while (allergens_identified_.size() < allergens_in_food_map_.size()) {
		for (auto it = allergens_in_food_map_.begin(); it != allergens_in_food_map_.end(); it++) {
			if (allergens_identified_.count(it->first)) {
				continue; // skip allergen already identified
			}

			if (it->second.size() == 1) { // if allergen contained in only one food
				common = foods_[it->second[0]].first;

				// remove all ingedients identified as allergens
				remove_identified_allergens_from_ingredients(allergens_list, common, tmp);
				tmp.swap(common);
			} else { // allergen contained in more foods
				common = foods_[it->second[0]].first;

				for (size_t i = 1; i < it->second.size(); i++) {
					find_common_ingredients(common, foods_[it->second[i]].first, tmp);
					tmp.swap(common);
				}

				remove_identified_allergens_from_ingredients(allergens_list, common, tmp);
				tmp.swap(common);
			}
			// if there is only one ingredient contained in all foods then it is allergen
			if (common.size() == 1) {
				allergens_identified_[it->first] = common[0];
				allergens_list.push_back(common[0]);
				std::sort(allergens_list.begin(), allergens_list.end());
			}
		}
	}
}

uint64_t AoC2020_day21::count_ingredients_without_allergen() {
	std::vector<std::string> allergens = {}, tmp = {};
	uint64_t result = 0;

	for (auto it = allergens_identified_.begin(); it != allergens_identified_.end(); it++) {
		allergens.push_back(it->second);
	}

	std::sort(allergens.begin(), allergens.end());

	for (size_t i = 0; i < foods_.size(); i++) {
		remove_identified_allergens_from_ingredients(allergens, foods_[i].first, tmp);
		result += tmp.size();
	}

	return result;
}

void AoC2020_day21::tests() {
	uint64_t result;

	if (init({"mxmxvkd kfcds sqjhc nhms (contains dairy, fish)", "trh fvjkl sbzzf mxmxvkd (contains dairy)", "sqjhc fvjkl (contains soy)",
			  "sqjhc mxmxvkd sbzzf (contains fish)"})) {
		identify_all_allergens();
		result = count_ingredients_without_allergen(); // 5
	}
}

bool AoC2020_day21::part1() {

	identify_all_allergens();

	result1_ = std::to_string(count_ingredients_without_allergen());

	return true;
}

bool AoC2020_day21::part2() {

	result2_ = std::to_string(0);

	return true;
}

int32_t AoC2020_day21::get_aoc_day() {
	return 21;
}

int32_t AoC2020_day21::get_aoc_year() {
	return 2020;
}

int main(void) {
	AoC2020_day21 day21;

	return day21.main_execution();
}
