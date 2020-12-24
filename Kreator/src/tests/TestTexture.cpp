#include "TestTexture.h"

namespace test
{

	TestTexture::TestTexture()
	{
		float verts[] = {
			-0.6f, -0.25f, 0.0f, 0.0f,
			 0.6f, -0.25f, 1.0f, 0.0f,
			 0.6f,  0.25f, 1.0f, 1.0f,
			-0.6f,  0.25f, 0.0f, 1.0f
		};

		unsigned int indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		m_Vao = std::make_unique<VertexArray>();
		m_Vbo = std::make_unique<VertexBuffer>(verts, 4 * 4 * sizeof(float));

		VertexBufferLayout vblayout;
		vblayout.Push<float>(2);
		vblayout.Push<float>(2);
		m_Vao->AddBuffer(*m_Vbo, vblayout);

		m_Ibo = std::make_unique<IndexBuffer>(indices, 6); 
		m_Ibo->Bind();

		m_Shader = std::make_unique<Shader>("resources/shaders/VFUniTexMVP.glsl");
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_TexSlot", 0);

		m_Texture = std::make_unique<Texture>("resources/textures/Kreator.png");

		glm::mat4 proj = glm::ortho(-1.778f, 1.778f, -0.9f, 0.9f, -1.0f, 1.0f);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 mvp = proj * view;

		m_Shader->SetUniformMat4f("u_MVP", mvp);
		
	}

	void TestTexture::OnUpdate(float delta)
	{
	}

	void TestTexture::OnRender()
	{
		Renderer rend;
		/* Render here */
		rend.SetClearColor({ 0.8f, 0.8f, 0.8f, 1.0f });
		rend.Clear();

		m_Texture->Bind();
		rend.Draw(*m_Vao, *m_Ibo, *m_Shader);
	}

	void TestTexture::OnImguiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
