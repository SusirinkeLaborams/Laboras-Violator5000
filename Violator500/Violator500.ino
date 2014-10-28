#include "RobotIO.h"

class Sensor {
private:
	int m_InputPin;
	int m_OutputPin;
public:
	Sensor()
	{
	}

	Sensor(int trig, int echo) : m_InputPin(trig), m_OutputPin(echo)
	{
		pinMode(m_InputPin, INPUT);
		pinMode(m_OutputPin, OUTPUT);
	}

	uint32_t GetDistance()
	{
		digitalWrite(m_OutputPin, LOW);
		delayMicroseconds(2);
		digitalWrite(m_OutputPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(m_OutputPin, LOW);

		return pulseIn(m_InputPin, HIGH);
	}
};

class Engine {
private:
	int m_DirectionPin;
	int m_SpeedPin;
public:
	Engine()
	{
	}

	Engine(int directionPin, int speedPin)
	{
		m_DirectionPin = directionPin;
		m_SpeedPin = speedPin;
		pinMode(m_DirectionPin, OUTPUT);
		pinMode(m_SpeedPin, OUTPUT);
	}

	void SetDirection(int engineDirection)
	{
		digitalWrite(m_DirectionPin, engineDirection);
	}

	void SetSpeed(int engineSpeed)
	{
		analogWrite(m_SpeedPin, engineSpeed);
	}
};

Engine Engines[2];
const int EngineCount = 2;

Sensor Sensors[2];
void setup()
{
	Engines[0] = Engine(4, 5);
	Engines[1] = Engine(7, 6);
	
	Sensors[0] = Sensor(13, 12);
	Sensors[1] = Sensor(11, 10);
	Serial.begin(9600);	
}


void loop()
{
	RobotOutput o;
	o.Magic = RobotOutput::MagicBytes;
	o.Sensors[0] = Sensors[0].GetDistance();
	o.Sensors[1] = Sensors[1].GetDistance();
	o.Hash = Hash(o);

	Serial.write((char*)&o, sizeof(o));
}

void serialEvent()
{
	if (Serial.available())
	{
		RobotInput input;
		Serial.readBytes((char*) &input, 1);
		
		Engines[0].SetDirection(input.DirectionL);
		Engines[0].SetSpeed(input.PowerL * 36);
		Engines[1].SetDirection(input.DirectionR);
		Engines[1].SetSpeed(input.PowerR * 36);
	}
}

