#include "mbed.h" // MBed library for microcontroller

DigitalOut LED_0(D2); // output array - 8 LEDs 
DigitalOut LED_1(D3);
DigitalOut LED_2(D4);
DigitalOut LED_3(D5);
DigitalOut LED_4(D6);
DigitalOut LED_5(D7);
DigitalOut LED_6(D8);
DigitalOut LED_7(D9);

Serial pc(PA_2, PA_3);  // PC serial USB interface
						//  used for testing, USB interface can't be plugged in while fan is spinning ;)

DigitalIn MagSen(D10);	// Magnetic field sensor input signal
Timer t;				// Timer to gather previous revolution's length of time
float loopTime;			// Place holder to store revious revolution's length of time gathered from timer
bool OneReset;			// Boolean to signal whether a timer has been reset
bool OnePrint;			// Boolean to signal whether one full message has been displayed completely
int printNumber;		// Each message is displayed for multiple revolutions, the higher the printNumber
						//  the longer the message will be displayed.

// char patterns array - APLHA[character][LED_array]
char ALPHA[27][8]={
    {0b00111111,0b01111111,0b11111111,0b11001100,0b11001100,0b11111111,0b01111111,0b00111111}, // 0 A
    {0b00100100,0b00111100,0b01111110,0b11011011,0b11011011,0b11011011,0b11111111,0b11111111}, // 1 B
    {0b11000011,0b11000011,0b11000011,0b11000011,0b11000011,0b11100111,0b01111110,0b00111100}, // 2 C
    {0b00111100,0b01111110,0b10111101,0b11000011,0b11000011,0b11000011,0b11111111,0b11111111}, // 3 D
    {0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11111111,0b11111111}, // 4 E
    {0b11011000,0b11011000,0b11011000,0b11011000,0b11011000,0b11011000,0b11111111,0b11111111}, // 5 F
    {0b00011111,0b11011111,0b11011000,0b11011011,0b11011011,0b11011011,0b11111111,0b11111111}, // 6 G
    {0b11111111,0b11111111,0b00011000,0b00011000,0b00011000,0b00011000,0b11111111,0b11111111}, // 7 H
    {0b11000011,0b11000011,0b11000011,0b11111111,0b11111111,0b11000011,0b11000011,0b11000011}, // 8 I
    {0b11000000,0b11000000,0b11000000,0b11111111,0b11111111,0b11000011,0b11001111,0b11001111}, // 9 J
    {0b10000001,0b11000011,0b11100111,0b01111110,0b00111100,0b00011000,0b11111111,0b11111111}, // 10 K
    {0b00000011,0b00000011,0b00000011,0b00000011,0b00000011,0b00000011,0b11111111,0b11111111}, // 11 L
    {0b11111111,0b11111111,0b01100000,0b01110000,0b01110000,0b01100000,0b11111111,0b11111111}, // 12 M
    {0b11111111,0b11111111,0b00011100,0b00111000,0b01110000,0b11100000,0b11111111,0b11111111}, // 13 N
    {0b01111110,0b11111111,0b11000011,0b11000011,0b11000011,0b11000011,0b11111111,0b01111110}, // 14 O
    {0b00100000,0b01110000,0b11111000,0b11001100,0b11001100,0b11001100,0b11111111,0b11111111}, // 15 P
    {0b01111110,0b11111111,0b11001111,0b11011111,0b11011011,0b11000011,0b11111111,0b01111110}, // 16 Q
    {0b01111001,0b11111011,0b11011111,0b11011110,0b11011100,0b11011000,0b11111111,0b11111111}, // 17 R
    {0b11001110,0b11011111,0b11011011,0b11011011,0b11011011,0b11011011,0b11111011,0b01110011}, // 18 S
    {0b11000000,0b11000000,0b11000000,0b11111111,0b11111111,0b11000000,0b11000000,0b11000000}, // 19 T
    {0b11111110,0b11111111,0b00000011,0b00000011,0b00000011,0b00000011,0b11111111,0b11111110}, // 20 U
    {0b11100000,0b11111100,0b00011110,0b00000011,0b00000011,0b00011110,0b11111100,0b11100000}, // 21 V
    {0b11111110,0b11111111,0b00000011,0b11111111,0b11111111,0b00000011,0b11111111,0b11111110}, // 22 W
    {0b01000010,0b11100111,0b01111110,0b00111100,0b00111100,0b01111110,0b11100111,0b01000010}, // 23 X
    {0b01000000,0b11100000,0b01110000,0b00111111,0b00111111,0b01110000,0b11100000,0b01000000}, // 24 Y
    {0b11000011,0b11100011,0b11110011,0b11111011,0b11011111,0b11001111,0b11000111,0b11000011}, // 25 Z
    {         0,         0,         0,         0,         0,         0,         0,         0}, // 26 (space)
    };

