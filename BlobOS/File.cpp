#include"File.h"
#include "mutex.h"
struct inode
{
	int size_int_byte; //rozmiar danych pliku
	vector<int> number; //pierwsze dwa to nr bloków bezpoœrednie danych, nastêpny to numer bloku indeksowego
	mutex mutex; 

	inode() {
		size_int_byte = 0;
	}


};


//File info


FileManager FM;

//Tworzy plik o podanej nazwie, umieszcza go w katalogu
void FileManager::create_file(string name) {

	//Sprawdzenie czy nie ma ju¿ takiego pliku
	int sec_name = find_file(name);
	if (sec_name == -1) {
		inode file_node; //Tworzy i-wêze³
		pair<string, inode> file(name, file_node); //Tworzy ,,plik"

		cataloge.push_back(file); //Dodaje plik do katalogu
	}
	else cout << "File exists." << endl;

}

//Wyszukuje pusty blok pamiêci i zwraca jego 
int FileManager::free_block() {
	for (int i = 0; i < 32; i++) {
		if (disc[i].free == 0)return i;
	}
	cout << "There are no free blocks" << endl;
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

//Czyœci plik z danych
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

//Pokazuje kolejkê zamka
//??? <- Ania F
void FileManager::show_lock_queue(const string &filename)
{
	int pom = find_file(filename);
	if (pom != -1) {
		std::deque<std::shared_ptr<PCB>> queue = FM.cataloge[pom].second.mutex.copy_queue();
		int counter = 0;
		std::cout << "Queue of processes waiting for the lock of '" << filename << "' file:\n";
		for (auto process : queue)
		{
			std::cout << counter << ". " << process->name << " " << process->pid << "\n";
			counter++;
		}
		std::cout << "\n";
	}
}

//Zapisuje tekst do pamiêci dla podanego pliku 
void FileManager::save_data_to_file(string name, string text) {
	//Wyszukuje plik
	int pom = find_file(name);
	bool save = 0;
	if (pom != -1) {
		string kopia = show_file(name); //Kopia danych na wypadek gdyby nie zmieœci³y siê w pamiêci i nie mo¿na by³o zapisaæ pliku
		unsigned int length = text.size();
	
		clean_file_data(name); 	//Czyœci aktualne dane pliku

		if (length <= 32) { //zapis dla plików o maksymalnie 32 bajtach
			int nr = free_block(); //wyszukuje pusty blok pamiêci
			if (nr != -1) {
				cataloge[pom].second.number.push_back(nr);
				//zapis danych
				for (int i = 0; i < length; i++) {
					disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
				}
				cataloge[pom].second.size_int_byte = length; //Zmiana d³ugoœci pliku
				disc[nr].free = 1;
			}
			else {
				clean_file_data(name);
				save = 1;
			}
		}

		if (length > 32 && length <= 64) { //zapis dla plików o maksymalnie 64 bajtach

			int nr1 = free_block(); //wyszukuje pusty blok pamiêci
			if (nr1 != -1) {
				disc[nr1].free = 1;
				int nr2 = free_block(); //wyszukuje pusty blok pamiêci
				if (nr2 != -1) {
					disc[nr2].free = 1;
					cataloge[pom].second.number.push_back(nr1);
					int i = 0;
					//zapis danych
					for (i; i < 32; i++) {
						disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
					}

					cataloge[pom].second.number.push_back(nr2);

					for (i; i < length; i++) {
						disc[cataloge[pom].second.number[1]].block.push_back(text[i]);

					}

					cataloge[pom].second.size_int_byte = length;
				}
				else {
					clean_file_data(name);
					save = 1;
				}
			}
		}

		if (length > 64) { //zapis dla wiêkszych plików 
			int i = 0;
			int nr1 = free_block(); //wyszukuje pusty blok pamiêci
			if (nr1 != -1) {
				disc[nr1].free = 1;
				int nr2 = free_block(); //wyszukuje pusty blok pamiêci
				if (nr2 != -1) {

					disc[nr2].free = 1;
					cataloge[pom].second.number.push_back(nr1);
					//zapis danych
					for (i; i < 32; i++) {
						disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
					}


					cataloge[pom].second.number.push_back(nr2);
					//zapis danych
					for (i; i < 64; i++) {
						disc[cataloge[pom].second.number[1]].block.push_back(text[i]);
					}


					int index = free_block(); //wyszukuje pusty blok pamiêci
					if (index != -1) {
						//blok indeksowy
						disc[index].free = 1;
						cataloge[pom].second.number.push_back(index);

						cataloge[pom].second.size_int_byte = length;
						int k = 0, len = length - 64;
						int j = 0, dl = 0;

						while (i < length) {
							//Sprawdzanie czy do bloku ma wpisaæ 32 bajty czy mniej
							if (len > 32)dl = 32;
							else dl = len;


							nr1 = free_block(); //wyszukuje pusty blok pamiêci
							if (nr1 != -1) {
								disc[nr1].free = 1;
								//Wpisywanie numeru bloku (ASCII)
								string a = to_string(nr1);
								if (nr1 > 9)a[0] += nr1 - 1;
								if (nr1 > 19)a[0] -= 1;
								if (nr1 > 29)a[0] -= 1;
								disc[index].block.push_back(a[0]);

								//zapis danych
								for (j = 0; j < dl; j++) {

									int help = int(disc[index].block[k]) - 48;
									disc[help].block.push_back(text[i]);
									i++;

								}
								k++;
								len -= 32;
							}
							else {
								clean_file_data(name);
								save = 1;
								break;
							}
						}
					}
					else {
						clean_file_data(name);
						save = 1;
					}
				}
				else {
					clean_file_data(name);
					save = 1;
				}
			}
			else {
				clean_file_data(name);
				save = 1;
			}
		}

		//Zapis poprzedniego pliku je¿eli, nowe dane siê nie mieszcz¹
		if (save == 1) {
			save_data_to_file(name, kopia);
		}
	}
}


//Sprawdza zameki zapisuje dane do pliku
void FileManager::edit_file(string name, string text) {
	int pom = find_file(name);

	if (pom != -1 && cataloge[pom].second.mutex.get_owner_id() == planist.ReadyPCB[0]->pid) {
			save_data_to_file(name, text);
	}
}

//Sprawdza zamek dla edytora i zapisuje dane do pliku
void FileManager::edit_file_editor(string name, string text) {
	int pom = find_file(name);

	if (pom != -1) {
		if ( cataloge[pom].second.mutex.lock_for_editor()) {
			save_data_to_file(name, text);
		}
	}
}


// Po podaniu nazwy pliku zwraca stringa z jego zawartoœci¹
string FileManager::show_file(string name) {
	string text;
	//show_disc();
	int pom = find_file(name);
	if (pom == -1) {

		return "There is no existing file";
	}
	int length = cataloge[pom].second.size_int_byte;

	//Dla plików nie wiêkszych ni¿ 32 bajty
	if (length <= 32) {

		for (int i = 0; i < length; i++) {
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
		}

	}

	//Dla plików nie wiêkszych ni¿ 64 bajty
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

	//Dla plików wiêkszych ni¿ 64 bajty
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
		if (cataloge[pom].second.mutex.get_owner_id() == planist.ReadyPCB[0]->pid)
		{
			//Zczytuje dane ju¿ zapisane i dopisuje na ich koñcu nowo dodane dane
			string plik = show_file(name);
			plik += text;
			edit_file(name, plik);
		}
	}
	else cout << "There is no existing file" << endl;
}

