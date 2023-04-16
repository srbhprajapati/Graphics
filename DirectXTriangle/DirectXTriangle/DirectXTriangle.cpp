#include <Windows.h>
#include <d3d12.h>
#include <iostream>
#include <dxgi1_4.h>
#include <vector>
#include "d3dx12.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define Check(x) \
	if(!(x)){MessageBoxA(0, #x, "Check Failed", MB_OK); __debugbreak();}

#define CheckSucceeded(hresult) Check(SUCCEEDED(hresult))

#define NUMFRAMES 3

using namespace std;

bool quit;

//D3D12 Global Variables
ID3D12Device* device;
IDXGIFactory2* dxgiFactory;
IDXGISwapChain3* swapChain;
ID3D12CommandQueue* graphicsCommandQueue;
ID3D12CommandAllocator* commandListAllocator;
ID3D12RootSignature* rootSignature;
ID3D12Debug* debugInterface;
ID3D12DescriptorHeap* renderTargetDescriptorHeap;

D3D12_CPU_DESCRIPTOR_HANDLE renderTargetCPUDescriptorHandles[NUMFRAMES];


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


void PrintShaderCompileErrorMessages(ID3DBlob* errorBlob)
{
	char temp[1024];
	sprintf_s(temp, sizeof(temp),"%.*s", (int)errorBlob->GetBufferSize(), (char *)errorBlob->GetBufferPointer());
	OutputDebugStringA(temp);

}

HRESULT CompileShader(const WCHAR* filename, const char* entryPoint, const char* profile, ID3DBlob** outBlob)
{
	ID3DBlob* errorBlob;
	HRESULT hresult = D3DCompileFromFile(filename, 0, 0, entryPoint, profile, D3DCOMPILE_DEBUG, 0, outBlob, &errorBlob);
	if (hresult != S_OK)
	{
		PrintShaderCompileErrorMessages(errorBlob);
	}

	return hresult;
}

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
	hresult = D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface));
	CheckSucceeded(hresult);
	debugInterface->EnableDebugLayer();
	
	hresult = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	CheckSucceeded(hresult);

	//UINT i = 0;
	//IDXGIAdapter* pAdapter;
	//std::vector <IDXGIAdapter*> vAdapters;
	//while (dxgiFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	//{
	//	vAdapters.push_back(pAdapter);
	//	++i;
	//}


	hresult = D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device));
	CheckSucceeded(hresult);

	D3D12_COMMAND_QUEUE_DESC graphicsCommandQueueDesc = {};
	graphicsCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	graphicsCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	hresult = device->CreateCommandQueue(&graphicsCommandQueueDesc, IID_PPV_ARGS(&graphicsCommandQueue));
	CheckSucceeded(hresult);



	IDXGISwapChain1* oldSwapChain;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferCount = NUMFRAMES;
	swapChainDesc.Width = 800;
	swapChainDesc.Height = 800;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	hresult = dxgiFactory->CreateSwapChainForHwnd(graphicsCommandQueue, window, &swapChainDesc, 0, 0, &oldSwapChain);
	CheckSucceeded(hresult);

	hresult = oldSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));
	CheckSucceeded(hresult);


	

	D3D12_DESCRIPTOR_HEAP_DESC renderTargetHeapDesc = {};
	renderTargetHeapDesc.NumDescriptors = NUMFRAMES;
	renderTargetHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	hresult = device->CreateDescriptorHeap(&renderTargetHeapDesc, IID_PPV_ARGS(&renderTargetDescriptorHeap));
	CheckSucceeded(hresult);

	size_t descriptorIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetDescriptorHandle = renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (int frame = 0; frame < NUMFRAMES; frame++)
	{
		ID3D12Resource* bufferResource;
		hresult = swapChain->GetBuffer(frame, IID_PPV_ARGS(&bufferResource));
		CheckSucceeded(hresult);
		D3D12_RENDER_TARGET_VIEW_DESC rtvd = {};
		rtvd.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
		device->CreateRenderTargetView(bufferResource, &rtvd, renderTargetDescriptorHandle);
		renderTargetCPUDescriptorHandles[frame] = renderTargetDescriptorHandle;
		renderTargetDescriptorHandle.ptr += descriptorIncrementSize;
	}

	hresult = device->CreateCommandAllocator(graphicsCommandQueueDesc.Type, IID_PPV_ARGS(&commandListAllocator));
	CheckSucceeded(hresult);


	ID3DBlob* vertexShader;
	hresult = CompileShader(L"C:\\Users\\srbhp\\Documents\\Github\\Graphics\\DirectXTriangle\\DirectXTriangle\\Shader.hlsl", "vsMain", "vs_5_0", &vertexShader);
	CheckSucceeded(hresult);
	ID3DBlob* pixelShader;
	hresult = CompileShader(L"C:\\Users\\srbhp\\Documents\\Github\\Graphics\\DirectXTriangle\\DirectXTriangle\\Shader.hlsl", "psMain", "ps_5_0", &pixelShader);
	CheckSucceeded(hresult);


	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, 0, 0, 0, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ID3DBlob* rootSignatureBlob;
	ID3DBlob* rootSignatureErrorBlob;
	hresult = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &rootSignatureErrorBlob);
	CheckSucceeded(hresult);

	hresult = device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	CheckSucceeded(hresult);

	D3D12_VIEWPORT viewport = { 0 };
	viewport.Width = (float)swapChainDesc.Width;
	viewport.Height = (float)swapChainDesc.Height;

	D3D12_RECT scissorRect = { 0 };
	scissorRect.right = swapChainDesc.Width;
	scissorRect.bottom = swapChainDesc.Height;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelinStateDesc = { 0 };
	pipelinStateDesc.VS.pShaderBytecode = vertexShader->GetBufferPointer();
	pipelinStateDesc.VS.BytecodeLength = vertexShader->GetBufferSize();
	pipelinStateDesc.PS.pShaderBytecode = pixelShader->GetBufferPointer();
	pipelinStateDesc.PS.BytecodeLength = pixelShader->GetBufferSize();
	pipelinStateDesc.pRootSignature = rootSignature;
	pipelinStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelinStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipelinStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pipelinStateDesc.DepthStencilState.DepthEnable = FALSE;
	pipelinStateDesc.DepthStencilState.StencilEnable = FALSE;
	pipelinStateDesc.NumRenderTargets = 1;
	pipelinStateDesc.RTVFormats[0] = swapChainDesc.Format;
	pipelinStateDesc.SampleMask = UINT_MAX;
	pipelinStateDesc.SampleDesc.Count = 1;
	pipelinStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	ID3D12PipelineState* pipelineState;
	hresult = device->CreateGraphicsPipelineState(&pipelinStateDesc, IID_PPV_ARGS(&pipelineState));
	CheckSucceeded(hresult);

	ID3D12GraphicsCommandList* graphicsCommandList;
	commandListAllocator->Reset();
	hresult = device->CreateCommandList(0, graphicsCommandQueueDesc.Type, commandListAllocator, 0, IID_PPV_ARGS(&graphicsCommandList));
	CheckSucceeded(hresult);


	while (!quit)
	{
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		UINT currFrame = swapChain->GetCurrentBackBufferIndex();

		D3D12_CPU_DESCRIPTOR_HANDLE currRenderTargetDescHandle = renderTargetCPUDescriptorHandles[currFrame];
		
		ID3D12Resource* bufferResource1;
		hresult = swapChain->GetBuffer(currFrame, IID_PPV_ARGS(&bufferResource1));
		CheckSucceeded(hresult);


		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			bufferResource1,
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		// Indicate that the back buffer will be used as a render target

		float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		graphicsCommandList->ResourceBarrier(1, &barrier);
		graphicsCommandList->OMSetRenderTargets(1, &currRenderTargetDescHandle, 0, 0);

		graphicsCommandList->ClearRenderTargetView(currRenderTargetDescHandle, clearColor, 0, 0);

		graphicsCommandList->RSSetViewports(1, &viewport);
		graphicsCommandList->RSSetScissorRects(1, &scissorRect);

		graphicsCommandList->SetGraphicsRootSignature(rootSignature);

		graphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphicsCommandList->SetPipelineState(pipelineState);

		graphicsCommandList->DrawInstanced(3, 1, 0, 0);
		

		CD3DX12_RESOURCE_BARRIER reverseBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			bufferResource1,
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

		graphicsCommandList->ResourceBarrier(1, &reverseBarrier);





		hresult = graphicsCommandList->Close();
		CheckSucceeded(hresult);
		

		ID3D12CommandList* commandLists[] = { graphicsCommandList };
		graphicsCommandQueue->ExecuteCommandLists(1, commandLists);



		swapChain->Present(1, 0);

		graphicsCommandList->Reset(commandListAllocator,0);
	}


	return 0;
}
