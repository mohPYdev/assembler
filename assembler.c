#include <stdio.h>
#include <string.h>

char* readFile(FILE *file)
{
    char text[] = " ";
    int c;
    char line[200];
    if (file) {
        while (fgets(line, sizeof(line), file)) {
                strcat(text , line);
        }
        
        fclose(file);
    }

    return text;
}


void createLabelAddress()
{


}

char* instType()
{

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
    const char *mnemonics[15] = {"add","sub","slt","or","nand","addi",
    "slti","ori","lui","lw","sw","beq","jalr","j","halt"};
    const int oppcodes[15] = {0b0000,0b0001,0b0010,0b0011,0b0100,0b0101,0b0110,
    0b0111,0b1000,0b1001,0b1010,0b1011,0b1100,0b1101,0b1110};

    FILE *file;
    file = fopen("program.as", "r");
    readFile(file);
    printf("%s" , readFile(file));    
    return 0;
}
