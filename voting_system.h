#ifndef VOTING_SYSTEM_H
#define VOTING_SYSTEM_H

#include <stack>
#include <string>
#include "dsa project.cpp"  // Keep your DSA code intact

// Expose global variables from your DSA project
extern int voteCount[9][4];
extern std::stack<std::pair<std::string,int>> voteHistory;
extern std::string parties[4];
extern std::string stations[9];
extern VoterNode* stationLists[9];

// Expose main functions from your DSA project
void generateVoters();
VoterNode* authenticateVoter(int stationIndex, std::string id, std::string name);

#endif
