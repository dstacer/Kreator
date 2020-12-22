#include "TextureArray.h"
#include "TestBatch.h"

namespace test
{

	TestBatch::TestBatch()
	{
		float verts[] = {
			-0.6f, -0.75f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.6f, -0.75f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.6f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			-0.6f, -0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-0.6f,  0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.6f,  0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.6f,  0.75f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.6f,  0.75f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};									

		unsigned int indices[12] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4
		};

		m_Vao = std::make_unique<VertexArray>();
		m_Vbo = std::make_unique<VertexBuffer>(verts, 8 * 9 * sizeof(float));

		VertexBufferLayout vblayout;
		vblayout.Push<float>(2);
		vblayout.Push<float>(4); 
		vblayout.Push<float>(2);
		vblayout.Push<float>(1);

		m_Vao->AddBuffer(*m_Vbo, vblayout);

		m_Ibo = std::make_unique<IndexBuffer>(indices, 12); 
		m_Ibo->Bind();

		std::vector<std::string> filepaths{ "resources/textures/Kreator.png",
											"resources/textures/KreatorBW.png" };

		m_Texture = std::make_unique<TextureArray>(filepaths);
		
		m_Shader = std::make_unique<Shader>("resources/shaders/VFBatch.glsl");
		m_Shader->Bind();
		
		glm::mat4 proj = glm::ortho(-1.6f, 1.6f, -1.2f, 1.2f, -1.0f, 1.0f);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 mvp = proj * view;

		m_Shader->SetUniformMat4f("u_MVP", mvp);

		/*
			Arrays of samplers can only be indexed by a compile time const
			expr in GLSL 3.30.  This is why we're using TextureArray.
		*/
		m_Shader->SetUniform1i("u_TexUnit", 0);
		m_Texture->Bind(0);
	}

	void TestBatch::OnUpdate(float delta)
	{
	}

	void TestBatch::OnRender()
	{
		Renderer rend;
		
		rend.SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		rend.Clear();

		rend.Draw(*m_Vao, *m_Ibo, *m_Shader);
	}

	void TestBatch::OnImguiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
