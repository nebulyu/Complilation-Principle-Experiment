#include<bits/stdc++.h>
#include<unordered_map>
#include<fstream>
#include"check_legal.cpp"
using namespace std;
enum class TokenType{
    //default type
    IDENTIFIER,
    //read from config files
    KEYWORD,
    OPERATOR,
    DELIMITER,
    //string type
    LITERAL,
    COMMENT
};
map<TokenType,string> StringOfTokenType={
    {TokenType::IDENTIFIER,"Identifier"},
    {TokenType::KEYWORD,"Keyword"},
    {TokenType::OPERATOR,"Operator"},
    {TokenType::DELIMITER,"Delimiter"},
    {TokenType::LITERAL,"Literal"},
    {TokenType::COMMENT,"Comment"}
};
class Token{
    public:
    TokenType type;
    string lexeme;
    int line;
};
vector<Token>tokens;
unordered_map <string,int > KeywordMap;
unordered_map <string,int > OperatorMap;
unordered_map <string,int > DelimiterMap;
void Try_Push_Lexeme(string &Lexeme,int CurrentLine,TokenType type = TokenType::IDENTIFIER){
    if(Lexeme.empty())return ;
    if(KeywordMap.count(Lexeme)>0)
    type = TokenType::KEYWORD;
    // cerr<<StringOfTokenType[type]<<" "<<Lexeme<<" "<<CurrentLine<<"\n";
    tokens.push_back({type,Lexeme,CurrentLine});
    Lexeme="";
}
string InputCode,OutputResult;
ofstream DebugOutput;
void Input_File(){
    // ofstream DebugOutput("debug.txt");
    string inputstring;
    ifstream inputFile("input.txt"); 
    InputCode = string((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    ifstream KeywordFile("config/keyword.txt");
    while(KeywordFile>>inputstring) KeywordMap[inputstring]=1;
    ifstream OperatorFile("config/operator.txt");
    while(OperatorFile>>inputstring) OperatorMap[inputstring]=1;
    ifstream DelimiterFile("config/delimiter.txt");
    while(DelimiterFile>>inputstring) DelimiterMap[inputstring]=1;
}
void Lexical_Analyse(){
    int CurrentLine = 1 ;
    string Lexeme = "";
    bool FlagChar = 0;
    bool FlagString = 0;
    for(int i=0;i<InputCode.size();++i){
        char c = InputCode[i];
        string tempstring = "";
        tempstring+=c;
        // new line
        if(c=='\n'){
            Try_Push_Lexeme(Lexeme,CurrentLine);
            CurrentLine+=1;
            continue;
        }
        // isspace will accept \n, so put new line processing forward
        if(isspace(c)){
            //space in a string
            if(FlagString){
                Lexeme += c;
                continue;
            }
            Try_Push_Lexeme(Lexeme,CurrentLine);
            continue;
        }
        if(c=='\''){
            if(FlagChar)
            Try_Push_Lexeme(Lexeme,CurrentLine,TokenType::LITERAL);
            else 
            Try_Push_Lexeme(Lexeme,CurrentLine);
            FlagChar^=1;
            continue;
        } 
        if(c=='"'){
            if(FlagString)
            Try_Push_Lexeme(Lexeme,CurrentLine,TokenType::LITERAL);
            else 
            Try_Push_Lexeme(Lexeme,CurrentLine);
            FlagString^=1;
            continue;
        } 
        if(isalpha(c) || isdigit(c)){
            Lexeme += c;
            continue;
        }
        // single line comment
        if(i+1<InputCode.size()){
            if(c=='/'&&InputCode[i+1]=='/'){
                Try_Push_Lexeme(Lexeme,CurrentLine);
                i+=2,c=InputCode[i];
                while(i<InputCode.size()&&c!='\n')
                Lexeme+=c,++i,c=InputCode[i];
                Try_Push_Lexeme(Lexeme,CurrentLine,TokenType::COMMENT);
                if(c=='\n')++CurrentLine;
                continue;
            }
        }
        // multi line comment
        if(i+3<InputCode.size()){
            if(c=='/'&&InputCode[i+1]=='*'){
                Try_Push_Lexeme(Lexeme,CurrentLine);
                i+=2,c=InputCode[i];
                int lineincrease=0;
                while(i+1<InputCode.size()&&!(c=='*'&&InputCode[i+1]=='/')){
                    if(c=='\n')lineincrease++;
                    Lexeme+=c,++i,c=InputCode[i];
                }
                Try_Push_Lexeme(Lexeme,CurrentLine,TokenType::COMMENT);
                CurrentLine+=lineincrease;
                if(c=='\n')++CurrentLine;
                ++i;
                continue;
            }
        }
        // delimiter
        if(DelimiterMap.count(tempstring)>0){
            // cerr<<"process \n";
            Try_Push_Lexeme(Lexeme,CurrentLine);
            Lexeme = tempstring;
            Try_Push_Lexeme(Lexeme,CurrentLine,TokenType::DELIMITER);
            continue;
        }
        // double length operator 
        if(i+1<InputCode.size()){
            string trydoubleoperator = tempstring + InputCode[i+1];
            if(OperatorMap.count(trydoubleoperator)>0){
                Try_Push_Lexeme(Lexeme,CurrentLine);
                Lexeme = trydoubleoperator;
                Try_Push_Lexeme(Lexeme,CurrentLine,TokenType::OPERATOR);
                ++i;
                continue;
            }
        }
        // single length operator
        if(OperatorMap.count(tempstring)>0){
            Try_Push_Lexeme(Lexeme,CurrentLine);
            Lexeme = tempstring;
            Try_Push_Lexeme(Lexeme,CurrentLine,TokenType::OPERATOR);
            continue;
        }
    }
}
void Output_Result(){
    ofstream OutputResult("temp/token.txt");
    int count=0;
    for(auto item:tokens){
        OutputResult<<(to_string(++count)+".Type: "+StringOfTokenType[item.type]);
        OutputResult<<(" Line: "+to_string(item.line));
        OutputResult<<(" Lexeme: "+item.lexeme+"\n");
    }
}
signed main(){
    Input_File();
    if(code_is_legal()){
        Lexical_Analyse();
        Output_Result();
    }
    else{
        Output_illegal();
    }
    return 0;
}
