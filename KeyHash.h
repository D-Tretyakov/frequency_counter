#pragma once
#include <string>

constexpr size_t InitialFNV = 2166136261U;
constexpr size_t FNVMultiple = 16777619;

struct KeyHash
{
    std::size_t operator()(const std::string& k) const
    {
        size_t hash = InitialFNV;
        for (size_t i = 0; i < k.length(); i++)
        {
            hash = hash ^ (k[i]);      /* xor  the low 8 bits */
            hash = hash * FNVMultiple; /* multiply by the magic number */
        }
        return hash;
    }
};