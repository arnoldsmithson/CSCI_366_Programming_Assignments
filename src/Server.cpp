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
    } else {
        this->board_size = board_size;
        string line;
        vector<string> lines(board_size, "");
        this->p1_setup_board.open(p1_setup_board);
        this->p2_setup_board.open(p2_setup_board);
/*        int i = 0;
            while ( getline (this->p1_setup_board,line) )
            {
                lines[i] = line;
                i++;
            }
        this->p1_setup_board.close();
        ofstream fout;
        fout.open(p1_setup_board);
        for (int j = 0; j < lines.size(); ++j) {
            string fileLine;
            switch(lines[j].at(lines[j].length()-1)){
                case 'D':
                    fileLine += "DD";
                    for (int k = 0; k < board_size-2; ++k) {
                        fileLine += " ";
                    }
                    break;
                case 'C':
                    break;
                case 'R':
                    break;
                case 'S':
                    break;
                case 'B':
                    break;
            }
        }
        while ( getline (this->p2_setup_board,line) )
        {
            cout << line << '\n';
        }
        this->p2_setup_board.close();*/

        cout << "Working parameters" << endl;
    }

}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if (x > board_size || x < 0) {
        return OUT_OF_BOUNDS;
    } else if (y > board_size || y < 0) {
        return OUT_OF_BOUNDS;
    } else if (player < 1 || player > MAX_PLAYERS) {
        throw ServerException("Bad Player Number.");
    } else {
        string name = "player_" + to_string(player) + ".setup_board.txt";
        ifstream fin;
        fin.open(name);
        string line;
        vector<string> lines(board_size, "");
        int i = 0;
        while (getline(fin, line)) {
            lines[i] = line;
            i++;
        }
        fin.close();
        char spot = lines[x].at(y);
        cout <<spot << "In board" << endl;
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
        if (!f.good()) {
            return NO_SHOT_FILE;
        } else {
            {
                cereal::JSONInputArchive fin(f);
                fin(x, y);
                if ((x < board_size && x >= 0) && (y < board_size && y >= 0)) {
                    result = evaluate_shot(player, x, y);
                } else {
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