gcc -Wall -O2 -I. -IC:/raylib/include -Isrc -o tubes_241524041_chal4.exe main.c src/firework.c src/algo/bresenham.c -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm

if ($?) {
    Write-Host "done" -ForegroundColor Green
} else {
    Write-Host "error" -ForegroundColor Red
}
