#include "storytokenizer.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

SectionToken::SectionToken(string text) {
	string commandComp;
	int x;

	secText = text;
	//this is where we determine secType
	if (text[0] == '[') {
		if (text[1] == '[') {
			secType = LINK;
		}
		else secType = BLOCK;
	}
	else if (text[0] == '(') {
		//switch command determining type
		istringstream ins(text);
		getline(ins, commandComp, ':');
		x = commandComp.size();
		switch (x) {
		case 3:
			secType = IF;
			break;
		case 4:
			secType = SET;
			break;
		case 5:
			secType = ELSE;
			break;
		case 6:
			secType = GOTO;
			break;
		case 8:
			secType = ELSEIF;
			break;
		}
	}
	else secType = TEXT;
}

string SectionToken::getText(){ //Return text from section
	return secText;
}

t_type SectionToken::getType(){ //Return type of token
	return secType;
}

PassageToken::PassageToken(string text){ 
	passageText = text;
	return;
}

string PassageToken::getText() { //Return passage text
	return passageText;
}

StoryTokenizer::StoryTokenizer(string story) { //Breaks story into Vector of Passage Tokens
	string buffer, intermediate;	
	string cutOff = "hidden>\n\n<tw-passagedata";
	string lastPass = "\n</tw-storydata";
	istringstream iss(story);
	int x;

	getline(iss, buffer, ' ');
	x = buffer.compare(cutOff);
	//Removes the header data
	while (x != 0) {
		getline(iss, buffer, ' ');
		x = buffer.compare(cutOff);
	}

	getline(iss, buffer, '>');
	getline(iss, intermediate, '<');
	x = buffer.compare(lastPass);
	
	//Moves passages to pt vector
	while (x != 0) {
		pt.emplace_back(intermediate);
		getline(iss, buffer, '>');
		getline(iss, buffer, '>');
		x = buffer.compare(lastPass);
		getline(iss, intermediate, '<');
		pMax++;
	}
}

bool StoryTokenizer::hasNextPassage() { //Check if next passage exists
	bool nextPassage = false;
	if (pMax >= (pIndex + 1)) {
		nextPassage = true;
	}
	return nextPassage;
}

PassageToken StoryTokenizer::nextPassage() { //Returns next passage
	pIndex++;
	return pt[pIndex - 1];
}

PassageTokenizer::PassageTokenizer(string Passage) { //Break passage into Section Tokens
	string tempPassage;
	int curPos, endIndex = 0, l, c; //curPos and endIndex are used to track progress through the passage and substr sections
	char startLink = '[', endLink = ']';
	char startCommand = '(', endCommand = ')';
	char temp; // temp is the current character value of the passage

	for (curPos = 0; curPos < Passage.size(); curPos++) {
		temp = Passage[curPos];
		//test if end of passage
		if (endIndex < 0) break;

		if (temp == startCommand) { //finds command sections
			endIndex = Passage.find(endCommand, curPos);
			tempPassage = Passage.substr(curPos, (endIndex - curPos + 1));
			st.emplace_back(tempPassage);

			curPos = endIndex;
			tempPassage.clear();
			sMax++;
		}

		if (temp == startLink) { //finds link sections
			if (temp == Passage[curPos + 1]) {
				endIndex = Passage.find(endLink, curPos);
				tempPassage = Passage.substr(curPos, ((endIndex + 2) - curPos));
				st.emplace_back(tempPassage);
				curPos = endIndex;
				tempPassage.clear();
				sMax++;
			}
			else { //finds block sections
				endIndex = Passage.find(endLink, curPos);
				tempPassage = Passage.substr(curPos, (endIndex - curPos + 1));
				st.emplace_back(tempPassage);
				curPos = endIndex;
				tempPassage.clear();
				sMax++;
			}
		}
		if (temp != startLink && temp != startCommand && temp != endLink && temp != endCommand) {
			l = Passage.find(startLink, curPos);
			c = Passage.find(startCommand, curPos);
			if (l > c) {
				endIndex = c;
			}
			if (l < c) {
				endIndex = l;
			}
			else endIndex = l;
			tempPassage = Passage.substr(curPos, (endIndex - curPos));
			st.emplace_back(tempPassage);

			curPos = endIndex - 1;
			tempPassage.clear();
			sMax++;
		}
	}
}

bool PassageTokenizer::hasNextSection() { //Check if next section exists in passage
	bool nextSection = false;
	if (sMax >= (sIndex + 1)) {
		nextSection = true;
	}
	return nextSection;
}

SectionToken PassageTokenizer::nextSection() { //Returns next section
	sIndex++;
	return st[sIndex - 1];
}