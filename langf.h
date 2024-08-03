#ifndef LANGF_INCLUDED
#define LANGF_INCLUDED

/////////////////////////////////////////// includes ///////////////////////////////////////////
#include <string>
#include <vector>
#include <unordered_map>
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// data structure ///////////////////////////////////////////
namespace langf
{
	// class responsible for opening and reading from files on disk
	class Translation
	{
	public: // functions

		// reads data from a file
		static bool Read(Translation& dataFile, const std::string& path, char separator = ',');

	public: // operator overloading

		inline Translation& operator[](const std::string& name)
		{
			// node map already does not contains an object with this name
			if (mObjectMap.count(name) == 0)
			{
				// create the object in the map and create a new empty DataFile on the object vector
				mObjectMap[name] = mObjectVec.size();
				mObjectVec.push_back({ name, Translation() });
			}

			// returns the object by it's map index
			return mObjectVec[mObjectMap[name]].second;
		}

		// trying to access a children node
		inline Translation& operator[](const size_t& index)
		{
			if (index > mObjectVec.size()) {
				std::abort();
			}

			return mObjectVec[index].second;
		}

	public: // utils

		// returns if either a property of this node exists or not
		bool Exists(std::string property) const;

		// returns a string value of a property
		const std::string GetString(const size_t count = 0) const;

		// sets a string value for the property
		void SetString(const std::string& str, const size_t count);

	protected:

		bool mIsComment = false; // used to identify if the property is a comment or not

	private:

		std::vector<std::string> mContent; // the items of this serializer
		std::vector<std::pair<std::string, Translation>> mObjectVec; // child nodes of this datafile
		std::unordered_map<std::string, size_t>  mObjectMap;
	};

	// supported alphabets by https://www.ibm.com/docs/en/tsafm/4.1.0?topic=installation-supported-languages-locales
	typedef enum Alphabet
	{
		US_English = 0,
		German,
		Spanish,
		French,
		Italian,
		Japanese,
		Korean,
		BrazilianPortuguese,
		ChineseSimplified,
		ChineseTraditional
	} Alphabet;

	// main structure, holds all information about the languages system
	typedef struct IO
	{
		std::string path = {};
		const char* extension = nullptr;
		const char* currentLanguage = nullptr;
		std::vector<std::string> languages = {};
		std::unordered_map<std::string, Translation> translations = {};
		std::string lastMessage = {};
		std::string lastError = {};
	} IO;
}
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// core functions ///////////////////////////////////////////

namespace langf
{
	/* Creates the context for the lang library */
	void CreateContext(std::string path, const char* extension = ".txt");

	/* Free used resources by the lang library */
	void DestroyContext();

	/* returns the main context for the lang library */
	IO& GetContext();

	/* Sets the language used for showing text */
	void SetLanguage(const char* prefix, Alphabet alphabet);

	/* Returns the correct translated text based on current language and requested id */
	const char* Text(std::string file, std::string tag, bool newLine = true);
}

//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// util functions ///////////////////////////////////////////
namespace langf
{
	// converts the utf-8 string used by locale based on desired alphabet
	const char* AlphabetToString(Alphabet alphabet);

	// returns the last error that has occured
	const char* GetLastError();

	// removes white spaces from a string
	void RemoveWhiteSpaces(std::string& str);
}
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// header-only ///////////////////////////////////////////

#ifdef LANGF_IMPLEMENTATION
#undef LANGF_IMPLEMENTATION
// Prevent tools like dependency checkers from detecting a cyclic dependency
#define LANGF_SOURCE "langf.cpp"
#include LANGF_SOURCE
#endif

//////////////////////////////////////////////////////////////////////////////////////

#endif // LANGF_INCLUDED

