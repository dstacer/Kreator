#pragma once

#include "glm/glm.hpp"
#include "Test.h"

namespace test 
{
	class TestClearColor : public Test
	{
	private:
		glm::vec4 m_ClearColor;
		
	public:
		TestClearColor();
		virtual ~TestClearColor();

		virtual void OnUpdate(float delta) override;
		virtual void OnRender() override;
		virtual void OnImguiRender() override;
	};

}