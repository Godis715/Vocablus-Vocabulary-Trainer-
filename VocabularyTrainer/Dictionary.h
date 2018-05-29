#pragma once

#include <string>
#include <map>
#include <fstream>
#include <algorithm>

using std::string;
typedef void(*OutToFile)(const std::pair<string, string>&);

enum Item { _default, _incorrect, _help };

Item GetItem(const string& str) {
	if (str == string("#default")) return _default;
	if (str == string("#help")) return _help;
	return _incorrect;
}

class IDict {
public:
	virtual void Add(const string&, const string&) = 0;
	virtual bool Find(const string&, string&) = 0;
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
			*_out << "<" << _pair.first << "><" << _pair.second << ">"; }));
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
	bool isHelp;
	void ShowHelp() {
		*out << "1 - add the word\n";
		*out << "2 - delete the word\n";
		*out << "3 - find the word\n";
	}
public:
	DictClient(const string& _path) : dictionary(new Dict(_path)) {
		isHelp = true;
	}
	void Run() {
		while (true) {

			ShowHelp();
			string word;
			*inp >> word;

		}
	}
};

