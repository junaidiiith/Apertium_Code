#Automatic Blank Handling coding challenges

1. Make the HTML format handler apertium-deshtml turn "<i>foo <b>bar</b></i>" into "[{<i>}]foo [{<i><b>}]bar"
In this coding challenge we had to basically turn the link each word with its associated inline tag.
I have used python to code it and make use of the xml.dom library to parse the html/xml
I have further implemented the code using stack.

To compile and run :
	 _python coding_challenge1.py_


2. Make apertium-deshtml *not* wrap tags like <p> or <div> in {} (ie. only for inline tags)
The 2nd challenge had to be done using language C++ as the entire apertium is built on C++.
It has the added functionality of not including the block tags in the [] brackets but only the inline tags.

To compile and run:
	_g++ libxml.cpp -I/usr/include/libxml2 -lxml2 -o output_

It has a prerequisites of libxml2


