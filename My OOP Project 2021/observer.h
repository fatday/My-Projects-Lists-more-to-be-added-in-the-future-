#ifndef OBSERVER_H
#define OBSERVER_H


#include <vector>
#include <memory>
#include "card.h"
#include "player.h"

using namespace std;


class Observer {
    public:
    virtual void notify(vector<shared_ptr<Card>> &CardsonTable, shared_ptr<Player> &player) = 0;
    void display_deck(vector<shared_ptr<Card>> &wholedeck);
};


class Text: public Observer {
    public:
    void notify(vector<shared_ptr<Card>> &CardsonTable, shared_ptr<Player> &player) override;
};

#endif