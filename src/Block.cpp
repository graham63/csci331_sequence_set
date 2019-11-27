#include "../include/Block.h"
#include <set>
#include <algorithm>
#include <string>
#include "../include/utils.h"

using namespace std;

////////////////////////////////////////////////

// Util variable on searching process.
long zipCodeToFind = 0;

/**
 * @brief a helpful function used in Records searching.
 * Return true if given Record is equal to zipCodeToFind
 * variable, and false if not.
 *
 * @param a Record object
 * @return true if is right Record, else if not.
 */
bool onSearchUtil(Record* record);
///////////////////////////////////////////////

Block::Block() {
  quantity = 0;
}

Block::~Block() {}

set<Record*>  Block::getRecordsSet() {
  return recordsSet;
}

Record* Block::findRecordByZipCode(long zipCode) {
  zipCodeToFind = zipCode;
  set<Record*>::iterator recordsIterator = std::find_if(recordsSet.begin(),
                                                        recordsSet.end(),
                                                        onSearchUtil);
  if(recordsIterator != recordsSet.end())
    return (*recordsIterator);
  return NULL;
}

void Block::addRecordToBlockSet(Record* record) {
  recordsSet.insert(record);
  quantity++;
}

void Block::removeRecordFromBlockSet(Record* record) {
  recordsSet.erase(record);
  quantity--;
}

void Block::removeRecordFromBlockByZipCode(long zipCode) {
  recordsSet.erase(findRecordByZipCode(zipCode));
  quantity--;
}

long Block::getRecordsSizeInsideBlockSet() {
  return quantity;
}

string Block::toString() {
  string blockSatate = "Block {\n quantity: " + string(intToString(quantity)) + ", \n recordsSet { \n";
  for(set<Record*>::iterator i = recordsSet.begin(); i != recordsSet.end(); i++)
    blockSatate += "\t" +(*i)->toString();
  blockSatate += " }\n}\n";
  return blockSatate;
}

bool onSearchUtil(Record* record) {
  return record->getZipCode() == zipCodeToFind;
}
