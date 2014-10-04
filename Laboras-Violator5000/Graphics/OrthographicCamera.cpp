#include "PrecompiledHeader.h"
#include "Core\Settings.h"
#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float aspectRatio, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation) :
	m_Position(position), 
	m_Rotation(rotation),
	m_DirtyViewMatrix(true)
{
}

OrthographicCamera::~OrthographicCamera()
{
}

void OrthographicCamera::RecalculateViewMatrix()
{
	DirectX::XMVECTOR position = DirectX::XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 1.0f);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	DirectX::XMVECTOR up = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), rotationMatrix);
	DirectX::XMVECTOR lookAt = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f), rotationMatrix);
	lookAt = DirectX::XMVectorAdd(lookAt, position);

	m_ViewMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(position, lookAt, up));

	RecalculateViewProjectionMatrix();
	m_DirtyViewMatrix = false;
}

void OrthographicCamera::RecalculateProjectionMatrix(float aspectRatio)
{
	DirectX::XMMATRIX orthoMatrix = DirectX::XMMatrixOrthographicRH(aspectRatio * Settings::kCentimetersInWindowHeight, Settings::kCentimetersInWindowHeight, 0.1f, 1000.0f);
	DirectX::XMMATRIX flipYMatrix = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);

	m_ProjectionMatrix = DirectX::XMMatrixTranspose(flipYMatrix * orthoMatrix);
}

void OrthographicCamera::RecalculateViewProjectionMatrix()
{
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

const DirectX::XMMATRIX& OrthographicCamera::GetViewProjectionMatrix()
{
	if (m_DirtyViewMatrix)
	{
		RecalculateViewMatrix();
	}

	return m_ViewProjectionMatrix;
}

void OrthographicCamera::SetPosition(const DirectX::XMFLOAT3& position)
{
	if (memcmp(&m_Position, &position, sizeof(DirectX::XMFLOAT3)) != 0)
	{
		m_Position = position;
		m_DirtyViewMatrix = true;
	}
}

void OrthographicCamera::SetRotation(const DirectX::XMFLOAT3& rotation)
{
	if (memcmp(&m_Rotation, &rotation, sizeof(DirectX::XMFLOAT3)) != 0)
	{
		m_Rotation = rotation;
		m_DirtyViewMatrix = true;
	}
}