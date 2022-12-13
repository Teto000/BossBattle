//===================================================
//
// プレイヤーの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <stdio.h>
#include <assert.h>
#include "player.h"
#include "object.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "camera.h"
#include "debug_proc.h"
#include "model.h"
#include "game.h"
#include "line.h"
#include "enemy.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
const float CPlayer::fPlayerSpeed = 7.0f;

//========================
// コンストラクタ
//========================
CPlayer::CPlayer() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最大値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最小値
	m_worldMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最大値
	m_worldMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最小値
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//大きさ
	m_type = MOTION_TYPE_IDOL;					//現在のモーション

	//モデル
	for (int i = 0; i < MAX_PARTS; i++)
	{
		m_pModel[i] = nullptr;
	}

	//線
	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = nullptr;
	}

	/* ↓ モーション情報 ↓ */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;
	m_bLoop = false;

	//キーセット情報の初期化
	for (int i = 0; i < MOTION_TYPE_MAX; i++)
	{
		for (int j = 0; j < MAX_PARTS; j++)
		{
			m_aKeySet[i].aKey[j].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
			m_aKeySet[i].aKey[j].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		}

		m_aKeySet[i].nFrame = 0;	//フレーム数
	}
}

//========================
// デストラクタ
//========================
CPlayer::~CPlayer()
{

}

//========================
// 初期化
//========================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_nCntMotion = 1;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//回転マトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);

	//--------------------
	// モデルの生成
	//--------------------
	SetModel();

	//-----------------------
	// モデルの大きさを設定
	//-----------------------
	m_vtxMin = D3DXVECTOR3(-70.0f, 0.0f, -120.0f);
	m_vtxMax = D3DXVECTOR3(70.0f, 100.0f, 30.0f);

	m_size.x = m_vtxMax.x - m_vtxMin.x;
	m_size.y = m_vtxMax.y - m_vtxMin.y;
	m_size.z = m_vtxMax.z - m_vtxMin.z;

	//-----------------------
	// 線の表示
	//-----------------------
	SetLine();

	//-----------------------
	// モーションの読み込み
	//-----------------------
	GetFileMotion();

	return S_OK;
}

//========================
// 終了
//========================
void CPlayer::Uninit()
{
	//-------------------------
	// モデルの終了
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Uninit();
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}

	CObject::Release();
}

//========================
// 更新
//========================
void CPlayer::Update()
{
	//待機モーションにする
	m_type = MOTION_TYPE_IDOL;

	//-------------------------
	// モデルの更新
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Update();
		}
	}

	//-------------------------
	// 移動
	//-------------------------
	// ジョイパッドでの操作
	CInputJoypad* joypad = CApplication::GetJoypad();

	if (!CGame::GetFinish())
	{//終了フラグが立っていないなら
		if (!joypad->IsJoyPadUse(0))
		{//ジョイパッドが使われていないなら
			MoveKeyboard(DIK_W, DIK_S, DIK_A, DIK_D);	//キーボード
		}
		else
		{
			MoveJoypad();	//ジョイパッド
		}
	}

	//タイヤの回転
	m_pModel[0]->SetRotX(m_rotWheel);

	//-------------------------
	// モーション
	//-------------------------
	//キーセット情報の設定
	//SetKeySet(m_type);

	//モーションの設定
	SetMotion(m_nNumKey, m_bLoop);

	//-------------------------
	// 線の更新
	//-------------------------
	UpdateLine();
}

//========================
// 描画
//========================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//---------------------------------------
	// クォータニオンを使った回転の反映
	//---------------------------------------
	/*//クォータニオン作成
	//引数：クォータニオン、回転軸、回転角
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

	//クォータニオンから回転マトリックスを作成
	//引数：回転マトリックス、クォータニオン
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	//回転マトリックス(保存用)に反映
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);

	//ワールドマトリックスに反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);*/

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------
	// モデルの描画
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (!m_pModel[i])
		{//モデルがnullなら
			return;
		}

		m_pModel[i]->Draw(&m_mtxWorld);
	}
}

