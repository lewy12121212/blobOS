#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<array>
#include<fstream>
#include <conio.h>

using namespace std;

struct inode;
struct  disc_area
{
	bool free;
	array<char, 32> block;

	disc_area() {
		free = 0;
	}

};

class FileManager {
	public:
	array<disc_area, 32> disc;
	vector < pair < string, inode >> cataloge;
	void create_file(string name);
	int free_block();
	void save_data_to_file( string text, int pom);
	void edit_file(string name, string text);
	string show_file(string name);
	void add_to_file();
	void delete_file();
};

