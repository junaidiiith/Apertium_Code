#include <iostream>
#include <string>
#include <stack>
#include <stdio.h>
#include <map>
using namespace std;

map<string, int> TAGS;

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

void print_stack(stack<string> &mystack , string tags,int flag)
{	
	string tempstring;
	stack<string> temp;
	if(flag == 2)
	{
		if(mystack.size()==0)
		{	
			cout << "<" << trim(tags) << ">";
			mystack.push(trim(tags));
		}
		else
		{	
			tempstring = mystack.top();
			if(trim(tempstring).compare(trim(tags)) != 0)
			{
				while(!mystack.empty())
				{
					cout << "</" << trim(mystack.top()) << ">";
					mystack.pop();
				}
				cout << "<" << trim(tags) << ">";
				mystack.push(trim(tags));
			}
		}
	}
	else if(flag == 1)
	{
		int i = 1;
		while(tags[i] != '}')
		{
			int j = i+1;
			string tag;
			while(tags[j] != '>')
			{
				tag = tag + tags[j++];
			}
			//cout << tag << endl;
			temp.push(trim(tag));
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
					cout << "<" << trim(temp.top()) << ">";
					temp.pop();
				}
				break;
			}
			else
			{	
				stack<string> myss;
				myss = temp;
				while(!temp.empty())
				{	
					string tstring;
					tstring = temp.top();
					if(!mystack.empty())
					{
						if(trim(tstring).compare(trim(mystack.top())) == 0)
						{	
							//cout << "Equal tag is " << mystack.top() << endl;
							mystack.pop();
							temp.pop();
						}
						else
						{
							while(!mystack.empty())
							{
								cout << "<" << trim(mystack.top()) << "/>";
								mystack.pop();
							}
							while(!temp.empty())
							{
								cout << "<" << trim(temp.top()) << ">";
								temp.pop();
							}
						}
					}
					else
					{	
						//cout << "mystack size is " << mystack.size() << endl;
						while(!mystack.empty())
						{
							cout << "</" << trim(mystack.top()) << ">";
							mystack.pop();
						}
						while(!temp.empty())
						{
							cout << "<" << trim(temp.top()) << ">";
							temp.pop();
						}
					}
				}
				mystack = myss;
				//cout << "size is" << mystack.size();
				break;
			}
		}
	}
}

int main(int argc, char **argv)
{	
	init_tags();
	string s="[<p>][<i>]^hello/hello<ij>$ [<i>]^brother/brother<n><sg>$ [<u>]^good/good<adv>/good<n><sg>/good<adj><sint>$[{<b><u>}] ^afternoon/afternoon<n><sg>$ [</p>]";
	//cin >> s;
	int l = s.length();
	//cout << l;
	stack<string> mystack;
	int i = 0;
	while(i<l)
	{	
		//cout << "value of i is " << i << endl;
		if((s[i] == '[' && i == 0) || (s[i] == '[' && s[i-1] != '\\'))
		{
			string tags;
			int j = i+1;
			while(s[j] != ']')
				tags = tags + s[j++];

			tags = trim(tags);
			//cout << "Tags are " << tags << endl;
			if(tags[0] == '{')
			{
				print_stack(mystack,tags,1);
			}
			else if(tags[0] == '<')
			{	
				int k = 1;
				string tag;
				while(tags[k] != '>')
					tag = tag + tags[k++];
				tag = trim(tag);
				if(tag[0] == '/')
				{	
					while(!mystack.empty())
					{
						cout << "</" << mystack.top() << ">";
						mystack.pop();
					}
					int le = tag.length();
					cout<< "</";
					for(int t = 1; t< le;t++)
						printf("%c",tag[t] );
					cout << ">";	
				}
				else
				{
					if(!is_inline(tag))
						cout << "<" << tag << ">";
					else
					{
						print_stack(mystack,tag,2);
						//cout << "Stack size is " << mystack.size() << endl;
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
