import socket
import time
import tkinter as tk
from tkinter import messagebox
import threading  # Bu satırı ekleyin

# Varsayılan Değerler
HOST = "192.168.1.8"  # Bağlanacağınız sunucu IP adresi
PORT = 7  # Sunucunun dinlediği port
BUFFER_SIZE = 4096  # Alınacak veri boyutu
TIME_LIMIT = 10  # Bağlantı süresi (sn)

# Tkinter Arayüzü
root = tk.Tk()
root.title("TCP Veri Alıcı")
root.geometry("350x300")

# Etiket ve Giriş Alanları
tk.Label(root, text="IP Adresi:").pack()
entry_host = tk.Entry(root)
entry_host.insert(0, HOST)
entry_host.pack()

tk.Label(root, text="Port:").pack()
entry_port = tk.Entry(root)
entry_port.insert(0, str(PORT))
entry_port.pack()

tk.Label(root, text="Buffer Boyutu (byte):").pack()
entry_buffer = tk.Entry(root)
entry_buffer.insert(0, str(BUFFER_SIZE))
entry_buffer.pack()

tk.Label(root, text="Bağlantı Süresi (sn):").pack()
entry_time = tk.Entry(root)
entry_time.insert(0, str(TIME_LIMIT))
entry_time.pack()

label_status = tk.Label(root, text="Bağlantı Bekleniyor...", font=("Arial", 12))
label_status.pack(pady=5)

label_speed = tk.Label(root, text="Hız: 0.00 MB/s", font=("Arial", 12))
label_speed.pack(pady=5)

label_total = tk.Label(root, text="Toplam Veri: 0.00 MB", font=("Arial", 12))
label_total.pack(pady=5)

total_data_received = 0
start_time = None

def receive_data_from_server():
    global total_data_received, start_time

    try:
        HOST = entry_host.get()
        PORT = int(entry_port.get())
        BUFFER_SIZE = int(entry_buffer.get())
        TIME_LIMIT = int(entry_time.get())

        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((HOST, PORT))
        label_status.config(text=f"Bağlantı Kuruldu: {HOST}:{PORT}")
        

        total_data_received = 0
        start_time = time.time()

        def update_speed():
            while time.time() - start_time < TIME_LIMIT:
                elapsed_time = time.time() - start_time
                speed = (total_data_received / 1024 / 1024) / elapsed_time if elapsed_time > 0 else 0
                label_speed.config(text=f"Hız: {speed:.2f} MB/s")
                label_total.config(text=f"Toplam Veri: {total_data_received / 1024 / 1024:.2f} MB")
                root.update_idletasks()
                time.sleep(1)

        threading.Thread(target=update_speed, daemon=True).start()
        client_socket.send("ttuzz".encode()) 
        while time.time() - start_time < TIME_LIMIT:
            data = client_socket.recv(BUFFER_SIZE)
            if not data:
                break
            total_data_received += len(data)

        label_status.config(text="Bağlantı Kapandı")
        client_socket.close()

    except Exception as e:
        messagebox.showerror("Hata", f"Sunucuya bağlanılamadı: {e}")

tk.Button(root, text="Veri Al", command=lambda: threading.Thread(target=receive_data_from_server, daemon=True).start()).pack(pady=10)

root.mainloop()
