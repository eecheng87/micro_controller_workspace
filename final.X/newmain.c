#include <xc.h>
#include <pic18f4520.h>
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// **********************************************************************************************************

char mystring[20];
int lenStr = 0;
int score = 0;
// UART function.
void UART_Initialize(void);
char * GetString();
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void ClearBuffer();
void MyusartRead();

// Interrupt service routine.
void __interrupt(high_priority) HIGH_ISR(void);

// Configure interrupt.
void configure_interrupt(void);
// Configure timer.
void configure_timer(void);

// Initialize the seed of random number generator.
void initialize_bullet(void);

// Operation on bullets.
void generate_bullet(void);
void move_bullet(void);

//Display array
void shine(int x, int y, int color);

// Count how many times the timer1 interrupt has occured.
volatile int count_timer_interrupt_1;
volatile int count_timer_interrupt_2;

// Current distribution of bullets on the board.
int is_bullet_on[7][8];
int count = 0;
volatile int player_arr[8] = { 0,0,0,0,2,0,0,0 };

// For restarting game.
int is_player_dead = 0;
int result = 0;

void main(void)
{    
	// Initialize.
	TRISD = 0;
	TRISC = 0;
	TRISB = 3;
	TRISE = 0;
	TRISA = 0;
	PORTD = 0;
	PORTC = 255;
    PORTBbits.RB5 = 1;
    PORTBbits.RB4 = 1;
    PORTBbits.RB3 = 1;
    PORTBbits.RB2 = 1;
	PORTE = 3;
	PORTA = 55;
    
	// Perform configuration.
    UART_Initialize();
	configure_timer();
	configure_interrupt();
	initialize_bullet();

	while (1)
	{
        if(strstr(GetString(), "r") != NULL){
            result = 0;
            score = 0;
            is_player_dead = 0;
            PORTAbits.RA3 = 0;
            ClearBuffer();
        }
        
        if (is_player_dead == 1){
            PORTAbits.RA3 = 1;
            if(result == 0){
     
                UART_Write_Text("\n       ****************************               \n");
                UART_Write(13);
                UART_Write_Text("        **                        **      |\\__||  \n");
                UART_Write(13);
                UART_Write_Text("        **  Congratulations !!    **      / x.x  | \n");
                UART_Write(13);
                UART_Write_Text("        **                        **    /_____   | \n");
                UART_Write(13);
                UART_Write_Text("        **  Game is over   !!     **   /^ ^ ^ \\  |\n");
                UART_Write(13);
                UART_Write_Text("        **                        **  |^ ^ ^ ^ |w| \n");
                UART_Write(13);
                UART_Write_Text("        ****************************   \\m___m__|_|\n");
                UART_Write(13);
                char str[10];
                UART_Write_Text("        Your score is : ");
                sprintf(str, "%d", score);
                UART_Write_Text(str);
                UART_Write_Text("\n");
                UART_Write(13);
                
                ClearBuffer();
                result = 1;
                
            }        
        }
        
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				shine(i, j, is_bullet_on[i][j]);

		for (int i = 0; i < 8; ++i)
			shine(7, i, player_arr[i]);
	}
}

void initialize_bullet(void)
{
	// Use current value in timer as the seed.
	int timer_value = TMR1H * 256 + TMR1L;
	// Set the seed.
	srand(timer_value);

	// Initialize count_timer_interrupt.
	count_timer_interrupt_1 = 0;
	count_timer_interrupt_2 = 0;

	// Initialize to be no bullet.
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 8; j++)
			is_bullet_on[i][j] = 0;
}

void configure_interrupt(void)
{
	OSCCONbits.IRCF = 7;
	ADCON1bits.PCFG = 15;

	// Set GIE and IPEN.
	RCONbits.IPEN = 1;
	INTCONbits.GIE = 1;

	// Clear TMR1IF.
	PIR1bits.TMR1IF = 0;

	// Enable TMR1IE.
	PIE1bits.TMR1IE = 1;

	// Enables the external interrupt
	INTCONbits.INT0IE = 1;
	INTCONbits.INT0IF = 0;
	INTCON3bits.INT1E = 1;
	INTCON3bits.INT1F = 0;
    INTCON2bits.INTEDG0 = 0;
	
    // high priority
	//INTCON3bits.INT1IP = 1;
}

