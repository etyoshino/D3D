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

		D3D11_BUFFER_DESC vbd;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		vbd.MiscFlags = 0u;
		vbd.ByteWidth = sizeof(Vbuffer) * vertices.size();
		vbd.StructureByteStride = sizeof(Vbuffer);
		D3D11_SUBRESOURCE_DATA vsd = {};
		vsd.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vbd, &vsd, &pVertexBuffer));
	};
	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT pStride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
