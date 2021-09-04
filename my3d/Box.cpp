#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"

Box::Box(Graphics& gfx,
    std::mt19937& rng,
    std::uniform_real_distribution<float>& adist,
    std::uniform_real_distribution<float>& ddist,
    std::uniform_real_distribution<float>& odist,
    std::uniform_real_distribution<float>& rdist)
    :
    r(rdist(rng)),
    droll(ddist(rng)),
    dpitch(ddist(rng)),
    dyaw(ddist(rng)),
    dphi(odist(rng)),
    dtheta(odist(rng)),
    dchi(odist(rng)),
    chi(adist(rng)),
    theta(adist(rng)),
    phi(adist(rng))
{
    struct Vertex
    {
        struct {
            float x;
            float y;
            float z;
        }pos;
    };
    const std::vector<Vertex> vertices =
    {
		{ -1.0f,-1.0f,-1.0f },
		{ 1.0f ,-1.0f,-1.0f },
		{ -1.0f,1.0f ,-1.0f },
		{ 1.0f ,1.0f ,-1.0f },
		{ -1.0f,-1.0f,1.0f },
		{ 1.0f ,-1.0f,1.0f },
		{ -1.0f,1.0f ,1.0f },
		{ 1.0f ,1.0f ,1.0f },
    };
    AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

    auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
    auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

    AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

    const std::vector<unsigned short> indices =
    {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7,
    };
}

void Box::Update(float dt) noexcept
{
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
    return DirectX::XMMATRIX();
}
