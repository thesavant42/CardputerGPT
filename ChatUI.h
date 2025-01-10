#ifndef CHAT_UI_H
#define CHAT_UI_H

#include <M5Cardputer.h>
#include <vector>
#include "ChatMessage.h"

// A UI that:
//  - Clears screen for each step (user typing, "Thinking...", final assistant text)
//  - Does typed animation for assistant text
//  - Scrolls with `'` (up) and `/` (down)
//  - Captures user input, triggers userJustPressedEnter + lastEnteredText when done

class ChatUI {
public:
  ChatUI();

  // Provide an M5Canvas from .ino
  void init(M5Canvas* c);

  // We always clear the screen, then typed out the text
  void showAssistantMessage(const String &text);

  // Same, but for "Thinking..."
  void showThinking();

  // Handles scroll & typed chars. 
  // If Enter is pressed, sets userJustPressedEnter + lastEnteredText
  void handleKeyboardInput();

  // Used by main code after user pressed Enter
  bool userJustPressedEnter;  // set to true once in handleKeyboardInput
  String lastEnteredText;     // the typed text from user

private:
  M5Canvas* canvas;
  String screenContent;  // what's currently on the screen
  String userInput;      // ephemeral typed text
  int scrollOffset;
  int lineHeight;
  int margin;

  void clearScreen();
  void drawScreenContent();
  int getTotalHeight(const String &text);
  void typeText(const String &fullText, bool clearFirst);
  void scrollUp();
  void scrollDown();
};

#endif