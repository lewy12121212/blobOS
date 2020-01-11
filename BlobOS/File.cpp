
#include"File.h"

struct inode
{
	int size_int_byte;

	//pierwsze dwa to nr bloki danych 
	//nastêpny to nr bloku indeksowego
	vector<int> number;

	inode() {
		size_int_byte = 0;
	}


};



FileManager FM;

//Tworzy plik o podanej nazwie, umieszcza go w katalogu
void FileManager::create_file(string name) {

	//Sprawdzenie czy nie ma ju¿ takiego pliku
	int sec_name = find_file(name);
	if (sec_name == -1) {
		inode file_node;
		pair<string, inode> file(name, file_node);

		cataloge.push_back(file);
	}
	else cout << "Taki plik ju¿ istnieje" << endl;

	//zamek???

}

//Wyszukuje pusty blok pamiêci
int FileManager::free_block() {
	for (int i = 0; i < 32; i++) {
		if (disc[i].free == 0)return i;
	}
	cout << "Brak wolnego bloku" << endl;
	return -1;
}

//Odszukuje plik o danej nazwie w katalogu i zwraca jego numer
int FileManager::find_file(string name) {

	int pom = -1;
	for (int i = 0; i < cataloge.size(); i++) {
		if (cataloge[i].first == name) {
			pom = i;
			//cout << "ZNALEZIONO plik" << endl;
		}

	}
	//if (pom == -1)cout << "Nie znaleziono pliku" << endl;
	return pom;
}


void FileManager::clean_block(string name) {
	int pom = find_file(name);

	if (pom != -1) {

		unsigned int length = cataloge[pom].second.size_int_byte;
		if (length > 0) {
			if (length <= 32) {
				disc[cataloge[pom].second.number[0]].block.clear();
				disc[cataloge[pom].second.number[0]].free = 0;
				cataloge[pom].second.number.clear();
				cataloge[pom].second.size_int_byte = 0;

			}

			if (length > 32 && length <= 64) {
			
				disc[cataloge[pom].second.number[0]].block.clear();
				disc[cataloge[pom].second.number[0]].free = 0;
				disc[cataloge[pom].second.number[1]].block.clear();
				disc[cataloge[pom].second.number[1]].free = 0;
				cataloge[pom].second.number.clear();
				cataloge[pom].second.size_int_byte = 0;
			}

			if (length > 64) {
	
				disc[cataloge[pom].second.number[0]].free = 0;
				disc[cataloge[pom].second.number[0]].block.clear();
			
				disc[cataloge[pom].second.number[1]].free = 0;
				disc[cataloge[pom].second.number[1]].block.clear();

				
				int k = 0, len = cataloge[pom].second.number[2];
				int ilosc = disc[len].block.size();
				cout << "ILOSC BLOKOW  " << ilosc << endl;
				while (k < ilosc) {
					int help = int(disc[cataloge[pom].second.number[2]].block[k]) - 48;
					disc[help].block.clear();
					disc[help].free = 0;
					k++;
				}
				disc[cataloge[pom].second.number[2]].free = 0;
				disc[cataloge[pom].second.number[2]].block.clear();
				cataloge[pom].second.size_int_byte = 0;
				cataloge[pom].second.number.clear();
			}
		}

	}
}

void FileManager::show_disc() {
	for (int j = 0; j < 32; j++) {
		cout << j << "("<<disc[j].free<<"): ";
		for(auto e:disc[j].block){
			cout << "[" << e << "] ";
		}
		cout << endl;
	}
}

//Zapisuje tekst do pamiêci dla podanego pliku 
//Dzia³a, ale jeszcze nie ma odczytu du¿ych plików
void FileManager::save_data_to_file(string name, string text) {
	int pom = find_file(name);

	if (pom != -1) {

		unsigned int length = text.size();
		
		clean_block(name);

		if (length <= 32) {
	
			int nr = free_block();
			cataloge[pom].second.number.push_back(nr);
			//zapis danych
			for (int i = 0; i < length; i++) {
				disc[cataloge[pom].second.number[0]].block.push_back( text[i]);
			}
			cataloge[pom].second.size_int_byte = length;

			disc[nr].free = 1;
		}

		if (length > 32 && length <= 64) {
			cout << "B";
			int nr1 = free_block();
			disc[nr1].free = 1;
			cataloge[pom].second.number.push_back(nr1);
			int i = 0;
			for (i; i < 32; i++) {
				disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
			}

			int nr2 = free_block();
			disc[nr2].free = 1;
			cataloge[pom].second.number.push_back(nr2);
			
			for (i; i < length; i++) {
				disc[cataloge[pom].second.number[1]].block.push_back(text[i]);
				
			}

			cataloge[pom].second.size_int_byte = length;

		
		}

		if (length > 64) {
			int nr1 = free_block();
			disc[nr1].free = 1;
			int nr2 = free_block();
			disc[nr2].free = 1;
			cataloge[pom].second.number.push_back(nr1);
			cataloge[pom].second.number.push_back(nr2);
			int i = 0;
			for (i; i < 32; i++) {
				disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
			}
			for (i; i < 64; i++) {
				disc[cataloge[pom].second.number[1]].block.push_back(text[i]);
			}
			
			int index = free_block();
			disc[index].free = 1;
			cataloge[pom].second.number.push_back(index);
			
			cataloge[pom].second.size_int_byte = length;
			int k = 0, len=length-64;
			int j = 0;
			while (i < length) {
				
			
				while (len > 32) {
					nr1 = free_block();

					//cout << "BLOK " << nr1 << " ZOSTAJE DODANY DO INDEX";
					string a = to_string(nr1);
					disc[index].block.push_back(a[0]);
					//disc[cataloge[pom].second.number[2]].block.push_back(a[0]);
					disc[nr1].free = 1;
					while(j<32){
						int help = int(disc[index].block[k]) - 48;
						disc[help].block.push_back(text[i]);
		
						i++;
						j++;
						
					}
					len -= 32;
					j = 0;
					k++;
				}
				if (len < 32) {
					nr1 = free_block();
					string a = to_string(nr1);
					
					disc[index].block.push_back(a[0]);
					disc[nr1].free = 1;
					for (j = 0; j < len; j++) {
	
						int help = int(disc[index].block[k])-48;
						//cout << "TAKI OTO " << help << " ZAWIERA DANE";
						disc[help].block.push_back(text[i]);
						
						i++;
						
					}
				}
				
		}
		}
		
	}
}


