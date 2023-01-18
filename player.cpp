//=============================================================
//
// プレイヤーの処理
// Author:Teruto Sato
//
//=============================================================

//-------------------------------
// インクルード
//-------------------------------
#include <stdio.h>
#include <stdlib.h>
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
#include "hp.h"
#include "combo.h"
#include "damage.h"
#include "style_shift.h"
#include "utility.h"

//-------------------------------
// 静的メンバ変数宣言
//-------------------------------
const float CPlayer::fDefaultAttack = 20.0f;	//基本の攻撃力
const float CPlayer::fDefaultSpeed = 7.0f;		//基本の速度

//=============================
// コンストラクタ
//=============================
CPlayer::CPlayer() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最大値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最小値
	m_worldMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最大値
	m_worldMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最小値
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//大きさ
	m_nNumCombo = 0;							//コンボ数
	m_status.nComboValue = 0;					//コンボの加算値
	m_nCntModeTime = 0;							//モード終了までの時間を数える
	fSizeWidth = 0.0f;							//サイズ(幅)
	fSizeDepth = 0.0f;							//サイズ(奥行き)
	m_bHitAttack = false;						//ダメージを与えたか
	bChangeAttack = false;						//攻撃が切り替わった状態
	m_bStyle = false;							//スタイルを表示したか
	m_type = MOTION_IDOL;						//現在のモーション
	m_battleStyle = BATTLESTYLE_NONE;			//バトルモード
	m_pHP = nullptr;							//HP
	m_pCombo = nullptr;							//コンボ
	m_pDamage = nullptr;						//ダメージ

	//ステータス
	m_status.nAttack = 0;			//攻撃力
	m_status.nAttackTime = 0;		//攻撃時間
	m_status.fSpeed = 0.0f;			//速度
	m_status.fLife = 0.0f;			//体力
	m_status.fRemLife = 0.0f;		//残り体力(%)
	m_status.fMaxLife = 0.0f;		//最大体力
	m_status.bNextAttack = false;	//次の攻撃に繋げるかどうか

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

	//スタイル
	for (int i = 0; i < BATTLESTYLE_MAX; i++)
	{
		m_pStyleShift[i] = nullptr;
	}

	/* ↓ モーション情報 ↓ */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;

	//-----------------------------------
	// キーセット情報の初期化
	//-----------------------------------
	for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
	{
		for (int i = 0; i < MOTION_MAX; i++)
		{
			for (int j = 0; j < MAX_PARTS; j++)
			{
				m_aMotionSet[nCnt].aKeySet[i].aKey[j].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
				m_aMotionSet[nCnt].aKeySet[i].aKey[j].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
			}

			m_aMotionSet[nCnt].bLoop = false;			//ループするかどうか
			m_aMotionSet[nCnt].nStartCollision = 0;		//当たり判定の開始時間
			m_aMotionSet[nCnt].nNextAtkTime;			//次の攻撃の入力開始時間
			m_aMotionSet[nCnt].aKeySet[i].nFrame = 0;	//フレーム数
		}

		m_aMotionSet[nCnt].nNumKey = 0;
		m_aMotionSet[nCnt].bLoop = false;
	}
}

//=============================
// デストラクタ
//=============================
CPlayer::~CPlayer()
{

}

