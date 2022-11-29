//===================================================
//
// �Q�[���w�b�_�[
// Author : Sato Teruto
//
//====================================================
#ifndef _GAME_H_
#define _GAME_H_	

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "application.h"

//--------------------------------
// �O���錾
//--------------------------------
class CCamera;		//�J����
class CPlayer;		//�v���C���[
class CMeshField;	//���b�V���t�B�[���h

//================================
// �Q�[���N���X�̒�`
//================================
class CGame
{
public:
	CGame();	//�R���X�g���N�^
	~CGame();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();

	//------------------
	// �Q�b�^�[
	//------------------
	static CCamera* GetCamera() { return m_pCamera; };	//�J�����̎擾
	static CPlayer*	GetPlayer() { return m_pPlayer; };	//�v���C���[�̎擾
	static CMeshField*	GetMeshField()
			{ return m_pMeshField; };	//���b�V���t�B�[���h�̎擾

private:
	//-------------------
	// �ÓI�����o�ϐ�
	//-------------------
	static CCamera*		m_pCamera;		//�J����
	static CPlayer*		m_pPlayer;		//�v���C���[
	static CMeshField*	m_pMeshField;	//���b�V���t�B�[���h
};

#endif