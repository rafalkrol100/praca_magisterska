% MATLAB code for calculating interference from model
Pt = 10; %transmitting power in mW
r_beta_alfa = 1; %scale of compensated power (1 due to no compensation)
Rv = 100; %cell size[m]
Rx = 0.75*Rv;

value_from_model_with_comp_enabled = calculate_sum(Rv, Pt, Rx);

function [sum_dbm] = calculate_sum(Rv, Pt, Rx)
sum = 0;
for i = 1:18
    for j = 1:6
        interference_linear = calculate_single_cell_interference(Rv,i, Pt, Rx, j);
        interference_logaritmic = 10 * log10(interference_linear);       
        Db = calculate_Db(i, Rv);       
        shadowing = calculate_single_cell_shadowing(Db);        
        interference_with_shadowing = interference_logaritmic - shadowing;        
        interference_with_shadowing_linear = 10^(interference_with_shadowing/10);       
        sum = sum + interference_with_shadowing_linear;

        disp("interference_linear: ")
        disp(interference_linear)
        disp("interference_logaritmic: ")
        disp(interference_logaritmic)
        disp("Db: ")
        disp(Db)
        disp("shadowing: ")
        disp(shadowing)
        disp("interference_with_shadowing: ")
        disp(interference_with_shadowing)
        disp("interference_with_shadowing: ")
        disp(interference_with_shadowing)
        disp("-----------------------------------------------------")
    end
end

for j = 2:6
    interference_linear = calculate_single_cell_interference(Rv,i, Pt, Rx, j);
    interference_logaritmic = 10 * log10(interference_linear);       
    Db = Rv/2;       
    shadowing = calculate_single_cell_shadowing(Db);        
    interference_with_shadowing = interference_logaritmic - shadowing;        
    interference_with_shadowing_linear = 10^(interference_with_shadowing/10);       
    sum = sum + interference_with_shadowing_linear;
    
    disp("interference_linear: ")
    disp(interference_linear)
    disp("interference_logaritmic: ")
    disp(interference_logaritmic)
    disp("Db: ")
    disp(Db)
    disp("shadowing: ")
    disp(shadowing)
    disp("interference_with_shadowing: ")
    disp(interference_with_shadowing)
    disp("interference_with_shadowing: ")
    disp(interference_with_shadowing)
    disp("-----------------------------------------------------")
end


sum_dbm = 10*log10(sum)-5;
end

function [q] = calculate_single_cell_interference(Rv,l,Pt, Rx, j)
rho = calculate_rho(Rv);
db = calculate_Db(l, Rv);
omega_zero = calculate_omega_zero(j);
polarfun = @(theta,r) Pt.*((sqrt(db^2.+r.^2.-2.*r.*db.*cos(pi-theta)).^(-2/2))).*rho.*r.*(cos(r.*sin(theta)./(2.*sqrt(db.^2+r.^2-2.*r.*db.*cos(pi-theta)) + omega_zero)).^(-6/(20.*log10(cos(pi/12)))));

q1 = integral2(polarfun,-pi/6,pi/6,0,Rx);
q2 = 0.75*integral2(polarfun,-pi/6,pi/6,Rx,Rv);
q = q1+q2;
end

function [shadowing] = calculate_single_cell_shadowing(Db)
shadowing = 38.57 + 10 * 2 * log10(Db);
end


function [rho] = calculate_rho(Rv)
rho = 6/(pi*Rv^2);
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

function [omega_zero] = calculate_omega_zero(j)
omega_zero = (j - 1) * ((2*pi)/6);
end