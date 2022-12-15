#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

int main(int argc, char** argv)
{
	int length;
	int max = 0;
	int maxLength = 0;
	unordered_map<int,int>map;
	unordered_set<string> word;
	unordered_set<string> wc;
	fstream file;
	string line;
	
	file.open(argv[1], ios::in); //open and read file entered as argv[1]
	if(file.is_open())
	{
		while (getline(file, line)) //collect everything inside file one line at a time
		{
			if(line[0] != '#') // ignore lines that start with '#'

			//remove any punctuation found in words
			for(int i=0, len= line.size(); i<len;i++)
			{ 
				if(ispunct(line[i]))
				{
					line.erase(i--,1);
					len=line.size();
				}
			}
			
			//turn all words into lower case so unordered_set can remove duplicates
			for(size_t i= 0; i < line.length(); i++)
			{
				line[i] = tolower(line[i]);
			}
				
			//parse through input and store each word separately
			string delimiter = " "; // space will signify where to stop searching and store word
			size_t pos = 0;
			string token; // variable to store each word from input separately
			while ((pos = line.find(delimiter)) != string::npos)
			{
				token = line.substr(0, pos);
				line.erase(0,pos+delimiter.length());
						
				wc.insert(line); //adds all words into a set to check word count of file
				map[token.length()]++; //store sizes of words into map
				
				//checking word lengths and comparing to each other to find size of most common and longest word
				for(auto it=map.begin(); it != map.end(); it++)
				{
					 if(max < it->second)
					 {
						 length = it->first;
						 max = it->second;
					 }
					 
					 if(maxLength < it->first)
					 {
						maxLength = it->first;
						maxLength = maxLength-2;
					 }
				}
				
				// store words of longest length only in different set
				if(token.length() == 10)
				{
					word.insert(token);
				}	 	
			}
		}
				
						
		cout << argv[1] << " contains " << wc.size()+2<< " words" <<endl; //added 2 because 2 words were stored in another variable when parsing
		cout << "words of size " << length << " occur the most: " << max << " times" << endl;
		cout << "the longest words have " << maxLength << " letters and occur " << map[maxLength] << " times" << endl;
		
		//printing out all elements stores inside set in a loop for proper formatting
		for(auto it=word.begin(); it != word.end(); it++)
		{
			cout << "\t" << *it << endl;
		}
		
		file.close(); //close file at the end
		
	}
	
	else
	cerr << "Usage: executable filename" << endl; //incase of wrong input, message will show proper input
	return 0;
}

