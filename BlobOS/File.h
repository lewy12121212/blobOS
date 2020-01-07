
#include<iostream>
#include<string>
#include<vector>
#include<array>
#include<fstream>
#include <conio.h>

using namespace std;


struct inode
{
	int size_int_byte;
	//numeracja od 0
	vector<int> number;


};

struct disc_area
{
	array<char, 32> block;


};

array<disc_area, 32> disc;


vector < pair < string, inode >> cataloge;

void create_file() {
	string name;
	cout << "Podaj nazwê pliku ";
	cin >> name;
	inode file_node;
	name += ".txt";
	ofstream plik(name.c_str());
	if (plik) {
		cout << "UTWORZONO PLIK" << endl;
	}
	else
	{
		cout << "BLAD TWORZENIA PLIKU" << endl;
	}
	plik.close();

	pair<string, inode> file(name, file_node);
	cataloge.push_back(file);
}

void edit_file() {
	string name;
	string text;
	string linia;
	cout << "Podaj nazwê pliku ";
	cin >> name;
	name += ".txt";
	fstream plik(name.c_str(), ios::in | ios::out);
	if (plik) {
		while (!plik.eof())
		{
			getline(plik, linia);
			cout << linia << endl;
		}
	}
	else
	{
		cout << "BLAD" << endl;
	}
	plik.close();

}

void add_to_file() {
	string name;
	string text;
	cout << "Podaj nazwê pliku ";
	cin >> name;
	name += ".txt";
	//czy ma pokazywaæ co juz jest w plkiu?
	ofstream plik(name.c_str(), ios::app);
	if (plik) {
		unsigned char znak;
		do
		{
			getline(cin, text);
			/*if ((int)text.back == 13) {
				plik << text;
				plik << endl;
			}
			else {
				plik << text;
			}*/
			znak = _getch();
			cout << znak;
			if (znak != 27)plik << znak;
			static_cast <int>(znak);
			while (_kbhit())
			{
				getline(cin, text);
				plik << text;
				znak = _getch();
				cout << znak;
				if (znak != 27)plik << znak;
				static_cast <int>(znak);
			}
			//std::cout << std::endl;


		} while (znak != 27); //ESC

	}
	else
	{
		cout << "BLAD" << endl;
	}
	plik.close();
	//dodawanie czegoœ na koñcu pliku
}

void delete_file() {

	vector<pair<string, inode>>::iterator i = cataloge.begin();
	string name;
	cout << "Podaj nazwê pliku ";
	cin >> name;
	name += ".txt";
	for (auto e : cataloge) {
		if (e.first == name) {
			break;
		}
		i++;
	}

	cataloge.erase(i);
	//remove(name.c_str());
	if (remove(name.c_str()) == 0)
		printf("Usunieto pomyslnie plik.");
	else
		printf("Nie udalo sie skasowac pliku.");


	//usuniêcie fizycznego pliku
}