#include <iostream>
#include <string>
#include "Compress.h"

int main()
{
	
	Compress compress;
	std::string filepath = "Source.txt";
	std::string decodedfile = "Decompressed.txt";
	std::string compressedfile =  "Compressed.txt";
	compress.Execute(filepath);
	std::cout << "Encoding.....";
	compress.DecodeToOriginal(compressedfile,decodedfile);
	std::cout << "\nDecoding.....";
	
	
	
	std::cin.get();
}