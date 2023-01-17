//===================================================
//
// �G�l�~�[�w�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "object.h"

//--------------------------------
// �O���錾
//--------------------------------
class CModel;	//���f��
class CLine;	//���C��
class CHP;		//HP

//--------------------------------
// �}�N����`
//--------------------------------
#define MAX_ENEMY_PARTS	(1)		//���f���p�[�c�̍ő吔
#define MAX_ENEMY_KEY	(2)		//�L�[�̍ő吔

//================================
// �G�l�~�[�N���X�̒�`
//================================
class CEnemy : public CObject
{
public:
	//�L�[�v�f
	struct KEY
	{
		float fPosX;	//�ʒu
		float fPosY;
		float fPosZ;
		float fRotX;	//����
		float fRotY;
		float fRotZ;
	};

	//�L�[���
	struct KEY_SET
	{
		int nFrame;	//�t���[����
		KEY aKey[MAX_ENEMY_PARTS];
	};

	enum ENEMYSTATE
	{
		ENEMYSTATE_NONE = 0,
		ENEMYSTATE_BREAK,
		ENEMYSTATE_MAX
	};

	enum GAUGE
	{
		GAUGE_HP = 0,
		GAUGE_BREAK,
		GAUGE_MAX
	};

	CEnemy();			//�R���X�g���N�^
	~CEnemy() override;	//�f�X�g���N�^

	//----------------
	// �����o�֐�
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// �Z�b�^�[
	//----------------
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }			//�ʒu�̐ݒ�
	void SubGauge(float fDamage, GAUGE type);	//�Q�[�W�������̏���

	//----------------
	// �Q�b�^�[
	//----------------
	D3DXVECTOR3 GetPosition() override  { return m_pos; }		//�ʒu�̎擾
	D3DXVECTOR3 GetRot()				{ return m_rot; }		//�����̎擾
	float GetWidth() override			{ return 0.0f; }		//���̎擾
	float GetHeight() override			{ return 0.0f; }		//�����̎擾
	D3DXVECTOR3 GetSize()				{ return m_size; }		//�傫���̎擾
	D3DXMATRIX GetmtxWorld()			{ return m_mtxWorld; }	//���[���h�}�g���b�N�X�̎擾
	ENEMYSTATE GetState()				{ return m_state; }		//�G�̏�Ԃ̎擾

	//----------------
	// �ÓI�����o�֐�
	//----------------
	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	void SetModel();			//���f���̐ݒ�
	void SetMotion(bool bLoop);	//���[�V�����̐ݒ�
	void SetLine();		//���̐ݒu
	void UpdateLine();	//���̍X�V
	void Attack();		//�U��
	void NockBack();	//�m�b�N�o�b�N���鏈��
	void Move();		//�ړ�

private:
	//----------------
	// �萔
	//----------------
	static const int nMaxLine = 24;	//���̍ő吔
	static const float fDefGravity;	//��{�̏d��

	//----------------
	// �����o�ϐ�
	//----------------
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�ړI�̌���
	D3DXVECTOR3 m_vtxMax;		//�傫���̍ő�l
	D3DXVECTOR3 m_vtxMin;		//�傫���̍ŏ��l
	D3DXVECTOR3 m_worldMin;		//���[���h��̍ő�l
	D3DXVECTOR3	m_worldMax;		//���[���h��̍ŏ��l
	D3DXVECTOR3 m_size;			//�傫��
	int m_nMoveTime;			//�ړ��܂ł̎���
	int m_nAttackTime;			//�U���܂ł̎���
	int m_nBreakTime;			//�u���C�N��Ԃ̎���
	float m_fLife;				//�̗�
	float m_fRemLife;			//�c��̗�(%)
	float m_fMaxLife;			//�ő�̗�
	float m_fGravity;			//�d�͂̒l
	float m_fBreak;
	float m_fRemBreak;
	float m_fMaxBreak;
	bool m_bNockBack;			//�m�b�N�o�b�N������
	CModel*  m_pModel[MAX_ENEMY_PARTS];	//���f��
	CLine*	 m_pLine[nMaxLine];			//���C��
	CHP*	 m_pHP[GAUGE_MAX];			//HP
	ENEMYSTATE m_state;					//�G�̏��


	/* �� ���[�V������� �� */
	int m_nCurrentKey;			//���݂̃L�[�ԍ�
	int m_nCntMotion;			//���[�V�����J�E���^�[
};

#endif