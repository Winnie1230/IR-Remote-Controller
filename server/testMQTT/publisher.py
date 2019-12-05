# Publisher.py
import paho.mqtt.client as mqtt

_g_cst_ToMQTTTopicServerIP = "winnieliu.ddns.net"
_g_cst_ToMQTTTopicServerPort = 1883 #port
_g_cst_MQTTTopicName = "qwe/APP/learn" #TOPIC name

mqttc = mqtt.Client("python_pub")
mqttc.connect(_g_cst_ToMQTTTopicServerIP, _g_cst_ToMQTTTopicServerPort)
mqttc.publish(_g_cst_MQTTTopicName, "111")
