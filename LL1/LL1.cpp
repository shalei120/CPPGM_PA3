#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
#include<string>
#include<sstream>
using namespace std;
map<string,int> nonterminal;//start from 100
map<string,int> terminal;   //start from 0
vector<string> code2terminal;
vector<string> code2nonterminal;
vector<vector<int> > generation,temp;
vector<set<int> > first(nonterminal.size()),follow(nonterminal.size());
vector<vector<int> > table;

vector<vector<int> > record;
string C2T(int code){
	return code2terminal[code];
}
string C2N(int code){
	return code2nonterminal[code-100];
}
string C2rule(int code){
	string res;
	for(int i=0;i<generation[code].size();i++){
		if(generation[code][i]<100){
			res+=C2T(generation[code][i]);
		}else{
			res+=C2N(generation[code][i]);
		}
		if(i==0)
			res+="->";
	}
	return res;
}
void addterminal(){
	terminal["integral-literal"]=0;
	terminal["OP_LPAREN"]=1;
	terminal["OP_RPAREN"]=2;
	terminal["defined"]=3;
	terminal["identifier_or_keyword"]=4;
	terminal["OP_PLUS"]=5;
	terminal["OP_MINUS"]=6;
	terminal["OP_LNOT"]=7;
	terminal["OP_COMPL"]=8;
	terminal["OP_STAR"]=9;
	terminal["OP_DIV"]=10;
	terminal["OP_MOD"]=11;
	terminal["OP_LSHIFT"]=12;
	terminal["OP_RSHIFT"]=13;
	terminal["OP_LT"]=14;
	terminal["OP_GT"]=15;
	terminal["OP_LE"]=16;
	terminal["OP_GE"]=17;
	terminal["OP_EQ"]=18;
	terminal["OP_NE"]=19;
	terminal["OP_AMP"]=20;
	terminal["OP_XOR"]=21;
	terminal["OP_BOR"]=22;
	terminal["OP_LAND"]=23;
	terminal["OP_LOR"]=24;
	terminal["OP_QMARK"]=25;
	terminal["OP_COLON"]=26;
	terminal["empty"]=27;
	terminal["#"]=28;
	code2terminal.push_back("integral-literal");
	code2terminal.push_back("OP_LPAREN");
	code2terminal.push_back("OP_RPAREN");
	code2terminal.push_back("defined");
	code2terminal.push_back("identifier_or_keyword");
	code2terminal.push_back("OP_PLUS");
	code2terminal.push_back("OP_MINUS");
	code2terminal.push_back("OP_LNOT");
	code2terminal.push_back("OP_COMPL");
	code2terminal.push_back("OP_STAR");
	code2terminal.push_back("OP_DIV");
	code2terminal.push_back("OP_MOD");
	code2terminal.push_back("OP_LSHIFT");
	code2terminal.push_back("OP_RSHIFT");
	code2terminal.push_back("OP_LT");
	code2terminal.push_back("OP_GT");
	code2terminal.push_back("OP_LE");
	code2terminal.push_back("OP_GE");
	code2terminal.push_back("OP_EQ");
	code2terminal.push_back("OP_NE");
	code2terminal.push_back("OP_AMP");
	code2terminal.push_back("OP_XOR");
	code2terminal.push_back("OP_BOR");
	code2terminal.push_back("OP_LAND");
	code2terminal.push_back("OP_LOR");
	code2terminal.push_back("OP_QMARK");
	code2terminal.push_back("OP_COLON");
	code2terminal.push_back("empty");
	code2terminal.push_back("#");
}

void RecordGeneration(char p[],string head){
	stringstream ss;
	vector<int> newv;
	string s(p);
	ss<<s;
	temp.push_back(newv);
	if(nonterminal.find(head)==nonterminal.end()){
		int code = nonterminal.size();
		nonterminal[head] = code + 100;
		code2nonterminal.push_back(head);
	}
	temp[temp.size()-1].push_back(nonterminal[head]);
	while(ss>>s){
		if(terminal.find(s)!=terminal.end()){
			temp[temp.size()-1].push_back(terminal[s]);
		}else if(nonterminal.find(s)!=nonterminal.end()){
			temp[temp.size()-1].push_back(nonterminal[s]);
		}else{
			int code = nonterminal.size();
			nonterminal[s] = code+100;
			code2nonterminal.push_back(s);
			temp[temp.size()-1].push_back(nonterminal[s]);
		}
	}
	

}

