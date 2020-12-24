#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "TextureArray.h"
#include "Renderer.h"

#include <iostream>

void ClearGlErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool LogGlErrors(const char* file, const char* function, int line)
{
    while (GLenum err = glGetError())
    {
        std::cout << "OPENGL ERROR: (0x" << std::hex << err << ") in "
            << function << " " << file << ": " << std::dec << line << std::endl;
        return false;
    }
    return true;
}

/* Batch-capable renderer class for use with test::Test subclasses */
Renderer::Renderer()
{
	GlApiCall(glEnable(GL_BLEND));
	GlApiCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

Renderer::~Renderer()
{
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, 
                    const Shader& shader, const unsigned int count) const
{
    shader.Bind();

    va.Bind();
    ib.Bind();
    unsigned int indexCount = count == 0 ? ib.GetCount() : count;
    GlApiCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GlApiCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetClearColor(glm::vec4 color)
{
    GlApiCall(glClearColor(color.r, color.g, color.b, color.a));
}

#if 1
static RenderData renderData;

/* Static batch-only rendererer */
void BatchRenderer::Init()
{
	renderData.Init();
}

void BatchRenderer::Shutdown()
{
	renderData.Destroy();
}

void BatchRenderer::BeginFrame()
{
	renderData.indexCount = 0;
	renderData.currVertex = renderData.vertices;

	SetClearColor({ 0.5f, 0.5f, 0.5f, 1.0f });
	Clear();
	ResetStats();
	renderData.vao->Bind();
	renderData.vbo->Bind();
}

void BatchRenderer::EndFrame()
{
	renderData.vbo->Unbind();
	renderData.vao->Unbind();
}

void BatchRenderer::BeginBatch()
{
	renderData.currVertex = renderData.vertices;
}

void BatchRenderer::EndBatch()
{
	GLsizeiptr size = (uint8_t*)renderData.currVertex - (uint8_t*)renderData.vertices;
	renderData.vbo->Fill(renderData.vertices, size);
}

void BatchRenderer::Flush()
{
	GlApiCall(glDrawElements(GL_TRIANGLES, renderData.indexCount, GL_UNSIGNED_INT, nullptr));
	renderData.stats.drawCount++;
	renderData.indexCount = 0;
}

void BatchRenderer::Clear()
{
	GlApiCall(glClear(GL_COLOR_BUFFER_BIT));
}

void BatchRenderer::SetClearColor(glm::vec4 color)
{
	GlApiCall(glClearColor(color.r, color.g, color.b, color.a));
}

void BatchRenderer::AddQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float texId)
{
	if (renderData.indexCount >= MAX_INDICES)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	renderData.currVertex->Position =  { pos.x, pos.y, 0.0f };
	renderData.currVertex->Color = color;// { 1.0f, 1.0f, 1.0f, 1.0f };
	renderData.currVertex->TexCoords = { 0.0f, 0.0f };
	renderData.currVertex->TexId = texId;
	renderData.currVertex++;

	renderData.currVertex->Position =  { pos.x + size.x, pos.y, 0.0f };
	renderData.currVertex->Color = color;// { 1.0f, 0.0f, 0.0f, 1.0f };
	renderData.currVertex->TexCoords = { 1.0f, 0.0f };
	renderData.currVertex->TexId = texId;
	renderData.currVertex++;
	
	renderData.currVertex->Position =  { pos.x + size.x, pos.y + size.y, 0.0f };
	renderData.currVertex->Color = color;// { 0.0f, 1.0f, 0.0f, 1.0f };
	renderData.currVertex->TexCoords = { 1.0f, 1.0f };
	renderData.currVertex->TexId = texId;
	renderData.currVertex++;
	
	renderData.currVertex->Position =  { pos.x, pos.y + size.y, 0.0f };
	renderData.currVertex->Color = color;// { 0.0f, 0.0f, 1.0f, 1.0f };
	renderData.currVertex->TexCoords = { 0.0f, 1.0f };
	renderData.currVertex->TexId = texId;
	renderData.currVertex++;

	renderData.indexCount += 6;
	renderData.stats.quadCount++;
}

const BatchRenderer::Stats& BatchRenderer::GetStats()
{
	return renderData.stats;
}

void BatchRenderer::ResetStats()
{
	renderData.stats.drawCount = 0;
	renderData.stats.quadCount = 0;
}

void RenderData::Init()
{
	if (!doneInit)
	{
		indexCount = 0;
		// Allocated verts
		vertices = new Vertex[MAX_VERTS];
		currVertex = vertices;

		// Create indices
		unsigned int indices[MAX_INDICES];
		unsigned int offset = 0;
		for (unsigned int i = 0; i < MAX_INDICES; i += 6)
		{
			/*
				3-----2
				|    /|
				|   / |
				|  /  |
				| /   |
				0-----1
			*/
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			// 4 verts, 6 indices per quad -> increment offset by 4 after every 6 indices
			offset += 4;
		}
		vao = std::make_unique<VertexArray>();
		vbo = std::make_unique<VertexBuffer>();
		VertexBufferLayout vblayout;
		vblayout.Push<float>(3);
		vblayout.Push<float>(4);
		vblayout.Push<float>(2);
		vblayout.Push<float>(1);

		vao->AddBuffer(*vbo, vblayout);
		ibo = std::make_unique<IndexBuffer>(indices, MAX_INDICES);

		std::vector<std::string> filepaths{ "resources/textures/Kreator.png",
											"resources/textures/KreatorBW.png" };
		tex = std::make_unique<TextureArray>(filepaths);

		shader = std::make_unique<Shader>("resources/shaders/VFBatch.glsl");

		stats = BatchRenderer::Stats();
		doneInit = true;
	}

	indexCount = 0;
	currVertex = vertices;

	tex->Bind(0);
	shader->Bind();

	glm::mat4 proj = glm::ortho(-600.f, 600.f, -337.f, 337.f, -1.0f, 1.0f);
	glm::mat4 view = glm::mat4(1.0);
	glm::mat4 mvp = proj * view;

	shader->SetUniformMat4f("u_MVP", mvp);

	/*
		Arrays of samplers can only be indexed by a compile time const
		expr in GLSL 3.30.  This is why we're using TextureArray.
	*/
	shader->SetUniform1i("u_TexUnit", 0);
}

void RenderData::Destroy()
{
	delete[] vertices;
	delete vao.release();
	delete vbo.release();
	delete ibo.release();
	delete tex.release();
	delete shader.release();
	doneInit = false;
}

#endif
