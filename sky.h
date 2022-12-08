//===================================================
//
// スカイヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _SKY_H_
#define _SKY_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "objectX.h"

//================================
// スカイヘッダーの定義
//================================
class CSky : public CObjectX
{
public:
	CSky();				//コンストラクタ
	~CSky() override;	//デストラクタ

	//----------------
	// メンバ関数
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition()override { return m_pos; }	//位置の取得
	float GetWidth() override { return 0; };			//幅の取得
	float GetHeight() override { return 0; };			//高さの取得

	//----------------
	// 静的メンバ関数
	//----------------
	static CSky* Create();

private:
	//----------------
	// メンバ変数
	//----------------
	D3DXVECTOR3 m_pos;		//位置
};

#endif#pragma once
