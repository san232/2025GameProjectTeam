#pragma once
#include "SubWindowRenderer.h"
#include "Scene.h"
#include "Core.h"
#include "Object.h"

class InvisibleEnemyOverlayRenderer
{
public:
    InvisibleEnemyOverlayRenderer(Scene* scene) : m_scene(scene) {}

    void Render(HDC hdc)
    {
        if (!m_scene) return;

        int saved = ::SaveDC(hdc);
        ::SetBkMode(hdc, TRANSPARENT);
        
        const std::vector<Object*>& ref = m_scene->GetLayerObjects(Layer::INVISIBLEENEMY);
        std::vector<Object*> snapshot(ref.begin(), ref.end());

        for (Object* obj : snapshot)
        {
            if (obj != nullptr)
                obj->Render(hdc);
        }

        ::RestoreDC(hdc, saved);
    }

private:
    Scene* m_scene;
};