//=============================
// 初期化
//=============================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	//---------------------------
	// 初期値の設定
	//---------------------------
	m_pos = pos;						//位置
	fSizeWidth = 30.0f;					//モデルの幅
	fSizeDepth = 30.0f;					//モデルの奥行き
	m_status.fLife = 300.0f;			//体力
	m_status.fRemLife = 100.0f;			//残り体力(%)
	m_status.fMaxLife = m_status.fLife;	//最大体力
	m_status.nAttack = 20;				//攻撃力
	m_nNumCombo = 0;					//コンボ数
	m_status.nComboValue = 1;			//コンボの加算値
	m_status.fSpeed = 7.0f;				//速度

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//回転マトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);

	//---------------------------
	// HPの生成
	//---------------------------
	{
		D3DXVECTOR3 hpPos(300.0f, 650.0f, 0.0f);
		m_pHP = CHP::Create(hpPos, 500.0f, 60.0f, CHP::GAUGETYPE_HP_PLAYER);
		m_pHP->SetLife(m_status.fLife, m_status.fRemLife);	//HPの設定
	}

	//---------------------------
	// モデルの生成
	//---------------------------
	SetModel();

	//------------------------------
	// モデルの大きさを設定
	//------------------------------
	m_vtxMin = D3DXVECTOR3(-fSizeWidth, 10.0f, -fSizeDepth);
	m_vtxMax = D3DXVECTOR3(fSizeWidth, 150.0f, fSizeDepth);

	m_size.x = m_vtxMax.x - m_vtxMin.x;
	m_size.y = m_vtxMax.y - m_vtxMin.y;
	m_size.z = m_vtxMax.z - m_vtxMin.z;

	//------------------------------
	// 線の表示
	//------------------------------
	//SetLine();

	//------------------------------
	// モーションの読み込み
	//------------------------------
	GetFileMotion();

	//------------------------------
	// コンボ数表示
	//------------------------------
	m_pCombo = CCombo::Create(D3DXVECTOR3(1100.0f, 360.0f, 0.0f), m_nNumCombo);

	//------------------------------
	// スタイルの表示
	//------------------------------
	{
		D3DXVECTOR3 firstPos(m_pos.x - 100.0f, m_pos.y + 180.0f, m_pos.z);
		m_pStyleShift[0] = CStyleShift::Create(firstPos, CStyleShift::STYLE_TYPE_ATTACK);

		D3DXVECTOR3 secondPos(m_pos.x, m_pos.y + 200.0f, m_pos.z);
		m_pStyleShift[1] = CStyleShift::Create(secondPos, CStyleShift::STYLE_TYPE_SPEED);

		D3DXVECTOR3 thirdPos(m_pos.x + 100.0f, m_pos.y + 180.0f, m_pos.z);
		m_pStyleShift[2] = CStyleShift::Create(thirdPos, CStyleShift::STYLE_TYPE_COMBO);
	}

	return S_OK;
}

//=============================
// 終了
//=============================
void CPlayer::Uninit()
{
	//--------------------------------
	// モデルの終了
	//--------------------------------
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

//=============================
// 更新
//=============================
void CPlayer::Update()
{
	//1つ前の位置を保存
	m_posOld = m_pos;

	//--------------------------------
	// モーションのリセット
	//--------------------------------
	if (m_type != MOTION_ATTACK_1
		&& m_type != MOTION_ATTACK_2)
	{//攻撃モーションじゃないなら
		//待機モーションにする
		ChangeMotion(MOTION_IDOL);
	}

	//--------------------------------
	// モデルの更新
	//--------------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Update();
		}
	}

	if (!CGame::GetFinish())
	{//終了フラグが立っていないなら
		if (!m_bStyle)
		{//スタイル表示中じゃないなら
			//--------------------------------
			// 移動
			//--------------------------------
			// ジョイパッドでの操作
			CInputJoypad* joypad = CApplication::GetJoypad();

			if (m_type != MOTION_ATTACK_1
				&& m_type != MOTION_ATTACK_2)
			{//攻撃中じゃないなら
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

			//--------------------------------
			// 攻撃処理
			//--------------------------------
			/*switch (m_type)
			{
			case MOTION_ATTACK_2:
				Attack(MOTION_ATTACK_2, MOTION_ATTACK_3);
				break;

			default:
				Attack(MOTION_ATTACK_1, MOTION_ATTACK_2);
				break;
			}*/
			Attack(MOTION_ATTACK_1, MOTION_ATTACK_2);
		}

		//--------------------------------
		// モードチェンジ
		//--------------------------------
		//ChangeMode();
	}

	//--------------------------------
	// モーションの設定
	//--------------------------------
	SetMotion(m_type, m_aMotionSet[m_type].bLoop, m_aMotionSet[m_type].nNumKey);

	//--------------------------------
	// 線の更新
	//--------------------------------
	//UpdateLine();

	//--------------------------------
	// 敵との当たり判定
	//--------------------------------
	m_pos = CUtility::GetCollisionPos(m_pos, m_posOld, m_size, m_mtxWorld
										, CObject::OBJTYPE_ENEMY);
}

//=============================
// 描画
//=============================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//--------------------------------
	// モデルの描画
	//--------------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (!m_pModel[i])
		{//モデルがnullなら
			return;
		}

		m_pModel[i]->Draw(&m_mtxWorld);
	}
}

