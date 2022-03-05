// 950181F63D0A883F183EC0A5CC67B19928FE896A
//  project1.cpp
//  project1
//
//  Created by Aditya Dhir on 9/12/21.
//


#include "project1.h"
#include <iostream>
#include <vector>
#include <deque>
#include <getopt.h>
#include <string>
#include <tuple>

using namespace std;

class Spaceship{
public:
    void get_options(int argc, char** argv);
    void read_M();
    void read_L();
    void search();
    void print();
    
private:
    char policy = '\0';
    string out_type = "M";
    uint32_t dimensions;
    uint32_t levels;
    tuple<uint32_t, uint32_t, uint32_t> start;
    tuple<uint32_t, uint32_t, uint32_t> hangar;
    void print_m();
    void print_l();
    //representation of map
    vector <vector<vector<Coord> > > map;
    // search container
    deque <tuple<uint32_t,uint32_t,uint32_t>> container;
    void add_to_container(deque<tuple<uint32_t,uint32_t,uint32_t> > &in, const uint32_t &z, const uint32_t &y, const uint32_t &x);
};


int main(int argc, char** argv) {
    // helps in efficiency
    ios_base::sync_with_stdio(false);
    Spaceship ship;
    char format;
    cin >> format;
    if(format == 'M'){
        ship.read_M();
    }
    else if(format == 'L'){
        ship.read_L();
    }
    ship.get_options(argc, argv);
    ship.search();
    ship.print();
    return 0;
}

void Spaceship::get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    opterr = false;
    struct option longOpts[] = {{ "output", required_argument, nullptr, 'o'},
                                { "stack", no_argument, nullptr, 's' },
                                { "queue", no_argument, nullptr, 'q'},
                                { "help", no_argument, nullptr, 'h' },
                                { nullptr, 0, nullptr, '\0' }};
    while ((option = getopt_long(argc, argv, "o:sqh",
            longOpts, &option_index)) != -1) {
        switch (option) {
            case 'o':
                out_type = optarg;
                if ((out_type != "M") && (out_type != "L")){
                    cerr << "Invalid output mode specified" <<endl;
                    exit(1);
                }
                break;
            case 's':
                if (policy != '\0'){
                    cerr << "Multiple routing modes specified" <<endl;
                    exit(1);
                }
                policy = 's';
                break;
            case 'q':
                if (policy != '\0'){
                    cerr << "Multiple routing modes specified" <<endl;
                    exit(1);
                }
                policy = 'q';
                break;
            case 'h':
                cout << "here to help!";
                exit(0);
        }
    }
}

void Spaceship::add_to_container(deque<tuple<uint32_t,uint32_t,uint32_t> > &in, const uint32_t &z, const uint32_t &y, const uint32_t &x){
    if (policy == 's') {
        in.push_front(make_tuple(z,y,x));
    }
    else if (policy == 'q') {
        in.push_back(make_tuple(z,y,x));
    }
    else {
        cerr << "No routing mode specified" <<endl;
        exit(1);
    }
}

void Spaceship::read_M(){
    ios_base::sync_with_stdio(false);
    cin >> levels;
    cin >> dimensions;
    map.resize(levels);
    for(uint32_t z=0;z<levels;++z){
        map[z].resize(dimensions);
        for(uint32_t y=0;y<dimensions;++y){
            bool broken = false;
            map[z][y].resize(dimensions);
            for(uint32_t x=0;x<dimensions;++x){
                if (broken) {
                    broken = false;
                    x--;
                }
                char temp;
                cin >> temp;
                if (temp == '/') {
                    string gone;
                    getline(cin, gone);
                    broken = true;
                    continue;
                }
                Coord inputter;

                inputter.type = temp;
                map[z][y][x] = inputter;

                if((map[z][y][x].type != 'S') && (map[z][y][x].type != '.') && (map[z][y][x].type != '#') && (map[z][y][x].type != 'H') && (map[z][y][x].type != 'E')){
                    cerr << "Invalid map character" << endl;
                    exit(1);
                }
                if (map[z][y][x].type == 'S') {
                    start = make_tuple(z,y,x);
                }
                else if (map[z][y][x].type == 'H') {
                    hangar = make_tuple(z,y,x);
                }
            }
        }
    }
}

