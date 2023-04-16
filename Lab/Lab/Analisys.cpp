#include "Analisys.h"

void Analisys::ShowError(std::string err) {
    std::cout << err + "\nAt line" + std::to_string(scaner.GetLine()) << std::endl;
    std::cout << std::endl;
    //root->Print();
    std::exit(0);
}

//Программа
void Analisys::Analise() {
    savePoint = scaner.GetPoint();
    saveLine = scaner.GetLine();
    currentType = scaner.Scan(&currentLex);
    while (currentType != End) {
        if (currentType == keyBool || currentType == keyShort || currentType == keyLong || currentType == keyInt) {
            currentType = scaner.Scan(&currentLex);
            if (currentType == Id) {
                currentType = scaner.Scan(&currentLex);
                if (currentType == LBracket) {
                    scaner.SetPoint(savePoint);
                    scaner.SetLine(saveLine);
                    AnaliseFunc();
                }
                else {
                    scaner.SetPoint(savePoint);
                    scaner.SetLine(saveLine);
                    AnaliseData();
                }
            }
            else if (currentType == keyMain) {
                scaner.SetPoint(savePoint);
                scaner.SetLine(saveLine);
                AnaliseFunc();
            }
            else ShowError("Not id");
        }
        else ShowError("Not type");
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
    }
    std::cout << "No errors" << std::endl;
    std::cout << std::endl;
    //root->Print();
}

//Функция
void Analisys::AnaliseFunc() {
    SematicTree* SavedPos = root;
    SematicTree* ThisFunc = root;
    int type;
    currentType = scaner.Scan(&currentLex);
    if (currentType != keyBool && currentType != keyShort && currentType != keyLong && currentType != keyInt) ShowError("Not type");
    type = currentType;
    currentType = scaner.Scan(&currentLex);
    if (currentType == Id) {
        ThisFunc = root->Add(&currentLex, TFunc);
        SavedPos = root->NewBlock();
        ThisFunc->SetTypeVar(type);
        currentType = scaner.Scan(&currentLex);
        if (currentType != LBracket) ShowError("Not (");
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
        if (currentType == keyBool || currentType == keyShort || currentType == keyLong || currentType == keyInt) {
            scaner.SetPoint(savePoint);
            scaner.SetLine(saveLine);
            do {
                currentType = scaner.Scan(&currentLex);
                if (currentType != keyBool && currentType != keyShort && currentType != keyLong && currentType != keyInt) ShowError("Not type");
                type = currentType;
                currentType = scaner.Scan(&currentLex);
                if (currentType != Id) ShowError("Not id");
                root->CheckIfVarExist(root->Cur, currentLex, false);
                root->Add(&currentLex, TVar)->SetTypeVar(type);
                ThisFunc->AddParam();
                currentType = scaner.Scan(&currentLex);
            } while (currentType == Comma);
        }
        if (currentType != RBracket) ShowError("Not )");
    }
    else if (currentType == keyMain) {
        ThisFunc = root->Add(&currentLex, TFunc);
        SavedPos = root->NewBlock();
        ThisFunc->SetTypeVar(type);
        currentType = scaner.Scan(&currentLex);
        if (currentType != LBracket) ShowError("Not (");
        currentType = scaner.Scan(&currentLex);
        if (currentType != RBracket) ShowError("Not )");
    }
    else ShowError("Not function");
    root->CheckIfFuncExist(ThisFunc->Up, ThisFunc->GetTypeVar(), ThisFunc->GetId(), ThisFunc->GetParam(), false);
    AnaliseSosOp();
    root->SetCurrent(SavedPos);
}

//Составной оператор
void Analisys::AnaliseSosOp() {
    currentType = scaner.Scan(&currentLex);
    if (currentType != LBrace) ShowError("Not {");
    savePoint = scaner.GetPoint();
    saveLine = scaner.GetLine();
    currentType = scaner.Scan(&currentLex);
    while (currentType != RBrace) {
        if (currentType == keyBool || currentType == keyShort || currentType == keyLong || currentType == keyInt) {
            scaner.SetPoint(savePoint);
            scaner.SetLine(saveLine);
            AnaliseData();
        }
        else {
            scaner.SetPoint(savePoint);
            scaner.SetLine(saveLine);
            AnaliseOp();
        }
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
    }
}

