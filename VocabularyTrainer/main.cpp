#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Dictionary.h"
using std::string;

//template <class TKey, class TVal > class IDict {
//public:
//	virtual void Add(const TKey&, const TVal&) = 0;
//	virtual bool Find(const TKey&, TVal&);
//	virtual bool IsEmpty() const;
//};

//
//class ITrainingSet {
//public:
//	virtual Word GetNextWord() = 0;
//	virtual bool CheckAnswer() = 0;
//	virtual void PutNewWord(const string&, const string&) = 0;
//	virtual void LoadFromFile() = 0;
//	virtual void SaveToFile() = 0;
//};
//
//class Trainer {
//private:
//	ITrainingSet* trainingSet;
//public:
//	void Run();
//};
//
//class DictClient {
//private:
//	IDict* dictionary;
//public:
//	void Run();
//};
//
//class Program {
//private:
//	Trainer* trainer;
//	DictClient* dictClient;
//public:
//	void Run();
//};

class Word {
private:
public:
	Word() {}
	Word(const string& _word, const string& _trans) : word(_word), translation(_trans) {

	}
	string word;
	string translation;

	int key;
};

int main() {
	Dict dict(string("dictionary.txt"));
	return 0;
}
