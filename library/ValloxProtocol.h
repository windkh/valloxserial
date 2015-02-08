// (c) windkh 2015
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

const uint8_t VALLOX_VARIABLE_FAN_SPEED = 0x29;
const uint8_t VALLOX_VARIABLE_HUMIDITY = 0x2A; // higher measured relative humidity from 2F and 30. Translating Formula (x-51)/2.04
const uint8_t VALLOX_VARIABLE_CO2_HIGH = 0x2B;
const uint8_t VALLOX_VARIABLE_CO2_LOW = 0x2C;

const uint8_t VALLOX_VARIABLE_HUMIDITY_SENSOR1 = 0x2F; // sensor value: (x-51)/2.04
const uint8_t VALLOX_VARIABLE_HUMIDITY_SENSOR2 = 0x30; // sensor value: (x-51)/2.04

const uint8_t VALLOX_VARIABLE_TEMP_OUTSIDE = 0x32;
const uint8_t VALLOX_VARIABLE_TEMP_EXHAUST = 0x33;
const uint8_t VALLOX_VARIABLE_TEMP_INSIDE = 0x34;
const uint8_t VALLOX_VARIABLE_TEMP_INCOMMING = 0x35;


// This variable is cyclically polled from Panel1 but not described in the protocol
const uint8_t VALLOX_VARIABLE_UNKNOWN = 0x71;

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
const uint8_t VALLOX_VARIABLE_FAN_SPEED_MAX = 0xA5;
const uint8_t VALLOX_VARIABLE_SERVICE_REMINDER = 0xA6;
const uint8_t VALLOX_VARIABLE_PRE_HEATING_SET_POINT = 0xA7;
const uint8_t VALLOX_VARIABLE_INPUT_FAN_STOP = 0xA8;  // Temp threshold: fan stops if input temp falls below this temp.
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

const uint8_t VALLOX_VARIABLE_BASIC_HUMIDITY_LEVEL = 0xAE;
const uint8_t VALLOX_VARIABLE_HRC_BYPASS = 0xAF; // Heat recovery cell bypass setpoint temp 
const uint8_t VALLOX_VARIABLE_DC_FAN_INPUT_ADJUSTMENT = 0xB0; // 0-100%
const uint8_t VALLOX_VARIABLE_DC_FAN_OUTPUT_ADJUSTMENT = 0xB1; // 0-100%
const uint8_t VALLOX_VARIABLE_CELL_DEFROSTING = 0xB2; // Defrosting starts when exhaust air drops below this setpoint temp 
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
const uint8_t VALLOX_VARIABLE_UNKNOWN2 = 0xC0;

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
	static uint8_t convertTemperature(uint8_t value)
	{
		return VALLOX_TEMPERATURE_MAPPING[value];
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
};

#endif // ValloxProtocol_h