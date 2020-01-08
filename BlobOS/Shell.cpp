#include"Shell.h"
#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include"Help.h"

extern Memory memory;
extern ProcTree PTree;

Shell::Shell() {
	status = true;
	this->line.clear();
	this->parsed.clear();

	//Coœ potem do procesów
}

void Shell::boot() {
	loop();
}

void Shell::read_line() {
	std::cout << "$ ";
	getline(std::cin, line);
	std::transform(line.begin(), line.end(), line.begin(), ::tolower);
	parse();
}

void Shell::parse() {

	line = line + ' ';

	std::string temp;
	for (int i = 0; i < line.size(); i++) {
		if (line[i] != ' ') {
			temp += line[i];
		}
		else {
			parsed.push_back(temp);
			temp.clear();
		}
	}
}

void Shell::loop() {
	do {
		read_line();
		try {
			execute();
		}
		catch (std::string & e) {
			std::cout << e << std::endl;
			std::cout << "Try \'" << parsed[0] << " --help\' for more information." << std::endl;
		}

		line.clear();
		parsed.clear();

	} while (status);
}

void Shell::not_recognized() {
	std::cout << parsed[0] << ": command not found" << std::endl;
}

void Shell::help() {
	std::cout << "Help" << std::endl;
}

void Shell::execute() {
									 //procesy
	if (parsed[0] == "cp") { cp(); }
	else if (parsed[0] == "dp") { dp(); }
	else if (parsed[0] == "show") { show(); }
	else if (parsed[0] == "touch") { touch(); } //pliki
	else if (parsed[0] == "rm") { rm(); }
	else if (parsed[0] == "copy") { copy(); }
	else if (parsed[0] == "cat") { cat(); }
	else if (parsed[0] == "wf") { wf(); }
	else if (parsed[0] == "fileinfo") { fileinfo(); }
	else if (parsed[0] == "go" || parsed[0] == "") { go(); }
	else if (parsed[0] == "help") { help(); }
	else if (parsed[0] == "clear") { clear(); }
	else if (parsed[0] == "exit") { exit(); }
	else not_recognized();
}

void Shell::exit() {
	status = false;
	std::cout << "Koniec" << std::endl;
}

void Shell::clear() {
	system("cls");
	std::cout << "Czysto" << std::endl;
}

void Shell::cp() {

	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::cp();
	}
	else if (parsed.size() == 2||parsed.size()==1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else if (parsed.size() == 3) {
		//std::cout << "Czekamy na Eryka -cp()" << std::endl;
		// To do dadania 3 parametr (nazwa procesu, nazwa pliku, parent_pid)
		PTree.create_process_file(parsed[1], parsed[2], 1);
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}

void Shell::dp() {
	if (parsed.size() == 1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::dp();
	}
	else if (parsed.size() == 2) {
		std::cout << "Czekamy na Eryka -kill" << std::endl;
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[2] + "\'";
		throw exc;
	}
}

void Shell::show() {
	 if (parsed.size() == 1) {
			std::string exc = parsed[0] + ": " + "missing operand";
			throw exc;
	}
	else if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() == 2) {
		if (parsed[1] == "-pcb") showpcb();
		else if (parsed[1] == "-pcblist") showpcblist();
		else if (parsed[1] == "-tree") showtree();
		else if (parsed[1] == "-ram") showram();
		else if (parsed[1] == "-pagetable") showpagetable();
		else if (parsed[1] == "-queue") showqueue();
		else if (parsed[1] == "-frames") showframes();
		else if (parsed[1] == "-pagefile") showpagefile();

		else {
			std::string exc = "error: unsupported option";
			throw exc;
		}
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1]+": " + "extra operand \'" + parsed[2] + "\'";
		throw exc;
	}
}

void Shell::showpcblist() {
	planist.display_PCB_list();
}

void Shell::showpcb() {

	std::cout << "showpcblist" << std::endl;
}

void Shell::showram() {
	
	memory.show_ram();
}

void Shell::showroot() {

	std::cout << "root" << std::endl;
}

void Shell::showtree() {


}


void Shell::showpagetable() {


}

void Shell::showqueue() {


}

void Shell::showframes() {


}

void Shell::showpagefile() {
	memory.ShowPages(2);
}


void Shell::touch() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::touch();
	}
	else if(parsed.size()==2) std::cout << "touch -tworzenie pliku" << std::endl;
	else if (parsed.size() == 1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[2] + "\'";
		throw exc;
	}
}

void Shell::rm() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::rm();
	}
	else if (parsed.size() == 2) std::cout << "rm -usuwanie pliku" << std::endl;
	else if (parsed.size() == 1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[2] + "\'";
		throw exc;
	}
}

void Shell::wf() {
		if (parsed.size() == 2 && parsed[1] == "--help") {
			Help::wf();
		}
		else if (parsed.size() == 2) std::cout << "wf -edytor pliku" << std::endl;
		else if (parsed.size() == 1) {
			std::string exc = parsed[0] + ": " + "missing operand";
			throw exc;
		}
		else {
			std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[2] + "\'";
			throw exc;
		}
}

void Shell::cat() {
	if (parsed.size() == 1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::cat();
	}
	else if (parsed.size() == 2) {
		std::cout << "Wyswietlanie plikow" << std::endl;
	}
}

void Shell::copy() {
	if (parsed.size() < 3) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else if (parsed.size() == 3) {
		std::cout << "Kopiowanie pliku" << std::endl;
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[2] + "\'";
		throw exc;
	}
}

void Shell::fileinfo() {
	if (parsed.size() == 1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::fileinfo();
	}
	else if (parsed.size() == 2) {
		std::cout << "file info" << std::endl;
	}
	else {
		std::string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
}

void Shell::go() {
	if (parsed.size() == 1) {
		std::cout << "go go go" << std::endl;
	}
	else if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::go();
	}
	else{
		std::string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
}