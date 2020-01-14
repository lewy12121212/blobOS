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
	vector<char> block;

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
	int find_file(string name);
	void clean_file_data(string name);
	void save_data_to_file(string name, string text);
	void show_disc();
	void edit_file(string name, string text);
	string show_file(string name);
	void add_to_file(string name,string text);
	void delete_file(string name);
};

