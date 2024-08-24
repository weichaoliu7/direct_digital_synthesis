clear all;
clc;

% Design parameters
N = 4;                        % Filter order
Rp = 0.1;                     % Passband ripple (dB)
Rs = 45;                      % Stopband attenuation (dB)
f_sampling = 120e6;           % Sampling frequency (Hz)
f_cutoff = 1e5;               % Cutoff frequency (Hz)
T_sampling = 1/f_sampling;    % Sampling period

% Normalize the cutoff frequency
Wn = (2 * f_cutoff) / f_sampling;  % Normalized cutoff frequency (0 to 1 scale)

% Design the Elliptic Lowpass Filter using ellip function
[b_z, a_z] = ellip(N, Rp, Rs, Wn, 'low');

% Display numerator and denominator coefficients of discrete transfer function
disp('numerator coefficients of discrete transfer function b_z:');
disp(b_z);
disp('denominator coefficients of discrete transfer function a_z:');
disp(a_z);

%  Write numerator and denominator coefficients
fileID = fopen('D:\graduate\vsCode\C\direct_digital_synthesis\table\coefficients.txt', 'w');
fprintf(fileID, 'coefficients:\n');
fprintf(fileID, '%f ', b_z);
fprintf(fileID, '\n');
fprintf(fileID, '%f ', a_z);
fprintf(fileID, '\n');
fclose(fileID);

