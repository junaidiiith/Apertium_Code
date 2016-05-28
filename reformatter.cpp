#include <iostream>
#include <string>
#include <stack>
#include <stdio.h>
#include <map>
#include <fstream>
#include <utility>
using namespace std;

map<string, int> TAGS;
map<int,string> att_hash;

int is_inline(string tag) {
	if(TAGS.find(tag) != TAGS.end()) {
		return 1;
	}
	return 0;
}


void init_tags() {
	TAGS["em"] = 1;
	TAGS["u"] = 1;
	TAGS["i"] = 1;
	TAGS["b"] = 1;
	TAGS["span"] = 1;
	TAGS["a"] = 1;
	TAGS["abbr"] = 1;
	TAGS["bdo"] = 1;
	TAGS["big"] = 1;
	TAGS["cite"] = 1;
	TAGS["code"] = 1;
	TAGS["dfn"] = 1;
	TAGS["font"] = 1;
	TAGS["img"] = 1;
	TAGS["input"] = 1;
	TAGS["kbd"] = 1;
	TAGS["label"] = 1;
	TAGS["q"] = 1;
	TAGS["s"] = 1;
	TAGS["samp"] = 1;
	TAGS["select"] = 1;
	TAGS["small"] = 1;
	TAGS["strike"] = 1;
	TAGS["strong"] = 1;
	TAGS["sub"] = 1;
	TAGS["sup"] = 1;
	TAGS["textarea"] = 1;
	TAGS["tt"] = 1;
	TAGS["u"] = 1;
	TAGS["var"] = 1;
}

string trim(string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

int main(int argc, char **argv)
{	

	ifstream myfile ("tag_attributes.txt");

	string line;
	while(getline(myfile,line))
	{
		// cout << line << endl;
		size_t found = line.find_first_of("=");
		int l = line.length();
		string str = line.substr(found+1,l);
		int num = stoi(line);

		att_hash[num] = str;
	}

	
	ifstream in("deformatter_output.txt");
	std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	init_tags();
	//cin >> s;
	//cout << s;
	
	int l = s.length();
	//cout << l;
	stack<pair<string,int>> mystack;
	int i = 0;
	while(i<l)
	{	
		
		if(((s[i] == '[' && i == 0) || (s[i] == '[' && s[i-1] != '\\')) && i+1 < l)
		{
			if(s[i+1] == ']')
			{	
				while(!mystack.empty())
				{
					cout << "</" << mystack.top().first << ">";
					mystack.pop();
				}
			}
			else if(s[i+1]=='{' && i+1 < l)
			{	
				int j = i+2;
				while(s[j] != '}')
				{
					string tag;
					while(s[j]!='<')
						cout << s[j++];
						
					j++;
					while(s[j] != '>')
						tag = tag + s[j++];

					
					int pos = 0;
					while(isalpha(tag[pos++]));
					string att_tag = tag.substr(0,pos-1);
					
					int ind = stoi(tag.substr(pos-1,tag.length()));
					
					mystack.push(make_pair(trim(att_tag),ind));
					j++;
				}
				stack<pair<string,int>> temp;
				temp = mystack;
				while(!temp.empty())
				{
					cout << "<" << temp.top().first << att_hash[temp.top().second] << ">";
					temp.pop();
				}
				i = j+2;
			}
			else
			{	
			
				while(!mystack.empty())
				{
					cout << "</" << mystack.top().first << ">";
					mystack.pop();
				}
				int j = i+1;
				while(s[j] != ']' && j < l)
				{	
					if(s[j] == '<')
					{	
						string tag;
						j++;
						bool closing_tag = false;
						while(s[j] != '>')
							tag = tag + s[j++];
						int k = 0;
						int ltag = tag.length();
						while(k < ltag)
						{
							if(tag[k] == '/')
							{
								closing_tag = true;
								break;
							}
							k++;
						}
						if(closing_tag)
						{
							cout << "<" << tag.substr(k,ltag) << ">";	
						}
						else
						{	
							int pos = 0;
							while(isalpha(tag[pos++]));
							string att_tag = tag.substr(0,pos-1);
							int ind = stoi(tag.substr(pos-1,tag.length()));
							cout << "<" << trim(att_tag) << att_hash[ind] << ">";
						}
						j++;
						
					}
					else
						cout << s[j++];			
				}
				i = j+1;
			}
		}
		else
		{
			if(s[i] == '\\')
				i++;
			else
				cout << s[i++];
		}
	}
	return 0;
}