var ToughGuyQuotes = function(Pebble){
	
	var settingsVersionKey = "4QPF";
	
	var getSettingsURL = "http://x.setpebble.com/api/" + settingsVersionKey + "/" + Pebble.getAccountToken();
	var setSettingsURL = "http://x.setpebble.com/" + settingsVersionKey + "/" + Pebble.getAccountToken();
	
	localStorage[settingsVersionKey] = localStorage[settingsVersionKey] || {};
	
	var req = new XMLHttpRequest();
	req.onload = function(e) {
		console.log("onload");
		if (req.readyState == 4 && req.status == 200) {
			console.log("response ok");
			var response = JSON.parse(req.responseText);
			var text = response.value[0].joke;
			Pebble.showSimpleNotificationOnPebble("Tough Guy", text);	
			return;
			
		} else {
			 Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't load the quotes");
		}	
	}
	req.ontimeout = function(e) {
		console.log("timed out");
		Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't load the quotes it timed out connecting to the internet");
	}
	
	
	req.timeout = 3000;
	
	var checkSettingsAreOK = function() {
		
		if(localStorage[settingsVersionKey]) {
			var firstNameSet = localStorage[settingsVersionKey].hasOwnProperty('firstName') && localStorage[settingsVersionKey].firstName.length > 0;
			var lastNameSet = localStorage[settingsVersionKey].hasOwnProperty('lastName') && localStorage[settingsVersionKey].lastName.length > 0;
			var showExplicitSet = localStorage[settingsVersionKey].hasOwnProperty('showExplicit') && (localStorage[settingsVersionKey].showExplicit === 1 || localStorage[settingsVersionKey].showExplicit === 0);
			
			if(firstNameSet && lastNameSet && showExplicitSet) {
				console.log("settings are ok");
				return true;
			} else {
				console.log("firstNameSet: " + firstNameSet + " lastNameSet: " + lastNameSet + " showExplicitSet: " + showExplicitSet);
				return false;
				
			}
			
			
		} else {
			console.log("no settings saved in local storage");
			return false;
		}
			
		
	};

	 
	var getQuotes = function() {
		
		if(checkSettingsAreOK() === false) {
			Pebble.showSimpleNotificationOnPebble("Tough Guy", "Could not load your settings, please ensure you have set them in the pebble app.");
			return;
		}
		
		var count = 1;
		var url = "http://api.icndb.com/jokes/random/" + count + "?firstName=" + localStorage[settingsVersionKey].firstName + "&lastName=" + localStorage[settingsVersionKey].lastName;
		console.log(url);
		
		url = encodeURI(url);
		
		req.open('GET', url, true);
		console.log('done get');
	 	req.send(null);
		console.log("triggered send");
	};
	

	var parseSettings = function(settingsJSON) {
	
	
		try {
			//n.b. settingsJSON not an array, but the key is a number.
			localStorage[settingsVersionKey].firstName = settingsJSON["1"];
			localStorage[settingsVersionKey].lastName = settingsJSON["2"];
			localStorage[settingsVersionKey].showExplicit = settingsJSON["3"];
			
		} catch(e) {
	
			Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't save your name etc in the settings");
	      	
	    }
		
	};

	Pebble.addEventListener("showConfiguration", function(e) {
		Pebble.openURL(setSettingsURL);
	});
	
	
	Pebble.addEventListener("webviewclosed", function(e) {
	
		if ((typeof(e.response) == 'string') && (e.response.length > 0)) {
			
			var settingsJSON = JSON.parse(e.response);
			parseSettings(settingsJSON);
			
		}  else {
			
			throw Error("The response was empty");
			
		}

	});
	
	Pebble.addEventListener("appmessage", function(e) {
		
		getQuotes();
	});
	
	Pebble.addEventListener("ready", function(e) {
		
	
		
		
	});
	
	
}(Pebble);

