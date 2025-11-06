#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <boost/variant.hpp>

using namespace std;

struct Var;
using ArrayType = vector<Var>;
using Var = boost::variant<double, string, ArrayType>;

unordered_map<string, Var> variables;

// --------- Utils ---------
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if(start==string::npos||end==string::npos) return "";
    return s.substr(start,end-start+1);
}

bool is_number(const string& s) {
    try { stod(s); return true; } catch(...) { return false; }
}

// --------- Print visitor ---------
struct PrintVisitor : public boost::static_visitor<> {
    void operator()(double d) const { cout << d; }
    void operator()(const string& s) const { cout << s; }
    void operator()(const ArrayType& arr) const {
        cout << "[";
        for(size_t i=0;i<arr.size();i++){
            boost::apply_visitor(PrintVisitor(), arr[i]);
            if(i!=arr.size()-1) cout<<", ";
        }
        cout << "]";
    }
};

// --------- Expression evaluation (handles &) ---------
Var eval_expression(const string& expr) {
    stringstream ss(expr);
    string token;
    Var result;
    bool first = true;

    while(getline(ss, token, '&')) {
        token = trim(token);
        Var val;
        if(variables.find(token)!=variables.end()) val = variables[token];
        else if(is_number(token)) val = stod(token);
        else if(!token.empty() && token.front()=='"' && token.back()=='"') val = token.substr(1,token.size()-2);
        else val = token;

        if(first) result = val;
        else {
            string a,b;
            if(string* sa = boost::get<string>(&result)) a=*sa;
            else if(double* da = boost::get<double>(&result)) a=to_string(*da);
            if(string* sb = boost::get<string>(&val)) b=*sb;
            else if(double* db = boost::get<double>(&val)) b=to_string(*db);
            result = a+b;
        }
        first=false;
    }
    return result;
}

// --------- Commands ---------
void handle_assignment(const string& line) {
    auto pos = line.find(':');
    if(pos==string::npos) return;
    string name = trim(line.substr(0,pos));
    string expr = trim(line.substr(pos+1));
    variables[name] = eval_expression(expr);
}

void handle_out(const string& line) {
    auto start=line.find('(');
    auto end=line.rfind(')');
    if(start==string::npos||end==string::npos||end<=start) return;
    string expr = trim(line.substr(start+1,end-start-1));
    Var val = eval_expression(expr);
    boost::apply_visitor(PrintVisitor(), val);
    cout << endl;
}

Var handle_in(const string& prompt) {
    if(!prompt.empty()) cout << prompt;
    string input; getline(cin,input);
    if(is_number(input)) return stod(input);
    return input;
}

// --------- Blocks ---------
vector<string> get_block(ifstream& file) {
    vector<string> block;
    string line;
    while(getline(file,line)) {
        string t = trim(line);
        if(t=="end") break;
        block.push_back(line);
    }
    return block;
}

void execute_line(const string& line, ifstream& file);

void handle_repeat(const string& line, ifstream& file) {
    string arg = trim(line.substr(6));
    int times=0;
    if(variables.find(arg)!=variables.end()) {
        if(double* d = boost::get<double>(&variables[arg])) times = static_cast<int>(*d);
    } else if(is_number(arg)) times = stoi(arg);

    vector<string> block = get_block(file);
    for(int i=0;i<times;i++)
        for(auto& l:block) execute_line(l,file);
}

// --------- Execute line ---------
void execute_line(const string& line, ifstream& file) {
    string t = trim(line);
    if(t.empty()||t[0]=='#') return;
    if(t.find("out(")==0) handle_out(t);
    else if(t.find("in(")==0) {
        auto start=t.find('(');
        auto end=t.rfind(')');
        string prompt="";
        if(start!=string::npos && end!=string::npos && end>start)
            prompt=trim(t.substr(start+1,end-start-1));
        Var val = handle_in(prompt);
        cout<<"Input received: ";
        boost::apply_visitor(PrintVisitor(), val);
        cout<<endl;
    }
    else if(t.find("repeat")==0) handle_repeat(t,file);
    else handle_assignment(t); // assignment via :
}

// --------- Run file ---------
void run_file(const string& filename) {
    ifstream file(filename);
    if(!file.is_open()){ cerr<<"Cannot open file: "<<filename<<endl; return;}
    string line;
    while(getline(file,line)) execute_line(line,file);
}

// --------- Main ---------
int main(int argc,char* argv[]){
    if(argc<2){ cerr<<"Usage: "<<argv[0]<<" <shyne_file.shy>"<<endl; return 1;}
    run_file(argv[1]);
    return 0;
}
