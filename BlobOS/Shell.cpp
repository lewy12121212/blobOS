#include"Shell.h"
#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
#include"Help.h"

using namespace std;

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
	cout << "$ ";
	getline(cin, line);
	parse();
}

void Shell::parse() {

	line = line + ' ';

	string temp;
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
		catch (string & e) {
			cout << e << endl;
			cout << "Try \'" << parsed[0] << " --help\' for more information." << endl;
		}

		line.clear();
		parsed.clear();

	} while (status);
}

void Shell::not_recognized() {
	cout << parsed[0] << ": command not found" << endl;
}

void Shell::help() {
	cout << "Help" << endl;

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
	else if (parsed[0] == "cls") { cls(); }
	else if (parsed[0] == "exit") { exit(); }
	else not_recognized();

}

void Shell::exit() {
	status = false;
	cout << "Koniec" << endl;
}

void Shell::cls() {
	system("cls");
	cout << "Czysto" << endl;
}

void Shell::cp() {

	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::cp();
	}
	if ((parsed.size() == 2 && parsed[1] != "--help") || parsed.size() > 3) {
		string exc = "error: unsupported option";
		throw exc;
	}

	else cout << "cp" << endl;
}

void Shell::dp() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::dp();
	}
	else
		cout << "df" << endl;
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
			string exc = "error: unsupported option";
			throw exc;
		}
	}
	else if (parsed.size() == 1) {
			string exc = parsed[0] + ": " + "missing operand";
			throw exc;
	}
	else {
		string exc = parsed[0] + ": " + "extra operand \'" + parsed[1] + "\'";
		throw exc;
	}
}

void Shell::showpcblist() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() == 1) {
		string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else {
		cout << "showpcblist" << endl;
	}
}

void Shell::showpcb() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() > 1) {
		string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		cout << "showpcblist" << endl;
	}
}

void Shell::showram() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() > 1) {
		string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		cout << "ram" << endl;
	}
}

void Shell::showpagefile() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() > 1) {
		string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		cout << "showpagefile" << endl;
	}
}

void Shell::showroot() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::show();
	}
	else if (parsed.size() > 1) {
		string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		cout << "root" << endl;
	}
}

void Shell::cf() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::cf();
	}
	else
		cout << "cf cf" << endl;
}

void Shell::df() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::df();
	}
	else
		cout << "df df" << endl;
}

void Shell::wf() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::wf();
	}
	else
		cout << "wf wf" << endl;
}

void Shell::fileinfo() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::fileinfo();
	}
	else if (parsed.size() > 1) {
		string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
	else {
		cout << "file info" << endl;
	}
}

void Shell::go() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::go();
	}
	else
		cout << "go go go" << endl;
}

