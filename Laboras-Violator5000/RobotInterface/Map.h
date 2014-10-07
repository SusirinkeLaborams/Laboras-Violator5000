#pragma once
#include "IncomingData.h"
class Map
{
public:
	struct Line
	{
		const DX::XMFLOAT2 Start;
		const DX::XMFLOAT2 End;
		Line(DX::XMFLOAT2 start, DX::XMFLOAT2 end)
			:Start(start), End(end){}
		float Length(){ return 1.0f; }
	};

private:
	std::vector<Line> lines;

public:

	void Add(Line line);
	bool GetCollision(const Line &line, DX::XMFLOAT2 &point);

private:
	bool GetCollision(const Line &first, const Line &second, DX::XMFLOAT2 &point);
	float GetAngle(DX::XMVECTOR vec);
};

