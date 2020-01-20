// Set of independent routines to setup all of the interrupts supported by the hardware
// as configured.

#include <stdio.h>
#include "interrupts.h"
#include "xscugic.h"                  // Includes for the interrupt controller.
#include "xscutimer.h"                // Includes for the private timer of the ARM.
#include "xsysmon.h"                  // Includes for the system monitor (contains the XADC).
#include "leds.h"        // Easy LED access functions can be found here.
#include "globalTimer.h" // global timer routines aid in measuring time.

#ifdef ENABLE_INTERVAL_TIMER_0_IN_TIMER_ISR
#include "intervalTimer.h"
#endif

//static queue_t debugAdcQueue;
//#define ADC_QUEUE_SIZE 100000
//
//void initAdcQueue() {
//  queue_init(&debugAdcQueue, ADC_QUEUE_SIZE);
//}
//
//queue_data_t interrupts_popAdcQueueData() {
//	return queue_pop(&debugAdcQueue);
//}
//
//bool interrupts_adcQueueEmpty() {
//	return queue_empty(&debugAdcQueue);
//}
//
//queue_size_t interrupts_adcQueueElementCount() {
//	return queue_elementCount(&debugAdcQueue);
//}


// The sysmon runs off the bus-clock when accessed via the AXI_XADC IP.
// This default will allow nearly a 26 Mhz clock which is the maximum frequency to achieve 1 megasamples
// because a single conversion requires 26 clock cycles.

// Set this constant to determine the period between interrupts.
// The private timer clock is 1/2 the processor frequency, default processor freq.
// for ZYBO is 650 MHz. The default is set for timer interrupts to occur at 100 kHz rate.
#define ZYBO_BUS_CLOCK (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define PRIVATE_TIMER_PRESCALER_DEFAULT 0
#define PRIVATE_TIMER_LOAD_VALUE_DEFAULT 3249  // Provides a 10 us interrupt period.
//#define PRIVATE_TIMER_LOAD_VALUE_DEFAULT 324     // Provides a 1 us interrupt period.
#define PRIVATE_TIMER_TICKS_PER_HEART_BEAT PRIVATE_TIMER_TICKS_PER_SECOND / 8
#define PRIVATE_TIMER_TICKS_PER_ADC_SAMPLE 1
//#define PRIVATE_TIMER_TICKS_PER_ADC_SAMPLE 10

// ****************** These #defines enable/disable certain functionality ********************************

#define INTERRUPTS_ENABLE_HEARTBEAT_LED     // Comment out to disable the LED heart beat.
#define HEARTBEAT_TOGGLES_PER_SECOND 8     // How many times the LED LD4 heartbeat toggle off and on per second.
#define INTERRUPTS_ENABLE_ADC_DATA_CAPTURE  // Comment out to disable ADC sample capture to queue.

// ****************** end of #define enable/disable section **********************************************

// ******************* Place the various handles and instance pointers here. *****************************
static XScuGic_Config *GicConfig;    // The configuration parameters of the controller.
static XScuGic InterruptController;  // Pointer to the Xilinx-provided interrupt controller routine.
static XScuTimer_Config *ConfigPtr;  // Pointer to the ARM private timer.
static XScuTimer TimerInstance;      // The timer instance (allows access to timer registers).
static XSysMon_Config *xSysMonConfig;// Handle to the SysMon.
static XSysMon xSysMonInst;          // Instance of the system monitor (to access AXI_XADC registers).

// *********************************** Globals Start ****************************************
volatile int interrupts_isrFlagGlobal = 0;
// *********************************** Globals End   ****************************************

// The sysmon (XADC) runs off the bus-clock when accessed via the AXI_XADC IP (as is done here).
// This default will allow nearly a 26 Mhz clock which is the maximum frequency to achieve 1 megasamples
// because a single conversion requires 26 clock cycles.
#define XADC_CLOCK_DIVIDER 4  // 100 MHz bus clock divided by 4 is 25 MHz.


// *********************** Place globals (to this file) and their accessors here *************************

u32 heartBeatTimer = 0;                                           // Used to blink an LED while the program is running.
u32 isrInvocationCount = 0;                                       // Keep track of number of times ISR is called.

u32 privateTimerPrescaler = PRIVATE_TIMER_PRESCALER_DEFAULT;      // Keep track of the private-timer prescaler value
u32 privateTimerLoadValue = PRIVATE_TIMER_LOAD_VALUE_DEFAULT;     // Keep track of the private-timer load value.
// For convenience, compute the number of ticks per second based upon the above values.
u32 privateTimerTicksPerSecond = ZYBO_BUS_CLOCK /((privateTimerPrescaler+1) * (privateTimerLoadValue+1));
// Compute the number of ticks per heart-beat toggle.
u32 privateTimerTicksPerHeartbeat = ZYBO_BUS_CLOCK /((privateTimerPrescaler+1) * (privateTimerLoadValue+1));

