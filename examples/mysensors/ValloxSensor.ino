/* (c) windkh 2015
Vallox Sensor see www.mysensors.org
hardware requirements
- 1x Arduino Mega 
- 1x Radio NRF24L01+
- 2x MAX485 to TTL converter (board recommended)

- Pin 2  grey	IRQ Radio
- Pin 3  orange	CE	Radio
- Pin 4  yellow	CSN	Radio
- Pin 5			MAX485 TX DE/RE (RS485 direction control)
- Pin 6	 BoostSwitch (optional)			

- Pin 13 LED blinks on rx error

- Pin 16 RX2	MAX485 RX RO
- Pin 17 TX2    MAX485 RX DI
- Pin 18 TX1	MAX485 TX RO
- Pin 19 RX1	MAX485 TX DI

- Pin 50 violet	MISO Radio
- Pin 51 blue	MOSI Radio
- Pin 52 green	SCK	 Radio

Radio VCC 3.3V
MAX485 VCC 5V
MAX485 RX DE/RE (RS485 direction control) connected to GND

The Arduino Mega is used as it has several hardware serials which makes receiving telegrams more robust.
*/

#include <MySensor.h> 
#include <SPI.h>
#include <EEPROM.h>  
#include "ClickButton.h"

//#include <AltSoftSerial.h>
#include <ValloxSerial.h>

#define SET_BIT(value, place)		(value | (1 << place))
#define CLEAR_BIT(value, place)		(value & (~(1 << place)))
#define TOGGLE_BIT(value, place)		(value ^ (1 << place))

//-------------------------------------------------------------------------------------------------
// uncomment this if you only have one MAX485 for sending and receiving (not recommended)
//#define SINGLE_SERIAL_MODE // only one RS485 chip is used for rx and tx

//-------------------------------------------------------------------------------------------------
// printing diagnostics via Serial can be turned off by commenting those defines out.
#define PRINT_RECEIVED_TELEGRAMS
#define PRINT_RECEIVED_PROPERTIES
#define PRINT_TX_PROPERTIES
#define PRINT_ERRORS
#define PRINT_VALLOX_LOG
#define BLINK

//-------------------------------------------------------------------------------------------------
// if you don't have any of those you do not need the sensor data (simply set em to false)
#define OPTION_SELECT true			// bit encoded select variable
#define OPTION_HUMIDITY false		// humidity sensor 1 & 2
#define OPTION_CO2 false			// CO2 sensor
#define OPTION_HEATER true			// heater & pre heater
#define OPTION_PROGRAM true	    	// bit encoded program variable
#define OPTION_PROGRAM2 true		// bit encoded program variable 2
#define OPTION_MULTIPURPOSE1 true	// bit encoded io port 1
#define OPTION_MULTIPURPOSE2 true	// bit encoded io port 2
#define OPTION_USE_TIMER true		// sends the cached values every 3 minutes



//-------------------------------------------------------------------------------------------------
#ifdef OPTION_USE_TIMER
#include "Timer.h"
Timer timer;
#define TEMP_UPDATE_INTERVAL 3*60000
#endif

//-------------------------------------------------------------------------------------------------


// Note that AltSoftSerial uses RX=8 TX=9 PIN 10 is unsusable!
#define CE_PIN 3 // orange 
#define CS_PIN 4 // yellow

#define SERIAL_TX_CONTROL_PIN 5   //RS485 Direction control
#define RS485_TX HIGH
#define RS485_RX LOW

// boost switch button
#define BOOST_BUTTON_PIN 6
#define BOOST_FAN_SPEED 8
#define BOOST_TIME1 30 
#define BOOST_TIME2 20
#define BOOST_TIME3 30
#define BOOST_SPEED1 8 
#define BOOST_SPEED2 8
#define BOOST_SPEED3 6
#define RETRY_DELAY_MS 500 

// Diagnostic LED
#define BLINK_LED_PIN 13
#define BLINK_TIME 10

#define SERIAL_BAUDRATE 115200


//-------------------------------------------------------------------------------------------------
// Child sensor ids
const uint8_t FAN_SPEED								= 0;
const uint8_t TEMP_INSIDE							= 1;
const uint8_t TEMP_OUTSIDE							= 2;
const uint8_t TEMP_EXHAUST							= 3;
const uint8_t TEMP_INCOMMING						= 4;

const uint8_t EFFICIENCY_IN							= 5;
const uint8_t EFFICIENCY_OUT						= 6;
const uint8_t EFFICIENCY_AVERAGE					= 7;

const uint8_t POWER_STATE							= 8;
const uint8_t CO2_ADJUST_STATE						= 9;
const uint8_t HUMIDITY_ADJUST_STATE					= 10;
const uint8_t HEATING_STATE							= 11;
const uint8_t FILTER_GUARD_INDICATOR				= 12;
const uint8_t HEATING_INDICATOR						= 13;
const uint8_t FAULT_INDICATOR						= 14;
const uint8_t SERVICE_REMINDER_INDICATOR			= 15;

const uint8_t HUMIDITY								= 16;
const uint8_t BASIC_HUMIDITY_LEVEL					= 17;
const uint8_t HUMIDITY_SENSOR_1						= 18;
const uint8_t HUMIDITY_SENSOR_2						= 19;

