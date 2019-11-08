#ifndef RECORD_H
#define RECORD_H

#include <string>
#include <iostream>
#include <cstdlib>

class Record {
private:
    long zipCode;
    std::string state;
    std::string county;
    std::string placeName;
    double latitude;
    double longitude;

public:
    /**
     * Constructor with params for an instance of Record class.
     * 
     * @param zipCode
     * @param state
     * @param county
     * @param placeName
     * @param latitude
     * @param longitude
     */
	Record(long zipCode, std::string state, std::string county, 
		std::string placeName, double latitude, double longitude);

    /**
     * Default constructor. It just returns an instance.
     * 
     */
    Record();

    /**
     * Default descrutor. 
     * 
     */
    ~Record();

    // *** Just getters and setters *** //
    long getZipCode();

    void setZipCode(long zipCode);

    std::string getState();

    void setState(std::string state);

    std::string getCounty();

    void setCounty(std::string county);

    std::string getPlaceName();

    void setPlaceName(std::string placeName);

    double getLatitude();

    void setLatitude(double latitude);

    double getLongitude();

    void setLongitude(double longitude);

    /**
     * It returns instance state in a string
     *
     * @return a string with attribute values.
     */
    std::string toString();
};

#endif

