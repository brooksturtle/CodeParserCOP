#include "reader.h"


vector<string>Reader::getSyntax()
{
    vector<string>syntax;
    if (hasComma)
    {
        syntax.push_back(",");
    }
    if (hasSemi)
    {
        syntax.push_back(";");
    }
    return syntax;
}
vector<string>Reader::getKeywords()
{
    return keywords;
}

bool Reader::getInnerError()
{
    return innerError;
}
int Reader::getdepth()
{
    return depth;
}
int Reader::getmaxDepth()
{
    return maxDepth;
}

vector<string>Reader::getErrors()
{
    return errors;
}

vector<string>Reader::getConstants()
{
    return constants;
}

vector<string>Reader::getVariables()
{
    return variables;
}

vector<string>Reader::getOperands()
{
    return operands;
}

void Reader::readLine(string name)
{
    int counter = 0;

    ifstream infile(name.c_str());
    string line;
    while (getline(infile, line))
    {
        istringstream iss(line);

        //cout<<counter<<". "<<line<<endl;
        //counter++;

        decipherLine(line);
        //cout<<endl;
    }
}

// intakes line from file and distinguishes it into individual words
void Reader::decipherLine(string line)
{
    istringstream iss(line);

    string sub;
    string subPre;
    vector <string> lineWords;

    // adding all spaced strings in line to a vector
    while (iss)
    {
        iss >> sub;
        // this prevents double input of last word in line
        if (sub == subPre)
        {
            break;
        }
        subPre= sub;
        lineWords.push_back(sub);
    }
    // deciphering vector, deciding which function to send it to
    for (unsigned i=0; i < lineWords.size(); i++)
    {
        sub=lineWords[i];
        // exhausting all cases for which this may be a for line
        if (sub== "FOR" ||sub[0] =='F' || sub[0]=='f'||sub[0]=='('
            || (sub[1]=='o' || sub[1]=='O' && (sub[2]=='R' ||sub[2]=='r')))
            {
                forLine(lineWords);
                forPreceding = true;
                beginPreceding=false;
                myStack.push("FOR");
                break;
            }
        if (sub=="BEGIN" || sub[0] =='B' || sub[0] == 'b' || forPreceding)
        {
            beginLine(lineWords);
            myStack.push("BEGIN");
            forPreceding = false;
            beginPreceding = true;
            break;
        }
        bool equalsExist = false;
        for (int i=0; i<sub.length(); i++)
        {
            if (sub[i]=='=')
            {
                equalsExist=true;
            }
        }
        //
        if (beginPreceding || equalsExist && sub!= "END")
        {

            functionLine(lineWords);
            beginPreceding=false;
            forPreceding=false;
            myStack.push("FUNCTION");
            break;
        }
        if (sub =="END" || sub[0] == 'E' || sub[0] == 'e' ||
            (sub[1]=='N' && sub[2]=='D') || sub =="end")
        {
            endLine(lineWords);
            myStack.push("END");
        }

    }
    analyzeStack();
}

// this checks for extra lines, or to see if any are missing
void Reader::checkForMissing()
{
    int enormity=secondStack.size();

    int numberFor =0;
    int numberBegin =0;
    int numberFunction =0;
    int numberEnd=0;
    for (int i = 0; i< enormity; i++)
    {
        if (secondStack[i]=="FOR")
            numberFor++;
        if (secondStack[i]=="BEGIN")
            numberBegin++;
        if (secondStack[i]=="FUNCTION")
            numberFunction++;
        if (secondStack[i]=="END")
            numberEnd++;
    }
    // gets mode of numbers
    double avg = (double)(numberBegin+numberEnd+numberFor+numberFunction)/4;
    double remainder= avg - floor(avg);
    int mode;
    if (remainder>= .5)
    {
        mode = ceil(avg);
    }
    else
        mode = floor(avg);

    if (numberBegin!= mode)
    {
        innerError=true;
            errors.push_back("BEGIN");
    }
    if (numberEnd!= mode)
    {
        innerError=true;
            errors.push_back("END");
    }
    if (numberFor!= mode)
    {
        innerError=true;
            errors.push_back("FOR");
    }

    // now we will get key words
    if(numberFor>0)
        keywords.push_back("FOR");
    if(numberBegin>0)
        keywords.push_back("BEGIN");
    if(numberEnd>0)
        keywords.push_back("END");
}
// this will count number of nested loops
void Reader::analyzeStack()
{
    secondStack.push_back(myStack.top());
    int stackIndex = 0;
    // resets innerLoop by erasing it from stack


        if(myStack.top()=="BEGIN")
        {
            depth++;
            if (depth>maxDepth)
            {
                maxDepth=depth;
            }
        }
        if (myStack.top()=="END")
        {
            depth--;
            if (depth>=0)
            {
                myStack.pop();
                while(myStack.top()!= "END" && myStack.size()!=0)
                    {
                        myStack.pop();
                        if (myStack.top()=="FOR")
                        {
                            myStack.pop();
                            break;
                        }
                    }
            }
        }

    // Going to pop for loop if there are missing lines

}

