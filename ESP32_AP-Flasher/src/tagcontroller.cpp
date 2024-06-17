#include "tagcontroller.h"
#include "web.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>

void tagcontrollerUpdate() {
    if (config.runStatus == RUNSTATUS_STOP) return;
    if (config.tagControllerUrl.isEmpty()) return;

    String tags = tagDBtoJson();

    HTTPClient http;
    http.begin(config.tagControllerUrl);
    http.setTimeout(1000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    const int httpCode = http.POST(tags);
    if (httpCode != 200) {
        http.end();
        wsErr(String("[httpGetJson] http ") + config.tagControllerUrl + " code " + httpCode);
        return;
    }

    DeserializationError error;
    DynamicJsonDocument json(1000);
    error = deserializeJson(json, http.getString());
    http.end();
    if (error) {
        Serial.printf("[httpGetJson] JSON: %s\r\n", error.c_str());
        wsErr("[httpGetJson] JSON: " + String(error.c_str()));
        return;
    }
}
