#include "card.h"
#include "player.h"
#include <map>

using namespace std;

map<char,int> Card::rankChar_to_Int {{'A', 1}, {'2', 2}, {'3', 3},
{'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'T', 10}, {'J', 11},
{'Q', 12}, {'K', 13}};


map<int,char> Card::rankInt_to_Char {{1, 'A'}, {2, '2'}, {3, '3'}, {4, '4'},
{5, '5'}, {6, '6'}, {7, '7'}, {8, '8'}, {9, '9'}, {10, 'T'}, {11, 'J'}, {12, 'Q'},
{13, 'K'}};

Card::Card(char rank, char suit): rank{rank}, suit{suit} {}

int Card::getCardrankInt() const {
    return rankChar_to_Int[this->rank];
}

char Card::getCardrank() const {
    return this->rank;
}

char Card::getCardSuit() const {
    return this->suit;
}


string Card::getCardName() const {
    string name = string(1, this->rank) + string(1, this->suit);
    return name;
}

string Card::getStraightsNext() const {
    string name = string(1, this->suit);
    int rankint = rankChar_to_Int[this->rank];
    if (rankint == 13) {
        return "";
    } else {
        name = string(1, rankInt_to_Char[rankint + 1]) + name;
        return name;
    }
}

string Card::getStraightsPrev() const {
    string name = string(1, this->suit);
    int rankint = rankChar_to_Int[this->rank];
    if (rankint == 1) {
        return "";
    } else {
        name = string(1, rankInt_to_Char[rankint - 1]) + name;
        return name;
    }
}
