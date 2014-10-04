#pragma once

__declspec(align(16))
class OrthographicCamera
{
private:
	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjectionMatrix;
	DirectX::XMMATRIX m_ViewProjectionMatrix;
	bool m_DirtyViewMatrix;

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Rotation;

	void RecalculateViewMatrix();
	void RecalculateProjectionMatrix(float aspectRatio);
	void RecalculateViewProjectionMatrix();

public:
	OrthographicCamera(float aspectRatio, const DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), 
					   const DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	~OrthographicCamera();

	const DirectX::XMMATRIX& GetViewProjectionMatrix();

	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetRotation(const DirectX::XMFLOAT3& rotation);
};

