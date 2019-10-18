var host = "nckuwinnieliu.ddns.net"
var port = 11230

$("#send_button").click(()=>{
    send_topic = document.getElementById("input_topic").value;
    send_message = document.getElementById("input_message").value;
    
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
    
    //$("#mqtt_receive_topic").append("<p>start</p>");
    //alert(send_topic);
    //alert(send_message);
});
