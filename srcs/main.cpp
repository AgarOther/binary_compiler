#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>
#include "colors.hpp"

static int handleError(const std::string & errorMsg)
{
	std::cerr << BOLD_NORMAL << "binary_compiler: " << BOLD_RED << "fatal error: " << RESET << errorMsg + "\ncompilation terminated." << std::endl;
	return EXIT_FAILURE;
}

static std::vector<char> readFile(std::ifstream & file)
{
	std::fstream::pos_type pos = file.tellg();
	if (pos < 0)
		throw std::runtime_error("couldn't get file size");
	size_t fileSize = (size_t) pos;
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	if (!file.read(buffer.data(), fileSize))
		throw std::runtime_error("couldn't read file");
	return buffer;
}

int main(int ac, char **av)
{
	if (ac < 2)
		return handleError("no input files");

	std::ifstream file(av[1], std::ios::ate);
	if (file.fail())
		return handleError("invalid input file");

	const std::string outfileName = ac == 3 ? av[2] : "a.out";
	std::vector<char> bytes;
	try
	{
		bytes = readFile(file);
	} catch (const std::exception & e)
	{
		return handleError(e.what());
	}

	unsigned char tmp = 0;
	size_t cursor = 0;
	size_t line = 1;
	size_t row = 1;
	bool commentMode = false;
	std::vector<char> outBytes;

	outBytes.reserve(bytes.size() / 8);
	for (size_t i = 0; i < bytes.size(); ++i)
	{
		const char c = bytes[i];
		if (c == '\n')
		{
			line++;
			commentMode = false;
			row = 1;
			continue;
		}
		else if (commentMode)
			continue;
		else if (c == '/' && i + 1 < bytes.size() && bytes[i + 1] == '/')
		{
			commentMode = true;
			i++;
			continue;
		}
		if (c == '0' || c == '1')
		{
			if (c == '1')
				tmp |= static_cast<unsigned char>(1u << (7 - cursor));
			cursor++;
			if (cursor == 8)
			{
				outBytes.push_back(tmp);
				cursor = tmp = 0;
			}
		}
		else if (!std::isspace(static_cast<unsigned char>(c)))
			return handleError("invalid character at line " + std::to_string(line) + " row " + std::to_string(row) + ": '" + c + "'");
		row++;
	}
	if (cursor != 0)
		return handleError("memory is not aligned as 8 bits (missing " + std::to_string(8 - cursor) + " bits) at line "+ std::to_string(line) + " row " + std::to_string(row - 1) + ".");
	std::ofstream outfile(outfileName, std::ios::binary | std::ios::trunc);
	if (outfile.fail())
		return handleError("invalid output file");
	outfile.write(outBytes.data(), outBytes.size());
	return EXIT_SUCCESS;
}
