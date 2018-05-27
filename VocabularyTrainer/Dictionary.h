#pragma once

#include <string>
#include <map>
#include <fstream>

using std::string;

class IDict {
public:
	virtual void Add(const string&, const string&) = 0;
	virtual bool Find(const string&, string&) = 0;
	virtual bool IsEmpty() const = 0;
	virtual void PutNewWord(const string&, const string&) = 0;
	virtual void SaveToFile() = 0;
};

class Dict : public IDict {
private:
	enum readFileStatue { success, noFile, incorrectFile };
	std::map<string, string> map;
	string path;
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
public:

	Dict(const string& _path) : path(_path) {
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
	virtual void Add(const string&, const string&) {}
	virtual bool Find(const string&, string&) { return true; }
	virtual bool IsEmpty() const { return true; }
	virtual void PutNewWord(const string&, const string&) {}
	virtual void SaveToFile() {}
};

