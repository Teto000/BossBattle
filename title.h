//===================================================
//
// タイトルヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _TITLE_H_
#define _TITLE_H_	

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>

//--------------------------------
// 前方宣言
//--------------------------------
class CBg;			//背景
class CTitleCamera;	//カメラ
class CStage;		//ステージ
class CMeshField;	//メッシュフィールド
class CSky;			//空

//================================
// タイトルクラスの定義
//================================
class CTitle
{
public:
	CTitle();	//コンストラクタ
	~CTitle();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();

	//------------------
	// ゲッター
	//------------------
	static CTitleCamera* GetCamera() { return m_pCamera; }		//カメラの取得

private:
	//------------------
	// メンバ変数
	//------------------
	CBg* m_pBg;	//背景
	CStage* m_pStage;	//ステージ
	CSky*	m_pSky;		//空

	//------------------
	// 静的メンバ変数
	//------------------
	static CTitleCamera* m_pCamera;
	static CMeshField*	m_pMeshField;	//メッシュフィールド
};

#endif