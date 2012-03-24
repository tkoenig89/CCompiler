#ifndef _DHBWCC_IR_CODE_GEN_H
#define _DHBWCC_IR_CODE_GEN_H
/**	ir_code_gen is partly based on http://foja.dcs.fmph.uniba.sk/kompilatory/docs/compiler.pdf
***	
**/	

//enum for 3-adress code representation:
enum code_ops 
{
	opASSIGN, opADD, opSUB, opMUL, opDIV, opMINUS, opMOD, opSHIFT_LEFT, opSHIFT_RIGHT,	
	
	opLOGICAL_AND, opLOGICAL_OR, opLOGICAL_NOT, opNE, opEQ, opGT, opGTEQ, opLS, opLSEQ,
	
	/*opIFEQ, opIFNE, opIFGT, opIFGE, opIFLT, opIFLE,*/opIF, opGOTO,
	
	opRETURN, opCALL, opMEM_LD, opMEM_ST, opADDR
};

struct strCode
{
	enum code_ops op;
	struct symInt *int0;		//Note: not all of these have to be in use for every opcode...
	struct symInt *int1;		
	struct symInt *int2;		
	struct symFunc *func;		
	const char *jmplabel;		
};

void addcodeass(struct symInt *int0, struct symInt *int1);
struct symInt *addcodeopexp1(enum code_ops operation, struct symInt *int1);
struct symInt *addcodeopexp2(enum code_ops operation, struct symInt *int1, struct symInt *int2);
void printcode();

#endif