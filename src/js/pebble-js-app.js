var ToughGuyQuotes = function(Pebble){
	
	var req = new XMLHttpRequest();
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			if(req.status == 200) {
				var response = JSON.parse(req.responseText);
			    Pebble.showSimpleNotificationOnPebble("Tough Guy", response[Object.keys(response)[0]]);
		   
			} else {
			    Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't load the quotes");
			}
		}
	}
	
	req.timeout = 3000;
	 
	 
	var getQuotes = function() {
		
		req.open('GET', 'http://toughguyquotes.azurewebsites.net/?' + Date.now(), true);
	 	req.send(null);
	};
	
	return {
		getQuotes : getQuotes
	}

}(Pebble);




Pebble.addEventListener("ready",
  function(e) {
	ToughGuyQuotes.getQuotes();
  }
);


Pebble.addEventListener("appmessage",
  function(e) {
    console.log("Received message: " + e.payload);
	ToughGuyQuotes.getQuotes();
  }
);
