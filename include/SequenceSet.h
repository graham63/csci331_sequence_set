#ifndef SEQUENCESET_H
#define SEQUENCESET_H

#include <list>
#include "Block.h"
#include <string>
#include "Record.h"

class SequenceSet {
private:
	long recordsInCurrentBlock;
	bool isBlockListEmpty;
	long totalRecordsInsideSequenceSet;
	long recordsQuantityPerBlock;
	std::list<Block*> blockList;
	std::string fieldsName[6];

public:
	/**
	 * Default constructor.
	 *
	 */
	SequenceSet();
	
	/**
	 * Default destructor.
	 *
	 */
	~SequenceSet();

	/**
	 * It adds a record inside the sequence set
	 * structure.
	 * 
	 * @param a Record object
	 */
	void addRecord(Record* record);

	/**
	 * It gets a zip code and searchs for a 
	 * Record object and returns it.
	 *
	 * @param Record's zip code
	 * @return a Record object
	 */
	Record* queryRecordByZipCode(long zipCode);

	/**
	 * It returns how many Records there're inside
	 * each block.
	 * 
	 * @return Block's quantity of Records
	 */
	long getRecordsQuantityPerBlock();

	/**
	 * It removes a Record object using zipCode
	 * to search and find it to remove.
	 * 
	 * @param Record's zip code
	 */
	void removeRecordByZipCode(long zipCode);

	/**
	 * It retuns the current quantity of records inside 
	 * this whole structure.
	 * 
	 * @return how many records there're inside
	 */
	long getTotalRecordsInsideSequenceSet();

	/**
	 * It returns a reference to the block list.
	 *
	 * @return a reference to block list
	 */
	std::list<Block*> getBlockList();

	/**
	 * It returns a reference for fields names
	 * array.
	 * 
	 * @return reference to field names array
	 */
	std::string* getFieldsNames();

	/**
     * It returns instance state in a string
     *
     * @return a string with attribute values.
     */
	std::string toString();

};

#endif
