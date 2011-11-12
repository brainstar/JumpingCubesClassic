#ifndef _JCC_TYPES_H_
#define _JCC_TYPES_H_

#include <vector>
using namespace std;

class Element {
public:
	Element() {
		owner = 0;
		value = 1;
	}

	int x, y; // Coordinates
	int owner; // 0 := no owner, 1 - 4 := player id
	int value; // Actual value of this element
	int n; // Maximum value of this element
};

typedef vector<vector<Element> > Field;

#endif
