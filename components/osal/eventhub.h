#ifndef _EVENTHUB_H
#define _EVENTHUB_H

#define TYPE_BIT(x)     (1u << x)

#define EVENTHUB_TYPE_NET_CONNECTED     TYPE_BIT(0)
#define EVENTHUB_TYPE_MQTT_START        TYPE_BIT(1)
#define EVENTHUB_TYPE_GUI_START         TYPE_BIT(2)

typedef void (*eventhub_callback_f)(int type);

int eventhub_init(void);
int eventhub_subscribe(eventhub_callback_f cb);
int eventhub_publish(int type, int from_isr);
#endif
