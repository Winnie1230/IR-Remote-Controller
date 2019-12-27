import 'dart:io';
import 'dart:convert';
import 'package:flutter/material.dart';
import 'helpers/Constants.dart';
import 'package:progress_dialog/progress_dialog.dart';


class SettingPage extends StatefulWidget{

  final ProgressDialog pr;
  const SettingPage({Key key, @required this.pr}) : super(key: key);

  @override
  _SettingPageState createState() =>_SettingPageState();
}

class _SettingPageState extends State<SettingPage>{

  final _pwdcontroller = TextEditingController();
  String _txt = '';
  List<String> _ssid = [];

  ProgressDialog pr2;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    Future.delayed(Duration(seconds: 10)).then((onValue){
      if(widget.pr.isShowing()){
        widget.pr.hide();
      }
    });

    _connectWeb();
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build


    pr2 = ProgressDialog(context,type: ProgressDialogType.Normal);
    pr2.style(
      message: 'Connecting ...',
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


    return Scaffold(
      appBar: AppBar(
        title: Text('WiFi 設定',style: TextStyle(color: Colors.white,fontSize: 20.0),),
      ),
      body: ListView.builder(
          itemCount: _ssid.length,
          itemBuilder: (context,index){
            return Card(
              child: ListTile(
                title: Text(_ssid[index]),
                trailing: FlatButton(
                  child: new Text("Connect",style: TextStyle(color: bluecolor),),
                  onPressed: (){
                    _wifiDialog(_ssid[index]);
                  },
                ),
              ),
            );
          }
      ),
    );
  }

  void _connectWeb () async{
    try{
      HttpClient httpclient = HttpClient();
      HttpClientRequest request = await httpclient.getUrl(Uri.parse('http://192.168.4.1/ssid'));
      HttpClientResponse response = await request.close();
      _txt = await response.transform(utf8.decoder).join();
      httpclient.close();
      widget.pr.hide();
      setState(() {
        _ssid = _txt.split(':');
      });


    }catch(e){
      _txt = '';
    }

  }

  void _ssidpwd (String a,String b) async{

    try{
      HttpClient httpclient = HttpClient();
      HttpClientRequest request = await httpclient.getUrl(Uri.parse('http://192.168.4.1/wifi?ssid='+a+'&password='+b));
      HttpClientResponse response = await request.close();
      _txt = await response.transform(utf8.decoder).join();
      httpclient.close();
      if(pr2.isShowing()){
        pr2.hide();
      }

      if(_txt == 'successful'){
        _successfulDialog();

      }else if(_txt == 'fail'){
        _failDialog();
      }

    }catch(e){
      _txt = '';
    }

//    if(pr2.isShowing()){
//      pr2.hide();
//    }
//
//    if(_txt == 'successful'){
//      _successfulDialog();
//
//    }else if(_txt == 'fail'){
//      _failDialog();
//    }
  }

  void _wifiDialog(String a){
    showDialog(
      context: context,
      builder: (BuildContext context) {
        // return object of type Dialog
        return AlertDialog(
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
          title: new Text("請輸入" + a + "的密碼"),
          content: new TextField(
            controller: _pwdcontroller,
            decoration: InputDecoration(hintText: 'password'),
          ),
          actions: <Widget>[
            // usually buttons at the bottom of the dialog
            new FlatButton(
              child: new Text("確定",style: TextStyle(color: bluecolor),),
              onPressed: () {

                Navigator.pop(context);
                pr2.show();
                Future.delayed(Duration(seconds: 10)).then((onValue){
                  print("PR status  ${pr2.isShowing()}" );
                  if(pr2.isShowing()) {
                    pr2.hide();
                    _failDialog();
                  }
                  print("PR status  ${pr2.isShowing()}" );
                });

                _ssidpwd(a,_pwdcontroller.text);
                _pwdcontroller.clear();

              },
            ),
            new FlatButton(
              child: new Text("取消",style: TextStyle(color: bluecolor),),
              onPressed: () {
                Navigator.pop(context);
              },
            ),
          ],
        );
      },
    );
  }

  void _successfulDialog() {
    // flutter defined function
    showDialog(
      context: context,
      builder: (BuildContext context) {
        // return object of type Dialog
        return AlertDialog(
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
          title: new Text("連線成功！"),
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
          title: new Text("連線失敗！"),
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