#include "address_map_arm.h"

int HEX_Lookup(char);
char ASCII_Convert (int);

int main(void)
{
	volatile int * HEX3_HEX0_ptr		= (int *) HEX3_HEX0_BASE;
	volatile int * HEX5_HEX4_ptr		= (int *) HEX5_HEX4_BASE;
	volatile int * PS2_ptr				= (int *) PS2_BASE;
	int entries [6] = {0};
	int input;
	int ready;
	char ASCII;

	while (1)
	{
		int temp = *PS2_ptr;        // Read the data into an int as not to lose data
		if ((temp & 0x8000) == 0x8000) // Indicates there are codes not read in yet
		{
			if ((temp & 0xFF) == 0xF0 | (temp & 0xFF) == 0xE0)  //Test for key break. Note some keys have 2 breaks.
			{
				ready = 1;
			}
			else if (ready == 1)   // Else if means a check that the input is not a break is completed.
			{
				input = (temp & 0xFF);
				ready = 0;
				ASCII = ASCII_Convert(input);
				if (ASCII == 'X')
				{
					continue;       //Error occurred, don't finish this loop
				}
				else if (ASCII == 'N')
				{
					int i = 0;
					while (i < 6)
					{
						entries[i] = 0;   //Enter pressed, clear all entries
						i++;
					}
				}
				else
				{
				    temp = HEX_Lookup(ASCII);
					entries[5] = entries[4];  //Shift all entries over
					entries[4] = entries[3];
					entries[3] = entries[2];
					entries[2] = entries[1];
					entries[1] = entries[0];
					entries[0] = temp;

					temp = (entries[5] & 0xFF); //Display upper digits
					temp = temp << 8;
					temp = temp | (entries[4] & 0xFF);
					*HEX5_HEX4_ptr = temp;

					temp = (entries[3] & 0xFF); //Display lower digits
					temp = temp << 8;
					temp = temp | (entries[2] & 0xFF);
					temp = temp << 8;
					temp = temp | (entries[1] & 0xFF);
					temp = temp << 8;
					temp = temp | (entries[0] & 0xFF);
					*HEX3_HEX0_ptr = temp;
				}
			}
		}
	}
}

int HEX_Lookup(char c)
{ 
	switch(c)
	{
		case '0':
			return 0x3F;
		case '1':
			return 0x06;
		case '2':
			return 0x5B;
		case '3':
			return 0x4F;
		case '4':
			return 0x66;
		case '5':
			return 0x6D;
		case '6':
			return 0x7D;
		case '7':
			return 0x07;
		case '8':
			return 0x7F;
		case '9':
			return 0x6F;
	}		
}

char ASCII_Convert (int n)
{
		if (n == 0x70)       //0
		{
			return '0';
		}
		else if (n == 0x69)  //1
		{
			return '1';
		}
		else if (n == 0x72)  //2
		{
			return '2';
		}
		else if (n == 0x7A)  //3
		{
			return '3';
		}
		else if (n == 0x6B)  //4
		{
			return '4';
		}
		else if (n == 0x73)  //5
		{
			return '5';
		}
		else if (n == 0x74)   //6
		{
			return '6';
		}
		else if (n == 0x6C)   //7
		{
			return '7';
		}
		else if (n == 0x75)   //8
		{
			return '8';
		}
		else if (n == 0x7D)   //9
		{
			return '9';
		}
		else if (n == 0x5A)   //Enter
		{
			return 'N';
		}
		else				  //Error
		{
			return 'X';
		}
}