const uint8_t CO2_HIGH								= 20;
const uint8_t CO2_LOW								= 21;
const uint8_t CO2_SET_POINT_HIGH					= 22;
const uint8_t CO2_SET_POINT_LOW						= 23;

const uint8_t FAN_SPEED_MAX							= 24;
const uint8_t FAN_SPEED_MIN							= 25;
const uint8_t DC_FAN_INPUT_ADJUSTMENT				= 26;
const uint8_t DC_FAN_OUTPUT_ADJUSTMENT				= 27;
const uint8_t INPUT_FAN_STOP_THRESHOLD				= 28;
const uint8_t HEATING_SET_POINT						= 29;
const uint8_t PRE_HEATING_SET_POINT					= 30;
const uint8_t HRC_BYPASS_THRESHOLD					= 31;
const uint8_t CELL_DEFROSTING_THRESHOLD				= 32;

// program
const uint8_t ADJUSTMENT_INTERVAL_MINUTES			= 33;
const uint8_t AUTOMATIC_HUMIDITY_LEVEL_SEEKER_STATE = 34;
const uint8_t BOOST_SWITCH_MODE						= 35;
const uint8_t RADIATOR_TYPE							= 36;
const uint8_t CASCADE_ADJUST						= 37;

// program2
const uint8_t MAX_SPEED_LIMIT_MODE					= 38;

const uint8_t SERVICE_REMINDER						= 39;

//-------------------------------------------------------------------------------------------------
const uint8_t BOOST_SWITCH							= 40;

//-------------------------------------------------------------------------------------------------
// multi purpose 1
const uint8_t POST_HEATING_ON						= 41;

// multi purpose 2
const uint8_t DAMPER_MOTOR_POSITION					= 42;
const uint8_t FAULT_SIGNAL_RELAY					= 43;
const uint8_t SUPPLY_FAN_OFF						= 44;
const uint8_t PRE_HEATING_ON						= 45;
const uint8_t EXHAUST_FAN_OFF						= 46;
const uint8_t FIRE_PLACE_BOOSTER_ON					= 47;

const uint8_t INCOMMING_CURRENT						= 48;

const uint8_t LAST_ERROR_NUMBER						= 49;
//-------------------------------------------------------------------------------------------------

struct ChildSensor
{
	bool    active;
	uint8_t childSensorId;
	uint8_t variableTypeId;
	uint8_t valueTypeId;
	const char* name;
};

