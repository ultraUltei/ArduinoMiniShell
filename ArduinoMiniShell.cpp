#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PIN 20

#define BIG_SIZE 32
#define M_SIZE 16
#define SM_SIZE 8

typedef struct staticPin{
  int pinNum;
  char mode;
  bool isAnalog; 
}staticPin;


//globals
staticPin* pinarray = (staticPin*)malloc(sizeof(staticPin)*MAX_PIN);
int npin = 0; // active pins

int i = 0;
int j = 0;

char* small = (char*)malloc(sizeof(char)*SM_SIZE);
char* medium = (char*)malloc(sizeof(char)*M_SIZE);
char* big = (char*)malloc(sizeof(char)*BIG_SIZE);

//strings
char prompt[] = "arduino:~$ ";

//////

//proto
void handleList();
void handeUp();
void handleDown();
void handleRead();

void printPinArray();
void setPrompt(const char* newprompt);
void shellInit();
void shellTask();
void addPin(int pinNumber, char* mode, bool analog);
void removePin(int pinNumber);
bool checkPin(int pinNumber);
//////


//functions

bool checkPin(int pinNumber){
    i = 0;
    for(; i<npin; i++) if((pinarray+i)->pinNum == pinNumber) return true;
    i = 0;
    return false;
}

void removePin(int pinNumber){
	i = 0;
   	j = -1;
    for(; i<npin; i++) if((pinarray+i)->pinNum == pinNumber){
		j = i;
     	break;
    }
  	if(j == -1) return;
  
  	for(i = j; i < npin - 1; i++) pinarray[i] = pinarray[i + 1];
  
  	i = 0;
  	j = 0;
    npin--;
}

void setPrompt(const char* newprompt)
{
    strcpy(prompt, newprompt);
}

void printPinArray(){
  for(int i=0; i<npin; i++){
    Serial.print(F("["));
    Serial.print((pinarray+i)->pinNum);
    Serial.print(F("]"));
    if(!((pinarray+i)->isAnalog)) Serial.print(F(" Digital(PWM)\t")); 
    else Serial.print(F(" Analog\t"));

	switch((pinarray+i)->mode){
      case 'I': Serial.println(F("INPUT"));
      break;
      case 'O': Serial.println(F("OUTPUT"));
      break;
      case 'P': Serial.println(F("PULLUP"));
      break;
    }     
  }
}

void addPin(int pinNumber, char* mode, bool analog){  
	if(npin == MAX_PIN) return;	 
    (pinarray+npin)->pinNum = pinNumber;
    (pinarray+npin)->mode = *mode;
    (pinarray+npin)->isAnalog = analog;
  	npin++;
}

void handleUp(){
    int i = 0;
    while (big[i] != ' ' && big[i] != '\0') i++;

    if (big[i] == '\0'){
        Serial.println("Invalid syntax. Use: UP PIN|ANALOG <pin> <mode/value>");
        return;
    }

    if (!strncmp(big, "PIN", 3) && big[3] == ' '){
        memmove(big, big + 4, strlen(big + 4) + 1); // +1 per '\0'
      
      	small = strtok(big, " ");
        if (small == NULL) {
            Serial.println("Invalid syntax. Use: UP PIN <number> <mode>");
            return;
        }
        i = atoi(small);

		if(checkPin(i))
        {
        	Serial.println("pin already active");
          	return;
        }

        small = strtok(NULL, " ");
        if (small == NULL) {
            Serial.println("Invalid syntax. Use: UP PIN <number> <mode>");
            return;
        }
        
        if (!strcmp(small, "OUTPUT")) {
            pinMode(i, OUTPUT);
            Serial.print(F("PIN "));
            Serial.print(i);
            Serial.println(F(" set to OUTPUT"));
        }
        else if (!strcmp(small, "INPUT")) {
            pinMode(i, INPUT);
            Serial.print(F("PIN "));
            Serial.print(i);
            Serial.println(F(" set to INPUT"));
        }
      	else if (!strcmp(small, "PULLUP")) {
            pinMode(i, INPUT_PULLUP);
            Serial.print(F("PIN "));
            Serial.print(i);
            Serial.println(F(" set to PULLUP"));
        }
      	digitalWrite(i, HIGH);
      	addPin(i, small, false);
      
      	i = 0;
    }
    else if (!strncmp(big, "ANALOG", 6) && big[6] == ' ') {
        memmove(big, big + 7, strlen(big + 7) + 1);     
      
        small = strtok(big, " ");
        if (small == NULL) {
            Serial.println("Invalid syntax. Use: UP ANALOG <pin> <value>");
            return;
        }
        i = atoi(small);

		if(checkPin(i))
        {
        	Serial.println("pin already active");
          	return;
        }

        small = strtok(NULL, " ");
        if (small == NULL) {
            Serial.println("Invalid syntax. Use: UP ANALOG <pin> <value>");
            return;
        }
        j = atoi(small); 	
      
      	if (j < 0) j = 0;
        if (j > 255) j = 255;

        pinMode(i, OUTPUT); 
        analogWrite(i, j);
      	addPin(i, "OUTPUT", true);
      
      	Serial.print(F("PIN "));
      	Serial.print(i);
      	Serial.println(F(" set to OUTPUT"));
        Serial.print(F("Analog value: "));
        Serial.println(j);
      
      	i = 0;
      	j = 0;
    }
    else Serial.println("Invalid syntax. Use: UP PIN|ANALOG <pin> <mode/value>");   
}

