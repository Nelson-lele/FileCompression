#pragma once

#include <iostream>
#include <queue>
#include <unordered_map>

struct Decoder
{
	std::unordered_map<std::string,char>_MapTree;
	std::string _EncodedString;
};

struct TreeNode
{
	int _data;
	char _character;


	TreeNode* left;
	TreeNode* right;

	TreeNode(int  value, char ch) :_character(ch), _data(value), right(nullptr), left(nullptr) {}


	bool operator<(const TreeNode& other) const
	{
		return _data > other._data;
	}
	~TreeNode()
	{
		delete left;
		delete right;
	}

};
struct Compare
{
	// Create a comparator for the priority_queue
	bool operator()(const TreeNode* first, const TreeNode* last) const
	{
		return first->_data > last->_data;
	}
};

class Compress
{	
public:
	void Execute(const std::string&);
	void DecodeToOriginal(const std::string&, const std::string&);
	std::string ReadBinaryFromFile(const std::string&) const;

private:

	std::priority_queue<TreeNode*, std::vector<TreeNode*>, Compare> MinHeap_;
	std::unordered_map<char, std::string> HuffCodes_;
	std::string OriginalString_;
	std::string EncodedString_;
	std::string EncodedTree_;

private:
	void CompressData(const std::string&);
	std::string GetEncodedString()const;
	void GenerateCodes(TreeNode*,std::string,std::unordered_map<char,std::string>&);
	void SaveEncodedData(const std::string&, std::unordered_map<char, std::string>&);
	Decoder GetEncodedData(const std::string&);
};

