#include <iostream>
#include <array>
#include <vector>

using namespace std;

struct nodeType {
    int index;
    char state;
    int score;
};


const char HUMAN = 'O';
const char AI = 'X';
const char EMPTY = '_';
const int WIN = 99999;
const int TIE = 0;
const int LOSS = -99999;
const int START = 0;
const int MAX = 50;
int SIZE = 16;

nodeType board[MAX] = { };

bool wrongIndex = false;
bool three = false;
bool humanFirst = false;
bool quit = false;
void clearScore();
vector<nodeType> getAvailableMoves();
int minValue(nodeType node, int alpha, int beta, int depth);
int maxValue(nodeType node, int alpha, int beta, int depth);
nodeType minMax(char player);
bool isAvailable(int index);
void initializeBoard();
bool fullBoard();
bool gameOver();
int stateOfBoard(char player, int depth);
int getIndex(int row, int col);
bool checkWin(char player);
void printBoard(bool three);
void printBoardScore();



int main() {
    while (!quit) {
        cout << "Enter a number from the menu to select a board size:" << endl << "1. 3 x 3 BOARD" << endl << "2. 4 x 4 BOARD" << endl << "3. QUIT" << endl;
        int choice;
        cin >> choice;
        if (choice == 1) {
            three = true;
            SIZE = 9;
        }
        else if(choice == 3) { return 0; }
        else if(choice == 2) {
            three = false;
            SIZE = 16;
        } else {
            cout << "ERROR not a good choice!" << endl;
            return 0;
        }
        cout << "Enter a number from the menu to select who goes first:" << endl << "1. HUMAN" << endl << "2. AI" << endl;
        int choice2 = 0;
        cin >> choice2;
        if (choice2 == 1) {
            humanFirst = true;
        } else if (choice2 == 2) {
             humanFirst = false;
        }
        else {
            cout << "ERROR not a good choice!" << endl;
            return 0;
        }
        
        initializeBoard();
        //board[5].state = AI;
        printBoard(three);
        

        while(!gameOver()) {
            if (!humanFirst) {
                if (!wrongIndex) {
                    cout << "AI playing!" << endl;
                    nodeType aiState = minMax(AI);
                    cout << aiState.index << aiState.score << aiState.state;
                    board[aiState.index] = aiState;
                    printBoard(three);
                    if (gameOver()) {
                        break;
                    }
                }
            }
            wrongIndex = false;
            cout << "Humans turn, where would you like to place your O?" << endl << endl;
            int row;
            int col;
            cout << "Row: ";
            cin >> row;
            cout << "Col: ";
            cin >> col;
            cout << endl << endl;
            int index = getIndex(row, col);
            if(isAvailable(index)) {
                board[index].state = HUMAN;
            } else {
                cout << "Wrong index, try again..." << endl;
                wrongIndex = true;
                continue;
            }
            printBoard(three);
            if (gameOver()) {
                break;
            }
            // If human just went
            if (humanFirst) {
                if (!wrongIndex) {
                    cout << "AI playing!" << endl;
                    nodeType aiState = minMax(AI);
                    board[aiState.index] = aiState;
                    printBoard(three);
                    if (gameOver()) {
                        break;
                    }
                }
            }
            
        }
        if (checkWin(AI)) {
            cout << "*****AI wins!*****" << endl << endl;
        } else if(checkWin(HUMAN)) {
            cout << "*****Human Wins!*****" << endl << endl;
        } else
            cout << "*****TIE!!!!*****" << endl << endl;
    }
    
}

