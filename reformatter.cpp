#include <iostream>
#include <string>
#include <stack>
#include <stdio.h>
#include <map>
#include <fstream>
#include <utility>
using namespace std;

map<int,string> att_hash;


string find_tag(string &str, int type)
{
	size_t position = str.find_first_of(",");
	int l = str.length();
	if(type == 2)
		return str.substr(0,position);
	else if(type == 1)
		return str.substr(position+1,l-position-1);
}

string trim(string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

void empty_stack(stack<int> &mystack)
{
	stack<int> temp;
	while(!mystack.empty())
	{
		temp.push(mystack.top());
		mystack.pop();
	}
	while(!temp.empty())
	{	
		string tag_part = find_tag(att_hash[temp.top()],2);
		cout << "</" << tag_part << ">";
		temp.pop();
	}
}

int main(int argc, char **argv)
{	
	if(argc != 2)
		return 1;
	ifstream myfile ("tag_attributes.txt");

	string line;
	while(getline(myfile,line))
	{
		// cout << line << endl;
		size_t found = line.find_first_of("=");
		int l = line.length();
		string str = line.substr(found+1,l-found-1);
		int num = stoi(line);

		att_hash[num] = str;
	}
	ifstream in(argv[1]);
	std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	
	int l = s.length();
	stack<int> mystack;
	int i = 0;
	while(i<l)
	{	
		if(((s[i] == '[' && i == 0) || (s[i] == '[' && s[i-1] != '\\')) && i+1 < l)
		{	
			empty_stack(mystack);
			if(s[i+1]=='{' && i+1 < l)
			{	
				int j = i+2;
				while(s[j] != '}')
				{	
					string tag;

					while(s[j]!='<')
						cout << s[j++];
					j++;
					while(s[j] != '>')
					{	
						//cout << s[j];
						tag = tag + s[j++];
					}
					j++;

					
/*					int pos = 0;
					int tlen = tag.length();
					while(isalpha(tag[pos++]));
					string att_tag = tag.substr(0,pos-1);
					int ind = stoi(tag.substr(pos-1,tlen-pos-1));
*/					int ind = stoi(tag);
					
					mystack.push(ind);
				}
				
				stack<int> temp = mystack;
				while(!temp.empty())
				{	
					string tag_and_attributes = find_tag(att_hash[temp.top()], 1);
					cout << "<" << tag_and_attributes << ">";
					temp.pop();
				}
				i = j+2;
			}
			else
			{	
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
						j++;
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
							/*int pos = 0;
							while(isalpha(tag[pos++]));
							string att_tag = tag.substr(0,pos-1);
							int ind = stoi(tag.substr(pos-1,tag.length()-pos-1));*/
							int ind = stoi(tag);
							string tag_and_attributes = find_tag(att_hash[ind],1);
							cout << "<" << tag_and_attributes << ">";
						}
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
