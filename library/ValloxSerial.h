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
		FanSpeedProperty				= 0,
		TempInsideProperty				= 1,
		TempOutsideProperty				= 2,
		TempExhaustProperty				= 3,
		TempIncommingProperty			= 4,

		// status bits
		PowerStateProperty				= 5,
		CO2AdjustStateProperty			= 6,
		HumidityAdjustStateProperty		= 7,
		HeatingStateProperty			= 8,
		FilterGuardIndicatorProperty	= 9,
		HeatingIndicatorProperty		= 10,
		FaultIndicatorProperty			= 11,
		ServiceReminderIndicatorProperty= 12,

#ifndef MINIMUM_PROPERTIES

		HumidityProperty				= 13,
		BasicHumidityLevelProperty		= 14,
		HumiditySensor1Property			= 15,
		HumiditySensor2Property			= 16,

		CO2HighProperty					= 17,
		CO2LowProperty					= 18,
		CO2SetPointHighProperty			= 19,
		CO2SetPointLowProperty			= 20,

		FanSpeedMaxProperty				= 21,
		FanSpeedMinProperty				= 22,
		DCFanInputAdjustmentProperty	= 23,
		DCFanOutputAdjustmentProperty	= 24,
		InputFanStopThresholdProperty	= 25,

		HeatingSetPointProperty			= 26,
		PreHeatingSetPointProperty		= 27,
		HrcBypassThresholdProperty		= 28,
		CellDefrostingThresholdProperty	= 29,

		// program
		AdjustmentIntervalMinutesProperty= 30,
		AutomaticHumidityLevelSeekerStateProperty= 31,
		BoostSwitchModeProperty			= 32,
		RadiatorTypeProperty			= 33,
		CascadeAdjustProperty			= 34,

		// program2
		MaxSpeedLimitModeProperty		= 35,

		ServiceReminderProperty			= 36,

		
#endif
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
		ProgramProperty					= 201,
		Program2Property				= 202
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

#ifndef MINIMUM_PROPERTIES
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
#endif

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

#ifndef MINIMUM_PROPERTIES
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

	bool m_TxSuspended;

#endif

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