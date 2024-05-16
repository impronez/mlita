/*
5.4. Возрастающая таблица (4)
Требуется написать программу, которая по значению ячейки таблицы определит числа в заголовках строки и столбца.
Ввод. Первая строка содержит натуральное число T (1 ≤ T ≤ 10^5 ) — количество наборов входных данных. 
В каждой из следующих T строк задаётся натуральное число X (1 ≤ X ≤ 10^9 ) — значение ячейки таблицы.
Вывод. Для каждой из T строк выведите два натуральных числа, разделённых пробелом — заголовки строки и столбца,
на пересечении которых находится ячейка со значением X. Гарантируется, что для любых входных данных эти значения определяются однозначно.
Пример
Ввод
3
80
16
576
Вывод
5 16
1 16
9 64
ПС-21, Орзаев Святослав, Microsoft Visual Studio 2019
*/

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <utility>

const size_t MIN_SETS_NUM = 1;
const size_t MAX_SETS_NUM = 100000;
const size_t MIN_NUM = 1;
const size_t MAX_NUM = 1000000000;

std::vector<size_t> GetNumsFromFile(std::istream& input)
{
    size_t size;
    input >> size;
    if (size < MIN_SETS_NUM || size > MAX_SETS_NUM)
    {
        throw std::range_error("Incorrect num of sets!");
    }

    std::vector<size_t> nums{};
    for (size_t i = 0; i < size; i++)
    {
        size_t num;
        input >> num;
        if (num < MIN_NUM || num > MAX_NUM || input.fail())
        {
            throw std::invalid_argument("Incorrect value of set");
        }
        nums.push_back(num);
    }
    return nums;
}

size_t GetLineHeader(size_t divisible)
{
    while ((divisible & 1) == 0)
    {
        divisible = divisible >> 1;
    }

    return divisible;
}

std::vector<std::pair<size_t, size_t>> GetPositionsSet(std::vector<size_t>& nums)
{
    size_t size = nums.size();
    std::vector<std::pair<size_t, size_t>> positions{ size };

    for (size_t i = 0; i < size; i++)
    {
        size_t num = nums.at(i);
        size_t lineHeader = GetLineHeader(num);
        size_t columnHeader = num / lineHeader;
        positions.at(i) = { lineHeader, columnHeader };
    }

    return positions;
}

void PrintInfo(std::vector<std::pair<size_t, size_t>>& positions, std::ostream& output)
{
    size_t size = positions.size();
    for (size_t i = 0; i < size; i++)
    {
        std::pair<size_t, size_t> pair = positions.at(i);
        output << pair.first << ' ' << pair.second << std::endl;
    }
}

int main()
{
    clock_t start = clock();
    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cout << "Failed to open text file!\n";
        return -1;
    }

    std::vector<size_t> nums;

    try
    {
        nums = GetNumsFromFile(input);
    }
    catch (const char& except)
    {
        std::cout << except << std::endl;
        return -1;
    }

    std::vector<std::pair<size_t, size_t>> positions = GetPositionsSet(nums);

    std::ofstream output("output.txt");
    if (!output.is_open())
    {
        std::cout << "Failed to open output file!\n";
        return -1;
    }

    PrintInfo(positions, output); 

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
}