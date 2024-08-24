# AD9851 Direct Digital Synthesis (DDS) System

This project simulates a Direct Digital Synthesis (DDS) system using the AD9851 DDS chip. DDS is a technique used to generate precise frequency waveforms, typically sine waves, by digitally synthesizing the waveform using a phase accumulator, a lookup table, and a DAC (Digital-to-Analog Converter). The output is then filtered.

## References

- [AD9851 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/AD9851.pdf)
- [GitHub Repository](https://github.com/Tektronica/direct_digital_synthesis_demo)

## Description

This simulation includes the following features:

- **Phase Accumulator**: Generates the phase values for the DDS.
- **Sine Wave Generation**: Uses a ROM table to generate sine wave samples.
- **DAC Simulation**: Converts the digital sine wave values into analog output.
- **Low-Pass Filter (LPF)**: Filters the output to remove high-frequency components.
- **Square Wave Generation**: Provides a square wave output based on the DAC output.
