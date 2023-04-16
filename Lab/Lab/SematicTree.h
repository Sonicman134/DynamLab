#pragma once
#include <string>
#include <iostream>

enum TypeObj {
	TVar = 1,
	TFunc
};

enum TypeVar {
	TBool = 11,
	TShort,
	TLong,
	TInt,
	TypeDef = 100
};

struct Node {
	std::string id;
	int typeObject = -1;
	int typeVar = -1;
	int param = 0;
};

class SematicTree
{
public:
	Node* n;
	SematicTree* Left, *Right, *Up;
	static SematicTree* Cur;
	SematicTree(SematicTree* l, SematicTree* r, SematicTree* u, Node* Data);
	void SetLeft(Node* Data);
	void SetRight(Node* Data);
	SematicTree* FindUp(SematicTree* From, std::string id);
	SematicTree* Add(std::string* lex, int objType);
	SematicTree* NewBlock();
	void PrintError(std::string error, std::string lex);
	void SetTypeVar(int type);
	void AddParam();
	static void SetCurrent(SematicTree* tree);
	std::string GetId() { return n->id; };
	int GetTypeObject() { return n->typeObject; };
	int GetTypeVar() { return n->typeVar; };
	int GetParam() { return n->param; };
	void Print();

	void CheckIfFuncExist(SematicTree* Start, int returnType, std::string lex, int enterParam, bool typeFind);
	void CheckIfVarExist(SematicTree* Start, std::string lex, bool typeFind);

};

