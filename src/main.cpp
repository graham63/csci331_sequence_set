#include "../include/RecordHandler.h"
#include "../include/Record.h"
#include "../include/SequenceSet.h"
#include "../include/PersistenceHandler.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iterator>
#include "../include/utils.h"

using namespace std;

// Some define flags to reference interface operations
#define INSERT_RECORD 1
#define DELETE_RECORD 2
#define MODIFY_FIELD_IN_RECORD 3
#define DISPLAY_SPECIFIC_FIELD 4
#define DISPLAY_RECORD 5
#define CHECK_CONSISTENCY 6
#define CLOSE_APP 7
const string DATA_SET_PATH = "data/data_set.csv";
string givenDataSetPath;
string backupDataSetPath = "data/data_set_backup.csv";

int currentKey = 0;

#define DEBUG if(true)

// Sequence set object used
SequenceSet* sequenceSet;

// Show header
void showHeader();

// Show menu
void showMenu();

// It creates a backup for given data set
void createDataSetBackup(SequenceSet* sequenceSet);

// string reference to store feedback message
string feedBackMessage;

// flag used to say if system should show feedback message
bool shouldShowFeedBackMessage = false;

//**** These functions works with app's obligations. ****//
void insertRecord();                                     //
void deleteRecord();                                     //
void modifyFieldInRecord();                              //
void displayRecord();                                    //
void displaySpecificField();                             //
void openRecordsFromPath();                              //
void checkFileConsistency();                             //
string checkFilePath(string filePath);                   //
///////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
  //cleanDataSet("data_set.csv", "proper_data_set.csv");

  if(argc == 1)
    givenDataSetPath = "data/data_set.csv";
  else
    givenDataSetPath = checkFilePath(argv[1]);

  sequenceSet = bufferizeDataSet(givenDataSetPath);
  createDataSetBackup(sequenceSet);
  bool shouldRun = true;
  while(shouldRun)
  {
    system("clear || cls");
    string input;
    int option;
    showHeader();
    showMenu();

    if(shouldShowFeedBackMessage)
    {
      cout << "| System says: " << feedBackMessage << " |" << endl;
      shouldShowFeedBackMessage = false;
    }
    cout << "Type an option >>> ";
    bool isTryingToGetInput = true;
    while(isTryingToGetInput)
    {
      try
      {
        getline(cin, input);
        option = stringToLong(input);
        isTryingToGetInput = false;
      }
      catch(...)
      {
        system("clear || cls");
        showHeader();
        showMenu();
        cout << "| System says: " << "Bad input!" << " |" << endl;
        cout << "Type an option >>> ";
      }
    }

    switch(option)
    {
    case INSERT_RECORD:
      insertRecord();
      break;
    case DELETE_RECORD:
      deleteRecord();
      break;
    case MODIFY_FIELD_IN_RECORD:
      modifyFieldInRecord();
      break;
    case DISPLAY_SPECIFIC_FIELD:
      displaySpecificField();
      break;
    case DISPLAY_RECORD:
      displayRecord();
      break;
    case CLOSE_APP:
      shouldRun = false;
      break;
    case CHECK_CONSISTENCY:
      checkFileConsistency();
      break;
    default:
      shouldShowFeedBackMessage = true;
      feedBackMessage = "Invalid paramenter";
      break;
    }// End switch(option)
  }// End while(shouldRun)
  updateDataSet(sequenceSet, givenDataSetPath);
  delete sequenceSet;

  return 0;

}// End main()

string checkFilePath(string filePath)
{
  ifstream inputStream;
  inputStream.open(filePath.c_str());
  while(!inputStream.is_open())
  {
    cout << "No file found at " << filePath << ". Try again: ";
    getline(cin, filePath);
    inputStream.open(filePath.c_str());
  }
  inputStream.close();

  return filePath;
}// End checkFilePath

