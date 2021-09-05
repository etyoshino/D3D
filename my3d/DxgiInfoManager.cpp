#include "DxgiInfoManager.h"
#include "Window.h"
#include "Graphics.h"
#include <dxgidebug.h>
#include <memory>
#include "WindowsThrowMacros.h"

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

DxgiInfoManager::DxgiInfoManager()
{
	// 定义方法DXGIGetDebugInterface的函数类型
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// 加载包含DXGIGetDebugInterface函数的dll(动态链接库Dynamic Link Library) 失败抛出异常
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// 获取DXGIGetDebugInterface的指针
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(pDxgiInfoQueue.GetAddressOf())));
}

DxgiInfoManager::~DxgiInfoManager()
{
	if (pDxgiInfoQueue != nullptr) {
		pDxgiInfoQueue->Release();
	}
}

void DxgiInfoManager::Set()
{
	// 设置消息索引
	// 在改方法调用后才能拿到生成的错误
	next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;
		// 获得消息i的大小(bytes)
		GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// 为消息分配内存
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// 拿到消息将消息描述存入消息队列
		GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}