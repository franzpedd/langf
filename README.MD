# Multi-language text support
Sometime applications supports multiple languages, sometimes they don't. This library aims to help publishing stuff with multi-language support. Written in C++17 and only using the standart template library this library loads all translation uppon start-up and return's required text based on current set language.

## Build
Include [langf.h](https://github.com/franzpedd/langf/blob/main/langf.h) in your project and in exactly one source file add the following:
```cpp
#define LANGF_IMPLEMENTATION
#include "langf.h"
```
on any other source file just include [langf.h](https://github.com/franzpedd/langf/blob/main/langf.h) 

## Example
The Examples folder has the structure for the given example (as well as the example itself).
```cpp
#include <iostream>

#define LANGF_IMPLEMENTATION
#include "langf.h"

int main(int argc, char* argv[])
{
    // we first create the context, it ensure all resources will be ready for the translations
    langf::CreateContext("Language", ".txt");

    // testing portuguese
    {
        // need to decide witch active language we want to read from
        langf::SetLanguage("pt-br", langf::Alphabet::BrazilianPortuguese);

        // test static text-stuff
        printf(langf::Text("TEST", "TAG_HELLO_WORLD"));

        // test dynamic text-stuff
        int value = 5;
        const wchar_t* str = L"Olá mundo";
        printf(langf::Text("TEST", "TAG_WITH_ARGUMENTS"), value, str);
    }
    
    // testing english
    {
        // need to decide witch active language we want to read from
        langf::SetLanguage("en", langf::Alphabet::US_English);
        
        // test static text-stuff
        printf(langf::Text("TEST", "TAG_HELLO_WORLD"));
        
        // test dynamic text-stuff
        int value = 5;
        const wchar_t* str = L"Hello World";
        printf(langf::Text("TEST", "TAG_WITH_ARGUMENTS"), value, str);
    }

    // last, we should release any used resource
    langf::DestroyContext();

    return 0;
}
```

## License
[MIT](https://choosealicense.com/licenses/mit/)