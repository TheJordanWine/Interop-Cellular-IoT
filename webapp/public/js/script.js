"use strict";

/**
 * CODE FOR THE INDEX PAGE
 */

var statusButton = document.getElementById('status');
if (!!statusButton) {
    $.ajax({
        method: "GET",
        url: "/status",
        success: function success(data) {
            // setTimeout(function () {
            //     if (data == 'true') {
            //         statusButton.innerHTML = 'IN-CSE Server Status<span class="badge badge-success">Running</span>';
            //     } else {
            //         statusButton.innerHTML = 'IN-CSE Server Status<span class="badge badge-danger">Not Running</span>';
            //     }
            // }, 2000);
        }
    });
}

var intervalJSONGetter = function intervalJSONGetter() {};
/**
 * Functionaitly to change to play to stop and stop to play
 */
var resumeBtn = document.getElementById('resume');
if (!!resumeBtn) {
    resumeBtn.addEventListener('click', function () {
        if (resumeBtn.innerHTML.indexOf('play_arrow') > -1) {
            intervalJSONGetter = setInterval(function () {
                getJSON();
            }, 3000);
            resumeBtn.innerHTML = '<i class="material-icons">stop</i>';
        } else {
            clearInterval(intervalJSONGetter);
            resumeBtn.innerHTML = '<i class="material-icons">play_arrow</i>';
        }
    });
}

var limitChart = 20;

var rangeLimit = document.getElementById('range');
if (!!rangeLimit) {
    rangeLimit.onchange = function (e) {
        limitChart = e.target.value;
    };
}

/**
 * CODE FOR THE DATA PAGE
 */
var resourceName;
try {
    var queryString = location.search.split('?').filter(function (el) {
        return el !== '';
    });
    queryString.forEach(function (el) {
        if (el.split('=')[0] == 'ae') {
            resourceName = el.split('=')[1];
        }
    });
} catch (e) {
    console.log('Unable to parse Query String');
    console.error(e);
    resourceName = "default";
}

/**
 * Adds the data to the chart
 */
function updateChart(AE_JSON) {
    myLineChart.data.datasets[0].data = AE_JSON.map(function (el) {
        return el.data;
    });
    myLineChart.data.labels = AE_JSON.map(function (el) {
        return el.date;
    });
    myLineChart.update();
}

function getJSON() {
    $.ajax({
        method: "GET",
        url: "/api/get/" + resourceName,
        success: function success(data) {
            data = data.reverse();
            data = data.slice(0, limitChart);
            data = data.reverse();
            data = data.map(function (el) {
                var respObj = {
                    date: new Date(el.date).toLocaleString().split(', ')[1],
                    data: JSON.parse(el.data).kWH
                };
                return respObj;
            });
            window.AE_JSON = data;
            updateChart(window.AE_JSON);
        },
        error: function(err) {
            M.toast({
                html: err.statusText
            });
        }
    });
}

function subscribe() {
    localStorage.setItem('ishttps', document.forms[0].ishttps.checked);
    localStorage.setItem('om2mhost', document.forms[0].om2mhost.value);
    localStorage.setItem('om2mport', document.forms[0].om2mport.value);
    localStorage.setItem('listenaddr', document.forms[0].listenaddr.value);
    localStorage.setItem('listenport', document.forms[0].listenport.value);
    $.ajax({
        type: "post",
        url: '/api/subscribe',
        data: jQuery('form').serialize(),
        contentType: "application/x-www-form-urlencoded",
        success: function (responseData, textStatus, jqXHR) {
            location.reload();
        },
        error: function (jqXHR, textStatus, errorThrown) {
            alert(errorThrown);
        }
    });
}
function deleteResource(resourceName) {
    // var userConfirm = confirm('Are you sure you want to delete the resource?');
    // if(userConfirm) {
        
    // }
    $.ajax({
        type: "post",
        url: '/api/delete',
        data: `resourceName=${resourceName}&ishttps=${localStorage.getItem('ishttps')}&om2mhost=${localStorage.getItem('om2mhost')}&om2mport=${localStorage.getItem('om2mport')}`,
        contentType: "application/x-www-form-urlencoded",
        success: function (responseData, textStatus, jqXHR) {
            location.reload();
        },
        error: function (jqXHR, textStatus, errorThrown) {
            alert(errorThrown);
        }
    });
}

var ctx = "myChart";

var data = {
    labels: [],
    datasets: [{
        label: "kWH Usage for today",
        fill: false,
        lineTension: 0.1,
        backgroundColor: "rgba(75,192,192,0.4)",
        borderColor: "rgba(75,192,192,1)",
        borderCapStyle: 'butt',
        borderDash: [],
        borderDashOffset: 0.0,
        borderJoinStyle: 'miter',
        pointBorderColor: "rgba(75,192,192,1)",
        pointBackgroundColor: "#fff",
        pointBorderWidth: 1,
        pointHoverRadius: 5,
        pointHoverBackgroundColor: "rgba(75,192,192,1)",
        pointHoverBorderColor: "rgba(220,220,220,1)",
        pointHoverBorderWidth: 2,
        pointRadius: 5,
        pointHitRadius: 10,
        data: []
    }]
};
var option = {
    showLines: true
};
var myLineChart;
if(!!document.querySelector('canvas#myChart')) {
    myLineChart = Chart.Line(ctx, {
        data: data,
        options: option
    });
}


if(typeof M !== 'undefined') {
    M.AutoInit();
}