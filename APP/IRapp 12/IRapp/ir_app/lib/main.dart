import 'package:flutter/material.dart';
import 'package:ir_app/HomePage.dart';
import 'package:ir_app/LoginPage.dart';
import 'package:ir_app/SettingPage.dart';
import 'helpers/Constants.dart';

void main() => runApp(MyApp());

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() =>_MyAppState();
}

class _MyAppState extends State<MyApp> {


  final routes = <String, WidgetBuilder>{
    loginpagetag : (context) => LoginPage(),
    homepagetag : (context) => HomePage(),
    settingpagetag : (context) => SettingPage(),
    //newbutton : (context) => NewButton(),
  };


  @override
  Widget build(BuildContext context) {
    // TODO: implement build

    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: apptitle,
      theme: new ThemeData(
          primaryColor: bluecolor),
      home: LoginPage(),
      routes: routes,
    );
  }

}
