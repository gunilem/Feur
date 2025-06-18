#include "fpch.h"
#include "Core/Application.h"

#include <stdexcept>

static int allocationCount = 0;

void* operator new(std::size_t sz)
{
	allocationCount++;
	if (sz == 0)
		++sz; // avoid std::malloc(0) which may return nullptr on success

	if (void* ptr = std::malloc(sz))
		return ptr;

	throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void operator delete(void* ptr) noexcept
{
	allocationCount--;
	std::free(ptr);
}

int main() {
	GUFeur::Application* application = new GUFeur::Application();

	
	try {
		application->run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	delete application;

	std::cout << "app allocation : " << allocationCount;
	
	return EXIT_SUCCESS;
}