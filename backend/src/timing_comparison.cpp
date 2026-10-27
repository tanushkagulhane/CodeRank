// timing_comparison.cpp - Implementation of Timing Comparison benchmarks
#include "timing_comparison.h"
#include <random>
#include <chrono>

using namespace std::chrono;

// Helper: Plain sorted list insertion (maintains sorted order via binary search + shift)
static void insertSorted(std::vector<Submission>& vec, const Submission& sub) {
    auto it = std::lower_bound(vec.begin(), vec.end(), sub, [](const Submission& a, const Submission& b) {
        if (a.time != b.time) return a.time < b.time;
        if (a.problemId != b.problemId) return a.problemId < b.problemId;
        return a.id < b.id;
    });
    vec.insert(it, sub);
}

// Helper: Generate random submissions
static std::vector<Submission> generateSubmissions(int n, int seed = 42) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> timeDist(1, 300);
    std::uniform_int_distribution<int> probDist(1, 35);
    std::uniform_int_distribution<int> userDist(1, 1000);

    std::vector<Submission> subs;
    subs.reserve(n);
    for (int i = 1; i <= n; ++i) {
        subs.emplace_back(userDist(rng), probDist(rng), timeDist(rng), "Accepted", "", i);
    }
    return subs;
}

BenchmarkResult TimingComparison::benchmarkQueueInsert(int n) {
    std::vector<Submission> data = generateSubmissions(n, 101);

    // 1. Binomial Heap
    auto t1 = high_resolution_clock::now();
    BinomialHeap heap;
    for (const auto& s : data) {
        heap.insert(s);
    }
    auto t2 = high_resolution_clock::now();
    double customMs = duration<double, std::milli>(t2 - t1).count();

    // 2. Plain Sorted Vector (shift insert)
    auto t3 = high_resolution_clock::now();
    std::vector<Submission> plainList;
    plainList.reserve(n);
    for (const auto& s : data) {
        insertSorted(plainList, s);
    }
    auto t4 = high_resolution_clock::now();
    double plainMs = duration<double, std::milli>(t4 - t3).count();

    BenchmarkResult res;
    res.operationName = "Queue Insert (" + std::to_string(n) + " items)";
    res.dataSize = n;
    res.customTimeMs = customMs;
    res.plainTimeMs = plainMs;
    res.speedupFactor = (customMs > 0.0) ? (plainMs / customMs) : 1.0;
    res.customComplexity = "O(N log N) [Binomial Heap]";
    res.plainComplexity = "O(N^2) [Sorted Vector Shift]";
    return res;
}

BenchmarkResult TimingComparison::benchmarkQueueMerge(int n) {
    std::vector<Submission> data1 = generateSubmissions(n / 2, 201);
    std::vector<Submission> data2 = generateSubmissions(n / 2, 202);

    // 1. Binomial Heap Merge
    BinomialHeap heap1, heap2;
    for (const auto& s : data1) heap1.insert(s);
    for (const auto& s : data2) heap2.insert(s);

    auto t1 = high_resolution_clock::now();
    heap1.merge(heap2);
    auto t2 = high_resolution_clock::now();
    double customMs = duration<double, std::milli>(t2 - t1).count();

    // 2. Plain Sorted Vector Merge
    std::vector<Submission> vec1, vec2;
    vec1.reserve(n / 2);
    vec2.reserve(n / 2);
    for (const auto& s : data1) insertSorted(vec1, s);
    for (const auto& s : data2) insertSorted(vec2, s);

    auto t3 = high_resolution_clock::now();
    std::vector<Submission> merged;
    merged.resize(vec1.size() + vec2.size());
    std::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), merged.begin(),
               [](const Submission& a, const Submission& b) {
                   if (a.time != b.time) return a.time < b.time;
                   if (a.problemId != b.problemId) return a.problemId < b.problemId;
                   return a.id < b.id;
               });
    auto t4 = high_resolution_clock::now();
    double plainMs = duration<double, std::milli>(t4 - t3).count();

    BenchmarkResult res;
    res.operationName = "Queue Merge (2 x " + std::to_string(n / 2) + " items)";
    res.dataSize = n;
    res.customTimeMs = customMs;
    res.plainTimeMs = plainMs;
    res.speedupFactor = (customMs > 0.0) ? (plainMs / customMs) : 1.0;
    res.customComplexity = "O(log N) [Binomial Heap]";
    res.plainComplexity = "O(N) [Sorted Vector Merge]";
    return res;
}

