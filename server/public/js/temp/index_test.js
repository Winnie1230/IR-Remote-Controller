var host = "nckuwinnieliu.ddns.net"
var port = 11230
/*-----create js websocket-----*/
//var ws = new WebSocket("ws://nckuwinnieliu.ddns.net:11230");

/*
let socket = new WebSocket("ws://nckuwinnieliu.ddns.net:8765");

socket.onopen = function(e) {
  alert("[open] Connection established");
  alert("Sending to server");
  socket.send("John");
};

socket.onmessage = function(event) {
  alert(`[message] Data received from server: ${event.data}`);
};

socket.onclose = function(event) {
  if (event.wasClean) {
    alert(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
  } else {
    // e.g. server process killed or network down
    // event.code is usually 1006 in this case
    alert('[close] Connection died');
  }
};

socket.onerror = function(error) {
  alert(`[error] ${error.message}`);
};
*/

/*
window.onload = function() {
	var ws = new WebSocket("ws://nckuwinnieliu.ddns.net:11230");
};


function WebSocketConnect(){
	var ws = new WebSocket("ws://nckuwinnieliu.ddns.net:11230");

    ws.onopen = function(){
		// Web Socket 已连接上，使用 send() 方法发送数据
		ws.send("发送数据");
		alert("数据发送中...");
	};
        
	ws.onmessage = function (evt){ 
		var received_msg = evt.data;
        alert("数据已接收...");
	};
                
	ws.onclose = function(){ 
		// 关闭 websocket
        alert("连接已关闭..."); 
	};

}
*/

$("#send_button").click(()=>{
    send_topic = $("#input_topic").val();
    send_message = $("#input_message").val();
    
    /*clear input bar*/
    $("#input_topic").val("");	
    $("#input_message").val("");

    /*show on receive block*/
    $("#mqtt_receive_topic").append('<p class="receive_message_content">'+send_topic+'</p>');
    $("#mqtt_receive_message").append('<p class="receive_message_content">'+send_message+'</p>');

    $.ajax({
	url: "/send?"+"topic="+send_topic+"&message="+send_message,
	type: 'GET',
	data: {
		//user_name: $('#user_name').val()
	},
	error: function(xhr) {
		alert('Ajax request 發生錯誤');
	},
	success: function(response) {
		console.log("sucess");
	}
    });
});

$("#logout_button").click(()=>{
	alert("logout");
	window.location.href = "/";
});
