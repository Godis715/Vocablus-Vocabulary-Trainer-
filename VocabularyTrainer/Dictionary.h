#pragma once

#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <conio.h>

using std::string;
typedef void(*OutToFile)(const std::pair<string, string>&);

enum Item { _default, _incorrect, _help, _delete, _goOut, _save };

Item GetItem(const string& str) {
	if (str == string("#default")) return _default;
	if (str == string("#help")) return _help;
	if (str == string("#delete")) return _delete;
	if (str == string("#exit")) return _goOut;
	if (str == string("#save")) return _save;

	return _incorrect;
}

class IDict {
public:
	virtual void Add(const string&, const string&) = 0;
	virtual bool Find(const string&, string&) = 0;
	virtual void Delete(const string&) = 0;
	virtual bool IsEmpty() const = 0;
	virtual void SaveToFile(const string&) const = 0;
};

class Dict : public IDict {
private:
	enum readFileStatue { success, noFile, incorrectFile };
	std::map<string, string> map;
	readFileStatue LoadFromFile(const string& _path) {
		std::ifstream inp(_path, std::ios::in);
		if (!inp.is_open()) {
			/*...*/
			return noFile;
		}

		const int buffSize = 16;
		char buff[buffSize];
		int buffIter = 0;
		int currentBufSize = 0;

		bool isWord = true;
		string word;
		char sentence[256];
		int sentIter = 0;

		bool isReading = false;

		while (!inp.eof() || buffIter < currentBufSize) {
			if (buffIter == currentBufSize) {
				inp.read(buff, buffSize);
				currentBufSize = inp.gcount();
				buffIter = 0;
			}
			
			if (!isReading && (buff[buffIter] != '<')) {
				return incorrectFile;
			}
			else if(!isReading) {
				++buffIter;
			}
			isReading = true;
			for (buffIter; buffIter < currentBufSize; ++buffIter) {
				if (buff[buffIter] == '>') {
					if (!isWord) {
						++buffIter;
					}
					isReading = false;
					++buffIter;
					break;
				}
				if (buff[buffIter] == '<') {
					return incorrectFile;
				}

				sentence[sentIter] = buff[buffIter];
				sentIter++;
			}

			if (!isReading) {
				string str(sentence, sentIter);
				if (isWord) {
					word = str;
				}
				else {
					map.insert(std::make_pair(word, str));
				}
				isWord = !isWord;
				sentIter = 0;
			}
		}
		inp.close();
		return success;
	}
	auto GetOutToFileFunc(std::ostream& out) const  {
		auto _out = &out;
		return (([_out](const std::pair<string, string>& _pair) {
			*_out << "<" << _pair.first << "><" << _pair.second << ">\n"; }));
	}
public:

	Dict(const string& _path) {
		switch (LoadFromFile(_path)) {
		case incorrectFile: {
			/*throw*/
			break;
		}
		case noFile: {
			/*create*/
			break;
		}
		}
	}
	void Add(const string& word, const string& translation) {
		map.insert(make_pair(word, translation));
	}
	bool Find(const string& word, string& translation) {
		auto elem = map.find(word);
		if (elem != map.end()) {
			translation = (*elem).second;
			return true;
		}
		else {
			return false;
		}
	}

	void Delete(const string& word) {
		map.erase(word);
	}

	bool IsEmpty() const { 
		return (map.size() == 0);
	}
	void SaveToFile(const string& _path) const {
		std::ofstream out(_path);

		std::for_each(map.begin(), map.end(), GetOutToFileFunc(out));

		out.close();
	}
};

class DictClient {
private:
	IDict* dictionary;
	std::ostream* out;
	std::istream* inp;
	string path;

	bool UserAgreement() {
		while (true) {
			char c;
			c = _getch();
			if (c == 27) {
				return false;
			}
			else if (c == 13) {
				return true;
			}
		}
	}

public:
	DictClient(const string& _path, std::istream* _inp, std::ostream* _out) : dictionary(new Dict(_path)) {
		inp = _inp;
		out = _out;
		path = _path;
	}
	void Run() {

		enum state { enterWord, enterTrans, save};

		state currentState = enterWord;

		string currentWord;
		string currentTranslation;
		

		while (true) {

			system("cls");

			(*out) << "selected word: "
				<< ((!currentWord.empty()) ? currentWord : string("???"))
				<< " - "
				<< ((!currentTranslation.empty()) ? currentTranslation : string("???")) << "\n";

			if (currentState == enterWord) {
				(*out) << "type: ";
			}

			if (currentState == enterTrans) {
				(*out) << "enter translation\n";
			}

			string line;
			std::getline(*inp, line);

			if (line.size() == 0) {
				continue;
			}

			if (line[0] == '#') {
				Item item = GetItem(line);
				if (item == _delete && !currentWord.empty()) {
					(*out) << "delete?\nYes/No (enter/ESC)\n";
					if (UserAgreement()) {
						dictionary->Delete(currentWord);
						currentWord.clear();
						currentTranslation.clear();
					}
					continue;
				}
				if (item == _save) {
					dictionary->SaveToFile(path);
					continue;
				}
				if (item == _goOut) {
					(*out) << "exit?\nYes/No (enter/ESC)\n";
					if (UserAgreement()) {
						(*out) << "save changes?\nYes/No (enter/ESC)\n";
						if (UserAgreement()) {
							dictionary->SaveToFile(path);
						}
						return;
					}
					continue;
				}
				//
				continue;
			}

			if (currentState == enterWord) {
				string trans;
				bool isContains = dictionary->Find(line, trans);
				if (!isContains) {
					(*out) << "unknown word. Enter translation\nYes/No (enter/ESC)\n";
					if (UserAgreement()) {
						currentWord = line;
						currentTranslation.clear();
						currentState = enterTrans;
					}
				}
				else {
					(*out) << trans << "\n";
					currentWord = line;
					currentTranslation = trans;
				}
				continue;
			}

			if (currentState == enterTrans) {
				currentTranslation = line;
				(*out) << "save word?\nYes/No (enter/ESC)\n";
				if (UserAgreement()) {
					dictionary->Add(currentWord, currentTranslation);
					currentState = enterWord;
					continue;
				}
			}
		}
	}
};

