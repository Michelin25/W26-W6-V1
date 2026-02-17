% motion_analysis.m
% Load Run.txt and Walking.txt (CSV-formatted text), plot x/y/z vs sample index,
% and compute/plot FFT magnitude spectra for both datasets.

close all
clear
clc

runData = readmatrix('Run.txt');
walkData = readmatrix('Walking.txt');
running2Data = readmatrix('running2');

% Keep first 3 columns as X, Y, Z.
runData = runData(:, 1:3);
walkData = walkData(:, 1:3);
running2Data = running2Data(:, 1:3);

% If you know your sampling rate (Hz), set it here.
% Using 1 Hz by default means the x-axis of FFT is in cycles/sample.
Fs = 1;

plotTimeDomain(runData, 'Running');
plotTimeDomain(walkData, 'Walking');

plotFrequencySpectrum(runData, Fs, 'Running');
plotFrequencySpectrum(walkData, Fs, 'Walking');
plotFrequencySpectrum(running2Data, Fs, 'Running 2');

% Compare Z-axis FFT between running and walking.
plotZAxisFftComparison(runData, walkData, Fs);

% Compare running and walking over time (Z-axis).
plotTemporalComparison(runData, walkData);


function plotTimeDomain(data, activityName)
    n = size(data, 1);
    idx = 1:n;

    figure('Name', [activityName ' - Time Domain']);
    plot(idx, data(:, 1), 'r', 'LineWidth', 1.2); hold on;
    plot(idx, data(:, 2), 'g', 'LineWidth', 1.2);
    plot(idx, data(:, 3), 'b', 'LineWidth', 1.2);
    grid on;
    xlabel('Sample Index');
    ylabel('Magnitude');
    title([activityName ' Data (X, Y, Z)']);
    legend('X', 'Y', 'Z', 'Location', 'best');
end


function plotZAxisFftComparison(runData, walkData, Fs)
    [fRun, pRun] = singleSidedSpectrum(runData(:, 3), Fs);
    [fWalk, pWalk] = singleSidedSpectrum(walkData(:, 3), Fs);

    figure('Name', 'Z-Axis FFT Comparison');
    plot(fRun, pRun, 'r', 'LineWidth', 1.2); hold on;
    plot(fWalk, pWalk, 'b', 'LineWidth', 1.2);
    grid on;
    xlabel('Frequency (Hz)');
    ylabel('Magnitude');
    title('Z-Axis FFT Comparison: Running vs Walking');
    legend('Running Z', 'Walking Z', 'Location', 'best');
end


function plotTemporalComparison(runData, walkData)
    nCompare = min(size(runData, 1), size(walkData, 1));
    idx = 1:nCompare;

    figure('Name', 'Temporal Comparison (Z-Axis)');
    plot(idx, runData(1:nCompare, 3), 'r', 'LineWidth', 1.2); hold on;
    plot(idx, walkData(1:nCompare, 3), 'b', 'LineWidth', 1.2);
    grid on;
    xlabel('Sample Index');
    ylabel('Magnitude');
    title('Temporal Comparison (Z-Axis): Running vs Walking');
    legend('Running Z', 'Walking Z', 'Location', 'best');
end


function [f, P1] = singleSidedSpectrum(signal, Fs)
    N = length(signal);
    sigFft = fft(signal);

    halfN = floor(N/2) + 1;
    f = Fs * (0:halfN-1) / N;

    P1 = abs(sigFft(1:halfN)) / N;
    if N > 2
        P1(2:end-1) = 2 * P1(2:end-1);
    end
end


function plotFrequencySpectrum(data, Fs, activityName)
    N = size(data, 1);

    % FFT per axis.
    Xf = fft(data(:, 1));
    Yf = fft(data(:, 2));
    Zf = fft(data(:, 3));

    % Single-sided spectrum.
    halfN = floor(N/2) + 1;
    f = Fs * (0:halfN-1) / N;

    P1x = abs(Xf(1:halfN)) / N;
    P1y = abs(Yf(1:halfN)) / N;
    P1z = abs(Zf(1:halfN)) / N;

    if N > 2
        P1x(2:end-1) = 2 * P1x(2:end-1);
        P1y(2:end-1) = 2 * P1y(2:end-1);
        P1z(2:end-1) = 2 * P1z(2:end-1);
    end

    figure('Name', [activityName ' - Frequency Spectrum']);
    plot(f, P1x, 'r', 'LineWidth', 1.2); hold on;
    plot(f, P1y, 'g', 'LineWidth', 1.2);
    plot(f, P1z, 'b', 'LineWidth', 1.2);
    grid on;
    xlabel('Frequency (Hz)');
    ylabel('Magnitude');
    title([activityName ' FFT Spectrum (X, Y, Z)']);
    legend('X', 'Y', 'Z', 'Location', 'best');
end