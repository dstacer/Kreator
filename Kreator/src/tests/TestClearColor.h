#pragma once

#include "Test.h"

namespace test 
{
	class TestClearColor : public Test
	{
	private:
		float m_ClearColor[4];
		
	public:
		TestClearColor();
		virtual ~TestClearColor();

		virtual void OnUpdate(float delta) override;
		virtual void OnRender() override;
		virtual void OnImguiRender() override;
	};

}