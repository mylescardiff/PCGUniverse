#include "stdafx.h"
#include "CppUnitTest.h"

#include <Utilities/Vector2.h>
#include <Logger/Logger.h>
//#include "../ThirdParty/TinyXML/tinyxml2.h"
//#include "../ThirdParty/Lua/include/lua.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SlothUnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestLogger)
		{
			slth::Logger::LogError("test");
		}

		TEST_METHOD(CreateVector)
		{
			slth::Vector2 test(2.f, 2.f);

		}
	};
}