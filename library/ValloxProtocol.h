// Static functions and definitions of the vallox protocol
// 
// 01 21 11 00 A3 C9
// |  |  |  |  |  |
// |  |  |  |  |  CRC
// |  |  |  |  Get = Variable, Set = value
// |  |  |  Get = 0, >1 = variable
// |  |  Receiver
// |  Sender 11 = mainboard 21.. control panels 10 broadcast to mainboards, 20 broadcast to panels
// Domain always 1
// 
// Special telegram (two times)
// 01 11 20 91 0 C3   (91 = SUSPEND)
// 01 11 20 9F 0 C3   (8F = RESUME) 
// 
// Master is always 11
// Panels 21-29
// LON is always 28
// 
// RH% sensor value: (x-51)/2.04

#ifndef ValloxProtocol_h
#define ValloxProtocol_h

#include <inttypes.h>

const uint16_t VALLOX_BAUDRATE = 9600; 

const uint8_t VALLOX_LENGTH = 6; // always 6
const uint8_t VALLOX_DOMAIN = 1; // always 1

// Addresses for sender and receiver
const uint8_t VALLOX_ADDRESS_MAINBOARDS = 0x10;
const uint8_t VALLOX_ADDRESS_MASTER = VALLOX_ADDRESS_MAINBOARDS + 1;

const uint8_t VALLOX_ADDRESS_PANELS = 0x20;

const uint8_t VALLOX_ADDRESS_PANEL1 = VALLOX_ADDRESS_PANELS + 1;
const uint8_t VALLOX_ADDRESS_PANEL2 = VALLOX_ADDRESS_PANELS + 2;
const uint8_t VALLOX_ADDRESS_PANEL3 = VALLOX_ADDRESS_PANELS + 3;
const uint8_t VALLOX_ADDRESS_PANEL4 = VALLOX_ADDRESS_PANELS + 4;
const uint8_t VALLOX_ADDRESS_PANEL5 = VALLOX_ADDRESS_PANELS + 5;
const uint8_t VALLOX_ADDRESS_PANEL6 = VALLOX_ADDRESS_PANELS + 6;
const uint8_t VALLOX_ADDRESS_PANEL7 = VALLOX_ADDRESS_PANELS + 7;
const uint8_t VALLOX_ADDRESS_LON = VALLOX_ADDRESS_PANELS + 8;
const uint8_t VALLOX_ADDRESS_PANEL8 = VALLOX_ADDRESS_PANELS + 9;

// Variables 
const uint8_t VALLOX_VARIABLE_POLL = 0x00;		// poll request for variable in value.

// 1 1 1 1 1 1 1 1  
// | | | | | | | |
// | | | | | | | +- 0 Speed 1 - 0=0ff 1=on - readonly
// | | | | | | +--- 1 Speed 2 - 0=0ff 1=on - readonly
// | | | | | +----- 2 Speed 3 - 0=0ff 1=on - readonly
// | | | | +------- 3 Speed 4 - 0=0ff 1=on - readonly
// | | | +--------- 4 Speed 5 - 0=0ff 1=on - readonly
// | | +----------- 5 Speed 6 - 0=0ff 1=on - readonly
// | +------------- 6 Speed 7 - 0=0ff 1=on - readonly
// +--------------- 7 Speed 8 - 0=0ff 1=on - readonly
const uint8_t VALLOX_VARIABLE_IOPORT_FANSPEED_RELAYS = 0x06;

// 1 1 1 1 1 1 1 1 
// | | | | | | | |
// | | | | | | | +- 0 
// | | | | | | +--- 1 
// | | | | | +----- 2 
// | | | | +------- 3 
// | | | +--------- 4 
// | | +----------- 5  post-heating on - 0=0ff 1=on - readonly
// | +------------- 6 
// +--------------- 7 
const uint8_t VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_1 = 0x07;

// 1 1 1 1 1 1 1 1  0=0ff 1=on
// | | | | | | | |
// | | | | | | | +- 0 
// | | | | | | +--- 1 damper motor position - 0=winter 1=season - readonly
// | | | | | +----- 2 fault signal relay - 0=open 1=closed - readonly
// | | | | +------- 3 supply fan - 0=on 1=off
// | | | +--------- 4 pre-heating - 0=off 1=on - readonly
// | | +----------- 5 exhaust-fan - 0=on 1=off
// | +------------- 6 fireplace-booster - 0=open 1=closed - readonly 
// +--------------- 7 
const uint8_t VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2 = 0x08;