void Spaceship::read_L(){

    cin >> levels;
    cin >> dimensions;
    
    char throwaway;
    cin >> throwaway;
    
    map.resize(levels);
    for(uint32_t z=0;z<levels;++z){
        map[z].resize(dimensions);
        for(uint32_t y=0;y<dimensions;++y){
            map[z][y].resize(dimensions);
            for(uint32_t x=0;x<dimensions;++x){
                Coord inputter;
                inputter.type = '.';
                map[z][y][x] = inputter;
            }
        }
    }
    string line;

    while (getline(cin, line)) {
        if (line[0] == '/') {
            continue;
        }
        uint32_t d = 0;
        int commas = 0;
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t z = 0;
        char type;
        for(uint32_t i = 1; i < line.length()-1; i++){
            char c = line.at(i);
            if (c != ',') {
                d = d * 10 + static_cast<uint32_t>(c - '0');
            }
            else {
                if (commas == 0) {
                    z = d;
                }
                else if (commas == 1) {
                    y = d;
                }
                else if (commas == 2) {
                    x = d;
                    break;
                }
                commas++;
                d = 0;
            }
            
        }
        type = line.at(line.length()-2);
        if(x >= dimensions){
            cerr << "Invalid map column" << endl;
            exit(1);
        }
        else if(y >= dimensions){
            cerr << "Invalid map row" << endl;
            exit(1);
        }
        else if(z >= levels){
            cerr << "Invalid map level" << endl;
            exit(1);
        }
        map[z][y][x].type = type;
        if((map[z][y][x].type != 'S') && (map[z][y][x].type != '.') && (map[z][y][x].type != '#') && (map[z][y][x].type != 'H') && (map[z][y][x].type != 'E')){
            cerr << "Invalid map character" <<endl;
            exit(1);
        }
        if (map[z][y][x].type == 'S'){
            start = make_tuple(z,y,x);
//            start.is_discovered = true;
//
        }
        else if (map[z][y][x].type == 'H') {
            hangar =make_tuple(z,y,x);
        }
    }
}

void Spaceship::search() {
//    cout << "pushing front start" <<endl;
    map[get<0>(start)][get<1>(start)][get<2>(start)].is_discovered = true;
    add_to_container(container, get<0>(start), get<1>(start), get<2>(start));
    while (!(container.empty())) {
        tuple<uint32_t, uint32_t, uint32_t> point = container.front();
        uint32_t z = get<0>(point);
        uint32_t y = get<1>(point);
        uint32_t x = get<2>(point);
        Coord current = map[z][y][x];
        container.pop_front();
        if(current.type == 'H'){
            break;
        }
        if (y != 0 && map[z][y-1][x].type != '#' && !map[z][y-1][x].is_discovered) {

            Coord &N = map.at(z).at(y-1).at(x);
            N.is_discovered = true;
            N.how_you_got_there = 'n';
            add_to_container(container, z, y-1, x);
            if(N.type == 'H'){

                
                break;
            }
        }
        if (x != dimensions - 1 && map[z][y][x+1].type != '#' && !map[z][y][x+1].is_discovered) {
            Coord &E = map.at(z).at(y).at(x+1);
            E.is_discovered = true;
            E.how_you_got_there = 'e';
            add_to_container(container, z, y, x+1);
            if(E.type == 'H'){

                
                break;
            }
        }
         if (y != dimensions - 1 && map[z][y+1][x].type != '#' && !map[z][y+1][x].is_discovered) {


            Coord &S = map.at(z).at(y+1).at(x);

            S.is_discovered = true;
            S.how_you_got_there = 's';
            add_to_container(container, z, y+1, x);

            if(S.type == 'H'){
                break;
            }
        }
        if (x != 0 && map[z][y][x-1].type != '#' && !map[z][y][x-1].is_discovered) {


            Coord &W = map.at(z).at(y).at(x-1);
            W.is_discovered = true;
            W.how_you_got_there = 'w';
            add_to_container(container, z, y, x-1);
            if(W.type == 'H'){
                break;
            }
        }

        if (current.type == 'E') {
            for(uint32_t i = 0; i<levels; i++) {

                if(!map[i][y][x].is_discovered && map[i][y][x].type == 'E') {

                    map.at(i).at(y).at(x).how_you_got_there = static_cast<char>('0' + z);
                    map[i][y][x].is_discovered = true;
                   //                    cout << "pushing front" <<endl;
                    add_to_container(container, i,y,x);
                }
            }
        }
    }
}

