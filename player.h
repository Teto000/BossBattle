//===================================================
//
// �v���C���[�w�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "object.h"

//--------------------------------
// �O���錾
//--------------------------------
class CModel;	//���f��
class CLine;	//���C��

//--------------------------------
// �}�N����`
//--------------------------------
#define MAX_PARTS	(7)		//���f���p�[�c�̍ő吔
#define MAX_KEY		(4)		//�L�[�̍ő吔

//================================
// �v���C���[�N���X�̒�`
//================================
class CPlayer : public CObject
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
		KEY aKey[MAX_PARTS];
	};

	CPlayer();				//�R���X�g���N�^
	~CPlayer() override;	//�f�X�g���N�^

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
	void SetPosition(D3DXVECTOR3 pos);

	//----------------
	// �Q�b�^�[
	//----------------
	D3DXVECTOR3 GetPosition() override;	//�ʒu�̎擾
	D3DXVECTOR3 GetRot();				//�����̎擾
	float GetWidth() override;			//���̎擾
	float GetHeight() override;			//�����̎擾
	D3DXMATRIX GetmtxWorld();			//���[���h�}�g���b�N�X�̎擾
	CModel* GetModel() { return m_pModel[0]; };	//���f���̎擾

	//----------------
	// �ÓI�����o�֐�
	//----------------
	static CPlayer* Create();

private:
	void SetModel();				//���f���̐ݒ�
	void SetMotion(bool bLoop);		//���[�V�����̐ݒ�
	void MoveKeyboard(int nUpKey, int nDownKey, int nLeftKey, int nRightKey);	//�ړ�
	void MoveJoypad();	//�W���C�p�b�h���g�����ړ�
	void SetRot();		//�p�x�̐ݒ�
	void SetLine();		//���̐ݒu
	void UpdateLine();	//���̍X�V
	void GetSize();		//�傫���̎擾

private:
	//----------------
	// �萔
	//----------------
	static const int nMaxLine = 128;
	static const float fPlayerSpeed;	//�v���C���[�̑��x

	//----------------
	// �����o�ϐ�
	//----------------
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_rot;				//����
	D3DXVECTOR3 m_rotDest;			//�ړI�̌���
	D3DXVECTOR3 m_vtxMax;			//�傫���̍ő�l
	D3DXVECTOR3 m_vtxMin;			//�傫���̍ŏ��l
	D3DXVECTOR3 m_worldMin;			//���[���h��̍ő�l
	D3DXVECTOR3	m_worldMax;			//���[���h��̍ŏ��l
	float m_rotWheel;				//�^�C���̉�]
	CModel*  m_pModel[MAX_PARTS];	//���f��
	CLine* m_pLIne[nMaxLine];		//���C��

	/* �� ���[�V������� �� */
	int m_nCurrentKey;			//���݂̃L�[�ԍ�
	int m_nCntMotion;			//���[�V�����J�E���^�[

	/* �� �N�H�[�^�j�I�� �� */
	D3DXMATRIX		m_mtxRot;	//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION	m_quat;		//�N�H�[�^�j�I��
	D3DXVECTOR3		m_vecAxis;	//��]��
	float			m_fValueRot;//��]�p(���W�A��)
};

#endif