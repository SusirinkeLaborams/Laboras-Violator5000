#pragma once

#include "Core\NonCopyable.h"

__declspec(align(16)) 
class Tank : NonCopyable
{
private:
	DirectX::XMMATRIX m_WorldMatrix;
	bool m_DirtyWorldMatrix;

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Rotation;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;

	UINT m_VertexStride;
	UINT m_VertexCount;

	void LoadShaders();
	void CreateBuffers();

	void RecalculateWorldMatrix();
	void UploadConstantBufferData(const DirectX::XMMATRIX& viewProjectionMatrix);

public:
	Tank();
	~Tank();

	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetRotation(const DirectX::XMFLOAT3& rotation);

	void Render(const DirectX::XMMATRIX& viewProjectionMatrix);
};