import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:progress_dialog/progress_dialog.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:ir_app/SettingPage.dart';
//colors
Color bluecolor = Color.fromRGBO(0, 51, 102, 1.0);

//strings
const apptitle = 'IR Remote Controller';


//images
Image applogo = Image.asset('assets/images/icons8-remote-control-100.png');

//sizes

const bigradius = 66.0;
const buttonheight = 24.0;

//Pages
const loginpagetag = 'Login Page';
const homepagetag = 'Home Page';
const settingpagetag = 'Setting Page';
const newbutton = 'New Button';


//MQTT Message class
class Message {
  final String topic;
  final String message;
  final MqttQos qos;

  Message({this.topic, this.message, this.qos});
}

//Drawer
class Mydrawer extends StatelessWidget{
  ProgressDialog pr;
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
    return Drawer(
      child: ListView(
        // 去除顶部灰色条
        padding: EdgeInsets.zero,
        children: <Widget>[
          UserAccountsDrawerHeader(
            accountName: Text('產品序號：'+ number),
            currentAccountPicture: CircleAvatar(
              backgroundImage: AssetImage('assets/images/icons8-remote-control-100.png'),
              backgroundColor: Colors.white,
            ),
          ),

          ListTile(
              title: Text("WiFi設定"),
              leading: Icon(Icons.wifi),
              onTap: () {
                //Navigator.of(context).pushNamed(settingpagetag);
                Navigator.push(context, MaterialPageRoute<String>(
                    builder: (BuildContext context)=>SettingPage(pr: pr,)
                ));
                pr.show();
              }
          ),
          ListTile(
            title:  Text("重設產品序號"),
            leading: Icon(Icons.vpn_key),
            onTap: (){
              //_clearNumber();
              Navigator.of(context).pushNamed(loginpagetag);
            },
          )
        ],
      ),
    );
  }

  Future<void> _clearNumber() async{
    SharedPreferences prefs = await SharedPreferences.getInstance();
    await prefs.setString('number', null);
  }
}


//產品序號
String number = '';

//按鈕名稱（按鈕控制的紅外碼儲存於8266）
List<String> irName = [];
List<String> oftenUseIrName = [];

