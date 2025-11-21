import os

# --- AYARLAR ---
# Bu klasörleri görmezden gel
IGNORE_DIRS = {
    'build', 'build-web', 'emsdk', '.git', '.vs', 
    '.vscode', '.idea', '__pycache__', 'CMakeFiles'
}

# Sadece bu uzantıları al
ALLOWED_EXTS = {
    '.cpp', '.hpp', '.h', '.c',       # C++ Dosyaları
    '.txt', '.cmake',                 # CMake Dosyaları
    # '.ps1', '.py', '.bat', '.sh',     # Scriptler
    '.json', '.md'                    # Config ve Dökümanlar
}

OUTPUT_FILE = "FULL_PROJECT_CONTEXT.txt"

def is_text_file(filename):
    return any(filename.endswith(ext) for ext in ALLOWED_EXTS)

def pack():
    total_files = 0
    with open(OUTPUT_FILE, "w", encoding="utf-8") as out:
        out.write(f"PROJECT: Pong (SDL3)\n")
        out.write("="*50 + "\n\n")

        # Proje klasörünü gez
        for root, dirs, files in os.walk("."):
            # Ignore edilen klasörleri listeden çıkar (yerinde değiştirme)
            dirs[:] = [d for d in dirs if d not in IGNORE_DIRS]

            for file in files:
                if is_text_file(file):
                    file_path = os.path.join(root, file)
                    
                    # Dosyayı oku ve yaz
                    try:
                        with open(file_path, "r", encoding="utf-8") as f:
                            content = f.read()
                            
                        out.write(f"--- FILE START: {file_path} ---\n")
                        out.write(content + "\n")
                        out.write(f"--- FILE END: {file_path} ---\n\n")
                        
                        print(f"Eklendi: {file_path}")
                        total_files += 1
                    except Exception as e:
                        print(f"Hata (Atlandi): {file_path} -> {e}")

    print("-" * 30)
    print(f"Islem Tamam! {total_files} dosya '{OUTPUT_FILE}' icine paketlendi.")

if __name__ == "__main__":
    pack()
