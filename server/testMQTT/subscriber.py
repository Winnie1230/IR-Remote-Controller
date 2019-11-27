# Subscriber.py
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("aaa/web/SensorState")
    client.subscribe("aaa/web/#")
    client.subscribe("pm")
    client.subscribe("current")
    client.subscribe("temp")

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("nckuwinnieliu.ddns.net", 1883, 60)
client.loop_forever()
