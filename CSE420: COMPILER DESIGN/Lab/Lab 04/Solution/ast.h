#ifndef AST_H
#define AST_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

using namespace std;

class ASTNode
{
public:
    virtual ~ASTNode() {}
    virtual string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp, int &temp_count, int &label_count) const = 0;
};

// Expression node types

class ExprNode : public ASTNode
{
protected:
    string node_type; // Type information (int, float, void, etc.)
public:
    ExprNode(string type) : node_type(type) {}
    virtual string get_type() const { return node_type; }
};

// Variable node (for ID references)

class VarNode : public ExprNode
{
private:
    string name;
    ExprNode *index;            // For array access, nullptr for simple variables
    mutable string cached_temp; // Cache the first temp we create for this variable

public:
    VarNode(string name, string type, ExprNode *idx = nullptr)
        : ExprNode(type), name(name), index(idx), cached_temp("") {}

    ~VarNode()
    {
        if (index)
            delete index;
    }

    bool has_index() const { return index != nullptr; }

    string generate_index_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                               int &temp_count, int &label_count) const
    {
        if (!index)
            return "";

        // Generate code for the index expression
        string idx_temp = index->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        return idx_temp;
    }
    // Inside VarNode::generate_code
    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        if (has_index())
        {
            string idx_temp = index->generate_code(outcode, symbol_to_temp, temp_count, label_count);
            string result = "t" + to_string(temp_count++);
            outcode << result << " = " << name << "[" << idx_temp << "]" << endl;
            return result;
        }
        else
        {
            // Check if variable already has a temporary assigned
            if (symbol_to_temp.count(name) > 0)
            {
                // INCREMENT temp_count but don't output load instruction
                temp_count++;
                return symbol_to_temp[name]; // Reuse existing temporary
            }

            // Load variable into new temporary
            string temp = "t" + to_string(temp_count++);
            outcode << temp << " = " << name << endl;
            symbol_to_temp[name] = temp; // Store mapping
            return temp;
        }
    }
    string get_name() const { return name; }
};

// Constant node

class ConstNode : public ExprNode
{
private:
    string value;

public:
    ConstNode(string val, string type) : ExprNode(type), value(val) {}

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        // Create a new temporary variable
        string temp = "t" + to_string(temp_count++);
        // Write the assignment: t0 = 5
        outcode << temp << " = " << value << endl;
        return temp;
    }
};

// Binary operation node

class BinaryOpNode : public ExprNode
{
private:
    string op;
    ExprNode *left;
    ExprNode *right;

public:
    BinaryOpNode(string op, ExprNode *left, ExprNode *right, string result_type)
        : ExprNode(result_type), op(op), left(left), right(right) {}

    ~BinaryOpNode()
    {
        delete left;
        delete right;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        // Generate code for left and right operands
        string left_res = left->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        string right_res = right->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        // Create a new temporary for the result
        string result_temp = "t" + to_string(temp_count++);

        // Write the instruction
        outcode << result_temp << " = " << left_res << " " << op << " " << right_res << endl;

        return result_temp;
    }
};

// Unary operation node

class UnaryOpNode : public ExprNode
{
private:
    string op;
    ExprNode *expr;

public:
    UnaryOpNode(string op, ExprNode *expr, string result_type)
        : ExprNode(result_type), op(op), expr(expr) {}

    ~UnaryOpNode() { delete expr; }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        // Generate code for the expression
        string operand = expr->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        // Create a new temporary for the result
        string result_temp = "t" + to_string(temp_count++);

        // Write the instruction (e.g., t1 = -t0)
        outcode << result_temp << " = " << op << operand << endl;

        return result_temp;
    }
};

// Assignment node

class AssignNode : public ExprNode
{
private:
    VarNode *lhs;
    ExprNode *rhs;

public:
    AssignNode(VarNode *lhs, ExprNode *rhs, string result_type)
        : ExprNode(result_type), lhs(lhs), rhs(rhs) {}

    ~AssignNode()
    {
        delete lhs;
        delete rhs;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        string rhs_temp = rhs->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        string var_name = lhs->get_name();

        if (lhs->has_index())
        {
            string idx_temp = lhs->generate_index_code(outcode, symbol_to_temp, temp_count, label_count);
            outcode << var_name << "[" << idx_temp << "] = " << rhs_temp << endl;
        }
        else
        {
            outcode << var_name << " = " << rhs_temp << endl;

            // ✅ KEY: Only update mapping if variable was ALREADY loaded
            // If it was loaded before, the next use can reuse rhs_temp
            // If it wasn't loaded, next use should load it fresh
            if (symbol_to_temp.count(var_name) > 0)
            {
                symbol_to_temp[var_name] = rhs_temp;
            }
            // If not in map, don't add it - let next VarNode load it
        }
        return rhs_temp;
    }
};

// Statement node types

