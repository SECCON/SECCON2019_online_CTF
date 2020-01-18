function circle(){
    $("#register-form").append("<div id='loadingBG'><div class='circle'></div></div>");
}

function createQR(){
    sendData = {};
    sendData["amount"]   = $("#amount").val();
    sendData["password"] = $("#password").val();
    $.ajax({
	type:"POST",
	url:"/api/createQR",
	data:JSON.stringify(sendData),
	contentType: "application/json; charset=utf-8",
	dataType: "json",
	success : function(data) {
	    console.log("foo");
	    if(data["status"] == "ok"){
		$("#qr").html( data["dom"] );
	    }else{
		$("#qr").html( data["error"]);
	    }
	}
    });
}

function readQR(){
    fileData = {};
    fileData = $("#qrImg").prop("files")[0];
    formData = new FormData();
    formData.append('file', fileData);
    console.log(formData);
    $.ajax({
	type:"POST",
	url:"/api/readQR",
	data:formData,
	contentType: false,
	processData: false,
	success : function(data) {
	    if(data["status"] == "ok"){
		$("#receipt").html( data["dom"] );
	    }else{
		$("#receipt").html( data["error"] );
	    }
	}
    });
}
