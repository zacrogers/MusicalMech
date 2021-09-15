%% Plot damping after picking
% Load figures
h_after = hgload('harddamp_after.fig');
m_after = hgload('mediumdamping_after.fig');
s_after = hgload('softdamp_after.fig');

% Set subplots
figure;
h(1) = subplot(3,1,1);
title('Hard');
h(2) = subplot(3,1,2);
title('Medium');
h(3) = subplot(3,1,3);
title('Soft');

% Paste figs on subplots
copyobj(allchild(get(h_after, 'CurrentAxes')), h(1));
copyobj(allchild(get(m_after, 'CurrentAxes')), h(2));
copyobj(allchild(get(s_after, 'CurrentAxes')), h(3));

%% Plot damping during picking
% Load figures
h_during = hgload('harddamp_during.fig');
m_during = hgload('mediumdamping_during.fig');
s_during = hgload('softdamp_during.fig');

% Set subplots
figure;
h(1) = subplot(3,1,1);
title('Hard');
h(2) = subplot(3,1,2);
title('Medium');
h(3) = subplot(3,1,3);
title('Soft');

% Paste figs on subplots
copyobj(allchild(get(h_during, 'CurrentAxes')), h(1));
copyobj(allchild(get(m_during, 'CurrentAxes')), h(2));
copyobj(allchild(get(s_during, 'CurrentAxes')), h(3));

%% Plot picking examples
pick = hgload('picking.fig');
pick_rms = hgload('picking_rms.fig');

figure;
h(1) = subplot(2,1,1);
title('Amplitude');
h(2) = subplot(2,1,2);
title('RMS');

copyobj(allchild(get(pick, 'CurrentAxes')), h(1));
copyobj(allchild(get(pick_rms, 'CurrentAxes')), h(2));