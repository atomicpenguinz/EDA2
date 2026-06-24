set title 'Comparação'
set xlabel 'Tamanho do conjunto'
set ylabel 'Esforço computacional'
set datafile separator ','
set grid
set key left top
set border lw 1.5
set terminal pngcairo size 1200,800
set key autotitle columnhead

LOG = 0
if(LOG) set logscale xy

arquivos = "../src/insercao.csv ../src/remocao.csv"
saidas = "insercao.png remocao.png"
titulos = "Inserção Remoção"

do for [i=1:words(arquivos)] {
    # set title word(titulos, i)
    arq = word(arquivos, i)
    out = "grafico_" . word(saidas, i)

    set output out

    plot arq using 1:2 with linespoints lw 2 title 'AVL', \
        '' using 1:3 with linespoints lw 2 title 'Rubro-negra', \
        '' using 1:4 with linespoints lw 2 title 'B (ordem 1)', \
        '' using 1:5 with linespoints lw 2 title 'B (ordem 5)', \
        '' using 1:6 with linespoints lw 2 title 'B (ordem 10)'
}
