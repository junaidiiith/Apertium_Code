#include <stack>
#include <vector>
#include <map>
#include <cstring> 
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iostream>

using namespace std;

// void printn(int n) {
// 	int A=0;
// 	printf("|");
// 	for(A=0;A<n;A++) {
// 		printf("-");
// 	}
// }

stack <string> vec;
map<string, int> TAGS;

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

void print_stack() {
	if(!vec.size())
		return;
	cout<<"[";
	if(vec.size() > 1) {
		cout<<"{";
	}
	for (std::stack<string> dump = vec; !dump.empty(); dump.pop())
		std::cout << "<"<<dump.top() << '>';
	if(vec.size() > 1) {
		cout<<"}";
	}
	cout<<"]";
}

void print_element_names(int n, xmlNode * a_node) {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			//printn(n);
			//printf("%s\n",cur_node->name);
			if (is_inline((char*)cur_node->name))
				vec.push((char*)cur_node->name);
			else
			{	
				xmlAttr *cur_attr = NULL;
    			xmlChar *attr;
    			printf("[<%s",cur_node->name);
    			for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) 
    			{

	       			printf(" %s = ", cur_attr->name);

	       			// This part fixed the code :D 
	       			attr =  xmlNodeGetContent((xmlNode*)cur_attr);

	      			printf("\'%s\'", attr);
	    		}
	    		printf(">]");
			}
			print_element_names(n+1, cur_node->children);
			if (is_inline((char*)cur_node->name))
				vec.pop();
			else
				printf("[</%s>]",cur_node->name);
		}
		else 
		{
			char* strng;
			strng = (char*)cur_node->content;
			char * pch;
			pch = strtok (strng," \t");
			while (pch != NULL)
			{  
				print_stack();
				printf ("%s ",pch);
				pch = strtok (NULL, " \t");
			}  

			//printf(">>%s(%d)<<",cur_node->content,n);
			//printf("%s",cur_node->content);
		}
	}
}

int main(int argc, char **argv)
{
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

	print_element_names(0,root_element);
	cout<<endl;

	xmlFreeDoc(doc);

	xmlCleanupParser();

	return 0;
}