void insertRecord() {
  cout << "Type zip code: ";
  long zipCode;
  cin >> zipCode;
  getchar();
  cout << "Type state: ";
  string state;
  getline(cin, state);
  cout << "Type county: ";
  string county;
  getline(cin, county);
  string placeName;
  cout << "Type place name: ";
  getline(cin, placeName);
  cout << "Type latitute: ";
  double latitute;
  cin >> latitute;
  getchar();
  double longitude;
  cout << "Type longitude: ";
  cin >> longitude;
  getchar();
  Record* givenRecord = new Record(zipCode, state, county, placeName, latitute, longitude);
  DEBUG cout << givenRecord->toString();
  sequenceSet->addRecord(givenRecord); //memory
  appendRecord(givenRecord, givenDataSetPath); // disc
}

void deleteRecord() {
  if(sequenceSet->getTotalRecordsInsideSequenceSet() > 0) {
    cout << "Type record zip code: ";
    long zipCode;
    cin >> zipCode;
    getchar();
    Record* toRemove = sequenceSet->queryRecordByZipCode(zipCode);
    while(toRemove == NULL) {
      cout << "Was not found any records with given zip code. Try again: ";
      long zipCode;
      cin >> zipCode;
      getchar();
      toRemove = sequenceSet->queryRecordByZipCode(zipCode);
    }
    sequenceSet->removeRecordByZipCode(zipCode);
  } else {
    shouldShowFeedBackMessage = true;
    feedBackMessage = "There are no records inside the set.";
  }
}

void checkFileConsistency() {
  if(sequenceSet->getTotalRecordsInsideSequenceSet() > 0) {
    ifstream inputStream;
    inputStream.open(givenDataSetPath);
    if(inputStream.is_open()) {
      cout << "Record's file is consistent and available to use\n";
      cout << "Press any key to get back\n";
      getchar();
      inputStream.close();
      return;
    } else {
      cout << "Problem on handle record's file. Activating backup file.\n";
      sequenceSet = bufferizeDataSet(backupDataSetPath);
      inputStream.close();
      getchar();
      return;
    }
  } else {
    shouldShowFeedBackMessage = true;
    feedBackMessage = "There are no records inside the set.";
  }
}

void modifyFieldInRecord() {
  if(sequenceSet->getTotalRecordsInsideSequenceSet() > 0) {
    cout << "Type record zip code: ";
    long zipCode;
    cin >> zipCode;
    getchar();
    Record* recordToChange = sequenceSet->queryRecordByZipCode(zipCode);
    while(recordToChange == NULL) {
      cout << "Was not found any record with given zip code. Type it again: \n";
      cout << ">>> ";
      cin >> zipCode;
      getchar();
      recordToChange = sequenceSet->queryRecordByZipCode(zipCode);
    }
    DEBUG cout << recordToChange->toString();
    cout << "Type what you want to change: \n";
    cout << "\t1 Place name" << endl;
    cout << "\t2 State" << endl;
    cout << "\t3 County" << endl;
    cout << "\t4 Latitude" << endl;
    cout << "\t5 Longitude" << endl;
    cout << ">>> ";
    int input;
    cin >> input;
    getchar();

    string value;
    double localizationValue;

    switch(input) {
    case 1:
      cout << "Type new place name: ";
      getline(cin, value);
      recordToChange->setPlaceName(value);
      break;
    case 2:
      cout << "Type new state: ";
      getline(cin, value);
      recordToChange->setState(value);
      break;
    case 3:
      cout << "Type new county: ";
      getline(cin, value);
      recordToChange->setCounty(value);
      break;
    case 4:
      cout << "Type new Latitude: ";
      cin >> localizationValue;
      getchar();
      recordToChange->setLatitude(localizationValue);
      break;
    case 5:
      cout << "Type new Longitude: ";
      cin >> localizationValue;
      getchar();
      recordToChange->setLongitude(localizationValue);
      break;
    }

  } else {
    shouldShowFeedBackMessage = true;
    feedBackMessage = "There are no records inside the set.";
  }
}

