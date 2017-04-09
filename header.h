#ifndef READER_H
#define READER_H

#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

class Reader
{
    private:
        stack <string> myStack;
        int depth =0 ;
        int maxDepth = 0;
        vector <string> variables;
        vector <string> operands;
        vector <string> errors;
        vector <string> constants;
        bool innerError = false;
        bool forPreceding = false;
        bool beginPreceding=false;
        bool hasSemi = false;
        bool hasComma = false;
        vector <string> secondStack;
        vector<string> keywords;

    public:
        void readLine(string);
        void checkForMissing();
        void decipherLine(string line);
        void forDepth();
        void printStack();
        void addToStack(string sub);
        int getdepth();
        int getmaxDepth();
        void forLine(vector<string>&);
        void beginLine(vector<string>&);
        void functionLine(vector<string>&);
        void endLine(vector<string>&);
        void analyzeStack();
        bool getInnerError();
        vector<string>getErrors();
        vector<string>getConstants();
        vector<string>getVariables();
        vector<string>getOperands();
        vector<string>getSyntax();
        vector<string>getKeywords();
};



#endif // READER_H