//Оператор или данные
void Analisys::AnaliseOp() {
    std::string SavedLex;
    savePoint = scaner.GetPoint();
    saveLine = scaner.GetLine();
    currentType = scaner.Scan(&currentLex);
    if (currentType == Id) {
        SavedLex = currentLex;
        currentType = scaner.Scan(&currentLex);
        if (currentType == Save) {
            root->CheckIfVarExist(root->Cur, SavedLex, true);
            AnaliseExpression();
            currentType = scaner.Scan(&currentLex);
            if (currentType != Semicolon) ShowError("Not ;");
        }
        else if (currentType == LBracket) {
            scaner.SetPoint(savePoint);
            scaner.SetLine(saveLine);
            AnaliseAskFunc();
            currentType = scaner.Scan(&currentLex);
            if (currentType != Semicolon) ShowError("Not ;");
        }
        else {
            ShowError("Not = or function");
        }
    }
    else if (currentType == LBrace) {
        SematicTree* SavedPos = root->NewBlock();
        scaner.SetPoint(savePoint);
        scaner.SetLine(saveLine);
        AnaliseSosOp();
        root->SetCurrent(SavedPos);
    }
    else if (currentType == keySwitch) {
        scaner.SetPoint(savePoint);
        scaner.SetLine(saveLine);
        AnaliseSwitch();
    }
    else if (currentType == keyReturn) {
        AnaliseExpression();
        currentType = scaner.Scan(&currentLex);
        if (currentType != Semicolon) ShowError("Not ;");
    }
    else if (currentType != Semicolon) ShowError("Not operator");
}

//Данные
void Analisys::AnaliseData() {
    currentType = scaner.Scan(&currentLex);
    if (currentType != keyBool && currentType != keyShort && currentType != keyLong && currentType != keyInt) ShowError("Not type");
    int type;
    type = currentType;
    do {
        currentType = scaner.Scan(&currentLex);
        if (currentType != Id) ShowError("Not id");
        root->CheckIfVarExist(root->Cur, currentLex, false);
        root->Add(&currentLex, TVar)->SetTypeVar(type);
        currentType = scaner.Scan(&currentLex);
        if (currentType == Save) {
            AnaliseExpression();
            currentType = scaner.Scan(&currentLex);
        }
    } while (currentType == Comma);
    if (currentType != Semicolon) ShowError("Not ;");
}

//Выражение
void Analisys::AnaliseExpression() {
    do {
        AnaliseAnd();
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
    } while (currentType == Or);
    scaner.SetPoint(savePoint);
    scaner.SetLine(saveLine);
}

//Логическое и
void Analisys::AnaliseAnd() {
    do {
        AnaliseCompare();
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
    } while (currentType == And);
    scaner.SetPoint(savePoint);
    scaner.SetLine(saveLine);
}

//Сравнение
void Analisys::AnaliseCompare() {
    savePoint = scaner.GetPoint();
    saveLine = scaner.GetLine();
    currentType = scaner.Scan(&currentLex);
    if (currentType != Plus && currentType != Minus) {
        scaner.SetPoint(savePoint);
        scaner.SetLine(saveLine);
    }
    do {
        AnaliseAdd();
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
    } while (currentType == Less || currentType == More || currentType == Lesseq || currentType == Moreeq ||
        currentType == Equal || currentType == Negeq);
    scaner.SetPoint(savePoint);
    scaner.SetLine(saveLine);
}

//Слагаемое
void Analisys::AnaliseAdd() {
    do {
        AnaliseMultiply();
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
    } while (currentType == Plus || currentType == Minus);
    scaner.SetPoint(savePoint);
    scaner.SetLine(saveLine);
}

//Множитель
void Analisys::AnaliseMultiply() {
    do {
        AnaliseNe();
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
    } while (currentType == Divleft || currentType == Div || currentType == Mult);
    scaner.SetPoint(savePoint);
    scaner.SetLine(saveLine);
}

//Логическое не
void Analisys::AnaliseNe() {
    savePoint = scaner.GetPoint();
    saveLine = scaner.GetLine();
    currentType = scaner.Scan(&currentLex);
    if (currentType != Neg) {
        scaner.SetPoint(savePoint);
        scaner.SetLine(saveLine);
    }
    AnaliseBasicExpress();
}

