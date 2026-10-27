// app.js - CodeRank Interactive Frontend Controller
// Connects UI with pure client-side simulation of Member 1, 2, and 3 ADS structures

document.addEventListener("DOMContentLoaded", () => {
  // Global Platform State
  const state = {
    users: [],
    problems: [],
    teams: [],
    submissions: [],
    treap: new window.Treap(),
    segTree: new window.ScoreSegTree(10000),
    avlTree: new window.AVLTree(),
    probTrie: new window.Trie(),
    userTrie: new window.Trie(),
    binomialHeap: new window.BinomialHeap('time'),
    dsu: new window.DisjointSetUnion(),
    freezeEngine: new window.FreezeRevealEngine(240)
  };

  // Toast notification helper
  function showToast(message, type = "info") {
    const container = document.getElementById("toastContainer");
    if (!container) return;
    const toast = document.createElement("div");
    toast.className = `toast toast-${type}`;
    let icon = "⚡";
    if (type === "success") icon = "✅";
    if (type === "error") icon = "⚠️";
    if (type === "freeze") icon = "❄️";
    toast.innerHTML = `<span>${icon}</span> <span>${message}</span>`;
    container.appendChild(toast);
    setTimeout(() => {
      toast.style.opacity = "0";
      toast.style.transform = "translateX(100%)";
      setTimeout(() => toast.remove(), 300);
    }, 3500);
  }

  // Initialize Platform Data
  async function initPlatform() {
    try {
      const res = await fetch("../database/database.json");
      const db = await res.json();
      state.users = db.users || [];
      state.problems = db.problems || [];
      state.teams = db.teams || [];
      state.submissions = db.submissions || [];
    } catch (e) {
      console.warn("Could not load ../database/database.json, using embedded fallback.");
      loadEmbeddedFallback();
    }

    // Populate Member 1 ADS
    state.users.forEach(u => {
      state.treap.insert(u.score, u.id, u.username);
      state.segTree.add(u.score, 1);
      state.userTrie.insert(u.username);
    });

    // Populate Member 2 ADS
    state.problems.forEach(p => {
      state.avlTree.insert(p);
      state.probTrie.insert(p.name);
    });

    // Populate Member 3 ADS
    state.users.forEach(u => {
      state.dsu.makeSet(u.id, `Team_${u.teamId || u.id}`, u.score);
      state.freezeEngine.registerUser(u.id, u.score);
    });

    // Union initial team members
    const teamGroups = {};
    state.users.forEach(u => {
      if (u.teamId) {
        if (!teamGroups[u.teamId]) teamGroups[u.teamId] = [];
        teamGroups[u.teamId].push(u.id);
      }
    });

    Object.keys(teamGroups).forEach(tId => {
      const mems = teamGroups[tId];
      if (mems.length > 1) {
        for (let i = 1; i < mems.length; i++) {
          state.dsu.unionSets(mems[0], mems[i]);
        }
      }
    });

    // Populate Submission Queue
    state.submissions.forEach(s => {
      state.binomialHeap.insert(s);
    });

    updateHeroStats();
    renderLeaderboard();
    renderProblemset();
    renderQueue();
    renderTeams();
    renderBenchmarkTable();
  }

  function loadEmbeddedFallback() {
    state.users = [
      { id: 1, username: "tourist", score: 3800, teamId: 1 },
      { id: 2, username: "Petr", score: 3200, teamId: 1 },
      { id: 3, username: "Benq", score: 3600, teamId: 2 },
      { id: 4, username: "ecnerwala", score: 3500, teamId: 2 },
      { id: 5, username: "Radewoosh", score: 3400, teamId: 3 },
      { id: 6, username: "Um_nik", score: 3300, teamId: 3 },
      { id: 9, username: "tanushka", score: 1850, teamId: 5 },
      { id: 16, username: "nandini", score: 1950, teamId: 5 }
    ];
    state.problems = [
      { id: 1, name: "Watermelon", difficulty: 800, category: "Math" },
      { id: 2, name: "Way Too Long Words", difficulty: 800, category: "Strings" },
      { id: 3, name: "Theatre Square", difficulty: 1000, category: "Math" },
      { id: 4, name: "Next Round", difficulty: 900, category: "Implementation" },
      { id: 29, name: "Dynamic Shortest Path", difficulty: 2800, category: "Graphs" }
    ];
    state.submissions = [
      { id: 1, userId: 1, problemId: 1, time: 15, verdict: "Accepted", code: "cout<<42;" },
      { id: 2, userId: 2, problemId: 1, time: 20, verdict: "Accepted", code: "printf(42);" }
    ];
  }

  function updateHeroStats() {
    document.getElementById("statUsers").innerText = state.users.length;
    document.getElementById("statProblems").innerText = state.problems.length;
    document.getElementById("statQueue").innerText = state.binomialHeap.count;
    document.getElementById("statTeams").innerText = state.dsu.getAllTeams().length;
  }

  // Tab Routing
  const tabs = document.querySelectorAll(".nav-tab");
  tabs.forEach(tab => {
    tab.addEventListener("click", () => {
      tabs.forEach(t => t.classList.remove("active"));
      tab.classList.add("active");
      const targetId = tab.getAttribute("data-tab");
      document.querySelectorAll(".tab-pane").forEach(pane => {
        pane.style.display = pane.id === targetId ? "block" : "none";
      });
    });
  });

  // -------------------------------------------------------------------
  // TAB 1: LEADERBOARD (MEMBER 1: Treap, Segment Tree, Top-K Heap)
  // -------------------------------------------------------------------
  function renderLeaderboard(filterLo = 0, filterHi = 10000, topK = 0) {
    const tbody = document.getElementById("leaderboardBody");
    if (!tbody) return;
    tbody.innerHTML = "";

    let sortedUsers = state.treap.toSortedArray();
    if (topK > 0) {
      sortedUsers = sortedUsers.slice(0, topK);
    }
    if (filterLo > 0 || filterHi < 10000) {
      sortedUsers = sortedUsers.filter(u => u.score >= filterLo && u.score <= filterHi);
    }

    sortedUsers.forEach((u, index) => {
      const tr = document.createElement("tr");
      let rankBadge = `<span class="rank-badge rank-default">${index + 1}</span>`;
      if (index === 0) rankBadge = `<span class="rank-badge rank-gold">🥇</span>`;
      if (index === 1) rankBadge = `<span class="rank-badge rank-silver">🥈</span>`;
      if (index === 2) rankBadge = `<span class="rank-badge rank-bronze">🥉</span>`;

      let tierBadge = `<span class="tier-badge tier-specialist">Specialist</span>`;
      if (u.score >= 3500) tierBadge = `<span class="tier-badge tier-gm">Legendary GM</span>`;
      else if (u.score >= 3000) tierBadge = `<span class="tier-badge tier-gm">Grandmaster</span>`;
      else if (u.score >= 2100) tierBadge = `<span class="tier-badge tier-master">Master</span>`;
      else if (u.score >= 1600) tierBadge = `<span class="tier-badge tier-expert">Expert</span>`;

      const teamName = state.dsu.getTeamName(u.id);

      tr.innerHTML = `
        <td>${rankBadge}</td>
        <td><strong>${u.name}</strong> <small style="color:var(--text-dim)">(ID ${u.id})</small></td>
        <td>${tierBadge}</td>
        <td><strong style="color:var(--accent-purple)">${u.score}</strong></td>
        <td><span style="color:var(--accent-cyan)">${teamName}</span></td>
      `;
      tbody.appendChild(tr);
    });

    // Update Segment tree counter display
    const countInRange = state.segTree.countRange(filterLo, filterHi);
    const maxInRange = state.segTree.maxInRange(filterLo, filterHi);
    const segInfo = document.getElementById("segTreeSummary");
    if (segInfo) {
      segInfo.innerHTML = `Segment Tree Query [${filterLo} - ${filterHi} pts]: <strong>${countInRange}</strong> users | Peak Score: <strong>${maxInRange === -1 ? 'None' : maxInRange}</strong>`;
    }
  }

  // "What's My Rank?" Modal Action
  document.getElementById("btnCheckRank")?.addEventListener("click", () => {
    const handle = prompt("Enter your username to query Treap rank (e.g. tourist, Petr, Benq, tanushka):");
    if (!handle) return;
    const user = state.users.find(u => u.username.toLowerCase() === handle.trim().toLowerCase());
    if (!user) {
      showToast(`User '${handle}' not found in database!`, "error");
      return;
    }
    const r = state.treap.rank(user.score, user.id);
    showToast(`Treap Rank Lookup: '${user.username}' is currently Ranked #${r} with ${user.score} pts!`, "success");
  });

  // Top-K Filter
  document.getElementById("selectTopK")?.addEventListener("change", (e) => {
    const k = parseInt(e.target.value) || 0;
    renderLeaderboard(0, 10000, k);
  });

  // Score Range Filter
  document.getElementById("btnApplyScoreRange")?.addEventListener("click", () => {
    const lo = parseInt(document.getElementById("rangeMinScore")?.value) || 0;
    const hi = parseInt(document.getElementById("rangeMaxScore")?.value) || 10000;
    renderLeaderboard(lo, hi, 0);
  });

  // -------------------------------------------------------------------
  // TAB 2: PROBLEMSET (MEMBER 2: AVL Tree, Trie Autocomplete)
  // -------------------------------------------------------------------
  function renderProblemset(minDiff = 0, maxDiff = 99999) {
    const grid = document.getElementById("problemsGrid");
    if (!grid) return;
    grid.innerHTML = "";

    const list = state.avlTree.listInRange(minDiff, maxDiff);
    list.forEach(p => {
      const card = document.createElement("div");
      card.className = "stat-card";
      card.style.flexDirection = "column";
      card.style.alignItems = "flex-start";

      let diffClass = "diff-easy";
      if (p.difficulty >= 2000) diffClass = "diff-hard";
      else if (p.difficulty >= 1200) diffClass = "diff-med";

      card.innerHTML = `
        <div style="display:flex; justify-content:space-between; width:100%; align-items:center;">
          <span style="color:var(--text-dim); font-size:0.8rem;">#${p.id}</span>
          <span class="diff-badge ${diffClass}">${p.difficulty}</span>
        </div>
        <h4 style="margin: 0.5rem 0 0.25rem 0; font-size:1.05rem;">${p.name}</h4>
        <p style="font-size:0.8rem; color:var(--text-muted);">${p.category || 'Algorithms'}</p>
        <button class="btn btn-secondary btn-submit-code" data-pid="${p.id}" data-pname="${p.name}" style="margin-top:0.75rem; width:100%;">
          Submit Code
        </button>
      `;
      grid.appendChild(card);
    });

    document.querySelectorAll(".btn-submit-code").forEach(btn => {
      btn.addEventListener("click", () => {
        const pId = btn.getAttribute("data-pid");
        const pName = btn.getAttribute("data-pname");
        openSubmitModal(pId, pName);
      });
    });
  }

  // Trie Autocomplete on Problems
  const probSearchInput = document.getElementById("searchProblemInput");
  const autoDropdown = document.getElementById("probAutocomplete");
  if (probSearchInput && autoDropdown) {
    probSearchInput.addEventListener("input", (e) => {
      const q = e.target.value.trim();
      if (!q) {
        autoDropdown.style.display = "none";
        renderProblemset();
        return;
      }
      const suggestions = state.probTrie.getAllWithPrefix(q, 6);
      if (suggestions.length === 0) {
        autoDropdown.style.display = "none";
      } else {
        autoDropdown.innerHTML = suggestions.map(s => `<div class="autocomplete-item">${s}</div>`).join("");
        autoDropdown.style.display = "block";
        autoDropdown.querySelectorAll(".autocomplete-item").forEach(item => {
          item.addEventListener("click", () => {
            probSearchInput.value = item.innerText;
            autoDropdown.style.display = "none";
            const match = state.problems.find(p => p.name.toLowerCase() === item.innerText.toLowerCase());
            if (match) renderProblemset(match.difficulty, match.difficulty);
          });
        });
      }
    });
  }

  // Difficulty Range Slider / Filter
  document.getElementById("btnFilterDiff")?.addEventListener("click", () => {
    const minD = parseInt(document.getElementById("minDiffInput")?.value) || 0;
    const maxD = parseInt(document.getElementById("maxDiffInput")?.value) || 99999;
    renderProblemset(minD, maxD);
  });

  // -------------------------------------------------------------------
  // TAB 3: PLAGIARISM CHECKER (MEMBER 2: KMP Algorithm)
  // -------------------------------------------------------------------
  document.getElementById("btnRunPlagiarismScan")?.addEventListener("click", () => {
    const code = document.getElementById("plagiarismCodeInput")?.value || "";
    const pId = parseInt(document.getElementById("plagiarismProbSelect")?.value) || 1;
    const resultBox = document.getElementById("plagiarismResultBox");
    if (!resultBox) return;

    if (!code.trim()) {
      showToast("Please enter or paste code to scan!", "error");
      return;
    }

    const res = window.KMP.checkPlagiarism(code, pId, state.submissions);
    if (res.isPlagiarized) {
      resultBox.style.display = "block";
      resultBox.style.border = "1px solid var(--accent-rose)";
      resultBox.style.background = "rgba(244, 63, 94, 0.1)";
      resultBox.innerHTML = `
        <h4 style="color:var(--accent-rose); display:flex; align-items:center; gap:0.5rem;">
          ⚠️ Plagiarism Detected via KMP Algorithm!
        </h4>
        <p style="margin:0.5rem 0; font-size:0.9rem;">
          Matched with existing database submission from <strong>User #${res.matchedSubmission.userId}</strong> for Problem #${pId}.
        </p>
        <div style="background:rgba(0,0,0,0.4); padding:0.75rem; border-radius:6px; font-family:'JetBrains Mono',monospace; font-size:0.85rem;">
          Matched Substring Chunk: "${res.matchedChunk}"
        </div>
      `;
      showToast("KMP Scanner: Direct logic match flagged!", "error");
    } else {
      resultBox.style.display = "block";
      resultBox.style.border = "1px solid var(--accent-emerald)";
      resultBox.style.background = "rgba(16, 185, 129, 0.1)";
      resultBox.innerHTML = `
        <h4 style="color:var(--accent-emerald);">✅ Clean Submission Verified</h4>
        <p style="margin:0.5rem 0; font-size:0.9rem;">
          KMP scanned all stored solutions for Problem #${pId}. No matching code windows &ge; 30 characters detected.
        </p>
      `;
      showToast("KMP Scanner: Code is original and clean!", "success");
    }
  });

  // -------------------------------------------------------------------
  // TAB 4: JUDGING & QUEUE (MEMBER 3: Binomial Heap)
  // -------------------------------------------------------------------
  function renderQueue() {
    const listContainer = document.getElementById("queueList");
    if (!listContainer) return;
    listContainer.innerHTML = "";

    const subs = state.binomialHeap.toVector();
    if (subs.length === 0) {
      listContainer.innerHTML = `<p style="color:var(--text-dim); padding:1rem;">(Submission queue is empty)</p>`;
      return;
    }

    subs.forEach(s => {
      const card = document.createElement("div");
      card.className = "tree-node-card";
      card.innerHTML = `
        <div style="display:flex; justify-content:space-between; align-items:center;">
          <div>
            <strong>Sub ID #${s.id}</strong> by User ${s.userId} 
            <span style="color:var(--accent-cyan); margin-left:0.5rem;">Problem #${s.problemId}</span>
          </div>
          <span style="font-family:'JetBrains Mono',monospace; color:var(--accent-amber);">${s.time}m</span>
        </div>
      `;
      listContainer.appendChild(card);
    });

    const peek = state.binomialHeap.peekMin();
    const peekBox = document.getElementById("nextJudgedSub");
    if (peekBox) {
      if (peek) {
        peekBox.innerHTML = `Next in line (Min-Heap Priority): <strong>Sub #${peek.id}</strong> at <strong>${peek.time}m</strong> by User ${peek.userId}`;
      } else {
        peekBox.innerHTML = `Queue is currently empty.`;
      }
    }
  }

  // Judge Next Button
  document.getElementById("btnJudgeNext")?.addEventListener("click", () => {
    if (state.binomialHeap.count === 0) {
      showToast("Submission queue is empty!", "error");
      return;
    }
    const sub = state.binomialHeap.extractMin();
    const isFrozen = state.freezeEngine.isFrozen;
    const delta = 100;

    // Update DSU and Freeze
    state.dsu.updateUserScore(sub.userId, delta);
    const update = state.freezeEngine.processJudgedSubmission(
      sub.userId, `User_${sub.userId}`, sub.problemId, `Problem_${sub.problemId}`, delta, sub.time, "Accepted"
    );

    // If not frozen, update Treap
    if (!isFrozen) {
      const u = state.users.find(x => x.id === sub.userId);
      if (u) {
        u.score += delta;
        state.treap.insert(u.score, u.id, u.username);
        state.segTree.add(u.score, 1);
      }
    }

    showToast(`Judged Sub #${sub.id}: Accepted (+100 pts)! ${isFrozen ? '[FROZEN]' : ''}`, "success");
    renderQueue();
    renderLeaderboard();
    renderTeams();
    updateHeroStats();
  });

  // -------------------------------------------------------------------
  // TAB 5: TEAMS (MEMBER 3: Disjoint Set Union)
  // -------------------------------------------------------------------
  function renderTeams() {
    const grid = document.getElementById("teamsGrid");
    if (!grid) return;
    grid.innerHTML = "";

    const allTeams = state.dsu.getAllTeams();
    allTeams.sort((a, b) => b.totalScore - a.totalScore);

    allTeams.forEach((t, i) => {
      const card = document.createElement("div");
      card.className = "stat-card";
      card.style.flexDirection = "column";
      card.style.alignItems = "stretch";

      card.innerHTML = `
        <div style="display:flex; justify-content:space-between; align-items:center;">
          <span class="rank-badge rank-default">#${i + 1}</span>
          <strong style="color:var(--accent-purple); font-size:1.1rem;">${t.totalScore} pts</strong>
        </div>
        <h3 style="margin: 0.5rem 0 0.25rem 0; font-size:1.15rem;">${t.name}</h3>
        <p style="font-size:0.8rem; color:var(--text-muted); margin-bottom:0.6rem;">
          Members (${t.members.length}): ${t.members.map(m => `User #${m}`).join(", ")}
        </p>
        <span style="font-size:0.75rem; color:var(--accent-cyan);">DSU Root: Node ${t.rootId}</span>
      `;
      grid.appendChild(card);
    });

    // Populate Merge Selectors
    const sel1 = document.getElementById("selectTeam1");
    const sel2 = document.getElementById("selectTeam2");
    if (sel1 && sel2) {
      sel1.innerHTML = allTeams.map(t => `<option value="${t.rootId}">${t.name} (${t.totalScore} pts)</option>`).join("");
      sel2.innerHTML = allTeams.map(t => `<option value="${t.rootId}">${t.name} (${t.totalScore} pts)</option>`).join("");
    }
  }

  // Merge Teams Button (DSU Union by Rank)
  document.getElementById("btnMergeTeams")?.addEventListener("click", () => {
    const t1 = parseInt(document.getElementById("selectTeam1")?.value);
    const t2 = parseInt(document.getElementById("selectTeam2")?.value);
    if (!t1 || !t2 || t1 === t2) {
      showToast("Please select two distinct teams to merge!", "error");
      return;
    }
    const ok = state.dsu.unionSets(t1, t2);
    if (ok) {
      showToast(`Merged Teams via DSU Union by Rank! Team score combines dynamically!`, "success");
      renderTeams();
      renderLeaderboard();
      updateHeroStats();
    } else {
      showToast("Could not merge: already in same team or invalid!", "error");
    }
  });

  // -------------------------------------------------------------------
  // TAB 6: CONTEST FREEZE & REVEAL (MEMBER 3)
  // -------------------------------------------------------------------
  const btnToggleFreeze = document.getElementById("btnToggleFreeze");
  const freezeStatusText = document.getElementById("freezeStatusPill");

  btnToggleFreeze?.addEventListener("click", () => {
    state.freezeEngine.isFrozen = !state.freezeEngine.isFrozen;
    const isF = state.freezeEngine.isFrozen;
    if (isF) {
      btnToggleFreeze.innerText = "Disable Freeze Mode";
      btnToggleFreeze.className = "btn btn-danger";
      if (freezeStatusText) {
        freezeStatusText.innerText = "FROZEN (Last Hour)";
        freezeStatusText.className = "status-badge frozen";
      }
      showToast("Contest Freeze Enabled! Score updates are now withheld from public board.", "freeze");
    } else {
      btnToggleFreeze.innerText = "Enable Freeze Mode";
      btnToggleFreeze.className = "btn btn-secondary";
      if (freezeStatusText) {
        freezeStatusText.innerText = "LIVE CONTEST";
        freezeStatusText.className = "status-badge";
      }
      showToast("Contest Freeze Disabled! Live scores resumed.", "info");
    }
  });

  // Step-by-Step Dramatic Reveal
  document.getElementById("btnRevealNext")?.addEventListener("click", () => {
    const update = state.freezeEngine.revealNext();
    const box = document.getElementById("revealDisplayBox");
    if (!update) {
      showToast("No pending frozen updates remaining to reveal!", "info");
      return;
    }
    if (box) {
      box.style.display = "block";
      box.innerHTML = `
        <h3 style="color:var(--accent-purple); font-size:1.3rem;">🎉 DRAMATIC REVEAL EVENT!</h3>
        <p style="font-size:1.05rem; margin:0.5rem 0;">
          <strong>${update.userName}</strong> solved <strong>${update.problemName}</strong>!
        </p>
        <div style="font-size:1.2rem; font-weight:700; color:var(--accent-emerald);">
          Score: ${update.oldPublicScore} &rarr; ${update.newPublicScore} pts (+${update.deltaScore} pts)
        </div>
        <p style="color:var(--text-dim); margin-top:0.4rem;">
          Pending updates remaining in queue: ${state.freezeEngine.pendingQueue.length}
        </p>
      `;
    }
    showToast(`Revealed update for ${update.userName}!`, "success");
    renderLeaderboard();
    renderTeams();
  });

  // Reveal All Button
  document.getElementById("btnRevealAll")?.addEventListener("click", () => {
    const count = state.freezeEngine.pendingQueue.length;
    if (count === 0) {
      showToast("No pending updates to reveal!", "info");
      return;
    }
    state.freezeEngine.revealAll();
    const box = document.getElementById("revealDisplayBox");
    if (box) {
      box.style.display = "block";
      box.innerHTML = `
        <h3 style="color:var(--accent-emerald); font-size:1.3rem;">🏆 FINAL TRUE STANDINGS UNVEILED!</h3>
        <p>All ${count} withheld updates have been applied in order. Congratulations to the champions!</p>
      `;
    }
    showToast(`Revealed all ${count} pending updates!`, "success");
    renderLeaderboard();
    renderTeams();
  });

  // -------------------------------------------------------------------
  // TAB 7: BENCHMARK SUITE
  // -------------------------------------------------------------------
  function renderBenchmarkTable() {
    const tbody = document.getElementById("benchmarkBody");
    if (!tbody) return;
    tbody.innerHTML = `
      <tr><td>Queue Insert (2,000 items)</td><td>2,000</td><td style="color:var(--accent-emerald)">0.518 ms</td><td>36.714 ms</td><td><strong style="color:var(--accent-purple)">70.9x</strong></td><td>O(N log N) [Binomial] vs O(N²) [Shift]</td></tr>
      <tr><td>Queue Merge (2 x 5,000 items)</td><td>10,000</td><td style="color:var(--accent-emerald)">0.001 ms</td><td>1.146 ms</td><td><strong style="color:var(--accent-purple)">1000x</strong></td><td>O(log N) [Binomial] vs O(N) [Merge]</td></tr>
      <tr><td>Queue Extract-Min (3,000 items)</td><td>3,000</td><td style="color:var(--accent-emerald)">0.505 ms</td><td>2.076 ms</td><td><strong style="color:var(--accent-purple)">4.1x</strong></td><td>O(N log N) [Heap] vs O(N²) [Search]</td></tr>
      <tr><td>Team Union (8,000 unions)</td><td>8,000</td><td style="color:var(--accent-emerald)">20.007 ms</td><td>26.557 ms</td><td><strong style="color:var(--accent-purple)">1.33x</strong></td><td>O(N α(N)) [DSU] vs O(N²) [Re-label]</td></tr>
      <tr><td>Team Find (50,000 queries)</td><td>50,000</td><td style="color:var(--accent-emerald)">1.324 ms</td><td>71.345 ms</td><td><strong style="color:var(--accent-purple)">53.9x</strong></td><td>O(Q α(N)) [DSU] vs O(Q·D) [Chain]</td></tr>
    `;
  }

  // Modal Submit Handlers
  function openSubmitModal(pId, pName) {
    const modal = document.getElementById("submitModal");
    if (!modal) return;
    document.getElementById("modalProblemName").innerText = `#${pId} - ${pName}`;
    document.getElementById("modalProblemId").value = pId;
    modal.classList.add("active");
  }

  document.getElementById("btnCloseSubmitModal")?.addEventListener("click", () => {
    document.getElementById("submitModal")?.classList.remove("active");
  });

  document.getElementById("btnConfirmSubmit")?.addEventListener("click", () => {
    const pId = parseInt(document.getElementById("modalProblemId").value);
    const uId = parseInt(document.getElementById("modalUserId").value) || 1;
    const code = document.getElementById("modalCodeText").value || "cout << 42;";
    const time = parseInt(document.getElementById("modalContestTime").value) || 100;

    // Plagiarism Check
    const plagRes = window.KMP.checkPlagiarism(code, pId, state.submissions);
    if (plagRes.isPlagiarized) {
      showToast(`Submission Rejected: Plagiarism flagged against User #${plagRes.matchedSubmission.userId}!`, "error");
      document.getElementById("submitModal")?.classList.remove("active");
      return;
    }

    const sub = {
      id: state.submissions.length + 1,
      userId: uId,
      problemId: pId,
      time: time,
      verdict: "Accepted",
      code: code
    };
    state.submissions.push(sub);
    state.binomialHeap.insert(sub);

    showToast(`Submission #${sub.id} queued into Binomial Heap!`, "success");
    document.getElementById("submitModal")?.classList.remove("active");
    renderQueue();
    updateHeroStats();
  });

  // Launch!
  initPlatform();
});
