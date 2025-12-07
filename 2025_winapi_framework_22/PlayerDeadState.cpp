#include "pch.h"
#include "PlayerDeadState.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Scene.h"

PlayerDeadState::PlayerDeadState(Player* _owner)
	: m_player(_owner)
	, m_curTime(0.f)
	, m_deadTime(2.f)
{

}

void PlayerDeadState::Enter(StateMachine& _owner)
{
    m_player->ChangeAnimation(L"Dead", false);
    if (Rigidbody* rb = m_player->GetComponent<Rigidbody>())
    {
        rb->SetVelocity({ 0.f, 0.f });
    }
    if (Collider* col = m_player->GetComponent<Collider>())
    {
        col->SetTrigger(true);
    }
}

void PlayerDeadState::Update(StateMachine& _owner)
{
    m_curTime += fDT;

    if (m_deadTime <= m_curTime)
    {
        std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
        if (curScene)
        {
            curScene->RequestDestroy(m_player);
        }
    }
}

void PlayerDeadState::Exit(StateMachine& _owner)
{

}
