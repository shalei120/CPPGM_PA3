#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
using namespace std;
string long2str(int n, bool issigned, int maxwidth = -1){
		string res = "";
		if(!issigned){
			for(int a=n;a;a/=10){
				res += char(a % 10 + '0');
			}
			reverse(res.begin(),res.end());
			if(res == "") res = "0";
			res += 'u';
		}else{
			if(n >= 0){
				for(int a=n;a;a/=10){
					res += char(a % 10 + '0');
				}
				reverse(res.begin(),res.end());
			}else{
				int n1 = ~n+1;
				for(int a=n1;a;a/=10){
					res += char(a % 10 + '0');
				}
				res += '-';
				reverse(res.begin(),res.end());
			}
			if(res == "") res = "0";
		}
		if(maxwidth > 0){
			while(res.length() < (unsigned int)maxwidth) res += ' ';
		}
		return res;
	}	
int main(){
	ifstream cin("300-triple.t");
	int c=0,co=0;
	string s;
	string name = "300-triple_"+long2str(c,1)+".t";
	ofstream out(name.c_str());
	while(getline(cin,s)){
		out<<s<<endl;
		co++;
		if(co % 6000 == 0){
			c++;
			name = "300-triple_"+long2str(c,1)+".t";
			out.close();
			out.open(name.c_str());
		}
	}
}
