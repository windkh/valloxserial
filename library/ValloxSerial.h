#ifndef ValloxSerial_h
#define ValloxSerial_h

#include <ValloxProtocol.h>
#include <Stream.h>
#include <inttypes.h>

// uncomment this to reduce footprint.
//#define MINIMUM_PROPERTIES

extern "C" {
	enum ValloxProperty
	{
		// sensor data 
		FanSpeedProperty							= 0,  // VALLOX_VARIABLE_FAN_SPEED
		TempInsideProperty							= 1,  // VALLOX_VARIABLE_TEMP_INSIDE
		TempOutsideProperty							= 2,  // VALLOX_VARIABLE_TEMP_OUTSIDE
		TempExhaustProperty							= 3,  // VALLOX_VARIABLE_TEMP_EXHAUST
		TempIncommingProperty						= 4,  // VALLOX_VARIABLE_TEMP_INCOMMING

		// status bits
		PowerStateProperty							= 5,  // VALLOX_VARIABLE_SELECT
		CO2AdjustStateProperty						= 6,  // VALLOX_VARIABLE_SELECT
		HumidityAdjustStateProperty					= 7,  // VALLOX_VARIABLE_SELECT
		HeatingStateProperty						= 8,  // VALLOX_VARIABLE_SELECT
		FilterGuardIndicatorProperty				= 9,  // VALLOX_VARIABLE_SELECT
		HeatingIndicatorProperty					= 10, // VALLOX_VARIABLE_SELECT
		FaultIndicatorProperty						= 11, // VALLOX_VARIABLE_SELECT
		ServiceReminderIndicatorProperty			= 12, // VALLOX_VARIABLE_SELECT

		HumidityProperty							= 13, // VALLOX_VARIABLE_HUMIDITY
		BasicHumidityLevelProperty					= 14, // VALLOX_VARIABLE_BASIC_HUMIDITY_LEVEL
		HumiditySensor1Property						= 15, // VALLOX_VARIABLE_HUMIDITY_SENSOR1
		HumiditySensor2Property						= 16, // VALLOX_VARIABLE_HUMIDITY_SENSOR2

		CO2HighProperty								= 17, // VALLOX_VARIABLE_CO2_HIGH
		CO2LowProperty								= 18, // VALLOX_VARIABLE_CO2_LOW
		CO2SetPointHighProperty						= 19, // VALLOX_VARIABLE_CO2_SET_POINT_UPPER
		CO2SetPointLowProperty						= 20, // VALLOX_VARIABLE_CO2_SET_POINT_LOWER

		FanSpeedMaxProperty							= 21, // VALLOX_VARIABLE_FAN_SPEED_MAX
		FanSpeedMinProperty							= 22, // VALLOX_VARIABLE_FAN_SPEED_MIN
		DCFanInputAdjustmentProperty				= 23, // VALLOX_VARIABLE_DC_FAN_INPUT_ADJUSTMENT
		DCFanOutputAdjustmentProperty				= 24, // VALLOX_VARIABLE_DC_FAN_OUTPUT_ADJUSTMENT
		InputFanStopThresholdProperty				= 25, // VALLOX_VARIABLE_INPUT_FAN_STOP
		HeatingSetPointProperty						= 26, // VALLOX_VARIABLE_HEATING_SET_POINT
		PreHeatingSetPointProperty					= 27, // VALLOX_VARIABLE_PRE_HEATING_SET_POINT
		HrcBypassThresholdProperty					= 28, // VALLOX_VARIABLE_HRC_BYPASS
		CellDefrostingThresholdProperty				= 29, // VALLOX_VARIABLE_CELL_DEFROSTING

		// program
		AdjustmentIntervalMinutesProperty			= 30, // VALLOX_VARIABLE_PROGRAM
		AutomaticHumidityLevelSeekerStateProperty	= 31, // VALLOX_VARIABLE_PROGRAM
		BoostSwitchModeProperty						= 32, // VALLOX_VARIABLE_PROGRAM
		RadiatorTypeProperty						= 33, // VALLOX_VARIABLE_PROGRAM
		CascadeAdjustProperty						= 34, // VALLOX_VARIABLE_PROGRAM

		// program2
		MaxSpeedLimitModeProperty					= 35, // VALLOX_VARIABLE_PROGRAM2

		ServiceReminderProperty						= 36, // VALLOX_VARIABLE_SERVICE_REMINDER

		// ioport multi purpose 1
		PostHeatingOnProperty						= 37, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_1

		// ioport multi purpose 2
		DamperMotorPositionProperty					= 38, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2
		FaultSignalRelayProperty					= 39, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2
		SupplyFanOffProperty						= 50, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2
		PreHeatingOnProperty						= 41, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2
		ExhaustFanOffProperty						= 42, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2
		FirePlaceBoosterOnProperty					= 43, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2

		IncommingCurrentProperty					= 44, // VALLOX_VARIABLE_CURRENT_INCOMMING
		LastErrorNumberProperty                     = 45, // VALLOX_VARIABLE_LAST_ERROR_NUMBER


		// TODO: thos variables are to be implemented in future
		//VALLOX_VARIABLE_IOPORT_FANSPEED_RELAYS
		//VALLOX_VARIABLE_INSTALLED_CO2_SENSORS
		//VALLOX_VARIABLE_POST_HEATING_ON_COUNTER
		//VALLOX_VARIABLE_POST_HEATING_OFF_TIME
		//VALLOX_VARIABLE_POST_HEATING_TARGET_VALUE
		//VALLOX_VARIABLE_FLAGS_1
		//VALLOX_VARIABLE_FLAGS_2
		//VALLOX_VARIABLE_FLAGS_3
		//VALLOX_VARIABLE_FLAGS_4
		//VALLOX_VARIABLE_FLAGS_5
		//VALLOX_VARIABLE_FLAGS_6
		//VALLOX_VARIABLE_FIRE_PLACE_BOOSTER_COUNTER
		//VALLOX_VARIABLE_MAINTENANCE_MONTH_COUNTER

		// calculated  properties
		InEfficiencyProperty			= 100,
		OutEfficiencyProperty			= 101,
		AverageEfficiencyProperty		= 102,
		
		// TODO
		// Volumenstrom
		// Wärmeübertragung
		// Wärmegewinn

		// virtual properties to be able to poll for this variable
		SelectStatusProperty			= 200,
		ProgramProperty					= 201, // VALLOX_VARIABLE_PROGRAM
		Program2Property				= 202, // VALLOX_VARIABLE_PROGRAM2
		IoPortMultiPurpose1Property		= 203, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_1
		IoPortMultiPurpose2Property		= 204, // VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2
	};

	// callback function types
	typedef void(*PropertyChangedCallbackFunction)(ValloxProperty propertyId, int8_t value);
	typedef void(*StartSendingFunction)();
	typedef void(*StopSendingFunction)();

	typedef void(*LogCallbackFunction)(const char* message);
	typedef bool(*TelegramReceivedCallbackFunction)(uint8_t sender, uint8_t receiver, uint8_t command, uint8_t arg);
	typedef void (*TelegramChecksumFailureCallbackFunction)(uint8_t sender, uint8_t receiver, uint8_t command, uint8_t arg, uint8_t checksum);
	typedef void (*UnexpectedByteReceivedCallbackFunction)(uint8_t receivedByte);
	typedef void(*SuspendResumeCallbackFunction)(bool suspended);
}