//01H = speed 1
//03H = speed 2
//07H = speed 3
//0FH = speed 4
//1FH = speed 5
//3FH = speed 6
//7FH = speed 7
//FFH = speed 8
const uint8_t VALLOX_VARIABLE_FAN_SPEED = 0x29;

// 33H = 0% FFH = 100%
const uint8_t VALLOX_VARIABLE_HUMIDITY = 0x2A; // higher measured relative humidity from 2F and 30. Translating Formula (x-51)/2.04
const uint8_t VALLOX_VARIABLE_CO2_HIGH = 0x2B;
const uint8_t VALLOX_VARIABLE_CO2_LOW = 0x2C;

// 1 1 1 1 1 1 1 1 
// | | | | | | | |
// | | | | | | | +- 0 
// | | | | | | +--- 1 Sensor1 - 0=not installed 1=installed - readonly
// | | | | | +----- 2 Sensor2 - 0=not installed 1=installed - readonly
// | | | | +------- 3 Sensor3 - 0=not installed 1=installed - readonly
// | | | +--------- 4 Sensor4 - 0=not installed 1=installed - readonly
// | | +----------- 5 Sensor5 - 0=not installed 1=installed - readonly
// | +------------- 6 
// +--------------- 7 
const uint8_t VALLOX_VARIABLE_INSTALLED_CO2_SENSORS = 0x2D;

const uint8_t VALLOX_VARIABLE_CURRENT_INCOMMING = 0x2E; // Current/Voltage in mA incomming on machine - readonly

const uint8_t VALLOX_VARIABLE_HUMIDITY_SENSOR1 = 0x2F; // sensor value: (x-51)/2.04
const uint8_t VALLOX_VARIABLE_HUMIDITY_SENSOR2 = 0x30; // sensor value: (x-51)/2.04

const uint8_t VALLOX_VARIABLE_TEMP_OUTSIDE = 0x32;
const uint8_t VALLOX_VARIABLE_TEMP_EXHAUST = 0x33;
const uint8_t VALLOX_VARIABLE_TEMP_INSIDE = 0x34;
const uint8_t VALLOX_VARIABLE_TEMP_INCOMMING = 0x35;

//05H = Supply air temperature sensor fault
//06H = Carbon dioxide alarm
//07h = Outdoor air sensor fault
//08H = Extract air sensor fault
//09h = Water radiator danger of freezing
//0AH = Exhaust air sensor fault
const uint8_t VALLOX_VARIABLE_LAST_ERROR_NUMBER = 0x36;

//Post-heating power-on seconds counter. Percentage of X / 2.5
const uint8_t VALLOX_VARIABLE_POST_HEATING_ON_COUNTER = 0x55;

//Post-heating off time, in seconds, the counter. Percentage of X / 2.5
const uint8_t VALLOX_VARIABLE_POST_HEATING_OFF_TIME = 0x56;

//The ventilation zone of air blown to the desired temperature NTC sensor scale
const uint8_t VALLOX_VARIABLE_POST_HEATING_TARGET_VALUE = 0x57;

// 1 1 1 1 1 1 1 1 
// | | | | | | | |
// | | | | | | | +- 0 
// | | | | | | +--- 1 
// | | | | | +----- 2 
// | | | | +------- 3 
// | | | +--------- 4 
// | | +----------- 5 
// | +------------- 6 
// +--------------- 7 
const uint8_t VALLOX_VARIABLE_FLAGS_1 = 0x6C;

// 1 1 1 1 1 1 1 1 
// | | | | | | | |
// | | | | | | | +- 0 CO2 higher speed-request 0=no 1=Speed​​. up
// | | | | | | +--- 1 CO2 lower rate public invitation 0=no 1=Speed​​. down
// | | | | | +----- 2 %RH lower rate public invitation 0=no 1=Speed​​. down
// | | | | +------- 3 switch low. Spd.-request 0=no 1=Speed​. down
// | | | +--------- 4 
// | | +----------- 5 
// | +------------- 6 CO2 alarm 0=no 1=CO2 alarm
// +--------------- 7 sensor Frost alarm 0=no 1=a risk of freezing
const uint8_t VALLOX_VARIABLE_FLAGS_2 = 0x6D;

