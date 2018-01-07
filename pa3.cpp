#include "header.h"

using namespace std;


int main(int argc,  char *argv[])
{

    header reader;
    reader.readLine(argv[1]);
    reader.checkForMissing();

    vector<string>errors= reader.getErrors();
    vector<string>constants = reader.getConstants();
    vector<string>variables = reader.getVariables();
    vector<string>operands = reader.getOperands();
    vector<string>syntax = reader.getSyntax();
    vector<string>keywords =reader.getKeywords();


    cout<<"OUTPUT> The depth of nested loop(s) is ";
    if (reader.getInnerError())
    {
        cout<<reader.getmaxDepth()-1;
    }
    else
    {
        cout<<reader.getmaxDepth();
    }

    cout<<endl<<endl;
    cout<<"Keywords: ";
     for (int i = 0; i<keywords.size(); i++)
    {
        cout<<keywords[i]<<" ";
    }

    cout<<endl<<"Identifier: ";
    for (int i = 0; i<variables.size(); i++)
    {
        cout<<variables[i]<<" ";
    }

    cout<<endl<<"Constant: ";
    for (int i = 0; i<constants.size(); i++)
    {
        cout<<constants[i]<<" ";
    }


    cout<<endl<<"Operatros: ";
    for (int i = 0; i<operands.size(); i++)
    {
        cout<<operands[i]<<" ";
    }

    cout<<endl<<"Delimiters: ";
    for (int i = 0; i<syntax.size(); i++)
    {
        cout<<syntax[i]<<" ";
    }

    cout<<endl<<endl;

    cout<<"Syntax Error(s): ";
    for (int i = 0; i<errors.size(); i++)
    {
        cout<<errors[i]<<" ";

    }
    if (errors.size()== 0)
            cout<<"NA";



    //reader.printStack();
    //cout<<"depth: "<<reader.getdepth()<<endl;
    //cout<<"Max Depth: "<<reader.getmaxDepth()<<endl;
    //reader.printStack();
    /*
    stack<string> myStack;
    myStack.size();
    myStack.push("leafs");
    myStack.push("red");

    cout<<myStack.top()<<endl<<myStack.size()<<endl;

    myStack.pop();
    cout<<myStack.top()<<endl<<myStack.size();
    */
    //readLine();

}
