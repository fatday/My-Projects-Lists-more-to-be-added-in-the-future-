#include <string>
#include <sstream> 
#include <iomanip>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <time.h>
#include <memory>

#include "player.h"
#include "card.h"
#include "table.h"
#include "observer.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand (time(NULL));
    unsigned int seed = rand() % 10000;
    if (argc == 2) {
        try {
            seed = stoi(argv[1]);
        } catch(...) {
            return 1;
        }
    }

    // start the game and add 4 players
    int player_count = 0;
    default_random_engine eng{seed};
    unique_ptr<Table> table = make_unique<Table>(eng);
    while(true) {
        if (player_count == 4) break;
        cout << "Is Player" << (player_count + 1) << " a human (h) or a computer (c)?" << endl;
        string type;
        while(true) {
            getline(cin, type);
            if (type.at(0) == 'h' || type.at(0) == 'c') break;
            else {
                cout << "Invalid player type, please enter 'h' or 'c'." << endl;
            }
        }
        player_count += 1;
        auto add_player = make_shared<Player>(player_count, type.at(0));
        table->addPlayer(add_player);
    }

    table->startRound();

    while(!table->isGameEnd()) {
        string input;
        string command;
        string name;
        if (table->getCurPlayertype() == 'c') {
            cout << endl;
            cout << "Player" << table->curId() << "'s turn:" << endl;
            table->curPlayermove("", "");
        } else {
            cout << "Please enter a command. (Available commands: deck, play <card>, discard <card>, ragequit, quit)" << endl;
            while(true) {
                getline(cin, input);
                istringstream line{input};
                line >> command;
                line >> name;
                if (command == "deck") {
                    table->showdeck();
                    cout << "Please enter a command. (Available commands: deck, play <card>, discard <card>, ragequit, quit)" << endl;
                    continue;
                } else if (command == "quit" || command == "ragequit") {
                    table->curPlayermove(name, command);
                    break;
                } else if (command == "play" || command == "discard") {
                    if (table->curr_Player->is_valid_command(command, name, table->curr_Player->getLegalPLays(table->CardsonTable))) {
                        table->curPlayermove(name, command);
                        break;
                    }
                    cout << "Please re-enter a command. (Available commands: deck, play <card>, discard <card>, ragequit, quit)" << endl;
                    continue;
                } else {
                    cout << "The command is invalid, please re-enter a command. (Available commands: deck, play <card>, discard <card>, ragequit, quit)" << endl;
                    continue;
                }
            }

        }
    }

    if (table->isPlayerQuit()) {
        return 0;
    } else {
        table->updateWinnter();
        int win_size = table->Winners.size();
        if (win_size > 1) {
            for (int i = 0; i < win_size; ++i) {
                cout << "Player" << table->Winners.at(i)->getid() << " ";
            }
            cout << "win!" << endl;
    }   else {
            cout << "Player" << table->Winners.at(0)->getid() << " wins!" << endl;
        }
    }

}