// 1 1 1 1 1 1 1 1 
// | | | | | | | |
// | | | | | | | +- 0 
// | | | | | | +--- 1 
// | | | | | +----- 2 
// | | | | +------- 3 
// | | | +--------- 4 
// | | +----------- 5 
// | +------------- 6 
// +--------------- 7 
const uint8_t VALLOX_VARIABLE_FLAGS_3 = 0x6E;

// 1 1 1 1 1 1 1 1 
// | | | | | | | |
// | | | | | | | +- 0 
// | | | | | | +--- 1 
// | | | | | +----- 2 
// | | | | +------- 3 
// | | | +--------- 4 water radiator danger of freezing 0=no risk 1 = risk
// | | +----------- 5 
// | +------------- 6 
// +--------------- 7 slave/master selection 0=slave 1=master
const uint8_t VALLOX_VARIABLE_FLAGS_4 = 0x6F;

// 1 1 1 1 1 1 1 1 
// | | | | | | | |
// | | | | | | | +- 0 
// | | | | | | +--- 1 
// | | | | | +----- 2 
// | | | | +------- 3 
// | | | +--------- 4 
// | | +----------- 5 
// | +------------- 6 
// +--------------- 7 preheating status flag 0=on 1=off
const uint8_t VALLOX_VARIABLE_FLAGS_5 = 0x70;

// 1 1 1 1 1 1 1 1 
// | | | | | | | |
// | | | | | | | +- 0 
// | | | | | | +--- 1 
// | | | | | +----- 2 
// | | | | +------- 3 
// | | | +--------- 4 remote monitoring control 0=no 1=Operation - readonly
// | | +----------- 5 Activation of the fireplace switch read the variable and set this number one <-- bit can be set to activate fire place switch
// | +------------- 6 fireplace/booster status 0=off 1=on - read only
// +--------------- 7
const uint8_t VALLOX_VARIABLE_FLAGS_6 = 0x71;

//Function time in minutes remaining , descending - readonly
const uint8_t VALLOX_VARIABLE_FIRE_PLACE_BOOSTER_COUNTER = 0x79;

// Suspend Resume Traffic for CO2 sensor interaction: is sent twice as broadcast
const uint8_t VALLOX_VARIABLE_SUSPEND = 0x91;
const uint8_t VALLOX_VARIABLE_RESUME = 0x8F;

// 1 1 1 1 1 1 1 1
// | | | | | | | |
// | | | | | | | +- 0 Power state
// | | | | | | +--- 1 CO2 Adjust state
// | | | | | +----- 2 %RH adjust state
// | | | | +------- 3 Heating state
// | | | +--------- 4 Filterguard indicator
// | | +----------- 5 Heating indicator
// | +------------- 6 Fault indicator
// +--------------- 7 service reminder
const uint8_t VALLOX_VARIABLE_SELECT = 0xA3;
const uint8_t VALLOX_VARIABLE_HEATING_SET_POINT = 0xA4;


//01H = Speed 1
//03H = Speed 2
//07H = Speed 3
//0FH = Speed 4
//1FH = Speed 5
//3FH = Speed 6
//7FH = Speed 7
//FFH = Speed 8
const uint8_t VALLOX_VARIABLE_FAN_SPEED_MAX = 0xA5;
const uint8_t VALLOX_VARIABLE_SERVICE_REMINDER = 0xA6; // months
const uint8_t VALLOX_VARIABLE_PRE_HEATING_SET_POINT = 0xA7;
const uint8_t VALLOX_VARIABLE_INPUT_FAN_STOP = 0xA8;  // Temp threshold: fan stops if input temp falls below this temp.

//01H = Speed 1
//03H = Speed 2
//07H = Speed 3
//0FH = Speed 4
//1FH = Speed 5
//3FH = Speed 6
//7FH = Speed 7
//FFH = Speed 8
const uint8_t VALLOX_VARIABLE_FAN_SPEED_MIN = 0xA9;

// 1 1 1 1 1 1 1 1
// | | | | _______
// | | | |     |  
// | | | |     +--- 0-3 set adjustment interval of CO2 and %RH in minutes (Regelinterval)
// | | | |   
// | | | |   
// | | | | 
// | | | +--------- 4 automatic RH basic level seeker state
// | | +----------- 5 boost switch modde (1=boost, 0 = fireplace)
// | +------------- 6 radiator type 0 = electric, 1 = water
// +--------------- 7 cascade adjust 0 = off, 1 = on
const uint8_t VALLOX_VARIABLE_PROGRAM = 0xAA;

