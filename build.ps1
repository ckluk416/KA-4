gcc -Wall -O2 -I. -IC:/raylib/include -Isrc -o tubes_241524041.exe main.c src/firework.c src/algo/bresenham.c -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm

if ($?) {
    Write-Host "jalan" -ForegroundColor Green
} else {
    Write-Host "gagal" -ForegroundColor Red
}
