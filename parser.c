// Vilhelm Törmänen och Mohammed Jaber

/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "keytoktab.h"               
#include "lexer.h"                   
#include "symtab.h"                  
#include "optab.h"                   

#define DEBUG 0
static int  lookahead=0;
static int  is_parse_ok=1;

static void prog();
static void program_header();
static void var_part();
static void var_dec_list();
static void var_dec();
static void id_list();
static void type();
static void stat_part();
static void stat_list();
static void stat();
static void assign_stat();
static toktyp expr();
static toktyp term();
static toktyp factor();
static toktyp operand();

static void in(char* s)
{
    if(DEBUG) printf("\n *** In  %s", s);
}
static void out(char* s)
{
    if(DEBUG) printf("\n *** Out %s", s);
}

static void match(int t)
{
    if(DEBUG) printf("\n --------In match expected: %4d, found: %4d", t, lookahead);
    
    if (lookahead == t) {
        lookahead = get_token();
    }
    else {
        is_parse_ok=0;
        printf("\nSYNTAX:   Symbol expected %s found %s ", tok2lex(t), get_lexeme());
    }
}

static void prog()
{
    in("prog");
    program_header();
    var_part();
    stat_part();
    out("prog");
}

static void program_header()
{
    in("program_header");
    match(program); 
    
    if (lookahead == id) {
        addp_name(get_lexeme());
    }
    
    match(id); match('('); match(input);
    match(','); match(output); match(')'); match(';');
    out("program_header");
}

static void var_part()
{
    in("var_part");
    match(var);
    var_dec_list();
    out("var_part");    
}

static void var_dec_list()
{
    in("var_dec_list");
    var_dec();
    if(lookahead == id)
    {
        var_dec_list(); 
    }
    out("var_dec_list");
}

static void var_dec()
{
    in("var_dec");
    id_list(); 
    match(':'); 
    
    toktyp declared_type = lookahead; 
    type(); 
    setv_type(declared_type);
    
    match(';');
    out("var_dec");
}

static void id_list()
{
    in("id_list");
    
    if (lookahead == id) {
        char var_name[30];
        strcpy(var_name, get_lexeme());
        
        if (find_name(var_name) == 1) {
            printf("\nSEMANTIC: ID already declared: %s", var_name);
            is_parse_ok = 0;
        } else {
            addv_name(var_name);
        }
    }
    
    match(id);
    if(lookahead == ',')
    {
        match(',');
        id_list();
    }
    out("id_list");
}

static void type()
{
    in("type");
    switch(lookahead)
        {
        case integer:
            match(integer);
            break;
        case boolean:
            match(boolean);
            break;
        case real:
            match(real);
            break;
        }
    out("type");
}

static void stat_part()
{
    in("stat_part");
    match(begin);
    stat_list();
    match(end);
    match('.');
    out("stat_part");
}

static void stat_list()
{
    in("stat_list");
    stat();
    if(lookahead == ';')
    {
        match(';');
        stat_list();
    }
    out("stat_list");
}   

static void stat()
{
    in("stat");
    assign_stat();
    out("stat");
}

static void assign_stat()
{
    in("assign_stat");
    char var_name[30];
    toktyp lhs_type = error;
    
    if (lookahead == id) {
        strcpy(var_name, get_lexeme());
        if (find_name(var_name) == 0) {
            printf("\nSEMANTIC: ID NOT declared: %s", var_name);
            is_parse_ok = 0;
            lhs_type = undef;
        } else {
            lhs_type = get_ntype(var_name);
        }
    }
    
    match(id); 
    match(assign); 
    toktyp rhs_type = expr();
    
    if (lhs_type != rhs_type) {
        printf("\nSEMANTIC: Assign types: %s := %s", tok2lex(lhs_type), tok2lex(rhs_type));
        is_parse_ok = 0;
    }
    
    out("assign_stat");
}

static toktyp expr()
{
    in("expr");
    toktyp left = term();
    while (lookahead == '+')
    {
        match('+');
        toktyp right = term();
        left = get_otype('+', left, right);
    }
    out("expr");
    return left;
}

static toktyp term()
{
    in("term");
    toktyp left = factor();
    while (lookahead == '*')
    {
        match('*');
        toktyp right = factor();
        left = get_otype('*', left, right);
    }
    out("term");
    return left;
}

static toktyp factor()
{
    in("factor");
    toktyp type;
    if (lookahead == '(')
    {
        match('(');
        type = expr();
        match(')');
    }
    else
    {
        type = operand();
    }
    out("factor");
    return type;
}

static toktyp operand()
{
    in("operand");
    toktyp type = error;
    
    if (lookahead == id) {
        char var_name[30];
        strcpy(var_name, get_lexeme());
        
        if (find_name(var_name) == 0) {
            printf("\nSEMANTIC: ID NOT declared: %s", var_name);
            is_parse_ok = 0;
            type = undef;
        } else {
            type = get_ntype(var_name);
        }
        match(id);
    }
    else if (lookahead == number) {
        type = integer;
        match(number);
    }
    else {
        // Detta är kollen som fångar upp felet i fun3.pas!
        printf("\nSYNTAX:   Operand Expected ");
        is_parse_ok = 0;
        type = error;
    }
    out("operand");
    return type;
}

int parser()
{
    in("parser");
    lookahead = get_token();       
    prog();                        
    out("parser");
    
    // Hanterar skräpet som finns kvar på slutet
    if (lookahead != '$') {
        printf("\nSYNTAX:   Extra symbols after end of parse!\n          ");
        while (lookahead != '$') {
            printf("%s ", get_lexeme());
            lookahead = get_token();
        }
        printf("\n");
        is_parse_ok = 0;
    }
    
    // Om vi klarade hela testet!
    if (is_parse_ok) {
        printf("\nPARSE SUCCESSFUL! \n");
    }
    
    p_symtab();
    
    return is_parse_ok;            
}