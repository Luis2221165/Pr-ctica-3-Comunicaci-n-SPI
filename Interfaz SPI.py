import tkinter as tk
from tkinter import messagebox
import serial
import time

atmega328p = None  # Inicialmente, el objeto serial es None

# Función para conectar con el puerto serie
def connect():
    global atmega328p
    try:
        atmega328p = serial.Serial('COM4', 9600, timeout=1)  # Reemplaza 'COM4' con el puerto correcto
        messagebox.showinfo("Éxito", "Conexión exitosa")
    except serial.SerialException:
        messagebox.showerror("Error", "No se pudo conectar al puerto. Verifica el puerto y vuelve a intentarlo.")

# Función para leer valores de los potenciómetros
def read_values():
    if atmega328p and atmega328p.is_open:
        atmega328p.write(b'1\n')
        time.sleep(1)  # Espera un momento para asegurarse de que el mensaje se envíe correctamente
        atmega328p.write(b'si\n')
        time.sleep(1)  # Espera un momento para asegurarse de que el mensaje se envíe correctamente

        # Lee los valores enviados por el esclavo
        while atmega328p.in_waiting:
            line = atmega328p.readline().decode().strip()
            if "Valor de potenciometro 1" in line:
                val1 = line.split(":")[1].strip()
                pot1_label.config(text=f"Potenciómetro 1: {val1}")
            elif "Valor de potenciometro 2" in line:
                val2 = line.split(":")[1].strip()
                pot2_label.config(text=f"Potenciómetro 2: {val2}")
    else:
        messagebox.showerror("Error", "Primero debes conectarte al puerto.")

# Función para enviar un valor a los LEDs
def send_value():
    if atmega328p and atmega328p.is_open:
        try:
            value = int(entry.get())
            if 0 <= value <= 255:
                atmega328p.write(b'2\n')
                time.sleep(1)  # Espera un momento para asegurarse de que el mensaje se envíe correctamente
                atmega328p.write(f"{value}\n".encode())
            else:
                messagebox.showerror("Error", "Por favor ingrese un valor entre 0 y 255")
        except ValueError:
            messagebox.showerror("Error", "Por favor ingrese un número válido")
    else:
        messagebox.showerror("Error", "Primero debes conectarte al puerto.")

# Configuración de la interfaz gráfica
root = tk.Tk()
root.title("Control de Potenciómetros y LEDs")
root.configure(bg='blue')

frame = tk.Frame(root, bg='blue')
frame.pack(padx=100, pady=100)

connect_button = tk.Button(frame, text="Conectar", command=connect, bg='purple', fg='white', font=('Arial', 16, 'bold'), height=2, width=20)
connect_button.grid(row=0, column=0, pady=10)

pot1_label = tk.Label(frame, text="Potenciómetro 1: ---", bg='blue', fg='white', font=('Arial', 16, 'bold'))
pot1_label.grid(row=1, column=0, pady=5)

pot2_label = tk.Label(frame, text="Potenciómetro 2: ---", bg='blue', fg='white', font=('Arial', 16, 'bold'))
pot2_label.grid(row=2, column=0, pady=5)

read_button = tk.Button(frame, text="Leer Valores", command=read_values, bg='purple', fg='white', font=('Arial', 16, 'bold'), height=2, width=20)
read_button.grid(row=3, column=0, pady=10)

entry_label = tk.Label(frame, text="Valor para enviar:", bg='blue', fg='white', font=('Arial', 16, 'bold'))
entry_label.grid(row=4, column=0, pady=5)

entry = tk.Entry(frame)
entry.grid(row=5, column=0, pady=5)

send_button = tk.Button(frame, text="Enviar Valor", command=send_value, bg='purple', fg='white', font=('Arial', 16, 'bold'), height=2, width=20)
send_button.grid(row=6, column=0, pady=10)

root.mainloop()

# Cerrar el puerto serie al cerrar la ventana, si está conectado
if atmega328p and atmega328p.is_open:
    atmega328p.close()
