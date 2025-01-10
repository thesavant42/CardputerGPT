#include "OpenAI.h"

String OpenAI::buildRequestBody(const std::vector<ChatMessage>& chatHistory) {
  String requestBody = "{\"model\":\"gpt-4o-mini\",\"messages\":["
                       "{\"role\":\"system\",\"content\":\"" + String(systemPrompt) + "\"}";

  // Only take the last MAX_HISTORY from chatHistory
  int startIndex = chatHistory.size() - MAX_HISTORY;
  if (startIndex < 0) startIndex = 0;

  for (int i = startIndex; i < (int)chatHistory.size(); i++) {
    const ChatMessage &msg = chatHistory[i];
    requestBody += ",{\"role\":\"" + msg.role + "\",\"content\":\"" + msg.content + "\"}";
  }
  requestBody += "]}";
  
  return requestBody;
}

String OpenAI::getResponse(const std::vector<ChatMessage>& chatHistory) {
  if (WiFi.status() != WL_CONNECTED) {
    return "No WiFi connection.";
  }

  String requestBody = buildRequestBody(chatHistory);

  HTTPClient http;
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(openaiKey));

  int httpCode = http.POST(requestBody);
  if (httpCode <= 0) {
    http.end();
    return "HTTP Error: " + String(httpCode);
  }

  String payload = http.getString();
  http.end();

  // Parse JSON
  StaticJsonDocument<JSON_DOC_SIZE> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    return "JSON parsing error.";
  }

  // Extract the text from "choices[0].message.content"
  JsonArray choices = doc["choices"].as<JsonArray>();
  if (!choices.isNull() && choices.size() > 0) {
    return choices[0]["message"]["content"].as<String>();
  }
  
  return "No response from ChatGPT.";
}