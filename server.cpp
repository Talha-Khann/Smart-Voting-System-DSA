#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include "httplib.h"
#include <iostream>
#include <string>
#include "dsa project.cpp" // Ensure this file defines stations[9], voteCount[9][4], etc.

using namespace httplib;
using namespace std;

int main() {
    Server svr;

    // --- CORS & OPTIONS HANDLER (Required for Browser Security) ---
    svr.Options(R"(.*)", [](const Request&, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 200;
    });

    // --- 1. TEST CONNECTION ---
    svr.Get("/test", [](const Request&, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Secure Server Connected", "text/plain");
    });

    // --- 2. VOTE ENDPOINT ---
    svr.Post("/vote", [](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        // Get parameters from the JS fetch call
        string vID = req.get_param_value("voterID");
        string vName = req.get_param_value("voterName");
        int pIdx = stoi(req.get_param_value("partyIndex"));
        int sIdx = stoi(req.get_param_value("stationIndex"));

        cout << "Login Attempt: " << vName << " at Station " << sIdx << endl;

        // Use your DSA logic to check voter
        VoterNode* voter = authenticateVoter(sIdx, vID, vName);

        if (!voter) {
            res.set_content("Error: ID not found at this station or already voted.", "text/plain");
            return;
        }

        // Cast the vote in DSA structures
        voter->hasVoted = true;
        voteCount[sIdx][pIdx]++;
        voteHistory.push({voter->voterID, pIdx});

        cout << "SUCCESS: Vote cast for Party " << pIdx << " at Station " << sIdx << endl;
        res.set_content("Vote successfully cast! Thank you for participating.", "text/plain");
    });

    // --- 3. LIVE STATUS ENDPOINT ---
    svr.Get("/live_status", [](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        
        int sIdx = stoi(req.get_param_value("stationIndex"));
        
        int total = 0, voted = 0;
        VoterNode* ptr = stationLists[sIdx];
        while (ptr) { 
            total++; 
            if (ptr->hasVoted) voted++; 
            ptr = ptr->next; 
        }

        string result = "STATION: " + stations[sIdx] + "\n" +
                        "--------------------------\n" +
                        "Registered Voters: " + to_string(total) + "\n" +
                        "Votes Cast:       " + to_string(voted) + "\n" +
                        "Remaining:        " + to_string(total - voted);
        
        res.set_content(result, "text/plain");
    });

    // --- 4. RESULTS ENDPOINT ---
    svr.Get("/results", [](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        
        int sIdx = stoi(req.get_param_value("stationIndex"));
        
        string result = "OFFICIAL RESULTS: " + stations[sIdx] + "\n";
        result += "==========================\n";
        
        int winnerIdx = 0;
        int maxVotes = -1;

        for (int i = 0; i < NUM_PARTIES; i++) {
            result += parties[i] + ": " + to_string(voteCount[sIdx][i]) + " votes\n";
            if(voteCount[sIdx][i] > maxVotes) {
                maxVotes = voteCount[sIdx][i];
                winnerIdx = i;
            }
        }
        
        result += "--------------------------\n";
        result += "Current Leader: " + parties[winnerIdx];
        
        res.set_content(result, "text/plain");
    });

    // --- STARTUP ---
    generateVoters(); // Your DSA function to fill the linked lists
    cout << "------------------------------------------" << endl;
    cout << "  SMART VOTING BACKEND IS RUNNING" << endl;
    cout << "  Constituency: NA-46 Islamabad-I" << endl;
    cout << "  Endpoint: http://127.0.0.1:5000" << endl;
    cout << "------------------------------------------" << endl;

    svr.listen("0.0.0.0", 5000);

    return 0;
}