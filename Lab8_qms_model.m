% David Taylor, Georgia Tech, 7/12/23

clc, clear, close all
format shortE

%% settings (1 = yes, 0 = no)

% use 0 for model of crane plant
% use 1 for model of rocket plant

pendulum_up = 1;

%% link 0

J0_rotor = 4e-6;
J0_hub = 0.6e-6;

m0_housing = 0.067;
L0_housing = 0.038;
r0_housing = 0.014;

m0_housing_s = m0_housing*L0_housing/(r0_housing+L0_housing);
m0_housing_e = m0_housing*r0_housing/(r0_housing+L0_housing);
J0_housing_s = (1/12)*m0_housing_s*(6*r0_housing^2+L0_housing^2);
J0_housing_e = (1/2)*m0_housing_e*(r0_housing^2+L0_housing^2);
J0_housing = J0_housing_s+J0_housing_e;

J0 = J0_rotor+J0_hub+J0_housing;

%% link 1

m1 = 0.025;
L1_total = 0.11;
L1_offset = 0.02;
l1 = 0.5*L1_total-L1_offset;
I1 = (1/12)*m1*L1_total^2;
J1 = I1+m1*l1^2;

%% link 2

m2 = 0.024;
L2_total = 0.129;
L2_offset = 0.008;
l2 = 0.5*L2_total-L2_offset;
I2 = (1/12)*m2*L2_total^2;
J2 = I2+m2*l2^2;

%% other

L1 = 0.085;
g = 9.81;
Km = 0.042;
Rm = 8.4;
d1 = 1e-3;
d2 = 1e-5;

%% plant model parameters (voltage input)

J11 = J0+J1+m2*L1^2;
J12 = m2*L1*l2;
J21 = J12;
J22 = J2;
T22 = m2*g*l2;
den = J11*J22-J12*J21;

if pendulum_up == 1
    a32 = J12*T22/den;
    a33 = -J22*(d1+Km^2/Rm)/den;
    a34 = -J12*d2/den;
    a42 = J11*T22/den;
    a43 = -J21*(d1+Km^2/Rm)/den;
    a44 = -J11*d2/den;
    b3 = (J22*Km/Rm)/den;
    b4 = (J21*Km/Rm)/den;
else
    a32 = J12*T22/den;
    a33 = -J22*(d1+Km^2/Rm)/den;
    a34 = J12*d2/den;
    a42 = -J11*T22/den;
    a43 = J21*(d1+Km^2/Rm)/den;
    a44 = -J11*d2/den;
    b3 = (J22*Km/Rm)/den;
    b4 = -(J21*Km/Rm)/den;
end

%% full-order plant model (voltage input)

A = [0,0,1,0;0,0,0,1;0,a32,a33,a34;0,a42,a43,a44]
B = [0;0;b3;b4]
C = [1,0,0,0]

P = tf(ss(A,B,C,0))
[P_zeros,P_poles,P_gain] = zpkdata(P,'v')


% Regulator Bandwidth λr 8.0 rad/s
desired_eigenvalues = [-8;-8;-8;-8;-8]
A_script = [A zeros(4,1); C 0]
B_script = [B; 0]
K = acker(A_script, B_script, desired_eigenvalues)
