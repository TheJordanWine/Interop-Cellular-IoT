function getResource() {
    $.ajax({
        method: "GET",
        url: "http://127.0.0.1:8080/~/mn-cse",
        beforeSend: function(xhr){
            xhr.setRequestHeader('X-M2M-Origin', 'admin:admin');
            xhr.setRequestHeader('Accept', 'application/xml');
        },
        // headers: {
        //     "X-M2M-Origin": "admin:admin",
        //     "Accept": "application/xml"
        // },
        success: function (data) {
            document.getElementById('command-1').innerHTML = 'Response: ' + data;
        }
    });
}