//The maintenance counter informs about the next maintenance alarm time: remaining months, descending.
const uint8_t VALLOX_VARIABLE_MAINTENANCE_MONTH_COUNTER = 0xAB;

const uint8_t VALLOX_VARIABLE_BASIC_HUMIDITY_LEVEL = 0xAE;
const uint8_t VALLOX_VARIABLE_HRC_BYPASS = 0xAF; // Heat recovery cell bypass setpoint temp 
const uint8_t VALLOX_VARIABLE_DC_FAN_INPUT_ADJUSTMENT = 0xB0; // 0-100%
const uint8_t VALLOX_VARIABLE_DC_FAN_OUTPUT_ADJUSTMENT = 0xB1; // 0-100%
const uint8_t VALLOX_VARIABLE_CELL_DEFROSTING = 0xB2; // Defrosting starts when exhaust air drops below this setpoint temp (Hysteresis 4)
const uint8_t VALLOX_VARIABLE_CO2_SET_POINT_UPPER = 0xB3;
const uint8_t VALLOX_VARIABLE_CO2_SET_POINT_LOWER = 0xB4;

// 1 1 1 1 1 1 1 1
// | | | | | | | |
// | | | | | | | +- 0 Function of max speed limit 0 = with adjustment, 1 = always 
// | | | | | | +--- 1  
// | | | | | +----- 2
// | | | | +------- 3
// | | | +--------- 4
// | | +----------- 5
// | +------------- 6
// +--------------- 7
const uint8_t VALLOX_VARIABLE_PROGRAM2 = 0xB5;

// This one is queried at startup and answered with 3 but not described in the protocol
const uint8_t VALLOX_VARIABLE_UNKNOWN = 0xC0;

// see VALLOX_VARIABLE_PROGRAM2
enum ValloxMaxSpeedLimitMode
{
	WithAdjustment,
	Always
};

// see VALLOX_VARIABLE_PROGRAM
enum ValloxBoostSwitchMode
{
	Boost,
	Fireplace
};

// see VALLOX_VARIABLE_PROGRAM
enum ValloxRadiatorType
{
	Electric,
	Water
};

static uint8_t VALLOX_FAN_SPEED_MAPPING[] =
{
	0x01,
	0x03,
	0x07,
	0x0F,
	0x1F,
	0x3F,
	0x7F,
	0xFF
};


static int8_t VALLOX_TEMPERATURE_MAPPING[] =
{
	-74, -70, -66, -62, -59, -56, -54, -52,
	-50, -48, -47, -46, -44, -43, -42, -41,
	-40, -39, -38, -37, -36, -35, -34, -33,
	-33, -32, -31, -30, -30, -29, -28, -28,
	-27, -27, -26, -25, -25, -24, -24, -23,
	-23, -22, -22, -21, -21, -20, -20, -19,
	-19, -19, -18, -18, -17, -17, -16, -16,
	-16, -15, -15, -14, -14, -14, -13, -13,
	-12, -12, -12, -11, -11, -11, -10, -10,
	-9,  -9,  -9,  -8,  -8,  -8,  -7,  -7,
	-7,  -6,  -6,  -6,  -5,  -5,  -5,  -4,
	-4,  -4,  -3,  -3,  -3,  -2,  -2,  -2,
	-1,  -1,  -1,  -1,  0,   0,   0,   1,
	1,   1,   2,   2,   2,   3,   3,   3,
	4,   4,   4,   5,   5,   5,   5,   6,
	6,   6,   7,   7,   7,   8,   8,   8,
	9,   9,   9,   10,  10,  10,  11,  11,
	11,  12,  12,  12,  13,  13,  13,  14,
	14,  14,  15,  15,  15,  16,  16,  16,
	17,  17,  18,  18,  18,  19,  19,  19,
	20,  20,  21,  21,  21,  22,  22,  22,
	23,  23,  24,  24,  24,  25,  25,  26,
	26,  27,  27,  27,  28,  28,  29,  29,
	30,  30,  31,  31,  32,  32,  33,  33,
	34,  34,  35,  35,  36,  36,  37,  37,
	38,  38,  39,  40,  40,  41,  41,  42,
	43,  43,  44,  45,  45,  46,  47,  48,
	49,  49,  50,  51,  52,  53,  53,  54,
	55,  56,  57,  59,  60,  61,  62,  63,
	65,  66,  68,  69,  71,  73,  75,  77,
	79,  81,  82,  86,  90,  93,  97,  100,
	100, 100, 100, 100, 100, 100, 100, 100
};