//-------------------------------------------------------------------------------------------------
static ChildSensor CHILD_SENSORS[] =
{
	//active, child id, type, value type, name
	{ true, FAN_SPEED, S_DIMMER, V_DIMMER, "Fan speed" },
	{ true, TEMP_INSIDE, S_TEMP, V_TEMP, "Temp inside" },
	{ true, TEMP_OUTSIDE, S_TEMP, V_TEMP, "Temp outside" },
	{ true, TEMP_EXHAUST, S_TEMP, V_TEMP, "Temp exhaust" },
	{ true, TEMP_INCOMMING, S_TEMP, V_TEMP, "Temp incomming" },

	{ true, EFFICIENCY_IN, S_CUSTOM, V_VAR1, "Efficiency in" },
	{ true, EFFICIENCY_OUT, S_CUSTOM, V_VAR1, "Efficiency out" },
	{ true, EFFICIENCY_AVERAGE, S_CUSTOM, V_VAR1, "Efficiency average" },

	// select
	{ OPTION_SELECT, POWER_STATE, S_LIGHT, V_LIGHT, "Power state" },
	{ OPTION_SELECT, CO2_ADJUST_STATE, S_LIGHT, V_LIGHT, "CO2 adjust state" },
	{ OPTION_SELECT, HUMIDITY_ADJUST_STATE, S_LIGHT, V_LIGHT, "Humidity adjust state" },
	{ OPTION_SELECT, HEATING_STATE, S_LIGHT, V_LIGHT, "Heating state" },

	{ OPTION_SELECT, FILTER_GUARD_INDICATOR, S_CUSTOM, V_VAR1, "Filter guard indicator" },
	{ OPTION_SELECT, HEATING_INDICATOR, S_CUSTOM, V_VAR1, "Heating indicator" },
	{ OPTION_SELECT, FAULT_INDICATOR, S_CUSTOM, V_VAR1, "Fault indicator" },
	{ OPTION_SELECT, SERVICE_REMINDER_INDICATOR, S_CUSTOM, V_VAR1, "Serivce reminder indicator" },

	{ OPTION_HUMIDITY, HUMIDITY, S_HUM, V_HUM, "Humidity" },
	{ OPTION_HUMIDITY, BASIC_HUMIDITY_LEVEL, S_HUM, V_HUM, "Basic humidity level" },
	{ OPTION_HUMIDITY, HUMIDITY_SENSOR_1, S_HUM, V_HUM, "Humidity sensor 1" },
	{ OPTION_HUMIDITY, HUMIDITY_SENSOR_2, S_HUM, V_HUM, "Humidity sensor 2" },
	{ OPTION_CO2, CO2_HIGH, S_CUSTOM, V_VAR1, "CO2 high" },
	{ OPTION_CO2, CO2_LOW, S_CUSTOM, V_VAR1, "CO2 low" },
	{ OPTION_CO2, CO2_SET_POINT_HIGH, S_CUSTOM, V_VAR1, "CO2 set point high" },
	{ OPTION_CO2, CO2_SET_POINT_LOW, S_CUSTOM, V_VAR1, "CO2 set point low" },

	{ true, FAN_SPEED_MAX, S_DIMMER, V_DIMMER, "Fan speed max" },
	{ true, FAN_SPEED_MIN, S_DIMMER, V_DIMMER, "Fan speed min" },
	{ true, DC_FAN_INPUT_ADJUSTMENT, S_DIMMER, V_DIMMER, "DC fan input adjustment" },
	{ true, DC_FAN_OUTPUT_ADJUSTMENT, S_DIMMER, V_DIMMER, "DC fan output adjustment" },
	{ true, INPUT_FAN_STOP_THRESHOLD, S_DIMMER, V_DIMMER, "Input fan stop threshold" },
	{ OPTION_HEATER, HEATING_SET_POINT, S_DIMMER, V_DIMMER, "Heating setpoint" },
	{ OPTION_HEATER, PRE_HEATING_SET_POINT, S_DIMMER, V_DIMMER, "Pre heating setpoint" },
	{ true, HRC_BYPASS_THRESHOLD, S_DIMMER, V_DIMMER, "HRC bypass threshold" },
	{ OPTION_HEATER, CELL_DEFROSTING_THRESHOLD, S_DIMMER, V_DIMMER, "Cell defrosting threshold" },

	// program
	{ OPTION_PROGRAM, ADJUSTMENT_INTERVAL_MINUTES, S_CUSTOM, V_VAR1, "Adjustment interval minutes" },
	{ OPTION_PROGRAM, AUTOMATIC_HUMIDITY_LEVEL_SEEKER_STATE, S_CUSTOM, V_VAR1, "Automatic humidity level seeker state" },
	{ OPTION_PROGRAM, BOOST_SWITCH_MODE, S_CUSTOM, V_VAR1, "Boost switch mode" },
	{ OPTION_PROGRAM, RADIATOR_TYPE, S_CUSTOM, V_VAR1, "Radiator type" },
	{ OPTION_PROGRAM, CASCADE_ADJUST, S_CUSTOM, V_VAR1, "Cascade adjust" },

	// program2
	{ OPTION_PROGRAM2, MAX_SPEED_LIMIT_MODE, S_CUSTOM, V_VAR1, "Max speed limit mode" },
	
	{ true, SERVICE_REMINDER, S_CUSTOM, V_VAR1, "Service reminder" },

	// non vallox feature: boost switch with caller supplied minutes
	{ true, BOOST_SWITCH, S_DIMMER, V_DIMMER, "Boost switch minutes" },


	// multi purpose 1
	{ OPTION_MULTIPURPOSE1, POST_HEATING_ON, S_CUSTOM, V_VAR1, "Post heating on" },

	// multi purpose 2
	{ OPTION_MULTIPURPOSE2, DAMPER_MOTOR_POSITION, S_CUSTOM, V_VAR1, "Post heating on" },
	{ OPTION_MULTIPURPOSE2, FAULT_SIGNAL_RELAY, S_CUSTOM, V_VAR1, "Fault signal relay" },
	{ OPTION_MULTIPURPOSE2, SUPPLY_FAN_OFF, S_CUSTOM, V_VAR1, "Supply fan off" },
	{ OPTION_MULTIPURPOSE2, PRE_HEATING_ON, S_CUSTOM, V_VAR1, "Pre heating on" },
	{ OPTION_MULTIPURPOSE2, EXHAUST_FAN_OFF, S_CUSTOM, V_VAR1, "Exhaust fan off" },
	{ OPTION_MULTIPURPOSE2, FIRE_PLACE_BOOSTER_ON, S_CUSTOM, V_VAR1, "Fire place booster on" },

	{ true, INCOMMING_CURRENT, S_CUSTOM, V_VAR1, "Incomming current" },

	{ true, LAST_ERROR_NUMBER, S_CUSTOM, V_VAR1, "Last error number" },
};
static uint8_t CHILD_SENSORS_COUNT = sizeof(CHILD_SENSORS) / sizeof(ChildSensor);


