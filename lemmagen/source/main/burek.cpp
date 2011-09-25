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
#include <iostream>

extern "C" char *Lemmatize(const char *acWord);

extern "C" void LoadLemmatizer(const char *acFileName);

int main(int argc, char **argv){
	if (argc < 3) 
		std::cout << "Too few arguments for this program.\n  Usage: lemmatizeWrapper input_language_file.bin \"word_to_lemmatize\"";
	else {
		LoadLemmatizer(argv[1]);
		std::cout << "word: '" << argv[2] << "'\tlemma: '" << Lemmatize(argv[2]) << "'";
	}
}
