#include"Shell.h"
#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include"Help.h"
#include"File.h"
#include<windows.h>

extern FileManager FM;
extern Memory memory;
extern ProcTree PTree;
extern Interpreter interpreter;

inline void set_color(int col) {

	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, col);
}


Shell::Shell() {
	status = true;
	this->line.clear();
	this->parsed.clear();

	//Co� potem do proces�w
}

void Shell::boot() {
	logo();
	char logo;
	logo = _getch();
	while (logo != 13) {
		logo = _getch();
	}
	system("color 8");
	std::cout<<std::endl;
	std::cout << std::endl;
	loop();
}

void Shell::read_line() {
	set_color(lightGreen);
	std::cout << "$ ";
	set_color(lightAqua);
	getline(std::cin, line);
	std::transform(line.begin(), line.end(), line.begin(), ::tolower);
	if (line.size()>0) {
		while ((line.at(line.size() - 1) == ' '))	line.pop_back();
	}
	parse();
}

void Shell::parse() {

	line = line + ' ';
	std::string temp;
	for (int i = 0; i < line.size(); i++) {
		if (line[i] != ' ') temp += line[i];
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
			set_color(gray);
			std::cout << e << std::endl;
			std::cout << "Try \'" << parsed[0] << " --help\' for more information." << std::endl;
		}

		line.clear();
		parsed.clear();

	} while (status);
}

void Shell::logo() {

	set_color(5);
	printf(R"EOF(                                                                                                                                                                                                              
                                 ```...........``                                                                       
                             `..-------------------..`                                                                  
                         ``.---------------------------.``                .ooo.  -oy.         -o     .ooo.  +:/:        
                       `.---------------------------------.`              `m  -o   m-    .-.   d     s   y  m        
                    `.---------------------------------------.`            ooooo   m-   s  'o  doo\  d   m  oso+'       
                  `.-------------------------------------------.`          o  -o   m-   d. .h  h  m: h   d     ss       
                 .-----------------------------------------------.         +os+.   :oo. .ooo. .soo:  .ooo.  ooo+`       
               `---------------------------------------------------`                                                    
              .-----------------------------------------------------.                                                   
            `--------------------------------------------------------.`                                                 
           `-------------/o+---------------------------+o/-------------`                                                
          `-------------ys:+h:-----------------------:h+:ss-------------`                                               
         `-------------/h---oo-----------------------oo---h:-------------`                                              
         .---------------------------------------------------------------.                                              
        `-----------------------------------------------------------------`                                             
        --------------------------:-------------:-------------------------.                                             
       `---------------::::::::::::-------------::::::::::::---------------`                                            
   `````---------------:::///:::::::-----------:::::::///:::---------------`````                                        
  ::::::----------------:::://////::::-------:::://////::::----------------:::::-                                       
 `://///-----------------::::::://////::::::://///::::::::-----------------/////:`                                      
 .:::///:-------------------:::::::::::::::::::::::::::-------------------:///:::.                                      
  `-::::..---------------------:::::::::::::::::::::---------------------..::::-`                                       
    ```   .-------------------------:::::::::::-------------------------.   ```                                         
            .---------------------------------------------------------.                                                 
              `.---------------------------------------------------.`                                                   
                 ``.-------------------------------------------.``                                                      
                       ```....-----------------------....```                
                                            
Welcome to BlobOS. Press ENTER to start shell
)EOF");
}

void Shell::not_recognized() {
	set_color(gray);
	std::cout << parsed[0] << ": command not found" << std::endl;
}

void Shell::help() {
	set_color(white);
	printf(R"EOF(
show 
	-pcb [PROC NAME] [FILENAME] [PARENT PID]  display information about the process with this PID
	-pagetable [PROC PID]	display the contents of page table
	-frame [NO FRAME]	display the content of frame
	-pages [PROC PID]	display the content of pages
	-tree		print the tree of processes
	-ram		display the contents of RAM
	-queue		display the contents of FIFO queue
	-pagefile	display the contents of page file

cp [PROC NAME] [FILENAME] [PARENT PID] 

dp [PROC NAME]
dp [PID]

touch [FILENAME] - A FILE argument that does not exist is created empty

rm [FILENAME] -remove the FILE

wf [FILENAME] - display the contain of FILE and edit the FILE in text editor

copy [SOURCE FILENAME1] [DEST FILENAME2] - copy the contain of SOURCE to DEST

cat [FILENAME]... - concatenate FILE(s) to standard output

fileinfo [OPTION] [FILENAME] - list information about FILE

go 
ENTER

[COMMAND] --help	display help for this command

)EOF");
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
}

void Shell::clear() {
	system("cls");
}

