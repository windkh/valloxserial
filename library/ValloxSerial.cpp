#include <ValloxSerial.h>

const int8_t INITIAL_VALUE = -1;

ValloxSerial::ValloxSerial()
{
	m_pRxSerial = NULL;
	m_pTxSerial = NULL;

	m_SenderId = VALLOX_ADDRESS_PANEL8;	// we send commands in the name of panel8 (29)	
	m_ReceiverId = VALLOX_ADDRESS_PANEL1; // we always listen for the telegrams between the master and the panel1!

	m_PropertyChangedCallback = NULL;
	m_StartSendingCallback = NULL;
	m_StopSendingCallback = NULL;
	m_LogCallback = NULL;
	m_TelegramReceivedCallback = NULL;
	m_TelegramChecksumFailureCallback = NULL;
	m_UnexpectedByteReceivedCallbackFunction = NULL;
	m_SuspendResumeCallbackFunction = NULL;

	m_FanSpeed = 1;
	m_TempInside = INITIAL_VALUE;
	m_TempOutside = INITIAL_VALUE;
	m_TempExhaust = INITIAL_VALUE;
	m_TempIncomming = INITIAL_VALUE;

	m_PowerState = INITIAL_VALUE;
	m_CO2AdjustState = INITIAL_VALUE;
	m_HumidityAdjustState = INITIAL_VALUE;
	m_HeatingState = INITIAL_VALUE;
	m_FilterGuardIndicator = INITIAL_VALUE;
	m_HeatingIndicator = INITIAL_VALUE;
	m_FaultIndicator = INITIAL_VALUE;
	m_ServiceReminderIndicator = INITIAL_VALUE;

#ifndef MINIMUM_PROPERTIES
	m_Humidity = INITIAL_VALUE;
	m_BasicHumidityLevel = INITIAL_VALUE;

	m_HumiditySensor1 = INITIAL_VALUE;
	m_HumiditySensor2 = INITIAL_VALUE;
	m_CO2High = INITIAL_VALUE;
	m_CO2Low = INITIAL_VALUE;
	m_CO2SetPointHigh = INITIAL_VALUE;
	m_CO2SetPointLow = INITIAL_VALUE;

	m_FanSpeedMax = 8;
	m_FanSpeedMin = 1;
	m_DCFanInputAdjustment = INITIAL_VALUE;
	m_DCFanOutputAdjustment = INITIAL_VALUE;
	m_InputFanStopThreshold = INITIAL_VALUE;

	m_HeatingSetPoint = INITIAL_VALUE;
	m_PreHeatingSetPoint = INITIAL_VALUE;
	m_HrcBypassThreshold = INITIAL_VALUE;
	m_CellDefrostingThreshold = INITIAL_VALUE;

	// program
	m_AdjustmentIntervalMinutes = INITIAL_VALUE;
	m_AutomaticHumidityLevelSeekerState = INITIAL_VALUE;
	m_BoostSwitchMode = INITIAL_VALUE;
	m_RadiatorType = INITIAL_VALUE;
	m_CascadeAdjust = INITIAL_VALUE;

	// program 2
	m_MaxSpeedLimitMode = INITIAL_VALUE;

	m_ServiceReminder = INITIAL_VALUE;
#endif

	m_InEfficiency = INITIAL_VALUE;
	m_OutEfficiency = INITIAL_VALUE;
	m_AverageEfficiency = INITIAL_VALUE;
}

ValloxSerial::~ValloxSerial()
{
	m_pRxSerial = NULL;
	m_pTxSerial = NULL;
}

void ValloxSerial::setRxSerial(Stream& serial)
{
	m_pRxSerial = &serial;
}

void ValloxSerial::setTxSerial(Stream& serial)
{
	m_pTxSerial = &serial;
}

void ValloxSerial::setReceiverId(uint8_t receiverId)
{
	m_ReceiverId = receiverId;
}

void ValloxSerial::setSenderId(uint8_t senderId)
{
	m_SenderId = senderId;
}

