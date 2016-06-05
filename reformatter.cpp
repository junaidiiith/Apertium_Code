#include <iostream>
#include <string>
#include <stack>
#include <stdio.h>
#include <map>
#include <fstream>
#include <utility>
#include <sqlite3.h>
#include <stdlib.h>
#include <sstream>
using namespace std;

map<int,string> opening_tags;
map<int, string> closing_tags;

void make_maps(string filename)
{	
	sqlite3 *db;
	sqlite3_open(filename.c_str(), & db);
	string selectQuery = "SELECT * FROM TAGS_DATA;";
    sqlite3_stmt *selectStmt;
  
  	if(sqlite3_prepare_v2(db, "select * from TAGS_DATA;", -1, &selectStmt, 0) != SQLITE_OK) cout << "Didn't Select Item!" << endl;
  	else
  	{
    	int columns = sqlite3_column_count(selectStmt);
     	int result = 0;
     	while(true)
     	{
        	result = sqlite3_step(selectStmt);
        	if(result == SQLITE_ROW)
        	{
           		
          		stringstream s;
          		s << sqlite3_column_text(selectStmt, 0);
          		int id = stoi(s.str());

          		stringstream o_tag;
          		string opening_tag;
          		o_tag << sqlite3_column_text(selectStmt, 1);
          		opening_tag = o_tag.str();
					
				stringstream c_tag;
          		string closing_tag;
          		c_tag << sqlite3_column_text(selectStmt, 2);
          		closing_tag = c_tag.str();

          		opening_tags[id] = opening_tag;
          		closing_tags[id] = closing_tag;

        	}
        	else
           		break;
     	}
    	sqlite3_finalize(selectStmt);
    }	
}

bool is_blank(char ch)
{
	if(ch == ' ' || ch == '\n' || ch == '\t')
		return true;
	else
		return false;
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
		//cout << "Closing tag with id ="<< temp.top() << endl;
		cout << closing_tags[temp.top()];
		temp.pop();
	}
}


int main(int argc, char **argv)
{	

	make_maps("tags_data.db");

	if(argc!=2)
	{	
		cout << "Insufficient arguments\n";
		return 1;
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
					while(s[j]!=',' && s[j]!='}')
						tag += s[j++];
					//cout << "Tag is" << tag << "this\n";
					
					int ind = stoi(tag);

					mystack.push(ind);
					
					if(s[j] == '}')
						break;
					else
						j++;
				}
				
				stack<int> temp = mystack;
				while(!temp.empty())
				{	
					cout << opening_tags[temp.top()];
					temp.pop();
				}
				i = j+2;
			}
			else
			{	
				int j = i+1;
				string tag;
				bool blank_super = false;
				while(s[j] != ']' && j < l)
				{	
					if(is_blank(s[j]))
						blank_super = true;
					tag += s[j++];
				}
				if(blank_super)
					cout << tag;
				else
				{	
					int ind = stoi(tag);
					cout << opening_tags[ind];
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

	cout << endl;
	return 0;
}