/* Example:

  1111
 111111 
111  111
111  111
11111111
11111111
111  111
111  111	*/

// zero all LEDs after display
void all_zero() {
    LED_0 = 0;
    LED_1 = 0;
    LED_2 = 0;
    LED_3 = 0;
    LED_4 = 0;
    LED_5 = 0;
    LED_6 = 0;
    LED_7 = 0;
}
// function to parse each LED in char pattern            
int getBit(char word, int bitNum){
    int mask = 1 << bitNum;
    int masked_n = mask & word;
    return masked_n >> bitNum;
}
// function to display each char in char pattern
void char_loop(int charSelection)
{
    float arrayWaitTime = loopTime*0.002f;
    for (int i = 7; i > -1; i--)
    {
      LED_0 = getBit(ALPHA[charSelection][i], 0); // lowest bit selected
      LED_1 = getBit(ALPHA[charSelection][i], 1);
      LED_2 = getBit(ALPHA[charSelection][i], 2);
      LED_3 = getBit(ALPHA[charSelection][i], 3);
      LED_4 = getBit(ALPHA[charSelection][i], 4);
      LED_5 = getBit(ALPHA[charSelection][i], 5);
      LED_6 = getBit(ALPHA[charSelection][i], 6);
      LED_7 = getBit(ALPHA[charSelection][i], 7); // highest bit selected 
      wait(arrayWaitTime); // give some space between each LED array
    }
    all_zero(); // clear LED array after entire character has been displayed
    wait(arrayWaitTime*2); // give some space after entire character has been displayed
}
// provide an array of characters to display
void string_loop(char ar[], int length) {
    
    for(int i = 0; i < length; i++) {
        if(ar[i] == ' ')
            char_loop(26); // choose ' ' character (space)
        else
            char_loop(ar[i]-'A'); // choose selected character from array
    }
}
// main
int main() {
	//pc.printf("%c[%i;%if", 27, (signed int)5, (signed int)20);
    //pc.printf("%f", t.read());
	
    pc.printf("%c[1;1f%c[J", 27, 27);               			// clear screen and reset cursor to beginning
    pc.printf("\tEmbedded Systems Final Project");          	// Title 
    pc.printf("%c[%i;%if", 27, (signed int)3, (signed int)0); 	// set position 3 to write in terminal
    pc.printf("Displaying:\t EMBEDDED SYSTEMS ");           	// Display image
    pc.printf("%c[%i;%if", 27, (signed int)5, (signed int)0); 	// set position 5 to write in terminal
    pc.printf("The time taken was             seconds");		// display timer time 
	// leaving this section in the while loop significantly delays the loop, this is only to test the magnetic sensor
	//  and timer count information by hand turning the fan, place this section in the while loop if doing so,
	//  you can't have a USB port connected into the microcontroller while the fan is spinning ;)
	
    // We mustn't try to display a message before the fan starts spinning. The propellor
	//  spins it's first revolution and the timer counts that length of time. Then the most 
	//  recent revolution's time needs to be constantly updated depending on the the 
	//  changing fan speed. Most recent previous revolution times are used to predict the 
	//  next revolution's time. No matter how fast the fan spins the message appears in 
	//  roughly the same position.
	
    while(true){ 
		
        if(MagSen == 1 && OneReset == false)
        {
            t.reset();
            t.start();    
            OneReset = true;
            OnePrint = false;
        }
        else if(MagSen == 0 && OneReset == true)
        {
            t.stop();
            loopTime = t.read();
            OneReset = false;
        }
        else if(MagSen == 1 && OnePrint == false)  
        {
            OnePrint = true;
            if (printNumber < 0 || printNumber > 174)
                printNumber = 0;
            switch(printNumber) {
                case 0 ... 24 :
                    string_loop("CAL STATE UNIV SAN MARCOS", 25);
                    printNumber++;
                    break;
                case 25 ... 49 :
                    string_loop("    EMBEDDED SYSTEMS     ", 25);
                    printNumber++;
                    break;
                case 50 ... 74 :
                    string_loop("      WAYLIN T OWEN      ", 25);
                    printNumber++;
                    break;
                case 75 ... 99 :
                    string_loop("    DENICE HICKETHIER    ", 25);
                    printNumber++;
                    break;
                case 100 ... 124 :
                    string_loop("       ERIC ACKLEY       ", 25);
                    printNumber++;
                    break;
                case 125 ... 149 :
                    string_loop("    DR ALI AHMADINIA     ", 25);
                    printNumber++;
                    break;
                case 150 ... 174 :
                    string_loop("  LED PROPELLER DISPLAY  ", 25);
                    printNumber++;
                    break;
					
            }//end switch
        }//end else if
    }//end forever while loop
}//end main




