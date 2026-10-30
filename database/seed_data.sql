-- =====================================================================
-- CodeRank Platform - Seed Data
-- =====================================================================

-- ---------------------------------------------------------------------
-- Seed Teams
-- ---------------------------------------------------------------------
INSERT INTO teams (id, name, institution, total_score, member_count) VALUES
(1, 'RedSector', 'Saint Petersburg ITMO', 7000, 2),
(2, 'MIT_Alums', 'Massachusetts Institute of Technology', 7100, 2),
(3, 'WarsawLegends', 'University of Warsaw', 6700, 2),
(4, 'TokyoTechies', 'The University of Tokyo', 6800, 2),
(5, 'CodeRankTeam', 'Indian Institute of Technology', 3270, 2),
(6, 'ByteBrawlers', 'Carnegie Mellon University', 2850, 2),
(7, 'StanfordTree', 'Stanford University', 3400, 2),
(8, 'SoloStars', 'Independent', 1780, 1);

-- ---------------------------------------------------------------------
-- Seed Users
-- ---------------------------------------------------------------------
INSERT INTO users (id, username, full_name, score, rating, rank_tier, team_id, country, solved_count) VALUES
(1, 'tourist', 'Gennady Korotkevich', 3800, 3950, 'Legendary Grandmaster', 1, 'Belarus', 32),
(2, 'Petr', 'Petr Mitrichev', 3200, 3400, 'International Grandmaster', 1, 'Russia', 28),
(3, 'Benq', 'Benjamin Qi', 3600, 3750, 'Legendary Grandmaster', 2, 'USA', 30),
(4, 'ecnerwala', 'Andrew He', 3500, 3600, 'Legendary Grandmaster', 2, 'USA', 29),
(5, 'Radewoosh', 'Krzysztof Maziarz', 3400, 3500, 'Legendary Grandmaster', 3, 'Poland', 27),
(6, 'Um_nik', 'Alex Danilyuk', 3300, 3450, 'International Grandmaster', 3, 'Russia', 26),
(7, 'maroonrk', 'Riku Kawata', 3450, 3550, 'Legendary Grandmaster', 4, 'Japan', 28),
(8, 'koosaga', 'Jaehyun Koo', 3350, 3450, 'International Grandmaster', 4, 'South Korea', 27),
(9, 'tanushka', 'Tanushka Gulhane', 1850, 1920, 'Candidate Master', 5, 'India', 18),
(10, 'alex_coder', 'Alex Chen', 1420, 1530, 'Specialist', 5, 'Canada', 12),
(11, 'coder_bob', 'Bob Smith', 1200, 1340, 'Pupil', 6, 'UK', 9),
(12, 'alice_algo', 'Alice Walker', 1650, 1720, 'Expert', 6, 'Australia', 15),
(13, 'charlie_ads', 'Charlie Davis', 1900, 1980, 'Candidate Master', 7, 'Germany', 19),
(14, 'david_cpp', 'David Miller', 1500, 1600, 'Specialist', 7, 'France', 13),
(15, 'emma_dev', 'Emma Watson', 1780, 1850, 'Expert', 8, 'UK', 16),
(16, 'nandini', 'Nandini Kharche', 1950, 2010, 'Candidate Master', 5, 'India', 20);

