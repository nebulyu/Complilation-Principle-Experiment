#include<bits/stdc++.h>
using namespace std;
extern string InputCode;
int LastLegalLine;
int ErrorType;
int code_is_legal(){
    LastLegalLine=1;
    for(int i=0;i<InputCode.size();++i){
        char c=InputCode[i];
        if(c=='\''){
            if(i+2>=InputCode.size()) {ErrorType=1;return 0;}
            if(InputCode[i+2]!='\'') {ErrorType=1;return 0;}
            i+=2;continue;
        }
        if(c=='/'&&InputCode[i+1]=='*'){
            if(i+3>=InputCode.size()) {ErrorType=2;return 0;}
            i+=2,c=InputCode[i];
            while(i+1<InputCode.size()&&!(c=='*'&&InputCode[i+1]=='/')){
                if(c=='\n')++LastLegalLine;
                ++i,c=InputCode[i];
            }
            if(i+1>=InputCode.size()) {ErrorType=2;return 0;}
            if(c=='\n')++LastLegalLine;
            ++i;
            continue;
        }
        if(c=='\n')++LastLegalLine;
    }
    return 1;
}
string ErrorCase[]={"",
"Single quotation mark cannot pair",
"Unfinished multi lines comment",
};
void Output_illegal(){
    ofstream OutputResult("output.txt");
    OutputResult<<"Illegal input!\n";
    OutputResult<<"Error may occur in the line:"<<LastLegalLine<<"\n";
    OutputResult<<ErrorCase[ErrorType]<<"\n";
}