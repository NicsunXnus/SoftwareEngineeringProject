#include "QueryObject.h"

string_view QueryObject::getQueryObjectName() {
	return data;
}

string QueryObject::getCacheName() {
	return "";
}

bool QueryObject::shouldCache() {
	return false;
}