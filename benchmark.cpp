#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <cmath>

constexpr int iterations_number = 1'000;

int main()
{
    auto bench_start = std::chrono::system_clock::now();
    std::vector<double> measurements(iterations_number);
    for (int i = 0; i < iterations_number; i++)
    {
        if (i % 100 == 0)
            std::cout << i << " / " << iterations_number << std::endl;

        auto start = std::chrono::system_clock::now();

        // std::system(".\\freq.exe ../black_sabbath.txt ../output.txt");
        std::system("cat ../black_sabbath.txt");

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        measurements[i] = elapsed.count();
    }

    double average = std::accumulate(measurements.begin(), measurements.end(), 0.0) / measurements.size();
    double max_value = *std::max_element(measurements.begin(), measurements.end());
    double min_value = *std::min_element(measurements.begin(), measurements.end());

    std::vector<double> diff(measurements.size());
    std::transform(measurements.begin(), measurements.end(), diff.begin(), [average](double x) { return x - average; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / measurements.size());

    auto bench_end = std::chrono::system_clock::now();
    auto bench_elapsed = std::chrono::duration_cast<std::chrono::seconds>(bench_end - bench_start);

    std::cout << "Results:" << std::endl;
    std::cout << "average = " << average << "ms" << std::endl;
    std::cout << "max = " << max_value << "ms" << std::endl;
    std::cout << "min = " << min_value << "ms" << std::endl;
    std::cout << "std_dev = " << stdev << "ms" << std::endl;
    std::cout << "bench executed for " << std::fixed << bench_elapsed.count() << "s" << std::endl;

    std::ofstream output("benchmark_output.csv");
    for (int i = 0; i < iterations_number; i++)
    {
        output << measurements[i] << "," << i << "\n";
    }

    return 0;
}