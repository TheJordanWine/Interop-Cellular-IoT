function pingResource(e, ae) {
    e.preventDefault();
    $.ajax({
        method: 'POST',
        url: '/api/ping',
        data: `resourceName=${ae}&ishttps=${localStorage.getItem('ishttps')}&om2mhost=${localStorage.getItem('om2mhost')}&om2mport=${localStorage.getItem('om2mport')}`,
        success: function(data) {
            M.toast({
                html: data.message
            });
        },
        error: function(data) {
            M.toast({
                html: data.message
            });
        }
    })
}