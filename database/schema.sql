-- =====================================================================
-- CodeRank Platform - Relational Database Schema
-- Advanced Data Structures & Competitive Programming Contest System
-- =====================================================================

DROP TABLE IF EXISTS submissions;
DROP TABLE IF EXISTS team_members;
DROP TABLE IF EXISTS teams;
DROP TABLE IF EXISTS problems;
DROP TABLE IF EXISTS users;

-- ---------------------------------------------------------------------
-- 1. Users Table
-- ---------------------------------------------------------------------
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username VARCHAR(50) NOT NULL UNIQUE,
    full_name VARCHAR(100) NOT NULL,
    score INTEGER NOT NULL DEFAULT 0,
    rating INTEGER NOT NULL DEFAULT 1500,
    rank_tier VARCHAR(30) NOT NULL DEFAULT 'Specialist',
    team_id INTEGER DEFAULT NULL,
    country VARCHAR(50) DEFAULT 'International',
    solved_count INTEGER NOT NULL DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (team_id) REFERENCES teams(id) ON DELETE SET NULL
);

-- Index for ranking queries (mirrors Treap index)
CREATE INDEX idx_users_score ON users(score DESC, id ASC);
CREATE INDEX idx_users_username ON users(username);

-- ---------------------------------------------------------------------
-- 2. Teams Table (Managed by Disjoint Set Union)
-- ---------------------------------------------------------------------
CREATE TABLE teams (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(100) NOT NULL UNIQUE,
    institution VARCHAR(150) DEFAULT 'Independent',
    total_score INTEGER NOT NULL DEFAULT 0,
    member_count INTEGER NOT NULL DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_teams_score ON teams(total_score DESC, id ASC);

-- ---------------------------------------------------------------------
-- 3. Problems Table (Managed by AVL Tree & Trie)
-- ---------------------------------------------------------------------
CREATE TABLE problems (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(150) NOT NULL,
    difficulty INTEGER NOT NULL,
    category VARCHAR(50) NOT NULL,
    time_limit_ms INTEGER NOT NULL DEFAULT 1000,
    memory_limit_mb INTEGER NOT NULL DEFAULT 256,
    tags TEXT DEFAULT '',
    statement TEXT DEFAULT '',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Index for difficulty range queries (mirrors AVL tree order)
CREATE INDEX idx_problems_difficulty ON problems(difficulty ASC, id ASC);
CREATE INDEX idx_problems_name ON problems(name);

-- ---------------------------------------------------------------------
-- 4. Submissions Table (Managed by Binomial Heap Queue)
-- ---------------------------------------------------------------------
CREATE TABLE submissions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    problem_id INTEGER NOT NULL,
    contest_time_min INTEGER NOT NULL DEFAULT 0,
    verdict VARCHAR(30) NOT NULL DEFAULT 'Pending',
    language VARCHAR(30) NOT NULL DEFAULT 'C++17',
    code TEXT NOT NULL,
    is_frozen BOOLEAN NOT NULL DEFAULT 0,
    points_awarded INTEGER NOT NULL DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (problem_id) REFERENCES problems(id) ON DELETE CASCADE
);

-- Index for chronological judging priority (mirrors Binomial Min-Heap order)
CREATE INDEX idx_submissions_priority ON submissions(contest_time_min ASC, id ASC);
CREATE INDEX idx_submissions_user ON submissions(user_id);
CREATE INDEX idx_submissions_problem ON submissions(problem_id);

-- ---------------------------------------------------------------------
-- 5. Views for Leaderboards and Analytics
-- ---------------------------------------------------------------------
CREATE VIEW vw_live_leaderboard AS
SELECT 
    RANK() OVER (ORDER BY score DESC, id ASC) as rank,
    id as user_id,
    username,
    score,
    team_id,
    rank_tier,
    solved_count
FROM users;

CREATE VIEW vw_team_standings AS
SELECT
    RANK() OVER (ORDER BY COALESCE(SUM(u.score), 0) DESC, t.id ASC) as rank,
    t.id as team_id,
    t.name as team_name,
    COALESCE(SUM(u.score), 0) as calculated_team_score,
    COUNT(u.id) as current_members
FROM teams t
LEFT JOIN users u ON u.team_id = t.id
GROUP BY t.id, t.name;
