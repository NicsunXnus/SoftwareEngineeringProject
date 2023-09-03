#pragma once
#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <vector>
#include <iostream>
#include "QueryObject.h"
#include "../TokenClasses/Token.h"


/**
* This class is responsible for the creation of QueryObjects. A concrete class for each Query
*/
class QueryParser  {
public:
	QueryParser();
	
	virtual std::tuple<std::vector<std::shared_ptr<Token>>, std::vector<std::shared_ptr<Token>>> splitTokens(std::vector<std::shared_ptr<Token>> tokens);
	/*
	* This function generates the QueryObjects based on the tokens
	*/
	//virtual std::vector<QueryObject> generateQueryObjects(std::vector<Token> tokens);

};

#endif