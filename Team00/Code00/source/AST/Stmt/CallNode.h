#pragma once
#include "StmtNode.h"
#include "ProcedureNode.h"

class CallNode : public StmtNode {
public:
    CallNode(std::shared_ptr<ProcedureNode> procedure);
    std::shared_ptr<ProcedureNode> procedure;
};