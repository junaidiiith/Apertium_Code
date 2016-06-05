#include <stack>
#include <vector>
#include <map>
#include <cstring> 
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iostream>
#include <fstream>

using namespace std;

int indices = 1;

stack <xmlNode*> vec;
map<string, int> TAGS;
bool in_inline = false;
bool in_superblank = false;

int is_inline(char *tag) {
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

bool is_blank(char ch)
{
	if(ch == ' ' || ch == '\n' || ch == '\t')
		return true;
	else
		return false;
}

void print_stack(ostream& attributes, ostream& outfile) {
	if(!vec.size())
		return;
	outfile<<"[{";
	
	for (std::stack<xmlNode*> dump = vec; !dump.empty(); dump.pop())
	{	
		xmlAttr *cur_attr = NULL;
    	xmlChar *attr;
    	xmlNode *cur_node = dump.top();
		outfile << "<" << indices;
		//outfile << "Node is " << cur_node->name << "," << cur_node->name;
		attributes << indices << "=" << cur_node->name << "," << cur_node->name;
    	for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) 
		{

   			//printf(" %s = ", cur_attr->name);
   			attributes << " " << cur_attr->name << " = ";

   			// This part fixed the code :D 
   			attr =  xmlNodeGetContent((xmlNode*)cur_attr);

  			//printf("\'%s\'", attr);
  			attributes << "\"" << attr << "\"";
		}
		outfile << ">";
		attributes << "\n";
		indices++;
	}
	outfile << "}]";
}

void print_element_names(int n, xmlNode * a_node, ostream& attributes,ostream& outfile, xmlNode* parent) {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			if (is_inline((char*)cur_node->name))
			{	
				if(in_superblank)
				{
					outfile << "]";
					in_superblank = false;
				}
				vec.push((xmlNode*)cur_node);
				// in_inline = true;
				in_superblank = false;
			}
			else
			{	
				if(!in_superblank)
				{
					outfile << "[";
					in_superblank = true;
				}
				xmlAttr *cur_attr = NULL;
    			xmlChar *attr;
    			// printf("[<%s%d",cur_node->name,indices);
    			outfile << "<" << indices;
    			attributes << indices << "=" << cur_node->name << "," << cur_node->name;

    			for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) 
    			{

	       			//printf(" %s = ", cur_attr->name);
	       			attributes << " " << cur_attr->name << " = ";

	       			// This part fixed the code :D 
	       			attr =  xmlNodeGetContent((xmlNode*)cur_attr);

	      			//printf("\'%s\'", attr);
	      			attributes << "\"" << attr << "\"";
	    		}
	    		attributes << "\n";
	    		// outfile << ">]";
	    		outfile << ">";
	    		indices++;
			}
			print_element_names(n+1, cur_node->children, attributes,outfile, cur_node);
			if (is_inline((char*)cur_node->name))
			{	
				outfile << "[]";
				vec.pop();

			}
			else
			{	
				outfile << "[<\\/" << cur_node->name << ">]";
			}
		}
		else 
		{	

			char* strng;
			strng = (char*)cur_node->content;
			// if(in_superblank && !strng)
			// 	outfile << "]";
			int l = strlen(strng);

			int num = 0;
			while(is_blank(strng[num]))
				num++;
			//outfile << num;
			if(num < l)
			{	
				if(in_superblank)
				{
					in_superblank = false;
					outfile << "]";
				}
				int k = 0;
				print_stack(attributes,outfile);

				string start_blank="";
				while(is_blank(strng[k]) && k < l)
					start_blank = strng[k++];

				if(start_blank.length())
					outfile << "[" << start_blank << "]";
				int start_index = k;

				k = l-1;
				while(k>=0 && is_blank(strng[k]))
					k--;
				string end_blank ="";
				int end_index = k;
				k++;
				while(k<l)
					end_blank += strng[k++];

				k = start_index;
				while(k+1 <= end_index)
				{	
					
					if(is_blank(strng[k]) && is_blank(strng[k+1]))
					{	
						string buf="";
						int t = k;
						while(t < l)
						{	if(is_blank(strng[t]))
							{
								buf += strng[t];
								t++;
							}
							else
								break;
						}
				
						outfile << "[" << buf << "]";
						k = t;
					}
					else
					{
						outfile << strng[k];
						k++;
					}
				}
				if(k == end_index)
					outfile << strng[k];
				if(end_blank.length())
					outfile << "[" << end_blank << "]";
			}
			else
			{	
				if(!in_superblank)
					outfile << "[" << strng << "]";
				else
					outfile << strng;
			}
		}
	}
}



void merge_blocks(string s )
{	
	ofstream outputfile ("deformatter_output.txt");
	int l = s.length();
	int i = 1;
	string ans="";
	ans += s[0];
	// cout << s << endl << endl;
	//outputfile << s << endl << endl;
	bool block;

	while(i<l)
	{	
		if(i+2 > l)
			break;
		if((s[i]==']' && s[i+1]=='[' && s[i+2]!='{' && s[i-1]!='}'))
			i+=2;
		ans += s[i];
		i++;
	}

	// cout << ans << endl << endl;
	//outputfile << ans << endl << endl;

	i = 0;
	string ans1 = "";
	l = ans.length();
	while(i < l)
	{
		if(ans[i] == '[' && ans[i+1] == ']')
		{	
			string buf = "[]";
			int j = i+2;
			int non_super_found = 1;
			while(true)
			{
				if(ans[j] != '[')
				{
					non_super_found = 0;
					break;
				}
				else	
					break;
			
				j++;
			}
			if(!non_super_found)
				ans1 += buf;
			i = j;
		}
		else
		{
			ans1 += ans[i++];
		}
	}

	
	
	cout << ans1 << endl;
	outputfile << ans1 << endl;
}

int main(int argc, char **argv)
{	
	ofstream attributes ("tag_attributes.txt");
	if(!attributes.is_open())
	{
		cout << "Unable to open attributes file\n";
		return 0;
	}
	ofstream outfile ("temp.txt");
	
	init_tags();
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	if (argc != 2)
		return(1);

	LIBXML_TEST_VERSION

	doc = xmlReadFile(argv[1], NULL, 0);

	if (doc == NULL) {
		printf("error: could not parse file %s\n", argv[1]);
	}

	root_element = xmlDocGetRootElement(doc);

	print_element_names(0,root_element,attributes,outfile, NULL);
	outfile<<endl;

	xmlFreeDoc(doc);

	xmlCleanupParser();

	ifstream in("temp.txt");
	std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	merge_blocks(s);
	// outfile << s;

	return 0;
}
