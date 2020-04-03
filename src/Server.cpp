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
int get_file_length(ifstream *file) {
}

void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board) {
    if (board_size != BOARD_SIZE) {
        throw ServerException("Wrong Board Size");
    } else if (p1_setup_board.length() < 1 || p2_setup_board.length() < 1) {
        throw ServerException("Bad File Names.");
    }else if(p1_setup_board == p2_setup_board){
        throw ServerException("Same Player File.");
    } else {
        this->board_size = board_size;
        cout << "Working parameters" << endl;
        //Create the 2DBitArrays based on setup board using scan_setup_board
        // So, read in .txt and create the array in assembly.
    }
}


Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){//return BitArray2D to initialize arrays
    //Section to read in file for initialization
    ifstream fin;
    fin.open(setup_board_name);
    string line;
    vector<string> lines(board_size, "");
    int i = 0;
    while (getline(fin, line)) {
        lines[i] = line;
        cout << lines[i] << endl;
        i++;
    }
    int rows = lines.size();
    int cols = lines[0].size();
    fin.close();
    //End file read, ready to make arrays
    BitArray2D *array;
    array = new BitArray2D(rows,cols);

    return array;

}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if (x > board_size || x < 0) {
        return OUT_OF_BOUNDS;
    } else if (y > board_size || y < 0) {
        return OUT_OF_BOUNDS;
    } else if (player < 1 || player > MAX_PLAYERS) {
        throw ServerException("Bad Player Number.");
    } else {
        if(player == 1){
            player = 2;
        }
        else{
            player = 1;
        }
        string name = "player_" + to_string(player) + ".setup_board.txt";
        ifstream fin;
        fin.open(name);
        string line;
        vector<string> lines(board_size, "");
        int i = 0;
        while (getline(fin, line)) {
            lines[i] = line;
            cout << lines[i] << endl;
            i++;
        }
        fin.close();
        char spot = lines[x].at(y);
        cout <<spot << " In board at " << x << ","<<y<<endl;
        if (spot == 'B' || spot == 'D' || spot == 'R' || spot == 'S' || spot == 'C') {
            return HIT;
        } else {
            return MISS;
        }

    }
}


int Server::process_shot(unsigned int player) {
    if (player > 0 && player <= 2) {
        string name = "player_";
        name += to_string(player);
        name += ".shot.json";

        int x=0, y=0, result=0;
        std::ifstream f;
        f.open(name);
        if (!f) {
            return NO_SHOT_FILE;
        } else {
            {
                cereal::JSONInputArchive fin(f);
                fin(y,x);
                if ((x < board_size && x >= 0) && (y < board_size && y >= 0)) {
                    result = evaluate_shot(player,x,y);
                    /*if(player == 1){
                        result = evaluate_shot(player+1, x, y);
                    }
                    else{
                        result = evaluate_shot(player-1, x, y);
                    }*/

                } else {
                    cout << "OUT OF BOUNDS detected in process_shot" << endl;
                    result = OUT_OF_BOUNDS;
                }

            }
            string filename = "player_" + to_string(player) + ".result.json";
            ofstream fout;
            fout.open(filename);
            {
                cereal::JSONOutputArchive archive(fout);
                archive(CEREAL_NVP(result));
            }
            remove(name.c_str());
            return SHOT_FILE_PROCESSED;
        }
    } else {
        throw ServerException("Bad Player Number.");
    }

}