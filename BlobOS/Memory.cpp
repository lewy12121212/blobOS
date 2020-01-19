#include "Memory.h"
#include "process.h"

Memory memory;
extern ProcTree PTree;

//Metody użytkowe

Memory::Memory()
{
	for (char &e : RAM)
		e = '\0';
}

void Memory::insert_to_ram(int nr, int data, int PID)
{
	int n = data;
	int size = 0;
	while (n != 0) {
		n = n / 10;
		++size;
	}
	char str[16];
	_itoa_s(data, str, 10);
	for (int i = 0; i < size; i++) {
		int frame = PageHandler(nr+i, PID);
		RAM[nr + i] = str[i];
	}
}

std::array<char, 16> Memory::get_frame(int nr)
{
	std::array<char, 16> data;
	for (int j = 0; j < 16; j++)
	{
		data[j] = RAM[16 * nr + j];
	}
	return data;
}

int Memory::get_data(int nr, int size, int PID)
{
	int data = 0;
	char c = '\0';
	for (int i = 0; i < size; i++)
	{
		int frame = PageHandler(nr + i, PID);
		c = RAM[nr + i];
		if (c >= 48 && c <= 57)
			data += pow(10, size - i - 1) * ((int)c - 48);
		else
		{
			data = data / pow(10, size - i);
			break;
		}
	}
	return data;
}

void Memory::set(int address,char val, int PID){
	int frame = PageHandler(address, PID);
	address = address % 16;
	RAM[(16 * frame) + address] = val;
}

char Memory::get(int address, int PID) {
	int frame = PageHandler(address, PID);
	address = address % 16;
	return RAM[(16 * frame) + address];
}

void Memory::show_frame(int nr)
{
	char c;
	std::cout << "Frame nr " << nr << ":" << "\n";
	for (int j = 0; j < 16; j++)
	{
		c = RAM[16 * nr + j];
		if (c == '\0')
			std::cout << '_';
		else
			std::cout << c;
	}
	std::cout << "\n" << "\n";
}

void Memory::show_ram()
{
	char c;
	std::cout << "RAM: ";
	for (int i = 0; i < 16; i++)
	{
		std::cout << std::endl << "frame " << i << ":\t";
		for (int j = 0; j < 16; j++)
		{
			c = RAM[16 * i + j];
			if (c == '\0')
				std::cout << '_';
			else
				std::cout << c;
		}
	}
	std::cout << "\n" << "\n";
}

/*------------Virtual-------------*/

// Funckja do wypisywania tabelki
template<typename T> void print(T t, const int& width, bool l)
{
	if (l) cout << left << setw(width) << setfill(' ') << t;
	else cout << right << setw(width) << setfill(' ') << t;
}

// Pamięć wirtualna

PageInfo::PageInfo()
{
	this->bit = false;
	this->frame = -1;
};

PageInfo::PageInfo(int frame, bool bit)
{
	this->bit = bit;
	this->frame = frame;
}

Page::Page(){
	this->data = std::array<char, 16>();
	for (char& c : this->data)
	{
		c = '\0';
	}
}

Page::Page(std::string s){
	this->data = std::array<char, 16>();
	for (char &c : this->data)
	{
		c = '\0';
	}
	std::copy(s.begin(), s.end(), data.data());
}

int Memory::PageHandler(int address, int PID) {
	int page = address / 16;
	int offset = address % 16;
	shared_ptr<PCB> process = PTree.find_pid(PTree.init_proc, PID);

	// Jeśli strona nie jest w RAM, załadują ją do RAM
	if(!process->page_table.at(page).bit){
		if (FIFO.size() < 16) {
			// Jeśli są wolne ramki
			if (FIFO.size() == 0) {
				// Jeśli wszystkie ramki wolne
				for (int i = 0; i < 16; i++) {
					RAM.at(i) = PageFile.at(PID).at(page).data.at(i);
				}
				FIFO.push(0);
				Frames.at(0) = std::pair<int, int>(PID, page);
				process->page_table.at(page).frame = 0;
				process->page_table.at(page).bit = true;
			}
			else {
				// Jeśli są jeszcze wolne ramki
				int next_empty_frame = FIFO.back() + 1;

				if (next_empty_frame == 16) next_empty_frame = 0;

				for (int i = 0; i < 16; i++) {
					RAM.at((next_empty_frame * 16) + i) = PageFile.at(PID).at(page).data.at(i);
				}
				FIFO.push(next_empty_frame);
				Frames.at(next_empty_frame) = std::pair<int, int>(PID, page);
				process->page_table.at(page).frame = next_empty_frame;
				process->page_table.at(page).bit = true;
				return next_empty_frame;
			}
		}
		else {
			// Jeśli nie ma wolnych ramek (wymiana)
			// Ramka do zabicia

			int victim = FIFO.front();
			FIFO.pop();
			FIFO.push(victim);


			// Stronnica która znajduje się w ramce victim
			int page_to_update = Frames.at(victim).second;

			// Do kogo należy stronnica z ramki victim
			shared_ptr<PCB> process_to_update = PTree.find_pid(PTree.init_proc, Frames.at(victim).first);

			if (process_to_update != nullptr){
				// Przepisz zmiany z RAM do pliku wymiany
				for (int i = 0; i < 16; i++) {
					PageFile.at(process_to_update->pid).at(page_to_update).data.at(i) = RAM.at((victim * 16) + i);
				}

				// Update informacji w page_table procesu victima
				process_to_update->page_table.at(page_to_update).bit = false;
				process_to_update->page_table.at(page_to_update).frame = -1;
			}

			// Nadpisanie ramki victim nową stroną
			for (int i = 0; i < 16; i++) {
				RAM.at((victim * 16) + i) = PageFile.at(PID).at(page).data.at(i);
			}

			// Update informacji w page_table procesu działającego
			process->page_table.at(page).bit = true;
			process->page_table.at(page).frame = victim;
			return victim;
		}
	}
	return process->page_table.at(page).frame;
}

