#include "QueryObject.h"

string_view QueryObject::getQueryObjectName() {
	string_view sv(data.data(), data.size());
	return sv;
}

string QueryObject::getCacheName() {
	return "";
}

bool QueryObject::shouldCache() {
	return false;
}

int QueryObject::getSynCount() {
	shared_ptr<unordered_set<string>> synonyms = getSynonyms();
	if (synonyms->empty()) {
		return 0;
	}
	return static_cast<int>(synonyms->size());
}