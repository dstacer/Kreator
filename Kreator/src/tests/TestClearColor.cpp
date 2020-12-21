#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "Renderer.h"
#include "TestClearColor.h"


namespace test
{

	TestClearColor::TestClearColor()
		: m_ClearColor { 0.0f, 0.0f, 0.0f, 1.0f } 
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float delta)
	{
	}

	void TestClearColor::OnRender()
	{
		GlApiCall(glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a));
		GlApiCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnImguiRender()
	{
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	}
}