class Vallox
{
public:
	static int8_t convertTemperature(uint8_t value)
	{
		return VALLOX_TEMPERATURE_MAPPING[value];
	}

	static uint8_t convertBackTemperature(int8_t temperature)
	{
		uint8_t value = 100;

		for (uint8_t i = 0; i < 255; i++)
		{
			int8_t valueFromTable = VALLOX_TEMPERATURE_MAPPING[i];
			if (valueFromTable >= temperature)
			{
				value = i;
				break;
			}
		}

		return value;
	}

	// 8 --> 0xFF
	static uint8_t convertBackFanSpeed(uint8_t value)
	{
		return VALLOX_FAN_SPEED_MAPPING[value];
	}

	// 0xFF --> 8
	static uint8_t convertFanSpeed(uint8_t value)
	{
		uint8_t fanSpeed = 0;

		for (uint8_t i = 0; i < 8; i++)
		{
			if (VALLOX_FAN_SPEED_MAPPING[i] == value)
			{
				fanSpeed = i + 1;
				break;
			}
		}

		return fanSpeed;
	}

	// 1 1 1 1 1 1 1 1
	// | | | | | | | |
	// | | | | | | | +- 0 Power state
	// | | | | | | +--- 1 CO2 Adjust state
	// | | | | | +----- 2 %RH adjust state
	// | | | | +------- 3 Heating state
	// | | | +--------- 4 Filterguard indicator
	// | | +----------- 5 Heating indicator
	// | +------------- 6 Fault indicator
	// +--------------- 7 service reminder
	static void convertSelect(uint8_t select, 
		bool* pPowerState, 
		bool* pCo2AdjustState, 
		bool* pHumidityAdjustState, 
		bool* pHeatingState, 
		bool* pFilterGuardIndicator, 
		bool* pHeatingIndicator,
		bool* pFaultIndicator,
		bool* pServiceReminderIndicator)
	{
		*pPowerState				= (select & 0x01) != 0;
		*pCo2AdjustState			= (select & 0x02) != 0;
		*pHumidityAdjustState		= (select & 0x04) != 0;
		*pHeatingState				= (select & 0x08) != 0;
		*pFilterGuardIndicator		= (select & 0x10) != 0;
		*pHeatingIndicator			= (select & 0x20) != 0;
		*pFaultIndicator			= (select & 0x40) != 0;
		*pServiceReminderIndicator	= (select & 0x80) != 0;
	}

	static uint8_t calculateChecksum(uint8_t* pTelegram)
	{
		int checksum = 0;
		for (uint8_t i = 0; i < VALLOX_LENGTH - 1; i++)
		{
			checksum += *(pTelegram+i);
		}
		return (uint8_t)(checksum % 256);
	}

	// 1 1 1 1 1 1 1 1
	// | | | | _______
	// | | | |     |  
	// | | | |     +--- 0-3 set adjustment interval of CO2 and %RH in minutes 
	// | | | |   
	// | | | |   
	// | | | | 
	// | | | +--------- 4 automatic RH basic level seeker state
	// | | +----------- 5 boost switch modde (1=boost, 0 = fireplace)
	// | +------------- 6 radiator type 0 = electric, 1 = water
	// +--------------- 7 cascade adjust 0 = off, 1 = on
	static void convertProgram(uint8_t program, 
		uint8_t* pAdjustmentIntervalMinutes, 
		bool* pAutomaticHumidityLevelSeekerState, 
		bool* pBoostSwitchMode, 
		bool* pRadiatorType, 
		bool* pCascadeAdjust)
	{
		*pAdjustmentIntervalMinutes = program & 0x0F;

		*pAutomaticHumidityLevelSeekerState = (program & 0x10) != 0;
		*pBoostSwitchMode					= (program & 0x20) != 0;
		*pRadiatorType						= (program & 0x40) != 0;
		*pCascadeAdjust						= (program & 0x80) != 0;
	}

