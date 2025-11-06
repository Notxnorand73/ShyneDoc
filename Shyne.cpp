#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <stack>
#include <vector>
#include <sstream>
#include <cmath>
#include <cctype>

using namespace std;

// --- Variables ---
unordered_map<string,double> num_vars;
unordered_map<string,string> str_vars;

// --- Utility ---
string trim(const string& s){
    size_t start=s.find_first_not_of(" \t");
    size_t end=s.find_last_not_of(" \t");
    return (start==string::npos)?"":s.substr(start,end-start+1);
}
string strip_quotes(const string& s){
    string t=trim(s);
    return (t.size()>=2 && t[0]=='"' && t.back()=='"')?t.substr(1,t.size()-2):t;
}

// --- Replace variables in numeric expressions ---
string replace_vars(const string& expr){
    string res; size_t i=0;
    while(i<expr.size()){
        if(isalpha(expr[i])){
            string var="";
            while(i<expr.size() && isalnum(expr[i])) var+=expr[i], i++;
            if(num_vars.find(var)!=num_vars.end()) res+=to_string(num_vars[var]);
            else { cerr<<"[Error] Undefined numeric variable: "<<var<<endl; res+="0"; }
        } else res+=expr[i], i++;
    }
    return res;
}

// --- PEMDAS ---
int precedence(char op){
    if(op=='+'||op=='-') return 1;
    if(op=='*'||op=='/'||op=='%') return 2;
    if(op=='^') return 3;
    return 0;
}
double apply_op(double a,double b,char op){
    switch(op){
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': return a/b;
        case '%': return fmod(a,b);
        case '^': return pow(a,b);
    }
    return 0;
}

double eval_expr(const string& expr){
    string exp = replace_vars(expr);
    stack<double> values; stack<char> ops; string token;
    for(size_t i=0;i<exp.size();i++){
        if(exp[i]==' ') continue;
        if(isdigit(exp[i])||exp[i]=='.'){ token=""; while(i<exp.size()&&(isdigit(exp[i])||exp[i]=='.')) token+=exp[i],i++; i--; values.push(stod(token)); }
        else if(exp[i]=='(') ops.push('(');
        else if(exp[i]==')'){ while(!ops.empty()&&ops.top()!='('){ double val2=values.top(); values.pop(); double val1=values.top(); values.pop(); char op=ops.top(); ops.pop(); values.push(apply_op(val1,val2,op)); } if(!ops.empty()) ops.pop(); }
        else{ while(!ops.empty()&&precedence(ops.top())>=precedence(exp[i])){ double val2=values.top(); values.pop(); double val1=values.top(); values.pop(); char op=ops.top(); ops.pop(); values.push(apply_op(val1,val2,op)); } ops.push(exp[i]); }
    }
    while(!ops.empty()){ double val2=values.top(); values.pop(); double val1=values.top(); values.pop(); char op=ops.top(); ops.pop(); values.push(apply_op(val1,val2,op)); }
    return values.top();
}

// --- in() ---
string handle_in_value(const string& line){
    size_t start=line.find("("), end=line.find(")");
    string prompt = (start!=string::npos && end!=string::npos) ? strip_quotes(line.substr(start+1,end-start-1)) : "";
    cout<<prompt; string input; getline(cin,input); return input;
}

// --- Assignment ---
void handle_assignment(const string& line){
    size_t colon=line.find(":"); if(colon==string::npos) return;
    string name=trim(line.substr(0,colon)), value=trim(line.substr(colon+1));

    if(value.find("in(")==0){
        string input = handle_in_value(value);
        try{ num_vars[name]=stod(input); } catch(...){ str_vars[name]=input; }
        return;
    }

    try{ double val=eval_expr(value); num_vars[name]=val; } 
    catch(...){ str_vars[name]=strip_quotes(value); }
}

