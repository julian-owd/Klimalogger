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
			url: ("data/get_humidity_data.php?r=" + rId),
			dataType: "json",
			async: false
		}).responseText;
				  
		// Create data table out of JSON data
		var data = new google.visualization.DataTable(jsonData);
			
		var dateFormatter = new google.visualization.DateFormat({pattern: 'dd.MM.yyyy HH:mm:ss'});
		dateFormatter.format(data, 0);
				
		var options = {
			title: 'Luftfeuchtigkeit in %',
			legend: {position: 'bottom'},
			chartArea: {width: '95%', height: '65%'},
			hAxis: {format: 'HH:mm:ss'},
			colors: ['#3b9881']
		};
				
		// Instantiate and draw our chart, passing in some options.
		var chart = new google.visualization.LineChart(document.getElementById('humidity_chart'));
		chart.draw(data, options);
	}
});