//W sumie teraz to chyba nic nie robi, zobaczê potem czy nie bêdê tego potrzebowa³a do czegoœ innego
void FileManager::edit_file(string name, string text) {
	//edytor tekstu od Ani

	save_data_to_file(name, text);

	
}


// Po podaniu nazwy pliku zwraca stringa z jego zawartoœci¹
string FileManager::show_file(string name) {
	string text;
	show_disc();
	int pom = find_file(name);
	if (pom == -1) {
		
		return "Brak takiego pliku";
	}
	int length = cataloge[pom].second.size_int_byte;
	

	if (length<= 32) {
		
		for (int i = 0; i < length; i++) {
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
		}

	}

	if (length > 32 && length <= 64  ) {
		
		int i = 0, j = 0;
		for (i; i < 32; i++) {
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
		}
		for (i; i < length; i++) {
			text.push_back(disc[cataloge[pom].second.number[1]].block[j]);
			j++;
		}
		
	}
	if (length > 64) {
		
		int i = 0, j = 0;
		for (i; i < 32; i++) {
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
		}
		for (i; i < 64; i++) {
			text.push_back(disc[cataloge[pom].second.number[1]].block[j]);
			j++;
		}
		
		int k = 0, len = length - 64;

		while (i < length) {

			while (len > 32) {

				for (j = 0; j < 32; j++) {
					int help = int(disc[cataloge[pom].second.number[2]].block[k]) - 48;
					
					text.push_back(disc[help].block[j]);
					i++;
				

				}
				k++;
				len -= 32;
			}
			if (len < 32) {
				for (j = 0; j < len; j++) {
					int help = int(disc[cataloge[pom].second.number[2]].block[k]) - 48;

					text.push_back(disc[help].block[j]);
					i++;

				}
			}
		
		}
	
	}

	return text;
}


//Jeszcze nie dzia³a
void FileManager::add_to_file() {
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


	/*
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
			}
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

	else {


		cout << "Nie ma takiego pliku";
	}*/
	//dodawanie czegoœ na koñcu pliku
}


//Jeszcze nie usuwa, bêdzie mieæ nazwê pliku jako argument
void FileManager::delete_file(string name) {
	int pom = find_file(name);
	
	if (pom != -1) {

		unsigned int length = cataloge[pom].second.size_int_byte;

		if (length <= 32) {
			array<char, 32> clear_tab;
			//disc[cataloge[pom].second.number[0]].block.swap(clear_tab);
			disc[cataloge[pom].second.number[0]].free = 0;
			cataloge.erase(cataloge.begin()+pom);
			
		}

		if (length > 32 && length <= 64) {
			array<char, 32> clear_tab;
			//disc[cataloge[pom].second.number[0]].block.swap(clear_tab);
		//	disc[cataloge[pom].second.number[1]].block = clear_tab;
			cataloge.erase(cataloge.begin() + pom);

		}

		if (length > 64) {
			/*

			int index = free_block();
			disc[index].free = 1;
			cataloge[pom].second.number.push_back(index);

			cataloge[pom].second.size_int_byte = length;
			int k = 0, len = length - 64;

			while (i < length) {

				nr1 = free_block();
				disc[nr1].free = 1;
				disc[cataloge[pom].second.number[2]].block[k] = nr1;

				if (len >= 32) {

					for (j = 0; j < 32; j++) {
						//disc[nr1].block[j] = text[i];

						i++;

					}
					len -= 32;
				}
				if (len < 32) {
					for (j = 0; j < len; j++) {
						//disc[nr1].block[j] = text[i];

						i++;

					}
				}
				k++;
			}*/
		}


	}
}



