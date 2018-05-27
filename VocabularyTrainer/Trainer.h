#pragma once

#include "PQueue.h"
#include "Word.h"
#include <iostream>

#define REPEATS_NUMBER 5

class Trainer {
private:
	PQueue<Word*> wordQueue;
	std::pair<Word*, double> currentWord;

	double GetPriority(std::pair<Word*, double> word) {
		double f = (double)word.first->failure;
		double s = (double)word.first->success;
		double newPriority = f;
		newPriority += (s > REPEATS_NUMBER) ? 0.0 : (double)REPEATS_NUMBER - s;
		newPriority /= (s + 1.0);
		return (newPriority * 2.0) / 3.0 + (double)word.second / 3.0;
	}
	void WrongAnswer(std::pair<Word*, double> word) {
		++(word.first->failure);
		wordQueue.Push(word.first, GetPriority(word));
	}
	void GoodAnswer(std::pair<Word*, double> word) {
		++(word.first->success);
		wordQueue.Push(word.first, GetPriority(word));
	}

	bool StrCmp(const char* str1, const char* str2) {
		int i = 0;
		int j = 0;
		while (true) {
			if (str1[i] != str2[j]) {
				return false;
			}
			if (str1[i] == '\0') {
				return true;
			}
			i++;
			j++;
		}
	}
public:
	Trainer() { }
	bool Load(std::istream& inp) {
		while (!inp.eof()) {
			char* word = new char[255];
			char* translation = new char[255];
			int success;
			int failure;
			double priority;

			inp >> word >> translation >> success >> failure >> priority;

			Word* newWord = new Word(word, translation);
			newWord->success = success;
			newWord->failure = failure;
			
			wordQueue.Push(newWord, priority);
		}
		return !wordQueue.IsEmpty();
	}
	Word* GetWord() {
		if (wordQueue.IsEmpty()) {
			return nullptr;
		}
		currentWord = wordQueue.Pop();
		return currentWord.first;
	}
	bool CheckAnswer(const char* answer) {
		bool result = StrCmp(answer, currentWord.first->translation);
		if (result) {
			GoodAnswer(currentWord);
		}
		else {
			WrongAnswer(currentWord);
		}
		return result;
	}
};