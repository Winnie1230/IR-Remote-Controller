import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'helpers/Constants.dart';
import 'dart:async';
import 'package:progress_dialog/progress_dialog.dart';

class NewButton extends StatefulWidget{

  final MqttClient client;
  const NewButton({Key key, @required this.client}) : super(key: key);

  @override
  _NewButtonState createState() =>_NewButtonState();
}

class _NewButtonState extends State<NewButton>{


  final _controller = TextEditingController();
  ProgressDialog pr;
  StreamSubscription subscription;

  @override
  Widget build(BuildContext context) {
    pr = ProgressDialog(context,type: ProgressDialogType.Normal);
    pr.style(
      message: '學習中 ...',
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

    return Scaffold(
      appBar: AppBar(
        title: Text('新增一個按鍵',style: TextStyle(color: Colors.white,fontSize: 20.0),),
      ),
      body: Center(
        child: ListView(
          shrinkWrap: true,
          padding: EdgeInsets.only(left: 24.0,right: 24.0),
          children: <Widget>[
            Text('按鍵名稱 : ',style: TextStyle(color: Colors.black,fontSize: 20.0),),
            TextFormField(
              controller: _controller,
              decoration: InputDecoration(
                hintText: '名稱',
                contentPadding: EdgeInsets.fromLTRB(20.0, 10.0, 20.0, 10.0),

              ),
              style: TextStyle(
                color: Colors.black
              ),
            ),
            SizedBox(height: 40,),
            RaisedButton(
              shape: RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(24)),
              child: Text('開始學習',style: TextStyle(color: Colors.white),),
              padding: EdgeInsets.all(12),
              color: bluecolor,
              onPressed: (){
                if(_controller.text.isNotEmpty) {
                  subscription = widget.client.updates.listen(_onMessage);
                  //_sendMessage();
                  pr.show();
                  Future.delayed(Duration(seconds: 10)).then((onValue) {
                    print("PR status  ${pr.isShowing()}");
                    if (pr.isShowing()) {
                      pr.hide();
                      _failDialog();
                    }
                    print("PR status  ${pr.isShowing()}");
                  });
                  _sendMessage();
                }
              },
            )
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    subscription?.cancel();
    super.dispose();
  }

  void _onMessage(List<MqttReceivedMessage> event) {
    print(event.length);
    final MqttPublishMessage recMess =
    event[0].payload as MqttPublishMessage;
    final String message =
    MqttPublishPayload.bytesToStringAsString(recMess.payload.message);


    print('MQTT message: topic is <${event[0].topic}>, '
        'payload is <-- ${message} -->');
    print(widget.client.connectionStatus.state);
    if(event[0].topic == number+'/8266/Callback/LearnCallback'){
      if(message =='successful' && pr.isShowing()){
        pr.hide();
        irName.add(_controller.text);
        _successfulDialog();
      }
      else if(message =='fail' && pr.isShowing()){
        pr.hide();
        _failDialog();
      }
    }
  }

  void _sendMessage() {
    final MqttClientPayloadBuilder builder = MqttClientPayloadBuilder();

    builder.addString(_controller.text);
    widget.client.publishMessage(
      number + '/APP/Learn',
      MqttQos.exactlyOnce,
      builder.payload,
    );
    //Navigator.pop(context);
  }
  void _successfulDialog() {
    // flutter defined function
    showDialog(
      context: context,
      builder: (BuildContext context) {
        // return object of type Dialog
        return AlertDialog(
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
          title: new Text("學習成功！"),
          content: new Text("按鈕新增於主畫面！"),
          actions: <Widget>[
            // usually buttons at the bottom of the dialog
            new FlatButton(
              child: new Text("Close",style: TextStyle(color: bluecolor),),
              onPressed: () {
                Navigator.pop(context);
                Navigator.pop(context);
              },
            ),
          ],
        );
      },
    );
  }

  void _failDialog() {
    // flutter defined function
    showDialog(
      context: context,
      builder: (BuildContext context) {
        // return object of type Dialog
        return AlertDialog(
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
          title: new Text("學習失敗！"),
          content: new Text("請再試一次！"),
          actions: <Widget>[
            // usually buttons at the bottom of the dialog
            new FlatButton(
              child: new Text("再試一次",style: TextStyle(color: bluecolor),),
              onPressed: () {
                Navigator.pop(context);
              },
            ),
            new FlatButton(
              child: new Text("取消",style: TextStyle(color: bluecolor),),
              onPressed: () {
                Navigator.pop(context);
                Navigator.pop(context);
              },
            ),
          ],
        );
      },
    );
  }

}

