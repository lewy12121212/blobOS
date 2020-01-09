#include "Interpreter.h"
#include <iostream>
#include "process.h"
#include "procesor.h"
#include "File.h"


using namespace std;

extern FileManager FM;
extern ProcTree PTree;
extern Memory memory;

Interpreter interpeter;

void Interpreter::take_from_proc(const shared_ptr<PCB>& running_proc)
{
	instruction_counter = running_proc->register_copy[4];
	A = running_proc->register_copy[0];
	B = running_proc->register_copy[1];
	C = running_proc->register_copy[2];
	D = running_proc->register_copy[3];

}

void Interpreter::update_proc(const shared_ptr<PCB>& running_proc)
{
	running_proc->register_copy[4] = instruction_counter;
	running_proc->register_copy[0] = A;
	running_proc->register_copy[1] = B;
	running_proc->register_copy[2] = C;
	running_proc->register_copy[3] = D;
}


array<string, 4> Interpreter::instruction_separate(const std::string & instruction)
{
	string tmp;
	array<string, 4> command;
	int i = 0;

	for (const char& x : instruction) {
		if (x != ' ' || x == ';') {
			tmp += x;
		}
		else if (!tmp.empty())
		{
			command[i] = tmp;
			tmp.clear();
			i++;
		}
	}

	return command;
}


void Interpreter::display_registers()
{

	cout << "Registers: " << endl;
	cout << "A:  " << "  " << A << "B:  " << B << "  " << "C:  " << C << "  " << "D:  " << D << endl;
	cout << "Instruction counter:  " << instruction_counter << endl;

}


int Interpreter::execute_instruction(std::string& instruction, shared_ptr<PCB>& running_proc)
{
	

	//tutaj bêd¹ pobierane rejestry itd, szukanie procesu i wrzucenie go do instrukcji 
	exec_instruction = instruction_separate(instruction);
	int i = 0;
	int adres = 0;

	char val;

	int *rej1=0;
	int *rej2=0;
	int counter=0;



	string command;
	string file_name;
	string text;

	command = exec_instruction[0];


	if (!exec_instruction[1].empty()) {
		if (exec_instruction[1] == "A") { rej1 = &A; }
		else if (exec_instruction[1] == "B") { rej1 = &B; }
		else if (exec_instruction[1] == "C") { rej1 = &C; }
		else if (exec_instruction[1] == "D") { counter = D; }
		else if (exec_instruction[1] == "[") {
			
			exec_instruction[1].pop_back();
			exec_instruction[1].erase(exec_instruction[1].begin());

			adres = stoi(exec_instruction[1]);

		}
		else if (exec_instruction[1] == "(") {

			exec_instruction[1].pop_back();
			exec_instruction[1].erase(exec_instruction[1].begin());

			file_name=exec_instruction[1];

		}
		else {
			//val = exec_instruction[1].c_str;
			i = stoi(exec_instruction[1]);
			*rej1 = i;
		}
	}

	if (!exec_instruction[2].empty()) {
		if (exec_instruction[2] == "A") { rej2 = &A; }
		else if (exec_instruction[2] == "B") { rej2 = &B; }
		else if (exec_instruction[2] == "C") { rej2 = &C; }
		else if (exec_instruction[2] == "D") { counter = D; }
		else if (exec_instruction[2] == "[") {
			
			exec_instruction[2].pop_back();
			exec_instruction[2].erase(exec_instruction[2].begin());

			adres = stoi(exec_instruction[2]);

		}
		else if (exec_instruction[1] == "(") {

			exec_instruction[1].pop_back();
			exec_instruction[1].erase(exec_instruction[1].begin());

			text = exec_instruction[1];

		}
		else {
			//val = exec_instruction[2].c_str;
			//bla
			i = stoi(exec_instruction[2]);
			*rej2 = i;
		}
	}

	//tu s¹ ogarniane rozkazy 
	if (command == "AD") { *rej1 += *rej2; }
	else if (command == "SB") { *rej1 -= *rej2; }
	else if (command == "ML") { *rej1 *= *rej2; }
	else if (command == "DV") { *rej1 /= *rej2; }
	else if (command == "MD") {
		if (rej2 == 0) {
			cout << "Dzielenie przez 0!" << endl;
			*rej1 = *rej1 % *rej2;
		}
	}
	else if (command == "IN") { *rej1++; }
	else if (command == "DE") { *rej1--; }
	else if (command == "MV") { *rej1 = *rej2; }
	else if (command == "WR") {
		//tutaj pisanko do pamiêci
		memory.write_to_ram(adres, &val);
	}
	else if (command == "GT") {
		//pobieranko z pamiêci
		memory.get_frame(adres);
	}
	else if (command == "JP") {
		instruction_counter = adres;
	}
	else if (command == "JZ") {
		if (counter == 0)
			instruction_counter = adres;
	}
	else if (command == "JN") {
		if (counter != 0)
			instruction_counter = adres;
	}
	else if (command == "MF") {

		FM.create_file(file_name);
		//tu poleci coœ od plików
	}
	else if (command == "OF") {

		//tu te¿
	}
	else if (command == "WF") {

		FM.save_data_to_file(file_name, text);
		//i tu
	}
	else if (command == "AF") {
		FM.edit_file(file_name, text);
	}
	else if (command == "RF") {

		FM.show_file(file_name);
		//kurwa ile jeszcze 
	}
	else if (command == "CF") {
		//dobra koniec xd	
	}
	else if (command == "CP") {
		PTree.create_process_file(file_name, text, running_proc->pid);
		//tu teoretycznie moge stworzyc proces
	}
	else if (command == "KP") {
		//a tu go zabic 
		running_proc->kill();
		//blaa
	}
	else if (command == "HT") {

		running_proc->kill();
		return -1;
	}
	else if (command == "NN") {

	}



	return 1;
}

int Interpreter::execute_line(const std::string& name_proc) //czy na pewno nazwa?? 
{
	shared_ptr<PCB> running_proc = planist.ReadyPCB.front();
	take_from_proc(running_proc);
	execute_instruction(instruction, running_proc);
	update_proc(running_proc);

	return 0;
}


