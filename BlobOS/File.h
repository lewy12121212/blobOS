
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

	inode() {
		size_int_byte = 0;
	}

	void change_size(string name) {
		fstream plik(name.c_str(), ios::app);

		if (plik)
		{

			plik.seekg(0, plik.end);
			int length = plik.tellg();
			plik.seekg(0, plik.beg);

			cout << "Pom " << length << endl;
			size_int_byte = length;
		}


		plik.close();
	}

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
	cout << "Podaj nazwê pliku ";
	cin >> name;
	name += ".txt";
	int pom = -1;
	for (int i = 0; i < cataloge.size(); i++) {
		if (cataloge[i].first == name) {
			pom = i;
			cout << "ZNALEZIONO" << endl;
		}
	}

	if (pom != -1)
		cout << "1=" << cataloge[pom].second.size_int_byte << endl;
	else {
		cout << "Nie ma takiego pliku";
	}


	fstream plik;
	plik.open(name.c_str(), ios::in | ios::out);
	if (plik) {

		getline(cin, text);
		plik << text;
		plik << "qwerty";

	}
	else
	{
		cout << "BLAD" << endl;
	}
	plik.close();
	if (pom != -1)
		cataloge[pom].second.change_size(name);
	else {
		cout << "Nie ma takiego pliku";
	}

	if (pom != -1)
		cout << "2=" << cataloge[pom].second.size_int_byte << endl;
	else {
		cout << "Nie ma takiego pliku";
	}


}

void add_to_file() {
	string name;
	string text;
	cout << "Podaj nazwê pliku ";
	cin >> name;
	name += ".txt";

	int pom = -1;
	for (int i = 0; i < cataloge.size(); i++) {
		if (cataloge[i].first == name) {
			pom = i;
		}
	}



	//czy ma pokazywaæ co juz jest w plkiu?
	ofstream plik(name.c_str(), ios::app);
	if (plik) {
		unsigned char znak;
		do
		{
			getline(cin, text);
			plik << text;
			/*if ((int)text.back == 13) {

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

	cataloge[pom].second.change_size(cataloge[pom].first);

	if (pom != -1)
		cout << "3=" << cataloge[pom].second.size_int_byte << endl;
	else {
		cout << "Nie ma takiego pliku";
	}
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