int8_t ValloxSerial::getValue(ValloxProperty propertyId) const
{
	int8_t value = INITIAL_VALUE;

	switch (propertyId)
	{
	case FanSpeedProperty:
		value = m_FanSpeed;
		break;
	case TempInsideProperty:
		value = m_TempInside;
		break;
	case TempOutsideProperty:
		value = m_TempOutside;
		break;
	case TempExhaustProperty:
		value = m_TempExhaust;
		break;
	case TempIncommingProperty:
		value = m_TempIncomming;
		break;

	case PowerStateProperty:
		value = m_PowerState;
		break;
	case CO2AdjustStateProperty:
		value = m_CO2AdjustState;
		break;
	case HumidityAdjustStateProperty:
		value = m_HumidityAdjustState;
		break;
	case HeatingStateProperty:
		value = m_HeatingState;
		break;
	case FilterGuardIndicatorProperty:
		value = m_FilterGuardIndicator;
		break;
	case HeatingIndicatorProperty:
		value = m_HeatingIndicator;
		break;
	case FaultIndicatorProperty:
		value = m_FaultIndicator;
		break;
	case ServiceReminderIndicatorProperty:
		value = m_ServiceReminderIndicator;
		break;

#ifndef MINIMUM_PROPERTIES

	case HumidityProperty:
		value = m_Humidity;
		break;
	case BasicHumidityLevelProperty:
		value = m_BasicHumidityLevel;
		break;
	case HumiditySensor1Property:
		value = m_HumiditySensor1;
		break;
	case HumiditySensor2Property:
		value = m_HumiditySensor2;
		break;

	case CO2HighProperty:
		value = m_CO2High;
		break;
	case CO2LowProperty:
		value = m_CO2Low;
		break;

	case CO2SetPointHighProperty:
		value = m_CO2SetPointHigh;
		break;
	case CO2SetPointLowProperty:
		value = m_CO2SetPointLow;
		break;

	case FanSpeedMaxProperty:
		value = m_FanSpeedMax;
		break;
	case FanSpeedMinProperty:
		value = m_FanSpeedMin;
		break;
	case DCFanInputAdjustmentProperty:
		value = m_DCFanInputAdjustment;
		break;
	case DCFanOutputAdjustmentProperty:
		value = m_DCFanOutputAdjustment;
		break;
	case InputFanStopThresholdProperty:
		value = m_InputFanStopThreshold;
		break;
	case HeatingSetPointProperty:
		value = m_HeatingSetPoint;
		break;
	case PreHeatingSetPointProperty:
		value = m_PreHeatingSetPoint;
		break;
	case HrcBypassThresholdProperty:
		value = m_HrcBypassThreshold;
		break;
	case CellDefrostingThresholdProperty:
		value = m_CellDefrostingThreshold;
		break;

	case AdjustmentIntervalMinutesProperty:
		value = m_AdjustmentIntervalMinutes;
		break;
	case AutomaticHumidityLevelSeekerStateProperty:
		value = m_AutomaticHumidityLevelSeekerState;
		break;
	case BoostSwitchModeProperty:
		value = m_BoostSwitchMode;
		break;
	case RadiatorTypeProperty:
		value = m_RadiatorType;
		break;
	case CascadeAdjustProperty:
		value = m_CascadeAdjust;
		break;

	case MaxSpeedLimitModeProperty:
		value = m_MaxSpeedLimitMode;
		break;
	case ServiceReminderProperty:
		value = m_ServiceReminder;
		break;
#endif

	default:
		value = INITIAL_VALUE;
		break;
	}

	return value;
}

void ValloxSerial::attachPropertyChanged(PropertyChangedCallbackFunction callbackFunction)
{
	m_PropertyChangedCallback = callbackFunction;
}

void ValloxSerial::detachPropertyChanged(PropertyChangedCallbackFunction callbackFunction)
{
	m_PropertyChangedCallback = NULL;
}

void ValloxSerial::attach(StartSendingFunction startSendingCallback, StopSendingFunction stopSendingCallback)
{
	m_StartSendingCallback = startSendingCallback;
	m_StopSendingCallback = stopSendingCallback;
}

void ValloxSerial::detach(StartSendingFunction startSendingCallback, StopSendingFunction stopSendingCallback)
{
	m_StartSendingCallback = NULL;
	m_StopSendingCallback = NULL;
}


void ValloxSerial::attachLogger(LogCallbackFunction callbackFunction)
{
	m_LogCallback = callbackFunction;
}

void ValloxSerial::detachLogger(LogCallbackFunction callbackFunction)
{
	m_LogCallback = NULL;
}

void ValloxSerial::attach(TelegramReceivedCallbackFunction callbackFunction)
{
	m_TelegramReceivedCallback = callbackFunction;
}

void ValloxSerial::detach(TelegramReceivedCallbackFunction callbackFunction)
{
	m_TelegramReceivedCallback = NULL;
}

void ValloxSerial::attach(TelegramChecksumFailureCallbackFunction callbackFunction)
{
	m_TelegramChecksumFailureCallback = callbackFunction;
}

void ValloxSerial::detach(TelegramChecksumFailureCallbackFunction callbackFunction)
{
	m_TelegramChecksumFailureCallback = NULL;
}

