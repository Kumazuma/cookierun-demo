#pragma once

#define WINCX 1500
#define WINCY 800

// 키 입력 관련
//#define NOT_PRESS_(_Key)			((GetAsyncKeyState(_Key) ^ 0xFFFF0000) == 0)
//#define JUST_RELEASE_(_Key)			((GetAsyncKeyState(_Key) ^ 0xFFFF0001) == 0)
//#define KEEP_PRESSING_(_Key)		((GetAsyncKeyState(_Key) ^ 0xFFFF8000) == 0)
//#define JUST_PRESS_(_Key)			((GetAsyncKeyState(_Key) ^ 0xFFFF8001) == 0)
#define IS_PRESS_KEY_(_Key)				(GetAsyncKeyState(_Key) & 0x8000)

// 타이머 관련
#define WORLD_TIMER	CGameWorldTimer::Instance()

// 오브젝트 유효성 체크
#define IS_VALID_OBJ(_pObj) ((_pObj) && (_pObj->IsValid()))
#define DO_IF_IS_NOT_VALID_OBJ(_pObj) if(!IS_VALID_OBJ(_pObj))
#define DO_IF_IS_VALID_OBJ(_pObj) if(IS_VALID_OBJ(_pObj))

// 형 변환 관련
#define TO_SPACE(_pObj)				dynamic_cast<const CSpace*>(_pObj)
#define TO_PLAYER(_pObj)			dynamic_cast<CPlayer*>(_pObj)
#define TO_BULLET(_pObj)			dynamic_cast<CBullet*>(_pObj)
#define TO_SCREW_BULLET(_pObj)		dynamic_cast<CScrewBullet*> (_pObj)
#define TO_MONSTER(_pObj)			dynamic_cast<CMonster*>(_pObj)
#define TO_GAMEWORLD(_pGameWorld)	dynamic_cast<CMainApp&>(_pGameWorld)

// 수학 관련
#define PI acos(-1.f) //== acos(cos(pi)) == pi
#define TWO_PI (2.f * PI)
#define TO_RADIAN(_fDegree) ((_fDegree) * TWO_PI / 360.f)
#define TO_DEGREE(_fRadian) ((_fRadian) * 360.f / TWO_PI)

#define DIR_X_DEGREE(_fDegree) cosf(TO_RADIAN(_fDegree))
#define DIR_Y_DEGREE(_fDegree) sinf(TO_RADIAN(_fDegree))

#define DIR_X_RADIAN(_fRadian) cosf(_fRadian)
#define DIR_Y_RADIAN(_fRadian) sinf(_fRadian)

#define CLAMP_RADIAN(_fRadian) ((_fRadian) -static_cast<int>((_fRadian) / TWO_PI) * TWO_PI)
#define CLAMP_DEGREE(_fDegree) ((_fDegree) -static_cast<int>((_fDegree) / 360.f) * 360.f)