	// 1 1 1 1 1 1 1 1
	// | | | | | | | |
	// | | | | | | | +- 0 Function of max speed limit 0 = with adjustment, 1 = always 
	// | | | | | | +--- 1  
	// | | | | | +----- 2
	// | | | | +------- 3
	// | | | +--------- 4
	// | | +----------- 5
	// | +------------- 6
	// +--------------- 7
	static void convertProgram2(uint8_t program, 
		bool* pMaxSpeeLimitMode)
	{
		*pMaxSpeeLimitMode = (program & 0x01) != 0;
	}

	// 1 1 1 1 1 1 1 1  
	// | | | | | | | |
	// | | | | | | | +- 0 Speed 1 - 0=0ff 1=on - readonly
	// | | | | | | +--- 1 Speed 2 - 0=0ff 1=on - readonly
	// | | | | | +----- 2 Speed 3 - 0=0ff 1=on - readonly
	// | | | | +------- 3 Speed 4 - 0=0ff 1=on - readonly
	// | | | +--------- 4 Speed 5 - 0=0ff 1=on - readonly
	// | | +----------- 5 Speed 6 - 0=0ff 1=on - readonly
	// | +------------- 6 Speed 7 - 0=0ff 1=on - readonly
	// +--------------- 7 Speed 8 - 0=0ff 1=on - readonly
	static void convertIoPortFanSpeedRelays(uint8_t value,
		bool* pSpeed1,
		bool* pSpeed2,
		bool* pSpeed3,
		bool* pSpeed4,
		bool* pSpeed5,
		bool* pSpeed6,
		bool* pSpeed7,
		bool* pSpeed8)
	{
		*pSpeed1 = (value & 0x01) != 0;
		*pSpeed2 = (value & 0x02) != 0;
		*pSpeed3 = (value & 0x04) != 0;
		*pSpeed4 = (value & 0x08) != 0;
		*pSpeed5 = (value & 0x10) != 0;
		*pSpeed6 = (value & 0x20) != 0;
		*pSpeed7 = (value & 0x40) != 0;
		*pSpeed8 = (value & 0x80) != 0;
	}

	// 1 1 1 1 1 1 1 1 
	// | | | | | | | |
	// | | | | | | | +- 0 
	// | | | | | | +--- 1 
	// | | | | | +----- 2 
	// | | | | +------- 3 
	// | | | +--------- 4 
	// | | +----------- 5  post-heating on - 0=0ff 1=on - readonly
	// | +------------- 6 
	// +--------------- 7 
	static void convertIoPortMultiPurpose1(uint8_t value,
		bool* pPostHeatingOn)
	{
		*pPostHeatingOn = (value & 0x20) != 0;
	}

	// 1 1 1 1 1 1 1 1  0=0ff 1=on
	// | | | | | | | |
	// | | | | | | | +- 0 
	// | | | | | | +--- 1 damper motor position - 0=winter 1=season - readonly
	// | | | | | +----- 2 fault signal relay - 0=open 1=closed - readonly
	// | | | | +------- 3 supply fan - 0=on 1=off
	// | | | +--------- 4 pre-heating - 0=off 1=on - readonly
	// | | +----------- 5 exhaust-fan - 0=on 1=off
	// | +------------- 6 fireplace-booster - 0=open 1=closed - readonly 
	// +--------------- 7 
	static void convertIoPortMultiPurpose2(uint8_t value,
		//bool* pUnknown1,
		bool* pDamperMotorPosition,
		bool* pFaultSignalRelayClosed,
		bool* pSupplyFanOff,
		bool* pPreHeatingOn,
		bool* pExhaustFanOff,
		bool* pFireplaceBoosterClosed
		//bool* pUnknown2
		)
	{
		//*pUnknown1 = (value & 0x01) != 0;
		*pDamperMotorPosition = (value & 0x02) != 0;
		*pFaultSignalRelayClosed = (value & 0x04) != 0;
		*pSupplyFanOff = (value & 0x08) != 0;
		*pPreHeatingOn = (value & 0x10) != 0;
		*pExhaustFanOff = (value & 0x20) != 0;
		*pFireplaceBoosterClosed = (value & 0x40) != 0;
		//*pUnknown2 = (value & 0x80) != 0;
	}

