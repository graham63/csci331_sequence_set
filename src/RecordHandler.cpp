#include "../include/RecordHandler.h"
#include "../include/Record.h"
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iterator>
#include "../include/utils.h"

using namespace std;

/**
 * @brief Helps in process of splitting
 * a string (see below function).
 *
 * @param a string instance.
 * @param a char used to split
 * @param a template object hold modification in string.
 */
template<typename Out>
void split(const std::string &s, char delim, Out result);

/**
 * @brief Gets a string to be split and a char to be
 * used as splitting token.
 *
 * @param a string to be splited
 * @param a char to be the token
 * @return a vector wich elements are parts of string
 */
std::vector<std::string> split(const std::string &s, char delim);

/**
 * @brief Gets two strings and whenever the second
 * string appears in first, it'll be removed.
 *
 * @param string to be modified
 * @param substring
 */
void removeSubstrings(std::string& s, std::string& p);

/**
 * @brief Gets a number and returns how many digits it has.
 *
 * @param an int
 * @return how many digits it has
 */
int digitsOfNumber(int number);

/**
 * @brief Gets a number 'digits' and returns other number in form:
 * 10 ^ 'digits'.
 *
 * @param how many digits have a number
 * @return a number in form 10 ^ 'digits'
 */
double getRightValue(int digits);

/**
 * @brief Gets a latitude/longitude's parts in string format
 * and reuturns its proper value as a double.
 *
 * @param first part of latitude/longitude
 * @param second part of latitude/longitude
 * @return number in proper format
 */
double parseFloatingPointString(string decimalString, string floatingPointString);
//////////////////IMPLEMENTATION/////////////////
template<typename Out>
void split(const std::string &s, char delim, Out result) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

void removeSubstrings(string& s, string& p) {
  string::size_type n = p.length();
  for (string::size_type i = s.find(p);
       i != string::npos;
       i = s.find(p))
    s.erase(i, n);
}

int digitsOfNumber(int number) {
  int digits = 0;
  while(number > 0) {
    number /= 10;
    digits++;
  }
  return digits;
}

double getRightValue(int digits) {
  return pow(10, digits);
}

double parseFloatingPointString(string decimalString, string floatingPointString) {
  string pattern = "\"";
  removeSubstrings(floatingPointString, pattern);

  int floatingPointerStringPart = 0; // to handle the first zero like 0.0654
  int digitsOfStringNumber = 0;

  removeSubstrings(decimalString, pattern);
  double properNumber = stringToFloat(decimalString);

  if(properNumber >= 0) {
    floatingPointerStringPart = floatingPointString.size();
    digitsOfStringNumber = digitsOfNumber(stringToFloat(floatingPointString));
    if(floatingPointerStringPart == digitsOfStringNumber)
      properNumber += stringToFloat(floatingPointString) / getRightValue(digitsOfStringNumber);
    else
      properNumber += stringToFloat(floatingPointString) / getRightValue(digitsOfStringNumber + 1);
  } else {
    properNumber = abs(properNumber);

    floatingPointerStringPart = floatingPointString.size();
    digitsOfStringNumber = digitsOfNumber(stringToFloat(floatingPointString));
    if(floatingPointerStringPart == digitsOfStringNumber)
      properNumber += stringToFloat(floatingPointString) / getRightValue(digitsOfStringNumber);
    else
      properNumber += stringToFloat(floatingPointString) / getRightValue(digitsOfStringNumber + 1);

    properNumber = properNumber * (-1);
  }
  return properNumber;
}

Record* csvRecordParser(string recordStateAsCSV) {
  vector<string> parts = split(recordStateAsCSV, ',');
  long zipCode = stringToLong(parts[0]);
  string placeName = parts[1];
  string state = parts[2];
  string county = parts[3];
  double latitude = parseFloatingPointString(parts[4], parts[5]);
  double longitude = parseFloatingPointString(parts[6], parts[7]);
  return new Record(zipCode, state, county, placeName, latitude, longitude);
}

void cleanDataSet(string inputDataSet, string outputDataset) {
  fstream inputStream;
  ofstream outfile;
  outfile.open(outputDataset.c_str());
  inputStream.open(inputDataSet.c_str());
  if(inputStream.is_open()) {
    string line;
    while(!inputStream.eof()) {
      getline(inputStream, line);
      if(split(line, ',').size() == 8) {
        outfile << line << endl;
      }
    }
  }

  inputStream.close();
  outfile.close();
}
