import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:ir_app/EditButton.dart';
import 'package:ir_app/NewButton.dart';
import 'package:ir_app/NewOftenUse.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'helpers/Constants.dart';
import 'dart:async';
import 'package:progress_dialog/progress_dialog.dart';

class HomePage extends StatefulWidget{
  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State <HomePage>{

  PageController _pageController;
  int _page = 0;

  String broker = 'winnieliu.ddns.net' /*'soldier.cloudmqtt.com'*/;
  int port = 1883 /*10465*/;
  String user = 'pi' /*'mbklmuev'*/;
  String password = 'winnie1230' /*'ywNpN8zKlSwz'*/;
  String clientIdentifier = 'frank';

  MqttClient client;
  MqttConnectionState connectionState;

  StreamSubscription subscription;

  Set<String> topics = Set<String>();
  List<Message> messages = <Message>[];

  //sensor 回傳的data
  String temperature = '--';
  String humidity = '--';
  String co2 = '--';
  String pm25 = '--';
  String pm10 = '--';
  String tvoc = '--';
  String hcho = '--';
  String current = '--';

  ScrollController messageController = ScrollController();

  ProgressDialog pr;

  final String _subTopic = number+'/8266/#';


  @override
  Widget build(BuildContext context) {

    pr = ProgressDialog(context,type: ProgressDialogType.Normal);
    pr.style(
        message: 'Loading ...',
        borderRadius: 10.0,
        backgroundColor: Colors.white,
        progressWidget: CircularProgressIndicator(
          valueColor: AlwaysStoppedAnimation<Color>(bluecolor),
        ),
        elevation: 10.0,
        insetAnimCurve: Curves.easeInOut,
        progressTextStyle: TextStyle(
            color: Colors.black, fontSize: 13.0, fontWeight: FontWeight.w400),
        messageTextStyle: TextStyle(
            color: Colors.black, fontSize: 19.0, fontWeight: FontWeight.w600),
    );

    // TODO: implement build

    void navigationTapped(int page) {
      _pageController.animateToPage(page,
          duration: const Duration(milliseconds: 300), curve: Curves.ease);
    }

    void onPageChanged(int page) {
      setState(() {
        this._page = page;
      });
    }

    Text pageName(int page) {
      if(page == 0){
        return Text('常用',style: TextStyle(color: Colors.white,fontSize: 20.0));
      }else if(page == 1){
        return Text('遙控器',style: TextStyle(color: Colors.white,fontSize: 20.0));
        }else{
        return Text('儀表板',style: TextStyle(color: Colors.white,fontSize: 20.0));
      }
    }

    List<Widget> _act(){
      if(_page==0||_page==1){
        if(_page==0){
          return <Widget>[FlatButton(child: Text('編輯',style: TextStyle(color: Colors.white,fontSize: 15.0),), onPressed: () {
            Navigator.push(context, MaterialPageRoute<String>(
                builder: (BuildContext context)=>NewOftenUse()
            ));
          })];
        }
        else{
          return <Widget>[FlatButton(child: Text('編輯',style: TextStyle(color: Colors.white,fontSize: 15.0),), onPressed: () {
            Navigator.push(context, MaterialPageRoute<String>(
                builder: (BuildContext context)=>EditButton(client: client,)
            ));
          })];
        }
      }else return null;
    }

    return Scaffold(
      drawer: Mydrawer(),
      appBar: AppBar(
        title: pageName(_page),
        actions: _act(),
//        actions: <Widget>[
//          IconButton(icon: Icon(Icons.more_vert), onPressed: () {})
//        ],
      ),
      backgroundColor: Colors.white,
      body: PageView(
        controller: _pageController,
        onPageChanged: onPageChanged,
        children: <Widget>[
          _buildOftenUsePage(),
          _buildRemotePage(),
          _buildDashBoardPage(),
        ],
      ),
      floatingActionButton: _page == 1
          ? Builder(builder: (BuildContext context){
            return FloatingActionButton(
              child: Icon(Icons.add),
              backgroundColor: bluecolor,
              onPressed: (){
                  Navigator.push(context, MaterialPageRoute<String>(
                    builder: (BuildContext context)=>NewButton(client: client)
                  ));

              },
            );
      })
          :null,
      bottomNavigationBar: BottomNavigationBar(
        items: <BottomNavigationBarItem>[
          BottomNavigationBarItem(icon: Icon(Icons.star),title: Text('常用')),
          BottomNavigationBarItem(icon: Icon(Icons.settings_remote),title: Text('遙控器')),
          BottomNavigationBarItem(icon: Icon(Icons.dashboard),title: Text('儀表板')),
        ],
        currentIndex: _page,
        fixedColor: bluecolor,
        onTap: navigationTapped,

      ),
    );
  }

  GridView _buildOftenUsePage(){
    return GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: 2,
          childAspectRatio: 2.0,
          crossAxisSpacing: 10,
          mainAxisSpacing: 10,

        ),
        padding: EdgeInsets.all(15),
        itemCount: oftenUseIrName.length,
        itemBuilder: (context,index){
          return RaisedButton(
//              shape: RoundedRectangleBorder(
//                  borderRadius: BorderRadius.circular(24)),
            color: Colors.grey,
            child: Text(oftenUseIrName[index],style: TextStyle(color: Colors.black),),
            onPressed: () {
              final MqttClientPayloadBuilder builder = MqttClientPayloadBuilder();
              builder.addString(oftenUseIrName[index]);
              client.publishMessage(
                number + '/APP/Control',
                MqttQos.exactlyOnce,
                builder.payload,
              );
            },
          );
        }
    );

  }

  GridView _buildRemotePage(){
    return GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: 2,
          childAspectRatio: 2.0,
          crossAxisSpacing: 10,
          mainAxisSpacing: 10,

        ),
        padding: EdgeInsets.all(15),
        itemCount: irName.length,
        itemBuilder: (context,index){
          return RaisedButton(
//              shape: RoundedRectangleBorder(
//                  borderRadius: BorderRadius.circular(24)),
            color: Colors.grey,
            child: Text(irName[index],style: TextStyle(color: Colors.black),),
            onPressed: () {
              final MqttClientPayloadBuilder builder = MqttClientPayloadBuilder();
              builder.addString(irName[index]);
              client.publishMessage(
                number + '/APP/Control',
                MqttQos.exactlyOnce,
                builder.payload,
              );
            },
          );
        }
    );

  }

  ListView _buildDashBoardPage(){
    return ListView(
      children: <Widget>[
        Card(
          child: ListTile(
            title: Text('電流'),
            subtitle: Text(current+' A',style: TextStyle(fontSize: 20),),
          ),
        ),
        Card(
          child: ListTile(
            title: Text('溫度'),
            subtitle: Text('攝氏 '+temperature+' 度',style: TextStyle(fontSize: 20),),
          ),
        ),
        Card(
          child: ListTile(
            title: Text('濕度'),
            subtitle: Text(humidity+' %RH',style: TextStyle(fontSize: 20),),
          ),
        ),
        Card(
          child: ListTile(
            title: Text('CO2'),
            subtitle: Text(co2+' ppm',style: TextStyle(fontSize: 20),),
          ),
        ),
        Card(
          child: ListTile(
            title: Text('PM 2.5'),
            subtitle: Text(pm25+' ug/m3',style: TextStyle(fontSize: 20),),
          ),
        ),
        Card(
          child: ListTile(
            title: Text('PM 10'),
            subtitle: Text(pm10+' ug/m3',style: TextStyle(fontSize: 20),),
          ),
        ),
        Card(
          child: ListTile(
            title: Text('TVOC'),
            subtitle: Text(tvoc+' ug/m3',style: TextStyle(fontSize: 20),),
          ),
        ),
        Card(
          child: ListTile(
            title: Text('甲醛'),
            subtitle: Text(hcho+' ug/m3',style: TextStyle(fontSize: 20),),
          ),
        ),
      ],
    );
  }


  @override
  void dispose() {
    subscription?.cancel();
    super.dispose();
  }

  @override
  void initState() {
    _connect();
    _pageController = PageController();
    super.initState();
  }


  void _connect ()async{
    client = MqttClient(broker,'');
    client.port = port;
    client.logging(on: true);
    client.keepAlivePeriod = 30;
    client.onDisconnected = _onDisconnected;
    final MqttConnectMessage connMess = MqttConnectMessage()
        .withClientIdentifier(clientIdentifier)
        .startClean()
        .keepAliveFor(30)
        .withWillTopic('test/test')
        .withWillMessage('lamhx message test')
        .withWillQos(MqttQos.atMostOnce);
    print('MQTT client connecting....');
    client.connectionMessage = connMess;

    try {
      await client.connect(user, password);
    } catch (e) {
      print(e);
      _disconnect();
    }

    if (client.connectionStatus.state == MqttConnectionState.connected) {
      print('MQTT client connected');
      setState(() {
        connectionState = client.connectionStatus.state;
      });
    } else {
      print('ERROR: MQTT client connection failed - '
          'disconnecting, state is ${client.connectionStatus.state}');
      _disconnect();
    }
//    _subscribeToTopic(_subTopic);
//    subscription = client.updates.listen(_onMessage);
//    _publishAppState();

    pr.show();
    Future.delayed(Duration(seconds: 10)).then((onValue){
      print("PR status  ${pr.isShowing()}" );
      if(pr.isShowing())
        pr.hide();
      print("PR status  ${pr.isShowing()}" );
    });

    _subscribeToTopic(_subTopic);
    subscription = client.updates.listen(_onMessage);
    _publishAppState();

  }

  void _disconnect() {
    client.disconnect();
    _onDisconnected();
  }

  void _onDisconnected() {
    setState(() {
      topics.clear();
      connectionState = client.connectionStatus.state;
      client = null;
      subscription.cancel();
      subscription = null;
    });
    print('MQTT client disconnected');
  }


  void _onMessage(List<MqttReceivedMessage> event) {
    print(event.length);
    final MqttPublishMessage recMess =
    event[0].payload as MqttPublishMessage;
    final String message =
    MqttPublishPayload.bytesToStringAsString(recMess.payload.message);


    print('MQTT message: topic is <${event[0].topic}>, '
        'payload is <-- ${message} -->');
    print(client.connectionStatus.state);
    if(event[0].topic == number+'/8266/Sensor'){
      Map <String, dynamic>sensor = jsonDecode(message);
      setState(() {
        temperature = sensor['temp'];
        humidity = sensor['humi'];
        co2 = sensor['co2'];
        pm25 = sensor['pm25'];
        pm10 = sensor['pm10'];
        tvoc = sensor['tvoc'];
        hcho = sensor['hcho'];
        current = sensor['current'];

      });

    }else if(event[0].topic == number+'/8266/Callback/EEPROM'){
      if(message.isNotEmpty) {
        setState(() {
          irName = message.split(':');
        });
      }
      if(pr.isShowing()){
        pr.hide();
      }
    }
  }

  void _subscribeToTopic(String topic) {
    if (connectionState == MqttConnectionState.connected) {
      setState(() {
        if (topics.add(topic.trim())) {
          print('Subscribing to ${topic.trim()}');
          client.subscribe(topic, MqttQos.exactlyOnce);
        }
      });
    }
  }

  void _unsubscribeFromTopic(String topic) {
    if (connectionState == MqttConnectionState.connected) {
      setState(() {
        if (topics.remove(topic.trim())) {
          print('Unsubscribing from ${topic.trim()}');
          client.unsubscribe(topic);
        }
      });
    }
  }
//APP開啟時發布
  void _publishAppState(){
    final MqttClientPayloadBuilder builder = MqttClientPayloadBuilder();
    builder.addString('Start');
    client.publishMessage(
      number + '/APP/APPState',
      MqttQos.exactlyOnce,
      builder.payload,
    );
  }

}