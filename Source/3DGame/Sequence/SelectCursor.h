/**
* @file   SelectCursor.h
* @brief  選択カーソルの処理
* @author Syota Tachikawa
*/

#ifndef ___SELECTCURSOR_H
#define ___SELECTCURSOR_H

#include "..\Common\FileData.h"
#include "..\Common\STDXStruct.h"
#include "..\DirectX\InputMask.h"

/// タイトルに表示するテキストの座標(一番上に描画する物の)
const float TitlePosY = 400.0f;
const float TitlePosX = 1000.0f;
const float TitleCursorAdd = 90.0f;

/**
* @brief シーン中で使う選択カーソルのクラス
*/
class SelectCursor
{
public:
	/**
	* @brief コンストラクタ
	*/
	SelectCursor(const type_info& type);
	
	/**
	* @brief デストラクタ
	*/
	~SelectCursor();

	/**
	* @brief 更新
	* @return true - 選択終了：false - 処理の継続
	*/
	bool Update();

	/**
	* @brief 描画
	*/
	void Render(int alpha = 255);

	/**
	* @brief カーソル座標を取得する
	*/
	const Vector2& GetPos() const;

	/**
	* @brief カーソル番号の取得
	*/
	int GetNumber() const;

	/**
	* @brief 右の入力を取る(Optionでしか使わない)
	*/
	int GetSelectRight() const;

	/**
	* @brief 左の入力を取る(Optionでしか使わない)
	*/
	int GetSelectLeft() const;

	/**
	* @brief 入力デバイスを取得する
	*/
	const InputPtr& GetInputDevice();

private:
	/// テクスチャマネージャ
	TextureManager* m_2dTex;

	/// 入力デバイス
	InputPtr m_dev;

	/// カーソル座標
	Vector2 m_cursorPos;

	/// カーソル番号
	int m_cursorNumber;

	/// 前フレームの番号
	int m_oldCursorNumber;

	/// カーソルを動かす量
	float m_cursorAdd;

	/// 選択項目の最大数
	int m_selectMax;

	/// 選択フラグ
	bool m_selectFlag[2];
};

/// カーソルの
using SelectCursorPtr = unique_ptr<SelectCursor>;

#endif