#include "PrecompiledHeader.h"
#include "Core\Settings.h"
#include "Core\Window.h"
#include "D3D11GraphicsContext.h"
#include "Environment.h"
#include "Utilities\Utilities.h"

Environment::Environment() :
	m_DataBuffer(new DataStorage[Settings::kPathTracingBufferSize]),
	m_BufferBegin(0),
	m_BufferEnd(0)
{
	LoadShaders();
	CreateBuffers();
}

Environment::~Environment()
{
}

void Environment::AddData(const DirectX::XMFLOAT2& dataItem)
{
	m_DataBuffer[m_BufferEnd].position = dataItem;
	m_DataBuffer[m_BufferEnd].timestamp = static_cast<float>(Utilities::GetTime());

	m_BufferEnd = (m_BufferEnd + 1) % Settings::kPathTracingBufferSize;
	if (m_BufferEnd == m_BufferBegin)
	{
		m_BufferBegin = (m_BufferBegin + 1) % Settings::kPathTracingBufferSize;
	}
}

void Environment::AddData(const std::vector<DirectX::XMFLOAT2>& data)
{
	for (const auto& dataItem : data)
	{
		AddData(dataItem);
	}
}

void Environment::Render(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	// Upload data

	float time = static_cast<float>(Utilities::GetTime());
	UploadConstantBufferData(viewProjectionMatrix, time);
	
	// Figure out what to draw


	while (m_BufferBegin != m_BufferEnd)
	{
		if (time - m_DataBuffer[m_BufferBegin].timestamp < Settings::kPathTracingDecayTime)
		{
			break;
		}

		m_BufferBegin = (m_BufferBegin + 1) % Settings::kPathTracingBufferSize;
	}

	// Set GPU state

	UINT offsets[] = { 0, 0 };
	ID3D11Buffer* vertexBuffers[] = { m_VertexBuffer.Get(), m_InstanceBuffer.Get() };
	auto deviceContext = GetD3D11DeviceContext();

	deviceContext->IASetInputLayout(m_InputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0, 2, vertexBuffers, m_VertexStrides, offsets);
	deviceContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
	deviceContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	// Draw

	if (m_BufferBegin < m_BufferEnd)
	{
		deviceContext->DrawInstanced(m_VertexCount, m_BufferEnd - m_BufferBegin, 0, m_BufferBegin);
	}
	else if (m_BufferBegin > m_BufferEnd)
	{
		if (m_BufferBegin - 1 == m_BufferEnd)
		{
			deviceContext->DrawInstanced(m_VertexCount, Settings::kPathTracingBufferSize, 0, 0);
		}
		else
		{
			deviceContext->DrawInstanced(m_VertexCount, Settings::kPathTracingBufferSize - m_BufferBegin, 0, m_BufferBegin);
			deviceContext->DrawInstanced(m_VertexCount, m_BufferEnd, 0, 0);
		}
	}
}

void Environment::UploadConstantBufferData(const DirectX::XMMATRIX& viewProjectionMatrix, float currentTime)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto deviceContext = GetD3D11DeviceContext();

	// Upload instance buffer data

	auto result = deviceContext->Map(m_InstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Assert(result == S_OK);

	memcpy(mappedResource.pData, m_DataBuffer.get(), Settings::kPathTracingBufferSize * sizeof(DataStorage));

	deviceContext->Unmap(m_InstanceBuffer.Get(), 0);

	// Upload constant buffer data

	result = deviceContext->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Assert(result == S_OK);

	auto bufferPtr = static_cast<ConstantBuffer*>(mappedResource.pData);

	bufferPtr->viewProjectionMatrix = viewProjectionMatrix;
	bufferPtr->currentTime = currentTime;
	bufferPtr->lifeTime = Settings::kPathTracingDecayTime;

	deviceContext->Unmap(m_ConstantBuffer.Get(), 0);
}

void Environment::LoadShaders()
{
	HRESULT result;
	const auto vertexShaderData = Utilities::ReadFileToVector(L"Shaders\\EnvironmentVertexShader.cso");
	const auto pixelShaderData = Utilities::ReadFileToVector(L"Shaders\\EnvironmentPixelShader.cso");
	auto device = GetD3D11Device();

	result = device->CreateVertexShader(vertexShaderData.data(), vertexShaderData.size(), nullptr, &m_VertexShader);
	Assert(result == S_OK);

	result = device->CreatePixelShader(pixelShaderData.data(), pixelShaderData.size(), nullptr, &m_PixelShader);
	Assert(result == S_OK);

	D3D11_INPUT_ELEMENT_DESC inputLayoutDescription[3];
	inputLayoutDescription[0].SemanticName = "BASE_POSITION";
	inputLayoutDescription[0].SemanticIndex = 0;
	inputLayoutDescription[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayoutDescription[0].InputSlot = 0;
	inputLayoutDescription[0].AlignedByteOffset = 0;
	inputLayoutDescription[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDescription[0].InstanceDataStepRate = 0;

	inputLayoutDescription[1].SemanticName = "INSTANCE_POSITION";
	inputLayoutDescription[1].SemanticIndex = 0;
	inputLayoutDescription[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayoutDescription[1].InputSlot = 1;
	inputLayoutDescription[1].AlignedByteOffset = 0;
	inputLayoutDescription[1].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	inputLayoutDescription[1].InstanceDataStepRate = 1;

	inputLayoutDescription[2].SemanticName = "INSTANCE_SPAWN_TIME";
	inputLayoutDescription[2].SemanticIndex = 0;
	inputLayoutDescription[2].Format = DXGI_FORMAT_R32_FLOAT;
	inputLayoutDescription[2].InputSlot = 1;
	inputLayoutDescription[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayoutDescription[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	inputLayoutDescription[2].InstanceDataStepRate = 1;

	result = device->CreateInputLayout(inputLayoutDescription, sizeof(inputLayoutDescription) / sizeof(D3D11_INPUT_ELEMENT_DESC),
		vertexShaderData.data(), vertexShaderData.size(), &m_InputLayout);
	Assert(result == S_OK);
}

void Environment::CreateBuffers()
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	auto device = GetD3D11Device();

	// Vertex buffer

	const float size = Settings::kCentimetersInWindowHeight / Window::GetInstance().GetHeight();

	const DirectX::XMFLOAT4 vertices[] =
	{
		{ -size, size, 0.0f, 1.0f },
		{ size, size, 0.0f, 1.0f },
		{ size, -size, 0.0f, 1.0f },

		{ -size, size, 0.0f, 1.0f },
		{ size, -size, 0.0f, 1.0f },
		{ -size, -size, 0.0f, 1.0f },
	};

	m_VertexStrides[0] = sizeof(DirectX::XMFLOAT4);
	m_VertexCount = sizeof(vertices) / m_VertexStrides[0];

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

	// Instance buffer

	m_VertexStrides[1] = sizeof(DataStorage);

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = Settings::kPathTracingBufferSize * sizeof(DataStorage);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = GetD3D11Device()->CreateBuffer(&bufferDesc, nullptr, &m_InstanceBuffer);
	Assert(result == S_OK);

	// Constant buffer

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = GetD3D11Device()->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer);
	Assert(result == S_OK);
}