void configure_timer(void)
{
	// Choose internal clock, 1:1 prescaler, turn on timer1. 
	T1CON = 0x01000001;
}

void __interrupt(high_priority) HIGH_ISR(void)
{
	PIE1bits.TMR1IE = 0;

	// When count_timer_interrupt is incremented to 40, generate new bullet.
	// When count_timer_interrupt is incremented to 20, move existing bullet.
	if (PIR1bits.TMR1IF == 1)
	{
		if (count_timer_interrupt_1 == 40)
		{
            if(is_player_dead == 0)
                generate_bullet();
			count_timer_interrupt_1 = 0;
		}
		if (count_timer_interrupt_2 == 20)
		{
            if(is_player_dead == 0)
                move_bullet();
			count_timer_interrupt_2 = 0;
		}

		count_timer_interrupt_1++;
		count_timer_interrupt_2++;
	}


	if (INTCON3bits.INT1F == 1) {
		// left shift
		if (player_arr[0] != 2) {
			int tmp;
			for (int i = 0; i < 8; i++)
				if (player_arr[i] == 2)
					tmp = i;
			for (int i = 0; i < 8; i++)
				player_arr[i] = 0;
			player_arr[tmp - 1] = 2;
		}
		
	}
    
	if (INTCONbits.INT0IF == 1) {
		// right shift
		if (player_arr[7] != 2) {
			int tmp;
			for (int i = 7; i >= 0; i--)
				if (player_arr[i] == 2)
					tmp = i;
			for (int i = 0; i < 8; i++)
				player_arr[i] = 0;
			player_arr[tmp + 1] = 2;
		}
		
	}
    
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
        MyusartRead();
    }

	PIR1bits.TMR1IF = 0;
    INTCON3bits.INT1F = 0;
	PIE1bits.TMR1IE = 1;
    INTCONbits.INT0IF = 0;
}

void generate_bullet(void)
{
	const int MAX_NUM_OF_BULLET = 1;
	int count_bullet;
	int position;
    score++;
	position = rand() % 7;
	is_bullet_on[0][position] = 1;
}

void move_bullet(void)
{
    // Check collision.
    for(int i = 0; i < 8; i ++)
        if ((is_bullet_on[6][i] == 1) && (player_arr[i] == 2))
            is_player_dead = 1;
                
    // Move the bullet.
	for (int i = 5; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			is_bullet_on[i + 1][j] = is_bullet_on[i][j];
			is_bullet_on[i][j] = 0;
		}
	} 
}

