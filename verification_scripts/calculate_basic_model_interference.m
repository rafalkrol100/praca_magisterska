% MATLAB code for calculating interference from model
Pt = 10; %transmitting power in mW
r_beta_alfa = 1; %scale of compensated power (1 due to no compensation)
Ru = 1; %distance between interfering element and target BS
Rho = 1; %user density
Rv = 100; %cell size[m]
Db = 1; %cell distance[m]

value = calculate_sum(Rv, Pt, r_beta_alfa);

function [sum] = calculate_sum(Rv, Pt, r_beta_alfa)
sum = 0;
for i = 1:18
    interference_linear = calculate_single_cell_interference(Rv,i, Pt, r_beta_alfa);
    interference_logaritmic = 10 * log10(interference_linear);
    Db = calculate_Db(i, Rv);
    shadowing = calculate_single_cell_shadowing(Db);
    interference_with_shadowing = interference_logaritmic - shadowing;
    sum = sum + interference_with_shadowing;
end
end

function [q] = calculate_single_cell_interference(Rv,l,Pt,r_beta_alfa)
rho = calculate_rho(Rv);
db = calculate_Db(l, Rv);
polarfun = @(theta,r) Pt.*r_beta_alfa.*sqrt(db.^2.+r.^2.-2.*r.*db.*cos(pi-theta)).*rho.*r;
q = integral2(polarfun,0,2*pi,0,Rv);
end

function [shadowing] = calculate_single_cell_shadowing(Db)
shadowing = 38.57 + 10 * 3.9 * log10(Db);
end


function [rho] = calculate_rho(Rv)
rho = 1/(pi*Rv^2);
end

function [Db] = calculate_Db(l, Rv)
if l <= 6
    Db = Rv;
end
if l == 7 || l == 9 || l == 11 || l == 13 || l == 15 || l == 17
    Db = 2 * Rv;
end
if l == 8 || l == 10 || l == 12 || l == 14 || l == 16 || l == 18
    Db = 2 * sqrt(3) * Rv;
end
end