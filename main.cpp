#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cctype>

#include "sys/stat.h"
#ifndef _WIN32
#include <linux/limits.h>
#include <stdlib.h>
#endif

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <array>

#include "KeyHash.h"

void PrintHelp();
void OpenFile(FILE** file_desc, const char* filename, const char* mode);
char CheckAndConvert(char c);

int main(int argc, char* argv[])
{
    if (argc == 2 && (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0))
    {
        PrintHelp();
        return EXIT_SUCCESS;
    }
    if (argc < 3 || argc > 3)
    {
        std::puts("Needs exactly 2 arguments");
        PrintHelp();
        return EXIT_FAILURE;
    }

    FILE *input_file, *output_file;
    OpenFile(&input_file, argv[1], "rb");
    if (!input_file)
    {
        std::perror("Input file opening failed");
        return EXIT_FAILURE;
    }

    struct stat buff;
    fstat(fileno(input_file), &buff);
    long input_size = buff.st_size;

    if (input_size > 1'000'000'000)
        std::puts("Warning! File is bigger than 1 Gb");

    char* text;
    try
    {
        text = new char[input_size];
    }
    catch (const std::bad_alloc& exception)
    {
        std::puts("Not enough memory available, exit");
        return EXIT_FAILURE;
    }

    std::fread(text, sizeof(char), input_size, input_file);

    if (std::ferror(input_file))
    {
        std::puts("I/O error when reading");
    }

    std::fclose(input_file);

    std::unordered_map<std::string, int, KeyHash> freq;
    freq.reserve(input_size / 5); // average engilsh word length may be 5

    char word[128] = { 0 }; // almost any english word will fit
    int index = 0;
    for (long i = 0; i < input_size; i++)
    {
        char converted = CheckAndConvert(text[i]);
        if (!converted && (index == 0))
            continue;

        word[index++] = converted;
        if (!converted)
        {
            freq[word]++;
            index = 0;
        }
    }

    using freq_pair = std::pair<std::string, int>;
    std::vector<freq_pair> elems(freq.begin(), freq.end());
    std::sort(elems.begin(), elems.end(), [](const freq_pair& p1, const freq_pair& p2) {
        return p1.second > p2.second || (p1.second == p2.second && p1.first < p2.first);
    });

    OpenFile(&output_file, argv[2], "w");

    for (auto it = elems.begin(); it != elems.end(); ++it)
    {
        std::fprintf(output_file, "%d %s\n", it->second, it->first.c_str());
    }

    std::fclose(output_file);

    delete[] text;

    return EXIT_SUCCESS;
}

void PrintHelp()
{
    std::puts("Usage: ./freq {input_file} {output_file}");
    std::puts("utility to count words occurencies");
}

void OpenFile(FILE** file_desc, const char* filename, const char* mode)
{
#ifdef _WIN32 // i don't have visual studio, tested on MinGW
    char absolute_path[_MAX_PATH];
    char* result = _fullpath(absolute_path, filename, _MAX_PATH);
    if (!result)
    {
        std::perror("Error getting absolute path (maybe abs path too long)");
    }
#else
    char absolute_path[PATH_MAX];
    char* result = realpath(filename, absolute_path);
    if (!result && errno != 2) // ignore no such file error when creating new file
    {
        std::perror("Error getting absolute path");
    }
#endif

    *file_desc = std::fopen(absolute_path, mode);
}

char CheckAndConvert(char c)
{
    // convert A-Z to a-z
    if (c >= 65 && c <= 90)
        return c + 32;
    // if c is lower return it witout modification
    else if (c >= 97 && c <= 122)
        return c;

    return '\0';
}
