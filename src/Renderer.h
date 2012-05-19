/*
 * Renderer.h
 *
 *  Created on: 30.10.2011
 *      Author: Christian M.
 */

#ifndef _JCC_RENDERER_H
#define _JCC_RENDERER_H

#include "JCCTypes.h"
using namespace std;

class Renderer {
public:
	Renderer() {}
	
	virtual void push(Map m) = 0;
	virtual void flush() = 0;
};

#endif
