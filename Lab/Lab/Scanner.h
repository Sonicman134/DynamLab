#include <iostream>
#include <string>
#include <map>

#define Id 1
#define keyBool 11
#define keyShort 12
#define keyLong 13
#define keyInt 14
#define keySwitch 15
#define keyDefault 16
#define keyCase 17
#define keyBreak 18
#define keyReturn 19
#define keyMain 20
#define keyTrue 21
#define keyFalse 22
#define keyVoid 23
#define constInt10 31
#define constInt16 32
#define Comma 41
#define Twopoint 42
#define Semicolon 43
#define LBracket 44
#define RBracket 45
#define LBrace 46
#define RBrace 47
#define Less 51
#define More 52
#define Lesseq 53
#define Moreeq 54
#define Equal 55
#define Negeq 56
#define Or 57
#define And 58
#define Plus 59
#define Minus 60
#define Mult 61
#define Div 62
#define Divleft 63
#define Neg 64
#define Save 65
#define End 100
#define Error 200

class Scanner
{
private:
	std::string text;
	int point;
    int line;
    std::string keywords[13];
    int keynums[13];
public:
    int Scan(std::string *res);
    void ReadFile(std::string file);
    int GetLine();
    int GetPoint();
    void SetLine(int newLine);
    void SetPoint(int newpoint);
    Scanner();
};
