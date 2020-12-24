#pragma once

#include <memory>
#include <GL/glew.h>
#include "glm/glm.hpp"

class VertexArray;
class IndexBuffer;
class VertexBuffer;
class TextureArray;
class Shader;

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlApiCall(x) ClearGlErrors(); x; ASSERT(LogGlErrors(__FILE__, #x, __LINE__))

void ClearGlErrors();
bool LogGlErrors(const char* file, const char* function, int line);

const size_t MAX_QUADS(10000);
const size_t MAX_VERTS(4 * MAX_QUADS);
const size_t MAX_INDICES(6 * MAX_QUADS);
const size_t MAX_TEXTURES(32);

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexId;
};

class Renderer
{	
private:

public:
	Renderer();
	~Renderer();

	void Draw(const VertexArray& vb, const IndexBuffer& ib, 
			  const Shader& shader, const unsigned int count = 0) const;
	void Clear() const;
	void SetClearColor(glm::vec4 color);
};

class BatchRenderer
{
public:
	static void Init();
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();
	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void Clear();
	static void SetClearColor(glm::vec4 color);

	static void AddQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float texId);

	struct Stats
	{
		uint32_t drawCount = 0;
		uint32_t quadCount = 0;
	};

	static const Stats& GetStats();
	static void ResetStats();

};

class RenderData
{
public:
	void Init();
	void Destroy();

	Vertex* vertices;
	Vertex* currVertex;
	uint32_t indexCount;
	std::unique_ptr<VertexArray> vao;
	std::unique_ptr<VertexBuffer> vbo;
	std::unique_ptr<IndexBuffer> ibo;
	std::unique_ptr<TextureArray> tex;
	std::unique_ptr<Shader> shader;
	BatchRenderer::Stats stats;
	bool doneInit = false;
};
