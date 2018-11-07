// SafeMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "MapImage.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <assert.h>

// Maybe convert to lib / DLL?

CapcomRoutines *capcom;

void open_binary_file(const std::string& file, std::vector<uint8_t>& data) // Useless in our actual implementation;
{
	std::ifstream file_stream(file, std::ios::binary);
	file_stream.unsetf(std::ios::skipws);
	file_stream.seekg(0, std::ios::end);

	const auto file_size = file_stream.tellg();

	file_stream.seekg(0, std::ios::beg);
	data.reserve(static_cast<uint32_t>(file_size));
	data.insert(data.begin(), std::istream_iterator<uint8_t>(file_stream), std::istream_iterator<uint8_t>());
}

int main(int argc, char** argv)
{
	if (!Np_LockSections()) // Failed to setup locked sections, fail!
		return 0; 

	// Loader initial checks...

	KernelContext* KrCtx = Kr_InitContext();
	CapcomContext* CpCtx = Cl_InitContext();

	capcom = new CapcomRoutines(KrCtx, CpCtx);

	if (!KrCtx || !CpCtx)
		return -1;

	Khu_Init(CpCtx, KrCtx);
	std::vector<uint8_t> driver_image;
	open_binary_file(argv[1], driver_image);
	MapImage driver(driver_image);
	const auto kernel_memory = capcom->allocate_pool(0, NonPagedPool, true);

	if (kernel_memory == 0)
		return -1;


// Good old cleanup ;)
	delete capcom;
	Cl_FreeContext(CpCtx);
	Kr_FreeContext(KrCtx);

    std::cout << "Hello World!\n"; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
