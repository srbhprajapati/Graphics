#include <Windows.h>
#include <d3d12.h>
#include <iostream>
#include <dxgi1_4.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#define Check(x) \
	if(!(x)){MessageBoxA(0, #x, "Check Failed", MB_OK); __debugbreak();}

#define CheckSucceeded(hresult) Check(SUCCEEDED(hresult))

using namespace std;

bool quit;

//D3D12 Global Variables
ID3D12Device* device;
IDXGIFactory2* dxgiFactory;
IDXGISwapChain1* swapChain;
ID3D12CommandQueue* graphicsCommandQueue;
ID3D12CommandAllocator* commandListAllocator;



LRESULT CALLBACK WindProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;

	switch (message)
	{
	case WM_DESTROY:
		quit = true;
		break;
	default:
		result = DefWindowProcA(window, message, wparam, lparam);
		break;
	}
	return result;
};


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int showCommand)
{
	WNDCLASSA windowClass = { 0 };
	windowClass.lpfnWndProc = WindProc;
	windowClass.lpszClassName = "DirectXTriangle";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassA(&windowClass);

	HWND window = CreateWindowA("DirectXTriangle", "DirectXTriangle", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 800, 0, 0, instance, 0);
	ShowWindow(window, SW_SHOW);


	HRESULT hresult;
	hresult = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	CheckSucceeded(hresult);

	UINT i = 0;
	IDXGIAdapter* pAdapter;
	std::vector <IDXGIAdapter*> vAdapters;
	while (dxgiFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		vAdapters.push_back(pAdapter);
		++i;
	}


	hresult = D3D12CreateDevice(vAdapters[0], D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
	CheckSucceeded(hresult);

	D3D12_COMMAND_QUEUE_DESC graphicsCommandQueueDesc = {};
	graphicsCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	graphicsCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	hresult = device->CreateCommandQueue(&graphicsCommandQueueDesc, IID_PPV_ARGS(&graphicsCommandQueue));
	CheckSucceeded(hresult);


	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.BufferCount = 3;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	hresult = dxgiFactory->CreateSwapChainForHwnd(graphicsCommandQueue, window, &swapChainDesc, 0, 0, &swapChain);
	CheckSucceeded(hresult);




	while (!quit)
	{
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}


		swapChain->Present(0, 0);
	}


	return 0;
}
