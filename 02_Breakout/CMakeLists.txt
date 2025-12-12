cmake_minimum_required(VERSION 3.16)
project(Breakout CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- SDL3 AYARLARI ---
# Hem Web hem Masaüstü için Static kullanmak en temizidir.
set(SDL_SHARED OFF CACHE BOOL "Build shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build static library" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Build the tests" FORCE)
set(SDL_DISABLE_INSTALL ON CACHE BOOL "Don't install SDL" FORCE)

# --- KAYNAK KODU İNDİRME (FetchContent) ---
include(FetchContent)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

# --- EXECUTABLE ---
# Dosyalarını buraya ekliyorsun
add_executable(${PROJECT_NAME}
  main.cpp
  Game.cpp
  SoundManager.cpp
  Paddle.cpp
  Ball.cpp
)

# --- KÜTÜPHANE BAĞLAMA ---
# İndirdiğimiz SDL3'ü projeye bağlıyoruz
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)

# --- EMSCRIPTEN (WEB) ÖZEL AYARLARI ---
if(EMSCRIPTEN)
    # 1. Çıktı uzantısını .html yap (Yoksa .js üretir, tarayıcıda direk açamazsın)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    # 2. Emscripten Linker Bayrakları
    target_link_options(${PROJECT_NAME} PRIVATE
        # Emscripten'in kendi içindeki eski SDL portunu devre dışı bırak.
        # Bizim FetchContent ile indirdiğimiz SDL3'ü kullan.
        "SHELL:-s USE_SDL=0" 
        
        # WebAssembly çıktısı üret
        "SHELL:-s WASM=1"
        
        # Bellek yetmezse tarayıcının daha fazla RAM ayırmasına izin ver
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    )
    
    # (Opsiyonel) Konsol yerine varsayılan bir shell kullanmak istersen:
    # "SHELL:--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
endif()
