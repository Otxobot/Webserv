#!/usr/bin/env python3
import cgi

def calculate():
    form = cgi.FieldStorage()
    num1 = form.getvalue('num1')
    num2 = form.getvalue('num2')
    operation = form.getvalue('operation')
    result = None
    if num1 and num2 and operation:
        try:
            num1 = float(num1)
            num2 = float(num2)
            if operation == '+':
                result = num1 + num2
            elif operation == '-':
                result = num1 - num2
            elif operation == '*':
                result = num1 * num2
            elif operation == '/':
                result = num1 / num2
        except ValueError:
            result = 'Invalid input'

    print('<html><body><h1>Result: {}</h1></body></html>'.format(result))


if __name__ == '__main__':
    calculate()