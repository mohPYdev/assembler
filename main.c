#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void createLabelAddress()
{


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

void builtFormat()
{

}

void coovertToDec(){

}

void writeToFile(){

}




int main()
{
    const char *mnemonics[16] = {"add","sub","slt","or","nand","addi",
    "slti","ori","lui","lw","sw","beq","jalr","j","halt"};
    const int oppcodes[15] = {0b0000,0b0001,0b0010,0b0011,0b0100,0b0101,0b0110,
    0b0111,0b1000,0b1001,0b1010,0b1011,0b1100,0b1101,0b1110};

    FILE *file;
    file = fopen("program.as", "r");
    char line[200];
    char firstWord[6];
    char labels[10][6];
    char addresses[10];
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
    file1 = fopen("program.as", "r");

    if (file1){
        int lineCounter = 0;
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

            bool hasLabel = false;
            //determine whether this line has a label or not!
            for (int i = 0 ; i < sizeof(addresses)/4 ; i++){
                if (lineCounter == addresses[i]) // it has a lable
                    hasLabel = true;
            }
            // printf("%d" , hasLabel);

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
                                break;
                            }
                        }          
                    }
                    else if (tokenCounter == 3){            // tokenize the registers and labels with ","
                        int cammaCounter = 0;
                        char *pch2;
                        pch2 = strtok(pch , ",");
                        cammaCounter++;
                        while(pch2 != NULL){
                           
                            switch (cammaCounter)
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
                            cammaCounter++;
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
                                break;
                            }
                        }          
                    }
                    else if (tokenCounter == 2){            // tokenize the registers and labels with ","
                        int cammaCounter = 0;
                        char *pch2;
                        pch2 = strtok(pch , ",");
                        cammaCounter++;
                        while(pch2 != NULL){
                           
                            switch (cammaCounter)
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
                            cammaCounter++;
                        }
                    }
                    pch = strtok(NULL , "\t");
                    tokenCounter++;
                }
            } // not labeled
            lineCounter++;
        }// line
        fclose(file1);
    }    
    return 0;
}
