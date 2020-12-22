#pragma once

#include "Test.h"

class TextureArray;

namespace test 
{
	class TestBatch : public Test
	{
	private:
		std::unique_ptr<TextureArray> m_Texture;
		
	public:
		TestBatch();

		virtual void OnUpdate(float delta) override;
		virtual void OnRender() override;
		virtual void OnImguiRender() override;
	};

}