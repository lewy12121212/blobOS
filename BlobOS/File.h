#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<array>
#include<fstream>

using namespace std;

struct disc_area
{
	
};

array<char,32> disc;

struct inode
{

};

vector < pair < string, inode >> cataloge;

void create_file() {
	string name;
	cout << "Podaj nazwê pliku ";
	cin >> name;
	inode file_node;
	name += ".txt";
	ifstream plik(name);
	pair<string, inode> file(name,file_node);
	cataloge.push_back(file);
}