	// 1 1 1 1 1 1 1 1 
	// | | | | | | | |
	// | | | | | | | +- 0 
	// | | | | | | +--- 1 Sensor1 - 0=not installed 1=installed - readonly
	// | | | | | +----- 2 Sensor2 - 0=not installed 1=installed - readonly
	// | | | | +------- 3 Sensor3 - 0=not installed 1=installed - readonly
	// | | | +--------- 4 Sensor4 - 0=not installed 1=installed - readonly
	// | | +----------- 5 Sensor5 - 0=not installed 1=installed - readonly
	// | +------------- 6 
	// +--------------- 7 
	static void convertInstalledCO2Sensor(uint8_t value,
		//bool* pUnknown1,
		bool* pSensor1,
		bool* pSensor2,
		bool* pSensor3,
		bool* pSensor4,
		bool* pSensor5
		//bool* pUnknown2,
		//bool* pUnknown3
		)
	{
		//*pUnknown1 = (value & 0x01) != 0;
		*pSensor1 = (value & 0x02) != 0;
		*pSensor2 = (value & 0x04) != 0;
		*pSensor3 = (value & 0x08) != 0;
		*pSensor4 = (value & 0x10) != 0;
		*pSensor5 = (value & 0x20) != 0;
		//*pUnknown2 = (value & 0x40) != 0;
		//*pUnknown3 = (value & 0x80) != 0;
	}

	//// 1 1 1 1 1 1 1 1 
	//// | | | | | | | |
	//// | | | | | | | +- 0 
	//// | | | | | | +--- 1 
	//// | | | | | +----- 2 
	//// | | | | +------- 3 
	//// | | | +--------- 4 
	//// | | +----------- 5 
	//// | +------------- 6 
	//// +--------------- 7 
	//static void converFlags1(uint8_t value,
	//	bool* pUnknown1,
	//	bool* pUnknown2,
	//	bool* pUnknown3,
	//	bool* pUnknown4,
	//	bool* pUnknown5,
	//	bool* pUnknown6,
	//	bool* pUnknown7,
	//	bool* pUnknown8)
	//{
	//	*pUnknown1 = (value & 0x01) != 0;
	//	*pUnknown2 = (value & 0x02) != 0;
	//	*pUnknown3 = (value & 0x04) != 0;
	//	*pUnknown4 = (value & 0x08) != 0;
	//	*pUnknown5 = (value & 0x10) != 0;
	//	*pUnknown6 = (value & 0x20) != 0;
	//	*pUnknown7 = (value & 0x40) != 0;
	//	*pUnknown8 = (value & 0x80) != 0;
	//}

	// 1 1 1 1 1 1 1 1 
	// | | | | | | | |
	// | | | | | | | +- 0 CO2 higher speed-request 0=no 1=Speed​​. up
	// | | | | | | +--- 1 CO2 lower  0=no 1=Speed​​. down
	// | | | | | +----- 2 %RH lower  0=no 1=Speed​​. down
	// | | | | +------- 3 switch low speed-request 0=no 1=Speed ​. down
	// | | | +--------- 4 
	// | | +----------- 5 
	// | +------------- 6 CO2 alarm 0=no 1=CO2 alarm
	// +--------------- 7 Sensor frost alarm 0=no 1=a risk of freezing
	static void converFlags2(uint8_t value,
		bool* pCO2Higher,
		bool* pCO2Lower,
		bool* pHumidity,
		bool* pSwitch,
		//bool* pUnknown5,
		//bool* pUnknown6,
		bool* pCO2Alarm,
		bool* pFrostAlarm)
	{
		*pCO2Higher = (value & 0x01) != 0;
		*pCO2Lower = (value & 0x02) != 0;
		*pHumidity = (value & 0x04) != 0;
		*pSwitch = (value & 0x08) != 0;
		//*pUnknown5 = (value & 0x10) != 0;
		//*pUnknown6 = (value & 0x20) != 0;
		*pCO2Alarm = (value & 0x40) != 0;
		*pFrostAlarm = (value & 0x80) != 0;
	}

	//// 1 1 1 1 1 1 1 1 
	//// | | | | | | | |
	//// | | | | | | | +- 0 
	//// | | | | | | +--- 1 
	//// | | | | | +----- 2 
	//// | | | | +------- 3 
	//// | | | +--------- 4 
	//// | | +----------- 5 
	//// | +------------- 6 
	//// +--------------- 7 
	//static void converFlags3(uint8_t value,
	//	bool* pUnknown1,
	//	bool* pUnknown2,
	//	bool* pUnknown3,
	//	bool* pUnknown4,
	//	bool* pUnknown5,
	//	bool* pUnknown6,
	//	bool* pUnknown7,
	//	bool* pUnknown8)
	//{
	//	*pUnknown1 = (value & 0x01) != 0;
	//	*pUnknown2 = (value & 0x02) != 0;
	//	*pUnknown3 = (value & 0x04) != 0;
	//	*pUnknown4 = (value & 0x08) != 0;
	//	*pUnknown5 = (value & 0x10) != 0;
	//	*pUnknown6 = (value & 0x20) != 0;
	//	*pUnknown7 = (value & 0x40) != 0;
	//	*pUnknown8 = (value & 0x80) != 0;
	//}

