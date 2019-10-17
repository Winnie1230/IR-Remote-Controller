var mqtt = require('mqtt')
var client  = mqtt.connect('nckuwinnieliu.ddns.net')

client.on('connect', function (){
  client.subscribe('presence', function (err) {
    if (!err) {
      client.publish('presence', 'Hello mqtt')
    }
  })
})

client.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString())
  client.end()
})

$("#send_button").click(()=>{
    var send_topic = document.getElementById("input_topic").value;
    var send_message = document.getElementById("input_message").value;
    
    /*alert(send_topic);
    alert(send_message);*/
});
