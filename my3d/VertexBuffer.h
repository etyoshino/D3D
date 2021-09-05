#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<class Vbuffer>
	VertexBuffer(Graphics& gfx, const std::vector<Vbuffer>& vertices)
		:
		pStride(sizeof(Vbuffer))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT( sizeof(Vbuffer) * vertices.size() );
		bd.StructureByteStride = sizeof(Vbuffer);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &bd,&sd,&pVertexBuffer ) );
	};
	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT pStride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
