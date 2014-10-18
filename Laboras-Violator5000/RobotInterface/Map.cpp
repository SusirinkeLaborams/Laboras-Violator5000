#include "PrecompiledHeader.h"
#include "Map.h"
#include "Utilities\Utilities.h"

void Map::Add(Line line)
{
	lines.push_back(line);
}

DirectX::XMFLOAT2 Map::GetCollision(const Line &line)
{
	bool found = false;
	DirectX::XMFLOAT2 tmpPoint, point;
	float distance;
	for (auto l : lines)
	{
		if (GetCollision(l, line, tmpPoint))
		{
			if (!found)
			{
				distance = sqrt(pow(tmpPoint.x - line.Start.x, 2) + pow(tmpPoint.y - line.Start.y, 2));
				point = tmpPoint;
				found = true;
			}
			else
			{ 
				float tmpDistance = sqrt(pow(tmpPoint.x - line.Start.x, 2) + pow(tmpPoint.y - line.Start.y, 2));
				if (tmpDistance < distance)
				{
					distance = tmpDistance;
					point = tmpPoint;
				}
			}
		}
	}
	return found ? point : line.End;
}

bool Map::GetCollision(const Line &first, const Line &second, DirectX::XMFLOAT2 &point)
{
	float Ax = first.Start.x;
	float Ay = first.Start.y;
	float Bx = first.End.x;
	float By = first.End.y;
	float Cx = second.Start.x;
	float Cy = second.Start.y;
	float Dx = second.End.x;
	float Dy = second.End.y;

	float  distAB, theCos, theSin, newX, ABpos;

	//  Fail if either line segment is zero-length.
	if (Ax == Bx && Ay == By || Cx == Dx && Cy == Dy) 
		return false;

	//  Fail if the segments share an end-point.
	if (Ax == Cx && Ay == Cy || Bx == Cx && By == Cy
		|| Ax == Dx && Ay == Dy || Bx == Dx && By == Dy) {
		return false;
	}

	//  (1) Translate the system so that point A is on the origin.
	Bx -= Ax; By -= Ay;
	Cx -= Ax; Cy -= Ay;
	Dx -= Ax; Dy -= Ay;

	//  Discover the length of segment A-B.
	distAB = sqrt(Bx*Bx + By*By);

	//  (2) Rotate the system so that point B is on the positive X axis.
	theCos = Bx / distAB;
	theSin = By / distAB;
	newX = Cx*theCos + Cy*theSin;
	Cy = Cy*theCos - Cx*theSin; Cx = newX;
	newX = Dx*theCos + Dy*theSin;
	Dy = Dy*theCos - Dx*theSin; Dx = newX;

	//  Fail if segment C-D doesn't cross line A-B.
	if (Cy<0. && Dy<0. || Cy >= 0. && Dy >= 0.) 
		return false;

	//  (3) Discover the position of the intersection point along line A-B.
	ABpos = Dx + (Cx - Dx)*Dy / (Dy - Cy);

	//  Fail if segment C-D crosses line A-B outside of segment A-B.
	if (ABpos<0. || ABpos>distAB) 
		return false;

	//  (4) Apply the discovered position to line A-B in the original coordinate system.
	point = DirectX::XMFLOAT2(Ax + ABpos*theCos, Ay + ABpos*theSin);
	return true;
}

float Map::GetAngle(DirectX::XMVECTOR vec)
{
	auto axis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	float angle;
	DirectX::XMStoreFloat(&angle, DirectX::XMVector2AngleBetweenVectors(vec, axis));
	return angle;
}

Map Map::CreateSomeMap()
{
	Map ret;
	ret.Add(Line(DirectX::XMFLOAT2(-10.0f, 10.0f), DirectX::XMFLOAT2(0.0f, 10.0f)));
	ret.Add(Line(DirectX::XMFLOAT2(0.0f, 15.0f), DirectX::XMFLOAT2(10.0f, 15.0f)));
	return ret;
}