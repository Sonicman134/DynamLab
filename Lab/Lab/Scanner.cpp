#include "Scanner.h"

Scanner::Scanner() {
    point = 0;
    line = 1;
    keywords[0] = "bool";
    keywords[1] = "short";
    keywords[2] = "long";
    keywords[3] = "int";
    keywords[4] = "switch";
    keywords[5] = "default";
    keywords[6] = "case";
    keywords[7] = "break";
    keywords[8] = "return";
    keywords[9] = "main";
    keywords[10] = "true";
    keywords[11] = "false";
    keywords[12] = "void";
    keynums[0] = keyBool;
    keynums[1] = keyShort;
    keynums[2] = keyLong;
    keynums[3] = keyInt;
    keynums[4] = keySwitch;
    keynums[5] = keyDefault;
    keynums[6] = keyCase;
    keynums[7] = keyBreak;
    keynums[8] = keyReturn;
    keynums[9] = keyMain;
    keynums[10] = keyTrue;
    keynums[11] = keyFalse;
    keynums[12] = keyVoid;
}

int Scanner::GetLine() {
    return line;
}

void Scanner::SetLine(int newLine) {
    line = newLine;
}

int Scanner::GetPoint() {
    return point;
}

void Scanner::SetPoint(int newpoint) {
    point = newpoint;
}

void Scanner::ReadFile(std::string fileName) {
    std::string textInfo;
    FILE* file;
    fopen_s(&file, fileName.c_str(), "r");

    if (file == nullptr) {
        std::cout << "File is not open" << std::endl;
        std::exit(0);
    }
    while (!feof(file)) {
        char c;
        fscanf_s(file, "%c", &c);
        //fread(&c, sizeof(char), 1, file);
        textInfo.append(std::string(1, c));
    }

    textInfo.append("\0");
    textInfo.erase(textInfo.length() - 1);
    fclose(file);

    this->text = textInfo;
    point = 0;
    line = 1;
}

int Scanner::Scan(std::string *res)
{
    *res = "";
    while (true) {
        while ((text[point] == ' ') || (text[point] == '\n')) {
            if (text[point] == '\n') line++;
            point++;
        }
        if ((text[point] == '/') && (text[point + 1] == '/')) {
            point += 2;
            while (text[point] != '\n' && text[point] != '\0')
                point++;
        }
        else if ((text[point] == '/') && (text[point + 1] == '*')) {
            point += 2;
            while ((text[point + 1] != '/') || (text[point] != '*')) {
                if (text[point] == '\n') line++;
                point++;
                if (text[point + 1] == '\0') {
                    *res = "End";
                    return End;
                }
            }
            point += 2;
        }
        else if (text[point] == '\0') {
            *res = "End";
            return End;
        }
        else
        {
            if ((text[point] <= '9') && (text[point] >= '1')) {
                (*res).append(std::string(1, text[point++]));
                while ((text[point] <= '9') && (text[point] >= '0'))
                    (*res).append(std::string(1, text[point++]));
                return constInt10;
            }
            else if ((text[point] == '0')) {
                (*res).append(std::string(1, text[point++]));
                if (text[point] == 'x' || text[point] == 'X') {
                    (*res).append(std::string(1, text[point++]));
                    while (((text[point] <= '9') && (text[point] >= '0')) || ((text[point] <= 'F') && (text[point] >= 'A')))
                        (*res).append(std::string(1, text[point++]));
                    return constInt16;
                }
                else if ((text[point] > '9') || (text[point] < '0')) {
                    return constInt10;
                }
                while ((text[point] <= '9') && (text[point] >= '0')) {
                    (*res).append(std::string(1, text[point++]));
                }
                std::cout << "Wrong at line " + std::to_string(line) + "  Error symbol: " + *res + "\n";
                exit(Error);
                return Error;
            }
            else if (((text[point] >= 'a') && (text[point] <= 'z')) || text[point] == '_' || ((text[point] >= 'A') && (text[point] <= 'Z'))) {
                (*res).append(std::string(1, text[point++]));
                while ((text[point] <= '9') && (text[point] >= '0') || // состояние N2
                    ((text[point] >= 'a') && (text[point] <= 'z')) || text[point] == '_' || ((text[point] >= 'A') 
                        && (text[point] <= 'Z'))) (*res).append(std::string(1, text[point++]));
                for (int i = 0; i < 12; i++) {
                    if (*res == keywords[i]) return keynums[i];
                }
                return Id;
            }
            else if (text[point] == ',') {
                (*res).append(std::string(1, text[point++])); 
                return Comma;
            }
            else if (text[point] == ':') {
                (*res).append(std::string(1, text[point++])); 
                return Twopoint;
            }
            else if (text[point] == ';') {
                (*res).append(std::string(1, text[point++]));
                return Semicolon;
            }
            else if (text[point] == '(') {
                (*res).append(std::string(1, text[point++])); 
                return LBracket;
            }
            else if (text[point] == ')') {
                (*res).append(std::string(1, text[point++])); 
                return RBracket;
            }
            else if (text[point] == '{') {
                (*res).append(std::string(1, text[point++])); 
                return LBrace;
            }
            else if (text[point] == '}') {
                (*res).append(std::string(1, text[point++])); 
                return RBrace;
            }
            else if (text[point] == '|') {
                (*res).append(std::string(1, text[point++]));
                if (text[point] == '|') {
                    (*res).append(std::string(1, text[point++]));
                    return Or;
                }
            }
            else if (text[point] == '&') {
                (*res).append(std::string(1, text[point++]));
                if (text[point] == '&') {
                    (*res).append(std::string(1, text[point++]));
                    return And;
                }
            }
            else if (text[point] == '+') {
                (*res).append(std::string(1, text[point++])); 
                return Plus;
            }
            else if (text[point] == '-') {
                (*res).append(std::string(1, text[point++])); 
                return Minus;
            }
            else if (text[point] == '*') {
                (*res).append(std::string(1, text[point++])); 
                return Mult;
            }
            else if (text[point] == '/') {
                (*res).append(std::string(1, text[point++]));
                return Div;
            }
            else if (text[point] == '%') {
                (*res).append(std::string(1, text[point++]));
                return Divleft;
            }
            else if (text[point] == '!') {
                (*res).append(std::string(1, text[point++]));
                if (text[point] == '=') { 
                    (*res).append(std::string(1, text[point++])); 
                    return Negeq;
                }
                else return Neg;
            }
            else if (text[point] == '<') {
                (*res).append(std::string(1, text[point++]));
                if (text[point] == '=') { 
                    (*res).append(std::string(1, text[point++])); 
                    return Lesseq;
                }
                else return Less;
            }
            else if (text[point] == '>') {
                (*res).append(std::string(1, text[point++]));
                if (text[point] == '=') { 
                    (*res).append(std::string(1, text[point++])); 
                    return Moreeq;
                }
                else return More;
            }
            else if (text[point] == '=') {
                (*res).append(std::string(1, text[point++]));
                if (text[point] == '=') { 
                    (*res).append(std::string(1, text[point++])); 
                    return Equal;
                }
                else return Save;
            }   
            (*res).append(std::string(1, text[point++]));
            std::cout << "Wrong at line " + std::to_string(line) + "  Error symbol: " + *res + "\n";
            exit(Error);
            return Error;
        }
    }
}