//Элементарное выражение
void Analisys::AnaliseBasicExpress() {
    savePoint = scaner.GetPoint();
    saveLine = scaner.GetLine();
    currentType = scaner.Scan(&currentLex);
    if (currentType == LBracket) {
        AnaliseExpression();
        currentType = scaner.Scan(&currentLex);
        if (currentType != RBracket) ShowError("Not )");
    }
    else if (currentType == Id) {
        currentType = scaner.Scan(&currentLex);
        if (currentType == LBracket) {
            scaner.SetPoint(savePoint);
            scaner.SetLine(saveLine);
            AnaliseAskFunc();
        }
        else {
            scaner.SetPoint(savePoint);
            scaner.SetLine(saveLine);
            currentType = scaner.Scan(&currentLex);
            root->CheckIfVarExist(root->Cur, currentLex, true);
        }
    }
    else if (currentType != constInt10 && currentType != constInt16 && currentType != keyTrue && currentType != keyFalse) 
        ShowError("Not basic expression");
}

//Вызов функции
void Analisys::AnaliseAskFunc() {
    currentType = scaner.Scan(&currentLex);
    if (currentType != Id) ShowError("Not id");
    std::string FuncId = currentLex;
    int param = 0;
    currentType = scaner.Scan(&currentLex);
    if (currentType != LBracket) ShowError("Not (");
    savePoint = scaner.GetPoint();
    saveLine = scaner.GetLine();
    currentType = scaner.Scan(&currentLex);
    if (currentType != RBracket) {
        scaner.SetPoint(savePoint);
        scaner.SetLine(saveLine);
        do {
            AnaliseExpression();
            param++;
            savePoint = scaner.GetPoint();
            saveLine = scaner.GetLine();
            currentType = scaner.Scan(&currentLex);
        } while (currentType == Comma);
        if (currentType != RBracket) ShowError("Not )");
    }
    root->CheckIfFuncExist(root->Cur, -1, FuncId, param, true);
}

//switch
void Analisys::AnaliseSwitch() {
    SematicTree* SavedPos;
    currentType = scaner.Scan(&currentLex);
    if (currentType != keySwitch) ShowError("Not switch");
    currentType = scaner.Scan(&currentLex);
    if (currentType != LBracket) ShowError("Not (");
    AnaliseExpression();
    currentType = scaner.Scan(&currentLex);
    if (currentType != RBracket) ShowError("Not )");
    currentType = scaner.Scan(&currentLex);
    if (currentType != LBrace) ShowError("Not {");
    currentType = scaner.Scan(&currentLex);
    while (currentType == keyCase) {
        currentType = scaner.Scan(&currentLex);
        if (currentType != constInt10 && currentType != constInt16 && currentType != keyTrue && currentType != keyFalse) 
            ShowError("Not const");
        currentType = scaner.Scan(&currentLex);
        if (currentType != Twopoint) ShowError("Not :");
        SavedPos = root->NewBlock();
        AnaliseBodyCase();
        root->SetCurrent(SavedPos);
        currentType = scaner.Scan(&currentLex);
    }
    if (currentType == keyDefault) {
        currentType = scaner.Scan(&currentLex);
        if (currentType != Twopoint) ShowError("Not :");
        SavedPos = root->NewBlock();
        AnaliseBodyCase();
        root->SetCurrent(SavedPos);
        currentType = scaner.Scan(&currentLex);
    }
    if (currentType != RBrace) ShowError("Not }");
}

//Тело case
void Analisys::AnaliseBodyCase() {
    savePoint = scaner.GetPoint();
    saveLine = scaner.GetLine();
    currentType = scaner.Scan(&currentLex);
    while (currentType != keyCase && currentType != keyDefault && currentType != RBrace) {
        if (currentType == keyBreak) {
            currentType = scaner.Scan(&currentLex);
            if (currentType != Semicolon) ShowError("Not ;");
        }
        else {
            scaner.SetPoint(savePoint);
            scaner.SetLine(saveLine);
            AnaliseOp();
        }
        savePoint = scaner.GetPoint();
        saveLine = scaner.GetLine();
        currentType = scaner.Scan(&currentLex);
    }
    scaner.SetPoint(savePoint);
    scaner.SetLine(saveLine);
}

Analisys::Analisys(std::string fileName) {
    scaner.ReadFile(fileName);
    currentType = -1;
    savePoint = -1;
    saveLine = -1;
    SematicTree::SetCurrent(root);
}