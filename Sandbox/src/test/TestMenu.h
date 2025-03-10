#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include "Test.h"

namespace test
{
	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& currentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> tests;
	};

}