void shine(int x, int y, int color)
{
	//RED
	if (color == 1)
	{
		switch (x)
		{
		case 0:
			PORTDbits.RD0 = 1;
			break;
		case 1:
			PORTDbits.RD1 = 1;
			break;
		case 2:
			PORTDbits.RD2 = 1;
			break;
		case 3:
			PORTDbits.RD3 = 1;
			break;
		case 4:
			PORTDbits.RD4 = 1;
			break;
		case 5:
			PORTDbits.RD5 = 1;
			break;
		case 6:
			PORTDbits.RD6 = 1;
			break;
		case 7:
			PORTDbits.RD7 = 1;
			break;
		default:
			NOP();
			break;
		}
		switch (y)
		{
		case 0:
			PORTCbits.RC0 = 0;
			break;
		case 1:
			PORTCbits.RC1 = 0;
			break;
		case 2:
			PORTCbits.RC2 = 0;
			break;
		case 3:
			PORTCbits.RC3 = 0;
			break;
		case 4:
			PORTCbits.RC4 = 0;
			break;
		case 5:
			PORTCbits.RC5 = 0;
			break;
		case 6:
			PORTAbits.RA4 = 0;
			break;
		case 7:
			PORTAbits.RA5 = 0;
			break;
		default:
			NOP();
			break;
		}
	}
	//Yellow
	else if (color == 2)
	{
		switch (x)
		{
		case 0:
			PORTDbits.RD0 = 1;
			break;
		case 1:
			PORTDbits.RD1 = 1;
			break;
		case 2:
			PORTDbits.RD2 = 1;
			break;
		case 3:
			PORTDbits.RD3 = 1;
			break;
		case 4:
			PORTDbits.RD4 = 1;
			break;
		case 5:
			PORTDbits.RD5 = 1;
			break;
		case 6:
			PORTDbits.RD6 = 1;
			break;
		case 7:
			PORTDbits.RD7 = 1;
			break;
		default:
			NOP();
			break;
		}
		switch (y)
		{
		case 0:
			PORTEbits.RE0 = 0;
			break;
		case 1:
			PORTEbits.RE1 = 0;
			break;
		case 2:
			PORTBbits.RB2 = 0;
			break;
		case 3:
			PORTBbits.RB3 = 0;
			break;
		case 4:
			PORTBbits.RB4 = 0;
			break;
		case 5:
			PORTAbits.RA2 = 0;
			break;
		case 6:
			PORTAbits.RA0 = 0;
			break;
		case 7:
			PORTAbits.RA1 = 0;
			break;
		default:
			NOP();
			break;
		}
	}
	//Dark
	else
	{
		switch (x)
		{
		case 0:
			PORTDbits.RD0 = 0;
			break;
		case 1:
			PORTDbits.RD1 = 0;
			break;
		case 2:
			PORTDbits.RD2 = 0;
			break;
		case 3:
			PORTDbits.RD3 = 0;
			break;
		case 4:
			PORTDbits.RD4 = 0;
			break;
		case 5:
			PORTDbits.RD5 = 0;
			break;
		case 6:
			PORTDbits.RD6 = 0;
			break;
		case 7:
			PORTDbits.RD7 = 0;
			break;
		default:
			NOP();
			break;
		}
		switch (y)
		{
		case 0:
			PORTEbits.RE0 = 1;
			PORTCbits.RC0 = 1;
			break;
		case 1:
			PORTEbits.RE1 = 1;
			PORTCbits.RC1 = 1;
			break;
		case 2:
			PORTBbits.RB2 = 1;
			PORTCbits.RC2 = 1;
			break;
		case 3:
			PORTBbits.RB3 = 1;
			PORTCbits.RC3 = 1;
			break;
		case 4:
			PORTBbits.RB4 = 1;
			PORTCbits.RC4 = 1;
			break;
		case 5:
			PORTAbits.RA2 = 1;
			PORTCbits.RC5 = 1;
			break;
		case 6:
			PORTAbits.RA0 = 1;
			PORTAbits.RA4 = 1;
			break;
		case 7:
			PORTAbits.RA1 = 1;
			PORTAbits.RA5 = 1;
			break;
		default:
			NOP();
			break;
		}
	}
}

// -----------------
void UART_Initialize() {    
    
    TRISCbits.TRISC6 = 0;       // TX for output
    TRISCbits.TRISC7 = 1;       // RX for input
    
    TXSTAbits.SYNC = 0;    
    
    BAUDCONbits.BRG16 = 0;       
    TXSTAbits.BRGH = 0;
    SPBRG = 12;
    
    RCSTAbits.SPEN = 1;              //open serial port
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;         //Enable Tx
    RCSTAbits.CREN = 1;        //Enable Rx
    
    // setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //disable Tx interrupt
    IPR1bits.TXIP = 0;              //Setting Tx as low priority interrupt
    PIE1bits.RCIE = 1;              //Enable Rx interrupt
    IPR1bits.RCIP = 0;              //Setting Rc as low priority interrupt
}

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}

char *GetString(){  
    return mystring;
}

void UART_Write_Text(char* text) { 
    for(int i=0;text[i]!='\0';i++){
        UART_Write(text[i]);
        //for(int j = 0; j < 1000; ++j);
    }
}

void ClearBuffer(){
    for(int i = 0; i < 10 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void MyusartRead()
{
    mystring[lenStr] = RCREG;
    UART_Write(mystring[lenStr]);
    lenStr++;
    lenStr %= 10;

    return ;
}



