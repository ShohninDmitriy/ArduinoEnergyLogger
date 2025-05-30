// wifi
#include <ESP8266WiFi.h>
// post
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#include "post.h"
// openssl s_client -connect api.thingspeak.com:443 | openssl x509 -fingerprint -noout
// const uint8_t fingerprint[20] = {0x27, 0x18, 0x92, 0xDD, 0xA4, 0x26, 0xC3, 0x07, 0x09, 0xB9, 0x7A, 0xE6, 0xC5, 0x21, 0xB9, 0x5B, 0x48, 0xF7, 0x16, 0xE1};
uint16_t post_error = 0;
uint16_t wifi_error = 0;

void postData(const String &data)
{
    int ret = 0;
    // https
    // std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    WiFiClientSecure client;
    HTTPClient http;

    if (WiFi.status() == WL_CONNECTED)
    {
        // https insecure
        // client.setFingerprint(fingerprint);
        client.setInsecure();

        if (http_timeout > 0)
            http.setTimeout(http_timeout); // ms
        if (http.begin(client, api_url))
        {
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");

            String postStr = "api_key=" + String(api_key) + data + "\n\n";
            int httpCode = http.POST(postStr);

            if (httpCode == HTTP_CODE_OK)
            {
                // Serial.printf("[HTTP] POST... code: %d\n", httpCode);
                String payload = http.getString();
                // Serial.println("received payload:\n<<");
                // Serial.println(payload);
                // Serial.println(">>");

                if (payload != "1")
                {
                    if (post_error < 65500)
                        post_error++;
                }
                // Serial.print("error: ");
                // Serial.println(error);
            }
            else
            {
                // Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
                if (post_error < 65500)
                    post_error++;
                // http_code=httpCode;
            }
            http.end();
        }
    }
    else
    {
        if (wifi_error < 65500)
            wifi_error++;
    }
}