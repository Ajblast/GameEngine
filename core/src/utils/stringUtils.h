#pragma once

#include <string>
#include <vector>

namespace GRAVEngine
{
	namespace Utils
	{
		// Is an ASCII character lowercase
		template<typename char_t>
		inline bool isLower(char_t in)
		{
			return (in >= (char_t)'A' && in <= (char_t)'Z');
		}
		// Is an ASCII character uppercase
		template<typename char_t>
		inline bool isUpper(char_t in)
		{
			return (in >= (char_t)'a' && in <= (char_t)'z');
		}
		// Is an ASCII character an alphabetical character
		template<typename char_t>
		inline bool isAlpha(char_t in)
		{
			return isUpper(in) || isLower(in);
		}
		// Is an ASCII character a digit
		template<typename char_t>
		inline bool isDigit(char_t in)
		{
			return (in >= (char_t)'0' && in <= (char_t)'9');
		}
		// Is an ASCII character a hexadecimal digit
		template<typename char_t>
		inline bool isHex(char_t in)
		{
			return isDigit(in) || (in >= (char_t)'a' && in <= (char_t)'f') || (in >= (char_t)'A' && in <= (char_t)'F');
		}
		// Is an ASCII character an alphanumeric character
		template<typename char_t>
		inline bool isAlnum(char_t in)
		{
			return isAlpha(in) || isDigit(in);
		}
		
		// Is an ASCII character a blank character
		template<typename char_t>
		inline bool isBlank(char_t in)
		{
			return in == (char_t)' ' || in == (char_t)'\t'
		}
		// Is an ASCII character a space
		template<typename char_t>
		inline char_t isSpace(char_t in)
		{
			return in == (char_t)' ' || in == (char_t)'\t' || in == (char_t)'\n' || in == (char_t)'\v' || in == (char_t)'\f' || in == (char_t)'\r';
		}
		// Is an ASCII character control character
		template<typename char_t>
		inline bool iscntrl(char_t in)
		{
			return (in >= (char_t)0x00 && in <= (char_t)0x1f) || in == (char_t)0x7f;
		}
		// Is an ASCII character a printable character
		template<typename char_t>
		inline bool isPrint(char_t in)
		{
			return in != (char_t)0x7f && in >= (char_t)0x1f;
		}
		// Is an ASCII character a graphical character
		template<typename char_t>
		inline bool isGraph(char_t in)
		{
			return in != (char_t)' ' && isPrint(in);
		}		
		// Is an ASCII character punctuational character
		template<typename char_t>
		inline bool isPunct(char_t in)
		{
			return isGraph(in) && isAlnum(in) == false;
		}
		
		// Get the lowercase of an ASCII character
		template<typename char_t>
		inline char_t toLower(char_t in)
		{
			return isUpper(in) ? (char_t)(in + 0x20) : in;
		}
		// Get the uppercase of an ASCII character
		template<typename char_t>
		inline char_t toUpper(char_t in)
		{
			return isLower(in) ? (char_t)(in - 0x20) : in;
		}


		// Trim the left of a string
		inline void trimLeft(std::string& str, const std::string& delimiters = " \f\n\r\t\v")
		{
			str.erase(0, str.find_first_not_of(delimiters));
		}
		// Trim the right of a string
		inline void trimRight(std::string& str, const std::string& delimiters = " \f\n\r\t\v")
		{
			str.erase(str.find_last_not_of(delimiters) + 1);
		}
		// Trim the left and right of a string
		inline void trim(std::string& str, const std::string& delimiters = " \f\n\r\t\v")
		{
			trimLeft(str, delimiters);
			trimRight(str, delimiters);
		}

		// Split a string on the delimiter and store the tokens
		inline void split(const std::string& str, const std::string& delimiter, std::vector<std::string>& tokens)
		{
			size_t start = 0;
			size_t end;
			size_t delimLength = delimiter.length();
			std::string token;

			// Search through the string, updating the start position when one is found
			while ((end = str.find(delimiter, start)) != std::string::npos)
			{
				token = str.substr(start, end - start);	// Get the token
				start = end + delimLength;				// Update the start position
				tokens.push_back(token);				// Add the token
			}

			// Add the rest of the string
			tokens.push_back(str.substr(start));
		}
		inline void trimSplit(const std::string& str, const std::string& delimiter, std::vector<std::string>& tokens, const std::string& trimDelimiter = " \f\n\r\t\v")
		{
			// Split the string
			split(str, delimiter, tokens);

			// Trim each string
			for (auto it = tokens.begin(); it != tokens.end(); it++)
				trim((*it), trimDelimiter);
		}


	}

}