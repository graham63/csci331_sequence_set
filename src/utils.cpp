#include "../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string>

long stringToLong(std::string cLikeString) {
	char* parsedString = new char[cLikeString.size() + 1];
	strcpy(parsedString, cLikeString.c_str());
	return strtol(parsedString, NULL, 10);
}

float stringToFloat(std::string cLikeString) {
	char* parsedString = new char(cLikeString.size() + 1);
	strcpy(parsedString, cLikeString.c_str());
	return strtod(parsedString, NULL);
}

char* intToString(int longNumber) {
	int stringSize = log10(longNumber) + 1;
	char* string = (char*) malloc(sizeof(stringSize));
	sprintf(string, "%d", longNumber);
	return string;
}

char* doubleToString(double doubleNumber) {
	char* buffer = (char*) malloc(sizeof(char) * 50);
	snprintf(buffer, 50, "%lf", doubleNumber);
	return buffer;
}