//=============================
// 生成
//=============================
CPlayer* CPlayer::Create()
{
	CPlayer *pPlayer = nullptr;

	//-----------------------------------------
	// 生成と初期化
	//-----------------------------------------
	pPlayer = new CPlayer;	//生成

	if (pPlayer != nullptr)
	{//NULLチェック
		//初期化
		pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pPlayer->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer;
}

//=============================
// モデルの生成
//=============================
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

//==========================================
// ファイルからモーション情報を取得
//==========================================
void CPlayer::GetFileMotion()
{
	const int nMaxText = 256;	//文字の最大数

	FILE *pFile;				//ファイルポインタを宣言する
	char cText[nMaxText];		//1行分の文字読み取り用変数
	char cTextHead[nMaxText];	//頭文字を取るようの変数
	int nNumMotion = 0;			//読み込み中のモーション番号
	int nNumKey = 0;			//読み込み中のキー番号
	int nNumParts = 0;			//読み込み中のパーツ番号

	//--------------------------------
	// ファイルの読み込み
	//--------------------------------
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

		//========================================
		// モーション情報
		//========================================
		if (strcmp(&cTextHead[0], "MOTIONSET") == 0)
		{//頭文字がMOTIONSETなら
			//文字の読み取り処理
			while (fgets(cText, nMaxText, pFile) != nullptr)
			{
				//文字列の分析
				sscanf(cText, "%s", &cTextHead);

				//-------------------------------
				// ループするかどうか
				//-------------------------------
				if (strcmp(&cTextHead[0], "LOOP") == 0)
				{//頭文字がLOOPなら
					int nLoop = 0;

					//文字列からループの値を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &nLoop);

					if (nLoop == 0)
					{//読み取った値が0なら
						m_aMotionSet[nNumMotion].bLoop = false;
					}
					else if (nLoop == 1)
					{//読み取った値が1なら
						m_aMotionSet[nNumMotion].bLoop = true;
					}
				}
				//-------------------------------
				// 当たり判定の開始時間
				//-------------------------------
				else if (strcmp(&cTextHead[0], "COLLISION") == 0)
				{//頭文字がNUM_KEYなら
					//文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nStartCollision);
				}
				//-------------------------------
				// 次の攻撃入力開始時間
				//-------------------------------
				else if (strcmp(&cTextHead[0], "NEXT_ATTACK") == 0)
				{//頭文字がNUM_KEYなら
					//文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nNextAtkTime);
				}
				//-------------------------------
				// 攻撃に必要なポイント数
				//-------------------------------
				else if (strcmp(&cTextHead[0], "NUM_POINT") == 0)
				{//頭文字がNUM_KEYなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nNumPoint);
				}
				//-------------------------------
				// クリティカル率
				//-------------------------------
				else if (strcmp(&cTextHead[0], "CRITICAL") == 0)
				{//頭文字がNUM_KEYなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nCritical);
				}
				//-------------------------------
				// ダメージ倍率
				//-------------------------------
				else if (strcmp(&cTextHead[0], "DAMAGE_MAG") == 0)
				{//頭文字がNUM_KEYなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %f", &cTextHead, &m_aMotionSet[nNumMotion].fDamageMag);
				}
				//-------------------------------
				// キーの最大数
				//-------------------------------
				else if (strcmp(&cTextHead[0], "NUM_KEY") == 0)
				{//頭文字がNUM_KEYなら
					//文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nNumKey);
				}
				//========================================
				// キーセット情報
				//========================================
				else if (strcmp(&cTextHead[0], "KEYSET") == 0)
				{//頭文字がKEYSETなら
					//文字の読み取り処理
					while (fgets(cText, nMaxText, pFile) != nullptr)
					{
						//文字列の分析
						sscanf(cText, "%s", &cTextHead);

						//-------------------------------
						// フレーム数
						//-------------------------------
						if (strcmp(&cTextHead[0], "FRAME") == 0)
						{//頭文字がFRAMEなら
							//文字列からキーの最大数を読み取る
							sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].aKeySet[nNumKey].nFrame);
						}
						//========================================
						// キー情報
						//========================================
						else if (strcmp(&cTextHead[0], "KEY") == 0)
						{//頭文字がKEYなら
							//文字の読み取り処理
							while (fgets(cText, nMaxText, pFile) != nullptr)
							{
								//文字列の分析
								sscanf(cText, "%s", &cTextHead);

								//-------------------------------
								// 位置
								//-------------------------------
								if (strcmp(&cTextHead[0], "POS") == 0)
								{//頭文字がPOSなら
									//文字列から位置を読み取る
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.x,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.y,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.z);
								}
								//-------------------------------
								// 向き
								//-------------------------------
								else if (strcmp(&cTextHead[0], "ROT") == 0)
								{//頭文字がROTなら
									//文字列から向きを読み取る
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.x,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.y,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.z);
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
							if (nNumKey < m_aMotionSet[nNumMotion].nNumKey)
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
					//キー番号をリセット
					nNumKey = 0;
					//モーション番号の加算
					nNumMotion++;
					break;
				}
			}
		}

		//----------------------------------
		// 保存中の文字列の初期化
		//----------------------------------
		ZeroMemory(&cText, sizeof(cText));
		ZeroMemory(&cTextHead, sizeof(cTextHead));
	}

	//ファイルを閉じる
	fclose(pFile);
}

