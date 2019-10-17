var mqtt;
var reconnectTimeout = 2000;
var host = "nckuwinnieliu.ddns.net";

function onConnect(){
//once a connection has been made, make a subscription and send a message
    console.log("Connected");

}

$("#send_button").click(()=>{
    var send_topic = document.getElementById("input_topic").value;
    var send_message = document.getElementById("input_message").value;
    
    /*alert(send_topic);
    alert(send_message);*/
});