#pragma once

#include <array>
#include <string>
#include <vector>

using namespace std;

class PCB;
class Planist;

class Interpreter {

private:
	int A = 0, B = 0, C = 0, D = 0;
	unsigned int instruction_counter;
	std::string instruction;
	array<string, 4> exec_instruction;
	
	void take_from_proc(const shared_ptr<PCB>& running_proc);
	void update_proc(const shared_ptr<PCB>& running_proc);
	array<string, 4> instruction_separate(const std::string & instruction);
	int execute_instruction(std::string& instruction, shared_ptr<PCB>& running_proc);

public:

	int execute_line(const std::string& name_proc);

		
};