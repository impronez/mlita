/*
3.5. Земельные участки (5)
Фермеру Ивану выделено N квадратных единиц посевной площади. Для удобства вычислений земельный кадастр 
распределяет площадь квадратными участками, стороны которых измеряются целыми числами. 
Какое минимальное число участков может быть у Ивана? Какова их площадь?
Ввод.  В   единственной   строке   находится   целое   число  N  (1 ≤ N ≤ 10^5).
Вывод. В первой строке вывести минимально возможное число участков. Во второй их площади в произвольном порядке.
Примеры
Ввод 1    Ввод 2
3         10
Вывод 1   Вывод 2
3         2
1 1 1     9 1
ПС-21, Орзаев Святослав, Microsoft Visual Studio 2019
*/
#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

const size_t MIN_SQUARE = 1;
const size_t MAX_SQUARE = 100000;

size_t ReadSquare(std::istream& input)
{
    size_t num = 0;
    input >> num;
    if (input.fail())
    {
        std::cout << "Failed to read num from input file!\n";
        return -1;
    }
    if (num < MIN_SQUARE || num > MAX_SQUARE)
    {
        std::cout << "Incorrect value!\n";
        return -1;
    }

    return num;
}

bool IsSumOfTwoNums(size_t square, std::vector<size_t>& numSquares, std::vector<size_t>& squares)
{
    for (auto it = numSquares.begin(); it != numSquares.end(); it++)
    {
        auto it2 = std::find(numSquares.begin(), numSquares.end(), (square - *it));
        if (it2 != numSquares.end())
        {
            squares.push_back(*it);
            squares.push_back(*it2);
            return true;
        }
    }
    return false;
}

std::vector<size_t> GetPlotSquares(size_t square)
{
    std::vector<size_t> squares{};
    size_t remains = square;

    if (remains < 4)
    {
        for (size_t i = 0; i < remains; i++)
        {
            squares.push_back(1);
        }
    }
    else
    {
        size_t num = static_cast<size_t>(sqrt(remains));
        size_t sqr = num * num;

        if (remains == sqr)
        {
            squares.push_back(remains);
            return squares;
        }
        else if ((sqrt(remains / 2) * sqrt(remains / 2) * 2) == remains)
        {
            size_t l = remains / 2;
            for (int i = 0; i < 2; i++)
            {
                squares.push_back(l);
            }
            return squares;
        }
        std::vector<size_t> numSquares;
        bool isFound = false;
        for (size_t i = 1; i*i < square; i++) 
        {
            numSquares.push_back(i * i);
        }
        
        if (IsSumOfTwoNums(square, numSquares, squares)) // сумма 2-х квадратов
        {
            return squares;
        }

        for (auto it = numSquares.begin(); it != numSquares.end(); it++) // сумма 3-х квадратов
        {
            remains = square - *it;
            if (IsSumOfTwoNums(remains, numSquares, squares))
            {
                squares.push_back(*it);
                return squares;
            }
        }
        
        for (auto it = numSquares.begin(); it != numSquares.end(); it++) // сумма 4-х квадратов
        {
            remains = square - *it;
            for (auto it2 = numSquares.begin(); it2 != numSquares.end(); it2++)
            {
                remains -= *it2;
                if (IsSumOfTwoNums(remains, numSquares, squares))
                {
                    squares.push_back(*it);
                    squares.push_back(*it2);
                    return squares;
                }
            }
        }
        
    }

    return squares;
}

void PrintInfo(std::ostream& output, std::vector<size_t>& squares)
{
    size_t size = squares.size();
    output << size << std::endl;

    for (size_t i = 0; i < size; i++)
    {
        output << squares.at(i) << ' ';
    }
    output << std::endl;
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

    size_t square = ReadSquare(input);
    if (square == -1)
    {
        return -1;
    }


    std::vector<size_t> squares = GetPlotSquares(square);

    std::ofstream output("output.txt");
    if (!output.is_open())
    {
        std::cout << "Failed to open output file!\n";
        return -1;
    }
    PrintInfo(output, squares);

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
}