//========================
// 生成
//========================
CPlayer* CPlayer::Create()
{
	CPlayer *pPlayer = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pPlayer = new CPlayer;	//生成

	if (pPlayer != nullptr)
	{//NULLチェック
		//初期化
		pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pPlayer->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer;
}

//========================
// モデルの生成
//========================
void CPlayer::SetModel()
{
	//モデル0：タイヤ
	m_pModel[0] = CModel::Create("data\\MODEL\\wheel.x", nullptr,
		D3DXVECTOR3(0.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル1：体
	m_pModel[1] = CModel::Create("data\\MODEL\\body.x", nullptr,
		D3DXVECTOR3(0.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル2：頭
	m_pModel[2] = CModel::Create("data\\MODEL\\head.x", m_pModel[1],
		D3DXVECTOR3(0.0f, 105.0f, 12.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル3：右腕
	m_pModel[3] = CModel::Create("data\\MODEL\\armR.x", m_pModel[1],
		D3DXVECTOR3(-25.0f, 85.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル4：右手
	m_pModel[4] = CModel::Create("data\\MODEL\\handR.x", m_pModel[3],
		D3DXVECTOR3(-4.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル5：左腕
	m_pModel[5] = CModel::Create("data\\MODEL\\armL.x", m_pModel[1],
		D3DXVECTOR3(25.0f, 85.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル6：左手
	m_pModel[6] = CModel::Create("data\\MODEL\\handL.x", m_pModel[5],
		D3DXVECTOR3(4.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=====================================
// キーセット情報の設定
// 引数：モーションの種類
//=====================================
void CPlayer::SetKeySet(int nMotionType)
{
	switch (nMotionType)
	{
	//===================================
	// 待機モーション
	//===================================
	case MOTION_TYPE_IDOL:
		m_nNumKey = 3;	//キー数

		//----------------------
		// キー1
		//----------------------
		m_aKeySet[0].nFrame = 80;	//フレーム数
		m_aKeySet[0].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//タイヤ
		m_aKeySet[0].aKey[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//体
		m_aKeySet[0].aKey[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//頭
		m_aKeySet[0].aKey[3].rot = D3DXVECTOR3(-0.75f, 0.3f, -0.5f);	//右腕
		m_aKeySet[0].aKey[4].rot = D3DXVECTOR3(0.5f, 0.5f, 0.4f);		//右手
		m_aKeySet[0].aKey[5].rot = D3DXVECTOR3(0.7f, -0.5f, -0.06f);	//左腕
		m_aKeySet[0].aKey[6].rot = D3DXVECTOR3(1.5f, -0.2f, -1.1f);		//左手

		//----------------------
		// キー2
		//----------------------
		m_aKeySet[1].nFrame = 80;	//フレーム数
		m_aKeySet[1].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[1].rot = D3DXVECTOR3(-0.1f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[3].rot = D3DXVECTOR3(-0.6f, 0.3f, -0.5f);
		m_aKeySet[1].aKey[4].rot = D3DXVECTOR3(0.8f, 0.5f, 0.4f);
		m_aKeySet[1].aKey[5].rot = D3DXVECTOR3(0.9f, -0.3f, -0.06f);
		m_aKeySet[1].aKey[6].rot = D3DXVECTOR3(0.8f, -0.1f, -0.9f);

		//----------------------
		// キー3
		//----------------------
		m_aKeySet[2].nFrame = 80;	//フレーム数
		m_aKeySet[2].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[3].rot = D3DXVECTOR3(-0.75f, 0.3f, -0.5f);
		m_aKeySet[2].aKey[4].rot = D3DXVECTOR3(0.5f, 0.5f, 0.4f);
		m_aKeySet[2].aKey[5].rot = D3DXVECTOR3(0.7f, -0.5f, -0.06f);
		m_aKeySet[2].aKey[6].rot = D3DXVECTOR3(1.5f, -0.2f, -1.1f);
		break;

	//===================================
	// 移動モーション
	//===================================
	case MOTION_TYPE_MOVE:
		m_nNumKey = 3;	//キー数

		//----------------------
		// キー1
		//----------------------
		m_aKeySet[0].nFrame = 80;	//フレーム数
		m_aKeySet[0].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 //タイヤ
		m_aKeySet[0].aKey[1].rot = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);	 //体
		m_aKeySet[0].aKey[2].rot = D3DXVECTOR3(0.9f, 0.0f, 0.0f);	 //頭
		m_aKeySet[0].aKey[3].rot = D3DXVECTOR3(0.0f, 0.0f, -0.5f);	 //右腕
		m_aKeySet[0].aKey[4].rot = D3DXVECTOR3(0.75f, 0.5f, 0.0f);	 //右手
		m_aKeySet[0].aKey[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.5f);	 //左腕
		m_aKeySet[0].aKey[6].rot = D3DXVECTOR3(0.4f, 0.0f, 0.0f);	 //左手

		//----------------------
		// キー2
		//----------------------
		m_aKeySet[1].nFrame = 80;	//フレーム数
		m_aKeySet[1].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[1].rot = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[2].rot = D3DXVECTOR3(0.9f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[3].rot = D3DXVECTOR3(0.0f, 0.0f, -0.8f);
		m_aKeySet[1].aKey[4].rot = D3DXVECTOR3(0.75f, 0.5f, 0.0f);
		m_aKeySet[1].aKey[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.8f);
		m_aKeySet[1].aKey[6].rot = D3DXVECTOR3(0.4f, 0.0f, 0.0f);

		//----------------------
		// キー2
		//----------------------
		m_aKeySet[2].nFrame = 80;	//フレーム数
		m_aKeySet[2].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[1].rot = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[2].rot = D3DXVECTOR3(0.9f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[3].rot = D3DXVECTOR3(0.0f, 0.0f, -0.5f);
		m_aKeySet[2].aKey[4].rot = D3DXVECTOR3(0.75f, 0.5f, 0.0f);
		m_aKeySet[2].aKey[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.5f);
		m_aKeySet[2].aKey[6].rot = D3DXVECTOR3(0.4f, 0.0f, 0.0f);
		break;
	}

	//モーションの設定
	SetMotion(m_nNumKey, true);
}

//=====================================
// ファイルからモーション情報を取得
//=====================================
void CPlayer::GetFileMotion()
{
	const int nMaxText = 256;	//文字の最大数

	FILE *pFile;				//ファイルポインタを宣言する
	char cText[nMaxText];		//1行分の文字読み取り用変数
	char cTextHead[nMaxText];	//頭文字を取るようの変数
	int nNumKey = 0;			//読み込み中のキー数
	int nNumParts = 0;			//読み込み中のパーツ数

	//-------------------------
	// ファイルの読み込み
	//-------------------------
	//ファイルを開く
	pFile = fopen("data/MOTION/player.txt", "r");

	if (pFile == nullptr)
	{//開けなかったら
		assert(false);
	}

	//文字の読み取り処理
	while (fgets(cText, nMaxText, pFile) != nullptr)
	{
		//文字列の分析
		sscanf(cText, "%s", &cTextHead);

		//===================================
		// モーション情報
		//===================================
		if (strcmp(&cTextHead[0], "MOTIONSET") == 0)
		{//頭文字がMOTIONSETなら
			//文字の読み取り処理
			while (fgets(cText, nMaxText, pFile) != nullptr)
			{
				//文字列の分析
				sscanf(cText, "%s", &cTextHead);

				//------------------------
				// ループするかどうか
				//------------------------
				if (strcmp(&cTextHead[0], "LOOP") == 0)
				{//頭文字がLOOPなら
					int nLoop = 0;

					//文字列からループの値を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &nLoop);

					if (nLoop == 0)
					{//読み取った値が0なら
						m_bLoop = false;
					}
					else if (nLoop == 1)
					{//読み取った値が1なら
						m_bLoop = true;
					}
				}
				//------------------------
				// キーの最大数
				//------------------------
				else if (strcmp(&cTextHead[0], "NUM_KEY") == 0)
				{//頭文字がNUM_KEYなら
					//文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_nNumKey);
				}
				//===================================
				// キーセット情報
				//===================================
				else if (strcmp(&cTextHead[0], "KEYSET") == 0)
				{//頭文字がKEYSETなら
					//文字の読み取り処理
					while (fgets(cText, nMaxText, pFile) != nullptr)
					{
						//文字列の分析
						sscanf(cText, "%s", &cTextHead);

						//------------------------
						// フレーム数
						//------------------------
						if (strcmp(&cTextHead[0], "FRAME") == 0)
						{//頭文字がFRAMEなら
							//文字列からキーの最大数を読み取る
							sscanf(cText, "%s = %d", &cTextHead, &m_aKeySet[nNumKey].nFrame);
						}
						//===================================
						// キー情報
						//===================================
						else if (strcmp(&cTextHead[0], "KEY") == 0)
						{//頭文字がKEYなら
							//文字の読み取り処理
							while (fgets(cText, nMaxText, pFile) != nullptr)
							{
								//文字列の分析
								sscanf(cText, "%s", &cTextHead);

								//------------------------
								// 位置
								//------------------------
								if (strcmp(&cTextHead[0], "POS") == 0)
								{//頭文字がPOSなら
									//文字列から位置を読み取る
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aKeySet[nNumKey].aKey[nNumParts].pos.x,
										&m_aKeySet[nNumKey].aKey[nNumParts].pos.y,
										&m_aKeySet[nNumKey].aKey[nNumParts].pos.z);
								}
								//------------------------
								// 向き
								//------------------------
								else if (strcmp(&cTextHead[0], "ROT") == 0)
								{//頭文字がROTなら
									//文字列から向きを読み取る
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aKeySet[nNumKey].aKey[nNumParts].rot.x,
										&m_aKeySet[nNumKey].aKey[nNumParts].rot.y,
										&m_aKeySet[nNumKey].aKey[nNumParts].rot.z);
								}
								else if (strcmp(&cTextHead[0], "END_KEY") == 0)
								{//キーの読み取りが終了したら
									if (nNumParts + 1 < MAX_PARTS)
									{//パーツ数を超えないなら
										//パーツ番号の加算
										nNumParts++;
									}
									else
									{//パーツ数分読み込んだら
										//パーツ番号をリセット
										nNumParts = 0;
									}
									break;
								}
							}
						}
						else if (strcmp(&cTextHead[0], "END_KEYSET") == 0)
						{//キーセットの読み取りが終了したら
							if (nNumKey < m_nNumKey)
							{//キー数が最大じゃないなら
								//キー番号の加算
								nNumKey++;
							}
							break;
						}
					}
				}
				else if (strcmp(&cTextHead[0], "END_MOTIONSET") == 0)
				{//モーションの読み取りが終了したら
					break;
				}
			}
		}

		//---------------------------
		// 保存中の文字列の初期化
		//---------------------------
		ZeroMemory(&cText, sizeof(cText));
		ZeroMemory(&cTextHead, sizeof(cTextHead));
	}

	//ファイルを閉じる
	fclose(pFile);
}

//=====================================
// モーションの設定
// 引数：キー数、ループ状態
//=====================================
void CPlayer::SetMotion(int nNumKey, bool bLoop)
{
	if (m_nCurrentKey + 1 >= nNumKey)
	{//キーが最大数に達したら
		if (bLoop)
		{//ループするなら
			m_nCurrentKey = 0;	//キー番号の初期化
		}
		else
		{
			return;
		}
	}

	for (int i = 1; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		//-----------------------------------------
		// NULLチェック
		//-----------------------------------------
		if (!m_pModel[i])
		{//モデルパーツがnullなら
			return;
		}

		//キー情報を持った変数
		KEY key = m_aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = m_aKeySet[m_nCurrentKey + 1].aKey[i];

		//-----------------------------------------
		// 現在値を取得
		//-----------------------------------------
		//位置を取得
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//向きを取得
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-----------------------------------------
		// 差分の計算
		// (終了値 - 開始値)
		//-----------------------------------------
		//位置
		float fDifPosX = keyNext.pos.x - key.pos.x;
		float fDifPosY = keyNext.pos.y - key.pos.y;
		float fDifPosZ = keyNext.pos.z - key.pos.z;

		//向き
		float fDifRotX = keyNext.rot.x - key.rot.x;
		float fDifRotY = keyNext.rot.y - key.rot.y;
		float fDifRotZ = keyNext.rot.z - key.rot.z;

		//-----------------------------------------
		// 相対値の計算
		// (モーションカウンター / フレーム数)
		//-----------------------------------------
		float fNumRelative = m_nCntMotion / (float)m_aKeySet[m_nCurrentKey].nFrame;

		//-----------------------------------------
		// 現在値の計算
		// (開始値 + (差分 * 相対値))
		//-----------------------------------------
		//位置
		fPosX += key.pos.x + (fDifPosX * fNumRelative);
		fPosY += key.pos.y + (fDifPosY * fNumRelative);
		fPosZ += key.pos.z + (fDifPosZ * fNumRelative);

		//向き
		fRotX = key.rot.x + (fDifRotX * fNumRelative);
		fRotY = key.rot.y + (fDifRotY * fNumRelative);
		fRotZ = key.rot.z + (fDifRotZ * fNumRelative);

		//-----------------------------------------
		// モデル情報の設定
		//-----------------------------------------
		//位置の設定
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//向きの設定
		m_pModel[i]->SetRot(D3DXVECTOR3(fRotX, fRotY, fRotZ));
	}

	//モーションカウンターを進める
	m_nCntMotion++;

	//-------------------------
	// 初期化
	//-------------------------
	if (m_nCntMotion >= m_aKeySet[m_nCurrentKey].nFrame)
	{//モーションカウンターが再生フレームに達したら
		m_nCurrentKey++;	//キー番号を加算
		m_nCntMotion = 0;	//モーションカウンターを初期化
	}
}

//=====================================
// 移動
// 引数：上キー,下キー,左キー,右キー
//=====================================
void CPlayer::MoveKeyboard(int nUpKey, int nDownKey, int nLeftKey, int nRightKey)
{
	//カメラの情報取得
	D3DXVECTOR3 cameraRot = CGame::GetCamera()->GetRot();

	//-------------------------------
	// プレイヤーの操作
	//-------------------------------
	if (CInputKeyboard::Press(nLeftKey))
	{//Aキーが押された
		if (CInputKeyboard::Press(nUpKey))
		{//Wキーが押された
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;	//左奥移動
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;	//向きの切り替え
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//Sキーが押された
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;	//左前移動
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;	//左移動
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nRightKey))
	{//Dキーが押された
		if (CInputKeyboard::Press(nUpKey))
		{//Wキーが押された
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;	//右奥移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//Sキーが押された
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;	//右前移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;	//右移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nUpKey))
	{//Wキーが押された
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;	//奥移動
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	else if (CInputKeyboard::Press(nDownKey))
	{//Sキーが押された
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;	//前移動
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 0.0f;
	}

	//向きを目的の角度に合わせる
	SetRot();

	//タイヤの回転量の加算
	m_rotWheel += D3DXToRadian(-nWheelRotValue);

	if (!CInputKeyboard::Press(nUpKey) && !CInputKeyboard::Press(nDownKey)
		&& !CInputKeyboard::Press(nRightKey) && !CInputKeyboard::Press(nLeftKey))
	{//移動キーが押されていないなら
		//タイヤの回転量を0にする
		m_rotWheel = 0;
	}
	else
	{//どれかが押されているなら
		//移動モーションにする
		m_type = MOTION_TYPE_MOVE;
	}
}

//===========================
// ジョイパッドを使った移動
//===========================
void CPlayer::MoveJoypad()
{
	// ジョイパッドでの操作
	CInputJoypad* joypad = CApplication::GetJoypad();
	D3DXVECTOR3 stick = joypad->Stick(CInputJoypad::JOYKEY_LEFT_STICK, 0);

	if (joypad->IsJoyPadUse(0) == false)
	{//ジョイパッドが使われていないなら
		return;
	}

	//スティックを動かす値の設定
	float fMoveValue = 0.5f;

	//カメラの情報取得
	D3DXVECTOR3 cameraRot = CGame::GetCamera()->GetRot();

	//-----------------------------------
	// 右移動
	//-----------------------------------
	if (stick.x >= fMoveValue)
	{
		// スティックを倒した方向へ移動する
		if (stick.y <= -fMoveValue)
		{//右奥移動
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;	//向きの切り替え
		}
		else if (stick.y >= fMoveValue)
		{//右前移動
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * fMoveValue;
		}
	}
	//-----------------------------------
	// 左移動
	//-----------------------------------
	else if (stick.x <= -fMoveValue)
	{
		if (stick.y <= -fMoveValue)
		{//左奥移動
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;
		}
		else if (stick.y >= fMoveValue)
		{//左前移動
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * fMoveValue;
		}
	}
	//-----------------------------------
	// 奥移動
	//-----------------------------------
	else if (stick.y <= -fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	//-----------------------------------
	// 前移動
	//-----------------------------------
	else if (stick.y >= fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 0.0f;
	}

	//向きを目的の角度に合わせる
	SetRot();

	//タイヤの回転量の加算
	m_rotWheel += D3DXToRadian(-nWheelRotValue);

	if (stick.x < fMoveValue && stick.x > -fMoveValue 
		&& stick.y < fMoveValue && stick.y > -fMoveValue)
	{//スティックが動かされていないなら
		//タイヤの回転量を0にする
		m_rotWheel = 0;
	}
}

//===========================
// 角度の設定
//===========================
void CPlayer::SetRot()
{
	//-------------------------------
	// 目的の角度の正規化
	//-------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//回転したい角度が180以上なら
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//回転したい角度が-180以下なら
		m_rotDest.y += D3DX_PI * 2;
	}

	//-------------------------------
	// 目的の角度まで回転する
	//-------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.05f;	//減衰処理

	//-------------------------------
	// 角度の正規化
	//-------------------------------
	if (m_rot.y > D3DX_PI)
	{//角度が180以上なら
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//角度が-180以下なら
		m_rot.y += D3DX_PI * 2;
	}
}

//========================
// 線の設置
//========================
void CPlayer::SetLine()
{
	//線の色
	D3DXCOLOR lineCol(1.0f, 0.0f, 0.0f, 1.0f);

	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	//-----------------------------------
	// 下辺
	//-----------------------------------
	D3DXVECTOR3 start = D3DXVECTOR3(min.x, min.y, min.z);
	D3DXVECTOR3 end = D3DXVECTOR3(max.x, min.y, min.z);
	m_pLine[0] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLine[1] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[2] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[3] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// 上辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[4] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[5] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[6] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[7] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// 縦辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLine[8] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[9] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[10] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[11] = CLine::Create(m_pos, start, end, lineCol);
}

//========================
// 線の情報の更新
//========================
void CPlayer::UpdateLine()
{
	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//最大最小を求めなおす

	//剣を含まないでけいさんする

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	//-----------------------------------
	// 下辺
	//-----------------------------------
	D3DXVECTOR3 start = D3DXVECTOR3(min.x, min.y, min.z);
	D3DXVECTOR3 end = D3DXVECTOR3(max.x, min.y, min.z);
	m_pLine[0]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLine[1]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[2]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[3]->SetLine(start, end);

	//-----------------------------------
	// 上辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[4]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[5]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[6]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[7]->SetLine(start, end);

	//-----------------------------------
	// 縦辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLine[8]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[9]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[10]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[11]->SetLine(start, end);
}

//===========================
// プレイヤーの当たり判定
//===========================
bool CPlayer::GetCollisionPlayer()
{
	//-----------------------------
	// プレイヤーの端の設定
	//-----------------------------
	float fLeft = m_pos.x - (m_size.x / 2);		//プレイヤーの左端
	float fRight = m_pos.x + (m_size.x / 2);	//プレイヤーの右端
	float fTop = m_pos.y + (m_size.y / 2);		//プレイヤーの上端
	float fBottom = m_pos.y - (m_size.y / 2);	//プレイヤーの下端
	float fFront = m_pos.z - (m_size.z / 2);	//プレイヤーの前端
	float fBack = m_pos.z + (m_size.z / 2);		//プレイヤーの後端

	//-----------------------------
	// 敵の情報を取得
	//-----------------------------
	D3DXVECTOR3 enemyPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	D3DXVECTOR3 enemySize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (CGame::GetEnemy() != nullptr)
	{
		enemyPos = CGame::GetEnemy()->GetPosition();	//位置
		enemySize = CGame::GetEnemy()->GetSize();		//大きさ
	}

	//-----------------------------
	// 当たり判定
	//-----------------------------
	if (enemyPos.x + enemySize.x >= fLeft
		&& enemyPos.x - enemySize.x <= fRight
		&& enemyPos.y + enemySize.y >= fTop
		&& enemyPos.y - enemySize.y <= fBottom
		&& enemyPos.z + enemySize.z >= fFront
		&& enemyPos.z - enemySize.z <= fBack)
	{//敵とプレイヤーが当たった
		return true;
	}

	return false;
}