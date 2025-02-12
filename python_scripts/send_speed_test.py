import tkinter as tk
from tkinter import filedialog, messagebox
import socket
import time
import threading
import os

def select_file():
    file_path = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
    entry_file.delete(0, tk.END)
    entry_file.insert(0, file_path)
    
    if file_path:
        file_size = os.path.getsize(file_path)
        file_size_label.config(text=f"Dosya Boyutu: {file_size / 1024:.2f} KB")

def send_file():
    ip = entry_ip.get()
    port = entry_port.get()
    file_path = entry_file.get()
    chunk_size = int(entry_chunk.get())
    
    if not ip or not port or not file_path:
        messagebox.showerror("Hata", "Lütfen tüm alanları doldurun")
        return
    
    try:
        port = int(port)
        with open(file_path, "r") as f:
            data = f.read().encode()
    except Exception as e:
        messagebox.showerror("Hata", f"Dosya okunurken hata: {e}")
        return
    
    progress_var.set(0)
    progress_label.config(text="Gönderim Başlıyor...")
    root.update_idletasks()
    
    def transfer():
        try:
            start_time = time.time()
            client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client.connect((ip, port))
            
            total_sent = 0
            total_size = len(data)
            
            for i in range(0, total_size, chunk_size):
                chunk = data[i:i+chunk_size]
                client.send(chunk)
                total_sent += len(chunk)
                progress_var.set((total_sent / total_size) * 100)
                progress_label.config(text=f"%{progress_var.get():.2f} Gönderildi")
                root.update_idletasks()
            
            client.close()
            end_time = time.time()
            
            duration = end_time - start_time
            speed = (total_sent / (1024 * 1024)) / duration if duration > 0 else 0
            
            progress_label.config(text="Gönderim Tamamlandı")
            messagebox.showinfo("Gönderim Tamamlandı", f"Süre: {duration:.2f} saniye\nHız: {speed:.2f} MB/s")
        except Exception as e:
            messagebox.showerror("Hata", f"Bağlantı hatası: {e}")
    
    threading.Thread(target=transfer, daemon=True).start()

# GUI oluşturma
root = tk.Tk()
root.title("Txt Dosya Gönderici")
root.geometry("400x380")

tk.Label(root, text="IP Adresi:").pack()
entry_ip = tk.Entry(root)
entry_ip.insert(0, "192.168.1.8")
entry_ip.pack()

tk.Label(root, text="Port:").pack()
entry_port = tk.Entry(root)
entry_port.insert(0, "7")
entry_port.pack()

tk.Label(root, text="Paket Boyutu:").pack()
entry_chunk = tk.Entry(root)
entry_chunk.insert(0, "1024")
entry_chunk.pack()

tk.Label(root, text="Dosya Seç:").pack()
entry_file = tk.Entry(root, width=40)
entry_file.pack()
tk.Button(root, text="Seç", command=select_file).pack()

file_size_label = tk.Label(root, text="Dosya Boyutu: -")
file_size_label.pack()

progress_var = tk.DoubleVar()
progress_label = tk.Label(root, text="Gönderim Durumu: Beklemede")
progress_label.pack()

tk.Button(root, text="Gönder", command=send_file).pack()

root.mainloop()