void ValloxSerial::attach(UnexpectedByteReceivedCallbackFunction callbackFunction)
{
	m_UnexpectedByteReceivedCallbackFunction = callbackFunction;
}

void ValloxSerial::detach(UnexpectedByteReceivedCallbackFunction callbackFunction)
{
	m_UnexpectedByteReceivedCallbackFunction = NULL;
}

void ValloxSerial::attach(SuspendResumeCallbackFunction callbackFunction)
{
	m_SuspendResumeCallbackFunction = callbackFunction;
}

void ValloxSerial::detach(SuspendResumeCallbackFunction callbackFunction)
{
	m_SuspendResumeCallbackFunction = NULL;
}



void ValloxSerial::poll(ValloxProperty propertyId) const
{
	switch (propertyId)
	{
	case FanSpeedProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_FAN_SPEED);
		break;
	case TempInsideProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_TEMP_INSIDE);
		break;
	case TempOutsideProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_TEMP_OUTSIDE);
		break;
	case TempExhaustProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_TEMP_EXHAUST);
		break;
	case TempIncommingProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_TEMP_INCOMMING);
		break;

	// status bits
	case PowerStateProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;
	case CO2AdjustStateProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;
	case HumidityAdjustStateProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;
	case HeatingStateProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;
	case FilterGuardIndicatorProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;
	case HeatingIndicatorProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;
	case FaultIndicatorProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;
	case ServiceReminderIndicatorProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;

#ifndef MINIMUM_PROPERTIES

	case HumidityProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_HUMIDITY);
		break;
	case BasicHumidityLevelProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_BASIC_HUMIDITY_LEVEL);
		break;
	case HumiditySensor1Property:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_HUMIDITY_SENSOR1);
		break;
	case HumiditySensor2Property:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_HUMIDITY_SENSOR2);
		break;

	case CO2HighProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_CO2_HIGH);
		break;
	case CO2LowProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_CO2_LOW);
		break;

	case CO2SetPointHighProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_CO2_SET_POINT_UPPER);
		break;
	case CO2SetPointLowProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_CO2_SET_POINT_LOWER);
		break;

	case FanSpeedMaxProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_FAN_SPEED_MAX);
		break;
	case FanSpeedMinProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_FAN_SPEED_MIN);
		break;
	case DCFanInputAdjustmentProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_DC_FAN_INPUT_ADJUSTMENT);
		break;
	case DCFanOutputAdjustmentProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_DC_FAN_OUTPUT_ADJUSTMENT);
		break;
	case InputFanStopThresholdProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_INPUT_FAN_STOP);
		break;
	case HeatingSetPointProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_HEATING_SET_POINT);
		break;
	case PreHeatingSetPointProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PRE_HEATING_SET_POINT);
		break;
	case HrcBypassThresholdProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_HRC_BYPASS);
		break;
	case CellDefrostingThresholdProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_CELL_DEFROSTING);
		break;

	// program 
	case AdjustmentIntervalMinutesProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PROGRAM);
		break;
	case AutomaticHumidityLevelSeekerStateProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PROGRAM);
		break;
	case BoostSwitchModeProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PROGRAM);
		break;
	case RadiatorTypeProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PROGRAM);
		break;
	case CascadeAdjustProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PROGRAM);
		break;

	// program 2
	case MaxSpeedLimitModeProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PROGRAM2);
		break;

	case ServiceReminderProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SERVICE_REMINDER);
		break;
#endif

	// virtual properties for bit encoded values
	case SelectStatusProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_SELECT);
		break;
	case ProgramProperty:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PROGRAM);
		break;
	case Program2Property:
		send(VALLOX_VARIABLE_POLL, VALLOX_VARIABLE_PROGRAM2);
		break;
	default:
		// unknown property
		break;
	}
}

void ValloxSerial::setFanSpeed(uint8_t value) const
{
	uint8_t fanSpeed = Vallox::convertBackFanSpeed(value-1); // -1 as index in array is zero based 0-7
	send(VALLOX_VARIABLE_FAN_SPEED, fanSpeed);
}

void ValloxSerial::setFanSpeedMax(uint8_t value) const
{
	uint8_t fanSpeed = Vallox::convertBackFanSpeed(value - 1); // -1 as index in array is zero based 0-7
	send(VALLOX_VARIABLE_FAN_SPEED_MAX, fanSpeed);
}

void ValloxSerial::setFanSpeedMin(uint8_t value) const
{
	uint8_t fanSpeed = Vallox::convertBackFanSpeed(value - 1); // -1 as index in array is zero based 0-7
	send(VALLOX_VARIABLE_FAN_SPEED_MIN, fanSpeed);
}

