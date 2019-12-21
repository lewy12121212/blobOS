#include "Memory.h"

Memory memory;

//Metody użytkowe

Memory::Memory()
{
	for (char &e : RAM)
		e = ' ';
}


void Memory::write_to_ram(int address, char *data)
{
}

void Memory::show_frame(int nr)
{

}

void Memory::show_ram()
{
	char c;
	std::cout << "RAM content:";
	for (int i = 0; i < 16; i++)
	{
		std::cout << std::endl
				  << "frame " << i << ":\t";
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

void Memory::LoadProgram(std::string kod, int PID)
{
	// Kontener na stronnice procesu 
	this->PageFile.insert(std::pair<int, std::vector<Page>>(PID, std::vector<Page>()));

	// Tworzenie stronnic z kodem programu
	for (int i = 0; i < kod.length(); i += 16)
	{
		if (16 > kod.length() - i)
			this->PageFile.at(PID).push_back(Page(kod.substr(i, 16)));
		else
			this->PageFile.at(PID).push_back(Page(kod.substr(i)));
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