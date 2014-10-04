#include "PrecompiledHeader.h"
#include "D3D11GraphicsContext.h"
#include "Tank.h"
#include "Utilities\Utilities.h"

Tank::Tank() :
	m_Position(0.0f, 0.0f, 0.0f),
	m_Rotation(0.0f, 0.0f, 0.0f),
	m_DirtyWorldMatrix(true)
{
	LoadShaders();
	CreateBuffers();
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
	auto deviceContext = GetD3D11DeviceContext();
	UploadConstantBufferData(viewProjectionMatrix);

	UINT offset = 0;

	deviceContext->IASetInputLayout(m_InputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VertexStride, &offset);
	deviceContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
	deviceContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	deviceContext->Draw(m_VertexCount, 0);
}

void Tank::UploadConstantBufferData(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	if (m_DirtyWorldMatrix)
	{
		RecalculateWorldMatrix();
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto deviceContext = GetD3D11DeviceContext();

	auto result = deviceContext->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Assert(result == S_OK);

	*reinterpret_cast<DirectX::XMMATRIX*>(mappedResource.pData) = viewProjectionMatrix * m_WorldMatrix;

	deviceContext->Unmap(m_ConstantBuffer.Get(), 0);
}

void Tank::LoadShaders()
{
	HRESULT result;
	const auto vertexShaderData = Utilities::ReadFileToVector(L"Shaders\\TankVertexShader.cso");
	const auto pixelShaderData = Utilities::ReadFileToVector(L"Shaders\\TankPixelShader.cso");
	auto device = GetD3D11Device();

	result = device->CreateVertexShader(vertexShaderData.data(), vertexShaderData.size(), nullptr, &m_VertexShader);
	Assert(result == S_OK);

	result = device->CreatePixelShader(pixelShaderData.data(), pixelShaderData.size(), nullptr, &m_PixelShader);
	Assert(result == S_OK);

	D3D11_INPUT_ELEMENT_DESC inputLayoutDescription[1];
	inputLayoutDescription[0].SemanticName = "POSITION";
	inputLayoutDescription[0].SemanticIndex = 0;
	inputLayoutDescription[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayoutDescription[0].InputSlot = 0;
	inputLayoutDescription[0].AlignedByteOffset = 0;
	inputLayoutDescription[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDescription[0].InstanceDataStepRate = 0;

	result = device->CreateInputLayout(inputLayoutDescription, 1, vertexShaderData.data(), vertexShaderData.size(), &m_InputLayout);
	Assert(result == S_OK);
}

void Tank::CreateBuffers()
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	auto device = GetD3D11Device();

	// Vertex buffer

	const DirectX::XMFLOAT4 vertices[] =
	{
		{ -7.5f, 10.0f, 0.0f, 1.0f },
		{ 7.5f, 10.0f, 0.0f, 1.0f },
		{ 7.5f, -10.0f, 0.0f, 1.0f },

		{ -7.5f, 10.0f, 0.0f, 1.0f },
		{ 7.5f, -10.0f, 0.0f, 1.0f },
		{ -7.5f, -10.0f, 0.0f, 1.0f },

		{ -1.5f, 10.0f, 0.0f, 1.0f },
		{ -1.5f, 20.0f, 0.0f, 1.0f },
		{ 1.5f, 20.0f, 0.0f, 1.0f },

		{ -1.5f, 10.0f, 0.0f, 1.0f },
		{ 1.5f, 20.0f, 0.0f, 1.0f },
		{ 1.5f, 10.0f, 0.0f, 1.0f }
	};

	m_VertexStride = sizeof(DirectX::XMFLOAT4);
	m_VertexCount = sizeof(vertices) / m_VertexStride;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = GetD3D11Device()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer);
	Assert(result == S_OK);

	// Constant buffer

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = GetD3D11Device()->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer);
	Assert(result == S_OK);
}