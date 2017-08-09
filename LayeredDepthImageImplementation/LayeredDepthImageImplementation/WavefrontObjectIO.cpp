#include "stdafx.h"
#include "WavefrontObjectIO.h"


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

bool WavefrontObjectIO::LoadObjectFile(std::string objectFile, std::vector<float>* vertexArray, std::vector<unsigned int>* indexArray)
{
	ifstream file(objectFile);


	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::vector<std::string> tokens;
			std::stringstream ss(line);
			string item;

			while (getline(ss, item, ' '))
			{
				tokens.push_back(item);
			}

			if (tokens[0] == "v")
			{
				vertexArray->push_back(atof(tokens[1].c_str()));
				vertexArray->push_back(atof(tokens[2].c_str()));
				vertexArray->push_back(atof(tokens[3].c_str()));
			}
			else if (tokens[0] == "f")
			{
				indexArray->push_back(atoi(tokens[1].c_str()));
				indexArray->push_back(atoi(tokens[2].c_str()));
				indexArray->push_back(atoi(tokens[3].c_str()));
			}
		}

		file.close();
		return true;
	}
	else
	{
		return false;
	}
}