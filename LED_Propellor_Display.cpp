#include "mbed.h" // MBed library for microcontroller

DigitalOut LED_0(D2); // output array - 8 LEDs 
DigitalOut LED_1(D3);
DigitalOut LED_2(D4);
DigitalOut LED_3(D5);
DigitalOut LED_4(D6);
DigitalOut LED_5(D7);
DigitalOut LED_6(D8);
DigitalOut LED_7(D9);

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
// display one verticale bar to test timing
void test_bar_lights() {
    LED_0 = 1;
    LED_1 = 1;
    LED_2 = 1;
    LED_3 = 1;
    LED_4 = 1;
    LED_5 = 1;
    LED_6 = 1;
    LED_7 = 1;
    wait(0.0004f);
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
// function to display each char  
void char_loop(int charSelection)
{
    for(int i = 8; i > 0; i--)
    {
      LED_0 = getBit(ALPHA[charSelection][i], 0); // lowest bit selected
      LED_1 = getBit(ALPHA[charSelection][i], 1);
      LED_2 = getBit(ALPHA[charSelection][i], 2);
      LED_3 = getBit(ALPHA[charSelection][i], 3);
      LED_4 = getBit(ALPHA[charSelection][i], 4);
      LED_5 = getBit(ALPHA[charSelection][i], 5);
      LED_6 = getBit(ALPHA[charSelection][i], 6);
      LED_7 = getBit(ALPHA[charSelection][i], 7); // highest bit selected 
      wait(0.0004f); // give some space between each LED array
    }
    all_zero(); // clear LED array after entire character has been displayed
    wait(0.001f); // give some space after entire character has been displayed
}
// provide an array of characters to display
void string_loop(char ar[], int length) {
    
    for(int i = 0; i < length; i++) {
        if(ar[i] == ' ')
            char_loop(26);
        else
            char_loop(ar[i]-'A');
    }
}
// main
int main() {
   
    while(true){ 
        for(int i = 0; i < 50; i++) {
            string_loop("EMBEDDED SYSTEMS PROJECT      ", 30);
            wait(0.012f); // wait for 30 chars
        }
        wait(0.5f);
        for(int i = 0; i < 30; i++) {
            string_loop("LED PROPELLER DISPLAY         ", 30);
            wait(0.012f); // wait for 30 chars
        }
        wait(0.5f);
        for(int i = 0; i < 30; i++) {
            string_loop("PERSISTANCE OF VISION         ", 30);
            wait(0.012f); // wait for 30 chars
        }
        wait(0.5f);
        for(int i = 0; i < 30; i++) {
            string_loop("WAYLIN OWEN                   ", 30);
            wait(0.012f); // wait for 30 chars
        }
        wait(0.5f);
        for(int i = 0; i < 30; i++) {
            string_loop("DENICE HICKETHIER             ", 30);
            wait(0.012f); // wait for 30 chars
        }
        wait(0.5f);
        for(int i = 0; i < 30; i++) {
            string_loop("ERIC ACKLEY                   ", 30);
            wait(0.012f); // wait for 30 chars
        }
        wait(0.5f);
        for(int i = 0; i < 30; i++) {
            string_loop("PROFESSOR ALI AHMADINIA       ", 30);
            wait(0.012f); // wait for 30 chars
        }
        wait(0.5f);
		
        //string_loop("EMBEDDED SYSTEM", 15);
        //wait(0.075f); // wait for about 15 chars
        
        //string_loop("EMBEDDED S", 10);
        //wait(0.1f); // wait for about 10 chars
        
        //string_loop("EMBED", 5); 
        //wait(0.12659f); // wait for about 5 chars
        
        //char_loop("A", 1);
        //wait(0.141f); // wait for about one char
        
        //test_bar_lights();
        //wait(0.168f); // wait for about a vertical bar
    }
}