void displayRecord() {
  if(sequenceSet->getTotalRecordsInsideSequenceSet() > 0) {
    cout << "Type record zip code: ";
    long zipCode;
    cin >> zipCode;
    getchar();
    Record* record = sequenceSet->queryRecordByZipCode(zipCode);
    while(record == NULL) {
      cout << "Was not found any record with given zip code. Try again: ";
      long zipCode;
      cin >> zipCode;
      getchar();
      record = sequenceSet->queryRecordByZipCode(zipCode);
    }
    cout << "Record resume: \n";
    cout << "\tZip code: " << record->getZipCode() << endl;
    cout << "\tPlace name: " << record->getPlaceName() << endl;
    cout << "\tState: " << record->getState() << endl;
    cout << "\tCounty: " << record->getCounty() << endl;
    cout << "\tLatitude: " << record->getLatitude() << endl;
    cout << "\tLongitude: " << record->getLongitude() << endl;
    cout << "Press any key to get back";
    string input;
    getline(cin, input);
  } else {
    shouldShowFeedBackMessage = true;
    feedBackMessage = "There are no records inside the set.";
  }
}

void displaySpecificField() {
  if(sequenceSet->getTotalRecordsInsideSequenceSet() > 0) {
    cout << "Type record zip code: ";
    long zipCode;
    cin >> zipCode;
    getchar();
    Record* record = sequenceSet->queryRecordByZipCode(zipCode);
    while(record == NULL) {
      cout << "Was not found any record with given zip code. Try again: ";
      long zipCode;
      cin >> zipCode;
      getchar();
      record = sequenceSet->queryRecordByZipCode(zipCode);
    }
    cout << "Type which field you wanna see: \n";
    cout << "\t1. Place name\n";
    cout << "\t2. State\n";
    cout << "\t3. County\n";
    cout << "\t4. Latitude\n";
    cout << "\t5. Longitude\n";
    string fieldName;
    int field;
    bool isTryingToGetInput = true;
    while(isTryingToGetInput) {
      try {
        getline(cin, fieldName);
        field = stringToLong(fieldName);
        isTryingToGetInput = false;
      } catch(...) {
        cout << "Type online numbers: ";
      }
    }

    switch(field) {
    case 1:
      cout << "Place name: " << record->getPlaceName() << endl;
      break;
    case 2:
      cout << "State: " << record->getState() << endl;
      break;
    case 3:
      cout << "County: " << record->getCounty() << endl;
      break;
    case 4:
      cout << "Latitude: " << record->getLatitude() << endl;
      break;
    case 5:
      cout << "Longitude: " << record->getLongitude() << endl;
      break;
    }
    cout << "Press any key to get back";
    string input;
    getline(cin, input);
  } else {
    shouldShowFeedBackMessage = true;
    feedBackMessage = "There are no records inside the set.";
  }
}

void showHeader() {
  string* fieldsNames = sequenceSet->getFieldsNames();
  cout << "-----------------------" << endl;
  cout << "| Sequence set Header|" << endl;
  cout << "-----------------------" << endl;
  cout << "-> Fields per record: " << 6 << endl;
  cout << "-> Attributes names: " << endl;
  cout << "\t* "<< fieldsNames[0] << endl;
  cout << "\t* "<< fieldsNames[1] << endl;
  cout << "\t* "<< fieldsNames[2] << endl;
  cout << "\t* "<< fieldsNames[3] << endl;
  cout << "\t* "<< fieldsNames[4] << endl;
  cout << "\t* "<< fieldsNames[5] << endl;
  cout << "-> Total records inside: " << sequenceSet->getTotalRecordsInsideSequenceSet() << endl;
  cout << "-> Total records per node: " << sequenceSet->getRecordsQuantityPerBlock() << endl;
  cout << endl;
}

void createDataSetBackup(SequenceSet* sequenceSet) {
  updateDataSet(sequenceSet, backupDataSetPath);
}

void showMenu() {
  cout << "1. Insert a record\n";
  cout << "2. Delete a record\n";
  cout << "3. Modify a field in a record\n";
  cout << "4. Display a specific field in a record\n";
  cout << "5. Display a record\n";
  cout << "6. Check record's file consistency\n";
  cout << "7. Close app\n";
}
