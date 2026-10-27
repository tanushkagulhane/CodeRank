// timing_comparison.h - Performance benchmark comparing custom data structures against plain sorted lists
// Member 3 Shared Requirement - Advanced Data Structures
#pragma once

#include "Models.h"
#include "binomial_heap.h"
#include "dsu.h"
#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>

struct BenchmarkResult {
    std::string operationName;
    int dataSize;
    double customTimeMs;
    double plainTimeMs;
    double speedupFactor;
    std::string customComplexity;
    std::string plainComplexity;
};

class TimingComparison {
public:
    // Run the full suite of timing comparisons
    static std::vector<BenchmarkResult> runAllBenchmarks(bool verbose = true);

    // Benchmark 1: Submission Queue (Binomial Heap vs Plain Sorted Vector)
    static BenchmarkResult benchmarkQueueInsert(int n);
    static BenchmarkResult benchmarkQueueMerge(int n);
    static BenchmarkResult benchmarkQueueExtractMin(int n);

    // Benchmark 2: Teams Disjoint Set Union vs Plain Array Re-labeling
    static BenchmarkResult benchmarkTeamUnions(int n);
    static BenchmarkResult benchmarkTeamFind(int n);

    // Pretty-print results in a formatted table
    static void printResultsTable(const std::vector<BenchmarkResult>& results);
};