//-------------------------------------------------------------------------------------------------
// here you can select which variables are polled
static ValloxProperty PROPERTIES_TO_OBSERVE[] =
{
	// those are broadcasted periodically
	//FanSpeedProperty,
	//TempInsideProperty,
	//TempOutsideProperty,
	//TempExhaustProperty,
	//TempIncommingProperty,

	// status bits
	SelectStatusProperty,
	////PowerStateProperty,
	////CO2AdjustStateProperty,
	////HumidityAdjustStateProperty,
	////HeatingStateProperty,
	////FilterGuardIndicatorProperty,
	////HeatingIndicatorProperty,
	////FaultIndicatorProperty,
	////ServiceReminderIndicatorProperty,
	
	// we do not have a humidity sensor
	//HumidityProperty,
	//BasicHumidityLevelProperty,
	//HumiditySensor1Property,
	//HumiditySensor2Property,

	// we do not have a CO2 sensor
	//CO2HighProperty,
	//CO2LowProperty,
	//CO2SetPointHighProperty,
	//CO2SetPointLowProperty,

	FanSpeedMaxProperty,
	FanSpeedMinProperty,
	DCFanInputAdjustmentProperty,
	DCFanOutputAdjustmentProperty,
	InputFanStopThresholdProperty,

	HeatingSetPointProperty,
	PreHeatingSetPointProperty,
	HrcBypassThresholdProperty,
	CellDefrostingThresholdProperty,

	// program
	ProgramProperty,
	////AdjustmentIntervalMinutesProperty,
	////AutomaticHumidityLevelSeekerStateProperty,
	////BoostSwitchModeProperty,
	////RadiatorTypeProperty,
	////CascadeAdjustProperty,
	
	// program2
	Program2Property,
	////MaxSpeedLimitModeProperty,
	
	ServiceReminderProperty,

	// ioport multi purpose 1
	IoPortMultiPurpose1Property,
	////PostHeatingOnProperty,

	// ioport multi purpose 2
	IoPortMultiPurpose2Property,
	////DamperMotorPositionProperty,
	////FaultSignalRelayProperty,
	////SupplyFanOffProperty,
	////PreHeatingOnProperty,
	////ExhaustFanOffProperty,
	////FirePlaceBoosterOnProperty,

	IncommingCurrentProperty,
	LastErrorNumberProperty
};
static uint8_t PROPERTIES_TO_OBSERVE_COUNT = sizeof(PROPERTIES_TO_OBSERVE) / sizeof(ValloxProperty);

//-------------------------------------------------------------------------------------------------
// The active mode makes only sense when a hardware serial is used
static bool OBSERVE_PROPERTIES_ACTIVE = true;
static unsigned long OBSERVE_PROPERTIES_INTERVAL_MS = 5000;
uint8_t nextVariableToPollIndex = 0;
unsigned long lastPollMillis = 0;

MySensor gw(CE_PIN, CS_PIN);
ValloxSerial valloxSerial;

ClickButton BoostButton(BOOST_BUTTON_PIN, LOW, CLICKBTN_PULLUP);
bool boostModeActive = false;
unsigned long boostEndMillis = 0;
uint8_t boostRemainingMinutes = 0;

// cached values
int8_t lastFanSpeedValue;
int8_t lastTempInsideValue;
int8_t lastTempOutsideValue;
int8_t lastTempExhaustValue;
int8_t lastTempIncommingValue;

int8_t lastInEfficiencyValue;
int8_t lastOutEfficiencyValue;
int8_t lastAverageEfficiencyValue;

//-------------------------------------------------------------------------------------------------
// Note that AltSoftSerial uses RX=8 TX=9 PIN 10 is unsusable!
//#define SSerialRX        5 // 10  //Serial Receive pin
//#define SSerialTX        6 // 11  //Serial Transmit pin
// SoftwareSerial can not be used as it conflicts with MySensor.h/PinChangeInt.h
//SoftwareSerial softSerial(SSerialRX, SSerialTX); 
// As MySensors uses Serial and the Uno only has one HardwareSerial UART we must use this one on the Uno
// Unfortunately we loose a lot of telegrams as there is no hardware rx buffer. Better use the Mega
//AltSoftSerial softSerial;

void setupSerials(Stream** ppRxStream, Stream** ppTxStream)
{
	Serial.begin(SERIAL_BAUDRATE); // use used for diagnostic purposes e.g. Debugging

	Serial1.begin(VALLOX_BAUDRATE);
	*ppRxStream = &Serial1;

#ifdef SINGLE_SERIAL_MODE
	valloxSerial.setTxSerial(Serial1); // as this is the same we have to use the SERIAL_TX_CONTROL_PIN for controlling the direction
	*ppTxStream = &Serial1; 
#else
	Serial2.begin(VALLOX_BAUDRATE);
	*ppTxStream = &Serial2;
#endif
}


//-------------------------------------------------------------------------------------------------
void setupSensor()
{
	gw.begin(incomingMessage, AUTO, true);
	gw.sendSketchInfo("Vallox Digit SE", "2.4");

	for (uint8_t i = 0; i < CHILD_SENSORS_COUNT; i++)
	{
		ChildSensor& childSensor = CHILD_SENSORS[i];
		if (childSensor.active)
		{
			gw.present(childSensor.childSensorId, childSensor.variableTypeId);
		}
	}
}


//-------------------------------------------------------------------------------------------------
void setupVallox(Stream* pRxStream, Stream* pTxStream)
{
	valloxSerial.setRxSerial(*pRxStream);
	valloxSerial.setTxSerial(*pTxStream);

	// serial
	valloxSerial.setSenderId(VALLOX_ADDRESS_PANEL2);

	valloxSerial.attachPropertyChanged(onPropertyChanged);
	valloxSerial.attach(onStartSending, onStopSending);

	// for debugging purposes
	valloxSerial.attachLogger(onLog);
	valloxSerial.attach(onTelegramReceived);
	valloxSerial.attach(onTelegramChecksumFailure);
	valloxSerial.attach(onUnexpectedByteReceived);
	valloxSerial.attach(onSuspended);
}

