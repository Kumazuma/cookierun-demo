#include "stdafx.h"
#include "CMap.h"
#include "CBlock.h"
#include "MainApp.h"
#include "CGameWorld.h"
#include "item.h"

CMap::CMap(CGameWorld & _rGameWorld, const char * _szDataDirectory)
	:
	m_rGameWorld(_rGameWorld),
	m_fMapX(0.f),					// Map의 전체적인 X좌표 // Y 좌표는 고정
	m_fMapSpeed(0.f),				// X축 이동에 대한 Map의 이동속도
	m_fFirstBlockX(0.f),			// 첫 번째 블록의 X좌표 (기준이 되는 Block이다.)
	m_fFirstBlockY(WINCY >> 1),		// 첫 번재 블록의 Y좌표 (기준이 되는 Block이다.)
	m_iBlockWidth(100.f),			// Block의 너비
	m_iBlockHeight(100.f),			// Block의 높이
	m_pBlockUnderPlayer(nullptr)
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
		CObj* pObject = nullptr;	// 블록 위에 생성할 오브젝트
		int iHeightIndex = 0;
		int iBlockNum = 0;
		int bIsObjectsOnBlock = false;
		int iObjectsHeightIndex = 0;
		int eObjectType = OBJ::TYPE_END;
		float fCurrentX = m_fFirstBlockX;
		vector<_object_generation_info> vecObjGenInfos;
		_object_generation_info sObjGenInfos;
		vecObjGenInfos.reserve(20);
		while (!feof(fpIn)) {
			fscanf_s(fpIn, "%d %d %d", &iHeightIndex, &iBlockNum, &bIsObjectsOnBlock);

			vecObjGenInfos.clear();

			// 블록 위에 오브젝트가 존재한다면 오브젝트 생성 정보를 만들어 저장해둔다.
			if (bIsObjectsOnBlock == 1) {
				fscanf_s(fpIn, "%d", &iObjectsHeightIndex);
				iObjectsHeightIndex += 1;
				while (true) {
					fscanf_s(fpIn, "%d", &eObjectType);
					if (eObjectType == -2) break;
					if (eObjectType == -1) {
						// -1은 해당 칸을 건너뛰는 것을 의미한다.
						iObjectsHeightIndex++;
						continue;
					}
					// 아이템의 타입과 높이를 저장한다.
					// X값은 아래 블록 생성 시 블록의 X에 종속된다.
					sObjGenInfos.eType = static_cast<OBJ::E_TYPE>(eObjectType);
					sObjGenInfos.fHeight = iObjectsHeightIndex * m_iBlockHeight;
					vecObjGenInfos.emplace_back(sObjGenInfos);
					iObjectsHeightIndex++;
				}
			}

			if (iHeightIndex == -1) {
				// iHeightIndex가 == -1이라면 스킵한다.
				for (int i = 0; i < iBlockNum; i++) fCurrentX += m_iBlockWidth;
				continue;
			}
			for (int i = 0; i < iBlockNum; i++) {
				// 블록을 생성한다.
				pBlock = new CBlock(_rGameWorld, *this, fCurrentX, m_fFirstBlockY - m_iBlockHeight * iHeightIndex, m_iBlockWidth, m_iBlockHeight);
				m_vecBlocks.emplace_back(pBlock);

				// 아이템을 생성한다.
				if (!vecObjGenInfos.empty()) {
					for (auto& rObjGenInfo : vecObjGenInfos) {
						switch (rObjGenInfo.eType) {
						case OBJ::TYPE_OBSTACLE:
							break;
						case OBJ::TYPE_COIN:
							m_vecItems.emplace_back(CItem::CreateItem<Item::CCoin>(m_rGameWorld, *this, fCurrentX, pBlock->GetY() - rObjGenInfo.fHeight));
							break;
						case OBJ::TYPE_HEALTH:
							m_vecItems.emplace_back(CItem::CreateItem<Item::CLife>(m_rGameWorld, *this, fCurrentX, pBlock->GetY() - rObjGenInfo.fHeight));
							break;
						}
					}
				}
				
				fCurrentX += m_iBlockWidth;
			}
		}
		m_fKillX = m_fFirstBlockX - m_iBlockWidth;
		m_fRespawnX = m_vecBlocks.back()->GetX();
	}
	else abort();
}

CMap::~CMap()
{
}

void CMap::Update(void)
{
	m_fMapX += m_fMapSpeed;
	if (m_fMapX >= GetMapDistance()) m_fMapX -= GetMapDistance();
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->Update(); } }
	for (auto& pItem : m_vecItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Update(); } }
}

void CMap::LateUpdate(void)
{
	m_pBlockUnderPlayer = nullptr;	// 기존 플레이어 아래에 있던 블록은 플레이어 아래에 있지 않을 수 있으므로 재갱신을 위해 nullptr로 설정.
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->LateUpdate(); } }
	for (auto& pItem : m_vecItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->LateUpdate(); } }
}

void CMap::Render(const HDC& _hdc)
{
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->Render(_hdc); } }
	for (auto& pItem : m_vecItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Render(_hdc); } }
}

void CMap::Release(void)
{
}

float CMap::GetConvX(float _fX) const
{
	if (_fX - m_fMapX < GetKillX()) {
		return GetRespawnX() + (_fX - m_fMapX - GetKillX());
	}
	return _fX - m_fMapX;
}

float CMap::GetConvLeft(float _fX) const
{
	return GetConvX(_fX) - (m_iBlockWidth >> 1);
}

float CMap::GetConvRight(float _fX) const
{
	return GetConvX(_fX) + (m_iBlockWidth >> 1);
}

CBlock * CMap::GetBlockUnderPlayer(void) const
{
	return m_pBlockUnderPlayer;
}

void CMap::SetBlockUnderPlayer(CBlock * _pBlock)
{
	if (m_pBlockUnderPlayer) {
		// 블록이 있다면 가장 최상단 블록으로 세팅한다.
		if (_pBlock->GetY() < m_pBlockUnderPlayer->GetY())
			m_pBlockUnderPlayer = _pBlock;
	}
	m_pBlockUnderPlayer = _pBlock;
}