class ValloxSerial
{
public:
	ValloxSerial();
	~ValloxSerial();

	void setRxSerial(Stream& serial);			// this should be a hardserial to avoid loosing telegrams
	void setTxSerial(Stream& serial);			// this should be a hardserial to avoid loosing telegrams
	void setSenderId(uint8_t senderId);			// only neccessary when sending data
	void setReceiverId(uint8_t receiverId);		// neccessary to select device we should listen to.
	int8_t getValue(ValloxProperty propertyId) const;

	void setFanSpeed(uint8_t value) const;		// actor: control fan speed 1-8
	void setFanSpeedMin(uint8_t value) const;	// actor: control fan speed 1-8 min
	void setFanSpeedMax(uint8_t value) const;	// actor: control fan speed 1-8 max
	void setDCFanInputAdjustment(uint8_t value) const;	// actor: control DC input fan adjustment
	void setDCFanOutputAdjustment(uint8_t value) const;	// actor: control DC output fan adjustment
	
	void setHrcBypassThreshold(int8_t value) const;	// actor: control HRC bypass threshold
	void setInputFanStopThreshold(int8_t value) const;	// actor: control input fan stop threshold
	void setHeatingSetPoint(int8_t value) const;	// actor: control heating set point
	void setPreHeatingSetPoint(int8_t value) const;	// actor: control pre heating set point
	void setCellDefrostingThreshold(int8_t value) const;	// actor: control cell defrosting threshold (hysteresis 4)

