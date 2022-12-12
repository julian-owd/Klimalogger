$(function() {
	var rId = 1;
	
	const queryString = window.location.search;
	const urlParameters = new URLSearchParams(queryString);
	
	if (urlParameters.get('r')) {
		rId = urlParameters.get('r');
	}
	
	google.charts.load('current', {'packages':['corechart']});
	google.charts.setOnLoadCallback(drawChart);
			
	function drawChart() {
		var jsonData = $.ajax({
			url: ("data/get_temp_data.php?r=" + rId),
			dataType: "json",
			async: false
		}).responseText;
				  
		// Create data table out of JSON data
		var data = new google.visualization.DataTable(jsonData);
		var dateFormatter = new google.visualization.DateFormat({pattern: 'dd.MM.yyyy HH:mm:ss'});
		dateFormatter.format(data, 0);
				
		var options = {
			title: 'Temperatur in Grad Celcius',
			legend: {position: 'bottom'},
			chartArea: {width: '90%', height: '65%'},
			hAxis: {format: 'HH:mm'},
			vAxis: {format: '##'},
			colors: ['#3b7f98']
		};
		
		var chart = new google.visualization.LineChart(document.getElementById('temp_chart'));
		chart.draw(data, options);
	}
});