//==========================================
// モーションの設定
// 引数：種類、ループ状態、キー数
//==========================================
void CPlayer::SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey)
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
		//-------------------------------------------------------
		// NULLチェック
		//-------------------------------------------------------
		if (!m_pModel[i])
		{//モデルパーツがnullなら
			return;
		}

		//キー情報を持った変数
		KEY key = m_aMotionSet[type].aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = m_aMotionSet[type].aKeySet[m_nCurrentKey + 1].aKey[i];

		//-------------------------------------------------------
		// 現在値を取得
		//-------------------------------------------------------
		//位置を取得
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//向きを取得
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-------------------------------------------------------
		// 差分の計算
		// (終了値 - 開始値)
		//-------------------------------------------------------
		//位置
		float fDifPosX = keyNext.pos.x - key.pos.x;
		float fDifPosY = keyNext.pos.y - key.pos.y;
		float fDifPosZ = keyNext.pos.z - key.pos.z;

		//向き
		float fDifRotX = keyNext.rot.x - key.rot.x;
		float fDifRotY = keyNext.rot.y - key.rot.y;
		float fDifRotZ = keyNext.rot.z - key.rot.z;

		//-------------------------------------------------------
		// 相対値の計算
		// (モーションカウンター / フレーム数)
		//-------------------------------------------------------
		float fNumRelative = m_nCntMotion / (float)m_aMotionSet[type].aKeySet[m_nCurrentKey].nFrame;

		//-------------------------------------------------------
		// 現在値の計算
		// (開始値 + (差分 * 相対値))
		//-------------------------------------------------------
		//位置
		fPosX += key.pos.x + (fDifPosX * fNumRelative);
		fPosY += key.pos.y + (fDifPosY * fNumRelative);
		fPosZ += key.pos.z + (fDifPosZ * fNumRelative);

		//向き
		fRotX = key.rot.x + (fDifRotX * fNumRelative);
		fRotY = key.rot.y + (fDifRotY * fNumRelative);
		fRotZ = key.rot.z + (fDifRotZ * fNumRelative);

		//-------------------------------------------------------
		// モデル情報の設定
		//-------------------------------------------------------
		//位置の設定
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//向きの設定
		m_pModel[i]->SetRot(D3DXVECTOR3(fRotX, fRotY, fRotZ));
	}

	//モーションカウンターを進める
	m_nCntMotion++;

	//--------------------------------
	// 初期化
	//--------------------------------
	if (m_nCntMotion >= m_aMotionSet[type].aKeySet[m_nCurrentKey].nFrame)
	{//モーションカウンターが再生フレームに達したら
		m_nCurrentKey++;	//キー番号を加算
		m_nCntMotion = 0;	//モーションカウンターを初期化
	}
}

//==========================================
// モーションの変更
// 引数：変更したいモーションの列挙
//==========================================
void CPlayer::ChangeMotion(MOTION_TYPE type)
{
	//モーションの変更
	m_type = type;

	//モーション情報の初期化
	if (GetOutAttack(false))
	{//どれか攻撃モーションなら
		m_nCurrentKey = 0;
		m_nCntMotion = 0;
		m_status.nAttackTime = 0;	//攻撃時間のリセット
		m_bHitAttack = false;		//ダメージを与えていない状態にする
	}
}

