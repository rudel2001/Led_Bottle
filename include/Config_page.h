const char Configpage_1[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <style>
      @import url("https://fonts.googleapis.com/css2?family=Balsamiq+Sans&display=swap");
      body {
        font-family: "Balsamiq Sans", cursive;
        text-align: center;
        background: rgba(176, 165, 235, 0.3);
      }
      @import url("https://fonts.googleapis.com/css2?family=Roboto:wght@300&display=swap");
      table {
        border-spacing: 15px;
      }
      table.center {
        margin-left: auto;
        margin-right: auto;
      }
      input {
        border-radius: 9px;
        background: rgb(237, 237, 237);
        font-size: 16px;
      }
      button {
        width: 200px;
        height: 40px;
        font-family: "Roboto", sans-serif;
        margin: 0px 10px;
        border-radius: 100px;
        font-size: 16px;
      }
    </style>
  </head>
  <body>
    <center><h1>Configuracion de red</h1></center>
    <form>
      <fieldset>
        <div>
          <label for="ssid">Nombre de la red: </label>
          <input value="" id="ssid" placeholder="RED" />
        </div>
        <div><p></p></div>
        <div>
          <label for="password">Clave: </label>
          <input value="" id="password" placeholder="CLAVE" />
        </div>
        <div>
          <div><p></p></div>
          <button
            class="primary"
            id="savebtn"
            type="button"
            onclick="myFunction()"
          >
            Guardar configuración
          </button>
        </div>
        <div><p id="cambio_red"></p></div>
      </fieldset>
    </form>
  </body>
  <center>
    <h3>El IP asignado por su network es:</h3>
    <p id="ip_asignado"></p>
    <h3>La red que se está usando:</h3>
    <p id="red_uso"></p>
    <h3>El password que se está usando es:</h3>
    <p id="pass_uso"></p>
  </center>
  <script>
    function myFunction() {
      console.log("button was clicked!");
      var ssid = document.getElementById("ssid").value;
      var password = document.getElementById("password").value;
      var data = { ssid: ssid, password: password };
      var xhr = new XMLHttpRequest();
      var url = "/settings";
      var mensaje_boton =
        "Cierre esta página, apague la botella, vuelva a encenderla, espere 1 minuto, conectese a la red PASATIEMPO_?? y vuelva a entrar a la configuración para leer datos actualizados";
      xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          if (xhr.responseText != null) {
            console.log(xhr.responseText);
          }
        }
      };
      xhr.open("POST", url, true);
      xhr.send(JSON.stringify(data));
      document.getElementById("cambio_red").innerHTML = mensaje_boton;
    }
    function F_IP_uso() {
      console.log("Se recibió la data de IP");
      var xhr = new XMLHttpRequest();
      var url = "/loadip";

      xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          ip_data = this.responseText.split("***");
          document.getElementById("ip_asignado").innerHTML = ip_data[0];
          document.getElementById("red_uso").innerHTML = ip_data[1];
          document.getElementById("pass_uso").innerHTML = ip_data[2];
        }
      };
      xhr.open("GET", url, true);
      xhr.send();
    }
    function F_Red_uso() {
      console.log("Se recibió la data de red");
      var xhr = new XMLHttpRequest();
      var url = "/loadred";

      xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          red_data = this.responseText.split("***");
          document.getElementById("red_uso").innerHTML = red_data[1];
        }
      };
      xhr.open("GET", url, true);
      xhr.send();
    }
    function F_Pass_uso() {
      console.log("Se recibió la data de password");
      var xhr = new XMLHttpRequest();
      var url = "/loadpass";

      xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          pass_data = this.responseText;
          document.getElementById("pass_uso").innerHTML = pass_data;
        }
      };
      xhr.open("GET", url, true);
      xhr.send();
    }
    document.addEventListener("DOMContentLoaded", F_IP_uso, false);
  </script>
</html>
)=====";