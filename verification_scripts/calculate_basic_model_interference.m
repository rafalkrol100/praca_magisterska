% MATLAB code for calculating interference from model
Pt = 10; %transmitting power in mW
r_beta_alfa = 1; %scale of compensated power (1 due to no compensation)
Rv = 100; %cell size[m]

value_from_model_with_basic_setup = calculate_sum(Rv, Pt, r_beta_alfa);

function [sum_dbm] = calculate_sum(Rv, Pt, r_beta_alfa)
sum = 0;
for i = 1:18
    interference_linear = calculate_single_cell_interference(Rv,i, Pt, r_beta_alfa);
    disp("interference_linear: ")
    disp(interference_linear)
    interference_logaritmic = 10 * log10(interference_linear);
    disp("interference_logaritmic: ")
    disp(interference_logaritmic)
    Db = calculate_Db(i, Rv);
    disp("Db: ")
    disp(Db)
    shadowing = calculate_single_cell_shadowing(Db);
    disp("shadowing: ")
    disp(shadowing)
    interference_with_shadowing = interference_logaritmic - shadowing;
    disp("interference_with_shadowing: ")
    disp(interference_with_shadowing)
    interference_with_shadowing_linear = 10^(interference_with_shadowing/10);
    disp("interference_with_shadowing_linear: ")
    disp(interference_with_shadowing_linear)
    sum = sum + interference_with_shadowing_linear;
    disp("-----------------------------------------------------")
end
sum_dbm = 10*log10(sum)-5;
end

function [q] = calculate_single_cell_interference(Rv,l,Pt,r_beta_alfa)
rho = calculate_rho(Rv);
db = calculate_Db(l, Rv);
polarfun = @(theta,r) Pt.*((sqrt(db^2.+r.^2.-2.*r.*db.*cos(pi-theta)).^(-2/2))).*rho.*r;
q = integral2(polarfun,0,2*pi,0,Rv);
end

function [shadowing] = calculate_single_cell_shadowing(Db)
shadowing = 38.57 + 10 * 2 * log10(Db);
end


function [rho] = calculate_rho(Rv)
rho = 1/(pi*Rv^2);
end

function [Db] = calculate_Db(l, Rv)
if l <= 6
    Db = 2 * Rv;
end
if l == 7 || l == 9 || l == 11 || l == 13 || l == 15 || l == 17
    Db = 4 * Rv;
end
if l == 8 || l == 10 || l == 12 || l == 14 || l == 16 || l == 18
    Db = 2 * sqrt(3) * Rv;
end
end