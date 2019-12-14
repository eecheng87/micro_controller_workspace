#include <xc.h>
#include <pic18f4520.h>
#include <stdlib.h>

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

// Count how many times the timer1 interrupt has occured.
int count_timer_interrupt;

// Current distribution of bullets on the board.
int is_bullet_on[7][8];
volatile int player_arr[8] = {0,0,0,2,0,0,0,0};
void main(void)
{   
    // Perform configuration.
    configure_timer();
    configure_interrupt();
    
    // Initialize random seed.
    initialize_bullet();
    
    while(1);
    
    return;
}

void initialize_bullet(void)
{
    // Use current value in timer as the seed.
    int timer_value = TMR1H * 256 + TMR1L;
    // Set the seed.
    srand(timer_value);
    
    // Initialize count_timer_interrupt.
    count_timer_interrupt = 0;
    
    // Initialize to be no bullet.
    for(int i = 0; i < 7; i ++)
        for(int j = 0; j < 8; j ++)
            is_bullet_on[i][j] = 0;
}

void configure_interrupt(void)
{
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
    // high priority
    INTCON3bits.INT1IP = 1;
    
}

void configure_timer(void)
{
    // Choose internal clock, 1:1 prescaler, turn on timer1. 
    T1CON = 0x01000001;
}

void __interrupt(high_priority) HIGH_ISR(void)
{
    // When count_timer_interrupt is incremented to 20, generate new bullet.
    // When count_timer_interrupt is incremented to 5, move existing bullet.
    if ((count_timer_interrupt ++) == 20)
    {
        generate_bullet();
        
        // Reset count_timer_interrupt.
        count_timer_interrupt = 0;
    }
    else if ((count_timer_interrupt ++) == 5){
        move_bullet();
    }else if(INTCON3bits.INT1F == 1){
        // left shift
        if(player_arr[0]!=2){
            int tmp;
            for(int i = 0; i < 8; i++)
                if(player_arr[i]==2)
                    tmp = i;
            for(int i = 0; i < 8; i++)
                player_arr[i] = 0;
            player_arr[tmp-1] = 2;
        }
        INTCON3bits.INT1F == 1;
    }else if(INTCONbits.INT0IF == 1){
        // right shift
        if(player_arr[7]!=2){
            int tmp;
            for(int i = 7; i >= 0; i--)
                if(player_arr[i]==2)
                    tmp = i;
            for(int i = 0; i < 8; i++)
                player_arr[i] = 0;
            player_arr[tmp+1] = 2;
        }
        INTCONbits.INT0IF = 0;
    }
    // Clear TMR1IF.
    PIR1bits.TMR1IF = 0;
}

void generate_bullet(void)
{
    const int MAX_NUM_OF_BULLET = 5;
    int count_bullet;
    
    // Generate MAX_NUM_OF_BULLET at most.
    count_bullet = 0;
    for(int i = 0; i < 8; i ++)
    {
        if (((rand() % 10 + 1) > 5) && (count_bullet <= MAX_NUM_OF_BULLET))
        {
            is_bullet_on[0][i] = 1;
            count_bullet ++;
        }   
    }
    
    // Pass to function which turns on LED.
}

void move_bullet(void)
{
    // Move bullets one grid downward.
    for(int i = 0; i < 7; i ++)
        for(int j = 0; j < 8; j ++)
            if (is_bullet_on[i][j])
            {
                is_bullet_on[i][j] = 0;
                if (i != 6)
                    is_bullet_on[i + 1][j] = 1;
            }
    
    // Pass to function which turns on LED.
}