void ValloxSerial::setDCFanInputAdjustment(uint8_t value) const
{
	send(VALLOX_VARIABLE_DC_FAN_INPUT_ADJUSTMENT, value);
}

void ValloxSerial::setDCFanOutputAdjustment(uint8_t value) const
{
	send(VALLOX_VARIABLE_DC_FAN_OUTPUT_ADJUSTMENT, value);
}

void ValloxSerial::send(uint8_t variable, uint8_t value) const
{
	// When C02 sensor communication is active we discard telegrams
	if (!m_TxSuspended)
	{
		uint8_t telegram[VALLOX_LENGTH];
		telegram[0] = VALLOX_DOMAIN;
		telegram[1] = m_SenderId;
		telegram[2] = VALLOX_ADDRESS_MASTER;
		telegram[3] = variable;
		telegram[4] = value;
		telegram[5] = Vallox::calculateChecksum(telegram);

		onStartSending();
		m_pTxSerial->write(telegram, VALLOX_LENGTH);
		m_pTxSerial->flush();
		onStopSending();
	}
}


bool ValloxSerial::receive()
{
	bool telegramReceived = false;

	// wait until at least one complete telegram is in the input buffer
	if (m_pRxSerial->available() >= VALLOX_LENGTH)
	{
		uint8_t domain = m_pRxSerial->read();

		if (domain == VALLOX_DOMAIN)
		{
			uint8_t sender = m_pRxSerial->read();
			uint8_t receiver = m_pRxSerial->read();
			uint8_t command = m_pRxSerial->read();
			uint8_t arg = m_pRxSerial->read();
			uint8_t checksum = m_pRxSerial->read();

			uint16_t computedChecksum = (domain + sender + receiver + command + arg) & 0x00ff;
			if (checksum == computedChecksum)
			{
				bool handleTelegram = true;
				if (m_TelegramReceivedCallback)
				{
					handleTelegram = (*m_TelegramReceivedCallback)(sender, receiver, command, arg);
				}

				// the callback may return false to avoid handling this telegram!
				if (handleTelegram)
				{
					telegramReceived = onTelegramReceived(sender, receiver, command, arg);
				}
			}
			else
			{
				if (m_TelegramChecksumFailureCallback)
				{
					(*m_TelegramChecksumFailureCallback)(sender, receiver, command, arg, checksum);
				}
			}
		}
		else
		{
			if (m_UnexpectedByteReceivedCallbackFunction)
			{
				(*m_UnexpectedByteReceivedCallbackFunction)(domain);
			}
		}
	}

	return telegramReceived;
}

void ValloxSerial::calculateResults()
{
	updateEfficiencies();
}

