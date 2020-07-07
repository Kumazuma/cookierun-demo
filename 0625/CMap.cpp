#include "stdafx.h"
#include "CMap.h"
#include "CBlock.h"
#include "MainApp.h"

CMap::CMap(CGameWorld & _rGameWorld, const char * _szDataDirectory)
	:
	m_rGameWorld(_rGameWorld),
	m_fMapX(0.f),					// Map�� ��ü���� X��ǥ // Y ��ǥ�� ����
	m_fMapSpeed(0.f),				// X�� �̵��� ���� Map�� �̵��ӵ�
	m_fFirstBlockX(0.f),			// ù ��° ����� X��ǥ (������ �Ǵ� Block�̴�.)
	m_fFirstBlockY(WINCY >> 1),		// ù ���� ����� Y��ǥ (������ �Ǵ� Block�̴�.)
	m_iBlockWidth(100.f),			// Block�� �ʺ�
	m_iBlockHeight(100.f)			// Block�� ����
{
	FILE* fpIn;
	errno_t err = 0;

	err = fopen_s(&fpIn, _szDataDirectory, "rt");
	if (!err) {
		fscanf_s(fpIn, "%f %f %f %f %d %d",
			&m_fMapX,
			&m_fMapSpeed,
			&m_fFirstBlockX,
			&m_fFirstBlockY,
			&m_iBlockWidth,
			&m_iBlockHeight);
		
		CObj* pBlock = nullptr;
		int iHeightIndex = 0;
		int iBlockNum = 0;
		float fCurrentX = m_fFirstBlockX;
		while (!feof(fpIn)) {
			fscanf_s(fpIn, "%d %d", &iHeightIndex, &iBlockNum);
			if (iHeightIndex == -1) {
				for (int i = 0; i < iBlockNum; i++) fCurrentX += m_iBlockWidth;
				continue;
			}
			for (int i = 0; i < iBlockNum; i++) {
				pBlock = new CBlock(_rGameWorld, *this, fCurrentX, m_fFirstBlockY + m_iBlockHeight * iHeightIndex, m_iBlockWidth, m_iBlockHeight);
				m_vecBlocks.emplace_back(pBlock);
				fCurrentX += m_iBlockWidth;
			}
		}
	}
	else abort();
}

CMap::~CMap()
{
}

void CMap::Update(void)
{
	// TODO : ���⼭ MapX�� �����Ѵ�.
	m_fMapX += m_fMapSpeed;
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->Update(); } }
}

void CMap::LateUpdate(void)
{
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->LateUpdate(); } }
}

void CMap::Render(const HDC& _hdc)
{
	for (auto& pBlock : m_vecBlocks) {
		DO_IF_IS_VALID_OBJ(pBlock) {
			pBlock->Render(_hdc);
		}
	}
}

void CMap::Release(void)
{
}
