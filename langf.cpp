#include "langf.h"

// standart
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <filesystem>

namespace langf
{
	static IO s_langIO;	// holds the information about the library

	void CreateContext(std::string path, const char* extension)
	{
		IO& io = GetContext();

		std::filesystem::path relativePath = std::filesystem::current_path();
		relativePath.append(path);

		if (!std::filesystem::is_directory(relativePath))
		{
			if (!std::filesystem::create_directory(relativePath)) {
				io.lastError = "Failed to create translation path\n";
				return;
			}
		}

		io.extension = extension;
		io.path = relativePath.string();

		// check supported languages
		for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(io.path)) {
			if (std::filesystem::is_directory(dirEntry.path())) {
				io.languages.push_back(dirEntry.path().filename().string());
			}
		}

		// read languages translations
		for (auto language : io.languages) {
		
			std::filesystem::path path(io.path);
			path.append(language);
		
			for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(path)) {
				if (std::filesystem::is_directory(dirEntry.path())) {
					continue;
				}
				
				// a translation file
				if (dirEntry.path().extension().string().compare(io.extension) == 0) {
					std::string filePath = dirEntry.path().string();
					std::string fileName = dirEntry.path().filename().replace_extension().string();
					
					std::string key(language);
					key.append(":");
					key.append(fileName);

					Translation translationUnit;
					Translation::Read(translationUnit, filePath);
					io.translations.insert({ key, translationUnit });
				}
			}
		}
	}
	
	void DestroyContext()
	{
		IO& io = GetContext();
		io.path.clear();
		io.languages.clear();
	}

	IO& GetContext()
	{
		return s_langIO;
	}

	void SetLanguage(const char* prefix, Alphabet alphabet)
	{
		IO& io = GetContext();

		for (size_t i = 0; i < io.languages.size(); i++) {
			if (io.languages[i].compare(prefix) == 0) {

				// change typing property based on requested locale
				setlocale(LC_CTYPE, AlphabetToString(alphabet));

				io.currentLanguage = prefix;
				return;
			}
		}

		io.lastError = "Desired language with prefix '";
		io.lastError.append(prefix);
		io.lastError.append("' wasn't implemented\n");
	}

	const char* Text(std::string file, std::string tag, bool newLine)
	{
		// read from correct file and correct id
		IO& io = GetContext();

		if (!io.currentLanguage) {
			io.lastError = "No language was set";
			return "";
		}

		std::string key(io.currentLanguage);
		key.append(":");
		key.append(file);

		if (io.translations.find(key) != io.translations.end()) {
			if (io.translations[key].Exists(tag)) {
				io.lastMessage = io.translations[key][tag].GetString();

				if (newLine) {
					io.lastMessage.append("\n");
				}

				return io.lastMessage.c_str();
			}
			
			io.lastError = "Requested tag '";
			io.lastError.append(tag);
			io.lastError.append("' doesn't exists");
			return "";
		}

		io.lastError = "Requested file '";
		io.lastError.append(file);
		io.lastError.append("' doesn't exists");
		return "";
	}

	const char* langf::AlphabetToString(Alphabet alphabet)
	{
		switch (alphabet)
		{
			case langf::US_English: return "en_US.UTF-8";
			case German: return "de_DE.UTF-8";
			case Spanish: return "es_ES.UTF-8";
			case French: return "fr_FR.UTF-8";
			case Italian: return "it_IT.UTF-8";
			case Japanese: return "ja_JP.UTF-8";
			case Korean: return "ko_KR.UTF-8";
			case BrazilianPortuguese: return "pt_BR.UTF-8";
			case ChineseSimplified: return "zh_CN.UTF-8";
			case ChineseTraditional: return "zh_TW.UTF-8";
		}

		IO& io = GetContext();
		io.lastError = "Requested alphabet '";
		io.lastError.append(std::to_string(alphabet));
		io.lastError.append("' is invalid");

		return "";
	}

	const char* GetLastError()
	{
		return GetContext().lastError.c_str();
	}
}