#include "player.h"
#include "card.h"
#include <string>
#include <sstream> 
#include <iomanip>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

static bool compareCard(shared_ptr<Card> C1, shared_ptr<Card> C2) {
    return (C1->getCardrankInt()) < (C2->getCardrankInt());
}

Player::Player(int id, char type): id{id}, type{type}, score{0}, isquit{false} {}

int Player::getid() const {
    return this->id;
}

char Player::getype() const {
    return this->type;
}

int Player::getscore() const {
    return this->score;
}

vector<shared_ptr<Card>> Player::getCardsinHand() const {
    return this->CardsinHand;
}

vector<shared_ptr<Card>> Player::getCardsDiscarded() const {
    return this->CardsDiscarded;
}

bool Player::having7S() const {
    for (auto card: this->CardsinHand) {
        if (card->getCardName() == "7S") {
            return true;
        }
    }
    return false;
}


vector<string> Player::getLegalPLays(vector<shared_ptr<Card>> CardsonTable) const {
    vector<string> legalplays;
    int table_cards_num = CardsonTable.size();

    // check if the table is empty
    if (table_cards_num == 0) {
        legalplays.emplace_back("7S");
        return legalplays;
    }

    // make sure the increasing order of rank
    sort(CardsonTable.begin(),CardsonTable.end(),compareCard);



    // store the cards into suit vector
    // Spade
    vector<shared_ptr<Card>> Spade;
    for (int i = 0; i < table_cards_num; ++i) {
        char cur_suit = CardsonTable.at(i)->getCardSuit();
        if (cur_suit == 'S') {
            Spade.emplace_back(CardsonTable.at(i));
        }
    }


    if (Spade.at(0)->getStraightsPrev() != "") legalplays.emplace_back(Spade.at(0)->getStraightsPrev());
    if (Spade.at(Spade.size() - 1)->getStraightsNext() != "") legalplays.emplace_back(Spade.at(Spade.size() - 1)->getStraightsNext());


    // Club 
    vector<shared_ptr<Card>> Club;
    for (int i = 0; i < table_cards_num; ++i) {
        char cur_suit = CardsonTable.at(i)->getCardSuit();
        if (cur_suit == 'C') {
            Club.emplace_back(CardsonTable.at(i));
        }
    }
    if (Club.size() == 0) {
        legalplays.emplace_back("7C");
    } else {
        if (Club.at(0)->getStraightsPrev() != "") legalplays.emplace_back(Club.at(0)->getStraightsPrev());
        if (Club.at(Club.size() - 1)->getStraightsNext() != "") legalplays.emplace_back(Club.at(Club.size() - 1)->getStraightsNext());
    }


    // Diamond
    vector<shared_ptr<Card>> Diamond;
    for (int i = 0; i < table_cards_num; ++i) {
        char cur_suit = CardsonTable.at(i)->getCardSuit();
        if (cur_suit == 'D') {
            Diamond.emplace_back(CardsonTable.at(i));
        }
    }
    if (Diamond.size() == 0) {
        legalplays.emplace_back("7D");
    } else {
        if (Diamond.at(0)->getStraightsPrev() != "") legalplays.emplace_back(Diamond.at(0)->getStraightsPrev());
        if (Diamond.at(Diamond.size() - 1)->getStraightsNext() != "") legalplays.emplace_back(Diamond.at(Diamond.size() - 1)->getStraightsNext());
    }

    // Heart
    vector<shared_ptr<Card>> Heart;
    for (int i = 0; i < table_cards_num; ++i) {
        char cur_suit = CardsonTable.at(i)->getCardSuit();
        if (cur_suit == 'H') {
            Heart.emplace_back(CardsonTable.at(i));
        }
    }

    if (Heart.size() == 0) {
        legalplays.emplace_back("7H");
    } else {
        if (Heart.at(0)->getStraightsPrev() != "") legalplays.emplace_back(Heart.at(0)->getStraightsPrev());
        if (Heart.at(Heart.size() - 1)->getStraightsNext() != "") legalplays.emplace_back(Heart.at(Heart.size() - 1)->getStraightsNext());
    }

    // the vector store the cards in hand that is also in legalplays
    vector<string> res_legalplays;

    if (legalplays.size() == 0) {
        return legalplays;
    } else {
        int legalplay_size = legalplays.size();
        // store the card name into vector
        for (auto card: this->CardsinHand) {
            string name = card->getCardName();
            for (int i = 0; i < legalplay_size; ++i) {
                if (name == legalplays.at(i)) {
                    res_legalplays.emplace_back(name);
                }
            }
        }
    }

    return res_legalplays;

}

