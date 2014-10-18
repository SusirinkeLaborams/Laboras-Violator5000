#pragma once
#include "IncomingData.h"
#include "Core\NonCopyable.h"
class Map : NonCopyable
{
public:
	struct Line
	{
		const DirectX::XMFLOAT2 Start;
		const DirectX::XMFLOAT2 End;
		Line(DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end)
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
	DirectX::XMFLOAT2 GetCollision(const Line &line);

	static Map CreateSomeMap();

private:
	bool GetCollision(const Line &first, const Line &second, DirectX::XMFLOAT2 &point);
	float GetAngle(DirectX::XMVECTOR vec);
};

