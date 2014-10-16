/*
 * Tackle.cpp
 *
 *  Created on: Jun 20, 2014
 *      Author: Stephan Opfer
 */

#include "Tackle.h"

namespace alicaTests
{

	Tackle::Tackle() : alica::BasicBehaviour("Tackle")
	{
		this->callCounter = 0;
	}

	Tackle::~Tackle()
	{
	}

	void Tackle::run(void* msg)
	{
		cout << "Tackle was called " << callCounter++ << " times!" << endl;
	}

	void Tackle::initialiseParameters()
	{
	}

} /* namespace alica */
