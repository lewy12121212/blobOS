#include "Memory.h"
#include "process.h"
#include <fstream>

Memory memory;
extern ProcTree PTree;

//Metody użytkowe

Memory::Memory()
{
	for (char &e : RAM)
		e = ' ';
	char temp[16] = "JP [0];        ";
	std::array<char, 16> dummy;
	for (int i = 0; i < 16; i++)
		dummy[i] = temp[i];
	write_to_ram(0, dummy);
}


void Memory::write_to_ram(int nr, std::array<char, 16> data)
{
	char c;
	for (int j = 0; j < 16; j++)
	{
		c = data[j];
		if (c == '\0')
			RAM[16 * nr + j] = ' ';
		else
			RAM[16 * nr + j] = c;
	}
}

void Memory::insert_to_ram(int nr, int data)
{
	int n = data;
	int size = 0;
	while (n != 0) {
		n = n / 10;
		++size;
	}
	char str[16];
	_itoa_s(data, str, 10);
	for (int i = 0; i < size; i++)
		RAM[nr + i] = str[i];
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

int Memory::get_data(int nr, int size)
{
	int data = 0;
	char c = '\0';
	for (int i = 0; i < size; i++)
	{
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

void Memory::show_frame(int nr)
{
	char c;
	std::cout << "Frame nr " << nr << ":" << "\n";
	for (int j = 0; j < 16; j++)
	{
		c = RAM[16 * nr + j];
		if (c == ' ')
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
			if (c == ' ')
				std::cout << '_';
			else
				std::cout << c;
		}
	}
	std::cout << "\n" << "\n";
}

/*------------Virtual-------------*/

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

void Memory::LoadProgram(std::string file_name, int PID)
{
	// Kontener na stronnice procesu 
	this->PageFile.insert(std::pair<int, std::vector<Page>>(PID, std::vector<Page>()));

	// Wczytywanie kodu z pliku do pamięci wirtualnej
	std::fstream file(file_name, std::ios::in);
	std::string code = "";
	char ch;
	while (file >> std::noskipws >> ch) {
		code += ch;
	}
	file.close();

	// Tworzenie stronnic z kodem programu
	for (int i = 0; i < code.length(); i += 16)
	{
		if (16 > code.length() - i)
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

void Memory::CreatePageTable(int PID)
{
	auto v = std::vector<PageInfo>();

	std::for_each(this->PageFile.at(PID).begin(), this->PageFile.at(PID).end(), [&](Page& page) {
		v.push_back(PageInfo());
	});

	// Zapisanie PageTable w PCB
	PTree.find_pid(PID)->page_table = v;
}

void Page::Print()
{
	for (char c : this->data)
	{
		if (c != '\0')
			std::cout << "'" << c << "' ";
		else
			std::cout << "[ ] ";
	}
	std::cout << "\n";
}

void Memory::ShowPages(int PID)
{
	std::cout << "PID: " << PID << "\n";
	for (Page p : PageFile.at(PID))
	{
		p.Print();
	}
}