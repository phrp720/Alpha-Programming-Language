#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "OffsetCounter.h"
#include <string>
#include <vector>

enum Symbol_Type {
	library_function, user_function, global_variable, formal_argument, local_variable, member_variable
};

class Node {
    public:
        Node(std::string id, size_t lineNum, Symbol_Type type, size_t scope, size_t space, size_t offset):
        id(id),line_num(lineNum),type(type),scope(scope),active(true),offset(offset),space_t(space){};
        Node(Node &node);
        Node * clone();
        friend class Symbol_table;
        std::string id;
        int line_num;
        Symbol_Type type;
        size_t scope;
        size_t offset;
        bool active;
        size_t functionAddress;
        size_t locals;
        size_t space_t;
};

class Symbol_table{
    public:
        Symbol_table();
        Node* lookup(std::string id, size_t scope);
        Node* insert(std::string id, size_t lineNum, Symbol_Type type, size_t scope, size_t space, size_t offset);
        bool remove(std::string id, size_t scope);
        bool update(std::string id, size_t lineNum, Symbol_Type type, size_t scope);
        void print();
        bool hide(size_t scope);
        bool activate(size_t scope);
        size_t maxScope();
        size_t global=0;
    private:
        size_t hash(std::string id);
        Node* findInList(Node* q, std::string id, size_t scope);
        std::vector<Node*> hash_table;
};

#endif