	// 1 1 1 1 1 1 1 1 
	// | | | | | | | |
	// | | | | | | | +- 0 
	// | | | | | | +--- 1 
	// | | | | | +----- 2 
	// | | | | +------- 3 
	// | | | +--------- 4 water radiator danger of freezing 0=no risk 1 = risk
	// | | +----------- 5 
	// | +------------- 6 
	// +--------------- 7 slave/master selection 0=slave 1=master
	static void converFlags4(uint8_t value,
		//bool* pUnknown1,
		//bool* pUnknown2,
		//bool* pUnknown3,
		//bool* pUnknown4,
		bool* pFrostAlarmWaterRadiator,
		//bool* pUnknown6,
		//bool* pUnknown7,
		bool* pSlaveMasterSelection)
	{
		//*pUnknown1 = (value & 0x01) != 0;
		//*pUnknown2 = (value & 0x02) != 0;
		//*pUnknown3 = (value & 0x04) != 0;
		//*pUnknown4 = (value & 0x08) != 0;
		*pFrostAlarmWaterRadiator = (value & 0x10) != 0;
		//*pUnknown6 = (value & 0x20) != 0;
		//*pUnknown7 = (value & 0x40) != 0;
		*pSlaveMasterSelection = (value & 0x80) != 0;
	}

	// 1 1 1 1 1 1 1 1 
	// | | | | | | | |
	// | | | | | | | +- 0 
	// | | | | | | +--- 1 
	// | | | | | +----- 2 
	// | | | | +------- 3 
	// | | | +--------- 4 
	// | | +----------- 5 
	// | +------------- 6 
	// +--------------- 7 slave/master selection 0=slave 1=master
	static void converFlags5(uint8_t value,
		//bool* pUnknown1,
		//bool* pUnknown2,
		//bool* pUnknown3,
		//bool* pUnknown4,
		//bool* pUnknown5,
		//bool* pUnknown6,
		//bool* pUnknown7,
		bool* pPreHeatingStatus)
	{
		//*pUnknown1 = (value & 0x01) != 0;
		//*pUnknown2 = (value & 0x02) != 0;
		//*pUnknown3 = (value & 0x04) != 0;
		//*pUnknown4 = (value & 0x08) != 0;
		//*pUnknown5 = (value & 0x10) != 0;
		//*pUnknown6 = (value & 0x20) != 0;
		//*pUnknown7 = (value & 0x40) != 0;
		*pPreHeatingStatus = (value & 0x80) != 0;
	}

	// 1 1 1 1 1 1 1 1 
	// | | | | | | | |
	// | | | | | | | +- 0 
	// | | | | | | +--- 1 
	// | | | | | +----- 2 
	// | | | | +------- 3 
	// | | | +--------- 4 remote monitoring control 0=no 1=Operation - readonly
	// | | +----------- 5 Activation of the fireplace switch read the variable and set this number one
	// | +------------- 6 fireplace/booster status 0=off 1=on - read only
	// +--------------- 7
	static void converFlags6(uint8_t value,
		//bool* pUnknown1,
		//bool* pUnknown2,
		//bool* pUnknown3,
		bool* pRemoteMonitoringControl,
		bool* pFirePlaceSwitchActivator,
		bool* pFirePlaceBoosterStatus
		//bool* pUnknown7,
		//bool* pUnknown8
		)
	{
		//*pUnknown1 = (value & 0x01) != 0;
		//*pUnknown2 = (value & 0x02) != 0;
		//*pUnknown3 = (value & 0x04) != 0;
		*pRemoteMonitoringControl = (value & 0x08) != 0;
		*pFirePlaceSwitchActivator = (value & 0x10) != 0;
		*pFirePlaceBoosterStatus = (value & 0x20) != 0;
		//*pUnknown7 = (value & 0x40) != 0;
		//*pUnknown8 = (value & 0x80) != 0;
	}
};

#endif // ValloxProtocol_h