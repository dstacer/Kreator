#pragma once

namespace test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float delta) {}
		virtual void OnRender() {}
		virtual void OnImguiRender() {}

	};
}