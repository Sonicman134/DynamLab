#pragma once

#include <vector>
#include <string>
#include "Scanner.h"
#include "SematicTree.h"


class Analisys
{
	Scanner scaner;
	int currentType;
	int savePoint;
	int saveLine;
	std::string currentLex;
	SematicTree* root = new SematicTree(NULL, NULL, NULL, new Node);

	void AnaliseSosOp();
	void AnaliseData();
	void AnaliseExpression();
	void AnaliseAnd();
	void AnaliseCompare();
	void AnaliseAdd();
	void AnaliseMultiply();
	void AnaliseNe();
	void AnaliseBasicExpress();
	void AnaliseAskFunc();
	void AnaliseSwitch();
	void AnaliseOp();
	void AnaliseFunc();
	void AnaliseBodyCase();
	void ShowError(std::string err);
public: 
	Analisys(std::string fileName);
	void Analise();
};

