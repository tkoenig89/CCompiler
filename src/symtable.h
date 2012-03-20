#ifndef _DHBWCC_SYMTABLE_H
#define _DHBWCC_SYMTABLE_H

/* Function type. */
typedef double (*func_t) (double);

//Very simple symbol table, GET IT ? :D simple symbol ... HARHAR
struct symrec
{
char *name;
int type;
union
{
  int var;
  void* func;
} value;
struct symrec *next;
};

typedef struct symrec symrec;

/* The symbol table: a chain of ‘struct symrec’. */
extern symrec *sym_table;

//Function prototypes
symrec *putsym (char const *, int);
symrec *getsym (char const *);
void init_table ();

#endif
