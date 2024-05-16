/*2.10. Шестеренки (7)
Имеется система сцепленных шестеренок, пронумерованных от 1 до N. Заданы пары номеров сцепленных шестеренок. 
Первая должна двигаться по  часовой стрелке. Определить характер движения всех шестеренок.
Варианты движения:
    clockwise - по часовой;
    unclockwise - против;
    immovable - шестеренка неподвижна;
    block - заклинивание.
Ввод из файла INPUT.TXT. В первой строке записаны через пробел число N - количество шестеренок (2 ≤ N ≤ 10000)
и M - количество пар (1 ≤ M ≤ 100000). Во следующих M строках указаны пары шестеренок в виде двух номеров через пробел.
Вывод в файл OUTPUT.TXT. В i-й строке должна быть информация о движении i-й шестеренки: 
clockwise - по часовой, against - против, immovable – шестеренка неподвижна. Если система заклинивается, то выдать block.
Примеры
Ввод 1       Ввод 2
5 4          5 4
1 2          1 2
2 3          2 3
3 4          1 3
4 1          4 5
Вывод 1      Вывод 2
clockwise    block
unclockwise
clockwise
unclockwise
immovable

Орзаев Святослав, ПС-21
Microsoft Visual Studio 2022
*/

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>

const char CLOCKWISE = 'c';
const char UNCLOCKWISE = 'u';
const char IMMOVABLE = 'i';
const int MAX_COUNT_OF_GEARS = 50000;
const int MAX_COUNT_OF_PAIRS = 500000;

typedef std::vector<std::vector<int>> Pairs;

void SetDetermines(std::vector<char>& movements, Pairs& pairs,
    int& firstGear, int& secondGear, bool& isJamming);

int CharToInt(char a[])
{
    std::string s = std::string(a);
    int i = atoi(s.c_str());
    return i;
}

bool IsJamming(std::vector<char>& movements, int& firstGear, int& secondGear)
{
    if ((movements.at(firstGear - 1) == CLOCKWISE && movements.at(secondGear - 1) == CLOCKWISE) ||
        (movements.at(firstGear - 1) == UNCLOCKWISE && movements.at(secondGear - 1) == UNCLOCKWISE))
    {
        return true;
    }
    return false;
}

void AddPair(Pairs& pairs, int& firstGear, int& secondGear)
{
    pairs.at(firstGear - 1).push_back(secondGear);
    pairs.at(secondGear - 1).push_back(firstGear);
}

void CheckPairsForMovement(Pairs& pairs, std::vector<char>& movements, int& num, bool& isJamming)
{
    for (int i = 0; i < pairs.at(num - 1).size(); i++)
    {
        int numOfGear = pairs.at(num - 1).at(i);
        if (movements.at(num - 1) != IMMOVABLE &&
            movements.at(numOfGear - 1) != IMMOVABLE)
        {
            isJamming = IsJamming(movements, num, numOfGear);
            if (isJamming)
            {
                break;
            }
        }
        else
        {
            SetDetermines(movements, pairs, num, numOfGear, isJamming);
        }
    }
}

void SetDetermines(std::vector<char>& movements, Pairs& pairs,
    int& firstGear, int& secondGear, bool& isJamming)
{
    if (movements.at(firstGear - 1) != IMMOVABLE)
    {
        switch (movements.at(firstGear - 1))
        {
        case CLOCKWISE:
            movements.at(secondGear - 1) = UNCLOCKWISE;
            break;
        case UNCLOCKWISE:
            movements.at(secondGear - 1) = CLOCKWISE;
            break;
        default:
            break;
        }
        CheckPairsForMovement(pairs, movements, secondGear, isJamming);
    }
    else if (movements.at(secondGear - 1) != IMMOVABLE)
    {
        switch (movements.at(secondGear - 1))
        {
        case CLOCKWISE:
            movements.at(firstGear - 1) = UNCLOCKWISE;
            break;
        case UNCLOCKWISE:
            movements.at(firstGear - 1) = CLOCKWISE;
            break;
        default:
            break;
        }
        CheckPairsForMovement(pairs, movements, firstGear, isJamming);

    }
}

void DetermineMovements(std::istream& F, std::vector<char>& movements, Pairs& pairs, bool& isJamming)
{
    char buf[255];
    movements.at(0) = CLOCKWISE;
    while (!F.eof())
    {
        F >> buf;
        if (buf[0] == '\0')
        {
            continue;
        }
        int firstNum = CharToInt(buf);
        F >> buf;
        int secondNum = CharToInt(buf);
        isJamming = IsJamming(movements, firstNum, secondNum);
        if (isJamming)
        {
            break;            
        }
        else
        {
            SetDetermines(movements, pairs, firstNum, secondNum, isJamming);
            AddPair(pairs, firstNum, secondNum);
        }
        if (isJamming)
        {
            break;
        }
    }
    if (!isJamming)
    {
        for (int i = 1; i <= movements.size(); i++)
        {
            CheckPairsForMovement(pairs, movements, i, isJamming);
        }
    }
}

void OutputInfoAboutMovements(std::ostream& output, std::vector<char>& movements, bool& isJamming)
{
    if (isJamming)
    {
        output << "block\n";
        return;
    }

    for (int i = 0; i < movements.size(); i++)
    {
        switch (movements.at(i))
        {
        case CLOCKWISE:
            output << "clockwise\n";
            break;
        case UNCLOCKWISE:
            output << "unclockwise\n";
            break;
        case IMMOVABLE:
            output << "immovable\n";
            break;
        default:
            break;
        }
    }
}

int main()
{
    clock_t start = clock();
    std::ifstream input;
    input.open("input8.txt");
    if (!input.is_open())
    {
        std::cout << "Failed to open input file.";
        return 1;
    }
    int countOfGears, countOfPairs;
    input >> countOfGears >> countOfPairs;
    if (countOfGears < 2 || countOfGears > MAX_COUNT_OF_GEARS
        || countOfPairs < 1 || countOfPairs > MAX_COUNT_OF_PAIRS)
    {
        std::cout << "Incorrect count of gears or pairs.\n";
        return 1;
    }
    std::vector<char> movements;
    Pairs pairs;
    pairs.resize(countOfGears);
    movements.resize(countOfGears, IMMOVABLE);

    bool isJamming = false;
    DetermineMovements(input, movements, pairs, isJamming);
    
    std::ofstream output;
    output.open("output.txt");
    OutputInfoAboutMovements(output, movements, isJamming);
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
}