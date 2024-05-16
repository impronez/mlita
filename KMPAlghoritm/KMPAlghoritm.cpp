/*
13.2. �������� ����� � ������� � ������ (6)
��������� ����� ��� ��������� ������� � ��������� ����� ������� ���. � ����� ��� �������� ����. 
������� ����� �������� ������� � ���������� � ����� ������ ����� �� ������. ����� ������ ����� ����� ������������������ ��� ������. 
���������� ������ �� ������ �������� �� �������� ����, �� ���� ������ ����� � ������� � ����� ����� ���� ��� ��������, ��� � ���������.
���� �� ����� INPUT.TXT. ������ ������ ����� �������� �������� � ����� ����� �� 1 �� 255. ������ ������ ������ ��� ���������� �����.
����� � ���� OUTPUT.TXT. ������� � ������ ������ ����� ������ ������������������ ������� ����� � ������� � ������, 
������� � ������� ������� ������ � �����. ��������� ����� � ������� � ������ ���������� � 1. ���� ��������� ���, ������� No.
������
    ����
        ���� ���� ����
        Bukvar.txt
    �����
        1 1
        1 23
���������. ���� Bukvar.txt ������� �� ��������� ���� �����:
���� ���� ���� �����, ����
���� ���� �������.

������ ���������, ��-21, Microsoft Visual Studio 2022
*/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <time.h>

const int MIN_LENGTH_OF_EXEMPLAR = 1;
const int MAX_LENGTH_OF_EXEMPLAR = 255;

bool IsCorrectExemplar(size_t size)
{
    if (size < MIN_LENGTH_OF_EXEMPLAR || size > MAX_LENGTH_OF_EXEMPLAR)
    {
        return false;
    }
    return true;
}

bool ReadInputData(std::string& inputFilename, std::string& exemplar, std::string& textFilename)
{
    std::ifstream input(inputFilename);
    if (!input.is_open())
    {
        std::cout << "Failed to open input file!\n";
        return false;
    }
    std::getline(input, exemplar);
    input >> textFilename;
    if (!IsCorrectExemplar(exemplar.size()) || textFilename.empty())
    {
        std::cout << "Incorrect input data!\n";
        return false;
    }
    if (exemplar.empty())
    {
        std::cout << "Empty exemplar!\n";
        return false;
    }
    return true;
}

void StringToLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), std::tolower);
}

std::vector<size_t> PrefixFunction(const std::string& exemplar)
{
    size_t size = exemplar.length();
    std::vector<size_t> prefixs(size, 0);

    for (size_t i = 1; i < size; i++)
    {
        size_t j = prefixs[i - 1];

        while ((j > 0) && (exemplar[i] != exemplar[j]))
        {
            j = prefixs[j - 1];
        }

        if (exemplar[i] == exemplar[j])
        {
            j++;
        }
        prefixs[i] = j;
    }
    return prefixs;
}

void SubstringSearching(std::istream& textStream, const std::string& exemplar,
    std::vector<std::pair<size_t, size_t>>& occurrences)
{
    std::vector<size_t> prefixs = PrefixFunction(exemplar);
    std::string line;
    size_t numOfLine = 1, matchLineNum = 0, matchPosNum = 0;
    size_t exemplarSize = exemplar.size();
    size_t i = 0, j = 0;
    bool isFromPrevStr = false;
    char ch;
    while (textStream.get(ch))
    {
        if (ch == '\n')
        {
            ch = ' ';
            numOfLine++;
            i = 0;
            if (j != 0)
            {
                isFromPrevStr = true;
            }
        }
        else
        {
            i++;
            ch = std::tolower(ch);
        }

        if (ch == exemplar[j])
        {
            if (j == 0)
            {
                matchLineNum = numOfLine;
                matchPosNum = i;
            }
            j++;
        }
        else
        {
            if (matchLineNum != 0)
            {
                matchLineNum = 0;
                matchPosNum = 0;
            }
            if (isFromPrevStr)
            {
                isFromPrevStr = false;
            }
            if (j != 0)
            {
                while (j != 0)
                {
                    j = prefixs[j - 1];
                    if (ch == exemplar[j])
                    {
                        j++;
                        break;
                    }
                }
            }
        }
        if (j == exemplarSize)
        {
            if (isFromPrevStr)
            {
                occurrences.push_back({ matchLineNum, matchPosNum });
            }
            else
            {
                occurrences.push_back({ numOfLine, i - j + 1 });
            }
            matchPosNum = matchPosNum + (j - prefixs[j - 1]);
            j = prefixs[j - 1];
            if (isFromPrevStr && j != 0 && j < i)
            {
                isFromPrevStr = false;
            }
        }
    }
}

void PrintInfo(std::vector<std::pair<size_t, size_t>>& occurrences, std::ostream& output)
{
    if (occurrences.empty())
    {
        output << "No\n";
        return;
    }
    for (int i = 0; i < occurrences.size(); i++)
    {
        output << occurrences[i].first << " " << occurrences[i].second << std::endl;
    }
}

int main()
{
    setlocale(LC_ALL, "");
    clock_t start = clock();
    std::string inputFilename = "INPUT3.txt",
        textFilename, exemplar;
    if (!ReadInputData(inputFilename, exemplar, textFilename))
    {
        return -1;
    }
    StringToLower(exemplar);

    std::ifstream textFile(textFilename);
    if (!textFile.is_open())
    {
        std::cout << "Failed to open text file!\n";
        return -1;
    }
    std::vector<std::pair<size_t, size_t>> occurrences;
    SubstringSearching(textFile, exemplar, occurrences);

    std::ofstream output("output.txt");
    if (!output.is_open())
    {
        std::cout << "Failed to open output file!\n";
        return -1;
    }
    PrintInfo(occurrences, output);
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
}