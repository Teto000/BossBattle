//===================================================
//
// モーション処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <stdio.h>
#include <assert.h>
#include "motion.h"
#include "model.h"

//=======================================
// コンストラクタ
//=======================================
CMotion::CMotion()
{

}

//=======================================
// デストラクタ
//=======================================
CMotion::~CMotion()
{

}

//=======================================
// 終了処理
//=======================================
void CMotion::Uninit()
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
}

//=======================================
// 更新処理
//=======================================
void CMotion::Update()
{	
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
}

//=======================================
// 描画処理
//=======================================
void CMotion::Draw(D3DXMATRIX mtxWorld)
{
	//--------------------------------
	// モデルの描画
	//--------------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (!m_pModel[i])
		{//モデルがnullなら
			return;
		}

		m_pModel[i]->Draw(&mtxWorld);
	}
}

//=============================
// モデルの生成
//=============================
void CMotion::SetModel()
{
	//モデル0：タイヤ
	m_pModel[0] = CModel::Create("data\\MODEL\\Player\\wheel.x", nullptr,
		D3DXVECTOR3(0.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル1：体
	m_pModel[1] = CModel::Create("data\\MODEL\\Player\\body.x", nullptr,
		D3DXVECTOR3(0.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル2：頭
	m_pModel[2] = CModel::Create("data\\MODEL\\Player\\head.x", m_pModel[1],
		D3DXVECTOR3(0.0f, 105.0f, 12.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル3：右腕
	m_pModel[3] = CModel::Create("data\\MODEL\\Player\\armR.x", m_pModel[1],
		D3DXVECTOR3(-25.0f, 85.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル4：右手
	m_pModel[4] = CModel::Create("data\\MODEL\\Player\\handR.x", m_pModel[3],
		D3DXVECTOR3(-4.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル5：左腕
	m_pModel[5] = CModel::Create("data\\MODEL\\Player\\armL.x", m_pModel[1],
		D3DXVECTOR3(25.0f, 85.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル6：左手
	m_pModel[6] = CModel::Create("data\\MODEL\\Player\\handL.x", m_pModel[5],
		D3DXVECTOR3(4.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//==========================================
// ファイルからモーション情報を取得
//==========================================
void CMotion::GetFileMotion()
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
				{//頭文字がCOLLISIONなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nStartCollision);
				}
				//-------------------------------
				// 次の攻撃入力開始時間
				//-------------------------------
				else if (strcmp(&cTextHead[0], "NEXT_ATTACK") == 0)
				{//頭文字がNEXT_ATTACKなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nNextAtkTime);
				}
				//-------------------------------
				// 攻撃に必要なポイント数
				//-------------------------------
				else if (strcmp(&cTextHead[0], "NUM_POINT") == 0)
				{//頭文字がNUM_POINTなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nNumPoint);
				}
				//-------------------------------
				// クリティカル率
				//-------------------------------
				else if (strcmp(&cTextHead[0], "CRITICAL") == 0)
				{//頭文字がCRITICALなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nCritical);
				}
				//-------------------------------
				// ヒット数
				//-------------------------------
				else if (strcmp(&cTextHead[0], "NUM_HIT") == 0)
				{//頭文字がNUM_HITなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nNumHit);
				}
				//-------------------------------
				// ヒット間隔
				//-------------------------------
				else if (strcmp(&cTextHead[0], "HIT_INTERVAL") == 0)
				{//頭文字がHIT_INTERVALなら
				 //文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nHitInterval);
				}
				//-------------------------------
				// ダメージ倍率
				//-------------------------------
				else if (strcmp(&cTextHead[0], "DAMAGE_MAG") == 0)
				{//頭文字がDAMAGE_MAGなら
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
void CMotion::SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey)
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
CMotion::MOTION_TYPE CMotion::ChangeMotion(MOTION_TYPE type)
{
	//モーションの変更
	m_type = type;

	//モーション情報の初期化
	if (m_type == CMotion::MOTION_ATTACK_1
		|| m_type == CMotion::MOTION_ATTACK_2)
	{//どれか攻撃モーションなら
		m_nCurrentKey = 0;
		m_nCntMotion = 0;
		//m_status.nAttackTime = 0;	//攻撃時間のリセット
		//m_bFinishAttack = false;	//ダメージを与えていない状態にする
	}

	return m_type;
}