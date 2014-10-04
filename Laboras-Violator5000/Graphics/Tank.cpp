#include "PrecompiledHeader.h"
#include "Tank.h"


Tank::Tank()
{
}

Tank::~Tank()
{

}

void Tank::RecalculateWorldMatrix()
{
	DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(m_Parameters.position.x, m_Parameters.position.y, m_Parameters.position.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(m_Parameters.rotation.x, m_Parameters.rotation.y, m_Parameters.rotation.z);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_Parameters.scale.x, m_Parameters.scale.y, m_Parameters.scale.z);

	DirectX::XMMATRIX worldMatrix = scale * rotation * position;

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