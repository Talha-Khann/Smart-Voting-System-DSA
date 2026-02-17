#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

/* ===================== UTILITY ===================== */
string intToStr(int n) {
    return to_string(n);  // Fixed: using C++11's to_string
}

/* ===================== VOTER STRUCTURES ===================== */

struct VoterNode {
    string voterID;
    string name;
    string pollingStation;
    bool hasVoted = false;      // Initialize
    VoterNode* next = nullptr;  // Initialize
};

struct BSTNode {
    string voterID;
    VoterNode* voterPtr = nullptr; // Initialize
    BSTNode* left = nullptr;       // Initialize
    BSTNode* right = nullptr;      // Initialize
    int height = 1;                // Initialize
};

/* ===================== GLOBALS ===================== */

const int NUM_STATIONS = 9;
const int NUM_PARTIES = 4;

string stations[NUM_STATIONS] = { "G-8","G-9","G-10","F-8","F-9","F-10","I-8","I-9","I-10" };
string parties[NUM_PARTIES] = { "PTI","PML-N","PPP","Establishment" };

VoterNode* stationLists[NUM_STATIONS];
BSTNode* stationBST[NUM_STATIONS];
int voteCount[NUM_STATIONS][NUM_PARTIES];

stack<pair<string, int>> voteHistory;
bool electionLocked = false;

/* ===================== AVL / BST FUNCTIONS ===================== */

int getHeight(BSTNode* node) { return node ? node->height : 0; }
int getBalance(BSTNode* node) { return node ? getHeight(node->left) - getHeight(node->right) : 0; }

