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
    {"procedure", PROCEDURE}
};

static ENTITY EntityEnumToString(string entity) {
    //std::cout << "converting enum";
    if (entityMap.count(entity)) {
        return entityMap.at(entity);
    }
    throw runtime_error("entity is invalid!");
}

#endif