//-------------------------------------------------------------------------------------------------
void setupBoostButton()
{
	// Setup button timers (all in milliseconds / ms)
	// (These are default if not set, but changeable for convenience)
	BoostButton.debounceTime = 20;   // Debounce timer in ms
	BoostButton.multiclickTime = 250;  // Time limit for multi clicks
	BoostButton.longClickTime = 1000; // time until "held-down clicks" register
}

void startBoost(float minutes, int8_t fanSpeed = BOOST_FAN_SPEED)
{
	Serial.print("Starting boost mode ");
	Serial.print((uint8_t)minutes, DEC);
	Serial.println("m");

	int8_t maxFanSpeed = valloxSerial.getValue(FanSpeedMaxProperty);
	if (fanSpeed > maxFanSpeed)
	{
		fanSpeed = maxFanSpeed;
	}
	valloxSerial.setFanSpeed(fanSpeed);
		
	boostEndMillis = millis() + (minutes * 60 * 1000);
	boostModeActive = true;
}

void stopBoost()
{
	if (boostModeActive)
	{
		Serial.println("Stopping boost mode");
		
		int8_t minFanSpeed = valloxSerial.getValue(FanSpeedMinProperty);
		valloxSerial.setFanSpeed(minFanSpeed);

		boostEndMillis = 0;
		boostModeActive = false;
	}
}

void updateRemainingBoostTime()
{
	unsigned long now = millis();
	unsigned long remainingMillis = 0;
	if (now < boostEndMillis)
	{
		remainingMillis = boostEndMillis - now;
	}

	uint8_t remainingMinutes = remainingMillis / 1000 / 60;
	if (remainingMinutes != boostRemainingMinutes)
	{
		boostRemainingMinutes = remainingMinutes;
		sendMessage(BOOST_SWITCH, boostRemainingMinutes);
	}
}

void updateBoostTime()
{
	updateRemainingBoostTime();
	if (boostRemainingMinutes == 0)
	{
		stopBoost();
	}
}



