import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'helpers/Constants.dart';
import 'package:shared_preferences/shared_preferences.dart';

class EditButton extends StatefulWidget{
  final MqttClient client;
  const EditButton({Key key, @required this.client}) : super(key: key);

  @override
  _EditButtonState createState() =>_EditButtonState();
}

class _EditButtonState extends State<EditButton>{

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
      appBar: AppBar(
        title: Text('刪除按鍵',style: TextStyle(color: Colors.white,fontSize: 20.0),),
      ),

      body: ListView.builder(
          itemCount: irName.length,
          itemBuilder: (context,index){
            return Card(
              child: ListTile(
                title: Text(irName[index]),
                trailing: IconButton(
                  icon: Icon(Icons.clear),
                  onPressed: (){
                    _confirmDialog(irName[index]);
                  },
                ),
              ),
            );
          }
      ),

    );
  }

  void _confirmDialog(String a) {
    // flutter defined function
    showDialog(
      context: context,
      builder: (BuildContext context) {
        // return object of type Dialog
        return AlertDialog(
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
          title: new Text("即將刪除按鍵 "+ a +" ！"),
          content: new Text("是否確定要刪除此按鍵？"),
          actions: <Widget>[
            // usually buttons at the bottom of the dialog
            new FlatButton(
              child: new Text("是",style: TextStyle(color: bluecolor),),
              onPressed: () {
                setState(() {
                  _sendMessage(a);
                  irName.remove(a);
                  oftenUseIrName.remove(a);
                  _writeOftenUse();
                });
                //irName.remove(a);
                Navigator.pop(context);
              },
            ),
            new FlatButton(
              child: new Text("否",style: TextStyle(color: bluecolor),),
              onPressed: () {
                Navigator.pop(context);
              },
            ),
          ],
        );
      },
    );
  }

  void _sendMessage(String a) {
    final MqttClientPayloadBuilder builder = MqttClientPayloadBuilder();

    builder.addString(a);
    widget.client.publishMessage(
      number + '/APP/Remove',
      MqttQos.exactlyOnce,
      builder.payload,
    );
    //Navigator.pop(context);
  }
  Future<void> _writeOftenUse() async{
    SharedPreferences prefs = await SharedPreferences.getInstance();
    await prefs.setStringList('often', oftenUseIrName);
  }
}