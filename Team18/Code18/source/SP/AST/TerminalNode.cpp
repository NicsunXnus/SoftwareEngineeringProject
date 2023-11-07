#include "TerminalNode.h"
#include "../DesignExtractor/Extractor.h"

void VariableNode::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ConstantNode::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}