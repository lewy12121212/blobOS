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
extern Planist planist;
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
	PTree.init(make_shared<PCB>(PCB()));
	planist.add_process(PTree.init_proc);
}

void Shell::boot() {
	logo();
	char logo;
	logo = _getch();
	while (logo != 13) {
		logo = _getch();
	}
	system("color 8");
	//std::cout<<std::endl;
	//std::cout << std::endl;
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
	int timer = 120;
	set_color(10);
	printf(R"EOF(                                                                                                                                                                                                              
                                 ```...........``                                                                       
                             `..-------------------..`                                                                  
                         ``.---------------------------.``                     
                       `.---------------------------------.`                   
                    `.---------------------------------------.`                 
                  `.-------------------------------------------.`                
                 .-----------------------------------------------.                
               `---------------------------------------------------`                                                    
              .-----------------------------------------------------.                                                   
            `--------------------------------------------------------.`                                                 
           `-------------****-------------------------****------------`                                                
          `-------------Bl--ob-----------------------Bl--ob-------------`                                               
         `-------------OS----OS---------------------OS----OS-------------`                                              
         .---------------------------------------------------------------.                                              
        `-------------------------"------------"--------------------------`                                             
        -------------------------"--------------"-------------------------.                                             
       `-------------------::::::"--------------"::::::::------------------`                                            
   `````---------------.:::::::::"--------------"::::::::::.---------------`````                                        
  ::::::----------------:::::::::::"-----------"::::::::::::---------------:::::-                                       
 `://///-----------------::::::::::::::::::::::::::::::::::----------------/////:`                                      
 .:::///:-------------------:::::::::::::::::::::::::::-------------------:///:::.                                      
  `-::::..---------------------:::::::::::::::::::::---------------------..::::-`                                       
    ```   .-------------------------:::::::::::-------------------------.   ```                                         
            .---------------------------------------------------------.                                                 
              `.---------------------------------------------------.`                                                   
                 ``.-------------------------------------------.``                                                      
                       ```....-----------------------....```                
                                            

)EOF");
	Sleep(timer);
	system("cls");


	printf(R"EOF(                                                                                                                                                                                                              
                                 ```...........``                                                                       
                             `..-------------------..`                                                                  
                         ``.---------------------------.``                        
                       `.---------------------------------.`                    
                    `.---------------------------------------.`                   
                  `.-------------------------------------------.`                 
                 .-----------------------------------------------.                
               `---------------------------------------------------`                                                    
              .-----------------------------------------------------.                                                   
            `--------------------------------------------------------.`                                                 
           `-------------****-----------------------------------------`                                                
          `-------------Bl--ob-----------------------B****b-------------`                                               
         `-------------OS----OS---------------------OS----OS-------------`                                              
         .---------------------------------------------------------------.                                              
        `-------------------------"------------"--------------------------`                                             
        -------------------------"--------------"-------------------------.                                             
       `-------------------::::::"--------------"::::::::------------------`                                            
   `````---------------.:::::::::"--------------"::::::::::.---------------`````                                        
  ::::::----------------:::::::::::"-----------"::::::::::::---------------:::::-                                       
 `://///-----------------::::::::::::::::::::::::::::::::::----------------/////:`                                      
 .:::///:-------------------:::::::::::::::::::::::::::-------------------:///:::.                                      
  `-::::..---------------------:::::::::::::::::::::---------------------..::::-`                                       
    ```   .-------------------------:::::::::::-------------------------.   ```                                         
            .---------------------------------------------------------.                                                 
              `.---------------------------------------------------.`                                                   
                 ``.-------------------------------------------.``                                                      
                       ```....-----------------------....```                
                                            

)EOF");

Sleep(timer);
system("cls");
	printf(R"EOF(                                                                                                                                                                                                              
                                 ```...........``                                                                       
                             `..-------------------..`                                                                  
                         ``.---------------------------.``                        
                       `.---------------------------------.`                     
                    `.---------------------------------------.`                   
                  `.-------------------------------------------.`                 
                 .-----------------------------------------------.                
               `---------------------------------------------------`                                                    
              .-----------------------------------------------------.                                                   
            `--------------------------------------------------------.`                                                 
           `-------------****-----------------------------------------`                                                
          `-------------Bl--ob------------------------------------------`                                               
         `-------------OS----OS---------------------OS****OS-------------`                                              
         .---------------------------------------------------------------.                                              
        `-------------------------"------------"--------------------------`                                             
        -------------------------"--------------"-------------------------.                                             
       `-------------------::::::"--------------"::::::::------------------`                                            
   `````---------------.:::::::::"--------------"::::::::::.---------------`````                                        
  ::::::----------------:::::::::::"-----------"::::::::::::---------------:::::-                                       
 `://///-----------------::::::::::::::::::::::::::::::::::----------------/////:`                                      
 .:::///:-------------------:::::::::::::::::::::::::::-------------------:///:::.                                      
  `-::::..---------------------:::::::::::::::::::::---------------------..::::-`                                       
    ```   .-------------------------:::::::::::-------------------------.   ```                                         
            .---------------------------------------------------------.                                                 
              `.---------------------------------------------------.`                                                   
                 ``.-------------------------------------------.``                                                      
                       ```....-----------------------....```                
                                            

)EOF");

	Sleep(timer);
	system("cls");
	printf(R"EOF(                                                                                                                                                                                                              
                                 ```...........``                                                                       
                             `..-------------------..`                                                                  
                         ``.---------------------------.``                        
                       `.---------------------------------.`                      
                    `.---------------------------------------.`                  
                  `.-------------------------------------------.`                 
                 .-----------------------------------------------.                
               `---------------------------------------------------`                                                    
              .-----------------------------------------------------.                                                   
            `--------------------------------------------------------.`                                                 
           `-------------****-----------------------------------------`                                                
          `-------------Bl--ob------------------------------------------`                                               
         `-------------OS----OS---------------------********-------------`                                              
         .---------------------------------------------------------------.                                              
        `-------------------------"------------"--------------------------`                                             
        -------------------------"--------------"-------------------------.                                             
       `-------------------::::::"--------------"::::::::------------------`                                            
   `````---------------.:::::::::"--------------"::::::::::.---------------`````                                        
  ::::::----------------:::::::::::"-----------"::::::::::::---------------:::::-                                       
 `://///-----------------::::::::::::::::::::::::::::::::::----------------/////:`                                      
 .:::///:-------------------:::::::::::::::::::::::::::-------------------:///:::.                                      
  `-::::..---------------------:::::::::::::::::::::---------------------..::::-`                                       
    ```   .-------------------------:::::::::::-------------------------.   ```                                         
            .---------------------------------------------------------.                                                 
              `.---------------------------------------------------.`                                                   
                 ``.-------------------------------------------.``                                                      
                       ```....-----------------------....```                
                                            

)EOF");

	Sleep(timer);
	system("cls");
printf(R"EOF(                                                                                                                                                                                                              
                                 ```...........``                                                                       
                             `..-------------------..`                                                                  
                         ``.---------------------------.``                        
                       `.---------------------------------.`                     
                    `.---------------------------------------.`                   
                  `.-------------------------------------------.`                
                 .-----------------------------------------------.              
               `---------------------------------------------------`                                                    
              .-----------------------------------------------------.                                                   
            `--------------------------------------------------------.`                                                 
           `-------------****-----------------------------------------`                                                
          `-------------Bl--ob-----------------------Bl**ob-------------`                                               
         `-------------OS----OS---------------------OS----OS-------------`                                              
         .---------------------------------------------------------------.                                              
        `-------------------------"------------"--------------------------`                                             
        -------------------------"--------------"-------------------------.                                             
       `-------------------::::::"--------------"::::::::------------------`                                            
   `````---------------.:::::::::"--------------"::::::::::.---------------`````                                        
  ::::::----------------:::::::::::"-----------"::::::::::::---------------:::::-                                       
 `://///-----------------::::::::::::::::::::::::::::::::::----------------/////:`                                      
 .:::///:-------------------:::::::::::::::::::::::::::-------------------:///:::.                                      
  `-::::..---------------------:::::::::::::::::::::---------------------..::::-`                                       
    ```   .-------------------------:::::::::::-------------------------.   ```                                         
            .---------------------------------------------------------.                                                 
              `.---------------------------------------------------.`                                                   
                 ``.-------------------------------------------.``                                                      
                       ```....-----------------------....```                
                                            

)EOF");
	Sleep(170);
	system("cls");
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
           `-------------****-------------------------****------------`                                                
          `-------------Bl--ob-----------------------Bl--ob-------------`                                               
         `-------------OS----OS---------------------OS----OS-------------`                                              
         .---------------------------------------------------------------.                                              
        `-------------------------"------------"--------------------------`                                             
        -------------------------"--------------"-------------------------.                                             
       `-------------------::::::"--------------"::::::::------------------`                                            
   `````---------------.:::::::::"--------------"::::::::::.---------------`````                                        
  ::::::----------------:::::::::::"-----------"::::::::::::---------------:::::-                                       
 `://///-----------------::::::::::::::::::::::::::::::::::----------------/////:`                                      
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
	-pcblist	display the contents of WAITING and RUNNING list
	-pagetable [PROC PID]	display the contents of page table
	-frame [NO FRAME]	display the content of frame
	-pages [PROC PID]	display the content of pages
	-tree		print the tree of processes
	-ram		display the contents of RAM
	-queue		display the contents of FIFO queue
	-pagefile	display the contents of page file

cp [PROC NAME] [FILENAME] [PARENT PID] 

dp -n [PROC NAME]
dp -p [PID]

touch [FILENAME] - A FILE argument that does not exist is created empty

open [FILENAME]

close [FILENAME]

rm [FILENAME] -remove the FILE

edit [FILENAME] - display the contain of FILE and edit the FILE in text editor

write [FILENAME] [line to add to FILE] 

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
	else if (parsed[0] == "open") { open(); }
	else if (parsed[0] == "close") { close(); }
	else if (parsed[0] == "edit") { edit(); }
	else if (parsed[0] == "write") { write(); }
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
		int pid = PTree.create_process_file(parsed[1], parsed[2], std::stoi(parsed[3]));
		if (pid != -1) {
			//planist.add_process(PTree.find_pid(PTree.init_proc, pid));	
		}
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[4] + "\'";
		throw exc;
	}
}

