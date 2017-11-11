% ****   Sistema teste de 14 barras  **** 

% Dados de Barra
 DBAR = [

num_nos 14

   2 1 1 1.0         .0       .0      .0  -999.9  9999.9     2.0    1.6      0.0
   3 1 1 1.0         .0       .0      .0  -999.9  9999.9     3.0    1.5      1.1
   4 1 1 1.0         .0       .0      .0  -999.9  9999.9     2.0    0.8      1.2
   5 1 1 1.0         .0       .0      .0  -999.9  9999.9     1.5    1.2      0.0
   6 1 1 1.0         .0       .0      .0  -999.9  9999.9     4.0    2.7      0.0
   7 1 1 1.0         .0       .0      .0  -999.9  9999.9     5.0    3.0      1.2
   8 1 1 1.0         .0       .0      .0  -999.9  9999.9     1.0    0.9      0.0
   9 1 1 1.0         .0       .0      .0  -999.9  9999.9     0.6    0.1      0.6
  10 1 1 1.0         .0       .0      .0  -999.9  9999.9     4.5    2.0      3.7
  11 1 1 1.0         .0       .0      .0  -999.9  9999.9     1.0    0.9      0.0
  12 1 1 1.0         .0       .0      .0  -999.9  9999.9     1.0    0.7      1.8
  13 1 1 1.0         .0       .0      .0  -999.9  9999.9     1.0    0.9      0.0
  14 1 1 1.0         .0       .0      .0  -999.9  9999.9     2.1    1.0      1.8
   1 3 1 1.0         .0       .0      .0  -999.9  9999.9     0.0    0.0      0.0
];
% Pot�ncias em MW e MVAr
% Colunas
% 1. No. da barra; 2. Tipo (3-SE; 1-Carga); 3. Grupo Lim. Tens�o; 4.Volt(pu)
% 5. �ngulo(graus); 6. Pg(MW); 7. Qg(MVAr); 8. Qgmin(MVAr);  9. Qgmax(MVAr)
% 10. PLOAD(MW); 11. QLOAD(MVAr); 12. Capacitor(MVAr)

% ------------------------------ DADOS DE LINHA -----------------------------------
 DLIN = [
 %FROM  TO    

num_arestas 16

   1    2    0.075   0.10      0.0      .000     .000     .000    .0     900     .0     7
   2    3    0.080   0.11      0.0      .000     .000     .000    .0     900     .0     7
   2    4    0.090   0.18      0.0      .000     .000     .000    .0     900     .0     7
   4    5    0.040   0.04      0.0      .000     .000     .000    .0     900     .0     7
   1    6    0.110   0.11      0.0      .000     .000     .000    .0     900     .0     7
   6    7    0.080   0.11      0.0      .000     .000     .000    .0     900     .0     7
   6    8    0.110   0.11      0.0      .000     .000     .000    .0     900     .0     7
   7    9    0.110   0.11      0.0      .000     .000     .000    .0     900     .0     7
   7    10   0.080   0.11      0.0      .000     .000     .000    .0     900     .0     7
   1    11   0.110   0.11      0.0      .000     .000     .000    .0     900     .0     7
   11   12   0.090   0.12      0.0      .000     .000     .000    .0     900     .0     7
   11   13   0.080   0.11      0.0      .000     .000     .000    .0     900     .0     7
   13   14   0.040   0.04      0.0      .000     .000     .000    .0     900     .0     7
    3    9    0.040   0.04      0.0      .000     .000     .000    .0     900     .0     7
    8    12   0.040   0.04      0.0      .000     .000     .000    .0     900     .0     7
    5    14   0.090   0.12      0.0      .000     .000     .000    .0     900     .0     7
                                                                                  ];
% Colunas
% 1. Barra De; 2. Barra Para; r(pu); x(pu)

DTEN = [
 1   .9  1.05
 ];

DGER = [
%  Bar  minP  maxP
 1   1    .0 500.0
                  ];

Vb = 1.0
PB = 100
Zb = 1.0