#include "stdafx.h"
#include "State.h"
#include "Define.h"
#include "CObj.h"
#include "MainApp.h"
#include "CMap.h"
#include "CBlock.h"

constexpr float JUMP_SPPED = 600.f;
constexpr float GRAVITY_ACCELATION = -800.f;
void State::Default::OnLoaded(CObj* const pObject)
{
   
}
IPlayerState* State::Default::Update(CObj* const pObject, float fTimedelta)
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

void State::Jump::OnLoaded(CObj* const pObject)
{
    m_fSpeed = JUMP_SPPED;
}

IPlayerState* State::Gravity::Update(CObj* const pObject, float fTimedelta)
{
    auto& mainApp = (CMainApp&)pObject->GetGameWorld();
    CMap* pMap = mainApp.GetMap();
    auto* pBlock = pMap->GetBlockUnderPlayer();
    float fY = pObject->GetY();
    fY -= m_fSpeed * fTimedelta;
    pObject->SetY(fY);
    
    return this;
}

State::Gravity::Gravity():
    m_fSpeed{0.f}
{
    
}

void State::Gravity::GravityMove(CObj* const pObject, float fTimedelta)
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

bool State::Gravity::IsReturn(CObj* const pObject)
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
State::SingleJump::SingleJump():
    m_bReachedTop{ false }
{
    
}
IPlayerState* State::SingleJump::Update(CObj* const pObject, float fTimedelta)
{
    Gravity::Update(pObject, fTimedelta);
    Gravity::GravityMove(pObject, fTimedelta);
    if (IsReturn(pObject))
    {
        return new Default{};
    }
    if (m_bReachedTop == false && m_fSpeed <= 0.f)
    {
        m_bReachedTop = true;
    }
    if (m_bReachedTop == false && m_fSpeed < JUMP_SPPED / 2.f && IS_PRESS_KEY_(VK_SPACE))
    {
        //TODO: double Jump
        return new DoubleJump {};
    }
    //아무런 변화가 없으면 자신을 반환
    return this;
}

void State::DoubleJump::OnLoaded(CObj* const)
{
    m_fSpeed = JUMP_SPPED ;
}

IPlayerState* State::DoubleJump::Update(CObj* const pObject, float fTimedelta)
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

void State::Slide::OnLoaded(CObj* const pObject)
{
    m_fPrevHeight = pObject->GetHeight();
    m_fprevWidth = pObject->GetWidth();
    m_fPrevPosY = pObject->GetY();
    //슬라이딩 할 때는 높이를 1/2으로 만든다.
    float fY = m_fPrevPosY + m_fPrevHeight / 4;
    pObject->SetWidth(static_cast<size_t>( m_fprevWidth * 1.5f) );
    pObject->SetHeight(m_fPrevHeight / 2);
    pObject->SetY(fY);
}

IPlayerState* State::Slide::Update(CObj* const pObject, float fTimedelta)
{
    Gravity::Update(pObject, fTimedelta);
    Gravity::GravityMove(pObject, fTimedelta);
    IPlayerState* pNextState = nullptr;
    //점프 명령어를 우선한다.
    if (IS_PRESS_KEY_(VK_SPACE))
    {
        pNextState = new SingleJump{};
    }
    else if (IS_PRESS_KEY_(VK_CONTROL) == false)
    {
        pNextState = new Default{};
    }
    if (pNextState->IsNull() == false)
    {
        pObject->SetHeight(m_fPrevHeight);
        pObject->SetWidth(static_cast<size_t>(m_fprevWidth));
        pObject->SetY(m_fPrevPosY);
        return pNextState;
    }
    return this;
}