void Shell::dp() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::dp();
	}
	else if ((parsed.size() == 1)||(parsed.size()==2)) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else if ((parsed.size() == 3)&&(parsed[1]=="-p")) {
		PTree.kill_pid(std::stoi(parsed[2])); // dla pid
	}

	else if ((parsed.size() == 3) && (parsed[1] == "-n")) {
		PTree.kill_name(parsed[2]);
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
	else if (parsed[1] == "-disc") showdisc();
	else {
			std::string exc = "error: unsupported option";
			throw exc;
	}
}

void Shell::showpcblist() {

	if (parsed.size() == 2) {
		set_color(white);
		planist.display_PCB_list();
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}

void Shell::showpcb() {
	if (parsed.size() == 2) {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "missing operand";
		throw exc;
	}
	else if(parsed.size()==3) {
		set_color(white);
		shared_ptr<PCB> pcb_show = PTree.find_name(PTree.init_proc, (parsed[2]));
		pcb_show->show_info();
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}

void Shell::showram() {
	
	if (parsed.size() == 2) {
		set_color(white);
		memory.show_ram();
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}

void Shell::showtree() {

	if (parsed.size() == 2) {
		set_color(white);
		PTree.display_tree();
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}


void Shell::showpagetable() {

	if ((parsed.size() == 3)) {
		try {
			int znak = std::stoi(parsed[2]);
			set_color(white);
			memory.ShowPageTable(znak);
		}
		catch(const std::invalid_argument& a){
			std::cout << "Could not convert arg to PID"<<std::endl;
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
	if (parsed.size() == 2) {
		set_color(white);
		memory.ShowQueue();
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[3] + "\'";
		throw exc;
	}
}

void Shell::showframe() {
	if ((parsed.size() == 3)) {
		try {
			int znak = std::stoi(parsed[2]);
			set_color(white);
			memory.show_frame(std::stoi(parsed[2]));
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

void Shell::showpagefile() {
	if (parsed.size() == 2) {
		set_color(white);
		memory.ShowPageFile();
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[2] + "\'";
		throw exc;
	}
}

void Shell:: showpages() {

	if ((parsed.size() == 3)) {
		try {
			int znak = std::stoi(parsed[2]);
			set_color(white);
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

void Shell::showdisc() {
	if (parsed.size() == 2) {
		set_color(white);
		FM.show_disc();
	}
	else {
		std::string exc = parsed[0] + " " + parsed[1] + ": " + "extra operand \'" + parsed[2] + "\'";
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

void Shell::edit() {
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::edit();
	}
	else if (parsed.size() == 2) {
		set_color(white);
		editor(parsed[1]);
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

void Shell::cat() {
	if (parsed.size() == 1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	else if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::cat();
	}
	else if (parsed.size() == 2) { //nie dziala
		if (FM.find_file(parsed[1])>-1) {
			std::string text = FM.show_file(parsed[1]);
			set_color(white);
			std::cout << text << std::endl;
		}
		else {
			std::string exc = "File "+ parsed[1] + " does not exist";
			throw exc;
		}
	}
	else {
		std::string exc = parsed[0] + ": " + "extra operand \'" + parsed[2] + "\'";
		throw exc;
	}
}

void Shell::open() {

	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::open();
	}
	else if (parsed.size() == 2) {
		if (FM.find_file(parsed[1]) > -1) {
			FM.open_file(parsed[1]);
		}
		else {
			std::string exc = "File " + parsed[1] + " does not exist";
			throw exc;
		}
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

void Shell::close() {

	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::close();
	}
	else if (parsed.size() == 2) {
		if (FM.find_file(parsed[1]) > -1) {
			FM.close_file(parsed[1]);
		}
		else {
			std::string exc = "File " + parsed[1] + " does not exist";
			throw exc;
		}
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
		//std::cout << "go go go" << std::endl;
		set_color(white);
		interpreter.execute_line();
	}
	else if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::go();
	}
	else{
		std::string temp = parsed[0] + ": " + "extra operand" + " \'" + parsed[1] + "\'";
		throw temp;
	}
}

void Shell::editor(std::string filename){
	//Do testowania plików
	//FM.show_disc();
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
					//FM.show_disc();
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
				//FM.show_disc();
				for (auto i : tekst) {
					std::cout << i;
				}
			}
		}
	} while ((znak !=19) && (znak!=17)); //17 ^Q  19 ^S 
	if (znak == 19) {
		poczatkowy.clear();
		for (int i = 0; i < tekst.size(); i++) {
			poczatkowy.push_back(tekst[i]);
		}
		//Pozwala na edycję w konsoli bez sprawdzania zamka, sprawdza go przed zapisem i ewentualnie go pomija
		FM.edit_file(parsed[1], poczatkowy);
	}
	system("cls");
}

//dopisanie na koniec pliku
void Shell:: write() {
	if (parsed.size() == 1) {
		std::string exc = parsed[0] + ": " + "missing operand";
		throw exc;
	}
	if (parsed.size() == 2 && parsed[1] == "--help") {
		Help::write();
	}
	if (parsed.size() > 2) {
		if (FM.find_file(parsed[1]) > -1) {
			std::string temp;
			for (auto it = parsed.begin() + 2; it != parsed.end(); it++) {
				temp +=" "+ *it;
			}
			FM.edit_file_editor(parsed[1],temp);
		}
		else {
			std::string exc = "File " + parsed[1] + " does not exist";
			throw exc;
		}
	}
}