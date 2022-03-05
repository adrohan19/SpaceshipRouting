// 950181F63D0A883F183EC0A5CC67B19928FE896A
//  project1.hpp
//  project1
//
//  Created by Aditya Dhir on 9/12/21.
//

#ifndef project1_hpp
#define project1_hpp

#include <stdio.h>
#include <stdint.h>

struct Coord {
    Coord():  type('.'), is_discovered(false), how_you_got_there('\0'), where_to_go('\0')
    {};
//    uint32_t x;
//    uint32_t y;
//    uint32_t z;
//    x(0), y(0), z(0)
    char type;
    bool is_discovered;
    char how_you_got_there;
    char where_to_go;
};


#endif /* project1_hpp */
