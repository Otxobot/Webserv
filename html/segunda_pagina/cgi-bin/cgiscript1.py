#!/usr/bin/env python3
import os
import sys
import traceback
import cgi
# Función para manejar errores
def handle_error():
    print()
    print("<html>")
    print("<head>")
    print("<title>Error</title>")
    print("</head>")
    print("<body>")
    print("<h1>Error en el script CGI</h1>")
    print("<p>Ocurrió un error en la ejecución del script CGI.</p>")
    print("<h2>Traceback:</h2>")
    print("<pre>")
    traceback.print_exc(file=sys.stdout)
    print("</pre>")
    print("</body>")
    print("</html>")
try:
    # Obtiene los parámetros de la solicitud
    form = cgi.FieldStorage()
    # Genera la respuesta HTTP
    print("Content-Type: text/html")    # Indica el tipo de contenido
    print()                             # Línea en blanco para terminar los encabezados
    print("<html>")
    print("<head>")
    print("<title>CGI Script UNOOOOOOOOO</title>")
    print("</head>")
    print("<body>")
    print("<h1>Hello, CGI!</h1>")
    print("<p>Este es un script CGI ejecutado desde el servidor web.</p>")
    # Mostrar los parámetros recibidos
    print("<h2>Parametros recibidos:</h2>")
    print("<ul>")
    for key in form.keys():
        print(f"<li>{key}: {form.getvalue(key)}</li>")
    print("</ul>")
    print("</body>")
    print("</html>")
except Exception as e:
    handle_error()