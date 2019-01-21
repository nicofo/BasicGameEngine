#include "PickupScript.h"
#include "extern.h"
void PickupScript::init()
{
	realPlayerTransform = &ECS.getComponentFromEntity<Transform>(realPlayer);
	pickupTransform = &ECS.getComponentFromEntity<Transform>(owner_);
}

void PickupScript::update(float dt)
{
	auto distance = realPlayerTransform->position().distance(pickupTransform->position());
	if (distance <= distanceTrigger) {
		ECS.current_pickups++;
		pickupTransform->position(0.0f, -100.0f, 0.0f);

		if (ECS.current_pickups == ECS.total_pickups) {
			int multi_glyph = ECS.createEntity("lose glyph");
			GUIText& el_end = ECS.createComponentForEntity<GUIText>(multi_glyph);

			// set text properties
			el_end.width = 300;
			el_end.height = 200;
			el_end.font_face = "data/fonts/arial.ttf";
			el_end.font_size = 40;
			el_end.text = "YOU WIN";
			el_end.color = lm::vec3(1.0, 0.0, 1.0);
			ECS.game_win = true;
		}

		char* text = new char[32];
		GUIText& el_multi = ECS.getComponentFromEntity<GUIText>(ECS.gui_counter_id);
		sprintf(text, "%d / %d golden boxes", ECS.current_pickups, ECS.total_pickups);
		el_multi.text = text;
		ECS.gui_system_->updateTextures();
		ECS.gui_system_->lateInit();
	}

}
