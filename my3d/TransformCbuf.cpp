#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	pVcbuf(gfx),
	pParent(parent)
{}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	pVcbuf.Update(gfx,
		DirectX::XMMatrixTranspose(
			pParent.GetTransformXM() * gfx.GetProjection()
		)
	);
	pVcbuf.Bind(gfx);
}