//-------------------------------------------------------------------------------------------------
void onPropertyChanged(ValloxProperty propertyId, int8_t value)
{
	switch (propertyId)
	{
		// cached values
	case FanSpeedProperty:
	{
		sendMessage(FAN_SPEED, value);
		lastFanSpeedValue = value;
		break;
	}
	case TempInsideProperty:
	{
		sendMessage(TEMP_INSIDE, value);
		lastTempInsideValue = value;
		break;
	}
	case TempOutsideProperty:
	{
		sendMessage(TEMP_OUTSIDE, value);
		lastTempOutsideValue = value;
		break;
	}
	case TempExhaustProperty:
	{
		sendMessage(TEMP_EXHAUST, value);
		lastTempExhaustValue = value;
		break;
	}
	case TempIncommingProperty:
	{
		sendMessage(TEMP_INCOMMING, value);
		lastTempIncommingValue = value;
		break;
	}

	case InEfficiencyProperty:
	{
		sendMessage(EFFICIENCY_IN, value);
		lastInEfficiencyValue = value;
		break;
	}
	case OutEfficiencyProperty:
	{
		sendMessage(EFFICIENCY_OUT, value);
		lastOutEfficiencyValue = value;
		break;
	}
	case AverageEfficiencyProperty:
	{
		sendMessage(EFFICIENCY_AVERAGE, value);
		lastAverageEfficiencyValue = value;
		break;
	}


	// non cahced values
	case PowerStateProperty:
	{
		sendMessage(POWER_STATE, value);
		break;
	}
	case CO2AdjustStateProperty:
	{
		sendMessage(CO2_ADJUST_STATE, value);
		break;
	}
	case HumidityAdjustStateProperty:
	{
		sendMessage(HUMIDITY_ADJUST_STATE, value);
		break;
	}
	case HeatingStateProperty:
	{
		sendMessage(HEATING_STATE, value);
		break;
	}
	case FilterGuardIndicatorProperty:
	{
		sendMessage(FILTER_GUARD_INDICATOR, value);
		break;
	}
	case HeatingIndicatorProperty:
	{
		sendMessage(HEATING_INDICATOR, value);
		break;
	}
	case FaultIndicatorProperty:
	{
		sendMessage(FAULT_INDICATOR, value);
		break;
	}
	case ServiceReminderIndicatorProperty:
	{
		sendMessage(SERVICE_REMINDER_INDICATOR, value);
		break;
	}
	case HumidityProperty:
	{
		sendMessage(HUMIDITY, value);
		break;
	}
	case BasicHumidityLevelProperty:
	{
		sendMessage(BASIC_HUMIDITY_LEVEL, value);
		break;
	}
	case HumiditySensor1Property:
	{
		sendMessage(HUMIDITY_SENSOR_1, value);
		break;
	}
	case HumiditySensor2Property:
	{
		sendMessage(HUMIDITY_SENSOR_2, value);
		break;
	}
	case CO2HighProperty:
	{
		sendMessage(CO2_HIGH, value);
		break;
	}
	case CO2LowProperty:
	{
		sendMessage(CO2_LOW, value);
		break;
	}
	case CO2SetPointHighProperty:
	{
		sendMessage(CO2_SET_POINT_HIGH, value);
		break;
	}
	case CO2SetPointLowProperty:
	{
		sendMessage(CO2_SET_POINT_LOW, value);
		break;
	}
	case FanSpeedMaxProperty:
	{
		sendMessage(FAN_SPEED_MAX, value);
		break;
	}
	case FanSpeedMinProperty:
	{
		sendMessage(FAN_SPEED_MIN, value);
		break;
	}
	case DCFanInputAdjustmentProperty:
	{
		sendMessage(DC_FAN_INPUT_ADJUSTMENT, value);
		break;
	}
	case DCFanOutputAdjustmentProperty:
	{
		sendMessage(DC_FAN_OUTPUT_ADJUSTMENT, value);
		break;
	}
	case InputFanStopThresholdProperty:
	{
		sendMessage(INPUT_FAN_STOP_THRESHOLD, value);
		break;
	}
	case HeatingSetPointProperty:
	{
		sendMessage(HEATING_SET_POINT, value);
		break;
	}
	case PreHeatingSetPointProperty:
	{
		sendMessage(PRE_HEATING_SET_POINT, value);
		break;
	}
	case HrcBypassThresholdProperty:
	{
		sendMessage(HRC_BYPASS_THRESHOLD, value);
		break;
	}
	case CellDefrostingThresholdProperty:
	{
		sendMessage(CELL_DEFROSTING_THRESHOLD, value);
		break;
	}
	case AdjustmentIntervalMinutesProperty:
	{
		sendMessage(ADJUSTMENT_INTERVAL_MINUTES, value);
		break;
	}
	case AutomaticHumidityLevelSeekerStateProperty:
	{
		sendMessage(AUTOMATIC_HUMIDITY_LEVEL_SEEKER_STATE, value);
		break;
	}
	case BoostSwitchModeProperty:
	{
		sendMessage(BOOST_SWITCH_MODE, value);
		break;
	}
	case RadiatorTypeProperty:
	{
		sendMessage(RADIATOR_TYPE, value);
		break;
	}
	case CascadeAdjustProperty:
	{
		sendMessage(CASCADE_ADJUST, value);
		break;
	}
	case MaxSpeedLimitModeProperty:
	{
		sendMessage(MAX_SPEED_LIMIT_MODE, value);
		break;
	}
	case ServiceReminderProperty:
	{
		sendMessage(SERVICE_REMINDER, value);
		break;
	}
	case PostHeatingOnProperty:
	{
		sendMessage(POST_HEATING_ON, value);
		break;
	}
	case DamperMotorPositionProperty:
	{
		sendMessage(DAMPER_MOTOR_POSITION, value);
		break;
	}	
	case FaultSignalRelayProperty:
	{
		sendMessage(FAULT_SIGNAL_RELAY, value);
		break;
	}
	case SupplyFanOffProperty:
	{
		sendMessage(SUPPLY_FAN_OFF, value);
		break;
	}
	case PreHeatingOnProperty:
	{
		sendMessage(PRE_HEATING_ON, value);
		break;
	}
	case ExhaustFanOffProperty:
	{
		sendMessage(EXHAUST_FAN_OFF, value);
		break;
	}
	case FirePlaceBoosterOnProperty:
	{
		sendMessage(FIRE_PLACE_BOOSTER_ON, value);
		break;
	}
	case IncommingCurrentProperty:
	{
		sendMessage(INCOMMING_CURRENT, value);
		break;
	}
	case LastErrorNumberProperty:
	{
		sendMessage(LAST_ERROR_NUMBER, value);
		break;
	}

	default:
#ifdef PRINT_RECEIVED_PROPERTIES
		Serial.print("Property ");
		Serial.print(propertyId, DEC);
		Serial.print(" changed ");
		Serial.println(value, DEC);
#endif
		break;
	}
}

void onStartSending()
{
	digitalWrite(SERIAL_TX_CONTROL_PIN, RS485_TX);
}

void onStopSending()
{
	digitalWrite(SERIAL_TX_CONTROL_PIN, RS485_RX);
}

//-------------------------------------------------------------------------------------------------
inline void sendMessage(uint8_t childSensorId, int8_t value)
{
	ChildSensor& childSensor = CHILD_SENSORS[childSensorId];
	if (childSensor.active)
	{
#ifdef PRINT_RECEIVED_PROPERTIES
		Serial.print(childSensor.name);
		Serial.print(" ");
		Serial.println(value, DEC);
#endif
		uint8_t variableType = childSensor.valueTypeId;
		MyMessage message(childSensorId, variableType);
		message.set(value);
		gw.send(message);
	}
}

