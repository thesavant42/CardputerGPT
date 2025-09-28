#ifndef OPENAI_H
#define OPENAI_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <vector>
#include "config.h"
#include "ChatMessage.h" // we only reference the struct here

// How many previous messages to include in ChatGPT prompt
#define MAX_HISTORY 5

// The system prompt and endpoint
static const char systemPrompt[] PROGMEM = 
  "You are a concise assistant. Provide answers in simple text, each no longer than 150 characters";
static const char endpoint[]     PROGMEM = 
  "http://192.168.1.98:1234/v1/chat/completions";

class OpenAI {
public:
  OpenAI() {}
  
  // Construct a request for the last MAX_HISTORY messages and parse the result
  // Return the GPT response text
  String getResponse(const std::vector<ChatMessage>& chatHistory);

private:
  // Build JSON request body
  String buildRequestBody(const std::vector<ChatMessage>& chatHistory);
};

#endif
