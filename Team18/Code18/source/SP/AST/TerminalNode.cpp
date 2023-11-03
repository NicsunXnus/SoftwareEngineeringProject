#include "TerminalNode.h"
#include "../DesignExtractor/Extractor.h"

// TO ADDRESS: Should we have this method be part of the TerminalNode class?

void VariableNode::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ConstantNode::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}