//-------------------------------------------------------------------------------------------------
void incomingMessage(const MyMessage &message)
{
	if (message.sensor == FAN_SPEED && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(FanSpeedProperty);

		Serial.print("Changing fan speed from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setFanSpeed(requestedLevel);
	}

	if (message.sensor == FAN_SPEED_MAX && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(FanSpeedMaxProperty);

		Serial.print("Changing fan speed max from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setFanSpeedMax(requestedLevel);
	}

	if (message.sensor == FAN_SPEED_MIN && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(FanSpeedMinProperty);

		Serial.print("Changing fan speed min from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setFanSpeedMin(requestedLevel);
	}

	if (message.sensor == DC_FAN_INPUT_ADJUSTMENT && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(DCFanInputAdjustmentProperty);

		Serial.print("Changing DC fan input adjustment from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setDCFanInputAdjustment(requestedLevel);
	}

	if (message.sensor == DC_FAN_OUTPUT_ADJUSTMENT && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(DCFanOutputAdjustmentProperty);

		Serial.print("Changing DC fan output adjustment from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setDCFanOutputAdjustment(requestedLevel);
	}

	if (message.sensor == INPUT_FAN_STOP_THRESHOLD && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(InputFanStopThresholdProperty);

		Serial.print("Changing input fan stop temperature setpoint from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setInputFanStopThreshold(requestedLevel);
	}

	if (message.sensor == HEATING_SET_POINT && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(HeatingSetPointProperty);

		Serial.print("Changing heating temperature setpoint from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setHeatingSetPoint(requestedLevel);
	}
	
	if (message.sensor == PRE_HEATING_SET_POINT && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(PreHeatingSetPointProperty);

		Serial.print("Changing preheating temperature setpoint from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setPreHeatingSetPoint(requestedLevel);
	}
	
	if (message.sensor == CELL_DEFROSTING_THRESHOLD && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(CellDefrostingThresholdProperty);

		Serial.print("Changing cell defrosting temperature setpoint from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setCellDefrostingThreshold(requestedLevel);
	}
	
	if (message.sensor == HRC_BYPASS_THRESHOLD && message.type == V_DIMMER)
	{
		int requestedLevel = atoi(message.data);

#ifdef 	PRINT_TX_PROPERTIES
		int8_t currentLevel = valloxSerial.getValue(HrcBypassThresholdProperty);

		Serial.print("Changing HRC bypass temperature setpoint from ");
		Serial.print(currentLevel);
		Serial.print(", to ");
		Serial.println(requestedLevel);
#endif

		valloxSerial.setHrcBypassThreshold(requestedLevel);
	}

	if (message.sensor == POWER_STATE && message.type == V_LIGHT)
	{
		int8_t select = valloxSerial.getValue(SelectStatusProperty);

		if (message.getBool())
		{
			select = SET_BIT(select, 0);
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("Power on (select = ");
			Serial.print(select);
			Serial.println(")");
#endif
		}
		else
		{
			select = CLEAR_BIT(select, 0);
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("Power off (select = ");
			Serial.print(select);
			Serial.println(")");
#endif
		}

		valloxSerial.setSelectStatus(select);
	}

	if (message.sensor == CO2_ADJUST_STATE && message.type == V_LIGHT)
	{
		int8_t select = valloxSerial.getValue(SelectStatusProperty);

		if (message.getBool())
		{
			select = SET_BIT(select, 1);
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("CO2 adjust on (select = ");
			Serial.print(select);
			Serial.println(")");
#endif
		}
		else
		{
			select = CLEAR_BIT(select, 1);
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("CO2 adjust off (select = ");
			Serial.print(select);
			Serial.println(")");
#endif
		}

		valloxSerial.setSelectStatus(select);
	}

	if (message.sensor == HUMIDITY_ADJUST_STATE && message.type == V_LIGHT)
	{
		int8_t select = valloxSerial.getValue(SelectStatusProperty);

		if (message.getBool())
		{
			select = SET_BIT(select, 2);
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("Humidity adjust on (select = ");
			Serial.print(select);
			Serial.println(")");
#endif
		}
		else
		{
			select = CLEAR_BIT(select, 2);
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("Humidity adjust off (select = ");
			Serial.print(select);
			Serial.println(")");
#endif
		}

		valloxSerial.setSelectStatus(select);
	}


	if (message.sensor == HEATING_STATE && message.type == V_LIGHT)
	{
		int8_t select = valloxSerial.getValue(SelectStatusProperty);

		if (message.getBool())
		{
			select = SET_BIT(select, 3);
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("Heating state on (select = ");
			Serial.print(select);
			Serial.println(")");
#endif
		}
		else
		{
			select = CLEAR_BIT(select, 3);
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("Heating state off (select = ");
			Serial.print(select);
			Serial.println(")");
#endif
		}

		valloxSerial.setSelectStatus(select);
	}


	if (message.sensor == BOOST_SWITCH && message.type == V_DIMMER)
	{
		int minutes = atoi(message.data);

		if (minutes == 0)
		{
#ifdef 	PRINT_TX_PROPERTIES
			Serial.println("Stopping boost mode");
#endif
			stopBoost();
		}
		else
		{
#ifdef 	PRINT_TX_PROPERTIES
			Serial.print("Starting boost mode for ");
			Serial.print(minutes);
			Serial.println(" minutes");
#endif
			startBoost(minutes, BOOST_FAN_SPEED);
		}
	}
}


//-------------------------------------------------------------------------------------------------
// diagnostic callbacks
bool onTelegramReceived(uint8_t sender, uint8_t receiver, uint8_t command, uint8_t arg)
{
#ifdef PRINT_RECEIVED_TELEGRAMS
	Serial.print("Received telegram from ");
	Serial.print(sender, HEX);
	Serial.print("->");
	Serial.print(receiver, HEX);
	Serial.print(" command=");
	Serial.print(command, HEX);
	Serial.print(" arg=");
	Serial.print(arg, HEX);
	Serial.println("");
#endif
	return true;
}

//-------------------------------------------------------------------------------------------------
void onTelegramChecksumFailure(uint8_t sender, uint8_t receiver, uint8_t command, uint8_t arg, uint8_t checksum)
{
#ifdef PRINT_ERRORS
	Serial.print("Dropped telegram as checksum is wrong ");
	Serial.print(sender, DEC);
	Serial.print(receiver, DEC);
	Serial.print(command, DEC);
	Serial.print(arg, DEC);
	Serial.println(checksum, DEC);
#endif
	blink(BLINK_TIME);
}


//-------------------------------------------------------------------------------------------------
void onUnexpectedByteReceived(uint8_t receivedByte)
{
#ifdef PRINT_ERRORS
	Serial.print("Dropped ");
	Serial.print(receivedByte, HEX);
	Serial.println("");
#endif
	blink(BLINK_TIME);
}


//-------------------------------------------------------------------------------------------------
void onSuspended(bool suspended)
{
	if (suspended)
	{
		Serial.print("Communication suspended due to C02 sensor communication");
	}
	else
	{
		Serial.print("Communication resumed due to C02 sensor communication");
	}
}


//-------------------------------------------------------------------------------------------------
void onLog(const char* message)
{
#ifdef PRINT_VALLOX_LOG
	Serial.println(message);
#endif
}


//-------------------------------------------------------------------------------------------------
void blink(int duration)
{
#ifdef BLINK
	digitalWrite(BLINK_LED_PIN, HIGH);
	delay(duration);
	digitalWrite(BLINK_LED_PIN, LOW);
#endif
}


//-------------------------------------------------------------------------------------------------
void setup()
{
	Stream* pTxStream = NULL;
	Stream* pRxStream = NULL;
	setupSerials(&pRxStream, &pTxStream);

	pinMode(BLINK_LED_PIN, OUTPUT);

	// the RS485 Adapter has a read write enable line which has to be set before sending data.
	pinMode(SERIAL_TX_CONTROL_PIN, OUTPUT);
	digitalWrite(SERIAL_TX_CONTROL_PIN, RS485_RX);

	nextVariableToPollIndex = 0;
	lastPollMillis = millis();

	setupSensor();
	setupVallox(pRxStream, pTxStream);
	setupBoostButton();

#ifdef OPTION_USE_TIMER
	/*int tickEvent1 = */timer.every(TEMP_UPDATE_INTERVAL, sendValuesTimerHandler);
	Serial.println("Started cyclic update timer.");
#endif
}

#ifdef OPTION_USE_TIMER
void sendValuesTimerHandler()
{
	Serial.println("Sending cached values:");
	sendMessage(FAN_SPEED, lastFanSpeedValue);
	sendMessage(TEMP_INSIDE, lastTempInsideValue);
	sendMessage(TEMP_OUTSIDE, lastTempOutsideValue);
	sendMessage(TEMP_EXHAUST, lastTempExhaustValue);
	sendMessage(TEMP_INCOMMING, lastTempIncommingValue);

	sendMessage(EFFICIENCY_IN, lastInEfficiencyValue);
	sendMessage(EFFICIENCY_OUT, lastOutEfficiencyValue);
	sendMessage(EFFICIENCY_AVERAGE, lastAverageEfficiencyValue);
}
#endif

//-------------------------------------------------------------------------------------------------
void loop()
{
	// MySensors update
	gw.process();

	// Vallox RS485 RX
	if (valloxSerial.receive())
	{
		valloxSerial.calculateResults();
	}

	// Vallox RS485 TX
	if (OBSERVE_PROPERTIES_ACTIVE)
	{
		unsigned long now = millis();
		if (now > lastPollMillis)
		{
			unsigned long delay = now - lastPollMillis;
			if (delay >= OBSERVE_PROPERTIES_INTERVAL_MS)
			{
				ValloxProperty propertyId = PROPERTIES_TO_OBSERVE[nextVariableToPollIndex];

#ifdef PRINT_TX_PROPERTIES
				Serial.print("Polling ");
				Serial.println(propertyId, DEC);
#endif
				valloxSerial.poll(propertyId);
				
				nextVariableToPollIndex++;
				if (nextVariableToPollIndex == PROPERTIES_TO_OBSERVE_COUNT)
				{
					nextVariableToPollIndex = 0;
				}

				lastPollMillis = now;
			}
		}
		else
		{
			lastPollMillis = now; // avoid problems when millis run over
		}
	}

	// Vallox Button handling
	BoostButton.Update();
	if (BoostButton.clicks != 0)
	{
		switch (BoostButton.clicks)
		{
		case 1: // click
			{
				startBoost(BOOST_TIME1, BOOST_SPEED1);
			}
			break;
		case 2: // double click
			{
				startBoost(BOOST_TIME2, BOOST_SPEED2);
			}
		break;
		case 3: // tripple click
			{
				startBoost(BOOST_TIME3, BOOST_SPEED3);
			}
			break;
		case -1: // long click
			{
				stopBoost();
			}
			break;

		default:
			break;
		}
	}

	updateBoostTime();

#ifdef OPTION_USE_TIMER
	timer.update();
#endif
}
