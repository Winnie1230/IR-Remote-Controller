import 'package:flutter/material.dart';
import 'helpers/Constants.dart';
import 'package:shared_preferences/shared_preferences.dart';

class NewOftenUse extends StatefulWidget{
  @override
  _NewOftenUseState createState() =>_NewOftenUseState();
}

class _NewOftenUseState extends State<NewOftenUse>{
  List<bool> _isCheck;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    _isCheck = List(irName.length);
    if(oftenUseIrName.isNotEmpty) {
      for (int a = 0; a < irName.length; a++) {
        for (int b = 0; b < oftenUseIrName.length; b++) {
          if (oftenUseIrName[b] == irName[a]) {
            _isCheck[a] = true;
            break;
          }
          else {
            _isCheck[a] = false;
          }
        }
      }
    }else{
      for (int a = 0;a<irName.length;a++){
        _isCheck[a]=false;
      }
    }

//    oftenUseIrName.clear();
  }
  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
      appBar: AppBar(
        leading: IconButton(
          icon: Icon(Icons.arrow_back_ios),
          onPressed: (){
            Navigator.pop(context);
          },
        ),
        title: Text('新增按鍵到常用',style: TextStyle(color: Colors.white,fontSize: 20.0),),
        actions: <Widget>[
          FlatButton(
            child: Text('確定',style: TextStyle(color: Colors.white,fontSize: 15.0),),
            onPressed: (){
              oftenUseIrName.clear();
              for(int a = 0;a < irName.length;a++){
                if(_isCheck[a] == true){
                  oftenUseIrName.add(irName[a]);
                }
              }
              _writeOftenUse();
              Navigator.pop(context);
            },
          )
        ],
      ),
      body: ListView.builder(
          itemCount: irName.length,
          itemBuilder: (context,index){
            return Card(
              child: CheckboxListTile(
                title: Text(irName[index]),
                value: _isCheck[index],
                onChanged: (isCheck){
                  setState(() {
                    _isCheck[index] = isCheck;
                  });
                },
              ),
            );
          }
      ),
    );
  }

  Future<void> _writeOftenUse() async{
    SharedPreferences prefs = await SharedPreferences.getInstance();
    await prefs.setStringList('often', oftenUseIrName);
  }
}