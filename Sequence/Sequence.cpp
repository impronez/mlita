/*
10.12. Последовательность 4 (6)
Имеется последовательность целых чисел x1, x2, …, xn. Найти количество троек чисел, в каждой из которых одно из чисел равно сумме двух других. Две тройки считаются разными, если множества индексов отличаются хотя бы одним элементом.
Ввод из файла INPUT.TXT. В первой строке содержится значение n (3 ≤ n ≤ 5000). Во второй строке заданы через пробел значения x1, x2, …, xn  (1 ≤ xi ≤ 109).
Вывод в файл OUTPUT.TXT. В единственной строке выводится количество указанных троек чисел (xi, xj, xk).
Примеры
Ввод 1          Ввод 2
5               5
2 4 1 3 6       5 7 3 5 9
Вывод 1         Вывод 2
3               0
Орзаев Святослав, ПС-21, Microsoft Visual Studio 2022
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <time.h>

const int MIN_COUNT_OF_NUMS = 3;
const int MAX_COUNT_OF_NUMS = 5000;
const int MIN_NUM = 1;
const int MAX_NUM = 1000000000;

unsigned long int CharToInt(char a[])
{
    std::string s = std::string(a);
    unsigned long int i = atoi(s.c_str());
    return i;
}

std::vector<unsigned long int> ReadNumbersFromFile(std::istream& F, int& quantity, bool& isCorrectData)
{
    std::vector<unsigned long int> nums;
    char buf[255];
    F >> quantity;
    if (quantity >= MIN_COUNT_OF_NUMS && quantity <= MAX_COUNT_OF_NUMS)
    {
        while (!F.eof())
        {
            F >> buf;
            if (buf[0] == '\0')
            {
                continue;
            }

            unsigned long int num = CharToInt(buf);

            if (num < MIN_NUM || num > MAX_NUM)
            {
                isCorrectData = false;
                break;
            }

            nums.push_back(num);
        }
    }
    else
    {
        isCorrectData = false;
    }
    
    return nums;
}

unsigned long int NumberOfTriples(std::vector<unsigned long int>& nums)
{
    unsigned long int count = 0;
    for (int i = 2; i < nums.size(); i++)
    {
        unsigned long int num = nums.at(i);
        if (num < 2)
        {
            continue;
        }
        for (int j = 0; j < i; j++)
        {
            unsigned long int secondNum = nums.at(j);
            unsigned long int findNum = num - secondNum;
            if (findNum <= 0)
            {
                continue;
            }
            std::vector<unsigned long int>::iterator startIt = nums.begin() + j + 1;
            std::vector<unsigned long int>::iterator endIt = nums.begin() + i;
            auto range = std::equal_range(startIt, endIt, findNum);
            if (range.first != range.second)
            {
                count += static_cast<unsigned long int>(range.second - range.first);
            }
        }
    }

    return count;
}

int main()    
{
    clock_t start = clock();
    std::ifstream input("input9.txt");
    if (!input.is_open())
    {
        std::cout << "Failed to open input file for reading.\n";
        return -1;
    }

    int quantity;
    bool isCorrectData = true;
    std::vector<unsigned long int> nums = ReadNumbersFromFile(input, quantity, isCorrectData);
    if (!isCorrectData)
    {
        std::cout << "Incorrect data in input file.\n";
        return -1;
    }

    std::sort(nums.begin(), nums.end());
    unsigned long int numOfTriples = NumberOfTriples(nums);
    //std::cout << numOfTriples << std::endl;

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);

    std::ofstream output("output.txt");
    if (!output.is_open())
    {
        std::cout << "Failed to open output file for writing.\n";
        return -1;
    }
    output << numOfTriples << std::endl;

}