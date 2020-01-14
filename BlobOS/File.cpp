
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
	else cout << "File is existing" << endl;

	//zamek???

}

//Wyszukuje pusty blok pamiêci
int FileManager::free_block() {
	for (int i = 0; i < 32; i++) {
		if (disc[i].free == 0)return i;
	}
	cout << "There is no free blocks" << endl;
	return -1;
}

//Odszukuje plik o danej nazwie w katalogu i zwraca jego numer
int FileManager::find_file(string name) {
	int pom = -1;
	for (int i = 0; i < cataloge.size(); i++) {
		if (cataloge[i].first == name) {
			pom = i;
		
		}

	}
	return pom;
}

//Czyœci 
void FileManager::clean_file_data(string name) {
	//Szuka pliku
	int pom = find_file(name);

	if (pom != -1) {

		unsigned int length = cataloge[pom].second.size_int_byte;
		if (length > 0) {
			if (length <= 32) {
				//Czyœci dane zapisane w bloku pamiêci
				disc[cataloge[pom].second.number[0]].block.clear();
				disc[cataloge[pom].second.number[0]].free = 0;
				//Usuwa blok pamiêci z i-wêz³a
				cataloge[pom].second.number.clear();
				cataloge[pom].second.size_int_byte = 0;

			}

			if (length > 32 && length <= 64) {
				//Czyœci dane zapisane w blokach pamiêci
				disc[cataloge[pom].second.number[0]].block.clear();
				disc[cataloge[pom].second.number[0]].free = 0;
				disc[cataloge[pom].second.number[1]].block.clear();
				disc[cataloge[pom].second.number[1]].free = 0;
				//Usuwa bloki pamiêci z i-wêz³a
				cataloge[pom].second.number.clear();
				cataloge[pom].second.size_int_byte = 0;
			}

			if (length > 64) {
				//Czyœci dane zapisane w blokach pamiêci
				disc[cataloge[pom].second.number[0]].free = 0;
				disc[cataloge[pom].second.number[0]].block.clear();

				disc[cataloge[pom].second.number[1]].free = 0;
				disc[cataloge[pom].second.number[1]].block.clear();


				int k = 0, len = cataloge[pom].second.number[2];
				int ilosc = disc[len].block.size();
				while (k < ilosc) {
					//Czyœci bloki pamiêci zapisane w bloku indeksowym
					int help = int(disc[cataloge[pom].second.number[2]].block[k]) - 48;
					disc[help].block.clear();
					disc[help].free = 0;
					k++;
				}
				//Czyœci blok indeksowy
				disc[cataloge[pom].second.number[2]].free = 0;
				disc[cataloge[pom].second.number[2]].block.clear();
				cataloge[pom].second.size_int_byte = 0;
				//Usuwa bloki pamiêci z i-wêz³a
				cataloge[pom].second.number.clear();
			}
		}

	}
}

//Pokazuje dysk
void FileManager::show_disc() {
	for (int j = 0; j < 32; j++) {
		cout << j << "(" << disc[j].free << "): ";
		for (auto e : disc[j].block) {
			cout << "[" << e << "] ";
		}
		cout << endl;
	}
}

