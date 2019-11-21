#include "../include/Record.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "../include/utils.h"

using namespace std;

Record::Record(long zipCode, string state,
               string county, string placeName,
               double latitude, double longitude) {
  this->zipCode = zipCode;
  this->state = state;
  this->county = county;
  this->placeName = placeName;
  this->latitude = latitude;
  this->longitude = longitude;
}

Record::Record() {
  this->zipCode = -505;
  this->state = "";
  this->county = "";
  this->placeName = "";
  this->latitude = 0;
  this->longitude = 0;
}

Record::~Record() {}

long Record::getZipCode() {
  return zipCode;
}

void Record::setZipCode(long zipCode) {
  this->zipCode = zipCode;
}

string Record::getState() {
  return state;
}

void Record::setState(string state) {
  this->state = state;
}

string Record::getCounty() {
  return county;
}

void Record::setCounty(string county) {
  this->county = county;
}

string Record::getPlaceName() {
  return placeName;
}

void Record::setPlaceName(string placeName) {
  this->placeName = placeName;
}

double Record::getLatitude() {
  return latitude;
}

void Record::setLatitude(double latitude) {
  this->latitude = latitude;
}

double Record::getLongitude() {
  return longitude;
}

void Record::setLongitude(double longitude) {
  this->longitude = longitude;
}

string Record::toString() {
  string state = "Record: { zipCode: " + string(intToString((int) zipCode));
  state += ", state: " + this->state;
  state += ", county: " + county;
  state += ", placeName: " + placeName;
  state += ", latitude: " + string(doubleToString(latitude));
  state += ", longitude: " + string(doubleToString(longitude)) + " } \n";
  return state;
}
