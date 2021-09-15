h_damp = miraudio('Audio/harddamp.wav', 'Center');
h_damp_pick = miraudio('Audio/harddamppick.wav', 'Center');

m_damp = miraudio('Audio/mediumdamp.wav', 'Center');
m_damp_pick = miraudio('Audio/mediumdamppick.wav', 'Center');

s_damp = miraudio('Audio/softdamp.wav', 'Center');
s_damp_pick = miraudio('Audio/softdamppick.wav', 'Center');

picking = miraudio('Audio/pickingdemo.wav', 'Center');
picking_rms = mirrms('Audio/pickingdemo.wav', 'Frame');
% f = mirframe(a, 'Length', 3.5, 's');