class StmtNode : public ASTNode
{
public:
    virtual string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                                 int &temp_count, int &label_count) const = 0;
};

// Expression statement node

class ExprStmtNode : public StmtNode
{
private:
    ExprNode *expr;

public:
    ExprStmtNode(ExprNode *e) : expr(e) {}
    ~ExprStmtNode()
    {
        if (expr)
            delete expr;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        if (expr)
        {
            expr->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }
        return "";
    }
};

// Block (compound statement) node

class BlockNode : public StmtNode
{
private:
    vector<StmtNode *> statements;

public:
    ~BlockNode()
    {
        for (auto stmt : statements)
        {
            delete stmt;
        }
    }

    void add_statement(StmtNode *stmt)
    {
        if (stmt)
            statements.push_back(stmt);
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        for (auto stmt : statements)
        {
            if (stmt)
            {
                stmt->generate_code(outcode, symbol_to_temp, temp_count, label_count);
            }
        }
        return "";
    }
};

// If statement node

class IfNode : public StmtNode
{
private:
    ExprNode *condition;
    StmtNode *then_block;
    StmtNode *else_block; // nullptr if no else part

public:
    IfNode(ExprNode *cond, StmtNode *then_stmt, StmtNode *else_stmt = nullptr)
        : condition(cond), then_block(then_stmt), else_block(else_stmt) {}

    ~IfNode()
    {
        delete condition;
        delete then_block;
        if (else_block)
            delete else_block;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        // Evaluate the condition
        string cond_temp = condition->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        // Create labels
        int label_true = label_count++;
        int label_false = label_count++;
        int label_end = label_count++;

        // If condition is true, jump to true branch
        outcode << "if " << cond_temp << " goto L" << label_true << endl;
        // Otherwise jump to false branch
        outcode << "goto L" << label_false << endl;

        // True branch
        outcode << "L" << label_true << ":" << endl;
        if (then_block)
        {
            then_block->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }
        // Jump to end
        outcode << "goto L" << label_end << endl;

        // False branch
        outcode << "L" << label_false << ":" << endl;
        if (else_block)
        {
            else_block->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }

        // End label
        outcode << "L" << label_end << ":" << endl;

        return "";
    }
};

// While statement node

class WhileNode : public StmtNode
{
private:
    ExprNode *condition;
    StmtNode *body;

public:
    WhileNode(ExprNode *cond, StmtNode *body_stmt)
        : condition(cond), body(body_stmt) {}

    ~WhileNode()
    {
        delete condition;
        delete body;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        // Create labels
        int label_start = label_count++;
        int label_body = label_count++;
        int label_end = label_count++;

        // Start label
        outcode << "L" << label_start << ":" << endl;

        // Evaluate condition
        string cond_temp = condition->generate_code(outcode, symbol_to_temp, temp_count, label_count);

        // If true, jump to body
        outcode << "if " << cond_temp << " goto L" << label_body << endl;
        // Otherwise jump to end
        outcode << "goto L" << label_end << endl;

        // Body
        outcode << "L" << label_body << ":" << endl;
        if (body)
        {
            body->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }

        // Jump back to start
        outcode << "goto L" << label_start << endl;

        // End label
        outcode << "L" << label_end << ":" << endl;

        return "";
    }
};

// For statement node

class ForNode : public StmtNode
{
private:
    ExprNode *init;
    ExprNode *condition;
    ExprNode *update;
    StmtNode *body;

public:
    ForNode(ExprNode *init_expr, ExprNode *cond_expr, ExprNode *update_expr, StmtNode *body_stmt)
        : init(init_expr), condition(cond_expr), update(update_expr), body(body_stmt) {}

    ~ForNode()
    {
        if (init)
            delete init;
        if (condition)
            delete condition;
        if (update)
            delete update;
        delete body;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        // Run initialization
        if (init)
        {
            init->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }

        int label_start = label_count++;
        int label_body = label_count++;
        int label_end = label_count++;

        // Start label
        outcode << "L" << label_start << ":" << endl;

        // Evaluate condition
        if (condition)
        {
            // ✅ The condition might be wrapped in ExprStmtNode
            // We need to generate the code and capture the last temp created
            int temp_before = temp_count;

            // This will generate the condition expression and return empty string if it's ExprStmtNode
            // But it will still create the temp variables
            string cond_temp = condition->generate_code(outcode, symbol_to_temp, temp_count, label_count);

            // ✅ If cond_temp is empty, the last temp created is our condition result
            if (cond_temp.empty() && temp_count > temp_before)
            {
                cond_temp = "t" + to_string(temp_count - 1);
            }

            outcode << "if " << cond_temp << " goto L" << label_body << endl;
            outcode << "goto L" << label_end << endl;
        }

        // Body label
        outcode << "L" << label_body << ":" << endl;
        if (body)
        {
            body->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }

        // Update
        if (update)
        {
            update->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }

        // Jump back
        outcode << "goto L" << label_start << endl;

        // End label
        outcode << "L" << label_end << ":" << endl;

        return "";
    }
};

