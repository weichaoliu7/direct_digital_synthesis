# set the terminal type and output filename
set terminal png size 3840, 2160

# Plot 1: phase accumulator
set output 'phase_accumulator.png'
set title "phase accumulator"
set xlabel "time (s)"
set ylabel "phase accumulator"
set grid
plot "data.txt" using 1:2 with lines linecolor rgb "blue" linewidth 1 title "phase accumulator"

# Plot 2: dac output and filtered output
set output 'dac_output.png'
set title "dac output"
set xlabel "time (s)"
set ylabel "voltage (v)"
set grid
plot "data.txt" using 1:3 with lines linecolor rgb "blue" linewidth 1 title "dac output"

# Plot 3: square wave
set output 'square_wave.png'
set title "square wave"
set xlabel "time (s)"
set ylabel "square wave (logic level)"
set grid
plot "data.txt" using 1:4 with lines linecolor rgb "blue" linewidth 1 title "square wave"
