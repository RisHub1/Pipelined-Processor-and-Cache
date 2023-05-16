/**
 * Project 1 
 * Assembler code fragment for LC-2K
 * //sidkaul
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<stdint.h>

#define MAXLINELENGTH 1000
#define MAXMEMORY 65536

struct l {
    char n[7];
    int set;
};

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int helper(int stay, char* use, struct l * there) {
    int i = 0;
    while (i < stay) {
        if (!strcmp(use, there[i].n))
        {
            return there[i].set;
        }
        ++i;
    }
    return -1;
}


int
main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    int count_a = 0;
    int count_l = 0;
    struct l loc[MAXMEMORY];
    int rec;
    int code;
    int place;

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    int num;
    int gh;
    int cr;
    int check = 0;
    
    for (int i = 0;readAndParse(inFilePtr,label,opcode,arg0,arg1,arg2);++i) {
        int neg = -1;
        if (neg != helper(count_a, label, loc)) {
            exit(1);
        }
        int zero = 0;
        if (zero != strlen(label))
        {
            strcpy(loc[count_a].n,label);
            loc[count_a].set = count_l;
            ++count_a;
        }
        ++count_l;
    }

    rewind(inFilePtr);

    
    for (int i = 0;readAndParse(inFilePtr,label,opcode,arg0,arg1,arg2);++i) {
        if (!strcmp(opcode, "sw") || !strcmp(opcode, "beq") || !strcmp(opcode, "lw"))
        {
            if (strcmp(opcode, "lw") && strcmp(opcode, "sw")) {
                code = 0b100;
            }
            else if (!strcmp(opcode, "sw"))
            {
                code = 0b011;
            }
            else {
                code = 0b010;
            }
            
            gh = atoi(arg1);
            cr = atoi(arg0);
            
            if (!isNumber(arg2)) {
                place = helper(count_a, arg2, loc);
                int neg = -1;
                if (neg == place) {
                    exit(1);
                }
                if (0b100 == code) {
                    int temp = place - check;
                    place = temp - 1;
                }
            }
            else {
                place = atoi(arg2);
            }
            if (place>32767 || place<-32768) {
                exit(1);
            }
            
            num = (cr<<19) + (code<<22) + (gh<<16) + (0xFFFF & place);
        }
        
        
        else if (!strcmp(opcode, "noop") || !strcmp(opcode, "halt"))
        {
            if (!strcmp(opcode, "noop")) {
                code = 0b111;
            }
            else {
                code = 0b110;
            }
            num = code<<22;
        }
        
        else if (!strcmp(opcode, ".fill")) {
            if (!isNumber(arg0))
            {
                num = helper(count_a, arg0, loc);
                int neg = -1;
                if (neg == num) {
                    exit(1);
                }
            }
            else {
                num = atoi(arg0);
            }
        }
        
        else if (!strcmp(opcode,"nor") || !strcmp(opcode, "add")) {
            if(!strcmp(opcode,"nor")) {
                code = 0b001;
            }
            else {
               code = 0b000;
            }
            rec = atoi(arg2);
            gh = atoi(arg1);
            cr = atoi(arg0);
            num = rec + (code<<22) + (cr<<19) + (gh<<16);
        }
        
        else if (!strcmp(opcode, "jalr")) {
            
            gh = atoi(arg1);
            cr = atoi(arg0);
            code = 0b101;
            num = (cr<<19) + (gh<<16) + (code<<22);
        }
        
        else {
            exit(1);
        }

        ++check;
        fprintf(outFilePtr,"%d\n",num);
    }
    
    // /* here is an example for how to use readAndParse to read a line from
    //     inFilePtr */
    // if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
    //     /* reached end of file */
    // }
    

    // /* this is how to rewind the file ptr so that you start reading from the
    //     beginning of the file */
    // rewind(inFilePtr);

    // /* after doing a readAndParse, you may want to do the following to test the
    //     opcode */
    // if (!strcmp(opcode, "add")) {
    //     /* do whatever you need to do for opcode "add" */
    // }

    return(0);
}


/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if successfully read
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
        char *arg1, char *arg2) {
    char line[MAXLINELENGTH];

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }

    /* is there a label? */
    char *ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int
isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}
