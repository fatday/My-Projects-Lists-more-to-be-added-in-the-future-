#include "table.h"
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

static const vector<char>rank_char {'A', '2', '3', '4', '5', '6', '7', '8', '9','T', 'J', 'Q', 'K'};
static const vector<char>suit_char {'C', 'D', 'H', 'S'};

Table::Table(default_random_engine seed) {
    this->curr_Player = nullptr;
    this->Seed = seed;
    this->text = make_shared<Text>();
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 4; ++ j) {
            auto card = make_shared<Card>(rank_char.at(i), suit_char.at(j));
            this->Deck.emplace_back(card);
        }
    }
    this->RoundNum = 0;

}


void Table::shuffle_deck() {
    shuffle(this->Deck.begin(), this->Deck.end(), this->Seed);
}

void Table::deliver_card() {
    for (int i = 0; i < 4; ++i) {
        this->Players.at(i)->clearHand();
        for (int j = 13 * i; j < 13 * (i + 1); ++j) {
            this->Players.at(i)->addHand(this->Deck.at(j));
        }
    }
}

void Table::nextPlayer() {
    int index = this->curr_Player->getid()  % 4;
    this->curr_Player = this->Players.at(index);
}

void Table::showdeck() {
    this->text->display_deck(this->Deck);
}

void Table::roundResult() {
    cout << "This is the end of the current round, the following is the summary of the round:" << endl;
    for (int i = 0; i < 4; ++i) {
        int get_scores = 0;
        int discard_size = this->Players.at(i)->getCardsDiscarded().size();
        cout << "Player" << this->Players.at(i)->getid() << "'s discards:";
        for (int j = 0; j < discard_size; ++j) {
            int card_discard_score = this->Players.at(i)->getCardsDiscarded().at(j)->getCardrankInt();
            get_scores += card_discard_score;
            cout << " " << this->Players.at(i)->getCardsDiscarded().at(j)->getCardName();
        }
        cout << endl;
        int cur_score = get_scores + this->Players.at(i)->getscore();
        cout << "Player" << this->Players.at(i)->getid() << "'s score: " << this->Players.at(i)->getscore() << " + " << get_scores << " = " << cur_score << endl;
        this->Players.at(i)->updatescore(cur_score);
    }

}

void Table::startRound() {
    this->CardsonTable.clear();
    if (this->isGameEnd()) return;
    for (int i = 0; i < 4; ++i) {
        this->Players.at(i)->clearHand();
        this->Players.at(i)->clearDiscard();
    }

    this->shuffle_deck();
    this->deliver_card();

    for (int i = 0; i < 4; i++){
        if (this->Players.at(i)->having7S()) {
            this->curr_Player = this->Players.at(i);
            break;
        }
    }

    cout << "A new round begins. It's Player" << this->curr_Player->getid() <<"'s turn to play." << endl;
    this->RoundNum = this->RoundNum + 1;
    if (this->curr_Player->getype() == 'h' && (this->RoundNum == 1)) {
        this->text->notify(this->CardsonTable, this->curr_Player);
    }
}

void Table::curPlayermove(string name, string command) {
    this->curr_Player->Action(this->CardsonTable, this->curr_Player->getLegalPLays(this->CardsonTable), name, command);

    if (this->isRoundEnd()) {
        cout << endl;
        this->roundResult();
        cout << endl;
        this->startRound();
    } else {
        this->nextPlayer();
    }

    if (curr_Player->getype() == 'h' && !(this->isGameEnd())) {
        this->text->notify(this->CardsonTable, this->curr_Player);
    }
}

bool Table::isRoundEnd() const {
    bool isAllnoCard = true;
    for (int i = 0 ; i < 4; i++) {
        isAllnoCard = isAllnoCard && (this->Players.at(i)->getCardsinHand().size() == 0);
    }
    return isAllnoCard;
}

bool Table::isGameEnd() const {
    bool isscore_exceed_80 = false;
    bool isPlayerQuit = false;
    for (int i = 0 ; i < 4; i++) {
        isscore_exceed_80 = isscore_exceed_80 || (this->Players.at(i)->getscore() >= 80);
        isPlayerQuit = isPlayerQuit || (this->Players.at(i)->isQuit());
    }

    return isscore_exceed_80 || isPlayerQuit;
}


void Table::updateWinnter() {
    int min_score = this->curr_Player->getscore();
    for (int i = 0 ; i < 4; i++) {
        int cur_score = this->Players.at(i)->getscore();
        if (cur_score < min_score) min_score = cur_score;
    }


    for (int i = 0 ; i < 4; i++) {
        int cur_score = this->Players.at(i)->getscore();
        if (cur_score == min_score) {
            this->Winners.emplace_back(this->Players.at(i));
        }
    }
}

void Table::addPlayer(shared_ptr<Player> player) {
    this->Players.emplace_back(player);
}

char Table::getCurPlayertype() const {
    return this->curr_Player->getype();
}


int Table::curId() const {
    return curr_Player->getid();
}


bool Table::isPlayerQuit() const {
    bool quitexist = false;
    for (int i = 0; i < 4; ++i) {
        quitexist = quitexist || this->Players.at(i)->isQuit();
    }
    return quitexist;
}