#ifndef BLOCK_H
#define BLOCK_H

#include "Record.h"
#include <set>
#include <string>

class Block {
private:
	long quantity;
	std::set<Record*> recordsSet;

public:
	/**
	 * Default constructor. Just returns an instance.
	 */
	Block();

	/**
	 * Default destructor. 
	 */
	~Block();

	/**
	 * It gets a Record object and add it inside
	 * the set of Records.
	 *
	 * @param a Record object 
	 */
	void addRecordToBlockSet(Record* record);

	/**
	 * It gets a Record object and removes it from 
	 * Records set.
	 * 
	 * @param a Record object
	 */
	void removeRecordFromBlockSet(Record* record);

	/**
	 * It gets a zip code and use it to search for
	 * a Record object inside set.
	 * 
	 * @pram long for zip code
	 */
	void removeRecordFromBlockByZipCode(long zipCode);

	/**
	 * It gets a zip code and search for a Record with
	 * given value.
	 * If a Record with given zip code is inside set, this
	 * object'll be return. If not, it returns NULL.
	 * 
	 * @param a zip code
	 * @return object found or NULL
	 */
	Record* findRecordByZipCode(long zipCode);

	/**
	 * It returns how many Record objects there are 
	 * inside the set.
	 * 
	 * @return Record objects quantity
	 */
	long getRecordsSizeInsideBlockSet();

	/**
	 * It returns a reference to records set.
	 *
	 * @return reference for records set
	 */
	std::set<Record*> getRecordsSet();

	/**
     * It returns instance state in a string
     *
     * @return a string with attribute values.
     */
	std::string toString();
};

#endif
