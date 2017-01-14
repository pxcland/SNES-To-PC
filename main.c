#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <wchar.h>
#include <stdint.h>
#include <Windows.h>

typedef union
{
	uint16_t state_word;
     
	struct
	{
		uint8_t high;
		uint8_t low;
	} state_byte;
          
} buttonState_t;

int main(void)
{
	HANDLE	hCOM;				/* Serial port handle */
	DCB		serialFormat = {0};	/* DCB parameters for serial port */
	BOOL	status;				/* Port status */
	COMMTIMEOUTS timeout = {0};	/* timeout parameters for port */
	DWORD	bytesRead;			/* counter for bytes read from serial port */
	INPUT	in[12];				/* Input buffer for simulating keypress */	
	int		portNumber;			/* Serial port number to listen on */
	wchar_t	portPath[50];		/* Path name for windows to open */
	int i;						/* General loop counter */

	buttonState_t buttonState = {0xFFFF};	/* button state which is received over serial port */
	buttonState_t prevState = {0xFFFF};
	
	printf("Enter port to listen on: ");
	scanf("%d",&portNumber);
	swprintf(portPath,50,L"\\\\.\\COM%d",portNumber);

	printf("Attempting to open COM%d...\n",portNumber);
	hCOM = CreateFile(	portPath,
						GENERIC_READ,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);

	if(hCOM == INVALID_HANDLE_VALUE)
	{
		printf("Cannot open COM%d!\nExiting.\n",portNumber);
		CloseHandle(hCOM);
		return 1;
	}
	else
		printf("Successfully opened COM%d.\n",portNumber);


	serialFormat.DCBlength = sizeof(serialFormat);
	status = GetCommState(hCOM,&serialFormat);
	
	serialFormat.BaudRate = CBR_9600;
	serialFormat.ByteSize = 8;
	serialFormat.StopBits = ONESTOPBIT;
	serialFormat.Parity = NOPARITY;

	if(!SetCommState(hCOM,&serialFormat))
	{
		printf("Cannot set up parameters for COM%d!\nExiting.\n",portNumber);
		CloseHandle(hCOM);
		return 1;
	}

	timeout.ReadIntervalTimeout = 100;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 10;

	if(!SetCommTimeouts(hCOM,&timeout))
	{
		printf("Cannot set up timeout settings for COM%d!\nExiting.\n",portNumber);
		CloseHandle(hCOM);
		return 1;
	}

	for(i = 0; i < 12; i++)
	{
		in[i].type = INPUT_KEYBOARD;
		in[i].ki.time = 0;
		in[i].ki.wVk = 0;
		in[i].ki.dwExtraInfo = 0;
		in[i].ki.dwFlags = KEYEVENTF_SCANCODE;
		in[i].ki.wScan = MapVirtualKey(0x41 + i, MAPVK_VK_TO_VSC);
	}

	while(1)
	{
		prevState = buttonState;
		ReadFile(	hCOM,
					&buttonState.state_word,
					2,
					&bytesRead,
					NULL);

		for(i = 0; i < 12; i++)
		{
			if(!(buttonState.state_word & (1 << i)))
			{
				/* If previous state is not pressed, and current is pressed, simulate a keypress */
				if(prevState.state_word & (1 << i))
				{
					in[i].ki.dwFlags = KEYEVENTF_SCANCODE;
					SendInput(1,&in[i],sizeof(INPUT));
				}
			}
			else if(buttonState.state_word & (1 << i))
			{
				/* If previous state is pressed, and current is not pressed, simulate a key release */
				if(!(prevState.state_word & (1 << i)))
				{
					in[i].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
					SendInput(1,&in[i],sizeof(INPUT));
				}
			}
		}

	}

	CloseHandle(hCOM);
	return 0;

}