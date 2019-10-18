import asyncio
from aiohttp import web

FILE_PATH = './public'
MAIN_INDEX = '/index.html'

PORT = 11230
'''
async def ClothStatus(request):
    status = event_handler.machine.get_status(0)
    return web.json_response(status)

count = 0
async def ClothDrying(request):
    global key
    global count
    key = request.rel_url.query['key']
    finish = request.rel_url.query['end']
    
    if finish == '0':
        count += 1
        if count > 5:
            count = 0
            print("ok")
            return web.json_response({"status":"ok"})
    return web.json_response({"status":"nok"})
    #elif finish == '1':
    #    return web.Response(text=key + " finish",content_type='text/html')
    #elif finish == '2':
    #    return web.Response(text=key + " next cloth",content_type='text/html')
     

async def ClothGathering(request):
    global key
    print("Get new req")
    key = request.rel_url.query['key']
    pack = request.rel_url.query['group']
    state = request.rel_url.query['state']
    #with open(FILE_PATH + MAIN_INDEX) as f:
    #return web.Response(text=f.read(),content_type='text/html')
    return web.Response(text="Hello World",content_type='text/html')
'''

async def Test():
    await asyncio.sleep(1)
    print("test")
    return web.Response(text="Hello World",content_type='text/html')
   
    #while True:
        #await asyncio.sleep(1)
        #print("test")

async def init(loop):
    app = web.Application()
    app.router.add_static('/public','./public')
    
    #receiving request
    app.add_routes([web.get('/test',Test)])
    #app.router.add_get('/test',Test)
    #app.router.add_get('/drying',ClothDrying)
    #app.router.add_get('/gathering',ClothGathering)
    srv = await loop.create_server(app._make_handler(),host='0.0.0.0', port=PORT)
    print("server created")
    return srv

if __name__ == '__main__':
    try:
        loop = asyncio.get_event_loop()

        #create task(can add any function)
        #loop.create_task(Test())
        
        loop.run_until_complete(init(loop))
        loop.run_forever()
    except KeyboardInterrupt:
        pass
    finally:
        print("Closing Loop")
        loop.close()
