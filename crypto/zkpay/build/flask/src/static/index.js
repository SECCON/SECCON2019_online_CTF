function circle(){ $("#register-form").append("<div id='loadingBG'><div class='circle'></div></div>"); }

function invokeSignup()
{
    $("#pro-div").show();

    if(signup.waited){
	return true;
    }

    if($("#user-form")[0].value.match(/^[A-Za-z0-9]+$/) == null || $("#password-form")[0].value.match(/^[A-Za-z0-9]+$/) == null){
	Snackbar.show({text: "username, password: ^[A-Za-z0-9]+$"});
	return false;
    }


    signup.ok = false;
    signup.waited=true;
    elapsed=0;
    Snackbar.show({text: "Registering...", duration: 0});
    progress = setInterval(function(){
	elapsed++;
	percent = elapsed/59.0*100;
	$('.progress-bar').css('width', percent+'%').attr('aria-valuenow', percent);
	if (elapsed == 10){
	    Snackbar.show({text: "Proving...", duration: 0});
	}
	if (elapsed == 30){
	    Snackbar.show({text: "Still proving...", duration: 0});
	}
	if (elapsed == 45){
	    Snackbar.show({text: "Almost...", duration: 0});
	}
    }, 500);

    setTimeout(function() {
	if(signup.ok){
	    window.location.replace("/home");
	}
    }, 30000);

    sendData = {};
    sendData["user"]   = $("#user-form").val();
    sendData["pass"] = $("#password-form").val();
    $.ajax({
	type:"POST",
	url:"/api/signup",
	data:JSON.stringify(sendData),
	contentType: "application/json; charset=utf-8",
	dataType: "json",
	success : function(data) {
	    if(data["status"] == "ok"){
 		signup.ok = true;
	    }else{
		Snackbar.show({text: data["error"]});
		clearInterval(progress);
		signup.waited = false;
		$("#pro-div").removeAttr("style").hide();
	    }
	}
    });

}

$("#pro-div").removeAttr("style").hide();
