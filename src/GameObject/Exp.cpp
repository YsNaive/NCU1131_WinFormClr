
#include "Exp.h"

#include "Drawer.h"
#include "Player.h"

Exp::Exp(float value)
	:value(value)
{
	render_layer = Layer::Exp;
	tag.Add(Tag::Exp);
	rigidbody.decelerate = 0.95;
	collider.AddCircle({ {0,0},value * 3.0f });
}

void Exp::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (other->tag.Contains(Tag::Player)) {
		auto player = (Player*)other;
		player->ReciveExp(value);
		Destroy();
	}
}

void Exp::Render()
{
	Drawer::AddFillCircle(Color(0.3, 1.0, 0.3), { {0,0}, value * 3.0f });
	Drawer::AddCircle(Color(0.1, 0.6, 0.1), { {0,0}, value * 3.0f });
}