void Memory::LoadProgram(std::string file_name, int PID)
{
	// Kontener na stronnice procesu 
	this->PageFile.insert(std::pair<int, std::vector<Page>>(PID, std::vector<Page>()));

	// Wczytywanie kodu z pliku do pamięci wirtualnej
	std::fstream file(file_name, std::ios::in);
	std::string code = "";
	char ch;
	while (file >> std::noskipws >> ch) {
		if(ch != '\n') code += ch;
	}
	file.close();

	// Tworzenie stronnic z kodem programu
	for (int i = 0; i < code.length(); i += 16)
	{
		if (16 <= code.length() - i)
			this->PageFile.at(PID).push_back(Page(code.substr(i, 16)));
		else
			this->PageFile.at(PID).push_back(Page(code.substr(i)));
	}

	while (this->PageFile.at(PID).size() < 16) {
		this->PageFile.at(PID).push_back(Page());
	}

	// Wypełnienie tablicy stronnic w PCB
	this->CreatePageTable(PID);
}

void Memory::SetupInitProcess() {
	this->PageFile.insert(std::pair<int, std::vector<Page>>(0, std::vector<Page>()));
	Frames.at(0) = std::pair<int, int>(0, 0);
	this->PageFile.at(0).push_back(Page("JP [0];"));

	this->CreatePageTable(0);

	PTree.find_pid(PTree.init_proc, 0)->page_table.at(0).bit = true;
	PTree.find_pid(PTree.init_proc, 0)->page_table.at(0).frame = 0;

	for (int i = 0; i < 16; i++) {
		RAM.at(i) = PageFile.at(0).at(0).data.at(i);
	}

	FIFO.push(0);
}

void Memory::CreatePageTable(int PID)
{
	auto v = std::vector<PageInfo>();

	std::for_each(this->PageFile.at(PID).begin(), this->PageFile.at(PID).end(), [&](Page& page) {
		v.push_back(PageInfo());
	});

	// Zapisanie PageTable w PCB
	PTree.find_pid(PTree.init_proc, PID)->page_table = v;
}

void Memory::ShowPageTable(int PID) {
	//shared_ptr<PCB> process = PTree.find_pid(PID);
	shared_ptr<PCB> process;

	if (PID == 0) {
		process = PTree.init_proc;
	}else process = PTree.find_pid(PTree.init_proc, PID);

	const int header_width = 15;
	const int field_width = 6;

	print("Page", header_width, true);
	for (int i = 0; i < process->page_table.size(); i++) {
		print(i, field_width, false);
	}
	std::cout << '\n';
	print("Frame", header_width, true);
	for (int i = 0; i < process->page_table.size(); i++) {
		print(process->page_table.at(i).frame, field_width, false);
	}
	std::cout << '\n';
	print("Present bit", header_width, true);
	for (int i = 0; i < process->page_table.size(); i++) {
		print(process->page_table.at(i).bit, field_width, false);
	}
	std::cout << '\n';
}

void Memory::ShowPages(int PID)
{
	char c;
	std::stringstream ss;
	std::cout << "\nPID: " << PID;
	for (int i = 0; i < PageFile.at(PID).size(); i++)
	{
		ss.width(8);
		ss << std::left;
		ss << "\n  Page" << i;
		std::cout << ss.str() << ":\t";
		ss.str(std::string());
		for (int j = 0; j < 16; j++)
		{
			c = PageFile.at(PID).at(i).data.at(j);
			if (c == '\0') std::cout << '_';
			else if (c == '\n') std::cout << '\\';
			else std::cout << c;
		}
	}
	std::cout << "\n" << "\n";
}

void Memory::ShowPageFile() {
	char c;
	std::stringstream ss;
	for (auto pp : PageFile) {
		std::cout << "\nPID: " << pp.first;
		for (int i = 0; i < PageFile.at(pp.first).size(); i++)
		{
			ss.width(8);
			ss << std::left;
			ss << "\n  Page" << i;
			std::cout << ss.str() << ":\t";
			ss.str(std::string());
			for (int j = 0; j < 16; j++)
			{
				c = pp.second.at(i).data.at(j);
				if (c == '\0') std::cout << '_';
				else if (c == '\n') cout << '\\';
				else std::cout << c;
			}
		}
	}
	std::cout << "\n" << "\n";
}

void Memory::ShowQueue() {
	std::queue<int> q = memory.FIFO;
	std::vector<int> t;
	while (!q.empty()) {
		t.push_back(q.front());
		q.pop();
	}
	std::reverse(t.begin(), t.end());
	std::cout << "|-[";
	for (auto i : t) {
		std::cout << " " << i;
	}
	std::cout << " ]->\n\n";
}