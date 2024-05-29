#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include "card.h"

using namespace std;

class Card;


class Player {
    
    int id;
    char type;
    int score;
    bool isquit;
    vector<shared_ptr<Card>> CardsinHand;
    vector<shared_ptr<Card>> CardsDiscarded;

    public:
    // methods
    Player(int id, char type);
    virtual void Action(vector<shared_ptr<Card>>& CardsonTable, vector<string> legalCardPlay, string name, string command);
    virtual void AImove(vector<shared_ptr<Card>>& CardsonTable, vector<string> legalCardPlay);

    // public method
    int getid() const;
    int getscore() const;
    char getype() const;
    bool having7S() const;
    bool isQuit() const;
    int getcard_Hand_index(string name) const;
    int getcard_Discarded_index(string name) const;
    bool is_valid_command(string command, string name, vector<string> legalCardPlay);
    vector<shared_ptr<Card>> getCardsinHand() const;
    vector<shared_ptr<Card>> getCardsDiscarded() const;
    vector<string> getLegalPLays(vector<shared_ptr<Card>> CardsonTable) const;
    void clearHand();
    void clearDiscard();
    void addHand(shared_ptr<Card> card);
    void addDiscard(shared_ptr<Card> card);
    void updatescore(int curscore);

    
};





#endif