void Spaceship::print_m() {
    
    cout << "Start in level " << get<0>(start) << ", row " << get<1>(start) << ", column " << get<2>(start) <<endl;
    // cout << "//path taken" <<endl;
    for(uint32_t i =0; i < levels; i++) {
        cout << "//level " << i << endl;
        for(uint32_t j = 0; j < dimensions; j++){
            for(uint32_t k = 0; k <dimensions; k++){
                if (map[i][j][k].where_to_go != '\0'){
                    cout << map[i][j][k].where_to_go;
                }
                else {
                    cout << map[i][j][k].type;
                }
            }
            cout << endl;
        }
    }
}

void Spaceship:: print_l() {
 
    cout << "//path taken" <<endl;
    if(map[get<0>(hangar)][get<1>(hangar)][get<2>(hangar)].how_you_got_there == '\0'){
        return;
    }
    uint32_t z = get<0>(start);
    uint32_t y = get<1>(start);
    uint32_t x = get<2>(start);

    Coord ongoing = map[z][y][x];
    while (ongoing.type != 'H') {
        cout << '(' << z << ',' << y << ',' << x << ',' << ongoing.where_to_go << ')' << endl;
        
        if (ongoing.where_to_go == 's') {
            ongoing = map[z][y+1][x];
            y++;
        }
        else if (ongoing.where_to_go == 'n') {
         
            ongoing = map[z][y-1][x];
            y--;
        }
        else if (ongoing.where_to_go == 'e') {
            
            ongoing = map[z][y][x+1];
            x++;
        }
        else if (ongoing.where_to_go == 'w') {
            
            ongoing = map[z][y][x-1];
            x--;
        }
        else if (ongoing.where_to_go != '\0'){
            z = static_cast<uint32_t>(ongoing.where_to_go - '0');
            ongoing = map[static_cast<uint32_t>(ongoing.where_to_go - '0')][y][x];
            
        }
    }
}

void Spaceship::print() {
//    out.clear();
//    out.push_front(hangar);
    if(map[get<0>(hangar)][get<1>(hangar)][get<2>(hangar)].how_you_got_there == '\0'){
        if (out_type == "L")  {
            print_l();
        }
        else {
            print_m();
        }
        return;
    }
    uint32_t z = get<0>(hangar);
    uint32_t y = get<1>(hangar);
    uint32_t x = get<2>(hangar);
    Coord ongoing = map[z][y][x];
    while (ongoing.type != 'S') {
        if (ongoing.how_you_got_there == 's') {
            map[z][y-1][x].where_to_go = 's';
            ongoing = map[z][y-1][x];
            y--;
        }
        else if (ongoing.how_you_got_there == 'n') {
            map[z][y+1][x].where_to_go = 'n';
            ongoing = map[z][y+1][x];
            y++;
        }
        else if (ongoing.how_you_got_there == 'e') {
            map[z][y][x-1].where_to_go = 'e';
            ongoing = map[z][y][x-1];
            x--;
        }
        else if (ongoing.how_you_got_there == 'w') {
            map[z][y][x+1].where_to_go = 'w';
            ongoing = map[z][y][x+1];
            x++;
        }
        else if (ongoing.how_you_got_there != '\0'){
            
            map[static_cast<uint32_t>(ongoing.how_you_got_there - '0')][y][x].where_to_go = static_cast<char>(z + '0');
            
            //static_cast<uint32_t>
            z = static_cast<uint32_t>(ongoing.how_you_got_there - '0');
            ongoing = map[static_cast<uint32_t>(ongoing.how_you_got_there - '0')][y][x];
        }
    }
    
    if (out_type == "L")  {
        print_l();
    }
    else {
        print_m();
    }
}

// create 3D vector of size (dimensions, dimensions, levels)


//coordinate list: (level,row,column,character)

// add starting location to search container and mark as discovered
// while (search container is not empty) {

// Q-mode:
// put starting location coordinates in search container.
// set it as current location, pop it from search container, and investigate around.
// pop whatever is at the front
//if (type == 'l') {
//    cin >> coordinates;
//}
//deque <

