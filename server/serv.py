import asyncio
from aiohttp import web
import paho.mqtt.client as mqtt
import json
import websockets

IP = "winnieliu.ddns.net"

'''-----MQTT Server Setting-----'''
MQTTServerPort = 1883

'''-----Websocket Port Setting-----'''
WebsocketPort = 8765

FILE_PATH = './public'
MAIN_INDEX = '/index.html'

PORT = 11230 #web port

'''-----Set TimeOut for Receiving Data from ESP8266-----'''
TIME_OUT = 5 #10 sec

'''-----Websocket return data type-----'''
websocket_data_type = 0
#websocket_data_type = 1 -> return sensor state
#websocket_data_type = 2 -> reuturn initial check result

'''-----Search Product Num Result-----'''
search_result = 0
#search_result = 0 -> product number is used
#search_result = 1 -> product number is in the list but it is not used
#search_result = 2 -> product number is not in the list

'''-----Receive State-----'''
received = 0 #check if received remote controller state information
timeout = 0
connected = 0 #check if websocket is connected
#if timeout and received controller state information, then the state cannot be saved
state = {'co2':'off' , 'pm2.5':'off' , 'pm10':'off' , 'hcho':'off' , 'tvoc':'off' , 'humid':'off' , 'temp':'off' , 'current':'off'}
initial_result = 0
#initial_result = 1 -> sucess
#initial_result = 0 -> error

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
    product_num = request.rel_url.query['product']
    sensor = request.rel_url.query['sensor']
    state[sensor] = request.rel_url.query['change']   
    #state_change = 1 => turn on sensor
    #state_change = 0 => turn off sensor

    print("product: "+ product_num + "  sensor:" +sensor+ "  state:"+state[sensor])
    
    message = ''

    sensor = ['co2','pm2.5','pm10','hcho','tvoc','humid','temp','current']
    for i in sensor:
        message += '1' if state[i] == 'on' else '0'
    print("message:", message)
    
    '''-----MQTT Publish-----'''
    mqttc.reconnect()
    mqttc.publish(product_num+"/Web/SensorChange",message)
    #product_number/web/sensor

    return web.Response(text="Change sent",content_type='text/html')


async def ControllerState(request):
    print("controllerstate")

    '''
    print(request.rel_url.query['co2'])
    print(request.rel_url.query['pm2.5'])
    print(request.rel_url.query['pm10'])
    print(request.rel_url.query['hcho'])
    print(request.rel_url.query['tvoc'])
    print(request.rel_url.query['humid'])
    print(request.rel_url.query['temp'])
    print(request.rel_url.query['current'])
    '''

    if timeout or (not connected):
        return web.Response(text="Timeout or Websocket is not connected",content_type='text/html')
    else:
        global received, state, websocket_data_type
        received = 1
        websocket_data_type = 1

        '''-----Update Remote Controller State-----'''
        #state = "1" if state == "on" else "0"
        state['co2'] = 'on' if request.rel_url.query['co2'] == '1' else 'off'
        state['pm2.5'] = 'on' if request.rel_url.query['pm2.5'] == '1' else 'off'
        state['pm10'] = 'on' if request.rel_url.query['pm10'] == '1' else 'off'
        state['hcho'] = 'on' if request.rel_url.query['hcho'] == '1' else 'off'
        state['tvoc'] = 'on' if request.rel_url.query['tvoc'] == '1' else 'off'
        state['humid'] = 'on' if request.rel_url.query['humid'] == '1' else 'off'
        state['temp'] = 'on' if request.rel_url.query['temp'] == '1' else 'off'
        state['current'] = 'on' if request.rel_url.query['current'] == '1' else 'off'

        print(state)
        return web.Response(text="State Received",content_type='text/html')


