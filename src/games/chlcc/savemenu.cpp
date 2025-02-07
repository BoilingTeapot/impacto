#include "savemenu.h"

#include "../../profile/ui/savemenu.h"
#include "../../profile/games/chlcc/savemenu.h"
#include "../../renderer2d.h"
#include "../../mem.h"
#include "../../profile/scriptvars.h"
#include "../../inputsystem.h"
#include "../../ui/widgets/button.h"
#include "../../ui/widgets/chlcc/saveentrybutton.h"
#include "../../data/savesystem.h"
#include "../../vm/vm.h"

namespace Impacto {
namespace UI {
namespace CHLCC {

using namespace Impacto::Profile::SaveMenu;
using namespace Impacto::Profile::CHLCC::SaveMenu;
using namespace Impacto::Profile::ScriptVars;

using namespace Impacto::UI::Widgets::CHLCC;

Widget* EntryGrid[EntriesPerPage];

void SaveMenu::MenuButtonOnClick(Widgets::Button* target) {
  if ((SaveSystem::GetSaveSatus(EntryType, target->Id) != 0) ||
      ScrWork[SW_SAVEMENUMODE] == 1) {
    ScrWork[SW_SAVEFILENO] = target->Id;
    ChoiceMade = true;
  }
}

SaveMenu::SaveMenu() {
  MenuTransition.Direction = 1;
  MenuTransition.LoopMode = ALM_Stop;
  MenuTransition.DurationIn = FadeInDuration;
  MenuTransition.DurationOut = FadeOutDuration;

  TitleFade.Direction = 1.0f;
  TitleFade.LoopMode = ALM_Stop;
  TitleFade.DurationIn = TitleFadeInDuration;
  TitleFade.DurationOut = TitleFadeOutDuration;

  SelectDataTextFade.Direction = 1;
  SelectDataTextFade.LoopMode = ALM_Loop;
  SelectDataTextFade.DurationIn = SelectDataFadeDuration;

  RedBarSprite = InitialRedBarSprite;
  RedBarPosition = InitialRedBarPosition;

  auto onClick =
      std::bind(&SaveMenu::MenuButtonOnClick, this, std::placeholders::_1);

  // Quick Save Pages initialization

  for (int i = 0; i < Pages; i++) {
    MainItems = new Widgets::Group(this);
    MainItems->WrapFocus = false;

    for (int j = 0; j < EntriesPerPage; j++) {
      SaveEntryButton* saveEntryButton =
          new SaveEntryButton(i * EntriesPerPage + j, QuickLoadEntrySprite,
                              EntryHighlightedSprite, QuickLoadEntrySprite,
                              EntryPositions[j], i, false, LockedSymbolSprite);
      saveEntryButton->OnClickHandler = onClick;
      saveEntryButton->AddThumbnail(EmptyThumbnailSprite, ThumbnailRelativePos);
      MainItems->Add(saveEntryButton);
      EntryGrid[j] = saveEntryButton;
    }

    // Yes I know this is bad...
    for (int k = 0; k < EntriesPerPage - 2; k++) {
      if (k % 3 == 0) {
        EntryGrid[k]->SetFocus(EntryGrid[(k % 2) + 4], FDIR_RIGHT);
        EntryGrid[k + 1]->SetFocus(EntryGrid[(k % 2) + 4], FDIR_RIGHT);
        EntryGrid[(k % 2) + 4]->SetFocus(EntryGrid[k], FDIR_LEFT);
      }
      EntryGrid[k]->SetFocus(EntryGrid[(k + 1) % 4], FDIR_DOWN);
      EntryGrid[(k + 1) % 4]->SetFocus(EntryGrid[k], FDIR_UP);
    }
    EntryGrid[4]->SetFocus(EntryGrid[5], FDIR_DOWN);
    EntryGrid[4]->SetFocus(EntryGrid[5], FDIR_UP);
    EntryGrid[5]->SetFocus(EntryGrid[4], FDIR_UP);
    EntryGrid[5]->SetFocus(EntryGrid[4], FDIR_DOWN);

    QuickSavePages.push_back(MainItems);
  }
  // Maintaining focus across pages
  for (auto pageItr = QuickSavePages.begin(); pageItr != QuickSavePages.end();
       pageItr++) {
    auto prevItr = pageItr;
    auto nextItr = pageItr;
    // Page 1 leads to page 8 to the left
    if (pageItr == QuickSavePages.begin()) {
      prevItr = QuickSavePages.end();
    }
    prevItr--;
    // Page 8 leads to page 1 to the left
    if (pageItr == --QuickSavePages.end()) {
      nextItr = QuickSavePages.begin();
    } else {
      nextItr++;
    }
    Widgets::Group* curr = *pageItr;
    Widgets::Group* prev = *prevItr;
    Widgets::Group* next = *nextItr;
    for (int k = 0; k < EntriesPerPage - 2; k++) {
      curr->Children[k]->SetFocus(prev->Children[4 + k / 2], FDIR_LEFT);
    }
    curr->Children[4]->SetFocus(next->Children[0], FDIR_RIGHT);
    curr->Children[5]->SetFocus(next->Children[3], FDIR_RIGHT);
  }

  // Full Save Pages initialization

  for (int i = 0; i < Pages; i++) {
    MainItems = new Widgets::Group(this);
    MainItems->WrapFocus = false;

    for (int j = 0; j < EntriesPerPage; j++) {
      SaveEntryButton* saveEntryButton = new SaveEntryButton(
          i * EntriesPerPage + j, SaveEntrySprite, EntryHighlightedSprite,
          SaveEntrySprite, EntryPositions[j], i, true, LockedSymbolSprite);
      saveEntryButton->OnClickHandler = onClick;
      MainItems->Add(saveEntryButton);
      EntryGrid[j] = saveEntryButton;
    }

    // Yes I know this is bad...
    for (int k = 0; k < EntriesPerPage - 2; k++) {
      if (k % 3 == 0) {
        EntryGrid[k]->SetFocus(EntryGrid[(k % 2) + 4], FDIR_RIGHT);
        EntryGrid[k + 1]->SetFocus(EntryGrid[(k % 2) + 4], FDIR_RIGHT);
        EntryGrid[(k % 2) + 4]->SetFocus(EntryGrid[k], FDIR_LEFT);
      }
      EntryGrid[k]->SetFocus(EntryGrid[(k + 1) % 4], FDIR_DOWN);
      EntryGrid[(k + 1) % 4]->SetFocus(EntryGrid[k], FDIR_UP);
    }
    EntryGrid[4]->SetFocus(EntryGrid[5], FDIR_DOWN);
    EntryGrid[4]->SetFocus(EntryGrid[5], FDIR_UP);
    EntryGrid[5]->SetFocus(EntryGrid[4], FDIR_UP);
    EntryGrid[5]->SetFocus(EntryGrid[4], FDIR_DOWN);

    FullSavePages.push_back(MainItems);
  }

  // Maintaining focus across pages
  for (auto pageItr = FullSavePages.begin(); pageItr != FullSavePages.end();
       pageItr++) {
    auto prevItr = pageItr;
    auto nextItr = pageItr;
    // Page 1 leads to page 8 to the left
    if (pageItr == FullSavePages.begin()) {
      prevItr = FullSavePages.end();
    }
    prevItr--;
    // Page 8 leads to page 1 to the right
    if (pageItr == --FullSavePages.end()) {
      nextItr = FullSavePages.begin();
    } else {
      nextItr++;
    }
    Widgets::Group* curr = *pageItr;
    Widgets::Group* prev = *prevItr;
    Widgets::Group* next = *nextItr;
    for (int k = 0; k < EntriesPerPage - 2; k++) {
      curr->Children[k]->SetFocus(prev->Children[4 + k / 2], FDIR_LEFT);
    }
    curr->Children[4]->SetFocus(next->Children[0], FDIR_RIGHT);
    curr->Children[5]->SetFocus(next->Children[3], FDIR_RIGHT);
  }
}

void SaveMenu::UpdateEntry(SaveEntryButton* saveEntryButton) {
  Sprite entrySprite;
  switch (ScrWork[SW_SAVEMENUMODE]) {
    case 0:
      entrySprite = QuickLoadEntrySprite;
      break;
    case 1:
      entrySprite = SaveEntrySprite;
      break;
    case 2:
      entrySprite = LoadEntrySprite;
      break;
  }
  int idx = saveEntryButton->Id;
  uint8_t lock = SaveSystem::GetSaveFlags(EntryType, idx);
  saveEntryButton->IsLocked = lock == 1;
  saveEntryButton->AddNormalSpriteLabel(entrySprite, EntryPositions[idx % 6]);
  saveEntryButton->AddEntryNumberHintText(
      Vm::ScriptGetTextTableStrAddress(0, 6), 18, true,
      EntryNumberHintTextRelativePos);
  char tempNo[3];
  sprintf(tempNo, "%02d", idx + 1);
  saveEntryButton->AddEntryNumberText(std::string(tempNo), 18, true,
                                      EntryNumberTextRelativePos);
  saveEntryButton->AddThumbnail(EmptyThumbnailSprite,
                                EntryPositions[idx % 6] + ThumbnailRelativePos);
  if (SaveSystem::GetSaveSatus(EntryType, idx) != 0) {
    saveEntryButton->EntryActive = true;
    saveEntryButton->AddSceneTitleText(
        Vm::ScriptGetTextTableStrAddress(
            1, SaveSystem::GetSaveTitle(EntryType, idx)),
        24, true, SceneTitleTextRelativePos, NoDataTextRelativePos);
    saveEntryButton->AddPlayTimeHintText(Vm::ScriptGetTextTableStrAddress(0, 2),
                                         18, true, PlayTimeHintTextRelativePos);
    uint32_t time = SaveSystem::GetSavePlayTime(EntryType, idx);
    uint32_t hours = time / 3600;
    uint32_t minutes = (time % 3600) / 60;
    uint32_t seconds = (time % 3600) % 60;
    char temp[10];
    sprintf(temp, "%3d:%02d:%02d", hours, minutes, seconds);
    saveEntryButton->AddPlayTimeText(std::string(temp), 18, true,
                                     PlayTimeTextRelativePos);
    saveEntryButton->AddSaveDateHintText(Vm::ScriptGetTextTableStrAddress(0, 3),
                                         18, true, SaveDateHintTextRelativePos);
    tm date = SaveSystem::GetSaveDate(EntryType, idx);
    char tempDate[20];
    sprintf(tempDate, "%4d/%2d/%2d %02d:%02d:%02d", date.tm_year, date.tm_mon,
            date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
    saveEntryButton->AddSaveDateText(std::string(tempDate), 18, true,
                                     SaveDateTextRelativePos);
  } else {
    saveEntryButton->AddSceneTitleText(Vm::ScriptGetTextTableStrAddress(0, 1),
                                       24, true, SceneTitleTextRelativePos,
                                       NoDataTextRelativePos);
  }
}

void SaveMenu::Show() {
  if (State != Shown) {
    switch (ScrWork[SW_SAVEMENUMODE]) {
      case 0:
        EntryType = SaveSystem::SaveQuick;
        SavePages = &QuickSavePages;
        BackgroundColor = QuickLoadBackgroundColor;
        CircleSprite = QuickLoadCircle;
        MenuTitleTextSprite = QuickLoadTextSprite;
        CurrentPage = &CurrentQuickSavePage;
        break;
      case 1:
        EntryType = SaveSystem::SaveFull;
        SavePages = &FullSavePages;
        BackgroundColor = SaveBackgroundColor;
        CircleSprite = SaveCircle;
        MenuTitleTextSprite = SaveTextSprite;
        CurrentPage = &CurrentFullSavePage;
        break;
      case 2:
        EntryType = SaveSystem::SaveFull;
        SavePages = &FullSavePages;
        BackgroundColor = LoadBackgroundColor;
        CircleSprite = LoadCircle;
        MenuTitleTextSprite = LoadTextSprite;
        CurrentPage = &CurrentFullSavePage;
        break;
    }

    for (auto mainItems : *SavePages) {
      mainItems->Bounds = RectF(0.0f, 0.0f, 1280.0f, 720.0f);
      for (auto widget : mainItems->Children) {
        auto saveEntryButton = static_cast<SaveEntryButton*>(widget);
        UpdateEntry(saveEntryButton);
      }
    }

    State = Showing;
    MenuTransition.StartIn();
    SelectDataTextFade.StartIn();
    SavePages->at(*CurrentPage)->Show();
    CurrentlyFocusedElement = SavePages->at(*CurrentPage)->Children[0];
    SavePages->at(0)->Children[0]->HasFocus = true;
    if (UI::FocusedMenu != 0) {
      LastFocusedMenu = UI::FocusedMenu;
      LastFocusedMenu->IsFocused = false;
    }
    IsFocused = true;
    UI::FocusedMenu = this;
  }
}
void SaveMenu::Hide() {
  if (State != Hidden) {
    State = Hiding;
    SaveEntryButton::FocusedAlphaFadeReset();
    MenuTransition.StartOut();
    if (LastFocusedMenu != 0) {
      UI::FocusedMenu = LastFocusedMenu;
      LastFocusedMenu->IsFocused = true;
    } else {
      UI::FocusedMenu = 0;
    }
    IsFocused = false;
  }
}

void SaveMenu::Update(float dt) {
  UpdateInput();
  if (ScrWork[SW_SYSMENUCT] < 10000 && State == Shown) {
    Hide();
  } else if (GetFlag(SF_SAVEMENU) && ScrWork[SW_SYSMENUCT] > 0 &&
             State == Hidden) {
    Show();
  }

  if (ScrWork[SW_SYSMENUCT] == 0 && State == Hiding) {
    State = Hidden;
    SavePages->at(*CurrentPage)->Hide();
  } else if (ScrWork[SW_SYSMENUCT] == 10000 && State == Showing) {
    State = Shown;
    SaveEntryButton::FocusedAlphaFadeStart();
  }

  if (State != Hidden) {
    MenuTransition.Update(dt);
    SelectDataTextFade.Update(dt);
    if (MenuTransition.Direction == -1.0f && MenuTransition.Progress <= 0.72f) {
      TitleFade.StartOut();
    } else if (MenuTransition.IsIn() &&
               (TitleFade.Direction == 1.0f || TitleFade.IsOut())) {
      TitleFade.StartIn();
    }
    TitleFade.Update(dt);
    SavePages->at(*CurrentPage)->Update(dt);
    SaveEntryButton::UpdateFocusedAlphaFade(dt);
    auto currentlyFocusedButton =
        static_cast<SaveEntryButton*>(CurrentlyFocusedElement);
    if (currentlyFocusedButton) {
      int newPage = currentlyFocusedButton->GetPage();
      if (newPage != *CurrentPage) {
        SavePages->at(*CurrentPage)->Hide();
        *CurrentPage = newPage;
        SavePages->at(*CurrentPage)->Show();
        currentlyFocusedButton->HasFocus = true;
        CurrentlyFocusedElement = currentlyFocusedButton;
      }
    }
  }
}

void SaveMenu::Render() {
  if (State != Hidden && ScrWork[SW_FILEALPHA] != 0) {
    glm::vec3 tint(1.0f);
    glm::vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
    if (MenuTransition.IsIn()) {
      Renderer2D::DrawRect(RectF(0.0f, 0.0f, 1280.0f, 720.0f),
                           RgbIntToFloat(BackgroundColor));
    } else {
      DrawCircles();
    }
    DrawErin();
    float alpha =
        MenuTransition.Progress < 0.5f ? MenuTransition.Progress * 2.0f : 1.0f;
    Renderer2D::DrawSprite(BackgroundFilter, RectF(0.0f, 0.0f, 1280.0f, 720.0f),
                           glm::vec4(tint, alpha));
    DrawRedBar();
    DrawTitles();
    float yOffset = 0;
    if (MenuTransition.Progress > 0.22f) {
      if (MenuTransition.Progress < 0.72f) {
        // Approximated function from the original, another mess
        yOffset = glm::mix(
            -720.0f, 0.0f,
            1.00397f * std::sin(3.97161f - 3.26438f * MenuTransition.Progress) -
                0.00295643f);
      }
      SavePages->at(*CurrentPage)->MoveTo(glm::vec2(0, yOffset));
      SavePages->at(*CurrentPage)->Tint = col;
      SavePages->at(*CurrentPage)->Render();
      Renderer2D::DrawSprite(SaveListSprite,
                             SaveListPosition + glm::vec2(0, yOffset));
      DrawPageNumber(yOffset);
      DrawButtonPrompt();
      DrawSelectData(yOffset);
    }
  }
}

inline void SaveMenu::DrawCircles() {
  float y = CircleStartPosition.y;
  int resetCounter = 0;
  // Give the whole range that mimics ScrWork[SW_SYSMENUCT] given that the
  // duration is totalframes/60
  float progress = MenuTransition.Progress * FadeInDuration * 60.0f;
  for (int line = 0; line < 4; line++) {
    int counter = resetCounter;
    float x = CircleStartPosition.x;
    for (int col = 0; col < 7; col++) {
      if (counter + 1 <= (progress)) {
        float scale = ((progress) - (counter + 1.0f)) * 16.0f;
        scale = scale <= 320.0f ? scale : 320.0f;
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
};

inline void SaveMenu::DrawErin() {
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

inline void SaveMenu::DrawRedBar() {
  if (MenuTransition.IsIn()) {
    Renderer2D::DrawSprite(InitialRedBarSprite, InitialRedBarPosition);
  } else if (MenuTransition.Progress > 0.70f) {
    // Give the whole range that mimics ScrWork[SW_SYSMENUCT] given that the
    // duration is totalframes/60
    float progress = MenuTransition.Progress * FadeInDuration * 60.0f;
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

inline void SaveMenu::DrawTitles() {
  if (MenuTransition.Progress > 0.34f) {
    float labelX = RedBarLabelPosition.x;
    float labelY = RedBarLabelPosition.y;
    float rightTitleX = MenuTitleTextRightPos.x;
    float rightTitleY = MenuTitleTextRightPos.y -
                        (MenuTitleTextSprite.Bounds.Height + 2.0f) / 2.0f;
    float leftTitleY = glm::mix(
        1.0f, 721.0f,
        1.01011f * sin(1.62223f * TitleFade.Progress + 3.152f) + 1.01012f);
    if (MenuTransition.Progress < 0.72f) {
      labelX -= 572.0f * (MenuTransition.Progress * 4.0f - 3.0f);
      rightTitleX -= 572.0f * (MenuTransition.Progress * 4.0f - 3.0f);
      labelY += 460.0f * (MenuTransition.Progress * 4.0f - 3.0f) / 3.0f;
      rightTitleY += 460.0f * (MenuTransition.Progress * 4.0f - 3.0f) / 3.0f;
    }
    Renderer2D::DrawSprite(RedBarLabel, glm::vec2(labelX, labelY));
    Renderer2D::DrawSprite(MenuTitleTextSprite,
                           glm::vec2(rightTitleX, rightTitleY), glm::vec4(1.0f),
                           glm::vec2(1.0f), MenuTitleTextAngle);
    Renderer2D::DrawSprite(MenuTitleTextSprite,
                           glm::vec2(MenuTitleTextLeftPos.x, leftTitleY));
  }
}

inline void SaveMenu::DrawPageNumber(float yOffset) {
  Renderer2D::DrawSprite(
      PageNumBackgroundSprite,
      glm::vec2(PageNumBackgroundPos.x, PageNumBackgroundPos.y + yOffset));
  Renderer2D::DrawSprite(
      BigDigits[*CurrentPage + 1],
      glm::vec2(CurrentPageNumPos.x, CurrentPageNumPos.y + yOffset));
  Renderer2D::DrawSprite(PageNumSeparatorSlashSprite,
                         glm::vec2(PageNumSeparatorSlashPos.x,
                                   PageNumSeparatorSlashPos.y + yOffset));
  Renderer2D::DrawSprite(MaxPageNumSprite,
                         glm::vec2(MaxPageNumPos.x, MaxPageNumPos.y + yOffset));
}

inline void SaveMenu::DrawButtonPrompt() {
  if (MenuTransition.IsIn()) {
    Renderer2D::DrawSprite(ButtonPromptSprite, ButtonPromptPosition);
  } else if (MenuTransition.Progress > 0.734f) {
    float x = ButtonPromptPosition.x - 2560.0f * (MenuTransition.Progress - 1);
    Renderer2D::DrawSprite(ButtonPromptSprite,
                           glm::vec2(x, ButtonPromptPosition.y));
  }
}

inline void SaveMenu::DrawSelectData(float yOffset) {
  float alpha;
  for (int idx = 0; idx < 10; idx++) {
    alpha = 1.0f;
    if (SelectDataTextFade.Progress < 0.046f * (idx + 1)) {
      alpha = (SelectDataTextFade.Progress - 0.046f * idx) / 0.046f;
    }
    Renderer2D::DrawSprite(SelectDataTextSprites[idx],
                           glm::vec2(SelectDataTextPositions[idx].x,
                                     SelectDataTextPositions[idx].y + yOffset),
                           glm::vec4(glm::vec3(1.0f), alpha));
  }
}

}  // namespace CHLCC
}  // namespace UI
}  // namespace Impacto