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
    <h1 id="saida"></h1>
</body>
<script>
    addEventListener('onload', getTemp())

    function getTemp() {
        console.log('ok')
        fetch('http://192.168.15.10/temp')
            .then(response => {
                response.json()
                    .then(data => {
                        console.log(data.sensor)
                        saida.innerText = data.sensor
                        document.querySelector('title').innerText = `Temp: ${data.sensor}`
                    })
            })
            .catch(err => console.log(err))
    }
    setInterval(() => {
        getTemp()
    }, 3000);
</script>

</html>
)=====";