//Zapisuje tekst do pamiêci dla podanego pliku 
void FileManager::save_data_to_file(string name, string text) {
	//Wyszukuje plik
	int pom = find_file(name);

	if (pom != -1) {

		unsigned int length = text.size();
		//Wyczyœæ dane pliku
		clean_file_data(name);

		if (length <= 32) {

			int nr = free_block();
			if (nr != -1) {
				cataloge[pom].second.number.push_back(nr);
				//zapis danych
				for (int i = 0; i < length; i++) {
					disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
				}
				cataloge[pom].second.size_int_byte = length;

				disc[nr].free = 1;
			}
			//brak bloku
		}

		if (length > 32 && length <= 64) {
			
			int nr1 = free_block();
			if (nr1 != -1) {
				disc[nr1].free = 1;
				cataloge[pom].second.number.push_back(nr1);
				int i = 0;
				//zapis danych
				for (i; i < 32; i++) {
					disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
				}

				int nr2 = free_block();
				if (nr2 != -1) {
					disc[nr2].free = 1;
					cataloge[pom].second.number.push_back(nr2);

					for (i; i < length; i++) {
						disc[cataloge[pom].second.number[1]].block.push_back(text[i]);

					}

					cataloge[pom].second.size_int_byte = length;
				}
				//brak bloku 2
			}
			//brak bloku
		}

		if (length > 64) {
			int i = 0;
			int nr1 = free_block();
			if (nr1 != -1) {
				disc[nr1].free = 1;
				cataloge[pom].second.number.push_back(nr1);
				//zapis danych
				for (i; i < 32; i++) {
					disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
				}
				int nr2 = free_block();
				if (nr2 != -1) {
					disc[nr2].free = 1;
					cataloge[pom].second.number.push_back(nr2);
					//zapis danych
					for (i; i < 64; i++) {
						disc[cataloge[pom].second.number[1]].block.push_back(text[i]);
					}


				}
				//brak bloku 2
			}
			//brak bloku 1
			int index = free_block();
			if (index != -1) {
				//blok indeksowy
				disc[index].free = 1;
				cataloge[pom].second.number.push_back(index);

				cataloge[pom].second.size_int_byte = length;
				int k = 0, len = length - 64;
				int j = 0;
				while (i < length) {


					while (len > 32) {
						//Zapis do kolejnych bloków po 32 bajty
						nr1 = free_block();
						if (nr1 != -1) {
							disc[nr1].free = 1;

							string a = to_string(nr1);
							if (nr1 > 9)a[0] += nr1 - 1;
							if (nr1 > 19)a[0] -= 1;
							if (nr1 > 29)a[0] -= 1;
							disc[index].block.push_back(a[0]);

							while (j < 32) {
								int help = int(disc[index].block[k]) - 48;
								disc[help].block.push_back(text[i]);
								i++;
								j++;
							}
							len -= 32;
							j = 0;
							k++;
						}
						//brak i-tego bloku
					}
					if (len <= 32) {
						//Zapis je¿eli pozosta³e do zapisu dane maj¹ nie wiêcej ni¿ 32 bajty
						nr1 = free_block();
						if (nr1 != -1) {
							disc[nr1].free = 1;

							string a = to_string(nr1);
							if (nr1 > 9)a[0] += nr1 - 1;
							if (nr1 > 19)a[0] -= 1;
							if (nr1 > 29)a[0] -= 1;
							disc[index].block.push_back(a[0]);

							for (j = 0; j < len; j++) {

								int help = int(disc[index].block[k]) - 48;
								disc[help].block.push_back(text[i]);
								i++;
							}
						}
						//brak i-tego bloku
					}

				}
			}
			//brak bloku indexowego
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


	if (length <= 32) {

		for (int i = 0; i < length; i++) {
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
		}

	}

	if (length > 32 && length <= 64) {

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
			if (len <= 32) {
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


//dodawanie czegoœ na koñcu pliku
void FileManager::add_to_file(string name, string text) {
	//szuka pliku
	int pom = find_file(name);
	if (pom != -1) {
		//Zczytuje dane ju¿ zapisane i dopisuje na ich koñcu nowo dodane dane
		string plik = show_file(name);
		plik += text;
		save_data_to_file(name, plik);
	}
	else cout << "Brak pliku" << endl;
}


//Usuwa pliki
void FileManager::delete_file(string name) {
	//Wyszukuje plik
	int pom = find_file(name);
	if (pom == -1) {
		cout<< "Brak takiego pliku";
	}
	else {
		//Czyœci dane pliku i usuwa go z katalogu
		clean_file_data(name);
		cataloge.erase(cataloge.begin() + pom);
	}
}