bool Player::is_valid_command(string command, string name, vector<string> legalCardPlay) {
    if (command == "play") {
        if (find(legalCardPlay.begin(), legalCardPlay.end(), name) != legalCardPlay.end()) {
            return true;
        } else {
            cout << "This is not a legal play" << endl;
            return false;
        }
    } else if (command == "discard") {
        if (legalCardPlay.size() != 0) {
            cout << "You have a legal play. You may not discard." << endl;
            return false;
        } else if (this->getcard_Hand_index(name) == -1) {
            cout << "Sorry, the card is not in hand. Please discard the cards in your hand." << endl;
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

void Player::Action(vector<shared_ptr<Card>>& CardsonTable, vector<string> legalCardPlay, string name, string command) {
    if (CardsinHand.size() == 0) {
        cout << "Player" << this->id << " has no card in hand." << endl;
        cout << "Now proceed to the next player." << endl;
        return;
    }

    if (this->type == 'h') {
        if (command == "quit") {
            this->isquit = true;
            cout << "Player" << this->id << " quit, the game is over! If you want to play again, please restart the game." << endl;
            return;
        } else if (command == "ragequit") {
            this->type = 'c';
            cout << "Player" << this->getid() << " ragequits. A computer will now take over."  << endl;
            this->AImove(CardsonTable, legalCardPlay);
            return;
        } 

        int card_index = this->getcard_Hand_index(name);
        if (command == "play") {
            CardsonTable.emplace_back(this->CardsinHand.at(card_index));
            this->CardsinHand.erase(find(CardsinHand.begin(), CardsinHand.end(),this->CardsinHand.at(card_index)));
            cout << "Player" << this->id << " plays " << name << endl;   
        } else if (command == "discard") {
            this->CardsDiscarded.emplace_back(this->CardsinHand.at(card_index));
            this->CardsinHand.erase(find(CardsinHand.begin(), CardsinHand.end(),this->CardsinHand.at(card_index)));
            cout << "Player" << this->id << " discards " << name << endl;
            }
        
    } else {
        this->AImove(CardsonTable, legalCardPlay);
    }
}


int Player::getcard_Hand_index(string name) const {
    int index = -1;
    int handsize = this->CardsinHand.size();
    for (int i = 0; i < handsize; ++i) {
        if (this->CardsinHand.at(i)->getCardName() == name) {
            index = i;
            break;
        }
    }
    return index;
}


int Player::getcard_Discarded_index(string name) const {
    int index = -1;
    int discardsize = this->CardsDiscarded.size();
    for (int i = 0; i < discardsize; ++i) {
        if (this->CardsDiscarded.at(i)->getCardName() == name) {
            index = i;
            break;
        }
    }
    return index;
}

bool Player::isQuit() const {
    return this->isquit;
}

void Player::AImove(vector<shared_ptr<Card>>& CardsonTable, vector<string> legalCardPlay) {
    if (legalCardPlay.size() == 0) {
        string name = this->CardsinHand.at(0)->getCardName();
        this->CardsDiscarded.emplace_back(this->CardsinHand.at(0));
        this->CardsinHand.erase(this->CardsinHand.begin());
        cout <<  "Player" << this->id << "(Computer)" << " discards " << name << endl;
    } else {
        string legal_name = legalCardPlay.at(0);
        int index = this->getcard_Hand_index(legal_name);
        CardsonTable.emplace_back(this->CardsinHand.at(index));
        this->CardsinHand.erase(this->CardsinHand.begin() + index);
        cout << "Player" << this->id << "(Computer)" << " plays " << legal_name << endl;
    }
}

void Player::clearHand() {
    this->CardsinHand.clear();
}

void Player::clearDiscard() {
    this->CardsDiscarded.clear();
}


void Player::addHand(shared_ptr<Card> card) {
    this->CardsinHand.emplace_back(card);
}


void Player::addDiscard(shared_ptr<Card> card) {
    this->CardsDiscarded.emplace_back(card);
}

void Player::updatescore(int curscore) {
    this->score = curscore;
}
