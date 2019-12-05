# Subscriber.py
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("qwe/Web/SensorState")
    client.subscribe("qwe/Web/#")
    client.subscribe("qwe/APP/#")
    client.subscribe("qwe/#")
    client.subscribe("test/#")
    client.subscribe("temp")

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("winnieliu.ddns.net", 1883, 60)
client.loop_forever()