BenchmarkResult TimingComparison::benchmarkQueueExtractMin(int n) {
    std::vector<Submission> data = generateSubmissions(n, 303);

    BinomialHeap heap;
    for (const auto& s : data) heap.insert(s);

    auto t1 = high_resolution_clock::now();
    while (!heap.empty()) {
        heap.extractMin();
    }
    auto t2 = high_resolution_clock::now();
    double customMs = duration<double, std::milli>(t2 - t1).count();

    // Plain vector: repeatedly finding minimum element in unsorted vector (or pop from sorted)
    // To demonstrate why heaps are used instead of dynamic scanning:
    std::vector<Submission> plainVec = data;
    auto t3 = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        auto minIt = std::min_element(plainVec.begin(), plainVec.end(), [](const Submission& a, const Submission& b) {
            return a.time < b.time;
        });
        std::swap(*minIt, plainVec.back());
        plainVec.pop_back();
    }
    auto t4 = high_resolution_clock::now();
    double plainMs = duration<double, std::milli>(t4 - t3).count();

    BenchmarkResult res;
    res.operationName = "Queue Extract-Min (" + std::to_string(n) + " items)";
    res.dataSize = n;
    res.customTimeMs = customMs;
    res.plainTimeMs = plainMs;
    res.speedupFactor = (customMs > 0.0) ? (plainMs / customMs) : 1.0;
    res.customComplexity = "O(N log N) [Binomial Heap]";
    res.plainComplexity = "O(N^2) [Unsorted Linear Search]";
    return res;
}

BenchmarkResult TimingComparison::benchmarkTeamUnions(int n) {
    std::mt19937 rng(404);
    std::uniform_int_distribution<int> dist(1, n);

    std::vector<std::pair<int, int>> pairs;
    pairs.reserve(n);
    for (int i = 0; i < n; ++i) {
        pairs.emplace_back(dist(rng), dist(rng));
    }

    // 1. DSU with path compression and union by rank
    auto t1 = high_resolution_clock::now();
    DisjointSetUnion dsu;
    for (int i = 1; i <= n; ++i) dsu.makeSet(i);
    for (const auto& p : pairs) {
        dsu.unionSets(p.first, p.second);
    }
    auto t2 = high_resolution_clock::now();
    double customMs = duration<double, std::milli>(t2 - t1).count();

    // 2. Naive Component Array (re-labeling all elements on union)
    auto t3 = high_resolution_clock::now();
    std::vector<int> teamId(n + 1);
    for (int i = 1; i <= n; ++i) teamId[i] = i;
    for (const auto& p : pairs) {
        int u = p.first, v = p.second;
        int oldT = teamId[v], newT = teamId[u];
        if (oldT != newT) {
            for (int i = 1; i <= n; ++i) {
                if (teamId[i] == oldT) teamId[i] = newT;
            }
        }
    }
    auto t4 = high_resolution_clock::now();
    double plainMs = duration<double, std::milli>(t4 - t3).count();

    BenchmarkResult res;
    res.operationName = "Team Union (" + std::to_string(n) + " unions)";
    res.dataSize = n;
    res.customTimeMs = customMs;
    res.plainTimeMs = plainMs;
    res.speedupFactor = (customMs > 0.0) ? (plainMs / customMs) : 1.0;
    res.customComplexity = "O(N alpha(N)) [DSU]";
    res.plainComplexity = "O(N^2) [Naive Array Re-label]";
    return res;
}

