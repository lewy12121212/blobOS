#include "Help.h"
#include"Shell.h"
#include <iostream>

void Help::cp() {
	set_color(white);
	std::cout << "cp [PROC NAME] [FILENAME] [PARENT PID]" << std::endl;
}

void Help::dp() {
	set_color(white);
	std::cout << "dp [PROC NAME]" << std::endl;
	std::cout << "dp [PID]" << std::endl;
}

void Help::show() {
	set_color(white);
	printf(R"EOF(
Usage: show [OPTION]
Show the information about OPTION.

	-pcb [PROC NAME] [FILENAME] [PARENT PID]  display information about the process with this PID
	-pcblist	display pcb lists: READYPCB and WAITINGPCB
	-tree		print the tree of processes
	-ram		display the contents of RAM
	-pagetable	display the contents of page table
	-queue		display the contents of FIFO queue
	-frames		display the content of frames
	-pagefile	display the contents of page file

Example:
	show -pcb 11	Information about the process PID 11. 

)EOF");
}


void Help::touch() {

	set_color(white);
	printf(R"EOF(
Usage: touch [FILENAME]

A FILE argument that does not exist is created empty. 

	--help	display this help and exit

)EOF");
}

void Help::rm() {
	set_color(white);
	printf(R"EOF(
Usage: rm [FILENAME]

Remove the FILE.

	--help	display this help and exit

)EOF");
}

void Help::wf() {
	set_color(white);
	printf(R"EOF(
Usage: wf [FILENAME]

Display the contain of FILE and edit the FILE in text editor.

	--help	display this help and exit

)EOF");
}

void Help::copy() {
	set_color(white);
	printf(R"EOF(
Usage: copy [SOURCE FILENAME1] [DEST FILENAME2]

Copy the contain of SOURCE to DEST.

	--help	display this help and exit

)EOF");
}

void Help::cat() {
	set_color(white);
	printf(R"EOF(
Usage: cat [FILENAME]...

Concatenate FILE(s) to standard output.

With no FILE read standard input. // Mo¿e zrobie

	--help	display this help and exit

)EOF");
}

void Help::fileinfo() {
	set_color(white);
	printf(R"EOF(
Usage: fileinfo [OPTION] [FILENAME]

List information about FILE.

	--all	list information about all FILEs in the directory

	--help	display this help and exit

)EOF");
}

void Help::go() {
	set_color(white);
	std::cout << "Musze pomyslec jak tutaj napisac";
}