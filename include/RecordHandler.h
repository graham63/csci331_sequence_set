#ifndef RECORDHANDLER_H
#define RECORDHANDLER_H

#include "Record.h"
#include <string>

/**
 * It gets a Record's state string in csv format
 * to build a Record instance and returns it.
 *
 * @param state of a Record as csv
 * @return a Record object deserialized
 */
Record* csvRecordParser(std::string recordStateAsCSV);

/**
 * It gets a reference for a raw data set as string
 * and other string which is a the name to place the 
 * proper data set. 
 *
 * It's necessary due to some records are bad formated.
 *
 * @param raw data set reference
 * @param output data set reference
 */
void cleanDataSet(std::string inputDataSet, std::string outputDataset);

#endif
