#include "VulkanApp.h"
#include <iostream>

int main()
{
	VulkanApp app;
	try
	{
		app.init();
		app.run();
	}
	catch (std::runtime_error& e)
	{
		std::cout << "Runtime error ocurred!\nWhat: " << e.what() << std::endl;
	}
	return 0;
}