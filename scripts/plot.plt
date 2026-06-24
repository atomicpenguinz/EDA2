set title 'Comparação'
set xlabel 'Tamanho da entrada'
set ylabel 'Número de comparações'
set datafile separator ','
set grid
set key left top
set border lw 1.5
set terminal pngcairo size 1200,800
# set logscale xy
set key autotitle columnhead # caso tenha primeira linha com descrição

if (!exists("ARQ")) ARQ = "../src/insercao.csv"
if (!exists("OUT")) OUT = "grafico_insercao.png"

set output OUT

plot ARQ using 1:2 with linespoints lw 2 title 'AVL', \
    '' using 1:3 with linespoints lw 2 title 'Rubro-negra', \
    '' using 1:4 with linespoints lw 2 title 'B (ordem 1)', \
    '' using 1:5 with linespoints lw 2 title 'B (ordem 5)', \
    '' using 1:6 with linespoints lw 2 title 'B (ordem 10)'

