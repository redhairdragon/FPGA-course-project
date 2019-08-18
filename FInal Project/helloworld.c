/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

//#include <stdio.h>
//#include "platform.h"
//
//void print(char *str);
//
//int main()
//{
//    init_platform();
//
//    print("Hello World\n\r");
//
//    return 0;
//}

#include <stdio.h>
#include <stdlib.h>
#include <xparameters.h>
#include <xgpio.h>
#include <xuartns550_l.h>
#include <xio.h>
#include <xenv_standalone.h>
#include <xil_printf.h>
#include <xstatus.h>
#include <xuartlite_l.h>
#include <time.h>
#include "platform.h"
void print(char *str);
typedef enum {RIGHT = 1, LEFT = 3, UP = 2, DOWN = 4} FACING;

// coordinate struct
typedef struct Coordinate{
	int x;
	int y;
};

// Global Variables
FACING currentFacing = RIGHT;
struct Coordinate goal;
struct Coordinate robot;
struct Coordinate box;

void goForward(int unit) {
	return;
}
void rotateTo(FACING facing) {
	if (facing == currentFacing) {
		return;
	}
	else if (facing > currentFacing) {
		int rotateDegree = 90 * (facing - currentFacing);	// calculate the degree to rotate
		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x98);	// put the robot into script mode
		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x0D);	// script length is 13 = 0x0D
		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x89);	// put it in drive mode
		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00); XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);	// no velocity
		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00); XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x01);	// turn in place counterclockwise0

		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x9D);
		if (rotateDegree == 90) {
			XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);
			XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x5A);
		}
		else if (rotateDegree == 180) {
			XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);
			XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0xB4);
		}
		else {
			XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x01);
			XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x0E);
		}


		// the next 3 lines stop the robot
		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x89);	// put it in drive mode
		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00); XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);	// no velocity
		XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00); XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);	// turn in place counterclockwise


	}
	else {
		return;
	}

	return;
}


void exampleScript() {
//	This drives 40cm and stop
	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x96);
	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x0D);

	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x89);
	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x01);

	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x2C);
	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x80);

	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);
	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x9C);

	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x01);
	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x90);

	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x89);
	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);

	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);
	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);

	XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);

}

int main()
{
init_platform();
XUartNs550_SetBaud(XPAR_RS232_UART_0_BASEADDR, XPAR_XUARTNS550_CLOCK_HZ, 57600);
XUartNs550_SetLineControlReg(XPAR_RS232_UART_0_BASEADDR, XUN_LCR_8_DATA_BITS);
// Initialize
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x80);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x84);
// Sleep for 2 seconds
int i,j;
for (j=0;j<2;j++)
for (i=0;i<25000000;i++)
asm("nop");
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x89);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x80);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x7f);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0xff);
//XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x86); // another demo


return 0;
}
//rotateTo(UP);
// Forward

// Below is Drive
/*XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x89);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0xFF);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x38);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);*/

// Below is Drive Direct
/*XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x91);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x00);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0xff);
XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x38);*/
//XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x88); XUartNs550_SendByte(XPAR_RS232_UART_0_BASEADDR,0x03); /*This is demo*/

//exampleScript();