// User can set the load value on the private timer.
// Also updates ticks per heart beat so that the LD4 heart-beat toggle rate remains constant.
void interrupts_setPrivateTimerLoadValue(u32 loadValue) {
  XScuTimer_LoadTimer(&TimerInstance, loadValue);
  privateTimerLoadValue = loadValue;
  // Formula derived from the ARM documentation on the private timer (4.1.1)
  privateTimerTicksPerHeartbeat = (ZYBO_BUS_CLOCK /((privateTimerPrescaler+1) * (privateTimerLoadValue+1))) / HEARTBEAT_TOGGLES_PER_SECOND;
}

// User can set the prescaler on the private timer.
// Also updates ticks per heart beat so that the LD4 heart-beat toggle rate remains constant.
void interrupts_setPrivateTimerPrescalerValue(u32 prescalerValue) {
  XScuTimer_LoadTimer(&TimerInstance, prescalerValue);
  privateTimerPrescaler = prescalerValue;
  // Formula derived from the ARM documentation on the private timer (4.1.1)
  privateTimerTicksPerHeartbeat = (ZYBO_BUS_CLOCK /((privateTimerPrescaler+1) * (privateTimerLoadValue+1))) / HEARTBEAT_TOGGLES_PER_SECOND;
}

u32 interrupts_isrInvocationCount() {return isrInvocationCount;}  // Functional accessor for isrInvocationCount.
// Accessor to retrieve the number of times the ISR was invoked (same as count of timer ticks).
u32 interrupts_getPrivateTimerTicksPerSecond() {return ZYBO_BUS_CLOCK /((privateTimerPrescaler+1) * (privateTimerLoadValue+1));}

// Keep track of End-Of-Conversion interrupts.
u32 totalEocCount;
u32 interrupts_getTotalEocCount() {return totalEocCount;}

// **********************End globals and accessors section. *************************

// The ADC is performs conversions constantly (asynchronously) at approximately 1 Mhz. This routine reads the
// latest conversion and shifts out the the lower 4-bits which are essentially noise unless you perform averaging.
uint32_t interrupts_getAdcData() {
	return XSysMon_GetAdcData(&xSysMonInst, XADC_AUX_CHANNEL_14) >> 4;
}

// Reads the private counter on the Arm core.
u32 interrupts_getPrivateTimerCounterValue(void) {
  return XScuTimer_GetCounterValue(&TimerInstance);
}

// This will keep track of the number of EOC conversion interrupts that have been detected.
static int eocCount = 0;
// True if EOC interrupts have been enabled.
bool eocCountInterruptFlag = false;
// Accessor.
int interrupts_getEocCount() {
  if (!eocCountInterruptFlag){
    printf("getEocCount(): eoc interrupt has not been enabled.\n\r");
  }
  return eocCount;
}

// Enable EOC interrupt in the SysMon.
int interrupts_enableSysMonEocInts() {
  XSysMon_IntrEnable(&xSysMonInst, XSM_IPIXR_EOC_MASK);
  eocCountInterruptFlag = true;
  return 0;
}

// Disable EOC interrupt in the SysMon.
int interrupts_disableSysMonEocInts() {
  XSysMon_IntrDisable(&xSysMonInst, XSM_IPIXR_EOC_MASK);
  eocCountInterruptFlag = false;
  return 0;
}

// Count timer ticks to know when to get a sample from the XADC.
// volatile int sampleTimerTicks = PRIVATE_TIMER_TICKS_PER_ADC_SAMPLE;
int ledValue = 0;

// Used to detect if the GIC has been properly initialized before use.
static bool initGicFlag = false;

// Assumes that you are connected to auxiliary port 14.
#define XADC_AUX_CHANNEL_14 XSM_CH_AUX_MAX-1

// Implements a pulse on LED3 to see if things are still alive.
void updateHeartBeatLed() {
  if (!heartBeatTimer) {
	heartBeatTimer = privateTimerTicksPerHeartbeat;  // Reset the heart beat timer.
	ledValue = ledValue == 0 ? 1 : 0;                // Toggle the LED on and off.
	leds_writeLd4(ledValue);
  } else {
	heartBeatTimer--;
  }
}

