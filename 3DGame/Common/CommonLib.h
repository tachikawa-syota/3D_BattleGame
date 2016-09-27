/**
* @file   CommonLib.h
* @brief  DirectX関係やよく使うマクロなどを管理
* @author Syota Tachikawa
*/

#ifndef ___COMMONLIB_H
#define ___COMMONLIB_H

// 警告抑制
#pragma warning(disable:4819)

//必要なライブラリファイルのロード
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

// DirectX
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <crtdbg.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <mmsystem.h>
#include <process.h>
#include <ddraw.h>
#include <mmeapi.h>
#include <string.h>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <random>
#include <functional>

// 名前空間
//using namespace std;
using std::list;
using std::vector;
using std::map;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::string;

template<typename _T>
/**
* @brief ポインタ安全解放
*/
void SafeDelete(_T*& p)
{
	if (p != nullptr){
		delete(p);
		(p) = nullptr;
	}
}

template<typename _T>
/**
* @brief デバイス安全解放
*/
void SafeRelease(_T*& p)
{
	if (p != nullptr){
		p->Release();
		p = nullptr;
	}
}

/// 安全リリース配列
template<typename _T>
/**
* @brief 安全リリース配列
*/
void SafeDeleteArray(_T*& p)
{
	if (p != nullptr){
		delete[] p;
		p = NULL;
	}
}

#define PI	3.1415926535f

// 度数法→弧度法の値に変換
#define RAD(r) (r * 3.141592654f / 180.0f)

#endif