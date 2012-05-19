#ifndef _JCC_TYPES_H_
#define _JCC_TYPES_H_

#include <vector>
using namespace std;

class Field {
public:
	Field() {
		x = 0;
		y = 0;
		owner = 0;
		value = 1;
		n = 0;
	}

	Field& operator=(const Field &rhs) {
		x = rhs.x;
		y = rhs.y;
		owner = rhs.owner;
		value = rhs.value;
		n = rhs.n;

		return *this;
	}

	int x, y; // Coordinates
	int owner; // 0 := no owner, 1 - 4 := player id
	int value; // Actual value of this element
	int n; // Maximum value of this element
};

class Map {
public:
	Map() {
		iSize = 0;
		Field f;
		m.resize(size, vector<Field>(size, f));
	}
	
	Map& operator=(const Map &rhs) {
		size = rhs.size;
		m = rhs.m;
	}

	vector<Field>& operator[] (const int i) {
		return m[i];
	}
	
	void resize(int i) {
		iSize = i;
		Field f;
		m.resize(iSize, vector<Field>(size, f));
	}

	int size() {
		return iSize;
	}
	
	int size(int s) {
		iSize = s;
		return iSize;
	}

private:
	int iSize;
	vector<vector<Field> > m;
};

//typedef vector<vector<Field> > Map;

#endif
