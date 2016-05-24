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

void print_stack(stack<pair<string,int>> &mystack , string tags)
{	
	string tempstring;
	stack<pair<string,int>> temp;

	int i = 1;
	while(tags[i] != '}')
	{
		int j = i+1;
		string tag;
		
		while(tags[j] != '>')
			tag = tag + tags[j++];

		//cout << tag << endl;
		int pos = 0;
		while(isalpha(tag[pos++]));
		string att_tag = tag.substr(0,pos-1);
		int ind = stoi(tag.substr(pos-1,tag.length()));
		// cout << "<" << trim(att_tag) << att_hash[ind] << ">";
		temp.push(make_pair(trim(att_tag),ind));
		i = j+1;
	}
	while(true)
	{	
		//cout << mystack.size() << endl;
		if(mystack.empty())
		{	
			mystack = temp;
			while(!temp.empty())
			{	
				cout << "<" << trim(temp.top().first) << att_hash[temp.top().second] << ">";
				temp.pop();
			}
			break;
		}
		else
		{	
			stack<pair<string,int>> myss;
			myss = temp;
			while(!temp.empty())
			{	
				string tstring;
				tstring = temp.top().first;
				if(!mystack.empty())
				{
					if(trim(tstring).compare(trim(mystack.top().first)) == 0)
					{	
						//cout << "Equal tag is " << mystack.top() << endl;
						mystack.pop();
						temp.pop();
					}
					else
					{
						while(!mystack.empty())
						{
							cout << "</" << trim(mystack.top().first) << ">";
							mystack.pop();
						}
						while(!temp.empty())
						{
							cout << "<" << trim(temp.top().first) << att_hash[temp.top().second] << ">";
							temp.pop();
						}
					}
				}
				else
				{	
					
					while(!mystack.empty())
					{
						cout << "</" << trim(mystack.top().first) << ">";
						mystack.pop();
					}
					while(!temp.empty())
					{
						cout << "<" << trim(temp.top().first) << att_hash[temp.top().second] << ">";
						temp.pop();
					}
				}
			}
			mystack = myss;
			break;
		}
	}

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
	
	int l = s.length();
	//cout << l;
	stack<pair<string,int>> mystack;
	int i = 0;
	while(i<l)
	{	
		//cout << "value of i is " << i << endl;
		if((s[i] == '[' && i == 0) || (s[i] == '[' && s[i-1] != '\\'))
		{
			string tags="";
			int j = i+1;
			while(s[j] != ']')
				tags = tags + s[j++];
			if(tags.length() > 0)
			{	
				tags = trim(tags);
				//cout << "Tags are " << tags << endl;
				if(tags[0] == '{')
					print_stack(mystack,tags);
				else if(tags[0] == '<')
				{	
					int k = 1;
					int len = tags.length();
					while(k < len)
					{	
						string tag;
						while(tags[k] != '>')
							tag = tag + tags[k++];
						tag = trim(tag);
						if(tag[0] == '/')
						{	
							// cout << "first tag is " << mystack.top().first << endl;
							stack<string> tempstack;
							while(!mystack.empty())
							{	
								tempstack.push(mystack.top().first);
								mystack.pop();
							}
							while(!tempstack.empty())
							{
								cout << "</" << tempstack.top() << ">";
								tempstack.pop();
							}
							int le = tag.length();
							cout<< "</";
							for(int t = 1; t< le;t++)
								printf("%c",tag[t] );
							cout << ">";	
						}
						else
						{	
							int pos=0;
							while(isalpha(tag[pos++]));
							string att_tag = tag.substr(0,pos-1);
							// cout << "Inline tag is " << att_tag << endl;
							// if(!is_inline(att_tag))
							// {	
							int ind = stoi(tag.substr(pos-1,tag.length()));
								// cout << "stoi successful\n";
							cout << "<" << att_tag << att_hash[ind] << ">";
							// }
							// else
							// {	
							// 	// cout << "In inline" << endl;
							// 	print_stack(mystack,tag,2);
							// 	//cout << "Stack size is " << mystack.size() << endl;
							// }
						}
						while(tags[k++]!='<');
					}
				}
			}
			i = j + 1;
		}
		else
		{
			if(s[i] == '\\')
				i++;
			else
				cout << s[i++];
		}
	}
	cout << endl;
	return 0;
}