#include "imgui/imgui.h"

#include "Renderer.h"
#include "Shader.h"
#include "TestFlatColor.h"


namespace test
{

	TestFlatColor::TestFlatColor()
	{
		float verts[] = {
			-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 1.0f, 1.0f, 1.0f
		};

		unsigned int indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		m_Vao = std::make_unique<VertexArray>();
		m_Vbo = std::make_unique<VertexBuffer>(verts, 4 * 5 * sizeof(float));

		VertexBufferLayout vblayout;
		vblayout.Push<float>(2);
		vblayout.Push<float>(3);
		m_Vao->AddBuffer(*m_Vbo, vblayout);

		m_Ibo = std::make_unique<IndexBuffer>(indices, 6);
		m_Ibo->Bind();

		m_Shader = std::make_unique<Shader>("resources/shaders/VFAttribColor.glsl");
		m_Shader->Bind();

		glm::mat4 proj = glm::ortho(-1.778f, 1.778f, -0.9f, 0.9f, -1.0f, 1.0f);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 mvp = proj * view;

		m_Shader->SetUniformMat4f("u_MVP", mvp);
	}

	TestFlatColor::~TestFlatColor()
	{
	}

	void TestFlatColor::OnUpdate(float delta)
	{
	}

	void TestFlatColor::OnRender()
	{
		Renderer rend;

		rend.SetClearColor({ 0.8f, 0.8f, 0.8f, 1.0f });
		rend.Clear();

		rend.Draw(*m_Vao, *m_Ibo, *m_Shader);
	}

	void TestFlatColor::OnImguiRender()
	{
		
	}
}
