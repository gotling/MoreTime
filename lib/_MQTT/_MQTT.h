#ifndef _MQTT_H_
#define _MQTT_H_

extern void mqttSetup();
extern void mqttLoop();

void mqttPublish();
void connect();

#endif