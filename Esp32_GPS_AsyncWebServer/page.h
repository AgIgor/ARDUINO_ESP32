const char webpageCore[] =
R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gps Tracker</title>
    <style>
        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        body {
            height: 100vh;
        }

        #container {
            display: flex;
            flex-direction: column;
            height: 100%;
        }

        header {
            background-color: #3a3a3a;
            width: 100%;
            color: #fff;
            padding: 15px 30px;
            display: flex;
            align-items: center;
            height: 4rem;
            /* justify-content: center; */
        }

        main {
            flex: 1;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
        }

        #content {
            background-color: #7159c1cb;
            display: flex;
            align-items: center;
            justify-content: top;
            flex-direction: column;
            width: 100%;
            height: 100%;
        }

        footer {
            display: flex;
            align-items: center;
            justify-content: center;
            background-color: #7159c1;
            padding: 15px 30px;
            color: #fff;
            margin-top: auto;
        }

        @media only screen and (max-width: 600px) {
            body {
                height: 93.2vh;
            }
        }
    </style>
</head>

<body>
    <div id="container">
        <header>
            <h1 id="sats">GPS</h1>
            <span id="loading">
                <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"
                    style="margin: auto; background: none; display: block; shape-rendering: auto;" width="98px"
                    height="40px" viewBox="0 0 100 100" preserveAspectRatio="xMidYMid">
                    <circle cx="28" cy="75" r="11" fill="#1b0629">
                        <animate attributeName="fill-opacity" repeatCount="indefinite" dur="0.8695652173913042s"
                            values="0;1;1" keyTimes="0;0.2;1" begin="0s"></animate>
                    </circle>

                    <path d="M28 47A28 28 0 0 1 56 75" fill="none" stroke="#dc93f8" stroke-width="10">
                        <animate attributeName="stroke-opacity" repeatCount="indefinite" dur="0.8695652173913042s"
                            values="0;1;1" keyTimes="0;0.2;1" begin="0.08695652173913043s"></animate>
                    </path>
                    <path d="M28 25A50 50 0 0 1 78 75" fill="none" stroke="#1b0629" stroke-width="10">
                        <animate attributeName="stroke-opacity" repeatCount="indefinite" dur="0.8695652173913042s"
                            values="0;1;1" keyTimes="0;0.2;1" begin="0.17391304347826086s"></animate>
                    </path>
                </svg>
            </span>

        </header>
        <main>
            <div id="content">
                <h2 id="pos"></h2>
                <h2 id="alt"></h2>
            </div>
        </main>
        <footer>
            <b>
                <p id="leg"></p>
            </b>
        </footer>
    </div>
</body>
<script>
    addEventListener('DOMContentLoaded', () => {
        getTemp()
        console.log('loaded')
    })

    function getTemp() {
        fetch('http://192.168.15.175/gps')
            .then(response => {
                response.json()
                    .then(data => {
                        console.log(data)
                        if (data.GPS === 'Sem Dados') {
                            loading.setAttribute('hidden', 'true')
                        }
                        try {
                            sats.innerText = `GPS: ${data.sats} Hora: ${data.time.hora}:${data.time.minuto}:${data.time.segundo}`
                            pos.innerText = `Lat: ${data.gps.lat} Lon: ${data.gps.long}`
                            alt.innerText = `Alt: ${data.alt} Vel: ${data.speed}`
                            loading.setAttribute('hidden', 'true')

                        }
                        catch {
                            (e) => console.log(e)
                            document.querySelector('title').innerText = `${data.GPS}`
                            leg.innerText = data.GPS

                        }
                    }).catch(err => console.log(err))
            }).catch(err => {
                leg.innerText = 'Sem resposta do ESP'
                // loading.removeAttribute('hidden')
            })
    }

    setInterval(() => {
        loading.removeAttribute('hidden')
        getTemp()
    }, 3000);
</script>

</html>
)=====";
