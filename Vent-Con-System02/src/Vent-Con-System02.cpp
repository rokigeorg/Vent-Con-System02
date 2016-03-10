/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#include <cstring>
#include <cstdio>


#include "ModbusMaster.h"
#include "LiquidCrystal.h"
#include "lcd_port.h"
#include "DecimalEdit.h"
#include "MenuItem.h"
#include "PropertyEdit.h"
#include "SimpleMenu.h"
#include "DebouncedInput.h"

#include "TempEdit.h"
#include "PresEdit.h"
#include "SensorGeneral.h"
#include "TempSens.h"
#include "PresSens.h"
#include "AutoEdit.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/


/*****************************************************************************
 * Public functions
 ****************************************************************************/
static volatile int counter;
static volatile uint32_t systicks;
static volatile int systicks2;
static int dPort[4] = {0,0,1,0};
static int dPin[4] = {10,16,3,0};
static volatile bool tickFlag = false;
static volatile bool checkFlag = false;
static volatile bool AutoDispFlag = false;

volatile bool TempSensFlag = false;
volatile bool PresSensFlag = false;
volatile bool AutoFlag = true;


static volatile uint32_t TempCount;
/*
volatile bool TempFlag;
volatile bool PresFlag;
*/

static volatile int ctr = 0;

//it checks for the measerments if it reache the proper amount
static volatile uint32_t checktick;

//Auto counter to change the display of the screen

static volatile uint32_t AutoTimer;

// TODO: insert other definitions and declarations here

#define TICKRATE_HZ (1000)	/* 100 ticks per second */

#ifdef __cplusplus
extern "C" {
#endif


void ADC0A_IRQHandler(void)
{
	uint32_t pending;

	// Get pending interrupts /
	pending = Chip_ADC_GetFlags(LPC_ADC0);

	// Sequence A completion interrupt /
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		//TempFlag = true;
	}

	// Clear any pending interrupts /
	Chip_ADC_ClearFlags(LPC_ADC0, pending);
}


void RIT_IRQHandler(void)
{
	// Acknowledge interrupt by clearing RIT interrupt flag
	Chip_RIT_ClearIntStatus(LPC_RITIMER);
	// disable RIT � we want a single interrupt after the wait is over
	Chip_RIT_Disable(LPC_RITIMER);
	// set a flag to notify main program
	Flag = true;


}
void SysTick_Handler(void)
{

	TempCount++;
	checktick++;
	systicks2++;
	AutoTimer++;
	//Flag=true;
	if(counter > 0) {
		counter-=1;
	}

	if(AutoTimer%3000 >= 0)
	{
		AutoDispFlag = true;
	}

	if(checktick >= 10000)
	{
		checkFlag=true;
		checktick = 0;
	}
	//We dont need this, this should be implemented in  the specific class and a virtual method read sens should call this
	if (TempCount >= TICKRATE_HZ / 100) {
		TempCount = 0;
		Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
	}
	//end of the crap
}
#ifdef __cplusplus
}
#endif

unsigned int GetSysticks()
{
	return systicks2;
}


void Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}
/* this function is required by the modbus library */
uint32_t millis() {
	return systicks;
}



int isPressed(){
	int btnKeyNum =0;
	//create digi
	static DebouncedInput dInput4(4,100);
	static DebouncedInput dInput5(5,100);
	static DebouncedInput dInput6(6,100);
	static DebouncedInput dInput7(7,100);

	static bool dArray[4];

	dArray[0] = dInput4.read();
	dArray[1] = dInput5.read();
	dArray[2] = dInput6.read();
	dArray[3] = dInput7.read();

	for(int i =0;i < 4; i++){
		if(dArray[i]){
			btnKeyNum = i+1;
			checktick = 0;
		}
	}

	return btnKeyNum;
}

void printRegister(ModbusMaster& node, uint16_t reg) {
	uint8_t result;
	// slave: read 16-bit registers starting at reg to RX buffer
	result = node.readHoldingRegisters(reg, 1);

	// do something with data if read is successful
	if (result == node.ku8MBSuccess)
	{
		printf("R%d=%04X\n", reg, node.getResponseBuffer(0));
	}
	else {
		printf("R%d=???\n", reg);
	}
}


