/*
 * main.cpp
 *
 *  Created on: 02.10.2011
 *      Author: Christian M.
 */

#include <iostream>
#include <vector>
#include <string>
#include "renderers.h"

#ifdef CMAKE_HAVE_QT
#include "QtRenderer2d/main_qt.h"
#endif

#ifdef CMAKE_HAVE_SDL
// TODO: add SDLRenderer-Support here
#endif

using namespace std;

int main(int argc, char *argv[])
{
	vector<string> r = vector<string>();

	#ifdef CMAKE_HAVE_QT
		r.push_back("Qt");
	#endif
	#ifdef CMAKE_HAVE_SDL
//		r.push_back("SDL");
	#endif
	
	if (!r.size()) {
		cerr << "ERROR: No render engine available!" << endl;
		return 0;
	}
	
	cout << "Available render engines: " << endl;
	
	for (int i = 0; i < r.size(); i++) {
		cout << "	" << i + 1 << ". " << r[i] << endl;
	}
	
	int i;
	if (r.size() > 1) {
		cout << endl << "Choose your fate: ";
		cin >> i;
		i -= 1;
		
		if (i < 0 || i >= r.size()) {
			cerr << "ERROR: Engine " << i << " not available" << endl;
			return 0;
		}
	}
	else i = 0;
	
	#ifdef CMAKE_HAVE_QT
		if (r[i] == "Qt") {
			cout << "Starting Jumping Cubes Classic using Qt ..." << endl;
			main_qt(argc, argv);
		}
	#endif
	#ifdef CMAKE_HAVE_SDL
		if (r[i] == "SDL") {
			cout << "Starting Jumping Cubes Classic using SDL ..." << endl;
			//TODO: add SDLRenderer support here
		}
	#endif
}
