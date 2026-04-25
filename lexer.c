/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define BUFSIZE 1024
#define LEXSIZE   30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */

/**********************************************************************/
/* PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{
    int temp;
    while((temp = fgetc(stdin)) != EOF && pbuf < BUFSIZE - 1) {
        buffer[pbuf++] = (char)temp;
    }
    buffer[pbuf] = '\0';
    pbuf = 0;
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/

static void pbuffer()
{
    printf("________________________________________________________ \n");
    printf(" THE PROGRAM TEXT\n");
    printf("________________________________________________________ \n");
    
    printf("%s", buffer);
    
    if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] != '\n') {
        printf("\n");
    }
    printf("$\n");
    
    printf("________________________________________________________ \n");
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{
    if(plex < LEXSIZE - 1 && pbuf < BUFSIZE) {
        lexbuf[plex++] = buffer[pbuf++];
    }
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/* PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{
    static int first_run = 1;
    if(first_run) {
        get_prog();
        pbuffer();
        first_run = 0;
    }
    
    plex = 0;
    memset(lexbuf, 0, sizeof(lexbuf));
    
    while(isspace(buffer[pbuf])) {
        pbuf++;
    }

    if(buffer[pbuf] == '\0' || buffer[pbuf] == '$') {
        lexbuf[0] = '$';
        lexbuf[1] = '\0';
        return lex2tok(lexbuf); 
    }
    
    if (isalpha(buffer[pbuf])) {
        while (isalnum(buffer[pbuf])){
            get_char();
        }
    }
    else if(isdigit(buffer[pbuf])) {
        while (isdigit(buffer[pbuf])) {
            get_char();
        }
    }
    else if (buffer[pbuf] == ':' && buffer[pbuf + 1] == '=') {
        get_char();
        get_char();
    }
    else {
        get_char();
    }

    return lex2tok(lexbuf); 
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{
    return lexbuf;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/