void Reader::endLine(vector<string>& lineWords)
{
    bool correctEnd = true;
    int enormity = lineWords.size();
    if (enormity >1 )
    {
        correctEnd = false;
        for (int i =0; i< enormity; i++)
        {
            if (lineWords[i]!= "END")
            {
                if(find(errors.begin(), errors.end(), lineWords[i]) == errors.end())
                    errors.push_back(lineWords[i]);

                innerError=true;
            }
        }
    }
    else
    {
        if (lineWords[0]=="END")
        {

        }
        else
        {
            if(find(errors.begin(), errors.end(), lineWords[0]) == errors.end())
                errors.push_back(lineWords[0]);
            innerError=true;
        }

    }
}
void Reader::functionLine(vector<string>& lineWords)
{
    string sub = lineWords[0];
    bool correctFunction = true;
    int index = -1;
    // find index of equals sign to split sub
    for (int i =0; i<sub.length(); i++)
    {
        if (sub[i] == '=')
            index = i;
    }
    string sub1;
    string sub2;
    if (index == -1)
    {
        correctFunction = false;

    }

    // cannot assume that it is same variable on either side
    else
    {
        sub1= sub.substr(0, index);
        sub2= sub.substr(index+1, sub.length());

        if (sub1==sub2)
        {
            variables.push_back(sub1);
        }
        else
        {
            correctFunction=false;
            variables.push_back(sub1);
            errors.push_back(sub2);
        }
    }

    //sub = lineWords[1];
    int enormity = lineWords.size();
    // makes sure there is always variable, then operation
    bool opPrevious=false;
    // i should just take semicolon off end and work from there
    sub = lineWords[enormity-1];
    // removes semicolon from last string
    if (sub[sub.length()-1] == ';')
    {
        sub = sub.substr(0, sub.length()-1);
        hasSemi =true;
        lineWords.erase(lineWords.begin()+enormity-1);
        lineWords.push_back(sub);
        enormity=lineWords.size();
    }
    else
        correctFunction=false;

        for (int i =1; i<enormity ; i++)
        {
            if (lineWords[i] == "+" || lineWords[i]== "-" || lineWords[i] =="/" || lineWords[i] =="*")
            {
                if (opPrevious )
                {
                    correctFunction=false;
                    errors.push_back(lineWords[i]);
                }

                if(find(operands.begin(), operands.end(), lineWords[i]) == operands.end())
                    {
                        operands.push_back(lineWords[i]);
                    }
                opPrevious=true;
            }
            else
            {
                if (opPrevious == false)
                {
                    correctFunction=false;
                    errors.push_back(lineWords[i]);
                }
                // must already be defined variable
                else if(find(variables.begin(), variables.end(), lineWords[i]) == variables.end())
                    {
                        errors.push_back(lineWords[i]);
                        correctFunction=false;
                    }
                opPrevious = false;
            }
        }
            // if last is an operation sign then this will be pushed to errors. and lien is incorrect
            if (opPrevious)
            {
                correctFunction=false;
                errors.push_back(lineWords[enormity-1]);
            }
            if (!correctFunction)
                innerError=true;
}
// this will be fairly simple to see if line is correct. I just need to worry about damage control,
// also the possibility of multiple begins and other shit like an end creeping in.
// for now I should just worry about analyzing each line seperately and work on the weird cases after.
void Reader::beginLine(vector<string>& lineWords)
{
    bool correctBegin = true;
    int enormity = lineWords.size();
    if (enormity >1 )
    {
        correctBegin = false;
        for (int i =0; i< enormity; i++)
        {
            if (lineWords[i]!= "BEGIN")
            {
                if(find(errors.begin(), errors.end(), lineWords[i]) == errors.end())
                    errors.push_back(lineWords[i]);

                innerError=true;
            }
        }
    }
    else
    {
        if (lineWords[0]=="BEGIN")
        {

        }
        else
        {
            if(find(errors.begin(), errors.end(), lineWords[0]) == errors.end())
                errors.push_back(lineWords[0]);
            innerError=true;
        }

    }

}

