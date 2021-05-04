const char Mainpage_1[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="icon" href="data:," />
    <style>
      #button1 {
        width: 100px;
        height: 40px;
      }
    </style>

    <title>Ledstival</title>
  </head>
  <body>
    <center>
      <h1>Control de luces Wifi</h1>
      <p>
        <button
          type="button"
          id="button1"
          class="button"
          onclick="MyFunction('/rainbow')"
        >
          Arcoiris
        </button>

        <button
          type="button"
          id="button1"
          class="button"
          onclick="MyFunction('/wave?r=255&g=32&b=10')"
        >
          Movimiento
        </button>
        <button
          type="button"
          id="button1"
          class="button"
          onclick="MyFunction('/on_off_fast')"
        >
          On Off
        </button>
      </p>
      <p>
        <button
          type="button"
          style="
            background-color: #ffffff;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=255&g=255&b=255')"
        ></button>
        <button
          type="button"
          style="
            background-color: #ff0000;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=255&g=0&b=0')"
        ></button>
        <button
          type="button"
          style="
            background-color: #990099;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=153&g=0&b=153')"
        ></button>
        <button
          type="button"
          style="
            background-color: #cc00cc;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=204&g=0&b=204')"
        ></button>
        <button
          type="button"
          style="
            background-color: #3333cc;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=51&g=51&b=204')"
        ></button>
        <button
          type="button"
          style="
            background-color: #006666;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=0&g=102&b=102')"
        ></button>
      </p>

      <p>
        <button
          type="button"
          style="
            background-color: #00ccff;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=0&g=204&b=255')"
        ></button>
        <button
          type="button"
          style="
            background-color: #00cc00;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=0&g=204&b=0')"
        ></button>
        <button
          type="button"
          style="
            background-color: #669900;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=102&g=153&b=0')"
        ></button>
        <button
          type="button"
          style="
            background-color: #ffff00;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=255&g=255&b=0')"
        ></button>
        <button
          type="button"
          style="
            background-color: #ff9933;
            color: white;
            width: 20px;
            height: 20px;
          "
          onclick="MyFunction('/setleds?r=255&g=153&b=51')"
        ></button>
      </p>

      <form action="/setbrightness">
        <p>
          <label for="brillo">Brillo:</label>
          <input
            type="range"
            id="brillo"
            name="br"
            min="0"
            max="200"
            value=""
          /><input type="submit" value="Enviar" />
        </p>
      </form>
      <p>
        <button
          type="button"
          id="button1"
          class="button"
          onclick="MyFunction('/ledsoff')"
        >
          Apagar
        </button>
      </p>
      <p id="msg_estatus"></p>
    </center>
  </body>
  <script>
    function MyFunction(url) {
      console.log("Se recibió la data de IP");
      var xhr = new XMLHttpRequest();

      xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          mensaje = this.responseText.split("***");
          document.getElementById("msg_estatus").innerHTML = mensaje;
        }
      };
      xhr.open("GET", url, true);
      xhr.send();
    }
  </script>
</html>
)=====";