#pragma once

#include <iostream>
#include <stdlib.h>
#include "Vector.h"
#include "Dummy.h"


bool isSorted(const Vector<int>& vec);

Vector<int>& randomIntVector(size_t length, int lowerBound, int upperBound);

struct Afonso;


std::ostream& operator<<(std::ostream& stream, const Afonso& afonso);

void vectorTester();
