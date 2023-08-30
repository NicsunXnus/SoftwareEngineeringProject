#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(const std::string& name, std::vector<std::shared_ptr<StmtNode>> stmts)
    : name(name), stmts(std::move(stmts)) {}
