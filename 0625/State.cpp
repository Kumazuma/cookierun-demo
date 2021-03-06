#include "stdafx.h"
#include "State.h"
#include "Define.h"
#include "CObj.h"
#include "MainApp.h"
#include "CMap.h"
#include "CBlock.h"
#include "Player.h"
#include "UI_Gauge.h"
constexpr float JUMP_SPPED = 900.f;
constexpr float GRAVITY_ACCELATION = -2300.f;
void State::Move::Default::OnLoaded(CObj* const pObject)
{
   
}
IPlayeMoveState* State::Move::Default::Update(CObj* const pObject, float fTimedelta)
{
    Gravity::Update(pObject, fTimedelta);
    Gravity::GravityMove(pObject, fTimedelta);

    if (IS_PRESS_KEY_(VK_SPACE))
    {
        return new SingleJump{};
    }
    else if (IS_PRESS_KEY_(VK_CONTROL))
    {
        return new Slide{};
    }
    return this;
}

void State::Move::Jump::OnLoaded(CObj* const pObject)
{
    m_fSpeed = JUMP_SPPED;
}

IPlayeMoveState* State::Move::Gravity::Update(CObj* const pObject, float fTimedelta)
{
    auto& mainApp = (CMainApp&)pObject->GetGameWorld();
    CMap* pMap = mainApp.GetMap();
    auto* pBlock = pMap->GetBlockUnderPlayer();
    float fY = pObject->GetY();
    fY -= m_fSpeed * fTimedelta;
    pObject->SetY(fY);
    
    return this;
}

State::Move::Gravity::Gravity():
    m_fSpeed{0.f}
{
    
}

void State::Move::Gravity::GravityMove(CObj* const pObject, float fTimedelta)
{
    
    float fY = pObject->GetY();
    float fPlayerBottom = fY + pObject->GetHeight() / 2.f;
    auto& mainApp = (CMainApp&)pObject->GetGameWorld();
    CMap* pMap = mainApp.GetMap();
    auto* pBlock = pMap->GetBlockUnderPlayer();
    float fBlockTop = 0;
    if (pBlock == nullptr)
    {
        m_fSpeed += GRAVITY_ACCELATION * fTimedelta;
    }
    else
    {

        fBlockTop = pBlock->GetY() - pBlock->GetHeight() / 2.f;
        if (fBlockTop > fPlayerBottom)
        {
            m_fSpeed += GRAVITY_ACCELATION * fTimedelta;
        }
        
    }
}

bool State::Move::Gravity::IsReturn(CObj* const pObject)
{
    float fY = pObject->GetY();
    float fPlayerBottom = fY + pObject->GetHeight() / 2.f;
    auto& mainApp = (CMainApp&)pObject->GetGameWorld();
    CMap* pMap = mainApp.GetMap();
    auto* pBlock = pMap->GetBlockUnderPlayer();
    float fBlockTop = 0;
    if (pBlock != nullptr)
    {
        fBlockTop = pBlock->GetY() - pBlock->GetHeight() / 2.f;
        if (fBlockTop <= fPlayerBottom)
        {
            m_fSpeed = 0.0f;
            float fPlayerHeight = pObject->GetHeight();
            float newY = fBlockTop - fPlayerHeight / 2.f;
            pObject->SetY(newY);
            return true;
        }
    }
    return false;
}
State::Move::SingleJump::SingleJump():
    m_bReachedTop{ false }
{
    
}
IPlayeMoveState* State::Move::SingleJump::Update(CObj* const pObject, float fTimedelta)
{
    Gravity::Update(pObject, fTimedelta);
    Gravity::GravityMove(pObject, fTimedelta);
    if (IsReturn(pObject))
    {
        return new Default{};
    }
    /*if (m_bReachedTop == false && m_fSpeed <= 0.f)
    {
        m_bReachedTop = true;
    }*/
    if (m_bReachedTop == false && m_fSpeed < JUMP_SPPED / 2.f && IS_PRESS_KEY_(VK_SPACE))
    {
        //TODO: double Jump
        return new DoubleJump {};
    }
    //아무런 변화가 없으면 자신을 반환
    return this;
}

void State::Move::DoubleJump::OnLoaded(CObj* const)
{
    m_fSpeed = JUMP_SPPED ;
}