	bool receive();								// this one has to be called in the loop() function.
	void calculateResults();					// this one calculates all efficiency property calculations
	void poll(ValloxProperty propertyId) const;	// requests a variable from the master. The result will show up in receive

	void attachPropertyChanged(PropertyChangedCallbackFunction callbackFunction);
	void detachPropertyChanged(PropertyChangedCallbackFunction callbackFunction);
	
	void attach(StartSendingFunction startSendingCallback, StopSendingFunction stopSendingCallback);
	void detach(StartSendingFunction startSendingCallback, StopSendingFunction stopSendingCallback);

	// diagnostic callbacks
	void attachLogger(LogCallbackFunction callbackFunction);
	void detachLogger(LogCallbackFunction callbackFunction);

	void attach(TelegramReceivedCallbackFunction callbackFunction);
	void detach(TelegramReceivedCallbackFunction callbackFunction);

	void attach(TelegramChecksumFailureCallbackFunction callbackFunction);
	void detach(TelegramChecksumFailureCallbackFunction callbackFunction);

	void attach(UnexpectedByteReceivedCallbackFunction callbackFunction);
	void detach(UnexpectedByteReceivedCallbackFunction callbackFunction);

	void attach(SuspendResumeCallbackFunction callbackFunction);
	void detach(SuspendResumeCallbackFunction callbackFunction);

private:
	void send(uint8_t variable, uint8_t value) const;

	inline void updateFanSpeed(int8_t fanSpeed);
	inline void updateTempInside(int8_t temperature);
	inline void updateTempOutside(int8_t temperature);
	inline void updateTempExhaust(int8_t temperature);
	inline void updateTempIncomming(int8_t temperature);

	inline void  updateStatus(int8_t status);

	inline void updateHumidity(int8_t humidity);
	inline void updateBasicHumidityLevel(int8_t value);
	inline void updateHumiditySensor1(int8_t value);
	inline void updateHumiditySensor2(int8_t value);

	inline void updateCO2High(int8_t value);
	inline void updateCO2Low(int8_t value);
	inline void updateCO2SetPointHigh(int8_t value);
	inline void updateCO2SetPointLow(int8_t value);

	inline void updateFanSpeedMax(int8_t fanSpeed);
	inline void updateFanSpeedMin(int8_t fanSpeed);
	inline void updateDCFanInputAdjustment(int8_t value);
	inline void updateDCFanOutputAdjustment(int8_t value);
	inline void updateInputFanStopThreshold(int8_t temperature);

	inline void updateHeatingSetPoint(int8_t temperature);
	inline void updatePreHeatingSetPoint(int8_t temperature);
	inline void updateHRCBypassThreshold(int8_t temperature);
	inline void updateCellDefrostingThreshold(int8_t temperature);

