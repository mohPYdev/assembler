#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


// constant variables for our mnemonics and their opcodes in the instruction set.
const char *mnemonics[16] = {"add","sub","slt","or","nand","addi",
    "slti","ori","lui","lw","sw","beq","jalr","j","halt"};

const int oppcodes[15] = {0,1,2,3,4,5,6,
    7,8,9,10,11,12,13,14};


int lenOfLabels(char* fileAdd){

    FILE *file3;
    file3 = fopen(fileAdd, "r");
    char line[200];
    char firstWord[6];
    int labelCounter = 0;
    int lineCounter = 0;
    if (file3) {
        while (fgets(line, sizeof(line), file3)) {  
            firstWord[0] = '\0';
            int i = 0;
            while(line[i] != '\t'){
                strncat(firstWord , &line[i] , 1);
                i++;
            }
            for(int j = 0 ; j < 15 ; j++){
                int x = strcmp(firstWord,mnemonics[j]);
                if(x == 0){
                    labelCounter++;
                    break;
                }
            }
            lineCounter++;               
        }
        fclose(file3);
    }

    return lineCounter - labelCounter;
}


char instType(char* instruction)
{   
    char type ;
    
    if (strcmp(instruction , "add") == 0 || strcmp(instruction , "sub") == 0|| strcmp(instruction , "slt") == 0 || strcmp(instruction , "or") == 0 || strcmp(instruction , "nand") == 0)
        type = 'R';
    else if (strcmp(instruction , "j") == 0|| strcmp(instruction , "halt") == 0)
        type = 'J';
    else
        type = 'I';

    return type;
}


long builtFormat(char type , int opcode , int f1 , int f2 , int f3)
{

    unsigned long number = opcode;

    switch (type)
    {
    case 'R':

        number = number << 4;
        number += f2;
        number = number << 4;
        number += f3;
        number = number << 4;
        number += f1;
        number = number << 12; 
        break;
    case 'I':

        number = number << 4;
        number += f2;
        number = number << 4;
        number += f1;
        number = number << 16;
        number += f3;
        break;
    case 'J':
        number  = number << 8;
        number = number << 16;
        number += f1;
        break;
    }

    return number;

}


void writeToFile(char* fileWAdd , long number){
    char *numToS;
    long a = number;
    itoa(a , numToS , 10);
    strcat(numToS , "\n");    
    FILE *filee;
    filee = fopen(fileWAdd , "a");
    fputs(numToS , filee);
    fclose(filee);
}



