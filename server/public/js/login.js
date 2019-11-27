$("#login_button").click(()=>{
    var user = $("#input_username").val();
    var password = $("#input_password").val();
    if (user && password){
    
    /*clear input bar*/
    $("#input_username").val("");	
    $("#input_password").val("")

    $.ajax({
	url: "/login?"+"username="+user+"&password="+password,
	type: 'GET',
	data: {
		//user_name: $('#user_name').val()
	},
	error: function(xhr) {
		alert('Ajax request 發生錯誤');
	},
	success: function(response) {
		console.log(response);
		if(response=="ok")
			window.location.href = "/public/index.html";
		else
			alert("Wrong username or password");
	}
    });
    //$("#mqtt_receive_topic").append("<p>start</p>");
    //alert(send_topic);
    //alert(send_message);
    } 
    else{
        $.alert({
            title: "Alert!",
            content: "Please input username and password!",
        });
    }
});
