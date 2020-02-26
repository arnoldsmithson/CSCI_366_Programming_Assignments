//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Client.hpp"
#include <cereal/archives/json.hpp>
#include <fstream>

using namespace std;


Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size) {
    if (player > 2 || player < 1) {
        throw ClientWrongPlayerNumberException();
    }  else {
        this->player = player;
        this->board_size = board_size;
        string name = "player_";
        name += std::to_string(player);
        name += ".action_board.json";
        //Creates player_#.action_board.json.
        vector<int> v(board_size, 0);
        vector<vector<int>> board(board_size, v);
        ofstream file;
        file.open(name, ofstream::out);
        board.end();
        if (file) {
            {
                cereal::JSONOutputArchive arc(file);
                arc(CEREAL_NVP(board));
            }
            file.close();
            initialized = true;
        } else {
            cout << "File not found." << endl;
        }

    }
}


void Client::fire(unsigned int x, unsigned int y) {
    string name = "player_";
    vector<int> coords(board_size, 0);
    coords[0] = x;
    coords[1] = y;
    name += to_string(player);
    name += ".shot.json";
    ofstream file;
    {
        file.open(name, ofstream::out);
        cereal::JSONOutputArchive arc(file);
        arc(CEREAL_NVP(coords));
    }
    file.flush();
    //TODO:Call process_shot with player_name
}

bool Client::result_available() {
}


int Client::get_result() {
}


void Client::update_action_board(int result, unsigned int x, unsigned int y) {

}


string Client::render_action_board() {
}


