#pragma once

class Environment
{
	struct DataStorage
	{
		DirectX::XMFLOAT2 position;
		float timestamp;
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX viewProjectionMatrix;
		float lifeTime;
		float currentTime;
	};

	std::unique_ptr<DataStorage[]> m_DataBuffer;

	size_t m_BufferBegin;
	size_t m_BufferEnd;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;

	UINT m_VertexStrides[2];
	UINT m_VertexCount;

	void LoadShaders();
	void CreateBuffers();

	void UploadConstantBufferData(const DirectX::XMMATRIX& viewProjectionMatrix, float currentTime);

public:
	Environment();
	~Environment();

	void AddData(const DirectX::XMFLOAT2& dataItem);
	void AddData(const std::vector<DirectX::XMFLOAT2>& data);
	void Render(const DirectX::XMMATRIX& viewProjectionMatrix);
};