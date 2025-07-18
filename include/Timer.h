#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

class Timer {
private:
    inline static size_t const EXPECTED_MAX_DIGITS = 8;
    inline static size_t maxLabelSize = 0;
    inline static std::unordered_map<std::string, double> data = {};
    inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_GlobalStart;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    std::string label;

public:
    inline Timer(std::string const& label)
        : m_StartTimepoint(std::chrono::high_resolution_clock::now())
        , label(label)
    {
    }

    inline ~Timer() { Stop(); }

    inline static void printData()
    {
        auto const endTimepoint = std::chrono::high_resolution_clock::now();
        auto const globalDuration
            = std::chrono::duration<double, std::milli>(endTimepoint - m_GlobalStart).count();

        size_t const borderSize = maxLabelSize + EXPECTED_MAX_DIGITS + 14;
        std::string const border(borderSize, '-');

        std::cout << border << '\n';

        for (auto const& pair : data) {

            std::cout << std::left << std::setw(maxLabelSize) << pair.first << ": " << std::right
                      << std::fixed << std::setw(EXPECTED_MAX_DIGITS) << std::setprecision(3)
                      << pair.second << " ms | " << std::setprecision(1)
                      << (pair.second / globalDuration) * 100 << "%\n";
        }
        std::cout << border << std::endl;
    }

    inline static void Start() { m_GlobalStart = std::chrono::high_resolution_clock::now(); }

private:
    inline void Stop()
    {
        auto const endTimepoint = std::chrono::high_resolution_clock::now();
        auto const duration
            = std::chrono::duration<double, std::milli>(endTimepoint - m_StartTimepoint).count();

        if (label.size() > maxLabelSize) {
            maxLabelSize = label.size();
        }

        data[label] += duration;
    }
};
