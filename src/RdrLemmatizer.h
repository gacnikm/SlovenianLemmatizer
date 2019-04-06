/******************************************************************************
This file is part of the lemmagen library. It gives support for lemmatization.
Copyright (C) 2006-2007 Matjaz Jursic <matjaz@gmail.com>

The lemmagen library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/
#pragma once

#define AllInOneFile

//-------------------------------------------------------------------------------------------
//includes
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdint>

#include "string.h"

#ifndef AllInOneFile
	#include "RdrLemmData.h"
#endif

//-------------------------------------------------------------------------------------------
//if data is not loaded from separate file define empty structure
#ifndef RrdLemmData
	#define RrdLemmData
	#define DATA_LEN 8
	#define DATA_TBL {0x0000000000000000}
#endif

//-------------------------------------------------------------------------------------------
//const variables that algorithm depends on
#define AddrLen 4
#define FlagLen 1
#define ModLen 1
#define LenSpecLen 1
#define CharLen 1
#define DataStart 0

#define RoundDataLen 8

#define BitDefault	0x00
#define BitAddChar	0x01
#define BitInternal	0x02
#define BitEntireWr	0x04

#define TypeRule		(BitDefault)
#define TypeRuleEw		(BitDefault | BitEntireWr)
#define TypeLeafAC		(BitDefault | BitAddChar)
#define TypeLeafACEw	(BitDefault | BitAddChar | BitEntireWr)
#define TypeIntr		(BitDefault | BitInternal)
#define TypeIntrAC		(BitDefault | BitAddChar | BitInternal)

//-------------------------------------------------------------------------------------------
//main data structure and it's length
static uint32_t iDataLenStatic = DATA_LEN;
static uint64_t abDataStatic[] = DATA_TBL;

//-------------------------------------------------------------------------------------------
//helper macros for nicer code and faster execution
#if AddrLen == 3
	#define GETDWORD(type, wVar, wAddr) \
				type wVar = *((uint32_t*) &abData[wAddr]) & 0x00FFFFF
#else
	#define GETDWORD(type, wVar, wAddr) \
				type wVar = *((uint32_t *) &abData[wAddr])
#endif

#define GETBYTEMOVE(type, bByte, iSize) \
			type bByte = abData[iAddr]; \
			iAddr += iSize

#define GETDWORDMOVE(type, wVar, iSize) \
			GETDWORD(type, wVar, iAddr);  \
			iAddr += iSize

#define GETSTRINGMOVE(type, acString, iSize) \
			type acString = new char[iSize+1]; \
			strncpy(acString, (char*) &abData[iAddr], iSize); \
			acString[iSize] = NULL; \
			iAddr += iSize

//-------------------------------------------------------------------------------------------

class RdrLemmatizer{
public:
	uint8_t *abData;
	uint32_t iDataLen;

public:
	RdrLemmatizer(uint8_t *abData, uint32_t iDataLen);
	RdrLemmatizer(const char *acFileName);
	RdrLemmatizer();
	~RdrLemmatizer();

	uint32_t SizeOfTree() const;

	char *Lemmatize(const char *acWord, char *acOutBuffer = nullptr) const;

	void LoadBinary(const char *acFileName);
	void LoadBinary(std::istream &is);
};
