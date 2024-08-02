#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <stack>

namespace langf
{
	class Translation
	{
	public: // functions

		// reads data from a file
		static bool Read(Translation& dataFile, const std::string& path, char separator = ',')
		{
			std::ifstream file(path);
			if (file.is_open())
			{
				// variables may be outside the loop and we may need to refer to previous iterations
				std::string propName = {};
				std::string propValue = {};

				// using a stack to handle the reading
				// may re-factor this later
				std::stack<std::reference_wrapper<Translation>> dfStack;
				dfStack.push(dataFile);

				while (!file.eof())
				{
					std::string line;
					std::getline(file, line);

					RemoveWhiteSpaces(line);

					// line is not empty
					if (line.empty())
						continue;

					// test if it's a comment
					if (line[0] == '#')
					{
						Translation comment;
						comment.mIsComment = true;
						dfStack.top().get().mObjectVec.push_back({ line, comment });

						continue;
					}

					// check if equals symbol exists, if it does it is a property
					size_t x = line.find_first_of('=');
					if (x != std::string::npos)
					{
						propName = line.substr(0, x);
						RemoveWhiteSpaces(propName);

						propValue = line.substr(x + 1, line.size());
						RemoveWhiteSpaces(propValue);

						// elements may contain quotes and separations, must deal with this particularity
						bool inQuotes = false;
						std::string token = {};
						size_t tokenCount = 0;

						for (const auto c : propValue)
						{
							if (c == '\"')
							{
								inQuotes = true;
							}

							else
							{
								// it's in quotes, appends to a string
								if (inQuotes)
								{
									token.append(1, c);
								}

								else
								{
									// char is the separator, register the new property
									if (c == separator)
									{
										RemoveWhiteSpaces(token);
										dfStack.top().get()[propName].SetString(token, tokenCount);

										token.clear();
										tokenCount++;
									}

									// char is part of the token, appends to a string
									else
									{
										token.append(1, c);
									}
								}
							}
						}

						// any left char makes the final token, used to handle any mistake in the file avoiding crashes
						if (!token.empty())
						{
							RemoveWhiteSpaces(token);
							dfStack.top().get()[propName].SetString(token, tokenCount);
						}
					}

					else // no ' = ' sign
					{
						// the previous property is the new node
						if (line[0] == '{')
						{
							dfStack.push(dfStack.top().get()[propName]);
						}

						else
						{
							// node has been finished, pop it from the stack
							if (line[0] == '}')
							{
								dfStack.pop();
							}

							// line is a property with no assignment 
							else
							{
								propName = line;
							}
						}
					}
				}


				file.close();
				return true;
			}

			return false;
		}

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
			if (index > mObjectVec.size())
			{
				printf("Out of bounds, children doesnt exists\n");
				std::abort();
			}

			return mObjectVec[index].second;
		}

	public: // utils

		// returns if either a property of this node exists or not
		bool Exists(std::string property) const {
			return mObjectMap.find(property) != mObjectMap.end() ? true : false;
		}

		// returns a string value of a property
		const std::string GetString(const size_t count = 0) const {
			if (count >= mContent.size())
				return "";

			return mContent[count];
		}

		void SetString(const std::string& str, const size_t count)
		{
			if (count >= mContent.size())
				mContent.resize(count + 1);

			mContent[count] = str;
		}

	private:

		// remvoes the white spaces of a string
		static void RemoveWhiteSpaces(std::string& str) {
			str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
			str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
		}

	protected:

		bool mIsComment = false; // used to identify if the property is a comment or not

	private:

		std::vector<std::string> mContent; // the items of this serializer
		std::vector<std::pair<std::string, Translation>> mObjectVec; // child nodes of this datafile
		std::unordered_map<std::string, size_t>  mObjectMap;
	};
}