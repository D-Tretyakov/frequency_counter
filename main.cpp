#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#ifndef _WIN32
#include <linux/limits.h>
#endif

void PrintHelp();
void OpenFile(FILE** file_desc, const char* filename, const char* mode);

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
    OpenFile(&input_file, argv[1], "r");
    if (!input_file)
    {
        std::perror("Input file opening failed");
        return EXIT_FAILURE;
    }

    OpenFile(&output_file, argv[2], "w");

    int c;
    while ((c = std::fgetc(input_file)) != EOF)
    {
        std::putchar(c);
    }

    if (std::ferror(input_file))
    {
        std::puts("I/O error when reading");
    }

    std::fclose(input_file);

    return EXIT_SUCCESS;
}

void PrintHelp()
{
    std::puts("Usage: ./freq {input_file} {output_file}");
    std::puts("utility to count words occurencies");
}

void OpenFile(FILE** file_desc, const char* filename, const char* mode)
{
#ifdef _WIN32
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