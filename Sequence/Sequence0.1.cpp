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
#include <set>

const int MIN_COUNT_OF_NUMS = 3;
const int MAX_COUNT_OF_NUMS = 5000;
const int MIN_NUM = 1;
const int MAX_NUM = 1000000000;

int CharToInt(char a[])
{
    std::string s = std::string(a);
    int i = atoi(s.c_str());
    return i;
}

std::multiset<int> ReadNumbersFromFile(std::istream& F, int& quantity, bool& isCorrectData)
{
    std::multiset<int> numbers;
    //std::vector<int> nums;
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

            int num = CharToInt(buf);

            if (num < MIN_NUM || num > MAX_NUM)
            {
                isCorrectData = false;
                break;
            }
            numbers.insert(num);
            //nums.push_back(num);
        }
    }
    else
    {
        isCorrectData = false;
    }

    return numbers;
}


/*

*/

int NumberOfTriples(std::multiset<int>& nums) // 5000 - 2.52sec // set вместо vector??
{
    int count = 0;
    std::multiset<int>::iterator firstIt = nums.begin();
    for (int i = 2; i < nums.size(); i++)
    {
        std::advance(firstIt, i);
        int num = *firstIt;

        std::vector<std::vector<int>> pairs;
        pairs.resize(i + 1);

        for (int j = 0; j < (i / 2) + 1; j++)
        {
            std::set<int>::iterator secondIt = nums.begin();
            std::advance(secondIt, j);
            int secondNum = *secondIt;
            int findNum = num - secondNum;
            auto pointer = std::find(nums.begin(), nums.end(), findNum);
            if (pointer != nums.end())
            {
                while (*pointer == findNum)
                {
                    auto t = std::distance(nums.begin(), pointer);
                    int k = static_cast<int>(t);
                    std::vector<int>& pairsAtJ = pairs.at(j);
                    auto it = std::find(pairsAtJ.begin(), pairsAtJ.end(), k);
                    if (pairsAtJ.empty() || it == pairsAtJ.end())
                    {
                        count++;
                        pairsAtJ.push_back(k);
                    }
                    pointer++;
                }
            }
        }
    }

    return count;
}

int main()
{
    clock_t start = clock();
    std::ifstream input("in2.txt");
    if (!input.is_open())
    {
        std::cout << "Failed to open input file for reading.\n";
        return -1;
    }

    int quantity;
    bool isCorrectData = true;
    std::multiset<int> nums = ReadNumbersFromFile(input, quantity, isCorrectData);
    if (!isCorrectData)
    {
        std::cout << "Incorrect data in input file.\n";
        return -1;
    }

    //std::sort(nums.begin(), nums.end());
    int numOfTriples = NumberOfTriples(nums);

    std::cout << numOfTriples << std::endl;

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);

    /*std::ofstream output("output.txt");
    if (!output.is_open())
    {
        std::cout << "Failed to open output file for writing.\n";
        return -1;
    }*/

}