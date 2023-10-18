#include "AbstractionStarExtractor.h"


void AbstractionStarExtractor::extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) {
    this->extract(processedProgram);
    processAbstractionStar();
}