IPlayeMoveState* State::Move::DoubleJump::Update(CObj* const pObject, float fTimedelta)
{
    Gravity::Update(pObject, fTimedelta);
    GravityMove(pObject, fTimedelta);
    if (IsReturn(pObject))
    {
        //TODO: Default State
        return new Default{};
    }
    //아무런 변화가 없으면 자신을 반환
    return this;
}

void State::Move::Slide::OnLoaded(CObj* const pObject)
{
    m_fPrevHeight = pObject->GetHeight();
    m_fprevWidth = pObject->GetWidth();
    m_fPrevPosY = pObject->GetY();
    //슬라이딩 할 때는 높이를 1/2으로 만든다.
    float fY = m_fPrevPosY + 2 * m_fPrevHeight / 3;
    pObject->SetWidth(static_cast<size_t>( m_fprevWidth * 2.f) );
    pObject->SetHeight(m_fPrevHeight / 2.f);
    pObject->SetY(fY);
}

IPlayeMoveState* State::Move::Slide::Update(CObj* const pObject, float fTimedelta)
{
    Gravity::Update(pObject, fTimedelta);
    Gravity::GravityMove(pObject, fTimedelta);
    IPlayeMoveState* pNextState = nullptr;
    //점프 명령어를 우선한다.
    if (IS_PRESS_KEY_(VK_SPACE))
    {
        pNextState = new SingleJump{};
    }
    else if (IS_PRESS_KEY_(VK_CONTROL) == false)
    {
        pNextState = new Default{};
    }
    if (pNextState != nullptr)
    {
        pObject->SetHeight(m_fPrevHeight);
        pObject->SetWidth(static_cast<size_t>(m_fprevWidth));
        pObject->SetY(m_fPrevPosY);
        return pNextState;
    }
    return this;
}

void State::BumpState::OnLoaded(CPlayer* const pObject)
{
    pObject->SetImmortal(true);
    auto& app = (CMainApp&)pObject->GetGameWorld();
    auto* pMap = app.GetMap();
    pMap->SlowMap(BumpState::Interval);
}

IPlayerCollisionState* State::BumpState::Update(CPlayer* const pObject, float timedelta)
{
    m_fTime -= timedelta;
    if (m_fTime <= 0.f)
    {
        pObject->SetImmortal(false);
        return new NormalState{};
    }
    return this;
}

void State::BumpState::Render(CPlayer* const pObject, HDC hDC)
{
    if (m_bTicToc)
    {
        RECT rc{ pObject->GetRect() };
        Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
    }
    pObject->GetGauge()->Render(hDC);
    m_bTicToc = !m_bTicToc;
}

void State::NormalState::OnLoaded(CPlayer* const pObject)
{
    pObject->SetImmortal(false);
}

IPlayerCollisionState* State::NormalState::PostUpdate(CPlayer* const pObject)
{
    auto& app = (CMainApp&)pObject->GetGameWorld();
    auto* map = app.GetMap();
    auto& obstacles = map->GetObstacles();
    float playerLeft = pObject->GetX() - pObject->GetWidth() / 2.f;
    float playerRight = pObject->GetX() + pObject->GetWidth() / 2.f;
    float playerX = pObject->GetX();
    float playerY = pObject->GetY();
    for (auto obstacle : obstacles)
    {
        float x = map->GetConvX(obstacle->GetX());
        float distance = 0.0f;
        float radiusSum = 0.0f;
        distance = fabsf(x - playerX);
        radiusSum = (pObject->GetWidth() + obstacle->GetWidth()) * 0.5f;
        if (distance > radiusSum)
        {
            continue;
        }
        float y = obstacle->GetY();
        distance = fabsf(y - playerY);
        radiusSum = (pObject->GetHeight() + obstacle->GetHeight()) * 0.5f;
        if (distance > radiusSum)
        {
            continue;
        }

        //충돌이 일어났음. 2초 동안 줄어드는 포인트만큼 줄여 버린다.
        pObject->HealHP(CPlayer::DECREASE_POINT_PER_SECOND * -1);
        return new BumpState{};
    }
    return this;
}

void State::NormalState::Render(CPlayer* const pObject, HDC hDC)
{
    RECT rc{ pObject->GetRect() };
    Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
    pObject->GetGauge()->Render(hDC);
}