-- ---------------------------------------------------------------------
-- Seed Problems
-- ---------------------------------------------------------------------
INSERT INTO problems (id, name, difficulty, category, time_limit_ms, memory_limit_mb, tags, statement) VALUES
(1, 'Watermelon', 800, 'Math', 1000, 64, 'brute force,math', 'Determine whether a watermelon weighing w kilos can be split into two even integer parts.'),
(2, 'Way Too Long Words', 800, 'Strings', 1000, 256, 'strings', 'Abbreviate words longer than 10 characters by keeping the first and last letters and the count of letters in between.'),
(3, 'Theatre Square', 1000, 'Math', 1000, 256, 'math', 'Calculate the least number of flagstones of size a x a needed to pave the Theatre Square of size n x m.'),
(4, 'Next Round', 900, 'Implementation', 3000, 256, 'implementation', 'Count participants who earned a score equal to or greater than the k-th place finisher with positive score.'),
(5, 'String Task', 1000, 'Strings', 2000, 256, 'strings', 'Delete all vowels, insert a period before each consonant, and write all uppercase consonants in lowercase.'),
(6, 'Bit++', 800, 'Implementation', 1000, 256, 'implementation', 'Execute a program consisting of operations incrementing or decrementing variable X.'),
(7, 'Beautiful Matrix', 800, 'Greedy', 2000, 256, 'implementation,greedy', 'Find minimum moves to bring the single number 1 in a 5x5 matrix to the center at (3,3).'),
(8, 'Petya and Strings', 800, 'Strings', 2000, 256, 'strings', 'Compare two strings lexicographically regardless of case.'),
(9, 'Helpful Maths', 900, 'Greedy', 2000, 256, 'greedy,sorting', 'Rearrange summands in non-decreasing order in an addition statement containing 1, 2, and 3.'),
(10, 'Word Capitalization', 800, 'Strings', 2000, 256, 'strings', 'Capitalize the first letter of a given word.'),
(11, 'Elephant', 900, 'Greedy', 1000, 256, 'greedy,math', 'Calculate minimum steps of lengths 1, 2, 3, 4, or 5 to reach point x from 0.'),
(12, 'Soldier and Bananas', 900, 'Math', 1000, 256, 'math', 'Determine how much money the soldier must borrow to buy w bananas.'),
(13, 'Boy or Girl', 800, 'Strings', 1000, 256, 'strings', 'Determine gender based on whether the number of distinct characters in username is odd or even.'),
(14, 'Anton and Danik', 800, 'Implementation', 1000, 256, 'implementation', 'Determine who won more games in a chess championship given the result string.'),
(15, 'Translation', 800, 'Strings', 1000, 256, 'strings', 'Check whether string t is the exact reverse of string s.'),
(16, 'Registration System', 1300, 'Data Structures', 5000, 64, 'hashing,data structures', 'Implement database username registration with numeric suffix generation for duplicates.'),
(17, 'cAPS lOCK', 1000, 'Implementation', 500, 256, 'implementation', 'Correct accidentally typed words where Caps Lock was toggled inadvertently.'),
(18, 'Twins', 900, 'Greedy', 2000, 256, 'greedy,sortings', 'Take the minimum number of coins such that their sum is strictly greater than the rest.'),
(19, 'Chat room', 1000, 'Strings', 1000, 256, 'strings', 'Determine whether the word "hello" can be formed as a subsequence of a typed greeting.'),
(20, 'Lucky Division', 1000, 'Brute Force', 2000, 256, 'number theory', 'Check if a number is almost lucky (divisible by a lucky number consisting only of 4 and 7).'),
(21, 'K-th Not Divisible by n', 1200, 'Binary Search', 1000, 256, 'binary search,math', 'Find the k-th positive integer not divisible by n in O(1) or O(log k).'),
(22, 'Fence', 1200, 'Sliding Window', 1000, 256, 'data structures', 'Find k consecutive planks of the fence with the minimum sum of heights.'),
(23, 'BerSU Ball', 1200, 'Two Pointers', 1000, 256, 'dp,greedy,two pointers', 'Pair boys and girls with dance skills differing by at most 1 to maximize pairs.'),
(24, 'Vanya and Lanterns', 1200, 'Math', 1000, 256, 'math,sortings', 'Find the minimum light radius d such that the street of length l is fully lit by n lanterns.'),
(25, 'Two Substrings', 1500, 'Strings', 2000, 256, 'strings,greedy', 'Find non-overlapping occurrences of substrings "AB" and "BA" in string s.'),
(26, 'Boredom', 1500, 'Dynamic Programming', 1000, 256, 'dp', 'Choose integers to maximize score where picking x removes all elements equal to x-1 and x+1.'),
(27, 'Cut Ribbon', 1300, 'Dynamic Programming', 2000, 256, 'dp', 'Cut a ribbon of length n into maximum number of pieces of length a, b, or c.'),
(28, 'T-primes', 1300, 'Number Theory', 2000, 256, 'number theory', 'Check whether positive integers have exactly three distinct positive divisors.'),
(29, 'Dynamic Shortest Path', 2800, 'Graphs', 3000, 512, 'graphs,shortest paths', 'Maintain shortest paths in a directed weighted graph subject to dynamic edge weight updates.'),
(30, 'Maximum Flow Min Cut', 2600, 'Network Flow', 4000, 512, 'flow,graphs', 'Compute maximum s-t flow and reconstruct the minimum cut in a capacity network.'),
(31, 'Convex Hull Trick Deluxe', 3200, 'Advanced Data Structures', 3000, 512, 'dp,geometry,data structures', 'Optimize DP transitions using dynamic convex hull trick with non-monotonic slopes.'),
(32, 'Fast Fourier Transform Quest', 3500, 'Polynomials', 5000, 1024, 'fft,math', 'Compute multi-dimensional polynomial convolutions over modular arithmetic fields.');

-- -------------------------------------------------------------
-- Seed Initial Submissions (Contest Queue History)
-- -------------------------------------------------------------
INSERT INTO submissions (id, user_id, problem_id, contest_time_min, verdict, language, points_awarded, is_frozen, code) VALUES
(1, 1, 1, 15, 'Accepted', 'C++17', 100, 0, '#include <iostream>\nusing namespace std;\nint main() {\n    int w; cin >> w;\n    cout << (w > 2 && w % 2 == 0 ? "YES" : "NO");\n}'),
(2, 2, 1, 20, 'Accepted', 'C++17', 100, 0, '#include <stdio.h>\nint main() {\n    int w; scanf("%d", &w);\n    printf((w > 2 && w % 2 == 0) ? "YES\n" : "NO\n");\n}'),
(3, 3, 2, 35, 'Accepted', 'C++17', 100, 0, '#include <iostream>\n#include <string>\nusing namespace std;\nint main() {\n    string s; cin >> s;\n    if(s.length() > 10) cout << s[0] << s.length()-2 << s.back();\n    else cout << s;\n}'),
(4, 4, 3, 40, 'Accepted', 'C++17', 100, 0, '#include <iostream>\nusing namespace std;\nint main() {\n    long long n, m, a;\n    cin >> n >> m >> a;\n    cout << ((n + a - 1) / a) * ((m + a - 1) / a);\n}'),
(5, 5, 29, 120, 'Accepted', 'C++17', 500, 0, '// Dijkstra with Treap priority queue\n// Advanced Graph Solver'),
(6, 6, 30, 150, 'Accepted', 'C++17', 500, 0, '// Dinic Algorithm for Maximum Flow'),
(7, 9, 4, 180, 'Accepted', 'C++17', 100, 0, '#include <iostream>\nusing namespace std;\nint main() { int n, k, a[100]; cin >> n >> k; /* code */ }'),
(8, 16, 5, 200, 'Accepted', 'C++17', 100, 0, '// String Task solution with regex');
