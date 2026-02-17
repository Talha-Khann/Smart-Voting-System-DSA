# 🗳️ Smart Voting System — NA-46 Islamabad-I

> Electronic Voting Machine (EVM) simulation built as a Data Structures & Algorithms project.  
> **C++ Backend Server** ↔ **HTML/CSS/JS Frontend**

---

## 📌 Project Overview

This project simulates a real-world **Electronic Voting Machine (EVM)** for Constituency **NA-46 Islamabad-I**, Pakistan. It features a live C++ backend server that handles voter authentication, vote casting, and result computation using core DSA concepts — all connected to a modern browser-based frontend.

---

## 🎯 Features

| Feature | Description |
|---|---|
| 🔐 Voter Authentication | Voters are verified by CNIC + Full Name before voting |
| 🗳️ Vote Casting | Cast vote for PTI, PML-N, PPP, or Independent |
| 📊 Live Status | Real-time vote count and turnout per polling station |
| 🏆 Results Dashboard | Shows winner, vote counts, and bar chart visualization |
| 🔁 Undo Vote | Stack-based undo of the last cast vote |
| 🌐 REST API Backend | C++ HTTP server with CORS support for browser connectivity |
| 9 Polling Stations | G-8, G-9, G-10, F-8, F-9, F-10, I-8, I-9, I-10 |
| 180 Pre-registered Voters | 20 voters per station, loaded on server startup |

---

## 🧠 Data Structures Used

| DSA Concept | Where It's Used |
|---|---|
| **Linked List** | Each polling station has a singly linked list of registered voters |
| **AVL Tree (Self-Balancing BST)** | Fast O(log n) voter lookup by CNIC/Voter ID |
| **Stack** | Stores vote history for undo functionality |
| **Queue** | Used internally for vote processing flow |
| **Arrays** | Stores vote counts per station per party `voteCount[9][4]` |

---

## 🏗️ Project Structure

```
dsaproject/
│
├── backend/
│   ├── server.cpp          # Main HTTP server (REST API using cpp-httplib)
│   ├── dsa project.cpp     # All DSA logic: AVL Tree, Linked List, Stack, voter data
│   ├── voting_system.h     # Header declarations
│   └── httplib.h           # Single-header C++ HTTP library (cpp-httplib)
│
├── Frontend/
│   ├── index.html          # Full EVM UI with 5 screens
│   ├── style.css           # Styling with animations
│   └── script.js           # Fetch API calls to C++ backend
```

---

## 🔌 API Endpoints

The C++ server runs at `http://127.0.0.1:5000`

| Method | Endpoint | Description |
|---|---|---|
| `GET` | `/test` | Check if server is online |
| `POST` | `/vote` | Cast a vote (params: voterID, voterName, partyIndex, stationIndex) |
| `GET` | `/live_status?stationIndex=N` | Get turnout stats for a station |
| `GET` | `/results?stationIndex=N` | Get vote counts and current leader |

---

## ⚙️ How to Run

### Prerequisites
- Windows OS
- **MinGW g++ compiler** installed ([Download here](https://www.mingw-w64.org/))
- A modern web browser (Chrome, Edge, Firefox)

---

### Step 1 — Navigate to Backend Folder

Open **PowerShell** or **Command Prompt** and run:

```powershell
cd "C:\path\to\dsaproject\backend"
```

---

### Step 2 — Compile the C++ Server

```powershell
g++ -o server "server.cpp" -lws2_32 -lmswsock -ladvapi32 -std=c++17
```

✅ No output = Compiled successfully!

---

### Step 3 — Start the Server

```powershell
.\server.exe
```

You should see:
```
------------------------------------------
  SMART VOTING BACKEND IS RUNNING
  Constituency: NA-46 Islamabad-I
  Endpoint: http://127.0.0.1:5000
------------------------------------------
```

---

### Step 4 — Open the Frontend

Simply double-click `Frontend/index.html` in File Explorer — it opens in your browser and connects automatically to the backend.

> ⚠️ Keep the PowerShell/CMD window open while using the app. Closing it shuts down the server.

---

## 🖥️ Frontend Screens

| Screen | Description |
|---|---|
| **Connect Screen** | Checks if C++ backend is online |
| **Welcome / Station Select** | Choose your polling station |
| **Main Menu** | Navigate to Cast Vote, Live Status, or Results |
| **Ballot Paper** | Enter CNIC + Name, select party symbol, confirm vote |
| **Data Dashboard** | Bar chart + leaderboard showing vote counts |

---

## 🔐 Sample Voter Credentials (for testing)

| Station | Voter ID | Name |
|---|---|---|
| G-8 (Index 0) | 101 | Talha Khan |
| G-8 (Index 0) | 102 | Ahsan Malik |
| G-9 (Index 1) | 201 | Usman Khan |
| G-10 (Index 2) | 301 | Talha Abbasi |
| F-8 (Index 3) | 401 | Bilal Khan |
| I-8 (Index 6) | 701 | Ali Khan |

---

## 🔄 How It All Connects

```
[Browser: index.html]
       |
       | fetch() HTTP requests
       ↓
[C++ server.cpp — port 5000]
       |
       | calls DSA functions
       ↓
[dsa project.cpp]
  ├── generateVoters()     → fills Linked Lists + AVL Trees
  ├── authenticateVoter()  → searches AVL Tree O(log n)
  ├── voteCount[][]        → 2D array updated on vote
  └── voteHistory stack    → enables undo
```

---

## 👥 Authors

> BS Software Engineering — DSA Course Project  
> Constituency: NA-46 Islamabad-I  
> System: Electronic Voting Machine (EVM) Simulation
>Talha Khan
---

## 📄 License

This project is for **educational purposes only** as part of a university Data Structures & Algorithms course.