void printGene(ostream& os, vector<int> v){
	if(v.size()==0) {os<<"nothing to output"<<endl;return;}
	os<<C2N(v[0])<<" -> ";
	for(int j=1;j<v.size();j++){
		if(v[j]<100){
			os<<C2T(v[j])<<" ";
		}else{
			os<<C2N(v[j])<<" ";				
		}
	}cout<<endl;
}

void kill_left_recursion(){
	int i;
	for(i=0;i<temp.size();i++){
		if(temp[i][0]==temp[i][1]){
			break;
		}
	}
	if(i<temp.size()){
		vector<int> rule;
		rule = temp[0];
		string newNT = C2N(rule[1]);
		newNT+='\'';
		int code = nonterminal.size();
		nonterminal[newNT]=code+100;
		code2nonterminal.push_back(newNT);
		rule.push_back(nonterminal[newNT]);
		generation.push_back(rule);
		rule.clear();
		for(int i=0;i<temp.size();i++){
			rule.push_back(nonterminal[newNT]);
			if(i == 0){
				rule.push_back(terminal["empty"]);
			}else{
				for(int j=2;j<temp[i].size();j++){
					rule.push_back(temp[i][j]);
				}
				rule.push_back(nonterminal[newNT]);
			}
			generation.push_back(rule);	
			rule.clear();
		}
	}else if(temp[0][0] == nonterminal["controlling-expression"]){
		vector<int> rule;
		rule = temp[0];
		string newNT = C2N(rule[1]);
		newNT+='\'';
		int code = nonterminal.size();
		nonterminal[newNT]=code+100;
		code2nonterminal.push_back(newNT);
		rule.push_back(nonterminal[newNT]);
		generation.push_back(rule);
		rule.clear();
		for(int i=0;i<temp.size();i++){
			rule.push_back(nonterminal[newNT]);
			if(i == 0){
				rule.push_back(terminal["empty"]);
			}else{
				for(int j=2;j<temp[i].size();j++){
					rule.push_back(temp[i][j]);
				}				
			}
			generation.push_back(rule);	
			rule.clear();
		}
	}else if(temp[0][0] == nonterminal["primary-expression"]){
		for(int i = 0;i<5;i++){
				
			if(i==2){
				vector<int> rule;
				rule = temp[i];
				rule.pop_back();
				string newNT = C2T(rule[1]);
				newNT+='\'';
				int code = nonterminal.size();
				nonterminal[newNT]=code+100;
				code2nonterminal.push_back(newNT);
				rule.push_back(nonterminal[newNT]);
				record.push_back(rule);
				rule.clear();
				for(int j=0;j<2;j++){
					rule.push_back(nonterminal[newNT]);

					for(int k=2;k<temp[i+j].size();k++){
						rule.push_back(temp[i+j][k]);
					}	
					generation.push_back(rule);	
					rule.clear();
				}
				i++;
			}else{
				generation.push_back(temp[i]);
			}
		}
	}else{
		for(int i=0;i<temp.size();i++){
			generation.push_back(temp[i]);
		}

	}
}
void getFirst(){
	first.resize(nonterminal.size());
	int firstcount = 10,currcount = 0;
	while(1){
		currcount = 0;
		for(int i=0;i<first.size();i++){
			currcount += first[i].size();
		}
		if(firstcount == currcount)break;
		else firstcount = currcount;

		for(int i=0;i<generation.size();i++){
			if(generation[i].size() == 2 && generation[i][1]==terminal["empty"]){
				first[generation[i][0]-100].insert(terminal["empty"]);
				continue;
			}

			bool canempty = true;
			for(int j=1;j<generation[i].size();j++){
				if(generation[i][j]<100)
					first[generation[i][0]-100].insert(generation[i][j]);
				else{
					set<int> temp=first[generation[i][j]-100];
					temp.erase(terminal["empty"]);
					first[generation[i][0]-100].insert(temp.begin(),temp.end());
				}
				if(generation[i][j]<100 || first[generation[i][j]-100].find(terminal["empty"])==first[generation[i][j]-100].end()){
					canempty = false;
					break;
				}
			}
			if(canempty){
				first[generation[i][0]-100].insert(terminal["empty"]);
			}
		}
	}
}
void getFollow(){
	follow.resize(nonterminal.size());
	int followcount = 10,currcount = 0;
	follow[nonterminal["controlling-expression"]-100].insert(terminal["#"]);
	while(1){
		currcount = 0;
		for(int i=0;i<follow.size();i++){
			currcount += follow[i].size();
		}
		if(followcount == currcount)break;
		else followcount = currcount;

		for(int i=0;i<generation.size();i++){
			for(int j=1;j<generation[i].size()-1;j++){
				if(generation[i][j]>=100){
					for(int k=1;j+k < generation[i].size();k++){
						if(generation[i][j+k]<100){
							follow[generation[i][j]-100].insert(generation[i][j+k]);
						}else{
							set<int> temp = first[generation[i][j+k]-100];
							temp.erase(terminal["empty"]);
							follow[generation[i][j]-100].insert(temp.begin(),temp.end());
						}
						if(generation[i][j+k]<100 || first[generation[i][j+k]-100].find(terminal["empty"])==first[generation[i][j+k]-100].end()){
							break;
						}
					}
				}
			}
		}
		for(int i=0;i<generation.size();i++){
			for(int j=1;generation[i].size()-j>=1;j++){
				if(generation[i][generation[i].size()-j]<100){
					break;
				}
				follow[generation[i][generation[i].size()-j]-100].insert(follow[generation[i][0]-100].begin(),follow[generation[i][0]-100].end());
				if(first[generation[i][generation[i].size()-j]-100].find(terminal["empty"])==first[generation[i][generation[i].size()-j]-100].end()){
					break;
				}
			}
		}
	}
}
void getTable(){
	table.resize(nonterminal.size());
	for(int i=0;i<table.size();i++){
		table[i].resize(terminal.size());
	}
	for(int i=0;i<generation.size();i++){
		if(generation[i][1]<100){
			table[generation[i][0]-100][generation[i][1]]=i+1;
		}else{
			for(set<int>::iterator it=first[generation[i][1]-100].begin();it!=first[generation[i][1]-100].end();it++){
				if((*it)!=terminal["empty"]){
					table[generation[i][0]-100][*it]=i+1;						
				}
			}
		}
		bool canempty = true;
		for(int j=1;j<generation[i].size();j++){
			if(generation[i][j]<100){
				if(generation[i][j] == terminal["empty"]){
					break;
				}
				canempty = false;
				break;
			}else if(first[generation[i][j]-100].find(terminal["empty"])==first[generation[i][j]-100].end()){
				canempty = false;
				break;
			}
		}
		if(canempty){
			for(set<int>::iterator it=follow[generation[i][0]-100].begin();it!=follow[generation[i][0]-100].end();it++){
				table[generation[i][0]-100][*it]=i+1;		
			}
		}
	}
}
void printGeneration(ostream& os){
	os<<"string Generations[] = {"<<endl;
	for(int i=0;i<generation.size();i++){
		os<<"/* ["<<i<<"] */"<<"\t\""<<C2N(generation[i][0])<<" -> ";
		for(int j=1;j<generation[i].size();j++){
			if(generation[i][j]<100){
				os<<C2T(generation[i][j])<<" ";
			}else{
				os<<C2N(generation[i][j])<<" ";				
			}
		}
		os<<"\"";
		if(i!=generation.size()-1)os<<",";
		os<<endl;

	}
	os<<"};"<<endl;
}
void printTemp(ostream& os){
	for(int i=0;i<temp.size();i++){
		os<<C2N(temp[i][0])<<" --> ";
		for(int j=1;j<temp[i].size();j++){
			if(temp[i][j]<100){
				os<<C2T(temp[i][j])<<" ";
			}else{
				os<<C2N(temp[i][j])<<" ";				
			}
		}
		os<<endl;
	}
}
void printFirst(ostream& os){
	os<<"First.........................."<<endl;
	for(int i=0;i<first.size();i++){
		os<<C2N(i+100)<<": ";
		for(set<int>::iterator it=first[i].begin();it!=first[i].end();it++){
			os<<C2T(*it)<<' ';
		}
		os<<endl;
	}
}
void printFollow(ostream& os){
	os<<"Follow.........................."<<endl;
	for(int i=0;i<follow.size();i++){
		os<<C2N(i+100)<<": ";
		for(set<int>::iterator it=follow[i].begin();it!=follow[i].end();it++){
			os<<C2T(*it)<<' ';
		}
		os<<endl;
	}
}
void printTable(ostream& os){
	os<<"\n\nTable........."<<endl;
	os<<"\t";
	for(int i=0;i<terminal.size();i++){
		os<<C2T(i)<<'\t';
	}
	os<<endl;
	for(int i=0;i<nonterminal.size();i++){
		os<<C2N(i+100)<<'\t';
		for(int j=0;j<terminal.size();j++){
			if(table[i][j]>0)
				os<<C2rule(table[i][j]-1)<<'\t';
			else
				os<<'\t';
		}
		os<<endl;

	}
}
void printTableCode(ostream& os){
	os<<"\n\nconst int Table["<<nonterminal.size()<<"]["<<terminal.size()<<"] = {"<<endl;
	//os<<"\t";
	//for(int i=0;i<terminal.size();i++){
	//	os<<i<<'\t';
	//}
	//os<<endl;
	for(int i=0;i<nonterminal.size();i++){
		//os<<i+100<<'\t';
		os<<"\t{";
		for(int j=0;j<terminal.size();j++){
			if(table[i][j]>0)
				os<<table[i][j];
			else
				os<<"0";
			if(j!=terminal.size()-1) os<<",\t";
			else os<<"}";
		}
		if(i!=nonterminal.size()-1)os<<","<<endl;
		else os<<endl;

	}
	os<<"};"<<endl;
}