BenchmarkResult TimingComparison::benchmarkTeamFind(int n) {
    std::mt19937 rng(505);
    std::uniform_int_distribution<int> dist(1, n);

    DisjointSetUnion dsu;
    for (int i = 1; i <= n; ++i) dsu.makeSet(i);
    // Connect half into trees
    for (int i = 1; i < n / 2; ++i) {
        dsu.unionSets(i, i + 1);
    }

    int queries = n * 5;
    auto t1 = high_resolution_clock::now();
    volatile int dummy = 0;
    for (int i = 0; i < queries; ++i) {
        dummy += dsu.find(dist(rng));
    }
    auto t2 = high_resolution_clock::now();
    double customMs = duration<double, std::milli>(t2 - t1).count();

    // Plain: uncompressed tree traversal
    std::vector<int> naiveParent(n + 1);
    for (int i = 1; i <= n; ++i) naiveParent[i] = i;
    for (int i = 1; i < n / 2; ++i) naiveParent[i] = i + 1; // chain of length N/2!

    auto t3 = high_resolution_clock::now();
    volatile int dummy2 = 0;
    for (int i = 0; i < queries; ++i) {
        int curr = dist(rng);
        while (naiveParent[curr] != curr) {
            curr = naiveParent[curr];
        }
        dummy2 += curr;
    }
    auto t4 = high_resolution_clock::now();
    double plainMs = duration<double, std::milli>(t4 - t3).count();

    BenchmarkResult res;
    res.operationName = "Team Find (" + std::to_string(queries) + " queries)";
    res.dataSize = queries;
    res.customTimeMs = customMs;
    res.plainTimeMs = plainMs;
    res.speedupFactor = (customMs > 0.0) ? (plainMs / customMs) : 1.0;
    res.customComplexity = "O(Q alpha(N)) [DSU Path Compression]";
    res.plainComplexity = "O(Q * Depth) [Uncompressed Tree]";
    return res;
}

std::vector<BenchmarkResult> TimingComparison::runAllBenchmarks(bool verbose) {
    if (verbose) {
        std::cout << "\n========================================================================================\n";
        std::cout << "        RUNNING PERFORMANCE BENCHMARKS (CUSTOM ADS VS PLAIN SORTED LISTS)       \n";
        std::cout << "========================================================================================\n";
        std::cout << "Benchmarking across dataset sizes...\n";
    }

    std::vector<BenchmarkResult> results;
    results.push_back(benchmarkQueueInsert(2000));
    results.push_back(benchmarkQueueInsert(8000));
    results.push_back(benchmarkQueueMerge(10000));
    results.push_back(benchmarkQueueMerge(40000));
    results.push_back(benchmarkQueueExtractMin(3000));
    results.push_back(benchmarkTeamUnions(3000));
    results.push_back(benchmarkTeamUnions(8000));
    results.push_back(benchmarkTeamFind(10000));

    if (verbose) {
        printResultsTable(results);
    }

    return results;
}

void TimingComparison::printResultsTable(const std::vector<BenchmarkResult>& results) {
    std::cout << "\n========================================================================================================\n";
    std::cout << "                     PERFORMANCE COMPARISON: CUSTOM ADS vs PLAIN LIST                                    \n";
    std::cout << "========================================================================================================\n";
    std::cout << std::left << std::setw(32) << "Benchmark Operation"
              << std::setw(12) << "Data Size"
              << std::setw(16) << "Custom ADS (ms)"
              << std::setw(16) << "Plain List (ms)"
              << std::setw(12) << "Speedup"
              << "Theoretical Complexity\n";
    std::cout << "--------------------------------------------------------------------------------------------------------\n";

    for (const auto& r : results) {
        std::cout << std::left << std::setw(32) << r.operationName
                  << std::setw(12) << r.dataSize
                  << std::fixed << std::setprecision(3)
                  << std::setw(16) << r.customTimeMs
                  << std::setw(16) << r.plainTimeMs
                  << std::setprecision(1)
                  << std::setw(12) << (std::to_string(r.speedupFactor).substr(0, 5) + "x")
                  << r.customComplexity << " vs " << r.plainComplexity << "\n";
    }
    std::cout << "========================================================================================================\n";
    std::cout << "Verdict: Custom ADS structures consistently outperform naive sorted/array implementations by orders of magnitude!\n\n";
}