bool ValloxSerial::onTelegramReceived(uint8_t sender, uint8_t receiver, uint8_t command, uint8_t arg)
{
	bool telegramReceived = true;

	if (receiver == m_ReceiverId || receiver == m_SenderId || receiver == VALLOX_ADDRESS_PANELS)
	{
		uint8_t variable = command;
		int8_t value = (int8_t)arg;

		switch (variable)
		{
		case VALLOX_VARIABLE_IOPORT_FANSPEED_RELAYS:
		{
			// TODO
			// convertIoPortFanSpeedRelays
			break;
		}
		case VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_1:
		{
			// convertIoPortMultiPurpose1
			break;
		}
		case VALLOX_VARIABLE_IOPORT_MULTI_PURPOSE_2:
		{
			// TODO
			// convertIoPortMultiPurpose2
			break;
		}
		case VALLOX_VARIABLE_INSTALLED_CO2_SENSORS:
		{
			// TODO
			// convertInstalledCO2Sensor
			break;
		}
		case VALLOX_VARIABLE_CURRENT_INCOMMING:
		{
			// TODO
			break;
		}
		case VALLOX_VARIABLE_LAST_ERROR_NUMBER:
		{
			// TODO
			break;
		}
		case VALLOX_VARIABLE_POST_HEATING_ON_COUNTER:
		{
			// TODO
			break;
		}
		case VALLOX_VARIABLE_POST_HEATING_OFF_TIME:
		{
			// TODO
			break;
		}
		case VALLOX_VARIABLE_POST_HEATING_TARGET_VALUE:
		{
			// TODO
			break;
		}
		case VALLOX_VARIABLE_FLAGS_1:
		{
			// TODO
			// converFlags1
			break;
		}
		case VALLOX_VARIABLE_FLAGS_2:
		{
			// TODO
			// converFlags2
			break;
		}
		case VALLOX_VARIABLE_FLAGS_3:
		{
			// TODO
			// converFlags3
			break;
		}
		case VALLOX_VARIABLE_FLAGS_4:
		{
			// TODO
			// converFlags4
			break;
		}
		case VALLOX_VARIABLE_FLAGS_5:
		{
			// TODO
			// converFlags5
			break;
		}
		case VALLOX_VARIABLE_FLAGS_6:
		{
			// TODO
			// converFlags6
			break;
		}
		case VALLOX_VARIABLE_FIRE_PLACE_BOOSTER_COUNTER:
		{
			// TODO
			break;
		}
		case VALLOX_VARIABLE_MAINTENANCE_MONTH_COUNTER:
		{
			// TODO
			break;
		}
		


		case VALLOX_VARIABLE_FAN_SPEED:
		{
			int fanSpeed = Vallox::convertFanSpeed(value);
			updateFanSpeed(fanSpeed);
			break;
		}
		case VALLOX_VARIABLE_TEMP_OUTSIDE:
		{
			int temperature = Vallox::convertTemperature(value);
			updateTempOutside(temperature);
			break;
		}
		case VALLOX_VARIABLE_TEMP_EXHAUST:
		{
			int temperature = Vallox::convertTemperature(value);
			updateTempExhaust(temperature);
			break;
		}
		case VALLOX_VARIABLE_TEMP_INSIDE:
		{
			int temperature = Vallox::convertTemperature(value);
			updateTempInside(temperature);
			break;
		}
		case VALLOX_VARIABLE_TEMP_INCOMMING:
		{
			int temperature = Vallox::convertTemperature(value);
			updateTempIncomming(temperature);
			break;
		}
		case VALLOX_VARIABLE_SELECT:
		{
			updateStatus(value);
			break;
		}

#ifndef MINIMUM_PROPERTIES
		case VALLOX_VARIABLE_HUMIDITY:
		{
			updateHumidity(value);
			break;
		}
		case VALLOX_VARIABLE_BASIC_HUMIDITY_LEVEL:
		{
			updateBasicHumidityLevel(value);
			break;
		}
		case VALLOX_VARIABLE_HUMIDITY_SENSOR1:
		{
			updateHumiditySensor1(value);
			break;
		}
		case VALLOX_VARIABLE_HUMIDITY_SENSOR2:
		{
			updateHumiditySensor2(value);
			break;
		}

		case VALLOX_VARIABLE_CO2_HIGH:
		{
			updateCO2High(value);
			break;
		}
		case VALLOX_VARIABLE_CO2_LOW:
		{
			updateCO2Low(value);
			break;
		}
		case VALLOX_VARIABLE_CO2_SET_POINT_UPPER:
		{
			updateCO2SetPointHigh(value);
			break;
		}
		case VALLOX_VARIABLE_CO2_SET_POINT_LOWER:
		{
			updateCO2SetPointLow(value);
			break;
		}

		case VALLOX_VARIABLE_FAN_SPEED_MAX:
		{
			uint8_t fanSpeed = Vallox::convertFanSpeed(value);
			updateFanSpeedMax(fanSpeed);
			break;
		}
		case VALLOX_VARIABLE_FAN_SPEED_MIN:
		{
			uint8_t fanSpeed = Vallox::convertFanSpeed(value);
			updateFanSpeedMin(fanSpeed);
			break;
		}
		case VALLOX_VARIABLE_DC_FAN_OUTPUT_ADJUSTMENT:
		{
			updateDCFanOutputAdjustment(value);
			break;
		}
		case VALLOX_VARIABLE_DC_FAN_INPUT_ADJUSTMENT:
		{
			updateDCFanInputAdjustment(value);
			break;
		}
		case VALLOX_VARIABLE_INPUT_FAN_STOP:
		{
			int8_t temperature = Vallox::convertTemperature(value);
			updateInputFanStopThreshold(temperature);
			break;
		}

		case VALLOX_VARIABLE_HEATING_SET_POINT:
		{
			int temperature = Vallox::convertTemperature(value);
			updateHeatingSetPoint(temperature);
			break;
		}
		case VALLOX_VARIABLE_PRE_HEATING_SET_POINT:
		{
			int temperature = Vallox::convertTemperature(value);
			updatePreHeatingSetPoint(temperature);
			break;
		}
		case VALLOX_VARIABLE_HRC_BYPASS:
		{
			int8_t temperature = Vallox::convertTemperature(value);
			updateHRCBypassThreshold(temperature);
			break;
		}
		case VALLOX_VARIABLE_CELL_DEFROSTING:
		{
			int temperature = Vallox::convertTemperature(value);
			updateCellDefrostingThreshold(temperature);
			break;
		}

		case VALLOX_VARIABLE_PROGRAM:
		{
			updateProgram(value);
			break;
		}
		case VALLOX_VARIABLE_PROGRAM2:
		{
			updateProgram2(value);
			break;
		}

		case VALLOX_VARIABLE_SERVICE_REMINDER:
		{
			updateServiceReminder(value);
			break;
		}

		case VALLOX_VARIABLE_UNKNOWN:
		{
			break;
		}

		// C02 communication starts: no tx allowed!
		case VALLOX_VARIABLE_SUSPEND:
		{
			onSuspended(true);
			break;
		}

		// C02 communication ends: tx allowed!
		case VALLOX_VARIABLE_RESUME:
		{
			onSuspended(false);
			break;
		}
#endif
		default:
		{
			log("Unkown command received");
			telegramReceived = false;
			break;
		}
		}//switch
	}
	else
	{
		// telegram was not meant for us.
	}

	return telegramReceived;
}



