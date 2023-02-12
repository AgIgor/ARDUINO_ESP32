const char webpageCore[] =
R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Temperatura</title>
</head>

<body>
    <h1 id="sats">GPS</h1>
    <h2 id="pos"></h2> 
    <h2 id="alt"></h2>        
</body>
<script>
    addEventListener('DOMContentLoaded', () => {
        getTemp()
        console.log('ok')
    })

    function getTemp() {
        fetch('http://192.168.15.6/gps')
            .then(response => {
                response.json()
                    .then(data => {
                        console.log(data)
                        sats.innerText = `GPS: ${data.sats} Hora: ${data.time.hora}:${data.time.minuto}:${data.time.segundo}`
                        pos.innerText = `Lat: ${data.gps.lat} Lon: ${data.gps.long}`
                        alt.innerText = `Alt: ${data.alt} Vel: ${data.speed}`
                        
                        //document.querySelector('title').innerText = `Temp: ${data.sensor}`
                    })
                    .catch(err => console.log(err))
            })

    }
    setInterval(() => {
        getTemp()
    }, 3000);
</script>

</html>
)=====";