BSTNode* rightRotate(BSTNode* y) {
    BSTNode* x = y->left; BSTNode* T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

BSTNode* leftRotate(BSTNode* x) {
    BSTNode* y = x->right; BSTNode* T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

BSTNode* insertAVL(BSTNode* root, string id, VoterNode* ptr) {
    if (!root) {
        BSTNode* node = new BSTNode;
        node->voterID = id; node->voterPtr = ptr; node->left = node->right = nullptr; node->height = 1;
        return node;
    }
    if (id < root->voterID) root->left = insertAVL(root->left, id, ptr);
    else root->right = insertAVL(root->right, id, ptr);

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && id < root->left->voterID) return rightRotate(root);
    if (balance < -1 && id > root->right->voterID) return leftRotate(root);
    if (balance > 1 && id > root->left->voterID) { root->left = leftRotate(root->left); return rightRotate(root); }
    if (balance < -1 && id < root->right->voterID) { root->right = rightRotate(root->right); return leftRotate(root); }
    return root;
}

VoterNode* searchBST(BSTNode* root, string id) {
    if (!root) return nullptr;
    if (root->voterID == id) return root->voterPtr;
    if (id < root->voterID) return searchBST(root->left, id);
    return searchBST(root->right, id);
}

/* ===================== LINKED LIST FUNCTIONS ===================== */

void insertVoterLinkedList(int stationIndex, VoterNode* voter) {
    voter->next = stationLists[stationIndex];
    stationLists[stationIndex] = voter;
}

/* ===================== VOTER DATA INITIALIZATION ===================== */

void generateVoters() {
    string voterData[NUM_STATIONS][20][2] = {
        {{"101","Talha Khan"},{"102","Ahsan Malik"},{"103","Hamza Qureshi"},{"104","Bilal Abbasi"},{"105","Umar Siddiqui"},
         {"106","Hassan Raja"},{"107","Zain Hussain"},{"108","Ali Khan"},{"109","Ayaan Malik"},{"110","Salman Qureshi"},
         {"111","Fatima Khan"},{"112","Ayesha Malik"},{"113","Sara Qureshi"},{"114","Hina Abbasi"},{"115","Sana Siddiqui"},
         {"116","Maryam Khan"},{"117","Noor Malik"},{"118","Amna Qureshi"},{"119","Hira Raja"},{"120","Zoya Hussain"}},
        {{"201","Usman Khan"},{"202","Hamza Malik"},{"203","Ali Abbasi"},{"204","Bilal Qureshi"},{"205","Zain Siddiqui"},
         {"206","Hassan Raja"},{"207","Umar Hussain"},{"208","Salman Khan"},{"209","Ahmed Malik"},{"210","Ayaan Qureshi"},
         {"211","Fatima Raja"},{"212","Sara Hussain"},{"213","Hina Khan"},{"214","Sana Malik"},{"215","Ayesha Qureshi"},
         {"216","Maryam Abbasi"},{"217","Noor Siddiqui"},{"218","Amna Khan"},{"219","Hira Malik"},{"220","Zoya Qureshi"}},
        {{"301","Talha Abbasi"},{"302","Hamza Qureshi"},{"303","Usman Siddiqui"},{"304","Zain Khan"},{"305","Bilal Malik"},
         {"306","Ali Raja"},{"307","Hassan Hussain"},{"308","Ayaan Khan"},{"309","Umar Malik"},{"310","Ahmed Qureshi"},
         {"311","Fatima Abbasi"},{"312","Sara Siddiqui"},{"313","Hina Khan"},{"314","Sana Malik"},{"315","Ayesha Qureshi"},
         {"316","Maryam Raja"},{"317","Noor Hussain"},{"318","Amna Khan"},{"319","Hira Malik"},{"320","Zoya Qureshi"}},
        {{"401","Bilal Khan"},{"402","Hassan Malik"},{"403","Ali Qureshi"},{"404","Umar Siddiqui"},{"405","Ayaan Raja"},
         {"406","Hamza Hussain"},{"407","Talha Khan"},{"408","Usman Malik"},{"409","Ahmed Qureshi"},{"410","Salman Siddiqui"},
         {"411","Fatima Malik"},{"412","Sara Khan"},{"413","Hina Qureshi"},{"414","Sana Abbasi"},{"415","Ayesha Siddiqui"},
         {"416","Maryam Raja"},{"417","Noor Hussain"},{"418","Amna Malik"},{"419","Hira Khan"},{"420","Zoya Qureshi"}},
        {{"501","Hassan Khan"},{"502","Bilal Malik"},{"503","Ali Abbasi"},{"504","Talha Qureshi"},{"505","Umar Siddiqui"},
         {"506","Ayaan Raja"},{"507","Hamza Hussain"},{"508","Ahmed Khan"},{"509","Usman Malik"},{"510","Salman Qureshi"},
         {"511","Fatima Abbasi"},{"512","Sara Siddiqui"},{"513","Hina Khan"},{"514","Sana Malik"},{"515","Ayesha Qureshi"},
         {"516","Maryam Raja"},{"517","Noor Hussain"},{"518","Amna Malik"},{"519","Hira Khan"},{"520","Zoya Qureshi"}},
        {{"601","Talha Malik"},{"602","Ali Qureshi"},{"603","Bilal Siddiqui"},{"604","Hamza Khan"},{"605","Hassan Raja"},
         {"606","Umar Hussain"},{"607","Zain Khan"},{"608","Ahmed Malik"},{"609","Salman Qureshi"},{"610","Ayaan Siddiqui"},
         {"611","Fatima Khan"},{"612","Sara Malik"},{"613","Hina Qureshi"},{"614","Sana Abbasi"},{"615","Ayesha Siddiqui"},
         {"616","Maryam Raja"},{"617","Noor Hussain"},{"618","Amna Khan"},{"619","Hira Malik"},{"620","Zoya Qureshi"}},
        {{"701","Ali Khan"},{"702","Hassan Malik"},{"703","Usman Qureshi"},{"704","Bilal Siddiqui"},{"705","Talha Raja"},
         {"706","Hamza Hussain"},{"707","Umar Khan"},{"708","Ahmed Malik"},{"709","Zain Qureshi"},{"710","Salman Siddiqui"},
         {"711","Fatima Raja"},{"712","Sara Hussain"},{"713","Hina Khan"},{"714","Sana Malik"},{"715","Ayesha Qureshi"},
         {"716","Maryam Abbasi"},{"717","Noor Siddiqui"},{"718","Amna Khan"},{"719","Hira Malik"},{"720","Zoya Qureshi"}},
        {{"801","Hassan Khan"},{"802","Bilal Malik"},{"803","Ali Qureshi"},{"804","Talha Siddiqui"},{"805","Umar Raja"},
         {"806","Hamza Hussain"},{"807","Zain Khan"},{"808","Ahmed Malik"},{"809","Salman Qureshi"},{"810","Ayaan Siddiqui"},
         {"811","Fatima Khan"},{"812","Sara Malik"},{"813","Hina Qureshi"},{"814","Sana Abbasi"},{"815","Ayesha Siddiqui"},
         {"816","Maryam Raja"},{"817","Noor Hussain"},{"818","Amna Malik"},{"819","Hira Khan"},{"820","Zoya Qureshi"}},
        {{"901","Talha Khan"},{"902","Ali Malik"},{"903","Bilal Qureshi"},{"904","Hamza Siddiqui"},{"905","Hassan Raja"},
         {"906","Umar Hussain"},{"907","Zain Khan"},{"908","Ahmed Malik"},{"909","Salman Qureshi"},{"910","Ayaan Siddiqui"},
         {"911","Fatima Khan"},{"912","Sara Malik"},{"913","Hina Qureshi"},{"914","Sana Abbasi"},{"915","Ayesha Siddiqui"},
         {"916","Maryam Raja"},{"917","Noor Hussain"},{"918","Amna Malik"},{"919","Hira Khan"},{"920","Zoya Qureshi"}}
    };

    for (int s = 0;s < NUM_STATIONS;s++) {
        stationLists[s] = nullptr; stationBST[s] = nullptr;
        for (int i = 0;i < 20;i++) {
            VoterNode* v = new VoterNode;
            v->voterID = voterData[s][i][0];
            v->name = voterData[s][i][1];
            v->pollingStation = stations[s];
            v->hasVoted = false; v->next = nullptr;
            insertVoterLinkedList(s, v);
            stationBST[s] = insertAVL(stationBST[s], v->voterID, v);
        }
        for (int p = 0;p < NUM_PARTIES;p++) voteCount[s][p] = 0;
    }
}

/* ===================== VOTING FUNCTIONS ===================== */

VoterNode* authenticateVoter(int stationIndex, string id, string name) {
    VoterNode* voter = searchBST(stationBST[stationIndex], id);
    if (voter && voter->name == name && !voter->hasVoted) return voter;
    return nullptr;
}

void miniVoteMenu(int stationIndex) {
    while (true) {
        cout << "\n--- Cast Your Vote ---\n";
        cout << "1. Cast Vote\n2. Undo Last Vote\n3. Cast Next Vote\n4. Return to Main Menu\nChoice: ";
        int choice; cin >> choice; cin.ignore();
        if (choice == 4) break;

        if (choice == 1 || choice == 3) {
            string id, name;
            cout << "Enter Voter ID: "; cin >> id; cin.ignore();
            cout << "Enter Name: "; getline(cin, name);
            VoterNode* voter = authenticateVoter(stationIndex, id, name);
            if (!voter) { cout << "? Not registered or already voted.\n"; continue; }
            cout << "\nSelect Party:\n";
            for (int i = 0;i < NUM_PARTIES;i++) cout << i + 1 << ". " << parties[i] << endl;
            int p; cin >> p;
            if (p<1 || p>NUM_PARTIES) { cout << "Invalid choice\n"; continue; }
            voter->hasVoted = true;
            voteCount[stationIndex][p - 1]++;
            voteHistory.push(pair<string, int>(voter->voterID, p - 1));
            cout << "? Vote successfully cast.\n";
        }
        else if (choice == 2) {
            if (voteHistory.empty()) { cout << "No vote to undo.\n"; continue; }
            pair<string, int> last = voteHistory.top(); voteHistory.pop();
            string vid = last.first; int pidx = last.second;
            for (int i = 0;i < NUM_STATIONS;i++) {
                VoterNode* ptr = stationLists[i];
                while (ptr) { if (ptr->voterID == vid) { ptr->hasVoted = false; voteCount[i][pidx]--; break; } ptr = ptr->next; }
            }
            cout << "? Last vote undone successfully.\n";
        }
    }
}

void showLiveStatus(int stationIndex) {
    int total = 0, voted = 0;
    VoterNode* ptr = stationLists[stationIndex];
    while (ptr) { total++; if (ptr->hasVoted) voted++; ptr = ptr->next; }
    cout << "\nLive Status (" << stations[stationIndex] << ")\n";
    cout << "Total: " << total << " Voted: " << voted << " Pending: " << total - voted << "\n";
}

void showStationResult(int stationIndex) {
    cout << "\nPolling Station: " << stations[stationIndex] << endl;
    int maxVotes = -1, secondMax = -1, winner = -1;
    for (int i = 0;i < NUM_PARTIES;i++) {
        cout << parties[i] << ": " << voteCount[stationIndex][i] << " votes\n";
        if (voteCount[stationIndex][i] > maxVotes) { secondMax = maxVotes; maxVotes = voteCount[stationIndex][i]; winner = i; }
        else if (voteCount[stationIndex][i] > secondMax) secondMax = voteCount[stationIndex][i];
    }
    cout << "Winner: " << parties[winner] << " by margin " << (maxVotes - secondMax) << "\n";
}