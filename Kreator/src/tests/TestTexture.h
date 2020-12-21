#pragma once

#include "Test.h"

namespace test 
{
	class TestTexture : public Test
	{
	private:
		std::unique_ptr<Texture> m_Texture;
		
		
	public:
		TestTexture();

		virtual void OnUpdate(float delta) override;
		virtual void OnRender() override;
		virtual void OnImguiRender() override;
	};

}