/*
9.4. Песочные часы (6)
Имеется коллекция из N песочных часов. Часы с номером i имеют две характеристики: Ai – максимальная длительность их работы в секундах, 
Bi – время в секундах, которое уже прошло с запуска часов. 
Требуется найти минимальное количество переворотов часов, чтобы все часы закончили свою работу друг за другом с разницей в 1 сек.
    Ввод. 
Первая строка входных данных содержит единственное целое число N (1 ≤ N ≤ 20) – количество часов.  
Вторая строка входных данных содержит N целых чисел A1, A2, ..., AN (0 ≤ Ai ≤ 104) – длительность работы часов в секундах. 
Третья строка входных данных содержит N целых чисел B1, B2, ..., BN (0 ≤ Bi ≤ Ai ≤ 104) - время, которое прошло с запуска часов в секундах.
    Вывод. 
Выведите единственное целое число - минимальное количество переворотов часов, требующихся для выполнения условия. 
Если условие выполнить невозможно, то выведите -1.
Примеры
Ввод 1         Ввод 2           Ввод 3
3              5                4
5 4 9          1 3 5 7 9        13 13 16 14
3 3 6          1 3 5 7 9        9 5 6 7
Вывод 1        Вывод 2          Вывод 3
0              -1               1

Орзаев Святослав, ПС-21, Microsoft visual studio 2019
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>

const int MIN_COUNT_OF_HOURGLASSES = 1;
const int MAX_COUNT_OF_HOURGLASSES = 20;
const int MIN_COUNT_OF_SECONDS = 0;
const int MAX_COUNT_OF_SECONDS = 10000;
const int IMPOSSIBILITY = -1;

struct Time
{
    int num;
    int time;
    bool isRemainingTime = false;
};

int CharToInt(char a[])
{
    std::string s = std::string(a);
    int i = atoi(s.c_str());
    return i;
}

void ReadDataFromFile(std::istream& F, bool& isCorrectData, int& count, std::vector<Time>& set)
{
    F >> count;
    if (count >= MIN_COUNT_OF_HOURGLASSES
        && count <= MAX_COUNT_OF_HOURGLASSES)
    {
        std::vector<int> maxDuration(count);
        char buf[255];
        for (int i = 1; i <= count *2; i++)
        {
            F >> buf;
            int num = CharToInt(buf);
            if (num < MIN_COUNT_OF_SECONDS || num > MAX_COUNT_OF_SECONDS)
            {
                isCorrectData = false;
                break;
            }
            if (i <= count)
            {
                maxDuration.at(i-1) = num;
            }
            else if (i > count && i <= count *2)
            {
                int c = (i - count) - 1;
                int reTime = maxDuration.at(c) - num;
                set.push_back({c, reTime, true});
                set.push_back({c, num, false});
            }
        }
    }
    else
    {
        isCorrectData = false;
    }
}

bool Comparator(Time t1, Time t2)
{
    return t1.time < t2.time;
}

void SubsetVector(std::vector<std::vector<Time>>& options, std::vector<Time>& timesSet, std::vector<bool> taken,
    std::vector<Time> tmp, int size, int prevIterTime, int index, bool isFirstIter)
{
    for (int i = index; i < size * 2; i++)
    {
        Time t = timesSet.at(i);
        int r = t.time;
        if (r == prevIterTime)
        {
            if (i + 1 >= timesSet.size())
            {
                break;
            }
            continue;
        }
        if (r - prevIterTime != 1 && !isFirstIter)
        {
            break;
        }
        else if (r - prevIterTime == 1 || isFirstIter)
        {
            int num = t.num;
            if (taken.at(num))
            {
                continue;
            }
            else
            {
                if (((timesSet.size() - (i + 1)) > 0) && (r == timesSet.at(i + 1).time))
                {
                    if (i + (size - tmp.size()) < timesSet.size())
                    {
                        SubsetVector(options, timesSet, taken, tmp, size, prevIterTime, i + 1, isFirstIter);
                    }
                }
                prevIterTime = timesSet.at(i).time;
                taken.at(num) = true;
                tmp.push_back(t);
                if (isFirstIter)
                {
                    isFirstIter = false;
                }
            }
        }
        if (tmp.size() == size)
        {
            options.push_back(tmp);
            break;
        }
    }
}

int CountNumberOfCoups(std::vector<std::vector<Time>>& options)
{
    int k = -1;
    for (int i = 0; i < options.size(); i++)
    {
        std::vector<Time> times = options.at(i);
        int count = 0;
        for (int j = 0; j < times.size(); j++)
        {
            if (!times.at(j).isRemainingTime)
            {
                count++;
            }
        }
        if (k == -1)
        {
            k = count;
        }
        else if (k != -1 && count < k)
        {
            k = count;
        }
    }
    return k;
}

int CalculateCorrectSubset(int size, std::vector<Time>& timesSet)
{
    std::vector<std::vector<Time>> options{};
    std::vector<Time> tmp;
    int t = -1;
    bool isFirstIt = true;
    for (int i = 0; i <= size; i++)
    {
        if (isFirstIt)
        {
            t = timesSet.at(i).time;
            isFirstIt = false;
        }
        else if (timesSet.at(i).time == t)
        {
            continue;
        }
        std::vector<bool> taken(size, false);
        SubsetVector(options, timesSet, taken, tmp, size, -1, i, true);
    }
    
    if (options.empty())
    {
        return -1;
    }
    return CountNumberOfCoups(options);
}

int main()
{
    clock_t start = clock();
    std::ifstream input("100");
    if (!input.is_open())
    {
        std::cout << "Failed to open input file for reading.\n";
        return -1;
    }

    bool isCorrectData = true;
    std::vector<Time> set{};
    int count = 0;
    ReadDataFromFile(input, isCorrectData, count, set);

    if (!isCorrectData)
    {
        std::cout << "Incorrect values in input file!\n";
        return -1;
    }
    std::sort(set.begin(), set.end(), Comparator);

    int numberOfCoups = CalculateCorrectSubset(count, set);

    std::cout << numberOfCoups << std::endl;

    std::ofstream output("output.txt");
    if (!output.is_open())
    {
        std::cout << "Failed to open output file for writing.\n";
        return -1;
    }
    output << numberOfCoups;
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
}
