#pragma once
#include "DrawableBase.h"

class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float r;
	float roll = 0.0f;      //水平偏航转角	绕Y轴旋转
	float pitch = 0.0f;		//上下俯仰转角	绕X轴旋转
	float yaw = 0.0f;		//旋转角			绕Z轴旋转
	float theta;			//x-y平面上的夹角 同roll
	float phi;
	float chi;
	// speed(detal/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};
