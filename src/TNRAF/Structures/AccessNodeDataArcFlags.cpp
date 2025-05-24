/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#include <cstdio>
#include "AccessNodeDataArcFlags.h"

//______________________________________________________________________________________________________________________
AccessNodeDataArcFlags::AccessNodeDataArcFlags(
	unsigned int a,
	unsigned int b,
	unsigned int regionsCnt,
	unsigned short tnr_index
) :
	AccessNodeData(a, b), regionFlags(regionsCnt, false), tnr_index(tnr_index) {
}

//______________________________________________________________________________________________________________________
AccessNodeDataArcFlags::AccessNodeDataArcFlags(
	unsigned int a,
	unsigned int b,
	unsigned int regionsCnt,
	uint32_t regFlags,
	std::vector<unsigned int>& powersOf2
) :
	AccessNodeData(a, b), regionFlags(regionsCnt, false) {
	for (unsigned int i = 0; i < regionsCnt; i++) {
		if (regFlags & powersOf2[i]) {
			regionFlags[i] = true;
		}
	}
}
