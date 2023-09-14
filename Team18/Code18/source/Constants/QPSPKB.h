#ifndef QPSPKB_H
#define QPSPKB_H

enum ENTITY {
    STMT,
    READ,
    PRINT,
    CALL,
    WHILE,
    IF,
    ASSIGN,
    VARIABLE,
    CONSTANT,
    PROCEDURE
};


const map<string, ENTITY> entityMap = {
    {"stmt", STMT},
    {"read", READ},
    {"print", PRINT},
    {"call", CALL},
    {"while", WHILE},
    {"if", IF},
    {"assign", ASSIGN},
    {"variable", VARIABLE},
    {"constant", CONSTANT},
    {"procedure", PROCEDURE},
};

static ENTITY EntityEnumToString(string entity) {
    if (entityMap.count(entity)) {
        return entityMap.at(entity);
    }
    throw runtime_error("entity is invalid!");
}

enum ABSTRACTION {
    USES,
    MODIFIES,
    PARENT,
    PARENTSTAR,
    FOLLOWS,
    FOLLOWSSTAR
};

const map<string, ABSTRACTION> abstractionMap = {
    {"uses", USES},
    {"modifies", MODIFIES},
    {"parent", PARENT},
    {"parentstar", PARENTSTAR},
    {"follows", FOLLOWS},
    {"followsstar", FOLLOWSSTAR},
};

static ABSTRACTION AbstractionEnumToString(string abstraction) {
    if (abstractionMap.count(abstraction)) {
        return abstractionMap.at(abstraction);
    }
    throw runtime_error("abstraction is invalid!");
}

#endif