#include<reg51.h>
#define lcd P1
sbit rs=P3^6;
sbit e=P3^7;
sbit led=P0^0;
sbit s1=P2^0;
sbit s2=P2^1;
void delay (int);
void cmd (char);
void display (char);
void init (void);
void string (char *);
void view (int);
int count=0;
int no[10]={48,49,50,51,52,53,54,55,56,57};
void delay (int d)
{
	unsigned char i=0;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);
		for(i=248;i>0;i--);
	}
}
void cmd (char c) //Writing Commands on LCD
{
	lcd=c;
	rs=0; //Selects command register
	e=1; //Enable information on LCD datalines
	delay(5);
	e=0;
}
void display (char c)
{
	lcd=c;
	rs=1; //Selects data register
	e=1; //Enable information on LCD datalines
	delay(5);
	e=0;
}
void string (char *p) //Function to display a string
{
	while(*p)
	{
		display(*p++);
	}
}
void view (int n)
{
	cmd(0xc0); // Force cursor to beginning of second line
	//Separates 2-digit number and displays 
	display(no[(n/10)%10]);
	display(no[n%10]);
}
void init (void)
{
	cmd(0x38); //2 lines and 5x7 Matrix
	cmd(0x0c); //Display On, Cursor Off
	cmd(0x01); //Clear display screen
	cmd(0x80); //Force cursor to beginning of first line
}


void Ext_int_Init()				
{
	EA  = 1;	// Enable global interrupt
	EX0 = 1;      	// Enable Ext. interrupt0
	IT0 = 1;      	// Select Ext. interrupt0 on falling edge
}
											
void External0_ISR() interrupt 0
{
	count=0;	
	string("light off");
	led=1;
	delay(100);
	view(count);
} 

void main()
{
  	led=0; //Declaring output pin
  	init(); //Initializing LCD
  	Ext_int_Init(); //Initializing Interrupts
	string("People in room");
	cmd(0xc0); //Forcing cursor to beginning of second line 
	
	view(count);
	
	
	while(1)
	{
		if(s1==1) //IR sensor 1 detects person entering
		{
			while(s2==0); //Waiting for IR sensor 2 to detect person
			count=count+1; //Incrementing counter
			while(s2==1); //Waiting for person to fully enter room
			view(count); //Displaying updated count
		}
		else if(s2==1) //IR sensor 2 detects person leaving room
		{
			while(s1==0); //Waiting for IR sensor 1 to detect person
			if(count!=0)
			count=count-1; //Decrementing counter
			while(s1==1); //Waiting for person to fully leave room
			view(count); //Displaying updated count
		}
		else if(count>=1) //Light turned on if people in room
			led=0;
		else if(count==0) //Light turned off if no one in room
			led=1;
		
	}
}