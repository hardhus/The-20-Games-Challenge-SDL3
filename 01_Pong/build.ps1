param (
  [ValidateSet("desktop", "web", "clean")]
  [string]$target = "desktop",  # Varsayılan: desktop
  [switch]$run                  # -run parametresi eklenirse oyunu başlatır
)

$ErrorActionPreference = "Stop"
$ProjectName = "Pong"

# --- MASAÜSTÜ DERLEME (WINDOWS) ---
if ($target -eq "desktop")
{
  Write-Host "=== MASAUSTU (WINDOWS) DERLENIYOR ===" -ForegroundColor Cyan

  if (-not (Test-Path "build"))
  { 
    New-Item -ItemType Directory -Path "build" | Out-Null 
  }

  # Sadece Makefile yoksa Configure yap (Hız kazanmak için)
  if (-not (Test-Path "build/build.ninja"))
  {
    cmake -G "Ninja" -S . -B build  
  }

  cmake --build build
    
  if ($run)
  {
    $exePath = ".\build\$ProjectName.exe"
    if (Test-Path $exePath)
    {
      Write-Host "=== OYUN BASLATILIYOR ($exePath) ===" -ForegroundColor Green
      & $exePath
    } else
    {
      Write-Host "HATA: $exePath bulunamadi! Derleme basarisiz olabilir mi?" -ForegroundColor Red
    }
  }
}

# --- WEB DERLEME (EMSCRIPTEN) ---
elseif ($target -eq "web")
{
  Write-Host "=== WEB (EMSCRIPTEN) DERLENIYOR ===" -ForegroundColor Magenta

  if (-not (Test-Path "build-web"))
  { 
    New-Item -ItemType Directory -Path "build-web" | Out-Null 
  }

  if (-not (Test-Path "build-web/build.ninja"))
  {
    emcmake cmake -G "Ninja" -S . -B build-web
  }

  cmake --build build-web
    
  if ($run)
  {
    Write-Host "=== SUNUCU BASLATILIYOR (http://localhost:8000/$ProjectName.html) ===" -ForegroundColor Green
    python -m http.server --directory build-web
  }
}

# --- TEMİZLİK ---
elseif ($target -eq "clean")
{
  Write-Host "Tum build klasorleri temizleniyor..." -ForegroundColor Red
  if (Test-Path "build")
  { Remove-Item -Recurse -Force "build" 
  }
  if (Test-Path "build-web")
  { Remove-Item -Recurse -Force "build-web" 
  }
  Write-Host "Temizlendi."
}
