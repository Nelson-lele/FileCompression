
#include <iostream>

#include "Compress.h"

#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <sstream>



std::string Compress::ReadBinaryFromFile(const std::string& filepath) const
{
	std::ifstream file(filepath);

	std::string line,result;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			result += line + '\n';
		}
		
	}
	else
		std::cerr << "Unable to Open file" << std::endl;
	return result;
}

void Compress::Execute(const std::string& filename)
{
	
	OriginalString_ = ReadBinaryFromFile(filename);
	CompressData(OriginalString_);
	SaveEncodedData("Compressed.txt", HuffCodes_);

}

void Compress::DecodeToOriginal(const std::string& filepath,const std::string& dfile)
{
	
	Decoder decode = GetEncodedData(filepath);
	std::string decodedText;
	std::string currentBinarySequence;

	
	for (char bit : decode._EncodedString)
	{
		currentBinarySequence += bit;
		if (decode._MapTree.find(currentBinarySequence) != decode._MapTree.end())
		{
			decodedText += decode._MapTree[currentBinarySequence];
			currentBinarySequence = "";
		}
		
	}
	std::ofstream file(dfile);
	if (file.is_open())
	{
		file << decodedText << std::endl;
	}
	file.close();
	
}




void Compress::CompressData(const std::string& line)
{
	unsigned int count = 0;
	std::unordered_map<char, int> frequencyMap;
	for (char ch : line)
	{
		frequencyMap[ch]++;
	}

	for (const auto& pair : frequencyMap)
	{
		TreeNode* node = new TreeNode(pair.second,pair.first);
		MinHeap_.push(node);
	}

	while (MinHeap_.size() > 1)
	{
		TreeNode* leftNode = MinHeap_.top(); MinHeap_.pop();
		TreeNode* rightNode = MinHeap_.top(); MinHeap_.pop();

		int frequencies = leftNode->_data + rightNode->_data;

		char symbol = (count < line.size()) ? line[count] : '\0'; 

		TreeNode* newNode = new TreeNode(frequencies, '\0');
		newNode->left = leftNode;
		newNode->right = rightNode;
		MinHeap_.push(newNode);  
		

		
		count++;
	}
	if(!MinHeap_.empty())
		GenerateCodes(MinHeap_.top(), "", HuffCodes_);


}


void Compress::GenerateCodes(TreeNode* Node, std::string CurrentCode, std::unordered_map<char, std::string>& HuffCodes)
{
	if (!Node) return;

	if (Node->_character != '\0')
	{
		HuffCodes[Node->_character] = CurrentCode;
	}
	GenerateCodes(Node->left, CurrentCode + "0", HuffCodes);
	GenerateCodes(Node->right, CurrentCode + "1", HuffCodes);
}

void Compress::SaveEncodedData(const std::string& filepath,std::unordered_map<char,std::string>& huffCodes)
{
	std::ofstream file(filepath);

	if (file.is_open())
	{
		file << "MAP" << std::endl;
		for (const auto& word : huffCodes)
		{
			file << word.first << " : " << word.second << std::endl;
		}
		file << "ENCODED STRING" << std::endl;;
		file << GetEncodedString();
	}
	else
		std::cerr << "Unable to write to file " << std::endl;
	file.close();
}

std::string Compress::GetEncodedString() const
{
	std::string encodedString;
	for (char ch: OriginalString_)
	{
		encodedString += HuffCodes_.at(ch);
	}
	return encodedString;
}

Decoder Compress::GetEncodedData(const std::string& filepath)
{
	std::ifstream file(filepath);
	enum class DecoderType
	{
		NONE=-1,ENCODED=1,TREE=0
	};
	std::stringstream ss[2];
	std::string text;
	std::unordered_map<std::string,char> map;
	DecoderType type = DecoderType::NONE;
	if (file.is_open())
	{
		
		while (std::getline(file, text))
		{
			
			if (!text.empty())
			{
				int index = text.find(":");
				if (index != std::string::npos)  
				{
					char character = text[0];  
					std::string binaryCode = text.substr(index + 2); 

					map[binaryCode] = character;
				}
				
			}
			
			if (text.find("ENCODED STRING") != std::string::npos)
			{
				type = DecoderType::ENCODED;

			}
		}
		ss[(int)type] << text << std::endl;
		file.close();
	}
	else
		std::cerr << "Unable to open file" << std::endl;

	return { map,ss[1].str()};
}
