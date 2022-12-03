$(document).ready(function(){
	$.ajax({
		url : "http://localhost/wetter/climate_data.php",
		type : "GET",
		success : function(data){
			console.log(data);

			var dateArray = [];
			var tempArray = [];
			var humidityArray = [];
			var co2Array = [];

			for(var i in data) {
				dateArray.push(data[i].datum);
				tempArray.push(data[i].temp);
				humidityArray.push(data[i].humidity);
				co2Array.push(data[i].co2);
			}

      var maxTemp = Math.max(...tempArray);
      var maxHumidity = Math.max(...humidityArray);

			var tempData = {
				labels: dateArray,
				datasets: [
					{
						label: "Temperatur",
						fill: true,
						lineTension: 0.1,
						backgroundColor: "rgba(59, 89, 152, 0.75)",
						borderColor: "rgba(59, 89, 152, 1)",
						pointHoverBackgroundColor: "rgba(59, 89, 152, 1)",
						pointHoverBorderColor: "rgba(59, 89, 152, 1)",
						data: tempArray
					}
				]
			};

      var humidityData = {
        labels: dateArray,
        datasets: [
          {
            label: "Luftfeuchtigkeit",
            fill: true,
            lineTension: 0.1,
            backgroundColor: "rgba(29, 202, 255, 0.75)",
            borderColor: "rgba(29, 202, 255, 1)",
            pointHoverBackgroundColor: "rgba(29, 202, 255, 1)",
            pointHoverBorderColor: "rgba(29, 202, 255, 1)",
            data: humidityArray
          }
        ]
      };

      var co2Data = {
        labels: dateArray,
        datasets: [
          {
            label: "CO2-Gehalt",
            fill: true,
            lineTension: 0.1,
            backgroundColor: "rgba(211, 72, 54, 0.75)",
            borderColor: "rgba(211, 72, 54, 1)",
            pointHoverBackgroundColor: "rgba(211, 72, 54, 1)",
            pointHoverBorderColor: "rgba(211, 72, 54, 1)",
            data: co2Array
          }
        ]
      };

			var ctx1 = $("#tempCanvas");
      var ctx2 = $("#humidityCanvas");
      var ctx3 = $("#co2Canvas");

			new Chart(ctx1, {
				type: 'line',
				data: tempData,
        options: {
          responsive: true,
          scales: {
            yAxes: [
              {
                ticks: {
                  max: maxTemp + 0.1
                }
              }
            ]
          }
        }
			});
      new Chart(ctx2, {
        type: 'line',
        data: humidityData,
        options: {
          responsive: true,
          scales: {
            yAxes: [
              {
                ticks: {
                  max: maxHumidity + 0.1
                }
              }
            ]
          }
        }
      });
      new Chart(ctx3, {
        type: 'line',
        data: co2Data,
        options: {
          responsive: true,
          scales: {
            yAxes: [
              {
                ticks: {
									beginAtZero: true
                }
              }
            ]
          }
        }
      });
		},
		error : function(data) {

		}
	});
});
