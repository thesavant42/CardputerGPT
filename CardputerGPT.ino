#include <M5Cardputer.h>
#include "config.h"
#include "ChatMessage.h"
#include "OpenAI.h"
#include "ChatUI.h"

// We'll keep a global chatHistory
static std::vector<ChatMessage> chatHistory;
static OpenAI openAI;
static ChatUI chatUI;

inline void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // If you like, call M5Cardputer.update() or do something else
  }
}

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);
  M5Cardputer.Display.setRotation(1);

  // Connect WiFi
  connectWiFi();

  // Create canvas
  static M5Canvas canvas(&M5Cardputer.Display);
  canvas.createSprite(M5Cardputer.Display.width(), M5Cardputer.Display.height());
  canvas.setTextSize(2);

  chatUI.init(&canvas);

  // Show greeting
  chatUI.showAssistantMessage("Hi! How can I help you today?");
}

void loop() {
  // Update M5 hardware (including keyboard)
  M5Cardputer.update();

  // Check user typing or scrolling
  chatUI.handleKeyboardInput();

  // If user pressed Enter with some text, we send to GPT
  if (chatUI.userJustPressedEnter) {
    chatUI.userJustPressedEnter = false; // reset the flag

    // 1) Add user message to chatHistory
    ChatMessage userMsg;
    userMsg.role    = "user";
    userMsg.content = chatUI.lastEnteredText;
    chatHistory.push_back(userMsg);

    // 2) Show "Thinking..." typed out
    chatUI.showThinking();

    // 3) Get GPT response for the entire chatHistory
    String gptReply = openAI.getResponse(chatHistory);

    // 4) Add assistant message to chatHistory
    ChatMessage asstMsg;
    asstMsg.role    = "assistant";
    asstMsg.content = gptReply;
    chatHistory.push_back(asstMsg);

    // 5) Show the typed assistant message
    chatUI.showAssistantMessage(gptReply);
  }
}