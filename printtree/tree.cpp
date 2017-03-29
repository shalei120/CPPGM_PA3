#include<iostream>
#include<vector>
#include<stack>
#include<string>
#include <cstdint>
#include<algorithm>
using namespace std;

class SyntaxTreeNode{
	
public:	
	int Code;
	int GenerationCode;
	intmax_t inh,syn;
	bool issigned, inh_issigned;
	string idword;

	SyntaxTreeNode* leftChild,*rightSibling;	

	SyntaxTreeNode(){
		Code = -1;
		GenerationCode = -1;
		inh = syn = -1;
		issigned = inh_issigned = true;
		idword = "";
		leftChild = rightSibling = NULL;
	}
		SyntaxTreeNode(int c){
		Code = c;
		GenerationCode = -1;
		inh = syn = -1;
		issigned = inh_issigned = true;
		idword = "";
		leftChild = rightSibling = NULL;
	}

	bool isleaf(){
		return leftChild == NULL;
	}
};

string long2str(intmax_t n, bool issigned){
	string res = "";
	if(!issigned){
		for(intmax_t a=n;a;a/=10){
			res += char(a % 10 + '0');
		}
		reverse(res.begin(),res.end());
		res += 'u';
	}else{
		if(n >= 0){
			for(intmax_t a=n;a;a/=10){
				res += char(a % 10 + '0');
			}
			reverse(res.begin(),res.end());
		}else{
			intmax_t n1 = ~n+1;
			for(intmax_t a=n1;a;a/=10){
				res += char(a % 10 + '0');
			}
			res += '-';
			reverse(res.begin(),res.end());
		}
		if(res == "") res = "0";
	}
	return res;
}	

vector<string> GetTreeString(SyntaxTreeNode* root){
	vector<string> res;
	if(root -> isleaf()){
		string s = long2str(root -> Code, true);
		res.push_back(s);
		return res;
	}else{ 
		SyntaxTreeNode* ptr = root -> leftChild;
		bool first = true;
		while(ptr != NULL){
			vector<string> vs = GetTreeString(ptr);
			for(uint i = 0;i < vs.size()+1;i++){
				string s;
				if(i == 0){
					if(first){
						s = long2str(root -> Code, true);
						first = false;
					}else s = "  "; 
					s += "|--";
					first = false;
				}
				else {
					s += "  | ";
				}
				if(i<vs.size()) s += vs[i];
				res.push_back(s);					
			}
			ptr = ptr -> rightSibling;
		}
	}
	return res;
}

int main(){
	SyntaxTreeNode* root = new SyntaxTreeNode(0);
	root -> leftChild =  new SyntaxTreeNode(1);

	root -> leftChild -> rightSibling =  new SyntaxTreeNode(2);
	root -> leftChild -> rightSibling -> rightSibling=  new SyntaxTreeNode(3);
	root -> leftChild -> leftChild =  new SyntaxTreeNode(4);
	root -> leftChild -> leftChild  -> rightSibling =  new SyntaxTreeNode(5);
	root -> leftChild -> rightSibling  -> leftChild =  new SyntaxTreeNode(6);

	vector<string> vs = GetTreeString(root);
	for(uint i = 0;i<vs.size();i++){
		cout<<vs[i]<<endl;
	}
}
