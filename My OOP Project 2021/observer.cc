#include <string>
#include <sstream> 
#include <iomanip>
#include <vector>
#include <iostream>
#include <algorithm>

#include "card.h"
#include "player.h"
#include "table.h"
#include "observer.h"
using namespace std;

static bool compareCard(shared_ptr<Card> C1, shared_ptr<Card> C2) {
    return (C1->getCardrankInt()) < (C2->getCardrankInt());
}


void Observer::display_deck(vector<shared_ptr<Card>> &wholedeck) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 13 * i; j < 13 * (i + 1); ++j) {
            cout << wholedeck.at(j)->getCardName() << " ";
        }
        cout << endl;
    }
}



static void printSuit(string suitname, vector<char> ranks) {
    cout << suitname << ":";
    int ranks_size = ranks.size();
    for (int i = 0 ; i < ranks_size; i++) {
        cout << " " << ranks.at(i);
    }
    cout << endl;
}



void Text::notify(vector<shared_ptr<Card>> &CardsonTable, shared_ptr<Player> &player) {
    sort(CardsonTable.begin(),CardsonTable.end(),compareCard);

    int table_cards_num = CardsonTable.size();

    vector<char> Club;
    vector<char> Diamond;
    vector<char> Heart;
    vector<char> Spade;
    for (int i = 0; i < table_cards_num; ++i) {
        char cur_suit = CardsonTable.at(i)->getCardSuit();
        char cur_rank = CardsonTable.at(i)->getCardrank();
        if (cur_suit == 'C') {
            Club.emplace_back(cur_rank);
        } else if (cur_suit == 'D') {
            Diamond.emplace_back(cur_rank);
        } else if (cur_suit == 'H') {
            Heart.emplace_back(cur_rank);
        } else {
            Spade.emplace_back(cur_rank);
        }
    }

    cout << endl;
    cout << "Cards on the table:" << endl;
    printSuit("Clubs", Club);
    printSuit("Diamonds", Diamond);
    printSuit("Hearts", Heart);
    printSuit("Spades", Spade);

    cout << endl;
    cout << "Player" << player->getid() << "'s turn:" << endl;

    int hand_size = player->getCardsinHand().size();
    cout << "Your hand:";
    for (int i = 0; i < hand_size; ++i) {
        cout << " " <<  player->getCardsinHand().at(i)->getCardName();
    }
    cout << endl;


    vector<string> legal_play = player->getLegalPLays(CardsonTable);
    int legal_size = legal_play.size();
    cout << "Legal plays:";
    for (int i = 0; i < legal_size; ++i) {
        cout << " " <<  legal_play.at(i);
    }
    cout << endl;

}