void ValloxSerial::updateFanSpeed(int8_t fanSpeed)
{
	if (m_FanSpeed != fanSpeed)
	{
		m_FanSpeed = fanSpeed;
		onPropertyChanged(FanSpeedProperty, fanSpeed);
	}
}

void ValloxSerial::updateTempInside(int8_t temperature)
{
	if (m_TempInside != temperature)
	{
		m_TempInside = temperature;
		onPropertyChanged(TempInsideProperty, temperature);
	}
}

void ValloxSerial::updateTempOutside(int8_t temperature)
{
	if (m_TempOutside != temperature)
	{
		m_TempOutside = temperature;
		onPropertyChanged(TempOutsideProperty, temperature);
	}
}

void ValloxSerial::updateTempExhaust(int8_t temperature)
{
	if (m_TempExhaust != temperature)
	{
		m_TempExhaust = temperature;
		onPropertyChanged(TempExhaustProperty, temperature);
	}
}

void ValloxSerial::updateTempIncomming(int8_t temperature)
{
	if (m_TempIncomming != temperature)
	{
		m_TempIncomming = temperature;
		onPropertyChanged(TempIncommingProperty, temperature);
	}
}

void ValloxSerial::updateStatus(int8_t status)
{
	bool powerState;
	bool co2AdjustState;
	bool humidityAdjustState;
	bool heatingState;
	bool filterGuardIndicator;
	bool heatingIndicator;
	bool faultIndicator;
	bool serviceReminderIndicator;

	Vallox::convertSelect(status,
		&powerState, 
		&co2AdjustState, 
		&humidityAdjustState, 
		&heatingState,
		&filterGuardIndicator, 
		&heatingIndicator, 
		&faultIndicator,
		&serviceReminderIndicator);

	if (m_PowerState != powerState)
	{
		m_PowerState = powerState;
		onPropertyChanged(PowerStateProperty, m_PowerState);
	}

	if (m_CO2AdjustState != co2AdjustState)
	{
		m_CO2AdjustState = co2AdjustState;
		onPropertyChanged(CO2AdjustStateProperty, m_CO2AdjustState);
	}

	if (m_HumidityAdjustState != humidityAdjustState)
	{
		m_HumidityAdjustState = humidityAdjustState;
		onPropertyChanged(HumidityAdjustStateProperty, m_HumidityAdjustState);
	}

	if (m_HeatingState != heatingState)
	{
		m_HeatingState = heatingState;
		onPropertyChanged(HeatingStateProperty, m_HeatingState);
	}

	if (m_FilterGuardIndicator != filterGuardIndicator)
	{
		m_FilterGuardIndicator = filterGuardIndicator;
		onPropertyChanged(FilterGuardIndicatorProperty, m_FilterGuardIndicator);
	}

	if (m_HeatingIndicator != heatingIndicator)
	{
		m_HeatingIndicator = heatingIndicator;
		onPropertyChanged(HeatingIndicatorProperty, m_HeatingIndicator);
	}

	if (m_FaultIndicator != faultIndicator)
	{
		m_FaultIndicator = faultIndicator;
		onPropertyChanged(FaultIndicatorProperty, m_FaultIndicator);
	}

	if (m_ServiceReminderIndicator != serviceReminderIndicator)
	{
		m_ServiceReminderIndicator = serviceReminderIndicator;
		onPropertyChanged(ServiceReminderIndicatorProperty, m_ServiceReminderIndicator);
	}
}

#ifndef MINIMUM_PROPERTIES

