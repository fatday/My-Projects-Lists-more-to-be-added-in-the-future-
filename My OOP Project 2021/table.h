#ifndef TABLE_H
#define TABLE_H


#include <vector>
#include <memory>
#include <random>
#include "card.h"
#include "player.h"
#include "observer.h"

using namespace std;



class Table {

    // Players
    vector<shared_ptr<Player>> Players;

    // Cards
    vector<shared_ptr<Card>> Deck;
    default_random_engine Seed;  
    
    // Text
    shared_ptr<Text> text; 

    // #Round  

    int RoundNum;

    public:

    shared_ptr<Player> curr_Player;
    vector<shared_ptr<Player>> Winners;
    vector<shared_ptr<Card>> CardsonTable;

    // public methods
    Table(default_random_engine seed);
    void shuffle_deck();
    void deliver_card();
    void nextPlayer(); 
    void showdeck();
    void roundResult();
    void startRound();
    bool isGameEnd() const;
    bool isRoundEnd() const;
    void updateWinnter();
    void curPlayermove(string name, string command);
    void addPlayer(shared_ptr<Player> player);
    char getCurPlayertype() const;
    int curId() const;
    bool isPlayerQuit() const;


};


#endif