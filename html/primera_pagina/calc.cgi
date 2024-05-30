#!/usr/bin/env python3
import cgi

print("Content-type:text/html\r\n\r\n")

# Get data from form
form = cgi.FieldStorage()
num1 = form.getvalue('num1')
num2 = form.getvalue('num2')

# Check if both numbers are provided
if num1 is not None and num2 is not None:
    # Perform calculation
    try:
        num1 = float(num1)
        num2 = float(num2)
        result = num1 + num2  # Change this line to perform different operations
        print("<script>parent.showResult('{}');</script>".format(result))
    except ValueError:
        print("<script>alert('Los números ingresados no son válidos');</script>")
else:
    print("<script>alert('Por favor, ingrese ambos números');</script>")