void ValloxSerial::updateHumidity(int8_t humidity)
{
	if (m_Humidity != humidity)
	{
		m_Humidity = humidity;
		onPropertyChanged(HumidityProperty, m_Humidity);
	}
}

void ValloxSerial::updateBasicHumidityLevel(int8_t value)
{
	if (m_BasicHumidityLevel != value)
	{
		m_BasicHumidityLevel = value;
		onPropertyChanged(BasicHumidityLevelProperty, m_BasicHumidityLevel);
	}
}

void ValloxSerial::updateHumiditySensor1(int8_t value)
{
	if (m_HumiditySensor1 != value)
	{
		m_HumiditySensor1 = value;
		onPropertyChanged(HumiditySensor1Property, m_HumiditySensor1);
	}
}

void ValloxSerial::updateHumiditySensor2(int8_t value)
{
	if (m_HumiditySensor2 != value)
	{
		m_HumiditySensor2 = value;
		onPropertyChanged(HumiditySensor2Property, m_HumiditySensor2);
	}
}

void ValloxSerial::updateCO2High(int8_t value)
{
	if (m_CO2High != value)
	{
		m_CO2High = value;
		onPropertyChanged(CO2HighProperty, m_CO2High);
	}
}

void ValloxSerial::updateCO2Low(int8_t value)
{
	if (m_CO2Low != value)
	{
		m_CO2Low = value;
		onPropertyChanged(CO2LowProperty, m_CO2Low);
	}
}

void ValloxSerial::updateCO2SetPointHigh(int8_t value)
{
	if (m_CO2SetPointHigh != value)
	{
		m_CO2SetPointHigh = value;
		onPropertyChanged(CO2SetPointHighProperty, m_CO2SetPointHigh);
	}
}

void ValloxSerial::updateCO2SetPointLow(int8_t value)
{
	if (m_CO2SetPointLow != value)
	{
		m_CO2SetPointLow = value;
		onPropertyChanged(CO2SetPointLowProperty, m_CO2SetPointLow);
	}
}

void ValloxSerial::updateFanSpeedMax(int8_t fanSpeed)
{
	if (m_FanSpeedMax != fanSpeed)
	{
		m_FanSpeedMax = fanSpeed;
		onPropertyChanged(FanSpeedMaxProperty, m_FanSpeedMax);
	}
}

void ValloxSerial::updateFanSpeedMin(int8_t fanSpeed)
{
	if (m_FanSpeedMin != fanSpeed)
	{
		m_FanSpeedMin = fanSpeed;
		onPropertyChanged(FanSpeedMinProperty, m_FanSpeedMin);
	}
}

void ValloxSerial::updateDCFanInputAdjustment(int8_t value)
{
	if (m_DCFanInputAdjustment != value)
	{
		m_DCFanInputAdjustment = value;
		onPropertyChanged(DCFanInputAdjustmentProperty, m_DCFanInputAdjustment);
	}
}

void ValloxSerial::updateDCFanOutputAdjustment(int8_t value)
{
	if (m_DCFanOutputAdjustment != value)
	{
		m_DCFanOutputAdjustment = value;
		onPropertyChanged(DCFanOutputAdjustmentProperty, m_DCFanOutputAdjustment);
	}
}

void ValloxSerial::updateInputFanStopThreshold(int8_t temperature)
{
	if (m_InputFanStopThreshold != temperature)
	{
		m_InputFanStopThreshold = temperature;
		onPropertyChanged(InputFanStopThresholdProperty, m_InputFanStopThreshold);
	}
}

void ValloxSerial::updateHeatingSetPoint(int8_t temperature)
{
	if (m_HeatingSetPoint != temperature)
	{
		m_HeatingSetPoint = temperature;
		onPropertyChanged(HeatingSetPointProperty, m_HeatingSetPoint);
	}
}

void ValloxSerial::updatePreHeatingSetPoint(int8_t temperature)
{
	if (m_PreHeatingSetPoint != temperature)
	{
		m_PreHeatingSetPoint = temperature;
		onPropertyChanged(PreHeatingSetPointProperty, m_PreHeatingSetPoint);
	}
}

void ValloxSerial::updateHRCBypassThreshold(int8_t temperature)
{
	if (m_HrcBypassThreshold != temperature)
	{
		m_HrcBypassThreshold = temperature;
		onPropertyChanged(HrcBypassThresholdProperty, m_HrcBypassThreshold);
	}
}

void ValloxSerial::updateCellDefrostingThreshold(int8_t temperature)
{
	if (m_CellDefrostingThreshold != temperature)
	{
		m_CellDefrostingThreshold = temperature;
		onPropertyChanged(CellDefrostingThresholdProperty, m_CellDefrostingThreshold);
	}
}

