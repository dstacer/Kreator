#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

namespace test
{
	class Test
	{
	protected:
		std::unique_ptr<VertexArray> m_Vao;
		std::unique_ptr<VertexBuffer> m_Vbo;
		std::unique_ptr<IndexBuffer> m_Ibo;
		std::unique_ptr<Shader> m_Shader;
	public:
		Test() {}
		virtual ~Test() { std::cout << "Deleting test." << std::endl; }

		virtual void OnUpdate(float delta) {}
		virtual void OnRender() {}
		virtual void OnImguiRender() {}
	};

	class TestMenu : public Test
	{
	private:
		Test*& m_CurrentTest;
		std::vector< std::pair <std::string, std::function<Test*()>>> m_Tests;
	
	public:
		TestMenu(Test*& currentTest);
		virtual void OnImguiRender() override;
		
		template<typename T>
		void AddTest(const std::string& testName)
		{
			std::cout << "Adding test: \"" << testName << "\"" << std::endl;
			m_Tests.push_back(std::make_pair(testName, []() { return new T; }));
		}
	};
}