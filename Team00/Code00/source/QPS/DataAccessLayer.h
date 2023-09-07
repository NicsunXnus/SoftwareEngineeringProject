#ifndef DATAACCESSLAYER_H
#define DATAACCESSLAYER_H

#include <vector>
#include <string>
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

	 vector<int> getEntities(EntitiesAPI type);




};
#endif