//Kopiuje zawartoœæ pierwszego pliku do drugiego
void FileManager::copy_file(string name1, string name2) {
	int pom1 = find_file(name1);
	int pom2 = find_file(name2);

	if (pom1 != -1) {
		if (pom2 != -1) {
			string tekst1 = show_file(name1);
			edit_file(name2, tekst1);

		}
		else cout << "There is no existing file" << endl;
	}
	else cout << "There is no existing file" << endl;
}

//Usuwa pliki
void FileManager::delete_file(string name) {
	//Wyszukuje plik
	int pom = find_file(name);
	if (pom == -1) {
		cout<< "There is no existing file";
	}
	else {
		//Czyœci dane pliku i usuwa go z katalogu
		clean_file_data(name);
		cataloge.erase(cataloge.begin() + pom);
	}
}


//Otwieranie pliku z procesu
void FileManager::open_file(string name) {
	//Szuka pliku
	int pom = find_file(name);

	if (pom != -1) {
		//Probuje dostac zamek, jesli sie nie uda zmienia stan procesu na waiting i wywoluje planiste
		cataloge[pom].second.mutex.lock(planist.ReadyPCB[0]);
	}
	else cout << "There is no existing file" << endl;
}

//Otwieranie pliku z procesu
void FileManager::close_file(string name) {
	//Szuka pliku
	int pom = find_file(name);
	if (pom != -1){
		//Odblokowuje zamek
		cataloge[pom].second.mutex.unlock(planist.ReadyPCB[0]);
	}
	else cout << "There is no existing file" << endl;
}

void FileManager::file_info(string name) {

}