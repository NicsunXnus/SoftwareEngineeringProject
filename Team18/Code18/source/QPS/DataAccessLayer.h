#ifndef DATAACCESSLAYER_H
#define DATAACCESSLAYER_H

#include <vector>
#include <string>
#include "../PKB.h"
#include "../Constants/QPSPKB.h"

using namespace std;


/**
* This class represents an interface object that calls APIs from the PKB
*/
class DataAccessLayer {
private:
public:
	/**
	 * Constructor for the DataAccessLayer object
	 */
	DataAccessLayer() {};

	/**
	 * Destructor for the DataAccessLayer object
	 */
	~DataAccessLayer() {};

	 vector<string> getEntity(ENTITY type);


	 // TODO: UPDATE AFTER ENUMS PR IS MERGED, update all calls in clauseobject.h
	 map<string, vector<string>> getClause();


};
#endif