//==========================================
// 移動
// 引数：上キー,下キー,左キー,右キー
//==========================================
void CPlayer::MoveKeyboard(int nUpKey, int nDownKey, int nLeftKey, int nRightKey)
{
	//カメラの情報取得
	D3DXVECTOR3 cameraRot = CGame::GetCamera()->GetRot();

	//--------------------------------------
	// プレイヤーの操作
	//--------------------------------------
	if (CInputKeyboard::Press(nLeftKey))
	{//Aキーが押された
		if (CInputKeyboard::Press(nUpKey))
		{//Wキーが押された
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * m_status.fSpeed;	//左奥移動
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;	//向きの切り替え
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//Sキーが押された
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * m_status.fSpeed;	//左前移動
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * m_status.fSpeed;	//左移動
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.5f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nRightKey))
	{//Dキーが押された
		if (CInputKeyboard::Press(nUpKey))
		{//Wキーが押された
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * m_status.fSpeed;	//右奥移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//Sキーが押された
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * m_status.fSpeed;	//右前移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.5f) * m_status.fSpeed;	//右移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.5f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nUpKey))
	{//Wキーが押された
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * m_status.fSpeed;	//奥移動
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * m_status.fSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	else if (CInputKeyboard::Press(nDownKey))
	{//Sキーが押された
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * m_status.fSpeed;	//前移動
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * m_status.fSpeed;
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
		ChangeMotion(MOTION_MOVE);
	}
}

//================================
// ジョイパッドを使った移動
//================================
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

	//------------------------------------------
	// 右移動
	//------------------------------------------
	if (stick.x >= fMoveValue)
	{
		// スティックを倒した方向へ移動する
		if (stick.y <= -fMoveValue)
		{//右奥移動
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;	//向きの切り替え
		}
		else if (stick.y >= fMoveValue)
		{//右前移動
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * fMoveValue) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * fMoveValue) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * fMoveValue;
		}
	}
	//------------------------------------------
	// 左移動
	//------------------------------------------
	else if (stick.x <= -fMoveValue)
	{
		if (stick.y <= -fMoveValue)
		{//左奥移動
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;
		}
		else if (stick.y >= fMoveValue)
		{//左前移動
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * fMoveValue) * m_status.fSpeed;
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * fMoveValue) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * fMoveValue;
		}
	}
	//------------------------------------------
	// 奥移動
	//------------------------------------------
	else if (stick.y <= -fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * m_status.fSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * m_status.fSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	//------------------------------------------
	// 前移動
	//------------------------------------------
	else if (stick.y >= fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * m_status.fSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * m_status.fSpeed;
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

//================================
// 攻撃処理
//================================
void CPlayer::Attack(MOTION_TYPE type, MOTION_TYPE next)
{
	//------------------------------------------
	// 攻撃モーションへ移行
	//------------------------------------------
	if (CInputKeyboard::Trigger(DIK_RETURN)
		&& m_type != MOTION_ATTACK_1
		&& m_type != MOTION_ATTACK_2)
	{//ENTERキーが押された & 攻撃モーション中じゃないなら
		//-----------------------------
		// 敵のところまで移動する
		//-----------------------------
		D3DXVECTOR3 enemyPos = CGame::GetEnemy()->GetPosition();
		MoveAccess(enemyPos);

		//攻撃モーションにする
		ChangeMotion(type);
	}

	if (GetOutAttack(false))
	{//攻撃モーション中なら
		//------------------------------------------
		// モーションと攻撃時間を合わせる
		//------------------------------------------
		int nAttackFream = 0;
		int nOutRigor = 0;
		for (int i = 0; i < m_aMotionSet[m_type].nNumKey; i++)
		{//キー数-1回分回す
			//攻撃モーションのフレーム数を合計する
			nAttackFream += m_aMotionSet[m_type].aKeySet[i].nFrame;

			if (i != m_aMotionSet[m_type].nNumKey - 1)
			{//硬直キーじゃないなら
				//フレーム数を加算
				nOutRigor += m_aMotionSet[m_type].aKeySet[i].nFrame;
			}
		}

		//------------------------------------------
		// 攻撃の切り替え
		//------------------------------------------
		if (CInputKeyboard::Trigger(DIK_RETURN)
			&& nOutRigor > m_status.nAttackTime
			&& m_status.nAttackTime >= m_aMotionSet[m_type].nNextAtkTime)
		{//攻撃ボタンを押された & 硬直前のフレーム & 入力開始時間の後なら
			m_status.bNextAttack = true;	//次の攻撃フラグをオン
		}

		if (m_status.bNextAttack && nOutRigor <= m_status.nAttackTime && !bChangeAttack)
		{//攻撃切り替えフラグがオン & 硬直以外のフレーム数を超えた & 攻撃が切り替わっていないなら
			ChangeMotion(next);
			m_status.bNextAttack = false;
			bChangeAttack = true;
		}
		//------------------------------------------
		// フレーム数の加算
		//------------------------------------------
		if (nAttackFream <= m_status.nAttackTime)
		{//攻撃時間が攻撃モーションのフレーム数の合計を超えたら
			//待機モーションにする
			ChangeMotion(MOTION_IDOL);
			bChangeAttack = false;
		}
		else
		{
			//攻撃時間を加算
			m_status.nAttackTime++;
		}

		//------------------------------------------
		// 剣との当たり判定
		//------------------------------------------
		HitSword();
	}
}

//================================
// 剣との当たり判定
//================================
void CPlayer::HitSword()
{
	if (m_pModel[nSwordNumber]->GetCollisionAttack()
		&& !m_bHitAttack
		&& m_status.nAttackTime >= m_aMotionSet[m_type].nStartCollision)
	{//剣と当たっている & 攻撃を当てていない & 当たり判定の有効時間なら
		//技ごとのダメージ量を計算
		float fDamage = m_status.nAttack * m_aMotionSet[m_type].fDamageMag;

		//-----------------------------
		// クリティカルかどうか
		//-----------------------------
		int nRand = rand() % 101;
		bool bCritical = false;
		if (nRand <= m_aMotionSet[m_type].nCritical)
		{//ランダムな値がクリティカル率以内なら
			fDamage *= 1.5f;	//ダメージ1.5倍
			bCritical = true;	//クリティカル状態にする
		}

		//-----------------------------
		// ブレイク状態かどうか
		//-----------------------------
		if (CGame::GetEnemy()->GetState() != CEnemy::ENEMYSTATE_BREAK)
		{//敵がブレイク状態じゃないなら
			//攻撃力分敵の体力を減少
			CGame::GetEnemy()->SubGauge(fDamage, CEnemy::GAUGE_HP);

			//ブレイクゲージの減少
			CGame::GetEnemy()->SubGauge(fDamage, CEnemy::GAUGE_BREAK);
		}
		else
		{//ブレイク状態なら
			if (!bCritical)
			{//クリティカル状態じゃないなら
				fDamage *= 1.5f;	//ダメージ1.5倍
			}

			//クリティカルダメージ分敵の体力を減少
			CGame::GetEnemy()->SubGauge(fDamage, CEnemy::GAUGE_HP);
		}

		//コンボ数の加算
		CGame::GetPlayer()->AddCombo(m_status.nComboValue);

		//攻撃を当てた状態にする
		m_bHitAttack = true;
	}
}

//================================
// 目的の位置まで近づく処理
//================================
bool CPlayer::MoveAccess(D3DXVECTOR3 targetPos)
{
	//自分の位置と目的の位置のベクトルを求める
	D3DXVECTOR3 vec(m_pos - targetPos);

	//ベクトルの正規化
	D3DXVec3Normalize(&vec, &vec);

	//プレイヤーに向かって移動
	m_move = vec * 1.5f;
	m_pos += m_move;

	return false;
}

//================================
// 攻撃状態かどうかを返す
// 引数：trueでand,falseでor
//================================
bool CPlayer::GetOutAttack(bool and)
{
	switch (and)
	{
	case true:
		//----------------------------
		// 攻撃状態全部なら
		//----------------------------
		if (m_type == MOTION_ATTACK_1
			&& m_type == MOTION_ATTACK_2)
		{
			return true;
		}
		break;

	case false:
		//----------------------------
		// 攻撃状態どれかなら
		//----------------------------
		if (m_type == MOTION_ATTACK_1
			|| m_type == MOTION_ATTACK_2)
		{
			return true;
		}
		break;
	}

	return false;
}

//================================
// モードチェンジ
//================================
void CPlayer::ChangeMode()
{
	if (CInputKeyboard::Press(DIK_Z))
	{//Zキーが押されたとき
		if (!m_bStyle)
		{//スタイルが表示されていないなら
			//------------------
			// スタイルを表示
			//------------------
			for (int i = 0; i < BATTLESTYLE_MAX; i++)
			{
				if (m_pStyleShift[i])
				{//nullじゃないなら
					m_pStyleShift[i]->SetStyle(true);
				}
			}
			m_bStyle = true;	//スタイルを表示している状態
		}

		//--------------------------------------
		// スタイルの切り替え
		//--------------------------------------
		if (CInputKeyboard::Trigger(DIK_1))
		{
			m_battleStyle = BATTLESTYLE_ATTACK;
		}
		else if (CInputKeyboard::Trigger(DIK_2))
		{
			m_battleStyle = BATTLESTYLE_SPEED;
		}
		else if (CInputKeyboard::Trigger(DIK_3))
		{
			m_battleStyle = BATTLESTYLE_COMBO;
		}
	}
	else if (CInputKeyboard::Release(DIK_Z))
	{
		if (m_bStyle && m_pStyleShift)
		{//スタイルが表示されている & nullじゃないなら
			//------------------
			// 表示を消す
			//------------------
			for (int i = 0; i < BATTLESTYLE_MAX; i++)
			{
				if (m_pStyleShift[i])
				{//nullじゃないなら
					m_pStyleShift[i]->SetStyle(false);
				}
			}
			m_bStyle = false;	//スタイルを表示していない状態
		}
	}

	//--------------------------------------
	// モード別ステータスのリセット
	//--------------------------------------
	SetAttack(fDefaultAttack);		//攻撃力
	SetSpeed(fDefaultSpeed);		//速度
	SetComboValue(1);				//コンボの加算値

	//--------------------------------------
	// モードごとの処理
	//--------------------------------------
	switch (m_battleStyle)
	{
	case BATTLESTYLE_ATTACK:
		SetAttack(fDefaultAttack * 5.0f);			//攻撃力の強化
		break;
	case BATTLESTYLE_SPEED:
		SetSpeed(fDefaultSpeed * 2.0f);				//速度の強化
		break;
	case BATTLESTYLE_COMBO:
		SetComboValue(m_status.nComboValue * 2);	//コンボ加算の強化
		break;
	default:
		break;
	}

	//--------------------------------------
	// モードのリセット処理
	//--------------------------------------
	if (m_battleStyle != BATTLESTYLE_NONE)
	{//通常モード以外なら
		m_nCntModeTime++;

		if (m_nCntModeTime >= nResetModeTime)
		{//一定時間が経過したら
			//通常モードに戻す
			m_battleStyle = BATTLESTYLE_NONE;
			m_nCntModeTime = 0;
		}
	}
}

//================================
// 角度の設定
//================================
void CPlayer::SetRot()
{
	//--------------------------------------
	// 目的の角度の正規化
	//--------------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//回転したい角度が180以上なら
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//回転したい角度が-180以下なら
		m_rotDest.y += D3DX_PI * 2;
	}

	//--------------------------------------
	// 目的の角度まで回転する
	//--------------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.05f;	//減衰処理

	//--------------------------------------
	// 角度の正規化
	//--------------------------------------
	if (m_rot.y > D3DX_PI)
	{//角度が180以上なら
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//角度が-180以下なら
		m_rot.y += D3DX_PI * 2;
	}
}

//=============================
// 線の設置
//=============================
void CPlayer::SetLine()
{
	D3DXMATRIX mtxTrans;	//計算用マトリックス

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = CLine::CreateAll(m_pLine[i], i, m_pos, min, max);
	}
}

//=============================
// 線の情報の更新
//=============================
void CPlayer::UpdateLine()
{
	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		if (m_pLine[i])
		{
			m_pLine[i]->SetLinePos(i, min, max);
		}
	}
}

//================================
// コンボ数の加算
//================================
void CPlayer::AddCombo(int nNumber)
{
	m_nNumCombo = m_pCombo->AddNumber(nNumber);
}

//=============================
// HP減少時の処理
//=============================
void CPlayer::SubLife(float fDamage)
{
	m_status.fLife -= fDamage;	//体力の減少

	//残り体力を計算
	m_status.fRemLife = m_status.fLife * 100 / m_status.fMaxLife;

	//HPの設定
	m_pHP->SetLife(m_status.fLife, m_status.fRemLife);
}

//================================
// ワールド座標の取得
//================================
D3DXVECTOR3 CPlayer::GetWorldPos()
{
	D3DXVECTOR3 worldPos(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&worldPos, &m_pos, &m_mtxWorld);

	return worldPos;
}