int main(int argc , char* argv[])
{


    int labelSize = lenOfLabels(argv[1]);

    FILE *file;
    file = fopen(argv[1], "r");
    char line[200];
    char firstWord[6];
    char labels[labelSize+1][6];
    char addresses[labelSize+1];
    int lineCounter = 0;
    int z = 0;
    if (file) {
        while (fgets(line, sizeof(line), file)) {  
            firstWord[0] = '\0';
            int i = 0;
            while(line[i] != '\t'){
                strncat(firstWord , &line[i] , 1);
                i++;
            }
            bool inThere = false;
            for(int j = 0 ; j < 15 ; j++){
                int x = strcmp(firstWord,mnemonics[j]);
                if(x == 0){
                    inThere = true;
                    break;
                }
            }


            if (inThere == false){
                strcpy(labels[z] , firstWord);
                addresses[z] = lineCounter;
                z++;  
            }   

            lineCounter++;              
        }
        fclose(file);

    }// now we have the symbolic table for labels and addresses.


    FILE *file1;
    file1 = fopen(argv[1], "r");

    if (file1){
        int lineCounter = 0;
        long number;
        while (fgets(line, sizeof(line), file1)) {
            int tokenCounter = 0;
            char *pch;
            pch = strtok(line , "\t");
            tokenCounter++;
            int opcode;
            char *instruction;  
            char type;
            char *field1;
            char *field2;
            char *field3;
            int f1 , f2 , f3;
            bool isDirective = true;

            bool hasLabel = false;
            //determine whether this line has a label or not!
            for (int i = 0 ; i < 5 ; i++){
                if (lineCounter == addresses[i]) // it has a label
                    hasLabel = true;
            }
            
      
            if (hasLabel){
                while(pch != NULL){
                    if(tokenCounter == 2){                  // get the opcode / instruction / type

  
                        for (int i = 0 ; i < 15 ; i++){
                            int x = strcmp(mnemonics[i] , pch);
                            if(x == 0)
                            {
                                opcode = oppcodes[i];
                                instruction = pch;
                                type = instType(pch);
                                isDirective = false;
                                break;
                            }
                        }          
                    }
                    else if (tokenCounter == 3){            // tokenize the registers and labels with ","
                        
     

                        if (isDirective){                       /// handling the directives
                            int dirlabelAddress = -1;
                            for(int i = 0 ; i < labelSize ; i++){
                                if (strcmp(pch , labels[i]) == 0 ){
                                    dirlabelAddress = addresses[i];
                                    break; 
                                }
                            }
                            if (dirlabelAddress == -1)
                                number = atoi(pch);
                            else
                                number = dirlabelAddress;
                            break;
                        }

                        if (type == 'J'){ 
                            field3 = pch;
                            break; 
                        }

                        int commaCounter = 0;
                        char *pch2;
                        pch2 = strtok(pch , ",");
                        commaCounter++;
                        while(pch2 != NULL){
                           
                            switch (commaCounter)
                            {
                            case 1:
                                field1 = pch2;
                                break;
                            case 2:
                                field2 = pch2;
                                break;
                            case 3:
                                field3 = pch2;
                                break;
                            }
                            
                            pch2 = strtok(NULL , ",");
                            commaCounter++;
                        }
                    }
                    pch = strtok(NULL , "\t");
                    tokenCounter++;
                }
            }

            else{   // if the line doesn't have a label
                        
                while(pch != NULL){
                    if(tokenCounter == 1){                  // get the opcode / instruction / type
                        for (int i = 0 ; i < 15 ; i++){
                            int x = strcmp(mnemonics[i] , pch);
                            if(x == 0)
                            {
                                opcode = oppcodes[i];
                                instruction = pch;
                                type = instType(pch);
                                isDirective= false;
                                break;
                            }
                        }          
                    }
                    else if (tokenCounter == 2){            // tokenize the registers and labels with ","
                        
                        
                        if (isDirective){              /// handling the directives
                            int dirlabelAddress = -1;
                            for(int i = 0 ; i < labelSize ; i++){
                                if (strcmp(pch , labels[i]) == 0 ){
                                    dirlabelAddress = addresses[i];
                                    break; 
                                }
                            }
                            if (dirlabelAddress == -1)
                                number = atoi(pch);
                            else
                                number = dirlabelAddress;
                            break;
                        }

                        if (type == 'J'){ 
                            field3 = pch;
                            break; 
                        }
                        int commaCounter = 0;
                        char *pch2;
                        pch2 = strtok(pch , ",");
                        commaCounter++;
                        while(pch2 != NULL){
                           
                            switch (commaCounter)
                            {
                            case 1:
                                field1 = pch2;
                                break;
                            case 2:
                                field2 = pch2;
                                break;
                            case 3:
                                field3 = pch2;
                                break;
                            }
                            
                            pch2 = strtok(NULL , ",");
                            commaCounter++;
                        }
                    }
                    pch = strtok(NULL , "\t");
                    tokenCounter++;
                }
            } // not labeled

            if (!isDirective){
                f1 = atoi(field1);
                f2 = atoi(field2);
                f3 = atoi(field3);


                // find the offset for the labels in I type
                for (int i = 0 ; i < labelSize ; i++){
                    if (strcmp(field3 , labels[i]) == 0){
                        f3 = addresses[i];
                    }
                }


                switch(opcode){                     // handling the exceptions in formatting the fields
                    case 8:             //lui
                        f3 = f2;
                        f2 = 0;
                        break;
                    case 12:            // jalr
                        f3 = 0;
                        break;
                    case 13:
                        f1 = f3;
                        f2 = 0 , f3 = 0 ;
                        break;
                    case 14:
                        f1 = 0 , f2 = 0 , f3 = 0;
                }

                number = builtFormat(type , opcode , f1 , f2 , f3);
            }
            
            // printf("%li\n" , number);
            writeToFile(argv[2] , number);
            lineCounter++;
        }// line
        fclose(file1);
    }    
    return 0;
}
