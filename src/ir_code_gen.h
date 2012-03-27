#ifndef _DHBWCC_IR_CODE_GEN_H
#define _DHBWCC_IR_CODE_GEN_H
/**	ir_code_gen is partly based on http://foja.dcs.fmph.uniba.sk/kompilatory/docs/compiler.pdf
***	
**/	

//enum for 3-adress code representation:
enum code_ops 
{
	opASSIGN, opADD, opSUB, opMUL, opMINUS, opSHIFT_LEFT, opSHIFT_RIGHT,	
	
	opLOGICAL_AND, opLOGICAL_OR, opLOGICAL_NOT, opNE, opEQ, opGT, opGTEQ, opLS, opLSEQ,
	
	opIF, opGOTO,
	
	opRETURN, opPARAM, opCALL, opMEM_LD, opMEM_ST, opADDR, opFUNC_DEF
};

static char* enumStrings[] = {
							"ASSIGN", "ADD", "SUB", "MUL", "MINUS", "SHIFT_LEFT", "SHIFT_RIGHT",	
	
							"LOGICAL_AND", "LOGICAL_OR", "LOGICAL_NOT", "NE", "EQ", "GT", "GTEQ", "LS", "LSEQ",
	
							"IF", "GOTO",

							"RETURN", "PARAM", "CALL", "MEM_LD", "MEM_ST", "ADDR", "FUNC_DEF"
						
						};

struct strCode
{
	enum code_ops op;
	struct symInt *int0;		//Note: not all of these have to be in use for every opcode...
	struct symInt *int1;		
	struct symInt *int2;		
	struct symFunc *func;		
	int jmpTo;
};

void addcodeass(struct symInt *int0, struct symInt *int1);
struct symInt *addcodeopexp1(enum code_ops operation, struct symInt *int1);
struct symInt *addcodeopexp2(enum code_ops operation, struct symInt *int1, struct symInt *int2);
void addcodeopfunc(enum code_ops operation, struct symInt *int0, struct symFunc *func, int jmpTo);
void addcodeop1(enum code_ops operation, struct symInt *int0);
void addcodeop2(enum code_ops operation, struct symInt *int0, struct symInt *int1);
int opcodeFindFunctionDef(struct symFunc *func);

void printcode();
void debugPrintAllopcodes();

struct strCode  *getopcodeArray();
int getopcodeCount();

#endif