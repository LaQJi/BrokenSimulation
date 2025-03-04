#include "TestMenu.h"

namespace test
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		: currentTest(currentTestPointer)
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				currentTest = test.second();
			}
		}
	}
}