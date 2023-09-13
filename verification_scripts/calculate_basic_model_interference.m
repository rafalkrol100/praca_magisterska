% MATLAB code for calculating interference from model
Pt = 0.01; %transmitting power in W
r_beta_alfa = 1; %scale of compensated power (1 due to no compensation)
Ru = 1; %distance between interfering element and target BS
Rho = 1; %user density
Rv = 100; %cell size[m]
Db = 1; %cell size[m]

function [] = calculate_sum()

end

function [q] = calculate_single_cell_interference(l)
rho = calculate_rho(Rv);
db = calculate_Db(l);
polarfun = @(theta,r) Pt.*r_beta_alfa.*sqrt(db.^2.+r.^2.-2.*r.*db.*cos(pi-theta)).*rho.*r;
q = integral2(polarfun,0,2*pi,0,Rv);
end

function [] = calculate_single_cell_shadowing()

end


function [] = calculate_rho(r)

end

function [] = calculate_Db(l)

end