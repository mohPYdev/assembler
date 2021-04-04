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
        while (fgets(line, sizeof(line), file1)) {
            char *pch;
            int tokenCounter = 0;
            pch = strtok(line , "\t");
            tokenCounter++;
            while(pch != NULL){
                if(tokenCounter == 1 || tokenCounter == 2){
                    for (int i = 0 ; i < 15 ; i++){
                        int x = strcmp(mnemonics[i] , pch);
                        if(x == 0)
                        {
                            char type = instType(pch);
                            printf("%c\n" , type);
                        }
                    }
                }
                // printf("%s\t" , pch);
                pch = strtok(NULL , "\t");
                tokenCounter++;
            }
            // printf("\n");
        }
        fclose(file1);
    }    
    return 0;
}
