#include "PrecompiledHeader.h"
#include "Tank.h"


Tank::Tank() :
	m_Position(0.0f, 0.0f, 0.0f),
	m_Rotation(0.0f, 0.0f, 0.0f),
	m_DirtyWorldMatrix(true)
{
}

Tank::~Tank()
{

}

void Tank::RecalculateWorldMatrix()
{
	DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	DirectX::XMMATRIX worldMatrix = rotation * position;

	m_WorldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	m_DirtyWorldMatrix = false;
}

void Tank::SetPosition(const DirectX::XMFLOAT3& position)
{
	if (memcmp(&m_Position, &position, sizeof(DirectX::XMFLOAT3)) != 0)
	{
		m_Position = position;
		m_DirtyWorldMatrix = true;
	}
}

void Tank::SetRotation(const DirectX::XMFLOAT3& rotation)
{
	if (memcmp(&m_Rotation, &rotation, sizeof(DirectX::XMFLOAT3)) != 0)
	{
		m_Rotation = rotation;
		m_DirtyWorldMatrix = true;
	}
}

void Tank::Render(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	if (m_DirtyWorldMatrix)
	{
		RecalculateWorldMatrix();
	}


}