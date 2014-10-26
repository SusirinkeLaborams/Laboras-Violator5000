#include "RobotIO.h"
class Sensor {
private:
	int m_InputPin;
	int m_OutputPin;
public:
	Sensor(int input, int output) : m_InputPin(input), m_OutputPin(output)
	{
		pinMode(m_InputPin, INPUT);
		pinMode(m_OutputPin, OUTPUT);
	}

	int16_t GetDistance()
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
void setup()
{
	Engines[0] = Engine(4, 5);
	Engines[1] = Engine(6, 7);
	
	Serial.begin(9600);	
}

bool ReadInput(RobotInput &result)
{
	uint8_t magicByte = Serial.peek();
	if (magicByte == RobotInput::MagicByte)
	{
		RobotInput input;
		int bytesRead = 0;
		while (bytesRead != sizeof(input))
		{
			bytesRead += Serial.readBytes((char *)(&input + bytesRead), sizeof(input) - bytesRead);
		}

		if (input.Hash == Hash(input))
		{
			result = input;
			return true;
		}
		//Serial.println("Bad hash");
	}
	else
	{
		//Serial.println("				No magic byte");
	}

	while (Serial.available() && Serial.peek() != RobotInput::MagicByte)
	{
		Serial.read();
	}
	return false;	
}

bool InputAvailable()
{
	return Serial.available() >= sizeof(RobotInput);
}
int i = 0;
void loop()
{
	if (InputAvailable())
	{
		i = 0;
		RobotInput input;
		if (ReadInput(input))
		{
			//Engines[input.Engine].SetDirection(input.Direction);
			//Engines[input.Engine].SetSpeed(input.Power);
			Serial.print("Engine ");
			Serial.print(input.Engine);
			Serial.print(" power ");
			Serial.print(input.Power);
			Serial.print(" direction ");
			Serial.println(input.Direction);
		}
		else
		{
			Serial.println("failed to read");
		}
	}
	else
	{
		i++;
		if (i == 1000)
		{
			////digitalWrite(EN, LOW);
			int i = Serial.available();
			//Serial.print(2);
			Serial.print(i);
			Serial.println(" No input");
			i = 0;
		}
	}
}

