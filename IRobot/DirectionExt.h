//------------------------------
// DirectionExt.h
// optional extensions for
// the enum class Direction
//------------------------------

#ifndef _DIRECTION_EXT_H_
#define _DIRECTION_EXT_H_

// prefix++ for Direction, skipping Stay
inline Direction operator++(Direction& d) {
    d = (Direction)(((int)d + 1)%4);
    return d;
}

// returns the opposite direction
inline Direction opposite(Direction d) {
    static Direction opposite[] = {Direction::West, Direction::East, Direction::North, Direction::South, Direction::Stay};
    return opposite[(int)d];
}

// added for debug purposes
inline ostream& operator<<(ostream& out, Direction d) {
    static const char* desc[] = {"East", "West", "South", "North", "Stay"};
    return out << desc[(int)d];
}

extern Direction directions[4];

#endif