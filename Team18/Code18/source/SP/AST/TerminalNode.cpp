#include "TerminalNode.h"
#include "../DesignExtractor/Extractor.h"

void VariableNode::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ConstantNode::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}