// Default xSysMon ISR just clears the interrupt.
// Watch out, the code currently indiscriminately clears out all interrupts from the XADC.
void sysMonIsr(void *CallBackRef) {
  u32 intrStatusValue;
  XSysMon *xSysMonPtr = (XSysMon *)CallBackRef;
  // Get the interrupt status from the device and check the value.
  intrStatusValue = XSysMon_IntrGetStatus(xSysMonPtr);
  if (intrStatusValue & XSM_SR_EOC_MASK)  // inc eocCount if the EOC status bit is set.
    totalEocCount++;
  XSysMon_IntrClear(xSysMonPtr, intrStatusValue);  // Clear out ALL XADC interrupt.
}

// *********************************************************************************
// ******************************* Timer ISR ***************************************
// *********************************************************************************
void timerIsr(void* callBackRef){
#ifdef ENABLE_INTERVAL_TIMER_0_IN_TIMER_ISR  // Enable interval timing when this is defined.
	intervalTimer_start(INTERRUPT_CUMULATIVE_ISR_INTERVAL_TIMER_NUMBER);
#endif

#ifdef INTERRUPTS_ENABLE_HEARTBEAT_LED
	updateHeartBeatLed();
#endif
//	queue_data_t adcData = (queue_data_t) interrupts_getAdcData();
//	queue_overwritePush(&debugAdcQueue, adcData);

  // Put the code that you want executed on a timer interrupt between this line
	isr_function();	// This function is defined in isr.c
  // and this line.

  isrInvocationCount++;  				// Keep track of the interrupt count to compare to detected count in user program.
  interrupts_isrFlagGlobal = 1;	// Means that an interrupt has fired. Used in main to detect a timer interrupt.

#ifdef ENABLE_INTERVAL_TIMER_0_IN_TIMER_ISR   // Enable interval timing when this is defined.
  intervalTimer_stop(INTERRUPT_CUMULATIVE_ISR_INTERVAL_TIMER_NUMBER);
#endif
  XScuTimer_ClearInterruptStatus(&TimerInstance);
}
// *********************************************************************************
// ****************************** End Timer ISR *************************************
// *********************************************************************************

// Xilinx calls the Axi XADC module the SysMon (System Monitor).
// This sets up the XADC to continuously sample on aux. channel 14 in single channel mode, unipolar.
int initSysMonInterrupts() {
  int status;
  xSysMonConfig = XSysMon_LookupConfig(XPAR_AXI_XADC_0_DEVICE_ID);
  status = XSysMon_CfgInitialize(&xSysMonInst, xSysMonConfig, xSysMonConfig->BaseAddress);
  if (status != XST_SUCCESS) {
	printf("XSysMon initialize failed!!!\n\r");
    return XST_FAILURE;
  }
  status = XSysMon_SelfTest(&xSysMonInst);
  if (status != XST_SUCCESS) {
	printf("XSysMon self-test failed!!!\n\r");
    return XST_FAILURE;
  }
  XSysMon_SetAdcClkDivisor(&xSysMonInst, XADC_CLOCK_DIVIDER);
//  int adcClkDivisor = XSysMon_GetAdcClkDivisor(&xSysMonInst);
//  printf("Default ADC clock divisor: %d.\n\r", adcClkDivisor);
  XSysMon_SetSequencerMode(&xSysMonInst, XSM_SEQ_MODE_SINGCHAN); // Single-channel mode (channel 14).
  status =  XSysMon_SetSingleChParams(&xSysMonInst, XADC_AUX_CHANNEL_14,
						              FALSE, FALSE, FALSE);
  if(status != XST_SUCCESS) {
	printf("XSysMon set single channel parameters failed!!!\n\r");
    return XST_FAILURE;
  }
 // XSysMon_SetAvg(&xSysMonInst, XSM_AVG_16_SAMPLES); //Don't use, reduces conversion rate by 16.
  XSysMon_SetAlarmEnables(&xSysMonInst, 0x0);  // Disable all alarms.
//  // Set alarm threshold registers for VCCINT to min and max so alarm does not occur.
//  XSysMon_SetAlarmThreshold(&xSysMonInst, XSM_ATR_VCCINT_UPPER, 0xFFFF);
//  XSysMon_SetAlarmThreshold(&xSysMonInst, XSM_ATR_VCCINT_LOWER, 0x0);
//  // Connect the xSysMon ISR to the GIC ISR.

//  status = XScuGic_Connect(&InterruptController,
//		                    XPAR_FABRIC_AXI_XADC_0_IP2INTC_IRPT_INTR,
//		                   (Xil_ExceptionHandler) sysMonIsr,
//		                   (void *) &xSysMonInst);
//  if (status != XST_SUCCESS) {
//	print("XScuGic_Connect failed (sysmon).\n\r");
//	return status;
//  }

  // Clear out any pending interrupts in the interrupt status register.
//  int intrStatus = XSysMon_IntrGetStatus(&xSysMonInst);
//  XSysMon_IntrClear(&xSysMonInst, intrStatus);
  // Enable the sysmon interrupt on the GIC (does nothing to the sysmon).
//  XScuGic_Enable(&InterruptController, XPAR_FABRIC_AXI_XADC_0_IP2INTC_IRPT_INTR);
  return XST_SUCCESS;
}

