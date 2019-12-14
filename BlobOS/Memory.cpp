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

//Metody użytkowe

Memory::Memory()
{
	for (char &e : RAM)
		e = ' ';
}

void Memory::write_to_ram(int address, char* data)
{

}

void Memory::write_instruction(std::shared_ptr<std::vector<PageInfo>> pt, char* data)
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
}

void Memory::show_pages(int pid)
{

}