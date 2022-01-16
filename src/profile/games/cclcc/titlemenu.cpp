#include "titlemenu.h"
#include "../../../log.h"
#include "../../../window.h"
#include "../../profile_internal.h"

#include "../../ui/titlemenu.h"
#include "../../../game.h"
#include "../../../ui/ui.h"
#include "../../../games/cclcc/titlemenu.h"

namespace Impacto {
namespace Profile {
namespace CCLCC {
namespace TitleMenu {

Sprite BackgroundSprite;
Sprite MainBackgroundSprite;
Sprite CopyrightTextSprite;
Sprite MenuSprite;
Sprite ItemHighlightSprite;
Sprite ItemHighlightPointerSprite;

float CopyrightTextX;
float CopyrightTextY;
float MenuX;
float MenuY;
float ItemHighlightOffsetX;
float ItemHighlightOffsetY;
float ItemYBase;
float ItemPadding;
float ItemHighlightPointerY;

static void GetMemberSpriteArray(Sprite* arr, uint32_t count,
                                 char const* name) {
  EnsurePushMemberOfType(name, kArrayType);

  if (TopVal().Size() != count) {
    ImpLog(LL_Fatal, LC_Profile, "Expected to have %d sprites for %s\n", count,
           name);
    Window::Shutdown();
  }

  for (uint32_t i = 0; i < count; i++) {
    arr[i] = EnsureGetArrayElementSprite(i);
  }

  Pop();
}

void Configure() {
  BackgroundSprite = EnsureGetMemberSprite("BackgroundSprite");
  MainBackgroundSprite = EnsureGetMemberSprite("MainBackgroundSprite");
  CopyrightTextSprite = EnsureGetMemberSprite("CopyrightTextSprite");
  CopyrightTextX = EnsureGetMemberFloat("CopyrightTextX");
  CopyrightTextY = EnsureGetMemberFloat("CopyrightTextY");
  MenuSprite = EnsureGetMemberSprite("MenuSprite");
  MenuX = EnsureGetMemberFloat("MenuX");
  MenuY = EnsureGetMemberFloat("MenuY");
  ItemHighlightSprite = EnsureGetMemberSprite("ItemHighlightSprite");
  ItemHighlightOffsetX = EnsureGetMemberFloat("ItemHighlightOffsetX");
  ItemHighlightOffsetY = EnsureGetMemberFloat("ItemHighlightOffsetY");
  ItemYBase = EnsureGetMemberFloat("ItemYBase");
  ItemPadding = EnsureGetMemberFloat("ItemPadding");
  ItemHighlightPointerSprite =
      EnsureGetMemberSprite("ItemHighlightPointerSprite");
  ItemHighlightPointerY = EnsureGetMemberFloat("ItemHighlightPointerY");

  UI::CCLCC::TitleMenu* menu = new UI::CCLCC::TitleMenu();
  menu->PressToStartAnimation.DurationIn =
      Profile::TitleMenu::PressToStartAnimDurationIn;
  menu->PressToStartAnimation.DurationOut =
      Profile::TitleMenu::PressToStartAnimDurationOut;
  menu->PressToStartAnimation.LoopMode = ALM_ReverseDirection;

  UI::TitleMenuPtr = menu;

  auto drawType = Game::DrawComponentType::_from_integral_unchecked(
      EnsureGetMemberInt("DrawType"));

  UI::Menus[drawType].push_back(UI::TitleMenuPtr);
}

}  // namespace TitleMenu
}  // namespace CCLCC
}  // namespace Profile
}  // namespace Impacto