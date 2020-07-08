#pragma once

class CGameWorld;
class CBlock;

class CMap
{
public:
	CMap(CGameWorld& _rGameWorld, const char* _szDataDirectory);
	~CMap();

public:
	void Update(void);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

public:
	float GetMapX(void) const { return m_fMapX; }
	float GetConvX(float _fX) const;
	float GetConvLeft(float _fX) const;
	float GetConvRight(float _fX) const;
	float GetMapDistance(void) const { return m_fRespawnX - m_fKillX; }
	float GetRespawnX(void) const { return m_fRespawnX; }
	float GetKillX(void) const { return m_fKillX; }
	size_t GetBlockWidth(void) const { return m_iBlockWidth; }
	size_t GetBlockHeight(void) const { return m_iBlockHeight; }
	void SlowMap(float _fSlowTime);

	void SetBlockUnderPlayer(CBlock* _pBlock);
	CBlock* GetBlockUnderPlayer(void) const;
	const vector<const CObj*>& GetObstacles() const {return (vector<const CObj*>&)m_vecObstacles;}
private:
	void UpdateSlow(void);

private:
	CGameWorld& m_rGameWorld;		// Block�� ������ GameWorld ������ ����
	float m_fMapX;					// Map�� ��ü���� X��ǥ // Y ��ǥ�� ����
	float m_fMapMaxSpeed;			// X�� �̵��� ���� Map�� �ִ��̵��ӵ�(��� => �������� ���� �̵�)
	float m_fMapSpeed;				// ���� �� �ӵ� (��� : �ִ�ġ, ���ο� : �ִ�ġ ����)
	float m_fKillX;					// Block�� ������� ����
	float m_fRespawnX;				// KillX�� ���� Block�� �������Ǵ� ������ X
	float m_fFirstBlockX;			// ù ��° ����� X��ǥ (������ �Ǵ� Block�̴�.)
	float m_fFirstBlockY;			// ù ���� ����� Y��ǥ (������ �Ǵ� Block�̴�.)
	size_t m_iBlockWidth;			// Block�� �ʺ�
	size_t m_iBlockHeight;			// Block�� ����

	vector<CObj*> m_vecItems;		// �����۵�
	vector<CObj*> m_vecBlocks;		// Block��
	vector<CObj*> m_vecObstacles;	// ��ֹ���

	bool m_bIsSlow;
	float m_fSlowTime;
	float m_fSlowElapsedTime;
	CBlock* m_pBlockUnderPlayer;
};

