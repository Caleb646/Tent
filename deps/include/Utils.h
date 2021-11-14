#pragma once
#include <string>
#include <vector>


namespace _internal
{
	/*
	* @brief Splits a string by a delimiter and returns a new string
	* @param the string to split
	* @param number of pieces to return in the new string
	* @param the delimiter used to split the string
	* @return shortened string
	*/
	std::string shortenByDelimiter(std::string s, std::size_t numberOfSlices, std::string del)
	{

		//add delimiter to the end so that the last piece is added.
		s += del;
		int start = 0;
		int end = s.find(del);
		std::vector<std::string> pieces;
		while (end != -1) {
			pieces.push_back(s.substr(start, end - start));
			start = end + del.size();
			end = s.find(del, start);
		}
		std::string result;
		for (std::size_t i = pieces.size() - numberOfSlices; i < pieces.size(); i++)
		{
			result += "\\" + pieces[i];
		}
		return result;
	}
}