// Return statement node

class ReturnNode : public StmtNode
{
private:
    ExprNode *expr;

public:
    ReturnNode(ExprNode *e) : expr(e) {}
    ~ReturnNode()
    {
        if (expr)
            delete expr;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        if (expr)
        {
            string ret_val = expr->generate_code(outcode, symbol_to_temp, temp_count, label_count);
            outcode << "return " << ret_val << endl;
        }
        else
        {
            outcode << "return" << endl;
        }
        return "";
    }
};

// Declaration node

class DeclNode : public StmtNode
{
private:
    string type;
    vector<pair<string, int>> vars; // Variable name and array size (0 for regular vars)

public:
    DeclNode(string t) : type(t) {}

    void add_var(string name, int array_size = 0)
    {
        vars.push_back(make_pair(name, array_size));
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        for (auto const &var : vars)
        {
            string var_name = var.first;

            // ✅ DON'T erase mapping - let the existing temp be reused
            // Remove this entire block:
            // if (symbol_to_temp.count(var_name) > 0)
            // {
            //     symbol_to_temp.erase(var_name);
            // }

            if (var.second > 0)
            {
                outcode << "// Declaration: " << type << " " << var_name
                        << "[" << var.second << "]" << endl;
            }
            else
            {
                outcode << "// Declaration: " << type << " " << var_name << endl;
            }
        }
        return "";
    }
};
// Function declaration node

class FuncDeclNode : public ASTNode
{
private:
    string return_type;
    string name;
    vector<pair<string, string>> params; // Parameter type and name
    BlockNode *body;

public:
    FuncDeclNode(string ret_type, string n) : return_type(ret_type), name(n), body(nullptr) {}
    ~FuncDeclNode()
    {
        if (body)
            delete body;
    }

    void add_param(string type, string name)
    {
        params.push_back(make_pair(type, name));
    }

    void set_body(BlockNode *b)
    {
        body = b;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        outcode << endl;
        symbol_to_temp.clear();

        outcode << "// Function: " << return_type << " " << name << "(";
        for (size_t i = 0; i < params.size(); ++i)
        {
            outcode << params[i].first << " " << params[i].second;
            if (i < params.size() - 1)
                outcode << ", ";
            // ✅ No mapping - let VarNode handle loading into temps
        }
        outcode << ")" << endl;

        if (body)
        {
            body->generate_code(outcode, symbol_to_temp, temp_count, label_count);
        }
        return "";
    }
};

// Helper class for function arguments

class ArgumentsNode : public ASTNode
{
private:
    vector<ExprNode *> args;

public:
    ~ArgumentsNode()
    {
        // Don't delete args here - they'll be transferred to FuncCallNode
    }

    void add_argument(ExprNode *arg)
    {
        if (arg)
            args.push_back(arg);
    }

    ExprNode *get_argument(int index) const
    {
        if (index >= 0 && index < args.size())
        {
            return args[index];
        }
        return nullptr;
    }

    size_t size() const
    {
        return args.size();
    }

    const vector<ExprNode *> &get_arguments() const
    {
        return args;
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        return "";
    }
};

// Function call node

class FuncCallNode : public ExprNode
{
private:
    string func_name;
    vector<ExprNode *> arguments;

public:
    FuncCallNode(string name, string result_type)
        : ExprNode(result_type), func_name(name) {}

    ~FuncCallNode()
    {
        for (auto arg : arguments)
        {
            delete arg;
        }
    }

    void add_argument(ExprNode *arg)
    {
        if (arg)
            arguments.push_back(arg);
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        // Generate code for each argument and output param instruction
        for (auto arg : arguments)
        {
            if (arg)
            {
                string arg_temp = arg->generate_code(outcode, symbol_to_temp, temp_count, label_count);
                outcode << "param " << arg_temp << endl;
            }
        }

        // Create temporary for return value
        string result_temp = "t" + to_string(temp_count++);

        // Output call instruction
        outcode << result_temp << " = call " << func_name << ", " << arguments.size() << endl;

        return result_temp;
    }
};

// Program node (root of AST)

class ProgramNode : public ASTNode
{
private:
    vector<ASTNode *> units;

public:
    ~ProgramNode()
    {
        for (auto unit : units)
        {
            delete unit;
        }
    }

    void add_unit(ASTNode *unit)
    {
        if (unit)
            units.push_back(unit);
    }

    string generate_code(ofstream &outcode, map<string, string> &symbol_to_temp,
                         int &temp_count, int &label_count) const override
    {
        for (auto unit : units)
        {
            if (unit)
            {
                unit->generate_code(outcode, symbol_to_temp, temp_count, label_count);
            }
        }
        return "";
    }
};

#endif // AST_H