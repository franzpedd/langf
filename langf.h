#ifndef LANGF_INCLUDED
#define LANGF_INCLUDED

/////////////////////////////////////////// includes ///////////////////////////////////////////
#include <string>
#include <vector>
#include "langf_translation.h"
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// platform detection ///////////////////////////////////////////
// windows platform
#if defined(_WIN64)
	#define PLATFORM_WINDOWS

// linux platform
#elif defined(__linux__) || defined(__gnu_linux__)
	#define PLATFORM_LINUX
	#if defined(__ANDROID__)
		#define PLATFORM_ANDROID
	#endif

// apple platform
#elif __APPLE__
	#define PLATFORM_APPLE
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR
		#define PLATFORM_IOS
		#define PLATFORM_IOS_SIMULATOR
	#elif TARGET_OS_IPHONE
		#define PLATFORM_IOS
	#else
		#error "Unknown Apple platform"
	#endif
#elif

// unix platform
#elif defined(__unix__)
	#define PLATFORM_UNIX

// posix platform
#elif defined(_POSIX_VERSION)
	#define PLATFORM_POSIX

// unknown platform
#else
	#error "Unknown platform!"
#endif
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// data structure ///////////////////////////////////////////
namespace langf
{
	// supported alphabets
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
}
//////////////////////////////////////////////////////////////////////////////////////

#endif // LANGF_INCLUDED