void ValloxSerial::updateProgram(int8_t program)
{
	uint8_t adjustmentIntervalMinutes;
	bool automaticHumidityLevelSeekerState;
	bool boostSwitchMode;
	bool radiatorType;
	bool cascadeAdjust;

	Vallox::convertProgram(program,
		&adjustmentIntervalMinutes, 
		&automaticHumidityLevelSeekerState,
		&boostSwitchMode, 
		&radiatorType,
		&cascadeAdjust);

	if (m_AdjustmentIntervalMinutes != adjustmentIntervalMinutes)
	{
		m_AdjustmentIntervalMinutes = adjustmentIntervalMinutes;
		onPropertyChanged(AdjustmentIntervalMinutesProperty, m_AdjustmentIntervalMinutes);
	}

	if (m_AutomaticHumidityLevelSeekerState != automaticHumidityLevelSeekerState)
	{
		m_AutomaticHumidityLevelSeekerState = automaticHumidityLevelSeekerState;
		onPropertyChanged(AutomaticHumidityLevelSeekerStateProperty, m_AutomaticHumidityLevelSeekerState);
	}

	if (m_BoostSwitchMode != boostSwitchMode)
	{
		m_BoostSwitchMode = boostSwitchMode;
		onPropertyChanged(BoostSwitchModeProperty, m_BoostSwitchMode);
	}

	if (m_RadiatorType != radiatorType)
	{
		m_RadiatorType = radiatorType;
		onPropertyChanged(RadiatorTypeProperty, m_RadiatorType);
	}

	if (m_CascadeAdjust != cascadeAdjust)
	{
		m_CascadeAdjust = cascadeAdjust;
		onPropertyChanged(CascadeAdjustProperty, m_CascadeAdjust);
	}
}

void ValloxSerial::updateProgram2(int8_t program)
{
	bool maxSpeedLimitMode;
	Vallox::convertProgram2(program,
		&maxSpeedLimitMode);

	if (m_MaxSpeedLimitMode != maxSpeedLimitMode)
	{
		m_MaxSpeedLimitMode = maxSpeedLimitMode;
		onPropertyChanged(MaxSpeedLimitModeProperty, m_MaxSpeedLimitMode);
	}
}

void ValloxSerial::updateServiceReminder(int8_t value)
{
	if (m_ServiceReminder != value)
	{
		m_ServiceReminder = value;
		onPropertyChanged(ServiceReminderProperty, m_ServiceReminder);
	}
}


#endif


void ValloxSerial::updateEfficiencies()
{
	int8_t maxPossible = m_TempInside - m_TempOutside;
	if (maxPossible != 0)
	{
		float inEfficiency = (m_TempIncomming - m_TempOutside) * 100.0 / maxPossible;
		if (m_InEfficiency != (int8_t)inEfficiency)
		{
			m_InEfficiency = inEfficiency;
			onPropertyChanged(InEfficiencyProperty, inEfficiency);
		}

		float outEfficiency = (m_TempInside - m_TempExhaust) * 100.0 / maxPossible;
		if (m_OutEfficiency != (int8_t)outEfficiency)
		{
			m_OutEfficiency = outEfficiency;
			onPropertyChanged(OutEfficiencyProperty, outEfficiency);
		}

		float averageEfficiency = (m_InEfficiency + m_OutEfficiency) / 2;
		if (m_AverageEfficiency != (int8_t)averageEfficiency)
		{
			m_AverageEfficiency = averageEfficiency;
			onPropertyChanged(AverageEfficiencyProperty, averageEfficiency);
		}
	}
}


void ValloxSerial::onSuspended(bool suspended)
{
	if (m_TxSuspended != suspended)
	{
		m_TxSuspended = suspended;
		if (m_SuspendResumeCallbackFunction)
		{
			(*m_SuspendResumeCallbackFunction)(suspended);
		}
	}
}

void ValloxSerial::onPropertyChanged(ValloxProperty propertyId, int8_t value) const
{
	if (m_PropertyChangedCallback)
	{
		(*m_PropertyChangedCallback)(propertyId, value);
	}
}

void ValloxSerial::onStartSending() const
{
	if (m_StartSendingCallback)
	{
		(*m_StartSendingCallback)();
	}
}

void ValloxSerial::onStopSending() const
{
	if (m_StopSendingCallback)
	{
		(*m_StopSendingCallback)();
	}
}

void ValloxSerial::log(const char* message) const
{
	if (m_LogCallback)
	{
		(*m_LogCallback)(message);
	}
}

