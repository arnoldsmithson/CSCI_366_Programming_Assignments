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
        // [[0,0]
        //  [0,0]]
        ofstream file;
        file.open(name, ofstream::out);
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
    name += to_string(player);
    name += ".shot.json";
    ofstream file;
    {
        file.open(name, ofstream::out);
        cereal::JSONOutputArchive arc(file);
        arc(CEREAL_NVP(x),CEREAL_NVP(y));
    }
    file.flush();
}

bool Client::result_available() {
    string fileName = "player_"+to_string(player)+".result.json";
    ifstream fin;
    fin.open(fileName);
    if(fin){
        return true;
    }
    else{
        return false;
    }
}


int Client::get_result() {
    if(result_available()){
        int result = 0;
        string fileName = "player_"+to_string(player)+".result.json";
        ifstream fin;
        fin.open(fileName);
        cereal::JSONInputArchive arcin(fin);
        arcin(result);
        cout <<result<<endl;
        fin.close();
        remove(fileName.c_str());
        if(result >= -1 && result <= 1) {
            return result;
        }else{
            throw ClientException("Bad Result Num.");
        }

    }
}


void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    vector<int> v(2,0);
    vector<vector<int>> board(board_size,v);

    string filename = "player_"+to_string(player)+".action_board.json";
    ifstream fin;
    fin.open(filename);
    if(fin.good()){
        {
            cereal::JSONInputArchive arc(fin);
            arc(board);
        }
    }
    board[x][y] = result;
    ofstream fout;
    fout.open(filename);
    if(fout.good()){
        {
            cereal::JSONOutputArchive arcin(fout);
            arcin(CEREAL_NVP(board));
        }
    }


}


string Client::render_action_board() {
    vector<int> v(2,0);
    vector<vector<int>> board(board_size,v);

    string filename = "player_"+to_string(player)+".action_board.json";
    ifstream fin;
    fin.open(filename);
    if(fin.good()){
        {
            cereal::JSONInputArchive arc(fin);
            arc(board);
        }
    }
    string end;
    for (int i = 0; i < board_size; ++i) {
        for (int j = 0; j < board_size; ++j) {
            end += to_string(board[i][j]);
        }
        end += "\n";
    }
    return end;
}


