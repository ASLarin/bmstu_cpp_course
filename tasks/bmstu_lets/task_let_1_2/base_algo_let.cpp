#include "base_algo_let.h"
#include <algorithm>

std::vector<int> positive_numbers(const std::vector<int>& nums)
{
	// std::vector<int> result;
	// std::copy_if(nums.begin(), nums.end(), std::back_inserter(result),
	// 			 [](int value) { return value > 0; });
	// return result;
}

void sort_positive_numbers(std::vector<int>& nums)
{
	auto partition_point = std::partition(nums.begin(), nums.end(),
										  [](int num) { return num > 0; });
	return std::sort(nums.begin(), partition_point);
}

int sum_positive_numbers(const std::vector<int>& nums)
{
	int sum = 0;
	std::for_each(nums.begin(), nums.end(),
				  [&sum](int x)
				  {
					  if (x > 0)
					  {
						  sum += x;
					  }
				  });
	return sum;
}

bool is_divisible_by_10(const std::vector<int>& nums)
{
	return std::any_of(nums.begin(), nums.end(),
					   [](int x) { return x % 10 == 0; });
}

void replace_negative_numbers(std::vector<int>& nums)
{
	std::replace_if(
		nums.begin(), nums.end(), [](int value) { return value < 0; }, 0);
}

void double_values(std::vector<int>& nums)
{
	std::for_each(nums.begin(), nums.end(), [](int& value) { value *= 2; });
}

void sort_students_by_age(std::vector<Student>& data)
{
	std::sort(data.begin(), data.end(),
			  [](const Student& a, const Student& b) { return a.age < b.age; });
}

void sort_students_by_name(std::vector<Student>& students)
{
	std::sort(students.begin(), students.end(),
			  [](const Student& a, const Student& b)
			  { return a.name < b.name; });
}
