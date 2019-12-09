#pragma once
#include<vector>
#include<string>


using namespace std;

class Shell {
protected:
	bool status;
	string line;
	vector<string>parsed;
	

	void read_line();
	void parse();
	void execute();
	void loop();
	void not_recognized();
	void help();
	void cls();
	void exit();

	//procesy i proces
	void cp();
	void dp();
	void show(); //pcb, pcblist, ram, pagefile, root
	void showpcb();
	void showpcblist();
	void showtree();
	
	//pamiêæ
	void showram();
	void showpagefile();

	//pliki i katalogi
	void showroot();
	void cf();
	void df();
	void wf();
	void fileinfo();

	//interpreter
	void go();


public :
	void boot();
	Shell();
	~Shell() = default;
};
