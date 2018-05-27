#pragma once

class Word {
private:
public:

	Word(const char* _word, const char* _translation) : word(_word), translation(_translation) {
		success = 0;
		failure = 0;
	}

	const char* word;
	const char* translation;

	int success;
	int failure;
};