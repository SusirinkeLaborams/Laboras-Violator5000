#pragma once

__declspec(align(16)) 
class Tank
{
private:
	DirectX::XMMATRIX m_WorldMatrix;
	bool m_DirtyWorldMatrix;

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Rotation;

	void RecalculateWorldMatrix();

public:
	Tank();
	~Tank();

	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetRotation(const DirectX::XMFLOAT3& rotation);

	void Render(const DirectX::XMMATRIX& viewProjectionMatrix);
};