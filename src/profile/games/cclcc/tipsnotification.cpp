#include "tipsnotification.h"
#include "../../../log.h"
#include "../../../window.h"
#include "../../profile_internal.h"

namespace Impacto {
namespace Profile {
namespace CCLCC {
namespace TipsNotification {

Sprite NotificationBackground;

glm::vec2 BackgroundPosition;
glm::vec2 InitialNotificationPosition;
RectF NotificationRenderingBounds;
float TimerDuration;
float MoveAnimationDuration;

int TipNameColorIndex;
int FontSize;

void Configure() {
  NotificationBackground = EnsureGetMemberSprite("NotificationBackground");

  BackgroundPosition = EnsureGetMemberVec2("BackgroundPosition");
  InitialNotificationPosition =
      EnsureGetMemberVec2("InitialNotificationPosition");
  NotificationRenderingBounds =
      EnsureGetMemberRectF("NotificationRenderingBounds");
  TimerDuration = EnsureGetMemberFloat("TimerDuration");

  TipNameColorIndex = EnsureGetMemberInt("TipNameColorIndex");
  FontSize = EnsureGetMemberInt("FontSize");
}

}  // namespace TipsNotification
}  // namespace CCLCC
}  // namespace Profile
}  // namespace Impacto