// Sets up the timer for periodic interrupts.
int initTimerInterrupts() {
  int status;  // General Xilinx status reporting.
  // Get a handle to the timer.
  ConfigPtr = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);
  // Init the timer via the handle.
  status = XScuTimer_CfgInitialize(&TimerInstance,
		                           ConfigPtr,
		                           ConfigPtr->BaseAddr);
  if (status != XST_SUCCESS){
	print("XScuTimer_CfgInitialize failed.\n\r");
    return XST_FAILURE;
  }
  status = XScuTimer_SelfTest(&TimerInstance);
  if (status != XST_SUCCESS){
    print("XscuTimer_SelfTest failed.\n\r");
    return status;
  }
  // Connect the timer ISR to the GIC ISR.
  status = XScuGic_Connect(&InterruptController,
		                   XPAR_SCUTIMER_INTR,
		                   (Xil_ExceptionHandler) timerIsr,
		                   (void *) &TimerInstance);
  if (status != XST_SUCCESS) {
	print("XScuGic_Connect failed (timer).\n\r");
	return status;
  }
  // Enable the timer interrupt on the GIC (does nothing to the timer).
  XScuGic_Enable(&InterruptController, XPAR_SCUTIMER_INTR);
  // Enable auto reload mode.
  XScuTimer_EnableAutoReload(&TimerInstance);
  // Load the timer counter preload register.
  // Subtract 1 from the tick count to account for the extra cycle for the counter
  // to transition to zero to generate the interrupt. Page 4-2 of ARM Cortex A9 Mpcore r4p1
//  int loadValue = PRIVATE_TIMER_LOAD_VALUE;
//  int prescalar = XScuTimer_GetPrescaler(&TimerInstance);
//  printf("interrupt.c - load value: %d\n\r", loadValue);
//  printf("interrupt.c - prescalar value: %d\n\r", prescalar);
  XScuTimer_SetPrescaler(&TimerInstance, PRIVATE_TIMER_PRESCALER_DEFAULT);

  interrupts_setPrivateTimerLoadValue(PRIVATE_TIMER_LOAD_VALUE_DEFAULT);
  //XScuTimer_LoadTimer(&TimerInstance, PRIVATE_TIMER_LOAD_VALUE_DEFAULT);

  //  prescalar = XScuTimer_GetPrescaler(&TimerInstance);
//  printf("interrupt.c - prescalar value: %d\n\r", prescalar);
//  print("setupTimerInterrupts exited successfully.\n\r");
  return XST_SUCCESS;
}

// Inits all interrupts, which means:
// 1. Sets up the interrupt routine for ARM (GIC ISR) and does all necessary initialization.
// 2. Initializes all supported interrupts and connects their ISRs to the GIC ISR.
// 3. Enables the interrupts at the GIC, but not at the device itself.
// 4. Pretty much does everything but it does not enable the ARM interrupts or any of the device global interrupts.
// if printFailedStatusFlag is true, it prints out diagnostic messages if something goes awry.
int interrupts_initAll(bool printFailedStatusFlag) {
//	initAdcQueue();
  int status;  // General Xilinx status.
  // Lookup the GIC device and get its handle.
  GicConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
  if (!GicConfig) {
	if (printFailedStatusFlag)
  	  print("XScuGic_LookupConfig failed.\n\r");
	return 1;
  }
  // Init the GIC interrupt controller via its handle.
  status = XScuGic_CfgInitialize(&InterruptController, GicConfig, GicConfig->CpuBaseAddress);
  if (status != XST_SUCCESS) {
	if (printFailedStatusFlag)
	  print("XScuGic_CfgInitialize failed.\n\r");
	return 1;
  }
  Xil_ExceptionInit();  // Initialize the interrupt system.
  // Connect the GIC interrupt handler (provided by Xilinx) to the GIC interrupt.
  // You will connect timerIsr, sysMonIsr, etc., to this interrupt hander.
  Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
							   (Xil_ExceptionHandler) XScuGic_InterruptHandler,
							   &InterruptController);
    print("setupGICInterruptController exited successfully.\n\r");

//  u8 adcPriority;
  u8 timerPriority;
