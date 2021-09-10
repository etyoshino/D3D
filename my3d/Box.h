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
	float roll = 0.0f;      //ˮƽƫ��ת��	��Y����ת
	float pitch = 0.0f;		//���¸���ת��	��X����ת
	float yaw = 0.0f;		//��ת��			��Z����ת
	float theta;			//x-yƽ���ϵļн� ͬroll
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
