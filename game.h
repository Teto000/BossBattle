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
class CSky;			//スカイ
class CStage;		//ステージ
class CItem;		//アイテム
class CTime;		//タイム
class CMessage;		//メッセージ
class CBg;			//背景

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
	// セッター
	//------------------
	static void SetEnemyState() { m_bDeathEnemny = true; }

	//------------------
	// ゲッター
	//------------------
	static bool GetFinish()			   { return m_bFinish; }		//終了フラグを取得
	static bool GetStart()			   { return m_bStart; }			//開始フラグを取得
	static CCamera*	GetCamera()		   { return m_pCamera; }		//カメラの取得
	static CPlayer*	GetPlayer()		   { return m_pPlayer; }		//プレイヤーの取得
	static CEnemy*	GetEnemy()		   { return m_pEnemy; }			//エネミーの取得
	static CMeshField*	GetMeshField() { return m_pMeshField; }		//メッシュフィールドの取得

private:
	//------------------
	// プライベート関数
	//------------------
	void Finish();		//ゲーム終了処理
	void SetMessage();	//メッセージの表示
	void SetItem();		//アイテムの生成

private:
	//-------------------
	// メンバ変数
	//-------------------
	int m_nCntFinish;		//終了時間カウント
	int m_nCntItem;			//アイテム生成時間カウント
	int m_nCntMessage;		//メッセージを表示するまでの時間
	int m_nNumMessage;		//表示するメッセージ番号
	CItem*		m_pItem;	//アイテム
	CMessage*	m_pMessage;	//メッセージ
	CBg*		m_pBg;		//背景

	//-------------------
	// 静的メンバ変数
	//-------------------
	static bool m_bDeathEnemny;	//敵が死んでいるか
	static bool m_bFinish;		//終了フラグ
	static bool m_bStart;		//開始フラグ

	static CCamera*		m_pCamera;		//カメラ
	static CPlayer*		m_pPlayer;		//プレイヤー
	static CEnemy*		m_pEnemy;		//エネミー
	static CMeshField*	m_pMeshField;	//メッシュフィールド
	static CPolygon*	m_pPolygon;		//2Dポリゴン
	static CSky*		m_pSky;			//スカイ
	static CStage*		m_pStage;		//ステージ
	static CTime*		m_pTime;		//タイマー
};

#endif