// Check for a win, tie, or loss
bool checkWin(char player) {
    if (three) {
        if ((
             (board[0].state == player && board[1].state == player && board[2].state == player) ||
             (board[3].state == player && board[4].state == player && board[5].state == player) ||
             (board[6].state == player && board[7].state == player && board[8].state == player) ||
             (board[0].state == player && board[3].state == player && board[6].state == player) ||
             (board[1].state == player && board[4].state == player && board[7].state == player) ||
             (board[2].state == player && board[5].state == player && board[8].state == player) ||
             (board[0].state == player && board[4].state == player && board[8].state == player) ||
             (board[2].state == player && board[4].state == player && board[6].state == player)
             )) {
            return true;
        }
    } else {
        if ((
             (board[0].state == player && board[1].state == player && board[2].state == player) ||
             (board[4].state == player && board[5].state == player && board[6].state == player) ||
             (board[8].state == player && board[9].state == player && board[10].state == player) ||
             (board[0].state == player && board[4].state == player && board[8].state == player) ||
             (board[1].state == player && board[5].state == player && board[9].state == player) ||
             (board[2].state == player && board[6].state == player && board[10].state == player) ||
             (board[0].state == player && board[5].state == player && board[10].state == player) ||
             (board[8].state == player && board[5].state == player && board[2].state == player) ||
             //end of first square check
             (board[1].state == player && board[2].state == player && board[3].state == player) ||
             (board[5].state == player && board[6].state == player && board[7].state == player) ||
             (board[9].state == player && board[10].state == player && board[11].state == player) ||
             (board[3].state == player && board[7].state == player && board[11].state == player) ||
             (board[1].state == player && board[6].state == player && board[11].state == player) ||
             (board[9].state == player && board[6].state == player && board[3].state == player) ||
             //end of second square check
             (board[13].state == player && board[14].state == player && board[15].state == player) ||
             (board[12].state == player && board[13].state == player && board[14].state == player) ||
             (board[4].state == player && board[8].state == player && board[12].state == player) ||
             (board[5].state == player && board[9].state == player && board[13].state == player) ||
             (board[6].state == player && board[10].state == player && board[14].state == player) ||
             (board[7].state == player && board[11].state == player && board[15].state == player) ||
             (board[12].state == player && board[9].state == player && board[6].state == player) ||
             (board[4].state == player && board[9].state == player && board[14].state == player) ||
             (board[5].state == player && board[10].state == player && board[15].state == player) ||
             (board[13].state == player && board[10].state == player && board[7].state == player)
             )) {
            return true;
        }
    }
    return false;
}

int stateOfBoard(char player, int depth) {
    char op;
    if (player == HUMAN) {
        op = AI;
    } else
        op = HUMAN;
    if (checkWin(player)) { return WIN - depth * 10; }
    else if (checkWin(op)) { return LOSS + depth * 10;}
    else
        return TIE;
}


// Get all available moves
vector<nodeType> getAvailableMoves() {
    vector<nodeType> availableMoves;
    for (int i = 0; i < SIZE; i++) {
        if (board[i].state == EMPTY) { availableMoves.push_back(board[i]); }
    }
    return availableMoves;
}

// Is available
bool isAvailable(int index) {
    cout << "checking index " << index << "...";
    if (board[index].state == EMPTY) { return true; }
    return false;
}

int getIndex(int row, int col) {
    int index = 0;
    if (three) {
        switch (row) {
            case 0:
                if (col == 0) index = 0;
                else if (col == 1) index = 1;
                else index = 2;
                break;
            case 1:
                if (col == 0) index = 3;
                else if (col == 1) index = 4;
                else index = 5;
                break;
            case 2:
                if (col == 0) index = 6;
                else if (col == 1) index = 7;
                else index = 8;
                break;
            default:
                cout << "ERROR! Not a valid row or column." << endl;
                index = 8;
                break;
        }
    } else {
        switch (row) {
            case 0:
                if (col == 0) index = 0;
                else if (col == 1) index = 1;
                else if (col == 2) index = 2;
                else index = 3;
                break;
            case 1:
                if (col == 0) index = 4;
                else if (col == 1) index = 5;
                else if (col == 2) index = 6;
                else index = 7;
                break;
            case 2:
                if (col == 0) index = 8;
                else if (col == 1) index = 9;
                else if (col == 2) index = 10;
                else index = 11;
                break;
            case 3:
                if (col == 0) index = 12;
                else if (col == 1) index = 13;
                else if (col == 2) index = 14;
                else index = 15;
                break;
            default:
                index = 15;
                cout << "ERROR! Not a valid row or column." << endl;
                break;
        }
    }
    return index;
}

bool gameOver() {
    if (fullBoard() || checkWin(HUMAN) || checkWin(AI)) { return true; }
    return false;
}

bool fullBoard() {
    vector<nodeType> availableMoves = getAvailableMoves();
    if (availableMoves.size() == 0) { return true; }
    return false;
}