// --- out() ---
void handle_out(const string& line){
    size_t start=line.find("("), end=line.find(")");
    if(start==string::npos||end==string::npos) return;
    string arg=trim(line.substr(start+1,end-start-1));
    if(num_vars.find(arg)!=num_vars.end()) cout<<num_vars[arg]<<endl;
    else if(str_vars.find(arg)!=str_vars.end()) cout<<str_vars[arg]<<endl;
    else { 
        try{ double val=eval_expr(arg); cout<<val<<endl; } 
        catch(...){ cout<<strip_quotes(arg)<<endl; } 
    }
}

// --- Get block ---
vector<string> get_block(vector<string>& lines, size_t& i){
    vector<string> block; int depth=0;
    for(;i<lines.size();i++){
        string t=trim(lines[i]);
        if(t.find("repeat ")==0 || t.find("if ")==0) depth++;
        if(t=="end"){ if(depth==0) break; else depth--; }
        block.push_back(lines[i]);
    }
    return block;
}

// --- Run lines ---
void run_lines(vector<string>& lines){
    for(size_t i=0;i<lines.size();i++){
        string t=trim(lines[i]);
        if(t.empty()||t[0]=='#') continue;

        // Assignment
        if(t.find(":")!=string::npos && t.find("repeat ")!=0) { handle_assignment(t); continue; }

        // out()
        if(t.find("out(")==0) { handle_out(t); continue; }

        // repeat loops
        if(t.find("repeat ")==0){
            size_t inpos = t.find(" in ");
            size_t colon = t.find(":");

            if(colon != string::npos){ // repeat a:b
                string after_repeat = trim(t.substr(7));
                size_t real_colon = after_repeat.find(":");
                if(real_colon == string::npos){ cout << "[Error] Invalid repeat syntax\n"; continue; }
                string end_str = trim(after_repeat.substr(0, real_colon));
                string var_name = trim(after_repeat.substr(real_colon+1));

                double end_val = eval_expr(end_str);
                double start_val = 1;
                vector<string> block = get_block(lines, ++i);
                for(double idx=start_val; idx<=end_val; idx++){
                    num_vars[var_name] = idx;
                    run_lines(block);
                }
                continue;
            }

            else if(inpos != string::npos){ // repeat b in a
                string after_repeat = trim(t.substr(7));
                size_t real_in = after_repeat.find(" in ");
                string var = trim(after_repeat.substr(0, real_in));
                string arrname = trim(after_repeat.substr(real_in+4));

                vector<string> elems;
                if(num_vars.find(arrname)!=num_vars.end()) elems.push_back(to_string(num_vars[arrname]));
                else if(str_vars.find(arrname)!=str_vars.end()){
                    stringstream ss(str_vars[arrname]); string item;
                    while(getline(ss,item,',')) elems.push_back(trim(item));
                } else { cout<<"[Error] repeat in: variable '"<<arrname<<"' not found.\n"; continue; }

                vector<string> block = get_block(lines, ++i);
                for(auto& e: elems){
                    try{ num_vars[var]=stod(e); } catch(...){ str_vars[var]=e; }
                    run_lines(block);
                }
                continue;
            }
        }

        // if/elif/else
        if(t.find("if ")==0 || t.find("elif ")==0 || t.find("else")==0){
            vector<string> block=get_block(lines,++i);
            string cond=t;
            if(cond.find("if ")==0) cond=cond.substr(3);
            else if(cond.find("elif ")==0) cond=cond.substr(5);
            bool result=false;
            if(cond!="") try{ result=(eval_expr(cond)!=0); } catch(...){ result=false; }
            if(cond=="" || result) run_lines(block);
            continue;
        }

        if(t=="end") continue;

        cout<<"Unknown command: "<<t<<endl;
    }
}

// --- main ---
int main(int argc,char* argv[]){
    if(argc<2){ cout<<"Usage: ShyneInterp <file.shy>\n"; return 1; }
    string filename=argv[1]; ifstream file(filename);
    if(!file){ cout<<"Error: Cannot open "<<filename<<endl; return 1; }
    vector<string> lines; string line; while(getline(file,line)) lines.push_back(line);
    run_lines(lines);
    return 0;
}
