#include "moviemenu.h"

#include "../../profile/games/chlcc/moviemenu.h"
#include "../../profile/scriptvars.h"
#include "../../profile/profile_internal.h"
#include "../../renderer2d.h"
#include "../../ui/ui.h"
#include "../../data/savesystem.h"

namespace Impacto {
namespace UI {
namespace CHLCC {
using namespace Impacto::Profile::CHLCC::MovieMenu;
using namespace Impacto::Profile::ScriptVars;

using namespace Impacto::UI::Widgets;

/*void MovieMenu::MovieButtonOnClick(Button* target) {
  auto movieButton =
  static_cast<Widgets::CHLCC::ImageThumbnailButton*>(target); if
  (!movieButton->IsLocked) { ScrWork[SW_MOVIEMODE_CUR] = movieButton->Id;
  }

}*/

MovieMenu::MovieMenu() {
  MenuTransition.Direction = 1.0f;
  MenuTransition.LoopMode = ALM_Stop;
  MenuTransition.DurationIn = MenuTransitionDuration;
  MenuTransition.DurationOut = MenuTransitionDuration;

  TitleFade.Direction = 1.0f;
  TitleFade.LoopMode = ALM_Stop;
  TitleFade.DurationIn = TitleFadeInDuration;
  TitleFade.DurationOut = TitleFadeOutDuration;

  RedBarSprite = InitialRedBarSprite;
  RedBarPosition = InitialRedBarPosition;
}

void MovieMenu::Show() {
  ScrWork[SW_MOVIEMODE_CUR] = 255;
  if (State != Shown) {
    if (State != Showing) MenuTransition.StartIn();
    State = Showing;
    if (UI::FocusedMenu != 0) {
      LastFocusedMenu = UI::FocusedMenu;
      LastFocusedMenu->IsFocused = false;
    }
    IsFocused = true;
    UI::FocusedMenu = this;
  }
}

void MovieMenu::Hide() {
  if (State != Hidden) {
    if (State != Hiding) {
      MenuTransition.StartOut();
    }
    State = Hiding;
    if (LastFocusedMenu != 0) {
      UI::FocusedMenu = LastFocusedMenu;
      LastFocusedMenu->IsFocused = true;
    } else {
      UI::FocusedMenu = 0;
    }
    IsFocused = false;
  }
}

void MovieMenu::Render() {
  if (State != Hidden) {
    if (MenuTransition.IsIn()) {
      Renderer2D::DrawRect(RectF(0.0f, 0.0f, 1280.0f, 720.0f),
                           RgbIntToFloat(BackgroundColor));
    } else {
      DrawCircles();
    }
    DrawErin();
    glm::vec3 tint = {1.0f, 1.0f, 1.0f};
    // Alpha goes from 0 to 1 in half the time
    float alpha =
        MenuTransition.Progress < 0.5f ? MenuTransition.Progress * 2.0f : 1.0f;
    Renderer2D::DrawSprite(BackgroundFilter, RectF(0.0f, 0.0f, 1280.0f, 720.0f),
                           glm::vec4(tint, alpha));
    DrawRedBar();
    // DrawTitles();
    int yOffset = 0;
    if (MenuTransition.Progress > 0.22f) {
      if (MenuTransition.Progress < 0.72f) {
        // Approximated function from the original, another mess
        yOffset = glm::mix(
            -720.0f, 0.0f,
            1.00397f * std::sin(3.97161f - 3.26438f * MenuTransition.Progress) -
                0.00295643f);
      }
      /* Renderer2D::DrawSprite(
           ClearListLabel,
           glm::vec2(LabelPosition.x, LabelPosition.y + yOffset));
       DrawPlayTime(yOffset);
       DrawEndingCount(yOffset);
       DrawTIPSCount(yOffset);
       DrawAlbumCompletion(yOffset);
       DrawEndingTree(yOffset);
       DrawButtonPrompt();*/
    }
  }
}

void MovieMenu::Update(float dt) {
  if (ScrWork[SW_SYSMENUCT] < 10000 && State == Shown) {
    Hide();
  } else if (GetFlag(SF_MOVIEMENU) && ScrWork[SW_SYSMENUCT] > 0 &&
             State == Hidden) {
    Show();
  }

  if (MenuTransition.IsOut() && State == Hiding)
    State = Hidden;
  else if (MenuTransition.IsIn() && State == Showing) {
    State = Shown;
  }

  if (State != Hidden) {
    MenuTransition.Update(dt);
    if (MenuTransition.Direction == -1.0f && MenuTransition.Progress <= 0.72f) {
      TitleFade.StartOut();
    } else if (MenuTransition.IsIn() &&
               (TitleFade.Direction == 1.0f || TitleFade.IsOut())) {
      TitleFade.StartIn();
    }
    TitleFade.Update(dt);
  }
}

inline void MovieMenu::DrawCircles() {
  float y = CircleStartPosition.y;
  int resetCounter = 0;
  // Give the whole range that mimics ScrWork[SW_SYSMENUCT] given that the
  // duration is totalframes/60
  float progress = MenuTransition.Progress * MenuTransitionDuration * 60.0f;
  for (int line = 0; line < 4; line++) {
    int counter = resetCounter;
    float x = CircleStartPosition.x;
    for (int col = 0; col < 7; col++) {
      if (counter + 1 <= (progress)) {
        float scale = ((progress) - (counter + 1.0f)) * 16.0f;
        scale = scale <= 320.0f ? scale : 320.0f;
        scale *= CircleSprite.Bounds.Height / 106.0f;
        Renderer2D::DrawSprite(
            CircleSprite, RectF(x + (CircleSprite.Bounds.Width - scale) / 2.0f,
                                y + (CircleSprite.Bounds.Height - scale) / 2.0f,
                                scale, scale));
        x += CircleOffset;
      }
      counter += 2;
    }
    y += CircleOffset;
    resetCounter += 2;
  }
}

inline void MovieMenu::DrawErin() {
  float y = ErinPosition.y;
  if (MenuTransition.Progress < 0.78f) {
    y = 801.0f;
    if (MenuTransition.Progress > 0.22f) {
      // Approximation from the original function, which was a bigger mess
      y = glm::mix(
          -19.0f, 721.0f,
          0.998938f -
              0.998267f * sin(3.97835f - 3.27549f * MenuTransition.Progress));
    }
  }
  Renderer2D::DrawSprite(ErinSprite, glm::vec2(ErinPosition.x, y));
}

inline void MovieMenu::DrawRedBar() {
  if (MenuTransition.IsIn()) {
    Renderer2D::DrawSprite(InitialRedBarSprite, InitialRedBarPosition);
  } else if (MenuTransition.Progress > 0.70f) {
    // Give the whole range that mimics ScrWork[SW_SYSMENUCT] given that the
    // duration is totalframes/60
    float progress = MenuTransition.Progress * MenuTransitionDuration * 60.0f;
    float pixelPerAdvanceLeft = RedBarBaseX * (progress - 47.0f) / 17.0f;
    RedBarSprite.Bounds.X = RedBarDivision - pixelPerAdvanceLeft;
    RedBarSprite.Bounds.Width = pixelPerAdvanceLeft;
    RedBarPosition.x = RedBarBaseX - pixelPerAdvanceLeft;
    Renderer2D::DrawSprite(RedBarSprite, RedBarPosition);
    float pixelPerAdvanceRight = 13.0f * (progress - 47.0f);
    RedBarSprite.Bounds.X = RedBarDivision;
    RedBarSprite.Bounds.Width = pixelPerAdvanceRight;
    RedBarPosition = RightRedBarPosition;
    Renderer2D::DrawSprite(RedBarSprite, RedBarPosition);
  }
}

}  // namespace CHLCC
}  // namespace UI
}  // namespace Impacto