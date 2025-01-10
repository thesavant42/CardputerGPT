#include "ChatUI.h"

ChatUI::ChatUI() {
  canvas              = nullptr;
  screenContent       = "";
  userInput           = "";
  scrollOffset        = 0;
  lineHeight          = 32;
  margin              = 3;
  userJustPressedEnter= false;
  lastEnteredText     = "";
}

void ChatUI::init(M5Canvas* c) {
  canvas = c;
}

void ChatUI::clearScreen() {
  if (!canvas) return;
  canvas->fillSprite(TFT_BLACK);
  canvas->pushSprite(0, 0);

  scrollOffset  = 0;
  screenContent = "";
}

// Count lines based on '\n'
int ChatUI::getTotalHeight(const String &text) {
  if (text.isEmpty()) return lineHeight;
  int lines = 1;
  for (int i=0; i<(int)text.length(); i++) {
    if (text[i] == '\n') lines++;
  }
  return lines * lineHeight;
}

// Render screenContent with current scroll offset
void ChatUI::drawScreenContent() {
  if (!canvas) return;
  canvas->fillSprite(TFT_BLACK);

  int y = -scrollOffset;
  String line;
  for (int i=0; i < (int)screenContent.length(); i++) {
    char c = screenContent[i];
    if (c == '\n') {
      canvas->setCursor(margin, y);
      canvas->println(line);
      line.clear();
      y += lineHeight;
    } else {
      line += c;
    }
  }
  if (!line.isEmpty()) {
    canvas->setCursor(margin, y);
    canvas->println(line);
  }

  canvas->pushSprite(0, 0);
}

// Type out text char by char, optionally clearing first
void ChatUI::typeText(const String &fullText, bool clearFirst) {
  if (!canvas) return;
  if (clearFirst) {
    clearScreen();
  }
  screenContent = ""; // start fresh

  for (int i=0; i < (int)fullText.length(); i++) {
    screenContent += fullText[i];

    // If it exceeds screen, scroll down
    int totalH = getTotalHeight(screenContent);
    int screenH = canvas->height();
    if (totalH > screenH) {
      scrollOffset = totalH - screenH;
    }
    drawScreenContent();
    delay(40); // typing speed
  }
}

void ChatUI::showAssistantMessage(const String &text) {
  typeText(text, /*clearFirst=*/true);
}

void ChatUI::showThinking() {
  typeText("Thinking...", /*clearFirst=*/true);
}

void ChatUI::scrollUp() {
  scrollOffset -= lineHeight;
  if (scrollOffset < 0) scrollOffset = 0;
  drawScreenContent();
}

void ChatUI::scrollDown() {
  int totalH = getTotalHeight(screenContent);
  int screenH = canvas->height();
  int maxOffset = (totalH > screenH) ? (totalH - screenH) : 0;
  scrollOffset += lineHeight;
  if (scrollOffset > maxOffset) scrollOffset = maxOffset;
  drawScreenContent();
}

// The main keyboard handling: scroll, typed input, backspace, Enter
void ChatUI::handleKeyboardInput() {
  if (!canvas) return;
  if (!M5Cardputer.Keyboard.isChange() || !M5Cardputer.Keyboard.isPressed()) return;

  auto ks = M5Cardputer.Keyboard.keysState();

  // 1) Scroll
  for (auto c : ks.word) {
    if (c == '\'') {
      scrollUp();
      return;
    } else if (c == '/') {
      scrollDown();
      return;
    }
  }

  bool typedSomething = false;
  // 2) Normal chars
  for (auto c : ks.word) {
    if (c != '\'' && c != '/') {
      userInput += c;
      typedSomething = true;
    }
  }
  // 3) Backspace
  if (ks.del && !userInput.isEmpty()) {
    userInput.remove(userInput.length() - 1);
    typedSomething = true;
  }

  // If user typed, we show just "> userInput"
  if (typedSomething) {
    clearScreen();
    screenContent = "> " + userInput;
    int totalH = getTotalHeight(screenContent);
    int screenH = canvas->height();
    if (totalH > screenH) {
      scrollOffset = totalH - screenH;
    } else {
      scrollOffset = 0;
    }
    drawScreenContent();
  }

  // 4) Enter => finalize
  if (ks.enter) {
    userInput.trim();
    if (!userInput.isEmpty()) {
      lastEnteredText = userInput;
      userJustPressedEnter = true;
    }
    userInput = "";
  }
}