void printBoard(bool three) {
    if (three) {
        cout << endl;
        cout << "    0     1     2" << endl;
        cout << "  ------------------" << endl;
        cout << "0|  " << board[0].state << "  |  " << board[1].state << "  |  " << board[2].state << "  |" << endl;
        cout << " |" << "------------------" << endl;
        cout << "1|  " << board[3].state << "  |  " << board[4].state << "  |  " << board[5].state << "  |" << endl;
        cout << " |" << "------------------" << endl;
        cout << "2|  " << board[6].state << "  |  " << board[7].state << "  |  " << board[8].state << "  |" << endl;
        cout << "  " << "------------------" << endl;
        cout << endl << endl << endl;
    } else {
        cout << endl;
        cout << "    0     1     2     3" << endl;
        cout << "  -----------------------" << endl;
        cout << "0|  " << board[0].state << "  |  " << board[1].state << "  |  " << board[2].state << "  |  " << board[3].state << "  |" << endl;
        cout << " |" << "-----------------------" << endl;
        cout << "1|  " << board[4].state << "  |  " << board[5].state << "  |  " << board[6].state << "  |  " << board[7].state <<  "  |" << endl;
        cout << " |" << "-----------------------" << endl;
        cout << "2|  " << board[8].state << "  |  " << board[9].state << "  |  " << board[10].state << "  |  " << board[11].state <<  "  |" << endl;
        cout << " |" << "-----------------------" << endl;
        cout << "3|  " << board[12].state << "  |  " << board[13].state << "  |  " << board[14].state << "  |  " << board[15].state <<  "  |" << endl;
        cout << "  -----------------------" << endl;
        cout << endl << endl << endl;
    }
}

void printBoardScore() {
    cout << endl;
    cout << "    0     1     2" << endl;
    cout << "  ------------------" << endl;
    cout << "0|  " << board[0].score << "  |  " << board[1].score << "  |  " << board[2].score<< "|" << endl;
    cout << " |" << "------------------" << endl;
    cout << "1|  " << board[3].score << "  |  " << board[4].score << "  |  " << board[5].score << "|" << endl;
    cout << " |" << "------------------" << endl;
    cout << "2|  " << board[6].score << "  |  " << board[7].score << "  |  " << board[8].score << "|" << endl;
    cout << "  " << "------------------" << endl;
    cout << endl << endl << endl;
}


void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        nodeType tempNode;
        tempNode.state = '_';
        tempNode.index = i;
        tempNode.score = 0;
        board[i] = tempNode;
    }
}

nodeType minMax(char player) {
    nodeType best { 0, 'X', -1 };
    vector<nodeType> availableMoves = getAvailableMoves();
    for (int i = 0; i < availableMoves.size(); i++) {
        board[availableMoves[i].index].state = AI;
        board[availableMoves[i].index].score = minValue(board[availableMoves[i].index], LOSS, WIN, START);
        board[availableMoves[i].index].state = EMPTY;
    }
    for (int i = 0; i < availableMoves.size(); i++) {
        if (board[availableMoves[i].index].score > best.score) {
            best = board[availableMoves[i].index];
        }
    }if (best.score == -1) {
        best = availableMoves.back();
    }
    best.state = AI;
    return best;
}

int minValue(nodeType node, int alpha, int beta, int depth) {
    int v = WIN;
    if (gameOver()) {
        return stateOfBoard(AI, depth);
    }
    vector<nodeType> availableMoves = getAvailableMoves();
    for (int i = 0; i < availableMoves.size(); i++) {
        board[availableMoves[i].index].state = HUMAN;
        v = min(v, maxValue(board[availableMoves[i].index], alpha, beta, depth + 1));
        board[availableMoves[i].index].state = EMPTY;
        if (v <= alpha) { return v; }
        beta = min(beta, v);
    }
    return v;
}


int maxValue(nodeType node, int alpha, int beta, int depth) {
    int v = LOSS;
    if (gameOver()) {
        return stateOfBoard(AI, depth);
    }
    vector<nodeType> availableMoves = getAvailableMoves();
    for (int i = 0; i < availableMoves.size(); i++) {
        board[availableMoves[i].index].state = AI;
        v = max(v, minValue(board[availableMoves[i].index], alpha, beta, depth + 1));
        board[availableMoves[i].index].state = EMPTY;
        if (v >= beta) { return v; }
        alpha = max(alpha, v);
    }
    return v;
}



