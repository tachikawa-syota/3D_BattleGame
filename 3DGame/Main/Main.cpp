#include "..\ApplicationSystem\FrameWork.h"

/**
* @brief アプリケーションの初期エントリポイント
*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{ 
	// インスタンスを生成
	auto mainFrame = make_unique<FrameWork>();

	// フレームワークの作成に失敗したら"0"が返る
	if (!mainFrame->Create(hInst)) return 0;

	// 更新と描画
	mainFrame->Run();

	return 0;
}
