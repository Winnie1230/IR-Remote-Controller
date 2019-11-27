import asyncio
from aiohttp import web
import paho.mqtt.client as mqtt
import json
import websockets

IP = "nckuwinnieliu.ddns.net"

'''-----MQTT Server Setting-----'''
MQTTServerPort = 1883

'''-----Websocket Port Setting-----'''
WebsocketPort = 8765

FILE_PATH = './public'
MAIN_INDEX = '/index.html'

PORT = 11230 #web port

'''-----Set TimeOut for Receiving Data from ESP8266-----'''
TIME_OUT = 10 #10 sec

'''-----Receive State-----'''
received = 0 #check if received remote controller state information
state = {'co2':'off' , 'pm':'off' , 'current':'off' , 'Temp':'off'}

async def LoginPage(request):
    return web.FileResponse(FILE_PATH + '/login.html')

async def Login(request):
    user = request.rel_url.query['username']
    password = request.rel_url.query['password']
    print("user:"+user+"  password:"+password)
    
    #verify username and password in json file
    with open('user.json','r') as f:
        user_dict = json.load(f)
    if (user in user_dict) and (user_dict[user] == password):
        print("ok")
        return web.Response(text="ok",content_type='text/html')
    else:
        print("no")
        return web.Response(text="no",content_type='text/html')

async def Send(request):
    #await asyncio.sleep(1)
    print("send")
    topic = request.rel_url.query['topic']
    message = request.rel_url.query['message']
    print("topic:" + topic + "  message:" + message)
    
    '''-----MQTT Publish-----'''
    mqttc.publish(topic,message)

    return web.Response(text="Data send",content_type='text/html')


async def ControllerState(request):
    print("executed")
    global received, state
    received = 1
    
    '''-----Update Remote Controller State-----'''
    state['co2'] = request.rel_url.query['co2']
    state['pm'] = request.rel_url.query['pm']
    state['current'] = request.rel_url.query['current']
    state['Temp'] = request.rel_url.query['temp']

    print(state)
    return web.Response(text="Hello World",content_type='text/html')


async def WebsocketHandler(websocket, path):
    '''-----Test Code-----'''
    print("connected")

    global received
    counter = 0
    while True:
        await asyncio.sleep(1)
        counter += 1
        if counter > TIME_OUT:
            await websocket.send("timeout")
            print("Time out and try again")
            break
        else:
            if received == 1:
                '''-----change dict to json format-----'''
                json_str = json.dumps(state)
                await websocket.send(json_str)
                print("send message")
                received = 0
                break

async def Search(request):
    print("search")
    search_num = request.rel_url.query['product']
    print("search_num:"+search_num)

    #verify product_number and on/off state in json file
    with open('product_num.json','r') as f:
        product_dict = json.load(f)
    if (search_num in product_dict):
        if(product_dict[search_num] == 'on'):
	    #print("find product and it's used'")
            return web.Response(text="used",content_type='text/html')
        else:
	    #print("find product but not used")
            return web.Response(text="not_used",content_type='text/html')
    else:
	#print("no product")
        return web.Response(text="no",content_type='text/html')

async def ToggleChange(request):
    print("togglechange");
    sensor = request.rel_url.query['sensor']
    sensor_state = request.rel_url.query['state']
    print("sensor: " + sensor + "  state: " + sensor_state)
    
    '''MQTT Publish'''
    mqttc.publish(sensor,sensor_state)
    return web.Response(text="change sent",content_type='text/html')


async def init(loop):
    app = web.Application()
    app.router.add_static('/public','./public')
    
    #receiving request
    app.router.add_get('/send',Send)
    app.router.add_get('/', LoginPage)
    app.router.add_get('/login', Login)
    app.router.add_get('/state', ControllerState)
    app.router.add_get('/search', Search)
    app.router.add_get('/togglechange',ToggleChange)
    srv = await loop.create_server(app._make_handler(),host='0.0.0.0', port=PORT)
    print("server created")
    return srv

if __name__ == '__main__':
    try:
        loop = asyncio.get_event_loop()

        '''-----MQTT Server-----'''
        mqttc = mqtt.Client("python_hub")
        mqttc.connect(IP, MQTTServerPort)

        '''-----Websocket serve create-----'''		
        websocket_serv = websockets.serve(WebsocketHandler,IP , WebsocketPort)

        print("initial received:"+str(received))

        loop.run_until_complete(init(loop))
        loop.run_until_complete(websocket_serv)

        loop.run_forever()
    except KeyboardInterrupt:
        pass
    finally:
        print("Closing Loop")
        loop.close()
