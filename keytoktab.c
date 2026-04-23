/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
}tab;

/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
    {"id", 	            id},
    {"number",      number},
    {":=", 	        assign},
    {"undef", 	     undef},
    {"predef",      predef},
    {"tempty",      tempty},
    {"error",        error},
    {"type",           typ},
    {"$",              '$'},
    {"(",              '('},
    {")",              ')'},
    {"*",              '*'},
    {"+",              '+'},
    {",",              ','},
    {"-",              '-'},
    {".",              '.'},
    {"/",              '/'},
    {":",              ':'},
    {";",              ';'},
    {"=",              '='},
    {"TERROR", 	    nfound}
};


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	      input},
	{"output", 	     output},
	{"var", 	        var},
	{"begin", 	      begin},
	{"end", 	        end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	       real},
	{"KERROR", 	     nfound}
};

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{
    //Skriver ut Token-tabell
    printf("\n *** Tokentable");
    for (int i = 0; tokentab[i].token != nfound; i++) {
        printf(" %s\t%d\n", tokentab[i].text, tokentab[i].token);
    }

    //skriver ut Nyckelord-tabell
    printf("\n *** Keywordtable");
    for (int i = 0; keywordtab[i].token != nfound; i++) {
        printf(" %s\t%d\n", keywordtab[i].text, keywordtab[i].token);
    }
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
    //Går igenom taken-tabellen från början
    for (int i = 0; tokentab[i].token != nfound; i++) {
        
        //Jämför ordet med texten i tabellen
        if (strcmp(fplex, tokentab[i].text) == 0) {
            
            // Ingen skillnad, returnera tokenet.
            return tokentab[i].token;
        }
    }
    printf("\n *** Error", fplex);  return 0;
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{   //Gå igenom keyword tabellen från början
    for (int i = 0; keywordtab[i].token != nfound; i++)
    
    //jämför ordet med texten i tabellen
    if (strcmp(fplex, keywordtab[i].text) == 0) {

        //Ingen skillnad, returnera tokenet.
        return keywordtab[i].token;
    }
    printf("\n *** Error");  return 0;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
    //Leta igenom token tabellen.
    for (int i = 0; tokentab[i].token != nfound; i++)
        if (tokentab[i].token == ftok) {

            //Hittat, returnera texten.
            return tokentab[i].text;
        }
    
        //leta vidare i keyword tabellen.
    for (int i = 0; keywordtab[i].token != nfound; i++){
        if (keywordtab[i].token == ftok) {

            //Hittat, returnera text.
            return keywordtab[i].text;
        }
    }
    printf("\n *** ERROR");  return 0;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
