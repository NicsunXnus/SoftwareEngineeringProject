#pragma once
#ifndef DESIGNOBJECT_H
#define DESIGNOBJECT_H

#include <string_view>
#include "QueryObject.h"



using namespace std;
/*
* This class represents a Query object, for design entities
*/
class DesignObject : public QueryObject {
private:
	vector<string> res;
public:
	DesignObject(vector<string_view> data)
		: QueryObject{ data } {

	}
	void setResult(variant<vector<string>, map<string, vector<string>>> result) override {
		res = get<vector<string>>(result);
	}

	variant<vector<string>, map<string, vector<string>>> getResult() override {
		return res;
	}
};

/*
* This class represents a Query object, for design entity stmt
*/
class StmtObject : public DesignObject {
public:
	StmtObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(STMT));
	}

};

/*
* This class represents a Query object, for design entity read
*/
class ReadObject : public DesignObject {
public:
	ReadObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(READ));
	}
};

/*
* This class represents a Query object, for design entity print
*/
class PrintObject : public DesignObject {
public:
	PrintObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(PRINT));
	}
};

/*
* This class represents a Query object, for design entity call
*/
class CallObject : public DesignObject {
public:
	CallObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(CALL));
	}
};

/*
* This class represents a Query object, for design entity while
*/
class WhileObject : public DesignObject {
public:
	WhileObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(WHILE));
	}
};

/*
* This class represents a Query object, for design entity if
*/
class IfObject : public DesignObject {
public:
	IfObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(IF));
	}
};

/*
* This class represents a Query object, for design entity assign
*/
class AssignObject : public DesignObject {
public:
	AssignObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(ASSIGN));
	}
};

/*
* This class represents a Query object, for design entity variable
*/
class VariableObject : public DesignObject {
public:
	VariableObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {

		std::cout << "Calling data access layer from variable object\n";

		setResult(dataAccessLayer->getEntity(VARIABLE));
	}
};

/*
* This class represents a Query object, for design entity constant
*/
class ConstantObject : public DesignObject {
public:
	ConstantObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(CONSTANT));
	}
};

/*
* This class represents a Query object, for design entity procedure
*/
class ProcedureObject : public DesignObject {
public:
	ProcedureObject(vector<string_view> data)
		: DesignObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getEntity(PROCEDURE));
	}
};

#endif