// analyzes correctness of for Line, correct syntax, and variables
void Reader::forLine(vector<string>& lineWords)
{


    bool correctFor = true;
    string sub = lineWords[0];
    // check if FOR is spelled correctly
    if (sub== "FOR" ||sub[0] =='F' || sub[0]=='f'
        || (sub[1]=='o' || sub[1]=='O' && (sub[2]=='R' ||sub[2]=='r')))
        {
            // check for FOR incorrectness
            // if so it erases the for and allows us to continue analyzing line
            if (sub !="FOR" && sub[3]!='(')
            {
                correctFor = false;

                // erase the for word if it is not the paranthesese
                    errors.push_back(sub);
                    lineWords.erase(lineWords.begin()+0);

            }
            // checks case of no space between for and (j, will erase for and resume with (j
            else if (sub[3] == '(' && sub.substr(0,3) == "FOR")
            {
                lineWords.erase(lineWords.begin()+0);
                lineWords.insert(lineWords.begin(), sub.substr(3, sub.length()));
            }
            else if(sub[3]=='(' && sub.substr(0, 3) != "FOR")
            {
                errors.push_back(sub.substr(0,3));
                correctFor= false;
                lineWords.erase(lineWords.begin()+0);
                lineWords.insert(lineWords.begin(), sub.substr(3, sub.length()));

            }
            // get rid of word
            else
            {
                lineWords.erase(lineWords.begin()+0);
            }
        }
        // going to see to add FOR or mispelled word to errors
        if (sub[0] == '(' )
                {
                    errors.push_back("FOR");
                    correctFor = false;
                }
    bool hasPara = false;
    // this will be used to count how many paranthesis we have, error if this doesnt equal zero;
    int paraIndex = 0;
    sub = lineWords[0];
    // decipher first spaces of for()
    for (unsigned int i=0; i<sub.length(); i++)
    {
        if (sub[i] == '(')
        {
            hasPara = true;
            paraIndex++;
        }
    }
    // this will clean up rest of string, make it easier to operate on later.
    int counter = 0;
    if (paraIndex==1 || paraIndex>1)
    {

        while (counter < paraIndex)
        {
            sub.erase(sub.begin()+0);
            counter++;
        }
    }
    // else hasPara is already false
    // now going to look at first variable
    bool hasComma1 = false;
    string aVariable;
    //pushes first variable if it exist
    if (sub[sub.length()-1]==',')
    {
        hasComma1 = true;
        hasComma=true;
        aVariable = sub.substr(0, sub.length()-1);

       // checks for duplicate before pushing back
        if(find(variables.begin(), variables.end(), aVariable) == variables.end())
            variables.push_back(aVariable);

    }
    else
    {   // checks for duplicates before pushing back
        if(find(variables.begin(), variables.end(), aVariable) == variables.end())
            variables.push_back(sub);

        errors.push_back(",");
    }
    sub = lineWords[1];
    bool hasComma2 = false;
    string aConstant;

    if (sub[sub.length()-1]==',')
    {
        hasComma2 = true;
        hasComma= true;
        aConstant = sub.substr(0, sub.length()-1);
        // should check if aConstant is actually an int

    }
    // does not have comma
    else
    {
        // still need to check if constant is just numbers
        aConstant= sub.substr(0,sub.length());
    }
    // now that we are done with the first word, lets look at the next
    bool aNumber = false;
        bool goodConstant = true;
        for (int j = 0; j++; j<sub.length()-2)
        {
            aNumber = false;
            for (int i =0; i++; i<10)
            {
                if (sub[j] == '0'+i)
                {
                    aNumber = true;
                }
            }
            if (!aNumber)
                goodConstant = false;
        }
        if (goodConstant)
        {
            // checks if duplicate constant already exist
            if(find(constants.begin(), constants.end(), aConstant) == constants.end())
                constants.push_back(aConstant);
        }
        else
            errors.push_back(aConstant);

        if (!hasComma2)
            errors.push_back(",");

        // now we will work on the final part of the FOR ()
    sub = lineWords[2];
    int extraPara=0;
    hasPara = false;
    for (int i = 0; i<sub.length(); i++)
    {
        if (sub[i] == '(')
        {
            paraIndex++;
            extraPara++;
            hasPara = true;
        }
        if (sub[i]== ')')
        {
            extraPara++;
            paraIndex--;
            hasPara = true;
        }
    }
    // if less, than extra ')', para dont match up

        if (paraIndex< 0)
        {
            errors.push_back(")");
            correctFor = false;
        }
        if (paraIndex> 0)
        {
            errors.push_back("(");
            correctFor = false;
        }
    // now we are going to delete the parenthesis from sub
    if (hasPara)
    {
            sub = sub.substr(0, sub.length()-extraPara);
    }

    if(sub == "++" || sub == "--")
    {
        if(find(operands.begin(), operands.end(), sub) == operands.end())
            operands.push_back(sub);
    }
    // not a function
    else
    {
        correctFor = false;
        errors.push_back(sub);
    }
    // analysis if forLoop is correct, add to stack if it is
    if (correctFor && hasPara && hasComma1 && hasComma2 && goodConstant )
    {

    }
    else
    {
        // I am pushing it regardless so I can keep track of order later

        innerError = true;
    }
}
// need way to throw away the whole loop if there is a syntax error
// deciphers each string and puts for begin and end into a stack


