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
#include "Server.hpp"
#include <cereal/archives/json.hpp>
#include <cereal/access.hpp>


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}
class Coord{
public:
    Coord(int x, int y){
        this->x = x;
        this->y = y;
    }
    Coord(){
        this->x = -100;
        this->y = -100;
    }
    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(x,y);
    }
public:
    int x,y;
};

void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    if(board_size < 2) {
        throw ServerException("Wrong Board Size");
    }else if(p1_setup_board.length() < 1 || p2_setup_board.length() < 1){
        throw ServerException("Bad File Names.");
    }else{
        this->p1_setup_board.open(p1_setup_board);
        this->p2_setup_board.open(p2_setup_board);

        cout << "Working parameters" <<endl;
    }

}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if(x > board_size){
        return OUT_OF_BOUNDS;
    }
    else if(y > board_size){
        return OUT_OF_BOUNDS;
    }
    else if(player < 1 || player > MAX_PLAYERS) {
        throw ServerException("Bad Player Number.");
    }else{
        if(player == 1){
            cout << "Working with Player 1" <<endl;
        }else{
            cout << "Working with Player 2" << endl;
        }

        //TODO:Work with Files: Grab board, see if miss or hit, return result as json file
        if(player == 1){
            cout << "Finished with P1 File" <<endl;
        }else{
            cout << "Finished with P2 File" <<endl;
        }
    }

}


int Server::process_shot(unsigned int player) {
    string name = "player_";
    name += to_string(player);
    name += ".shot.json";
    vector<int> coord(2,0);
    Coord c;
    std::ifstream f;
    f.open(name);
    {
        if(f.good()){
            cereal::JSONInputArchive fin(f);
            fin(c);
            cout <<c.x<<endl;
            cout << c.y<< endl;
        }
        else{
            cout <<"File not found"<< endl;
        }

    }

    //TODO:Grab proper file, if no file then this statement, else call eval shot
   return NO_SHOT_FILE;
}