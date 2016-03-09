/*
 * TempSens.cpp
 *
 *  Created on: 2.3.2016
 *      Author: abdullai
 */

#include "TempSens.h"


TempSens::TempSens(std::string name){

	/* Setup ADC for 12-bit mode and normal power */
		Chip_ADC_Init(LPC_ADC0, 0);

		/* Setup for maximum ADC clock rate */
		Chip_ADC_SetClockRate(LPC_ADC0, ADC_MAX_SAMPLE_RATE);

		/* For ADC0, sequencer A will be used without threshold events.
		   It will be triggered manually  */
		Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, (ADC_SEQ_CTRL_CHANSEL(0) | ADC_SEQ_CTRL_CHANSEL(3) | ADC_SEQ_CTRL_MODE_EOS));

		/* For ADC0, select analog input pint for channel 0 on ADC0 */
		Chip_ADC_SetADC0Input(LPC_ADC0, 0);

		/* Use higher voltage trim for both ADC */
		Chip_ADC_SetTrim(LPC_ADC0, ADC_TRIM_VRANGE_HIGHV);

		/* Assign ADC0_0 to PIO1_8 via SWM (fixed pin) and ADC0_3 to PIO0_5 */
		Chip_SWM_EnableFixedPin(SWM_FIXED_ADC0_0);
		Chip_SWM_EnableFixedPin(SWM_FIXED_ADC0_3);

		/* Need to do a calibration after initialization and trim */
		Chip_ADC_StartCalibration(LPC_ADC0);
		while (!(Chip_ADC_IsCalibrationDone(LPC_ADC0))) {}

		/* Clear all pending interrupts and status flags */
		Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));

		/* Enable sequence A completion interrupts for ADC0 */
		Chip_ADC_EnableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);

		/* Enable related ADC NVIC interrupts */
		NVIC_EnableIRQ(ADC0_SEQA_IRQn);

		/* Enable sequencer */
		Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);

}

float TempSens::readValues()
{
	Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
	return ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC0, 0))/100;
}