void printTerminal(ostream& os){
	os<<"const map<string, int> TerminaltoCodeMap = {"<<endl;
	for(int i=0;i<terminal.size();i++){
		os<<"\t{\""<<code2terminal[i]<<"\","<<i<<"}";
		if(i!=terminal.size()-1) os<<",";
		os<<endl;
	}
	os<<"};"<<endl;
}
void printNonterminal(ostream& os){
	os<<"const map<string, int> NonterminaltoCodeMap = {"<<endl;
	for(int i=0;i<nonterminal.size();i++){
		os<<"\t{\""<<code2nonterminal[i]<<"\","<<i+100<<"}";
		if(i!=nonterminal.size()-1) os<<",";
		os<<endl;
	}
	os<<"};"<<endl;
}
int main(){
	ifstream cin("aaa.txt");
	ofstream out("bbb.txt");
	string head;
	char p[128];
	addterminal();
	while(cin>>head){	
		head=head.substr(0,head.length()-1);	
		cin.getline(p,128);
		while(1){
			cin.getline(p,128);
			if(p[0]=='\r' || p[0]==0)
				break;
			RecordGeneration(p,head);			
		}
		//printNonterminal(cout);
		//printTemp(cout);
		kill_left_recursion();
		temp.clear();
	}
	for(int i=0;i<record.size();i++){
		generation.push_back(record[i]);
	}
	printTerminal(out);
	printNonterminal(out);
	printGeneration(out);
	getFirst();
	//printFirst(cout);
	getFollow();
	//printFollow(cout);
	getTable();
	//printTable(out);
	printTableCode(out);


}
