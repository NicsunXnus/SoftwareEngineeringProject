#include "ComparisonQueryObject.h"

shared_ptr<QueryResultsTable> StaticStaticComparisonQueryObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	// e.g. if "3" == "3"
	shared_ptr<QueryResultsTable> table = 
		QueryResultsTable::createEmptyTable(ref1->getArgValue() == ref2->getArgValue());
	return table;
}

shared_ptr<QueryResultsTable> StaticAttrRefComparisonQueryObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	// call 2D table for attrRef, e.g. p.procName, p, similar to withclause Table
	shared_ptr<QueryResultsTable> attRefTable = attrRef->callAndProcess(dataAccessLayer);
	// filter out rows with attRef == static e.g. p.procName == 'Third'
	vector<string> targets;
	targets.push_back(svToString(ref->getArgValue()));
	shared_ptr<QueryResultsTable> filteredTable = attRefTable->filter(attRefTable->getPrimaryKey(), targets);
	return filteredTable;
}

shared_ptr<QueryResultsTable> AttrRefAttrRefComparisonQueryObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	// call 2D table for attrRef1, e.g. p, p.procName similar to withclause Table
	shared_ptr<QueryResultsTable> attRef1Table = attrRef1->callAndProcess(dataAccessLayer);
	// call 2D table for attrRef2, e.g. q, q.procName similar to withclause Table
	shared_ptr<QueryResultsTable> attRef2Table = attrRef2->callAndProcess(dataAccessLayer);

	if (attRef1Table->getPrimaryKey() == attRef2Table->getPrimaryKey()) {
		return attRef1Table; // same table, return 1 or 2 doesnt matter
	}

	shared_ptr<QueryResultsTable> crossProductTables = attRef1Table->crossProductSet(attRef2Table);

	shared_ptr<QueryResultsTable> filteredTable = crossProductTables->innerJoinOnTwoColumns(attRef1Table->getPrimaryKey(), attRef2Table->getPrimaryKey());
	
	return filteredTable;
}