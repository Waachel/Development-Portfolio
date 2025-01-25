//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Engine.h"
#include "Model.h"

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device *pHackDevice;
extern ID3D11DeviceContext *pHackDeviceContext;

using namespace Azul;

Model::Model( unsigned int _numVerts, unsigned int _numIndices)
	:
	numVerts{_numVerts},
	numIndices{_numIndices},
	poVertexBuffer_pos{nullptr},
	poVertexBuffer_color{nullptr},
	poVertexBuffer_norm{nullptr},
	poVertexBuffer_texCoord{nullptr},
	poIndexBuffer{nullptr},
	poConstantBuff_Projection{nullptr},
	poConstantBuff_World{nullptr},
	poConstantBuff_View{nullptr}
{

}

Model::~Model()
{
	SafeRelease(poVertexBuffer_pos);
	SafeRelease(poVertexBuffer_color);
	SafeRelease(poVertexBuffer_norm);
	SafeRelease(poVertexBuffer_texCoord);
	SafeRelease(poIndexBuffer);
	SafeRelease(poConstantBuff_Projection);
	SafeRelease(poConstantBuff_World);
	SafeRelease(poConstantBuff_View);
}

void Model::RenderIndexBuffer()
{
	// ---------------------------------------------
	//    Set (point to ) Index buffer 
	//    Render configuration: Triangles
	// ---------------------------------------------
	pHackDeviceContext->IASetIndexBuffer(poIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pHackDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------------
	// RENDER - using index data
	//--------------------------------------------------------
	pHackDeviceContext->DrawIndexed(numIndices, 0, 0);
}

void Model::TransferConstantBuffer(Camera *pCam, Mat4 *pWorld)
{
	assert(pCam);
	assert(pWorld);

	pHackDeviceContext->UpdateSubresource(poConstantBuff_View, 0, nullptr, &pCam->getViewMatrix(), 0, 0);
	pHackDeviceContext->UpdateSubresource(poConstantBuff_Projection, 0, nullptr, &pCam->getProjMatrix(), 0, 0);
	pHackDeviceContext->UpdateSubresource(poConstantBuff_World, 0, nullptr, pWorld, 0, 0);

}
void Model::ActivateModel()
{

	// ---------------------------------------------
	//    Set (point to ) vertex: position 
	//    Set (point to ) vertex: color
	// ---------------------------------------------
	const UINT vertexStride_pos = sizeof(VertexPos);
	const UINT vertexStride_color = sizeof(VertexColor);
	const UINT offset = 0;

	if(poVertexBuffer_pos)
	{
		pHackDeviceContext->IASetVertexBuffers((uint32_t)VertexSlot::Position, 1, &poVertexBuffer_pos, &vertexStride_pos, &offset);
	}
	if(poVertexBuffer_color)
	{
		pHackDeviceContext->IASetVertexBuffers((uint32_t)VertexSlot::Color, 1, &poVertexBuffer_color, &vertexStride_color, &offset);
	}

	if(poVertexBuffer_norm)
	{
		assert(false);
	}

	if(poVertexBuffer_texCoord)
	{
		assert(false);
	}

	//--------------------------------------------------
	// Enable shader
	//    Set (point to the shader to use)
	//    Set (point to the constant buffers to use)
	//--------------------------------------------------
	pHackDeviceContext->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::Projection, 1, &poConstantBuff_Projection);
	pHackDeviceContext->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::View, 1, &poConstantBuff_View);
	pHackDeviceContext->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::World, 1, &poConstantBuff_World);

}

ID3D11Buffer *Model::CreateVertexBuffer(unsigned int NumBytes, void *pData)
{
	assert(pData);

	// Create an initialize the vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc{0};
	D3D11_SUBRESOURCE_DATA resourceData{0};

	// type of buffer
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = NumBytes;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	resourceData.pSysMem = pData;
	ID3D11Buffer *pVertexBuffer;
	HRESULT hr;
	hr = pHackDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &pVertexBuffer);
	assert(SUCCEEDED(hr));

	return pVertexBuffer;
}

ID3D11Buffer *Model::CreateIndexBuffer(unsigned int NumBytes, void *pData)
{
	assert(pData);

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc{0};
	D3D11_SUBRESOURCE_DATA resourceData{0};

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = NumBytes;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	resourceData.pSysMem = pData;

	ID3D11Buffer *pIndexBuffer;
	HRESULT hr;
	hr = pHackDevice->CreateBuffer(&indexBufferDesc, &resourceData, &pIndexBuffer);
	assert(SUCCEEDED(hr));

	return pIndexBuffer;
}

ID3D11Buffer *Model::CreateConstantBuffer(unsigned int NumBytes)
{

	D3D11_BUFFER_DESC constantBufferDesc{0};

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = NumBytes;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Buffer *pConstantBuffer;
	HRESULT hr;
	hr = pHackDevice->CreateBuffer(&constantBufferDesc, nullptr, &pConstantBuffer);
	assert(SUCCEEDED(hr));

	return pConstantBuffer;
}

// --- End of File ---