async def WebsocketHandler(websocket, path):
    '''-----Test Code-----'''
    print("connected")

    global received, timeout, connected, websocket_data_type
    timeout = 0
    counter = 0
    connected = 1 #this handler function is executed when websocket is connected
    websocket_data_type = 0

    while True:
        if search_result == 1 or search_result == 2:
            print("Connection is closed")
            connected = 0
            received = 0
            break
        else: #search_result = 0
            await asyncio.sleep(1)
            counter += 1
            #print(counter)
            if counter > TIME_OUT:
                await websocket.send("timeout")
                print("Time out and try again")
                timeout = 1
                received = 0
                connected = 0
                break
            else:
                if received == 1:
                    if websocket_data_type == 1:
                        #-----change dict to json format-----
                        json_str = json.dumps(state)
                        await websocket.send(json_str)
                        print("send state message")
                        received = 0
                        connected = 0
                        break
                    elif websocket_data_type == 2:
                        await websocket.send(initial_result)
                        print("send initial result")
                        received = 0
                        connected = 0
                        break

 
async def Search(request):
    global search_result
    print("search")
    search_num = request.rel_url.query['product']
    print("search_num:"+search_num)

    #verify product_number and on/off state in json file
    with open('product_num.json','r') as f:
        product_dict = json.load(f)
    if (search_num in product_dict):
        if(product_dict[search_num] == 'on'):
	        #print("find product and it's used'")
            search_result = 0
            
            '''Publish MQTT message to ESP8266 for sensor state'''
            print("search sensorstate")
            mqttc.reconnect()
            mqttc.publish(search_num+"/Web/SensorState","1")
            #product_num/web/SensorState
            #"1"=> tell esp8266 to return sensors' state

            return web.Response(text="used",content_type='text/html')
        else:
	        #print("find product but not used")
            search_result = 1
            return web.Response(text="not_used",content_type='text/html')
    else:
	    #print("no product")
        search_result = 2
        return web.Response(text="no",content_type='text/html')

'''
async def ToggleChange(request):
    print("togglechange");
    sensor = request.rel_url.query['sensor']
    sensor_state = request.rel_url.query['state']
    print("sensor: " + sensor + "  state: " + sensor_state)
    
    #MQTT Publish
    mqttc.publish(sensor,sensor_state)
    return web.Response(text="change sent",content_type='text/html')
'''

async def Reload(request):
    print("reload")
    reload_num = request.rel_url.query['product']
    print("reload_num: "+reload_num)
    
    '''Publish MQTT message for ESP8266 sensors' states'''
    mqttc.reconnect()
    mqttc.publish(reload_num+"/Web/SensorState","1")
    #product_num/web/SensorState
    #"1"=>tell esp8266 to return sensors' states

    return web.Response(text="reloading",content_type='text/html')

async def Initial(request):
    print("initial")
    initial_num = request.rel_url.query['product']
    print("initial_num: ",initial_num)

    '''Publish MQTT message to initialize ESP8266'''
    mqttc.reconnect()
    mqttc.publish(initial_num+"/Web/Initial","1")

    return web.Response(text="initializing",content_type='text/html')

async def InitialCheck(request):
    print('initial check')
    global received, websocket_data_type, initial_result
    received = 1
    websocket_data_type = 2
    initial_result = request.rel_url.query['result']
    print("initialcheck = "+initial_result)

    if initial_result == '1':
        print('initial sucess')
    else:
        print('initial error')
    return web.Response(text="ok",content_type='text/html')


async def init(loop):
    app = web.Application()
    app.router.add_static('/public','./public')
    
    #receiving request
    app.router.add_get('/send',Send)
    app.router.add_get('/', LoginPage)
    app.router.add_get('/login', Login)
    app.router.add_get('/state', ControllerState)
    app.router.add_get('/search', Search)
    #app.router.add_get('/togglechange',ToggleChange)
    app.router.add_get('/reload',Reload)
    app.router.add_get('/initial', Initial)
    app.router.add_get('/initialcheck',InitialCheck)
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
