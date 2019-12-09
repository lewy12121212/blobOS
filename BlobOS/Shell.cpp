#include"Shell.h"
#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
#include"Help.h"

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

	if (parsed[0] == "cp") { cp(); }
	else if (parsed[0] == "dp") { dp(); }
	else if (parsed[0] == "show") { show(); }
	else if (parsed[0] == "cf") { cf(); }
	else if (parsed[0] == "df") { df(); }
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
	if ((parsed.size() == 2 && parsed[1] != "--help") || parsed.size() > 3) {
		std::string exc = "error: unsupported option";
		throw exc;
	}

	else std::cout << "cp" << std::endl;
}

void Shell::dp() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::dp();
	}
	else
		std::cout << "df" << std::endl;
}

void Shell::show() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() == 2) {
		if (parsed[1] == "-pcb") showpcb();
		else if (parsed[1] == "-pcblist") showpcblist();
		else if (parsed[1] == "-ram") showram();
		else if (parsed[1] == "-pgf") showpagefile();
		else {
			std::string exc = "error: unsupported option";
			throw exc;
		}
	}
	else if (parsed.size() == 1) {
			std::string exc = parsed[0] + ": " + "missing operand";
			throw exc;
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[1] + "\'";
		throw exc;
	}
}

void Shell::showpcblist() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() == 1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else {
		std::cout << "showpcblist" << std::endl;
	}
}

void Shell::showpcb() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() > 1) {
		std::string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		std::cout << "showpcblist" << std::endl;
	}
}

void Shell::showram() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() > 1) {
		std::string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		std::cout << "ram" << std::endl;
	}
}

void Shell::showpagefile() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() > 1) {
		std::string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		std::cout << "showpagefile" << std::endl;
	}
}

void Shell::showroot() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() > 1) {
		std::string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		std::cout << "root" << std::endl;
	}
}

void Shell::cf() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::cf();
	}
	else
		std::cout << "cf cf" << std::endl;
}

void Shell::df() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::df();
	}
	else
		std::cout << "df df" << std::endl;
}

void Shell::wf() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::wf();
	}
	else
		std::cout << "wf wf" << std::endl;
}

void Shell::fileinfo() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::fileinfo();
	}
	else if (parsed.size() > 1) {
		std::string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		std::cout << "file info" << std::endl;
	}
}

void Shell::go() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::go();
	}
	else
		std::cout << "go go go" << std::endl;
}

