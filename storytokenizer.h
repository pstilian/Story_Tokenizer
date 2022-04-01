#ifndef __STORYTOKENIZER_H
#define	__STORYTOKENIZER_H

#include <string>
#include <vector>
using namespace std;

enum t_type { LINK, GOTO, SET, IF, ELSEIF, ELSE, BLOCK, TEXT};

class SectionToken {
private:
	string secText = "";
	t_type secType;
public:
	SectionToken(string text); //default constructor
	string getText();
	t_type getType();
};

class PassageToken {
public:
	PassageToken(string text); //default constructor
	string getText();
private:
	string passageText = "";
};

class StoryTokenizer {
public:
	StoryTokenizer(string story); //constructor breaks story into passage tokens
	bool hasNextPassage(); //is there another passage
	PassageToken nextPassage(); //returns next passage token object
private:
	vector<PassageToken> pt;
	int pIndex = 0;   //current passage
	int pMax = 0;     //number of passages + 1
};

class PassageTokenizer {
public:
	PassageTokenizer(string passage); //default constructor breaks passage into section tokens
	bool hasNextSection();
	SectionToken nextSection();
private:
	vector<SectionToken> st;
	int sIndex = 0;  //current section
	int sMax = 0;    //number of sections + 1
};

#endif