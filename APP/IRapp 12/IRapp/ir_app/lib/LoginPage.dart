import 'package:flutter/material.dart';
import 'package:ir_app/HomePage.dart';
import 'helpers/Constants.dart';
import 'package:shared_preferences/shared_preferences.dart';

class LoginPage extends StatefulWidget{
  @override
  _LoginPageState createState() => _LoginPageState();
}
class _LoginPageState extends State<LoginPage>{

  final _codecontroller = TextEditingController();

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    _checkLogin();
  }

  @override
  Widget build(BuildContext context) {

    final logo = CircleAvatar(
      backgroundColor: Colors.white,
      radius: bigradius,
      child: applogo,
    );

    final code = TextFormField(
      controller: _codecontroller,
      keyboardType: TextInputType.text,
      maxLength: 10,
      maxLines: 1,
      autofocus: true,
      decoration: InputDecoration(
          hintText: '序號',
          contentPadding: EdgeInsets.fromLTRB(20.0, 10.0, 20.0, 10.0),

      ),
      style: TextStyle(
        color: Colors.black,
      ),
    );

    final loginbutton = Padding(
        padding: EdgeInsets.symmetric(
            vertical: 16.0),
        child: RaisedButton(
          shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(24)),
          onPressed: () {
            if(_codecontroller.text.isEmpty){

            }
            else {
              number = _codecontroller.text;
              oftenUseIrName.clear();
              irName.clear();
              _setNumber();
              Navigator.of(context).pushNamed(homepagetag);
            }
          },
          padding: EdgeInsets.all(12),
          color: bluecolor,
          child: Text("確定",style: TextStyle(color: Colors.white),),
        ),
    );
    // TODO: implement build
    return Scaffold(
      appBar: AppBar(
        title: Text('IR Remote Controller',style: TextStyle(color: Colors.white,fontSize: 20.0),),
      ),
      backgroundColor: Colors.white,
      body: Center(
        child: ListView(
          shrinkWrap: true,
          padding: EdgeInsets.only(left: 24.0,right: 24.0),
          children: <Widget>[
            logo,
            SizedBox(height: bigradius,),
            Text('您好！請輸入產品序號：',style: TextStyle(color: Colors.black,fontSize: 20.0),),
            SizedBox(height: 20.0,),
            code,
            SizedBox(height: buttonheight,),
            loginbutton
          ],
        ),
      ),
    );
  }

  Future<void>_setNumber() async{
    SharedPreferences prefs = await SharedPreferences.getInstance();
    await prefs.setString('number', _codecontroller.text);
    await prefs.setStringList('often', oftenUseIrName);
  }

  Future<void>_checkLogin() async{
    SharedPreferences prefs = await SharedPreferences.getInstance();
    if(prefs.getString('number') != null && number.isEmpty){
      number = prefs.getString('number');
      oftenUseIrName = prefs.getStringList('often');
      Navigator.push(context, MaterialPageRoute<String>(
        builder: (BuildContext context)=>HomePage()
      ));
    }
  }
}
