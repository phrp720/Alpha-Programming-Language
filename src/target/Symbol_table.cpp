#include<iostream>
#include <algorithm>
#include "Symbol_table.h"

using namespace std;

Symbol_table::Symbol_table() { }

Node* Symbol_table::lookup(std::string id, size_t scope)
{
    for (size_t i = 0; i < hash_table.size(); i++){
        Node* q = hash_table[i];
        if (q->id == id && q->scope == scope&&q->active==true){
            return q;
        }
    }

    return NULL;
}

Node::Node(Node &node) {
    type = node.type;
    active = node.type;
    line_num = node.line_num;
    functionAddress = node.functionAddress;
    offset = node.offset;
    scope = node.scope;
    id = node.id;
    space_t = node.space_t;
    locals = node.locals;
}


Node * Node::clone() {
    return new Node(*this);
}

Node* Symbol_table::insert(string id, size_t lineNum, Symbol_Type type, size_t scope, size_t space, size_t offset)
{
    Node* node = new Node(id, lineNum, type, scope, space, offset);
    if (space == programVar) {
        global++;
    }
    if (lookup(id, scope) == NULL){
        hash_table.push_back(node);
        return node;
    } else {
        return NULL;
    }
}

bool Symbol_table::hide(size_t scope)
{
    for (int i = 0; i < hash_table.size(); i++) {
        if(hash_table[i]->scope==scope) {
            hash_table[i]->active = false;
        }
    }
    return true;
}

bool Symbol_table::activate(size_t scope)
{
    for (int i = 0; i < hash_table.size(); i++) {
        hash_table[i]->active = true;
    }
    return true;
}

bool Symbol_table::remove(string id, size_t scope)
{
    for (size_t i = 0; i < hash_table.size(); i++){
        Node* q = hash_table[i];
        if (q->id == id && q->scope == scope){
            hash_table.erase(hash_table.begin() + i);
            return true;
        }
    }

    return false;
}

bool Symbol_table::update(string id, size_t lineNum, Symbol_Type type, size_t scope)
{
    Node* node;

    if ((node = lookup(id, scope)) != NULL){
        node->line_num = lineNum;
        node->type = type;
        node->scope = scope;
        return true;
    } else{
        return false;
    }
}

size_t Symbol_table::maxScope()
{
    size_t max = 0;
    for (int i = 0; i < hash_table.size(); i++){
        if (hash_table[i] != NULL){
            if (hash_table[i]->scope > max){
                max = hash_table[i]->scope;
            }
        }
    }

    return max;
}

bool compLines(Node* first, Node* second)
{
    return (&first->line_num < &second->line_num);
}

void Symbol_table::print()
{
    for (size_t n = 0; n <= maxScope(); n++){
        cout << "----------------    Scope #" << n << "    ---------------- " << endl;
        std::sort(hash_table.begin(), hash_table.end(), compLines);

        for (int i = 0; i < hash_table.size(); i++){
            Node* p = hash_table[i];

            if (p->scope == n) {
                std::string type;

                if (p->type == Symbol_Type::library_function) {
                    type = "library function";
                }

                if (p->type == Symbol_Type::user_function) {
                    type = "user function";
                }

                if (p->type == Symbol_Type::global_variable) {
                    type = "global variable";
                }

                if (p->type == Symbol_Type::formal_argument) {
                    type = "formal argument";
                }

                if (p->type == Symbol_Type::local_variable) {
                    type = "local argument";
                }

                if (p->type == Symbol_Type::member_variable) {
                    type = "member variable";
                }

                cout << "\"" << p->id << "\" [" << type << "] (line " << p->line_num << ") (scope " << p->scope << ")" << endl;
            }
        }
    }
}