//  u8 adcTrigger;
  u8 timerTrigger;
  XScuGic_GetPriorityTriggerType(&InterruptController, XPAR_SCUTIMER_INTR,
   					&timerPriority, &timerTrigger);
//   printf("timer priority: %x\n\r", timerPriority);
//   printf("timer trigger: %x\n\r", timerTrigger);

//   XScuGic_GetPriorityTriggerType(&InterruptController, XPAR_FABRIC_AXI_XADC_0_IP2INTC_IRPT_INTR,
//  					&adcPriority, &adcTrigger);
//  printf("xadc priority: %x\n\r", adcPriority);
//  printf("xadc trigger: %x\n\r", adcTrigger);

   // Try setting the XADC to a higher priority than the timer (less is higher priority).
//   adcPriority = timerPriority - 10;
//   adcTrigger = 3;  // Changing to rising edge doesn't work - the EOC interrupt count drops way down.
//   XScuGic_SetPriorityTriggerType(&InterruptController, XPAR_FABRIC_AXI_XADC_0_IP2INTC_IRPT_INTR,
//     					adcPriority, adcTrigger);
   // Verify that the setting changed.
//   XScuGic_GetPriorityTriggerType(&InterruptController, XPAR_FABRIC_AXI_XADC_0_IP2INTC_IRPT_INTR,
//   					&adcPriority, &adcTrigger);
//   printf("updated xadc priority: %x\n\r", adcPriority);
//   printf("updated xadc trigger: %x\n\r", adcTrigger);

  // init the timer interrupts.
  initTimerInterrupts();
  // Init the SysMon interrupts (XADC).
  initSysMonInterrupts();
  initGicFlag = true;

  // Enable capture of ADC values in queue if queue.h has been included.
//#ifdef QUEUE_H_
//// Enables capture of ADC value to queue.
//  queue_init(&adcDataQueue1, ADC_DATA_QUEUE_SIZE);
//#endif

  return 0;
}

// This enables overall ARM interrupts.
// Checks the init flag to make sure that the user has init'd the GIC.
int interrupts_enableArmInts() {
  if (initGicFlag) {
    Xil_ExceptionEnable();
    return 0;
  } else {
    printf("Error: Must call initGIC before enableArmInterrupts()\n\r.");
    return 1;
  }
}

// This disables overall ARM interrupts.
// Checks the init flag to make sure that the user has init'd the GIC.
int interrupts_disableArmInts() {
  if (initGicFlag) {
    Xil_ExceptionDisable();
    return 0;
  } else {
    printf("Error: Must call initGIC before disableArmInterrupts()\n\r.");
    return 1;
  }
}

int interrupts_enableTimerGlobalInts() {
  XScuTimer_EnableInterrupt(&TimerInstance);
  return 0;
}
int interrupts_disableTimerGlobalInts(){
  XScuTimer_DisableInterrupt(&TimerInstance);
  return 0;
}

//// Call this to change the timer load value from the default.
//int setTimerInterval(int loadValue) {
//  XScuTimer_LoadTimer(&TimerInstance, loadValue);
//  return 0;
//}

int interrupts_startArmPrivateTimer() {
  XScuTimer_Start(&TimerInstance);
  return 0;
}

int interrupts_stopArmPrivateTimer() {
  XScuTimer_Stop(&TimerInstance);
  return 0;
}

// Enable EOC (end of conversion) interrupts.
int interrupts_enableSysMonGlobalInts(){
  XSysMon_IntrGlobalEnable(&xSysMonInst);
  return 0;
}

// Disable EOC (end of converstion) interrupts.
int interrupts_disableSysMonGlobalInterrupts(){
  XSysMon_IntrGlobalDisable(&xSysMonInst);
  return 0;
}

// These functions do nothing for now.
//uint32_t interrupts_initBluetoothInterrupts() {printf("NYI!!!\n\r"); return 0;}
void bluetoothIsr() {
  // NYI
}

uint32_t interrupts_initBluetoothInterrupts() {
  uint32_t status=0;
//  // Connect the timer ISR to the GIC ISR.
//   status = XScuGic_Connect(&InterruptController,
//                         XPAR_FABRIC_SYSTEM_IRQ_F2P_INTR,
//                        (Xil_ExceptionHandler) bluetoothIsr,
//                        (void *) NULL);
//   if (status != XST_SUCCESS) {
//   print("XScuGic_Connect failed (timer).\n\r");
//   return status;
//   }
   return status;
}

void interrupts_enableBluetoothInterrupts() {}
void interrupts_disableBluetoothInterrupts() {}
void interrupts_ackBluetoothInterrupts() {}






