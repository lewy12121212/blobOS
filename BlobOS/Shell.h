#pragma once
#include<vector>
#include<string>
#include "procesor.h"
#include "Memory.h"
#include "process.h"
#include<conio.h>


class Shell {
protected:
	bool status;
	std::string line;
	std::vector<std::string>parsed;
	

	void read_line();
	void parse();
	void execute();
	void loop();
	void not_recognized();
	void help();
	void clear();
	void exit();

	//procesy i proces
	void cp();
	void dp();
	void show(); //pcb, pcblist, ram, pagefile, root
	void showpcb();
	void showpcblist();
	void showtree();
	
	//pami��
	void showram();
	void showpagefile();
	void showpagetable();
	void showqueue();
	void showframes();

	//pliki i katalogi
	void showroot();
	void touch();
	void rm();
	void wf();
	void copy();
	void cat();
	void fileinfo();
	void editor(std::string filename);

	//interpreter
	void go();


public :
	void boot();
	Shell();
	~Shell() = default;
};



/*
show
	-pcb
	-pcblist
	-tree
	-ram
	-pagetable
	-queue
	-frames
	-pagefile

	cp [PROC NAME] [FILENAME] [PARENT PID] -tworzenie procesu
	dp [PROC NAME] -usuwanie procesu
	touch [FILENAME] -tworzenie nowego pliku
	rm [FILENAME] -





*/