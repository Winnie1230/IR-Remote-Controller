import asyncio
from aiohttp import web
import paho.mqtt.client as mqtt

'''-----MQTT Server Setting-----'''
MQTTServerIP = "nckuwinnieliu.ddns.net"
MQTTServerPort = 1883

FILE_PATH = './public'
MAIN_INDEX = '/index.html'

PORT = 11230 #web port

async def Send(request):
    #await asyncio.sleep(1)
    print("send")
    topic = request.rel_url.query['topic']
    message = request.rel_url.query['message']
    print("topic:" + topic + "  message:" + message)
    
    '''-----MQTT Publish-----'''
    mqttc.publish(topic,message)

    return web.Response(text="Hello World",content_type='text/html')
   
async def init(loop):
    app = web.Application()
    app.router.add_static('/public','./public')
    
    #receiving request
    app.router.add_get('/send',Send)
    #app.router.add_get('/drying',ClothDrying)
    #app.router.add_get('/gathering',ClothGathering)
    srv = await loop.create_server(app._make_handler(),host='0.0.0.0', port=PORT)
    print("server created")
    return srv

if __name__ == '__main__':
    try:
        loop = asyncio.get_event_loop()

        '''-----MQTT Server-----'''
        mqttc = mqtt.Client("python_hub")
        mqttc.connect(MQTTServerIP, MQTTServerPort)
        
        loop.run_until_complete(init(loop))
        loop.run_forever()
    except KeyboardInterrupt:
        pass
    finally:
        print("Closing Loop")
        loop.close()