void handleDown() {
    long val = strtol(big, &small, 10);
    if (!*small){
    	i = (int)val;
        digitalWrite(i, LOW);
        Serial.print(F("PIN "));
        Serial.println(i);
        Serial.println(F("state: DOWN"));
        removePin(i);
        i = 0;
  	} else Serial.println(F("Invalid syntax. Use: DOWN <number>"));
}

void handleRead(){ 
    long val = strtol(big, &small, 10); 
    if (!*small){
      i = (int)val;   
      Serial.print(F("Reading A"));
      Serial.println(i);
      i = analogRead(i);    
      Serial.print(F("value: "));
      Serial.println(i);
      i = 0;
  	} else Serial.println(F("Invalid syntax. Use: READ <aPin>"));
}

void handleList(){
	if(!npin) Serial.println("No pin active!");
    printPinArray();
}

void shellInit() {   

  Serial.begin(115200);
  while (!Serial) { ; } // waiting serial connection

  Serial.println("\n\n");
  
  Serial.println(F("                                      +-----+"));
  Serial.println(F("         +----[PWR]-------------------| USB |--+"));
  Serial.println(F("         |                            +-----+  |"));
  Serial.println(F("         |         GND/RST2  [ ][ ]            |"));
  Serial.println(F("         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5 "));
  Serial.println(F("         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4 "));
  Serial.println(F("         |                             AREF[ ] |"));
  Serial.println(F("         |                              GND[ ] |"));
  Serial.println(F("         | [ ]N/C                    SCK/13[ ] |   B5"));
  Serial.println(F("         | [ ]IOREF                 MISO/12[ ] |   ."));
  Serial.println(F("         | [ ]RST                   MOSI/11[ ]~|   ."));
  Serial.println(F("         | [ ]3V3    +---+               10[ ]~|   ."));
  Serial.println(F("         | [ ]5v    -| A |-               9[ ]~|   ."));
  Serial.println(F("         | [ ]GND   -| R |-               8[ ] |   B0"));
  Serial.println(F("         | [ ]GND   -| D |-                    |"));
  Serial.println(F("         | [ ]Vin   -| U |-               7[ ] |   D7"));
  Serial.println(F("         |          -| I |-               6[ ]~|   ."));
  Serial.println(F("         | [ ]A0    -| N |-               5[ ]~|   ."));
  Serial.println(F("         | [ ]A1    -| O |-               4[ ] |   ."));
  Serial.println(F("         | [ ]A2     +---+           INT1/3[ ]~|   ."));
  Serial.println(F("         | [ ]A3                     INT0/2[ ] |   ."));
  Serial.println(F("         | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   ."));
  Serial.println(F("         | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0"));
  Serial.println(F("         |            [ ] [ ] [ ]              |"));
  Serial.println(F("         |  UNO_R3    GND MOSI 5V  ____________/"));
  Serial.println(F("          \\_______________________/"));
  Serial.println();
  Serial.println(F("            Arduino Shell Ready!\n"));
  Serial.print(prompt);
  
  
  Serial.flush(); 
}

char* tmp;

void shellTask() {
  
  if (Serial.available() > 0) {
    
    //clear all buffers
    memset(big, 0, strlen(big)+1);
    memset(medium, 0, M_SIZE);
    memset(small, 0, SM_SIZE);
    
    
    i = Serial.readBytesUntil('\n', big, BIG_SIZE - 1); // serial input
    big[i] = '\0';
    Serial.println(big);

    tmp = (char*)realloc(big, (i + 1) * sizeof(char));
    if (tmp) big = tmp; // resize of big buffer
    i = 0;
    
    for (; big[i]; i++) big[i] = toupper((unsigned char)big[i]); // force input upper case
    i = 0;
    
    
    i = strcspn(big, " "); // max index of first parameter in big buffer
	strncpy(medium, big, i); // copy of big buffer first parameter in medium buffer
    medium[i] = '\0';
    
    
    if (big[i] != '\0') memmove(big, big + i + 1, strlen(big) - i); // shift of big buffer values
    else big[0] = '\0';
    
	tmp = (char*)realloc(big, strlen(big)+1);
    if (tmp) big = tmp; // resize of big buffer
    i = 0;

    tmp = NULL;
    
    if (!strcmp(medium, "UP")) handleUp();
    else if(!strcmp(medium, "DOWN")) handleDown();    
    else if (!strcmp(medium, "READ")) handleRead();
    else if (!strcmp(medium, "LIST")) handleList();
    else{
      Serial.print(F("Unknown command: "));
      Serial.println(medium);
    }

    Serial.print(prompt);
  }
}




void setup()
{
	//setPrompt("yourprompt");
	shellInit();
	//setup code here
}

void loop()
{
	shellTask();
	//loop code here


	
}

