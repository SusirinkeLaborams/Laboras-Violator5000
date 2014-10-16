#pragma once
#include "IncomingData.h"
#include "Core\NonCopyable.h"
class Map : NonCopyable
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

	Map() = default;
	Map(Map &&other)
		:lines(std::move(other.lines))
	{}

	void Add(Line line);
	DX::XMFLOAT2 GetCollision(const Line &line);

	static Map CreateSomeMap();

private:
	bool GetCollision(const Line &first, const Line &second, DX::XMFLOAT2 &point);
	float GetAngle(DX::XMVECTOR vec);
};