/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */

void check(SensorGeneral *s, PropertyEdit *p, PropertyEdit *setFrq)
{

	p->setSensValue(s->readValues());

	if(p->getValue() < (p->getDesValue() - p->getTol()))
		setFrq->decrement();
	else if(p->getValue() > (p->getDesValue() - p->getTol()))
		setFrq->increment();


	//needs to be uncommented when working with ModBus
	//setFrq->save();
}


int main(void)
{

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	// button registers set up
	for(int i=0; i<4;i++){
		Chip_IOCON_PinMuxSet(LPC_IOCON, dPort[i], dPin[i], (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, dPort[i], dPin[i]);
	}

	Chip_RIT_Init(LPC_RITIMER);
	NVIC_EnableIRQ(RITIMER_IRQn);
	ModbusMaster node(2); // Create modbus object that connects to slave id 2

	/*

	 node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here


	node.writeSingleRegister(0, 0x0406); // prepare for starting

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well


	node.writeSingleRegister(0, 0x047F); // set drive to start mode


	Sleep(4000);
*/

	LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

	lcd.begin(16,2);
	lcd.setCursor(0,0);
	SimpleMenu menu;

	DecimalEdit FreqEditObj(lcd, node, std::string("Frequency"),20000,0);
	TempEdit TempEditObj(lcd, node, std::string("Temperature"),35,0);
	PresEdit PresEditObj(lcd, node, std::string("Pressure"),100,0);
	AutoEdit AutoEditObj(lcd, node, std::string("Welcome"),35,0,22);

	PropertyEdit *freqEditPoint;
	PropertyEdit *tempEditPoint;
	PropertyEdit *presEditPoint;
	PropertyEdit *autoEditPoint;


	TempSens TempSensObj;
	PresSens PresSensObj(0, 100000);

	SensorGeneral *tempSensPoint;
	SensorGeneral *presSensPoint;


	presSensPoint = &PresSensObj;
	tempSensPoint = &TempSensObj;

	presEditPoint = &PresEditObj;
	tempEditPoint = &TempEditObj;
	freqEditPoint = &FreqEditObj;
	autoEditPoint = &AutoEditObj;


	presEditPoint->setValue(22);
	tempEditPoint->setValue(22);
	autoEditPoint->setValue(22);

	menu.addItem(new MenuItem(AutoEditObj));
	menu.addItem(new MenuItem(TempEditObj));
	menu.addItem(new MenuItem(PresEditObj));
	menu.addItem(new MenuItem(FreqEditObj));

	menu.event(MenuItem::show); // display first menu item


	volatile static int k;
	while(1){

		//printRegister(node, 3); // for keeping the modbus on (change to an interupt)
		k = isPressed();
		if(k > 0) {
			switch(k){
			case 1:
				menu.event(MenuItem::up);
				break;
			case 2:
				menu.event(MenuItem::down);
				break;
			case 3:
				menu.event(MenuItem::back);
				break;
			case 4:
				menu.event(MenuItem::ok);
				break;
			}
		}

		 //check for the checkflag if true set it false and write down the functionality for checking

		if(checkFlag)
		{
			checkFlag = false;
			if(TempSensFlag)
				check(tempSensPoint, tempEditPoint, freqEditPoint);
			else if(PresSensFlag)
				check(presSensPoint, presEditPoint, freqEditPoint);
		}


		if(AutoDispFlag)
		{
			if(AutoEditObj.getTitle()!="Auto")
			{
				switch (AutoTimer/3000) {
				case 0:
					break;
				case 1:
					AutoEditObj.setTitle("Welcome .");
					break;
				case 2:
					AutoEditObj.setTitle("Welcome ..");
					break;
				case 3:
					AutoEditObj.setTitle("Welcome ...");
					break;
				case 4:
					AutoEditObj.setTitle("Welcome ....");
					break;
				case 5:
					AutoEditObj.setTitle("Welcome .....");
					break;
				case 6:
					AutoEditObj.setTitle("Auto");
					break;
				}
			}
		}

	}

	return 1;
}

