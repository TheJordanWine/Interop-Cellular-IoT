/**
 * Unable to access IN-CSE oR MN-CSE Server via client
 * Needs to be done server side due to CORS 
 */

// function getResource() {
//     $.ajax({
//         method: "GET",
//         url: "http://127.0.0.1:8080/~/mn-cse",
//         beforeSend: function(xhr){
//             xhr.setRequestHeader('X-M2M-Origin', 'admin:admin');
//             xhr.setRequestHeader('Accept', 'application/xml');
//         },
//         // headers: {
//         //     "X-M2M-Origin": "admin:admin",
//         //     "Accept": "application/xml"
//         // },
//         success: function (data) {
//             document.getElementById('command-1').innerHTML = 'Response: ' + data;
//         }
//     });
// }

$.ajax({
    method: "GET",
    url: "/status",
    success: function (data) {
        setTimeout(() => {
            var status = data;
            if(data == 'true') {
                document.getElementById('status').innerHTML = 'IN-CSE Server Status<span class="badge badge-success">Running</span>';
            }else {
                document.getElementById('status').innerHTML = 'IN-CSE Server Status<span class="badge badge-danger">Not Running</span>';
            }
        }, 2000);
    }
});