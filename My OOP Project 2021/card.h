#ifndef CARD_H
#define CARD_H
#include <map>

#include <vector>
#include <memory>

using namespace std;

class Card {
    char rank;
    char suit;
    static map<char,int> rankChar_to_Int;
    static map<int,char> rankInt_to_Char;

    public:
    Card(char rank, char suit);
    int getCardrankInt() const;
    char getCardrank() const;
    char getCardSuit() const;
    string getCardName() const;

    string getStraightsNext() const;
    string getStraightsPrev() const;
};


#endif