//------------------------------
// Point.h
//------------------------------

#ifndef _POINT_H_
#define _POINT_H_

#include "Direction.h"
#include "DirectionExt.h"

class Point {
    int x, y;
    struct NeighboursIterator {
    	Direction d;
		const Point& p;
		NeighboursIterator(Direction _d, const Point& _p): d(_d), p(_p) {}
		NeighboursIterator operator++() {
			d = (Direction)((int)d + 1); // Stay represent end
			return *this;
		}
		Point operator*()const {
			Point neighbour = p;
			neighbour.move(d);
			return neighbour;
		}
		bool operator!=(NeighboursIterator other)const {
			return !(other.p == p && other.d == d);
		}
	};
	struct Neighbours {
		NeighboursIterator begin() {return NeighboursIterator(directions[0], p);}
		NeighboursIterator end() {return NeighboursIterator(Direction::Stay, p);}
		const Point& p;
		Neighbours(const Point& _p): p(_p) {}
	};
public:
    Point(int _x, int _y) : x(_x), y(_y) {}
    Point distance(const Point& other)const {
        return Point(x - other.x, y - other.y);
    }
    int getX()const {return x;}
    int getY()const {return y;}
    Point& adjustToScreen(size_t width, size_t height) {
        x = (x + ((x<0? -x: x) / width + 1) * width) % width;
        y = (y + ((y<0? -y: y) / height + 1) * height) % height;
        return *this;
    }
    void move(int difx, int dify) {
        x += difx;
        y += dify;
    }
    
    void move(Direction d);
    
    // operator< required by map<Point, T>
    bool operator<(const Point& other)const {
        return (x < other.x || (x == other.x && y < other.y)); 
    }
    bool operator==(const Point& other)const {
        return (x == other.x && y == other.y); 
    }
	Neighbours neighbours()const {return Neighbours(*this);}
    Point neighbour(Direction d)const {
        Point p = *this;
        p.move(d);
        return p;
    }
    friend ostream& operator<<(ostream& out, const Point& p) {
        return out << "(" << p.x << "," << p.y << ")";
    }
};


#endif