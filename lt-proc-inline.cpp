#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <fstream>
using namespace std;


int merge_buffer(string &buf, string &str, int start_index)
{ 
  int start = start_index-1;
  string temp = "";
  string blanks = "";
  bool prev_blank = false;
  while(start > 0)
  {
    if(isblank(str[start]))
    {
      blanks = str[start--]+blanks;
    }
    else if((isalnum(str[start])) || (str[start] == ']' && str[start-1] != '}'))
    {
      break;
    }
    else if(str[start]==']' && str[start-1] == '}')
    { 
      prev_blank = true;
      ;
      while(str[start]!='{')
      {
        temp = str[start] + temp;
        start--;
      }
      temp = "[{" + temp;
      start--;
      break;
    }
  }
  if(!prev_blank)
  { 
    start = start_index;
    temp = "";
  }
  int i = 0;
  buf = temp+buf;
  int l = buf.length();
 
  string buffer="";
  while(i < l-3)
  {
    if(buf[i]=='}' && buf[i+1]==']' && buf[i+2]=='[' && buf[i+3]=='{')
    {
      buffer += ',';
      i += 4;
    }
    else
      buffer += buf[i++];
  }
  while(i < l)
    buffer += buf[i++];
  
  buf = buffer + blanks;
  return start;
}
  
void InlineTagIssue(vector<string> &v)
{

  int l = str.length();
  int i = 0;


  string inline_tag = "( )?(\\[\\{[[:digit:]]+(,[[:digit:]]+)*\\}\\])*( )?";

  for (vector<string>::iterator vit = v.begin() ; vit != v.end(); ++vit)
  {
    string substring = *vit;
    int len = substring.length();
    string pattern = "";
    int i = 0;
    bool messed_up = false;
    while(i < len)
    {
      while(substring[i]!=' ' && i < len)
        pattern += substring[i++];
      i++;
      if(i < len)
      {
        messed_up = true;
        pattern += inline_tag;
      }
      else
        break;
    }
    if(messed_up)
    {  
      regex rx(pattern);
      for(auto it = std::sregex_iterator(str.begin(), str.end(), rx);
          it != std::sregex_iterator();
           ++it)
      { 
          string buf="";
          int start = it->position();
    
          int length = it->str().length();
          int end = start + length - 1;
          string sub = it->str();
          if(sub.compare(substring) == 0)
            continue;
          string correct = "";
          i=0;
          
          while(i < length)
          {
            if(sub[i]=='[')
            {
              while(sub[i]!=']' && i < l)
                buf += sub[i++];
              buf += ']';
              i++;
            }
            else
              correct += sub[i++];
          }
          start = merge_buffer(buf,str,start);
          correct = buf + correct;
          str = str.substr(0,start) + correct + str.substr(end+1,l-end-1);
      }
    }
  }
  cout << str << endl;
}

void remove_inline(FILE* f)
{
  FILE* temp_input = fopen("temp_input.txt","wb");
  string s;
  int c;
  while((c = std::fgetc(input)) != EOF)
    s += (char)c;

  int l = s.length();
  int i = 0;
  string inp="";
  while(i < l-1)
  {
    if(s[i]=='[' && s[i+1]=='{')
    {
      while(s[i]!=']')
        i++;
      i++;
    }
    else
      fputc(s[i++],temp_input);
  }
}

int main()
{
  FILE* input;
  input = fopen("input.txt","rb");
  FILE* temp_input;
  string s;
  int c;
  while((c = std::fgetc(input)) != EOF)
  {
    s += (char)c;
  }
  remove_inline(s);

  return 0;
}