void Shell::cp() {

	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::cp();
	}
	else if (parsed.size() == 2||parsed.size()==1 || parsed.size()==3) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else if (parsed.size() == 4) {
		// To do dadania 3 parametr (nazwa procesu, nazwa pliku, parent_pid)
		PTree.create_process_file(parsed[1], parsed[2], std::stoi(parsed[3]));
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[4] + "\'";
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

		// warunek którego nie ogarniam ~ Eryk
		PTree.kill_pid(std::stoi(parsed[1])); // dla pid
		//PTree.kill_name(parsed[1]);  // dla name 
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
	else if (parsed[1] == "-pcb") showpcb();
	else if (parsed[1] == "-pcblist") showpcblist();
	else if (parsed[1] == "-tree") showtree();
	else if (parsed[1] == "-ram") showram();
	else if (parsed[1] == "-queue") showqueue();
	else if (parsed[1] == "-pagefile") showpagefile();
	else if (parsed[1] == "-pages") showpages();
	else if (parsed[1] == "-pagetable") showpagetable(); //OK
	else if (parsed[1] == "-frame") showframe(); //OK
	else {
			std::string exc = "error: unsupported option";
			throw exc;
	}
}

void Shell::showpcblist() {
	planist.display_PCB_list();
}

void Shell::showpcb() {

	/*
	shared_ptr<PCB> pcb_show = PTree.find_name(PTree.init_proc, name);
	pcb_show->show_info();
	*/
	std::cout << "showpcblist" << std::endl;
}

void Shell::showram() {
	
	memory.show_ram();
}

void Shell::showroot() {

	std::cout << "root" << std::endl;
}

void Shell::showtree() {
	

	PTree.display_tree();
}


void Shell::showpagetable() {

	if ((parsed.size() == 3)) {
		try {
			int znak = std::stoi(parsed[2]);
			memory.ShowPageTable(znak);
		}
		catch(const std::invalid_argument& a){
			std::cout << "Wrong PID"<<std::endl;
		}
	}
	else if (parsed.size() == 2) {
		std::string exc = parsed[0] + " "+ parsed[1]+": " + "missing operand";
		throw exc;
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}

void Shell::showqueue() {
	memory.ShowQueue();

}

void Shell::showframe() {
	if ((parsed.size() == 3)) {
		memory.show_frame(std::stoi(parsed[2]));
	}
	else if (parsed.size() == 2) {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "missing operand";
		throw exc;
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}

void Shell::showpagefile() {
	memory.ShowPageFile();
}

void Shell:: showpages() {

	if ((parsed.size() == 3)) {
		try {
			int znak = std::stoi(parsed[2]);
			memory.ShowPages(znak);
		}
		catch (const std::invalid_argument & a) {
			std::cout << "Could not convert arg to PID" << std::endl;
		}
	}
	else if (parsed.size() == 2) {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "missing operand";
		throw exc;
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}

void Shell::touch() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::touch();
	}
	else if (parsed.size() == 2) {
		//std::cout << "touch -tworzenie pliku" << std::endl;
		FM.create_file(parsed[1]);
	}
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
	else if (parsed.size() == 2) {
		std::cout << "rm -usuwanie pliku" << std::endl;
		FM.delete_file(parsed[1]);
	}
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
	else if (parsed.size() == 2) editor(parsed[1]);
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
	interpreter.execute_line();
}
void Shell::editor(std::string filename){
	//Do testowania plików
	FM.show_disc();

	std::string poczatkowy = FM.show_file(parsed[1]);
		std::vector<char>tekst;
		system("cls");
		std::copy(poczatkowy.begin(), poczatkowy.end(), std::back_inserter(tekst));
		std::cout << "Press \'CTRL\' + \'S\' to exit and save file." << std::endl;
		std::cout << "Press \'CTRL\' + \'Q\' to exit without saving." << std::endl;
		for (auto i:poczatkowy) {
			std::cout << i;
		}
		unsigned char znak;
		
		do{
			znak = _getch();
		
			if((znak !=19) && (znak!=17)) {
				if (znak == 8) {
					if (tekst.size() > 0) {
						system("cls");
						tekst.pop_back();
						std::cout << "Press \'CTRL\' + \'S\' to exit and save file." << std::endl;
						std::cout << "Press \'CTRL\' + \'Q\' to exit without saving." << std::endl;
						//Do testowania plików
						FM.show_disc();
						for (auto i : tekst) {
							std::cout << i;
						}
					
					}
				}
				else if (znak == 13) {
					tekst.push_back('\n');
					std::cout << std::endl;
				}
				else {
					tekst.push_back(znak);
					system("cls");
					std::cout << "Press \'CTRL\' + \'S\' to exit and save file." << std::endl;
					std::cout << "Press \'CTRL\' + \'Q\' to exit without saving." << std::endl;
					//Do testowania plików
					FM.show_disc();
					for (auto i : tekst) {
						std::cout << i;
					}
				}
			}
		} while ((znak !=19) && (znak!=17)); //17 ^Q  19 ^S 

		//Tu chyba na razie brakuje opcji zapis/bez zapisu więc po prostu poczotkowy to tekst zapisany w pliku
		//który potem jest zmieniany edytorem, poczotkowy jest nadpisywany i przesyłany do zapisu
		
		

		poczatkowy.clear();
		for (int i = 0; i < tekst.size();i++) {
			poczatkowy.push_back(tekst[i]);
		}
		FM.edit_file(parsed[1],poczatkowy);
}