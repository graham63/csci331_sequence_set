#include "../include/SequenceSet.h"
#include <list>
#include "../include/Block.h"
#include <string>
#include "../include/Record.h"
#include "../include/utils.h"

using namespace std;

SequenceSet::SequenceSet() {
	isBlockListEmpty = true;
	recordsQuantityPerBlock = 1000; //half
	totalRecordsInsideSequenceSet = 0;

	recordsInCurrentBlock = 0;

	fieldsName[0] = "Zip Code";
	fieldsName[1] = "Place Name";
	fieldsName[2] = "State";
	fieldsName[3] = "County";
	fieldsName[4] = "Latitude";
	fieldsName[5] = "Longitute";
}

SequenceSet::~SequenceSet() {}

string* SequenceSet::getFieldsNames() {
	return fieldsName;
}

long  SequenceSet::getTotalRecordsInsideSequenceSet() {
	return totalRecordsInsideSequenceSet;
}

list<Block*> SequenceSet::getBlockList() {
	return blockList;
}

void SequenceSet::addRecord(Record* record) {
	if(recordsInCurrentBlock == 0)
		blockList.push_back(new Block());

	for(list<Block*>::iterator i = blockList.begin(); i != blockList.end(); i++) {
		Block* currentBlock = (*i);
		if(currentBlock->getRecordsSizeInsideBlockSet() < recordsQuantityPerBlock) {
			currentBlock->addRecordToBlockSet(record);
			totalRecordsInsideSequenceSet++;
			recordsInCurrentBlock++;
			if(recordsInCurrentBlock == recordsQuantityPerBlock)
				recordsInCurrentBlock = 0;
		}
	}
}

Record* SequenceSet::queryRecordByZipCode(long zipCode) {
	Record* toReturn = NULL;
	for(list<Block*>::iterator i = blockList.begin(); i != blockList.end(); i++) {
		Block* currentBlock = (*i);
		Record* record = currentBlock->findRecordByZipCode(zipCode);
		if(record == NULL)
			continue;
		else {
			toReturn = record;
			break;
		}
	}
	return toReturn;
}

void SequenceSet::removeRecordByZipCode(long zipCode) {
	for(list<Block*>::iterator i = blockList.begin(); i != blockList.end(); i++) {
		Block* currentBlock = (*i);
		currentBlock->removeRecordFromBlockByZipCode(zipCode);
	}
	totalRecordsInsideSequenceSet--;
}

long SequenceSet::getRecordsQuantityPerBlock() {
	return recordsQuantityPerBlock;
}

string SequenceSet::toString() {
	string state = "SequenceSet {\n totalRecordsInsideSequenceSet: " + string(intToString(totalRecordsInsideSequenceSet));
	state += ",\n recordsInCurrentBlock: " + string(intToString(recordsInCurrentBlock)) + ",\n";
	state += ",\n recordsQuantityPerBlock: " + string(intToString(recordsQuantityPerBlock)) + ",\n";
	state += " blockList {\n";
	for(list<Block*>::iterator i = blockList.begin(); i != blockList.end(); i++)
		state += "\t " + (*i)->toString();
	state += "  }\n}\n";
	return state;
}
