#include "Memory.h"

//Metody struktury PageInfo

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

//Metody Page

Page::Page()
{
	for (char &c : this->data)
	{
		c = '\0';
	}
}

Page::Page(std::string s)
{
	for (char &c : this->data)
	{
		c = '\0';
	}
	std::copy(s.begin(), s.end(), data.data());
}

void Page::print()
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
//Metody użytkowe

Memory::Memory()
{
	for (char &e : RAM)
		e = ' ';
}

void Memory::load_program(std::string kod, int PID)
{
	this->PageFile.insert(std::pair<int, std::vector<Page>>(PID, std::vector<Page>()));

	for (int i = 0; i < kod.length(); i += 16)
	{
		//ProcessTree.get(PID).PageTable.push_back(PageInfo());
		if (16 > kod.length() - i)
			this->PageFile.at(PID).push_back(Page(kod.substr(i, 16)));
		else
			this->PageFile.at(PID).push_back(Page(kod.substr(i)));
	}
}

void Memory::write_to_ram(int address, char *data)
{
}

void Memory::write_instruction(std::shared_ptr<std::vector<PageInfo>> pt, char *data)
{
}

void Memory::show_frame(int nr)
{
}

std::shared_ptr<std::vector<PageInfo>> Memory::create_page_table(int size, int pid)
{
	return nullptr;
}

//Metody pracy krokowej i wyświetlania pamięci

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
}

void Memory::show_pages(int PID)
{
	std::cout << "PID: " << PID << "\n";
	for (Page p : PageFile.at(PID))
	{
		p.print();
	}
}