#include "SematicTree.h"

SematicTree* SematicTree::Cur = nullptr;

SematicTree::SematicTree(SematicTree* l, SematicTree* r, SematicTree* u, Node* d)
{
	n = new Node;
	Up = u; Left = l; Right = r;
	memcpy(n, d, sizeof(Node));
}

void SematicTree::SetLeft(Node* Data) 
{
	SematicTree* a = new SematicTree(NULL, NULL, this, Data);
	Left = a; 
}

void SematicTree::SetRight(Node* Data)
{
	SematicTree* a = new SematicTree(NULL, NULL, this, Data);
	Right = a;
}

SematicTree* SematicTree::FindUp(SematicTree* From, std::string id)
{
	SematicTree* i = From;
	while ((i != NULL) && (id != i->n->id))
		i = i->Up;
	return i;
}

SematicTree* SematicTree::Add(std::string* lex, int objType) {
	Node newData;
	newData.typeObject = objType;
	newData.id = *lex;
	newData.param = 0;
	newData.typeVar = -1;
	Cur->SetLeft(&newData);
	Cur = Cur->Left;
	return Cur;
}

SematicTree* SematicTree::NewBlock() {
	SematicTree* sav;
	Node dat;
	dat.id = "";
	dat.typeObject = NULL;
	if (Cur->Right != NULL) {
		Cur->SetLeft(&dat);
		Cur = Cur->Left;
	}
	Cur->SetRight(&dat);
	sav = Cur;
	Cur = Cur->Right;
	return sav;
}

void SematicTree::PrintError(std::string error, std::string lex) {
	std::cout << error + " " + lex << std::endl;
	exit(1);
}

void SematicTree::SetTypeVar(int type) {
	n->typeVar = type;
}

void SematicTree::AddParam() {
	n->param++;
}

void SematicTree::SetCurrent(SematicTree* tree) {
	Cur = tree;
}

void SematicTree::Print() {
	std::cout << "ID -> " + n->id << std::endl;
	if(Up != NULL) std::cout << "Up -> " + Up->n->id << std::endl;
	if (Left != NULL) std::cout << "Left -> " + Left->n->id << std::endl;
	if (Right != NULL) std::cout << "Right -> " + Right->n->id << std::endl;
	std::cout << std::endl;
	if (Right != NULL) Right->Print();
	if (Left != NULL) Left->Print();
}

void SematicTree::CheckIfFuncExist(SematicTree* Start, int returnType, std::string lex, int enterParam, bool typeFind) {
	bool res = false;
	SematicTree* tree;
	tree = FindUp(Start, lex);
	while (!res &&  tree != NULL) {
		if (tree->n->typeObject == TFunc && (tree->n->typeVar == returnType || returnType == -1) && tree->n->param == enterParam)
			res = true;
		tree = FindUp(tree->Up, lex);
	}
	if (res != typeFind) {
		if (typeFind) PrintError("Func dont exist", lex);
		else PrintError("Func already exist", lex);
	}
}

void SematicTree::CheckIfVarExist(SematicTree* Start, std::string lex, bool typeFind) {
	bool res = false;
	SematicTree* tree;
	tree = FindUp(Start, lex);
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TVar)
			res = true;
		tree = FindUp(tree->Up, lex);
	}
	if (res != typeFind) {
		if (typeFind) PrintError("Var dont exist", lex);
		else PrintError("Var already exist", lex);
	}
}