	inline void updateProgram(int8_t program);
	inline void updateProgram2(int8_t program);

	inline void updateServiceReminder(int8_t value);

	inline void updateMultiPurposeIoPort1(int8_t value);
	inline void updateMultiPurposeIoPort2(int8_t value);
	inline void updateCurrentIncomming(int8_t value);
	inline void updateLastErrorNumber(int8_t value);

	inline void updateEfficiencies();

	

	inline void log(const char* message) const;
	inline void onSuspended(bool suspended);
	inline void onPropertyChanged(ValloxProperty propertyId, int8_t value) const;
	inline void onStartSending() const;
	inline void onStopSending() const;
	inline bool onTelegramReceived(uint8_t sender, uint8_t receiver, uint8_t command, uint8_t arg);

	// callback functions
	PropertyChangedCallbackFunction m_PropertyChangedCallback;
	StartSendingFunction m_StartSendingCallback;
	StopSendingFunction m_StopSendingCallback;

	LogCallbackFunction m_LogCallback;
	TelegramReceivedCallbackFunction m_TelegramReceivedCallback;
	TelegramChecksumFailureCallbackFunction m_TelegramChecksumFailureCallback;
	UnexpectedByteReceivedCallbackFunction m_UnexpectedByteReceivedCallbackFunction;
	SuspendResumeCallbackFunction m_SuspendResumeCallbackFunction;

	// properties
	int8_t m_FanSpeed;
	int8_t m_TempInside;
	int8_t m_TempOutside;
	int8_t m_TempExhaust;
	int8_t m_TempIncomming;

	// status bits
	int8_t m_PowerState;
	int8_t m_CO2AdjustState;
	int8_t m_HumidityAdjustState;
	int8_t m_HeatingState;
	int8_t m_FilterGuardIndicator;
	int8_t m_HeatingIndicator;
	int8_t m_FaultIndicator;
	int8_t m_ServiceReminderIndicator;

	int8_t m_Humidity;
	int8_t m_BasicHumidityLevel;

	int8_t m_HumiditySensor1;
	int8_t m_HumiditySensor2;
	int8_t m_CO2High;
	int8_t m_CO2Low;
	int8_t m_CO2SetPointHigh;
	int8_t m_CO2SetPointLow;

	int8_t m_FanSpeedMax;
	int8_t m_FanSpeedMin;
	int8_t m_DCFanInputAdjustment;
	int8_t m_DCFanOutputAdjustment;
	int8_t m_InputFanStopThreshold;

	int8_t m_HeatingSetPoint;
	int8_t m_PreHeatingSetPoint;
	int8_t m_HrcBypassThreshold;
	int8_t m_CellDefrostingThreshold;
	
	// program
	int8_t m_AdjustmentIntervalMinutes;
	int8_t m_AutomaticHumidityLevelSeekerState;
	int8_t m_BoostSwitchMode;
	int8_t m_RadiatorType;
	int8_t m_CascadeAdjust;

	// program 2
	int8_t m_MaxSpeedLimitMode;

	int8_t m_ServiceReminder;

	// multi purpose ioport 1
	int8_t m_PostHeatingOn;

	// multi purpose ioport 2
	int8_t m_DamperMotorPosition;
	int8_t m_FaultSignalRelay;
	int8_t m_SupplyFanOff;
	int8_t m_PreHeatingOn;
	int8_t m_ExhaustFanOff;
	int8_t m_FirePlaceBoosterOn;

	int8_t m_IncommingCurrent;
	int8_t m_LastErrorNumber;

	bool m_TxSuspended;

	// calculated properties
	int8_t m_InEfficiency;
	int8_t m_OutEfficiency;
	int8_t m_AverageEfficiency;

	// members
	uint8_t m_ReceiverId;
	uint8_t m_SenderId;

	Stream* m_pRxSerial;
	Stream* m_pTxSerial;
};


#endif