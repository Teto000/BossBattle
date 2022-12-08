//===================================================
//
// ゲームヘッダー
// Author : Sato Teruto
//
//====================================================
#ifndef _GAME_H_
#define _GAME_H_	

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "application.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CCamera;		//カメラ
class CPlayer;		//プレイヤー
class CEnemy;		//エネミー
class CMeshField;	//メッシュフィールド
class CPolygon;		//2Dポリゴン

//================================
// ゲームクラスの定義
//================================
class CGame
{
public:
	CGame();	//コンストラクタ
	~CGame();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();

	//------------------
	// ゲッター
	//------------------
	static CCamera*	GetCamera() { return m_pCamera; };	//カメラの取得
	static CPlayer*	GetPlayer() { return m_pPlayer; };	//プレイヤーの取得
	static CEnemy*	GetEnemy() { return m_pEnemy; };	//エネミーの取得
	static CMeshField*	GetMeshField()
			{ return m_pMeshField; };	//メッシュフィールドの取得

private:
	//-------------------
	// 静的メンバ変数
	//-------------------
	static CCamera*		m_pCamera;		//カメラ
	static CPlayer*		m_pPlayer;		//プレイヤー
	static CEnemy*		m_pEnemy;		//エネミー
	static CMeshField*	m_pMeshField;	//メッシュフィールド
	static CPolygon*	m_pPolygon;		//2Dポリゴン
};

#endif